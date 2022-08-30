#include "stdafx.h"
#include "..\public\Blink.h"
#include "GameInstance.h"


CBlink::CBlink(_pDevice pGraphicDevice)
	: CEffect(pGraphicDevice)
{
}

CBlink::CBlink(const CBlink & rhs)
	: CEffect(rhs)
{
}

HRESULT CBlink::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBlink NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CBlink::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CBlink NativeConstruct - __super");

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CBlink NativeConstruct - SetUp_Component");

	DESC* pDesc = (DESC*)pArg;

	m_pTransformCom->SetUp_Scale(pDesc->vScale);
	m_pTransformCom->SetUp_State(CTransform::STATE_POS, pDesc->vPos);

	Set_FrameValue(m_pTextureCom->Get_ImageCount() - 1);

	return S_OK;
}

_int CBlink::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return OBJ_DEAD;

	if (0 > (__super::Tick(fTimeDelta, true)))
		MSG_MINUSONE("CBlink Tick - __super");

	return _int();
}

_int CBlink::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CBlink LateTick - __super");

	if (m_pRendererCom == nullptr)
		MSG_MINUSONE("CBlink::LateTick - m_pRendererCom == nullptr");

	BillBoarding(m_pTransformCom);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_NONALPHA, this)))
		MSG_MINUSONE("CBlink LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CBlink::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CBlink Render - __super");

	if (m_pRendererCom == nullptr)
		MSG_FAIL("CBlink Render - m_pRendererCom is null");

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CBlink Render - m_pVIBufferCom is null");

	if (m_pTextureCom == nullptr)
		MSG_FAIL("CBlink Render - m_pTextureCom is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CBlink Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureCom->Bind_OnDevice(_uint(Get_Frame()))))
		MSG_FAIL("CBlink Render - m_pTextureCom Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CBlink Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CBlink Render - m_pVIBufferCom Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CBlink Render - Release_RenderState");

	return S_OK;
}

HRESULT CBlink::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBlink Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CBlink::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CBlink::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CBlink::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

HRESULT CBlink::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBlink Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CNecromencer::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

HRESULT CBlink::SetUp_Component()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransformCom)))
		MSG_FAIL("CBlink SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CBlink SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferRect"), TEXT("ComVIBuffer"), (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CBlink SetUp_Component - Add_Component m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureBlink"), TEXT("ComTexture"), (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CBlink SetUp_Component - Add_Component m_pTextureCom");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CBlink::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}

CGameObject * CBlink::Clone(void * pArg)
{
	CGameObject* pInstance = new CBlink(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CBlink Clone Failed");
	}

	return pInstance;
}

CBlink * CBlink::Create(_pDevice pGraphicDevice)
{
	CBlink* pInstance = new CBlink(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CBlink Create Failed");
	}

	return pInstance;
}
