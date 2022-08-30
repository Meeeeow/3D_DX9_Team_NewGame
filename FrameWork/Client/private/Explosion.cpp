#include "stdafx.h"
#include "..\public\Explosion.h"
#include "GameInstance.h"

void CExplosion::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}

CGameObject * CExplosion::Clone(void * pArg /*= nullptr*/)
{
	CGameObject* pInstance = new CExplosion(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CExplosion Clone Failed");
	}

	return pInstance;
}

CExplosion * CExplosion::Create(_pDevice pDevice)
{
	CExplosion* pInstance = new CExplosion(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CExplosion Create Failed");
	}

	return pInstance;
}

CExplosion::CExplosion(_pDevice pDevice)
	:CEffect(pDevice)
{
}

CExplosion::CExplosion(const CExplosion & rhs)
	: CEffect(rhs)
{
}

HRESULT CExplosion::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CExplosion NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CExplosion::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CExplosion NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CExplosion NativeConstruct - SetUp_Component");


	Set_FrameValue(90);

	return S_OK;
}

_int CExplosion::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CExplosion Tick - __super");

	if (Get_Frame() >= 89)
	{
		m_bDead = true;
		return OBJ_DEAD;
	}

	return _int();
}

_int CExplosion::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CExplosion LateTick - __super");

	if (m_pRendererCom == nullptr)
		MSG_MINUSONE("CExplosion::LateTick - m_pRendererCom == nullptr");

	BillBoarding(m_pTransformCom);

	Compute_Distance(m_pTransformCom);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_ALPHA, this)))
		MSG_MINUSONE("CExplosion LateTick - Add_RenderGroup");

	

	return _int();
}

HRESULT CExplosion::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CExplosion Render - __super");

	if (m_pRendererCom == nullptr)
		MSG_FAIL("CExplosion Render - m_pRendererCom is null");

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CExplosion Render - m_pVIBufferCom is null");

	if (m_pTextureCom == nullptr)
		MSG_FAIL("CExplosion Render - m_pTextureCom is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CExplosion Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureCom->Bind_OnDevice(_uint(Get_Frame()))))
		MSG_FAIL("CExplosion Render - m_pTextureCom Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CExplosion Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CExplosion Render - m_pVIBufferCom Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CExplosion Render - Release_RenderState");

	return S_OK;
}

HRESULT CExplosion::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CExplosion Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
		MSG_FAIL("CExplosion::Set_RenderState - SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)))
		MSG_FAIL("CExplosion::Set_RenderState - SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)))
		MSG_FAIL("CExplosion::Set_RenderState - SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)");

	return S_OK;
}

HRESULT CExplosion::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CExplosion Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
		MSG_FAIL("CExplosion::Set_RenderState - SetRenderState");

	return S_OK;
}

HRESULT CExplosion::SetUp_Component(void* pArg)
{
	DESC tDesc = *(DESC*)pArg;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransformCom)))
		MSG_FAIL("CExplosion SetUp_Component - Add_Component m_pTransformCom");
	m_pTransformCom->SetUp_State(CTransform::STATE_POS, tDesc.vPos);
	m_pTransformCom->SetUp_Scale(tDesc.vSize);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CExplosion SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferRect"), TEXT("ComVIBuffer"), (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CExplosion SetUp_Component - Add_Component m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureExplosion"), TEXT("ComTexture"), (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CExplosion SetUp_Component - Add_Component m_pTextureCom");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}
