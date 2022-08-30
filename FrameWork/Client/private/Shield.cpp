#include "stdafx.h"
#include "..\public\Shield.h"
#include "TombStoneManager.h"


CShield::CShield(_pDevice pGraphicDevice)
	: CEffect(pGraphicDevice)
{
}

CShield::CShield(const CShield & rhs)
	: CEffect(rhs)
{
}

HRESULT CShield::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CShield NativeConstruct_Prototype - __super::NativeConstruct_Prototype");

	return S_OK;
}

HRESULT CShield::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CShield NativeConstruct - __super::NativeConstruct");

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CShield NativeConstruct - SetUp_Component");

	OBJ_INFO* pObj = (OBJ_INFO*)pArg;

	m_pTransformCom->SetUp_Scale(pObj->Get_Scale());
	m_pTransformCom->SetUp_State(CTransform::STATE_POS, pObj->Get_State(OBJ_INFO::STATE_POS));

	CTombStoneManager::Get_Instance()->Add_Shield(this);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeShieldCube"), pTagLayerTileCube, pObj, (void**)&m_pShieldCube)))
		MSG_FAIL("CShield NativeConstruct - Add_GameObjectToLayer PrototypeShieldCube");

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_int CShield::Tick(_float fTimeDelta)
{
	if (m_bDead)
	{
		CTombStoneManager::Get_Instance()->Sub_Shield();
		return OBJ_DEAD;
	}

	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CShield Tick - __super");

	return _int();
}

_int CShield::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CShield LateTick - __super");

	if (m_pRendererCom == nullptr)
		MSG_MINUSONE("CShield::LateTick - m_pRendererCom == nullptr");

	Compute_Distance(m_pTransformCom);

	if (m_pFrustumCom->Check_Culling(m_pTransformCom, 7.f))
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_ALPHA, this)))
			MSG_MINUSONE("CShield LateTick - Add_RenderGroup");
	}

	return _int();
}

HRESULT CShield::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CShield Render - __super");

	if (m_pRendererCom == nullptr)
		MSG_FAIL("CShield Render - m_pRendererCom is null");

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CShield Render - m_pVIBufferCom is null");

	if (m_pTextureCom == nullptr)
		MSG_FAIL("CShield Render - m_pTextureCom is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CShield Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureCom->Bind_OnDevice()))
		MSG_FAIL("CShield Render - m_pTextureCom Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CShield Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CShield Render - m_pVIBufferCom Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CShield Render - Release_RenderState");

	return S_OK;
}

HRESULT CShield::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CShield Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
		MSG_FAIL("CShield::Set_RenderState - SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)))
		MSG_FAIL("CShield::Set_RenderState - SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)))
		MSG_FAIL("CShield::Set_RenderState - SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)");

	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	return S_OK;
}

HRESULT CShield::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CShield Set_RenderState - m_pGraphicDevice is null");

	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
		MSG_FAIL("CShield::Set_RenderState - SetRenderState");

	return S_OK;
}

HRESULT CShield::SetUp_Component()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransformCom)))
		MSG_FAIL("CShield SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CShield SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferCube"), TEXT("ComVIBuffer"), (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CShield SetUp_Component - Add_Component m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureShield"), TEXT("ComTexture"), (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CShield SetUp_Component - Add_Component m_pTextureCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeFrustum"), TEXT("ComFrustum"), (CComponent**)&m_pFrustumCom)))
		MSG_FAIL("CShield SetUp_Component - Add_Component m_pFrustumCom");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CShield::Broken()
{
	m_pShieldCube->Set_Dead();

	if(nullptr != m_pMaster)
		m_pMaster->Broken_Shield();

	Set_Dead();
}

CShield * CShield::Create(_pDevice pGraphicDevice)
{
	CShield* pInstance = new CShield(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CShield Create - NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CShield::Clone(void * pArg)
{
	CShield* pInstance = new CShield(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CShield Create - NativeConstruct");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShield::Free()
{
	__super::Free();

	m_pMaster = nullptr;

	Safe_Release(m_pFrustumCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}
