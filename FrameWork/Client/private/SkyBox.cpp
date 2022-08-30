#include "stdafx.h"
#include "..\public\SkyBox.h"
#include "GameInstance.h"

void CSkyBox::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}

CGameObject * CSkyBox::Clone(void * pArg /*= nullptr*/)
{
	CGameObject* pInstance = new CSkyBox(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CSkyBox Clone Failed");
	}

	return pInstance;
}

CSkyBox * CSkyBox::Create(_pDevice pDevice)
{
	CSkyBox* pInstance = new CSkyBox(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CSkyBox Create Failed");
	}

	return pInstance;
}

_vec3 CSkyBox::Get_MinPos()
{
	return _vec3();
}

_vec3 CSkyBox::Get_MaxPos()
{
	return _vec3();
}

_vec3 CSkyBox::Get_WorldPos()
{
	return _vec3();
}

Engine::_vec3 CSkyBox::Get_Length()
{
	return Engine::_vec3();
}


CSkyBox::CSkyBox(_pDevice pDevice)
	:CGameObject(pDevice)
{
}

CSkyBox::CSkyBox(const CSkyBox & rhs)
	:CGameObject(rhs)
{
}

HRESULT CSkyBox::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CSkyBox NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CSkyBox::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CSkyBox NativeConstruct - __super");

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CSkyBox NativeConstruct - SetUp_Component");

	m_pTransformCom->SetUp_Scale(1.f, 1.f, 1.f);

	if (pArg != nullptr)
		m_tDesc = *(tagSkyBoxDesc*)pArg;

	return S_OK;
}

_int CSkyBox::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CSkyBox Tick - __super");

	return _int();
}

_int CSkyBox::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CSkyBox LateTick - __super");

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_PRIORITY, this)))
		MSG_MINUSONE("CSkyBox LateTick - Add_RenderGroup");

	_mat matView;
	if (FAILED(m_pGraphicDevice->GetTransform(D3DTRANSFORMSTATETYPE::D3DTS_VIEW, &matView)))
		MSG_FAIL("CSkyBox Tick - GetTransform");

	D3DXMatrixInverse(&matView, nullptr, &matView);
	m_pTransformCom->SetUp_State(CTransform::STATE_POS, *(_vec3*)(&matView.m[3][0]));

	return _int();
}

HRESULT CSkyBox::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CSkyBox Render - __super");

	if (m_pRendererCom == nullptr)
		MSG_FAIL("CSkyBox Render - m_pRendererCom is null");

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CSkyBox Render - m_pVIBufferCom is null");

	if (m_pTextureCom == nullptr)
		MSG_FAIL("CSkyBox Render - m_pTextureCom is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CSkyBox Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureCom->Bind_OnDevice(m_tDesc.iNumTexture)))
		MSG_FAIL("CSkyBox Render - m_pTextureCom Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CSkyBox Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CSkyBox Render - m_pVIBufferCom Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CSkyBox Render - Release_RenderState");

	return S_OK;
}

HRESULT CSkyBox::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CSkyBox Set_RenderState - m_pGraphicDevice is null");

	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE)))
		MSG_FAIL("CSkyBox Set_RenderState - D3DRS_ZWRITEENABLE is FALSE");


	return S_OK;
}

HRESULT CSkyBox::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CSkyBox Set_RenderState - m_pGraphicDevice is null");

	m_pGraphicDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	return S_OK;
}

HRESULT CSkyBox::SetUp_Component()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransformCom)))
		MSG_FAIL("CSkyBox SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CSkyBox SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferCube"), TEXT("ComVIBufferCube"), (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CSkyBox SetUp_Component - Add_Component m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureSkyBox"), TEXT("ComTextureSkyBox"), (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CSkyBox SetUp_Component - Add_Component m_pTextureCom");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}
