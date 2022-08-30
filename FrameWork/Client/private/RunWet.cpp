#include "stdafx.h"
#include "..\public\RunWet.h"
#include "Player.h"
#include "PlayerTagMgr.h"
#include "Attack.h"
#include "DmgFont.h"
#include "RunWetParticle.h"

CRunWet::CRunWet(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{
}

CRunWet::CRunWet(const CRunWet & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRunWet::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CRunWet NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CRunWet::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CRunWet NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CRunWet NativeConstruct - SetUp_Component");

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			int iRandDegree = rand() % 30;
			CRunWetParticle::DESC tDesc;
			tDesc.tObjInfo.matWorld = *m_pTransform->Get_WorldMatrixPtr();
			lstrcpy(tDesc.tObjInfo.szTextureName, pTagPrototypeTextureBossSkeletonStageCube);
			tDesc.tObjInfo.iTextureIndex = 3;
			tDesc.tObjInfo.SetUp_Scale(0.1f, 0.1f, 0.1f);
			tDesc.tObjInfo.Rotation_Axis(_vec3(0.f, 1.f, 0.f), _float(i * 30 + iRandDegree));
			tDesc.fPower = 5.f;
			tDesc.fPowerY = _float(rand() % 5);
			//tDesc.fDegree = ;
			
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeRunWetParticle, pTagLayerNonCollision,
				&tDesc)))
				MSG_FAIL("CRunWet::NativeConstruct - pTagPrototypeRunWetParticle");

		}
	}
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CRunWet::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CRunWet Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

HRESULT CRunWet::SetUp_Component(void *pArg)
{
	DESC tDesc = *(DESC*)(pArg);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform)))
		MSG_FAIL("CRunWet::SetUp_Component - m_pTransform");
	m_pTransform->Set_Matrix(tDesc.matWorld);

	return S_OK;
}

CRunWet * CRunWet::Create(_pDevice pDevice)
{
	CRunWet* pInstance = new CRunWet(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CRunWet Create Failed");
	}

	return pInstance;
}

void CRunWet::Free()
{
	Safe_Release(m_pTransform);

	__super::Free();
}

CGameObject * CRunWet::Clone(void * pArg)
{
	CGameObject* pInstance = new CRunWet(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CRunWet Clone Failed");
	}

	return pInstance;
}

_vec3 CRunWet::Get_MinPos()
{
	return _vec3();
}

_vec3 CRunWet::Get_MaxPos()
{
	return _vec3();
}

_vec3 CRunWet::Get_WorldPos()
{
	return _vec3();
}

Engine::_vec3 CRunWet::Get_Length()
{
	return m_pTransform->Get_Scale();
}
