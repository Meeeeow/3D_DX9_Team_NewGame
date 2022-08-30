#include "stdafx.h"
#include "..\public\NecromencerIce.h"
#include "Player.h"
#include "Attack.h"
#include "Monster.h"
#include "ResurrectionManager.h"


CNecromencerIce::CNecromencerIce(_pDevice pGraphicDevice)
	: CGameObject(pGraphicDevice)
{
}

CNecromencerIce::CNecromencerIce(const CNecromencerIce & rhs)
	: CGameObject(rhs)
{
}

HRESULT CNecromencerIce::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CNecromencerIce NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CNecromencerIce::NativeConstruct(void * pArg)
{
	if (pArg == nullptr)
		MSG_FAIL("CNecromencerIce::NativeConstruct - pArg == nullptr");

	DESC* pDesc = (DESC*)pArg;

	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CNecromencerIce NativeConstruct - __super::NativeConstruct");

	if (FAILED(SetUp_Component(pDesc->pObj)))
		MSG_FAIL("CNecromencerIce NativeConstruct - SetUp_Component");

	return S_OK;
}

_int CNecromencerIce::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CNecromencerIce Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	Move(fTimeDelta);

	return 0;
}

_int CNecromencerIce::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CNecromencerIce LateTick - __super");

	BillBoarding(m_pTransformCom);

	if (m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this))
		MSG_MINUSONE("CNecromencerIce::LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CNecromencerIce::Render()
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

	if (FAILED(m_pTextureCom->Bind_OnDevice()))
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

HRESULT CNecromencerIce::SetUp_Component(void * pArg)
{
	OBJ_INFO tObjInfo = *(OBJ_INFO*)(pArg);

	tObjInfo.dwColliderColor = 0xffffffff;
	tObjInfo.vColliderCenter = _vec3(0.f, 0.f, 0.f);
	tObjInfo.vColliderSize = _vec3(1.f, 1.f, 1.f);

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

	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureIce"), TEXT("ComTextureIce"), (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CNecromencerIce::SetUp_Component - ComTextureIce");

	m_pBoxColliderCom = CBoxCollider::Create_Tool(m_pGraphicDevice, tObjInfo.vColliderCenter, tObjInfo.vColliderSize, tObjInfo.dwColliderColor, this);

	return S_OK;
}

HRESULT CNecromencerIce::Set_RenderState()
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

HRESULT CNecromencerIce::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CNecromencerIce::Release_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CNecromencerIce::Release_RenderState - m_pGraphicDevice->SetRenderState");

	return S_OK;
}

HRESULT CNecromencerIce::Move(_float fTimeDelta)
{
	_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
	_float fDist = fTimeDelta * 10.f;
	vPos.y -= fDist;
	m_pTransformCom->SetUp_State(CTransform::STATE_POS, vPos);

	m_fMaxDist += fDist;

	if (10.f <= m_fMaxDist)
		m_bDead = true;

	return S_OK;
}

HRESULT CNecromencerIce::BillBoarding(CTransform * pTransform)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_mat matCamWorld = pGameInstance->Get_CamWorld();
	_vec3 vEffectScale = pTransform->Get_Scale();

	pTransform->SetUp_State(CTransform::STATE_RIGHT, (*(_vec3*)(&matCamWorld.m[0][0])) * vEffectScale.x);
	pTransform->SetUp_State(CTransform::STATE_LOOK, (*(_vec3*)(&matCamWorld.m[2][0])) * vEffectScale.z);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CNecromencerIce::Tick_Collider(_float fTimeDelta)
{
	m_pBoxColliderCom->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxColliderCom->Tick(fTimeDelta);

	return S_OK;
}

void CNecromencerIce::Free()
{
	__super::Free();

	Safe_Release(m_pBoxColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}

CGameObject * CNecromencerIce::Clone(void * pArg)
{
	CNecromencerIce* pInstance = new CNecromencerIce(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CNecromencerIce Clone Failed");
	}

	return pInstance;
}

CNecromencerIce * CNecromencerIce::Create(_pDevice pGraphicDevice)
{
	CNecromencerIce* pInstance = new CNecromencerIce(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CNecromencerIce Create Failed");
	}

	return pInstance;
}

void CNecromencerIce::Collision(CGameObject * pColObj)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pColObj);

	if (pPlayer != nullptr)
	{
		CAttack::DESC tDesc;
		tDesc.iDmg = 10;
		tDesc.vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
		tDesc.vScale = m_pTransformCom->Get_Scale();
		tDesc.bSingleAttack = true;
		pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeAttack, pTagLayerMonsterAttack, &tDesc);

		Set_Dead();
	}

	RELEASE_INSTANCE(CGameInstance);
}

_vec3 CNecromencerIce::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxColliderCom->Get_MinPos();
	return vMinPos;
}

_vec3 CNecromencerIce::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxColliderCom->Get_MaxPos();
	return vMaxPos;
}

_vec3 CNecromencerIce::Get_WorldPos()
{
	return m_pBoxColliderCom->Get_ColliderWolrdPos();
}

Engine::_vec3 CNecromencerIce::Get_Length()
{
	return m_pTransformCom->Get_Scale();
}