#include "stdafx.h"
#include "..\public\BlackHole.h"
#include "GameInstance.h"


CBlackHole::CBlackHole(_pDevice pGraphicDevice)
	: CEffect(pGraphicDevice)
{
}

CBlackHole::CBlackHole(const CBlackHole & rhs)
	: CEffect(rhs)
{
}

HRESULT CBlackHole::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CResurrection NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CBlackHole::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CResurrection NativeConstruct - __super");

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CResurrection NativeConstruct - SetUp_Component");

	OBJ_INFO* pObj = (OBJ_INFO*)pArg;

	m_pTransformCom->SetUp_Scale(pObj->Get_Scale());
	m_pTransformCom->SetUp_State(CTransform::STATE_POS, pObj->Get_State(OBJ_INFO::STATE_POS));

	Set_FrameValue(m_pTextureCom->Get_ImageCount() - 1);

	return S_OK;
}

_int CBlackHole::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
	{
		CGameInstance* pInstatnce = GET_INSTANCE(CGameInstance);
		OBJ_INFO tObj;
		tObj.matWorld = *m_pTransformCom->Get_WorldMatrixPtr();
		tObj.SetUp_Scale(1.f, 1.f, 1.f);

		tObj.dwColliderColor = 0xffffffff;
		tObj.vColliderCenter = _vec3(0.f, 0.f, 0.f);
		tObj.vColliderSize = _vec3(1.f, 1.f, 1.f);

		pInstatnce->Add_GameObjectToLayer(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeChimera"), pTagLayerMonster, &tObj);
		RELEASE_INSTANCE(CGameInstance);
		return OBJ_DEAD;
	}

	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CResurrection Tick - __super");

	Reduction(fTimeDelta);

	return _int();
}

_int CBlackHole::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CResurrection LateTick - __super");

	if (m_pRendererCom == nullptr)
		MSG_MINUSONE("CResurrection::LateTick - m_pRendererCom == nullptr");

	BillBoarding(m_pTransformCom);

	Compute_Distance(m_pTransformCom);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_NONALPHA, this)))
		MSG_MINUSONE("CResurrection LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CBlackHole::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CResurrection Render - __super");

	if (m_pRendererCom == nullptr)
		MSG_FAIL("CResurrection Render - m_pRendererCom is null");

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CResurrection Render - m_pVIBufferCom is null");

	if (m_pTextureCom == nullptr)
		MSG_FAIL("CResurrection Render - m_pTextureCom is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CResurrection Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureCom->Bind_OnDevice(_uint(Get_Frame()))))
		MSG_FAIL("CResurrection Render - m_pTextureCom Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CResurrection Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CResurrection Render - m_pVIBufferCom Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CResurrection Render - Release_RenderState");

	return S_OK;
}

HRESULT CBlackHole::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CResurrection Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CResurrection::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CResurrection::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CResurrection::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

HRESULT CBlackHole::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CResurrection Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CNecromencer::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

HRESULT CBlackHole::SetUp_Component()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransformCom)))
		MSG_FAIL("CResurrection SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CResurrection SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferRect"), TEXT("ComVIBuffer"), (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CResurrection SetUp_Component - Add_Component m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureBlackHole"), TEXT("ComTexture"), (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CResurrection SetUp_Component - Add_Component m_pTextureCom");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CBlackHole::Reduction(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	if (m_fDelay <= m_fTime)
	{
		m_fTime = 0.f;

		m_vSize = m_pTransformCom->Get_Scale();

		m_vSize *= 0.9f;

		if (0.2f >= m_vSize.x)
			m_bDead = true;

		m_pTransformCom->SetUp_Scale(m_vSize);
	}
}

void CBlackHole::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}

CGameObject * CBlackHole::Clone(void * pArg)
{
	CGameObject* pInstance = new CBlackHole(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CBlackHole Clone Failed");
	}

	return pInstance;
}

CBlackHole * CBlackHole::Create(_pDevice pGraphicDevice)
{
	CBlackHole* pInstance = new CBlackHole(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CBlackHole Create Failed");
	}

	return pInstance;
}
