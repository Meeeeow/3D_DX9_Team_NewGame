#include "stdafx.h"
#include "..\public\NecromencerElectric.h"
#include "Player.h"
#include "Attack.h"
#include "Monster.h"


CNecromencerElectric::CNecromencerElectric(_pDevice pGraphicDevice)
	: CGameObject(pGraphicDevice)
{
}

CNecromencerElectric::CNecromencerElectric(const CNecromencerElectric & rhs)
	: CGameObject(rhs)
{
}

HRESULT CNecromencerElectric::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CNecromencerIce NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CNecromencerElectric::NativeConstruct(void * pArg)
{
	if (pArg == nullptr)
		MSG_FAIL("CNecromencerIce::NativeConstruct - pArg == nullptr");

	OBJ_INFO* pObj = (OBJ_INFO*)pArg;

	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CNecromencerIce NativeConstruct - __super::NativeConstruct");

	if (FAILED(SetUp_Component(pObj)))
		MSG_FAIL("CNecromencerIce NativeConstruct - SetUp_Component");

	return S_OK;
}

_int CNecromencerElectric::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CNecromencerIce Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	Update_Frame(fTimeDelta);

	return 0;
}

_int CNecromencerElectric::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CNecromencerIce LateTick - __super");

	BillBoarding(m_pTransformCom);

	if (m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this))
		MSG_MINUSONE("CNecromencerIce::LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CNecromencerElectric::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CNecromencerIce Render - __super");

	if (m_pTransformCom == nullptr ||
		m_pTextureCom == nullptr ||
		m_pBoxColliderCom == nullptr ||
		m_pVIBufferCom == nullptr)
		MSG_FAIL("CNecromencerIce::Render - Com is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CNecromencerIce::Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureCom->Bind_OnDevice(m_tFrame.iStart)))
		MSG_FAIL("CNecromencerIce::Render - m_pTextureComArr->Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CNecromencerIce::Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CNecromencerIce::Render - m_pVIBufferCom->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CNecromencerIce::Render - Release_RenderState");

	if (FAILED(m_pBoxColliderCom->Render()))
		MSG_FAIL("CNecromencerIce::Render - m_pBoxColliderCom->Render");

	return S_OK;
}

HRESULT CNecromencerElectric::SetUp_Component(void * pArg)
{
	OBJ_INFO tObjInfo = *(OBJ_INFO*)(pArg);

	tObjInfo.dwColliderColor = 0xffffffff;
	tObjInfo.vColliderCenter = _vec3(0.f, 0.f, 0.f);
	tObjInfo.vColliderSize = _vec3(0.3f, 1.f, 0.3f);

	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.fSpeed = 10.f;
	tTransformDesc.fDegree = 3.f;

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransformCom, &tTransformDesc)))
		MSG_FAIL("CNecromencerIce::SetUp_Component - m_pTransformCom");
	m_pTransformCom->Set_Matrix(tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CNecromencerIce::SetUp_Component - m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CNecromencerIce::SetUp_Component - m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureElectric"), TEXT("ComTextureElectric"), (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CNecromencerIce::SetUp_Component - ComTextureElectric");
	m_tFrame.Set_Value(m_pTextureCom->Get_ImageCount() - 1);

	m_pBoxColliderCom = CBoxCollider::Create_Tool(m_pGraphicDevice, tObjInfo.vColliderCenter, tObjInfo.vColliderSize, tObjInfo.dwColliderColor, this);

	return S_OK;
}

HRESULT CNecromencerElectric::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CNecromencerIce::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CNecromencerIce::Set_RenderState - m_pGraphicDevice->SetRenderState");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CNecromencerIce::Set_RenderState - m_pGraphicDevice->SetRenderState");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CNecromencerIce::Set_RenderState - m_pGraphicDevice->SetRenderState");

	return S_OK;
}

HRESULT CNecromencerElectric::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CNecromencerIce::Release_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CNecromencerIce::Release_RenderState - m_pGraphicDevice->SetRenderState");

	return S_OK;
}

void CNecromencerElectric::Update_Frame(_float fTimeDelta)
{
	m_tFrame.fTime += fTimeDelta;

	if (m_tFrame.fDelay <= m_tFrame.fTime)
	{
		++m_tFrame.iStart;

		m_tFrame.fTime = 0.f;

		if (m_tFrame.iEnd <= m_tFrame.iStart)
		{
			m_bDead = true;
		}
	}
}

HRESULT CNecromencerElectric::BillBoarding(CTransform * pTransform)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_mat matCamWorld = pGameInstance->Get_CamWorld();
	_vec3 vEffectScale = pTransform->Get_Scale();

	pTransform->SetUp_State(CTransform::STATE_RIGHT, (*(_vec3*)(&matCamWorld.m[0][0])) * vEffectScale.x);
	pTransform->SetUp_State(CTransform::STATE_LOOK, (*(_vec3*)(&matCamWorld.m[2][0])) * vEffectScale.z);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CNecromencerElectric::Tick_Collider(_float fTimeDelta)
{
	m_pBoxColliderCom->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxColliderCom->Tick(fTimeDelta);

	return S_OK;
}

void CNecromencerElectric::Free()
{
	__super::Free();

	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}

CGameObject * CNecromencerElectric::Clone(void * pArg)
{
	CNecromencerElectric* pInstance = new CNecromencerElectric(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CNecromencerIce Clone Failed");
	}

	return pInstance;
}

CNecromencerElectric * CNecromencerElectric::Create(_pDevice pGraphicDevice)
{
	CNecromencerElectric* pInstance = new CNecromencerElectric(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CNecromencerIce Create Failed");
	}

	return pInstance;
}

void CNecromencerElectric::Collision(CGameObject * pColObj)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pColObj);

	if (pPlayer != nullptr)
	{
		CAttack::DESC tDesc;
		tDesc.iDmg = 30;
		tDesc.vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
		tDesc.vScale = m_pTransformCom->Get_Scale();
		tDesc.bSingleAttack = true;
		pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeAttack, pTagLayerMonsterAttack, &tDesc);

		Set_Dead();
	}

	RELEASE_INSTANCE(CGameInstance);
}

_vec3 CNecromencerElectric::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxColliderCom->Get_MinPos();
	return vMinPos;
}

_vec3 CNecromencerElectric::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxColliderCom->Get_MaxPos();
	return vMaxPos;
}

_vec3 CNecromencerElectric::Get_WorldPos()
{
	return m_pBoxColliderCom->Get_ColliderWolrdPos();
}

Engine::_vec3 CNecromencerElectric::Get_Length()
{
	return m_pTransformCom->Get_Scale();
}