#include "stdafx.h"
#include "..\public\Environment.h"
#include "GameInstance.h"
#include "Terrain.h"
#include "InputDevice.h"
#include "PlayerTagMgr.h"
#include "..\public\Rain.h"
#include "RainDrop.h"
#include "EnvironmentMgr.h"


CRain::CRain(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CEnvironment(pGraphicDevice)
{
}

CRain::CRain(const CRain & rhs)
	: CEnvironment(rhs)
{
}

HRESULT CRain::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CRain NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CRain::NativeConstruct(void * pArg)
{
	m_tRainDropDesc = *(DESC*)(pArg);
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CRain NativeConstruct - __super");

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CRain::NativeConstruct - SetUp_Component");

	m_tTimeRainClone.fDelay = 0.02f;

	CEnvironmentMgr::Get_Instance()->Add_Env(ENV_RAIN);

	CSoundMgr::Get_Instance()->PlayLoopSound(L"Rain.MP3", CSoundMgr::RAIN);

	return S_OK;
}

_int CRain::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CRain Tick - __super");

	if (m_bDead)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::RAIN);
		return OBJ_DEAD;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_mat matCamWorld = pGameInstance->Get_CamWorld();
	m_pTransform->Set_Matrix(matCamWorld);
	_vec3 vCamPos = m_pTransform->Get_State(CTransform::STATE_POS);
	vCamPos.y += 10.f;
	vCamPos.x -= 10.f;
	m_pTransform->SetUp_State(CTransform::STATE_POS, vCamPos);

	m_tTimeRainClone.Tick_Time(fTimeDelta);
	if (m_tTimeRainClone.Check_Time())
	{
		for (int i = 0; i < 12; ++i)
		{
			CRainDrop::DESC tDesc;
			tDesc.fDegree = m_tRainDropDesc.tRainDropDesc.fDegree;
			tDesc.pObjInfo = m_tRainDropDesc.tRainDropDesc.pObjInfo;
			_vec3 vPos = m_pTransform->Get_State(CTransform::STATE_POS);
			_int iRandX = rand() % 60 - 30;
			_int iRandZ = rand() % 60 - 30;
			vPos.x += (_float)iRandX;
			vPos.z += (_float)iRandZ;

			tDesc.pObjInfo->SetUp_State(OBJ_INFO::STATE_POS, vPos);
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pTagPrototypeRainDrop, pTagLayerRainDrop, &tDesc)))
				MSG_MINUSONE("CRain::Tick - pGameInstance->Add_GameObjectToLayer");
		}
	}

	RELEASE_INSTANCE(CGameInstance);
	return 0;
}

_int CRain::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CRain LateTick - __super");

	return _int();
}

HRESULT CRain::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CRain Render - __super");

	return S_OK;
}

HRESULT CRain::SetUp_Component()
{
	CTransform::TRANSFORM_DESC tDesc;
	tDesc.fSpeed = 100.f;
	if (CGameObject::Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform, &tDesc))
		MSG_FAIL("CRain::SetUp_Component - Add_Component pTagPrototypeTransform");

	return S_OK;
}

void CRain::Free()
{
	__super::Free();
	CEnvironmentMgr::Get_Instance()->Remove_Env(ENV_RAIN);

	Safe_Release(m_pTransform);
	Safe_Delete(m_tRainDropDesc.tRainDropDesc.pObjInfo);
}

CGameObject * CRain::Clone(void * pArg)
{
	CGameObject* pInstance = new CRain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CDmgFont Clone Failed");
	}

	return pInstance;
}

CRain * CRain::Create(_pDevice pDevice)
{
	CRain* pInstance = new CRain(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CDmgFont Create Failed");
	}

	return pInstance;
}

_vec3 CRain::Get_MinPos()
{
	return _vec3();
}

_vec3 CRain::Get_MaxPos()
{
	return _vec3();
}

_vec3 CRain::Get_WorldPos()
{
	return _vec3();
}

_vec3 CRain::Get_Length()
{
	return _vec3();
}
