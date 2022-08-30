#include "stdafx.h"
#include "..\public\NonControl.h"
#include "GameInstance.h"
#include "VIBufferRectColor.h"
#include "PlayerTagMgr.h"
#include "Player.h"
#include "NonControlObserver.h"

void CNonControl::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}

CGameObject * CNonControl::Clone(void * pArg /*= nullptr*/)
{
	CGameObject* pInstance = new CNonControl(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CNonControl Clone Failed");
	}

	return pInstance;
}

CNonControl * CNonControl::Create(_pDevice pDevice)
{
	CNonControl* pInstance = new CNonControl(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CNonControl Create Failed");
	}

	return pInstance;
}

_vec3 CNonControl::Get_MinPos()
{
	return _vec3();
}

_vec3 CNonControl::Get_MaxPos()
{
	return _vec3();
}

_vec3 CNonControl::Get_WorldPos()
{
	return _vec3();
}

_vec3 CNonControl::Get_Length()
{
	return _vec3();
}

CNonControl::CNonControl(_pDevice pDevice)
	:CEffect(pDevice)
{
}

CNonControl::CNonControl(const CNonControl & rhs)
	: CEffect(rhs)
{
}

HRESULT CNonControl::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CNonControl NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CNonControl::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CNonControl NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CNonControl NativeConstruct - SetUp_Component");

	DESC tDesc = *(DESC*)(pArg);
	D3DXMatrixOrthoLH(&m_matProject, (float)g_iWinCX, (float)g_iWinCY, 0.f, 1.f);
	_float fSizeX = tDesc.vScale.x;
	_float fSizeY = tDesc.vScale.y;
	_float fX = tDesc.vPos.x;
	_float fY = tDesc.vPos.y;

	m_pTransformCom->SetUp_State(CTransform::STATE::STATE_POS, _vec3(fX - ((float)g_iWinCX * 0.5f), fY - ((float)g_iWinCY * 0.5f), 0.f));
	m_pTransformCom->SetUp_Scale(fSizeX, fSizeY, 1.f);

	m_eType = tDesc.eType;
	if (m_eType == CPlayer::TYPE_WARRIOR)
		m_bRender = false;

	//옵저버 생성, 등록
	CObserver* pObserver = nullptr;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeObserverNonControl
		, pTagLayerObserver, this, (void**)&pObserver)))
		MSG_FAIL("CNonControl NativeConstruct - pTagPrototypeObserverNonControl");

	CPlayerTagMgr::Get_Instance()->Subscribe_ChangePlayer(pObserver);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CNonControl::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CNonControl Tick - __super");

	return _int();
}

_int CNonControl::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CNonControl LateTick - __super");

	if (m_pRendererCom == nullptr)
		MSG_MINUSONE("CNonControl::LateTick - m_pRendererCom == nullptr");

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_UI_ALPHA, this)))
		MSG_MINUSONE("CNonControl LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CNonControl::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CNonControl Render - __super");

	if (!m_bRender)
		return S_OK;

	if (m_pRendererCom == nullptr)
		MSG_FAIL("CNonControl Render - m_pRendererCom is null");

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CNonControl Render - m_pVIBufferCom is null");

	if (m_pTextureCom == nullptr)
		MSG_FAIL("CNonControl Render - m_pTextureCom is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CNonControl Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureCom->Bind_OnDevice(4)))
		MSG_FAIL("CNonControl Render - m_pTextureCom Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CNonControl Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CNonControl Render - m_pVIBufferCom Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CNonControl Render - Release_RenderState");

	return S_OK;
}

void CNonControl::Change_Player(CPlayer::TYPE eType)
{
	if (m_eType == eType)
		m_bRender = false;
	else
		m_bRender = true;
}

HRESULT CNonControl::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CNonControl Set_RenderState - m_pGraphicDevice is null");

	_mat tmp;
	D3DXMatrixIdentity(&tmp);

	m_pGraphicDevice->SetTransform(D3DTS_VIEW, &tmp);
	m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProject);

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
		MSG_FAIL("CNonControl::Set_RenderState - SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)))
		MSG_FAIL("CNonControl::Set_RenderState - SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)))
		MSG_FAIL("CNonControl::Set_RenderState - SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)");

	return S_OK;
}

HRESULT CNonControl::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CNonControl Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
		MSG_FAIL("CNonControl::Set_RenderState - SetRenderState");

	return S_OK;
}

HRESULT CNonControl::SetUp_Component(void* pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//트랜스폼 세팅하고 넘겨받은 좌표, 스케일 적용
	CTransform::tagTransformDesc tTransformDesc;
	tTransformDesc.fSpeed = 1.f;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransformCom, &tTransformDesc)))
		MSG_FAIL("CNonControl SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CNonControl SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTexturePlayerInfo, pTagComTexture, (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CNonControl SetUp_Component - Add_Component m_pTextureCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CNonControl SetUp_Component - Add_Component m_pVIBufferCom");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}