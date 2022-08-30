#include "stdafx.h"
#include "..\public\BlackScreen.h"
#include "GameInstance.h"
#include "VIBufferRectColor.h"
#include "PlayerTagMgr.h"
#include "Player.h"
#include "NonControlObserver.h"

void CBlackScreen::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}

CGameObject * CBlackScreen::Clone(void * pArg /*= nullptr*/)
{
	CGameObject* pInstance = new CBlackScreen(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CBlackScreen Clone Failed");
	}

	return pInstance;
}

CBlackScreen * CBlackScreen::Create(_pDevice pDevice)
{
	CBlackScreen* pInstance = new CBlackScreen(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CBlackScreen Create Failed");
	}

	return pInstance;
}

_vec3 CBlackScreen::Get_MinPos()
{
	return _vec3();
}

_vec3 CBlackScreen::Get_MaxPos()
{
	return _vec3();
}

_vec3 CBlackScreen::Get_WorldPos()
{
	return _vec3();
}

_vec3 CBlackScreen::Get_Length()
{
	return _vec3();
}

CBlackScreen::CBlackScreen(_pDevice pDevice)
	:CEffect(pDevice)
{
}

CBlackScreen::CBlackScreen(const CBlackScreen & rhs)
	: CEffect(rhs)
{
}

HRESULT CBlackScreen::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBlackScreen NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CBlackScreen::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CBlackScreen NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CBlackScreen NativeConstruct - SetUp_Component");

	DESC tDesc = *(DESC*)(pArg);
	m_iTextureNum = tDesc.iTextureNum;

	D3DXMatrixOrthoLH(&m_matProject, (float)g_iWinCX, (float)g_iWinCY, 0.f, 1.f);
	_float fSizeX = _float(g_iWinCX);
	_float fSizeY = _float(g_iWinCY);
	_float fX = _float(g_iWinCX >> 1);
	_float fY = _float(g_iWinCY >> 1);
	
	m_pTransformCom->SetUp_State(CTransform::STATE::STATE_POS, _vec3(fX - ((float)g_iWinCX * 0.5f), fY - ((float)g_iWinCY * 0.5f), 0.f));
	m_pTransformCom->SetUp_Scale(fSizeX, fSizeY, 1.f);

	return S_OK;
}

_int CBlackScreen::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CBlackScreen Tick - __super");

	return _int();
}

_int CBlackScreen::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CBlackScreen LateTick - __super");

	if (m_pRendererCom == nullptr)
		MSG_MINUSONE("CBlackScreen::LateTick - m_pRendererCom == nullptr");

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_UI_ALPHA, this)))
		MSG_MINUSONE("CBlackScreen LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CBlackScreen::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CBlackScreen Render - __super");

	if (m_pRendererCom == nullptr)
		MSG_FAIL("CBlackScreen Render - m_pRendererCom is null");

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CBlackScreen Render - m_pVIBufferCom is null");

	if (m_pTextureCom == nullptr)
		MSG_FAIL("CBlackScreen Render - m_pTextureCom is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CBlackScreen Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureCom->Bind_OnDevice(m_iTextureNum)))
		MSG_FAIL("CBlackScreen Render - m_pTextureCom Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CBlackScreen Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CBlackScreen Render - m_pVIBufferCom Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CBlackScreen Render - Release_RenderState");

	return S_OK;
}

HRESULT CBlackScreen::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBlackScreen Set_RenderState - m_pGraphicDevice is null");

	_mat tmp;
	D3DXMatrixIdentity(&tmp);

	m_pGraphicDevice->SetTransform(D3DTS_VIEW, &tmp);
	m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProject);

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
		MSG_FAIL("CBlackScreen::Set_RenderState - SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)))
		MSG_FAIL("CBlackScreen::Set_RenderState - SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)))
		MSG_FAIL("CBlackScreen::Set_RenderState - SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)");

	return S_OK;
}

HRESULT CBlackScreen::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBlackScreen Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
		MSG_FAIL("CBlackScreen::Set_RenderState - SetRenderState");

	return S_OK;
}

HRESULT CBlackScreen::SetUp_Component(void* pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransformCom)))
		MSG_FAIL("CBlackScreen SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CBlackScreen SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTextureBlackAlpha, pTagComTexture, (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CBlackScreen SetUp_Component - Add_Component m_pTextureCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CBlackScreen SetUp_Component - Add_Component m_pVIBufferCom");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}