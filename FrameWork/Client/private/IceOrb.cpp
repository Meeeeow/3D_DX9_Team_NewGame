#include "stdafx.h"
#include "..\public\IceOrb.h"
#include "PlayerTagMgr.h"
#include "NecromencerIce.h"


CIceOrb::CIceOrb(_pDevice pGraphicDevice)
	: CGameObject(pGraphicDevice)
{
}

CIceOrb::CIceOrb(const CIceOrb & rhs)
	: CGameObject(rhs)
{
}

HRESULT CIceOrb::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CIceOrb NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CIceOrb::NativeConstruct(void * pArg)
{
	if (pArg == nullptr)
		MSG_FAIL("CIceOrb::NativeConstruct - pArg == nullptr");

	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CIceOrb NativeConstruct - __super::NativeConstruct");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CIceOrb NativeConstruct - SetUp_Component");

	return S_OK;
}

_int CIceOrb::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CIceOrb Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	Shot(fTimeDelta);

	return 0;
}

_int CIceOrb::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CIceOrb LateTick - __super");

	BillBoarding(m_pTransformCom);

	if (m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this))
		MSG_MINUSONE("CIceOrb::LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CIceOrb::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CIceOrb Render - __super");

	if (m_pTransformCom == nullptr ||
		m_pTextureCom == nullptr ||
		m_pVIBufferCom == nullptr)
		MSG_FAIL("CIceOrb::Render - Com is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CIceOrb::Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureCom->Bind_OnDevice()))
		MSG_FAIL("CIceOrb::Render - m_pTextureComArr->Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CIceOrb::Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CIceOrb::Render - m_pVIBufferCom->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CIceOrb::Render - Release_RenderState");

	return S_OK;
}

HRESULT CIceOrb::SetUp_Component(void * pArg)
{
	OBJ_INFO tObjInfo = *(OBJ_INFO*)(pArg);

	//tObjInfo.dwColliderColor = 0xffffffff;
	//tObjInfo.vColliderCenter = _vec3(0.f, 0.f, 0.f);
	//tObjInfo.vColliderSize = _vec3(1.f, 1.f, 1.f);

	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.fSpeed = 10.f;
	tTransformDesc.fDegree = 3.f;

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransformCom, &tTransformDesc)))
		MSG_FAIL("CIceOrb::SetUp_Component - m_pTransformCom");
	m_pTransformCom->Set_Matrix(tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CIceOrb::SetUp_Component - m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CIceOrb::SetUp_Component - m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureIceOrb"), TEXT("ComTextureIceOrb"), (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CIceOrb::SetUp_Component - ComTextureIceOrb");

	m_pPlayerTransformCom = CPlayerTagMgr::Get_Instance()->Get_CurPlayerTransform();
	if (m_pPlayerTransformCom == nullptr)
		MSG_FAIL("CIceOrb::AI - m_pTransformComPlayer == nullptr");

	return S_OK;
}

HRESULT CIceOrb::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CIceOrb::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CIceOrb::Set_RenderState - m_pGraphicDevice->SetRenderState");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CIceOrb::Set_RenderState - m_pGraphicDevice->SetRenderState");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CIceOrb::Set_RenderState - m_pGraphicDevice->SetRenderState");

	return S_OK;
}

HRESULT CIceOrb::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CIceOrb::Release_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CIceOrb::Release_RenderState - m_pGraphicDevice->SetRenderState");

	return S_OK;
}

HRESULT CIceOrb::BillBoarding(CTransform * pTransform)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_mat matCamWorld = pGameInstance->Get_CamWorld();
	_vec3 vEffectScale = pTransform->Get_Scale();

	pTransform->SetUp_State(CTransform::STATE_RIGHT, (*(_vec3*)(&matCamWorld.m[0][0])) * vEffectScale.x);
	pTransform->SetUp_State(CTransform::STATE_UP, (*(_vec3*)(&matCamWorld.m[1][0])) * vEffectScale.y);
	pTransform->SetUp_State(CTransform::STATE_LOOK, (*(_vec3*)(&matCamWorld.m[2][0])) * vEffectScale.z);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CIceOrb::Shot(_float fTimeDelta)
{
	m_fShotTime += fTimeDelta;

	if (m_fDelayTime <= m_fShotTime)
	{
		// น฿ป็
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		CNecromencerIce::DESC pDesc{};

		OBJ_INFO pObj{};
		pObj.SetUp_Scale(0.7f, 0.7f, 0.7f);
		pObj.SetUp_State(OBJ_INFO::STATE_POS, m_pTransformCom->Get_State(CTransform::STATE_POS));

		pDesc.vTargetPos = m_pPlayerTransformCom->Get_State(CTransform::STATE_POS);
		pDesc.pObj = &pObj;

		pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("PrototypeNecromencerIce"), pTagLayerMonsterAttack, &pDesc);

		RELEASE_INSTANCE(CGameInstance);

		m_fShotTime = 0.f;

		--m_iShotCount;

		if (0 >= m_iShotCount)
			m_bDead = true;
	}
}

void CIceOrb::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}

CGameObject * CIceOrb::Clone(void * pArg)
{
	CIceOrb* pInstance = new CIceOrb(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CIceOrb MSG_FAIL Failed");
	}

	return pInstance;
}

CIceOrb * CIceOrb::Create(_pDevice pGraphicDevice)
{
	CIceOrb* pInstance = new CIceOrb(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CIceOrb Create Failed");
	}

	return pInstance;
}

_vec3 CIceOrb::Get_MinPos()
{
	return _vec3();
}

_vec3 CIceOrb::Get_MaxPos()
{
	return _vec3();
}

_vec3 CIceOrb::Get_WorldPos()
{
	return _vec3();
}

_vec3 CIceOrb::Get_Length()
{
	return _vec3();
}
