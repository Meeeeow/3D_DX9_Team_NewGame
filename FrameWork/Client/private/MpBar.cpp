#include "stdafx.h"
#include "..\public\MpBar.h"
#include "GameInstance.h"
#include "Player.h"


CMpBar::CMpBar(_pDevice pGraphicDevice)
	: CObserver(pGraphicDevice)
{
}

CMpBar::CMpBar(const CMpBar & rhs)
	: CObserver(rhs)
{
}

HRESULT CMpBar::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("Failed To CMpBar NativeConstruct_Prototype - super");

	return S_OK;
}

HRESULT CMpBar::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("Failed To CMpBar NativeConstruct - super");

	// 저장 정보 불러옴
	OBJ_INFO* pInfo = (OBJ_INFO*)pArg;

	D3DXMatrixOrthoLH(&m_matProject, (float)g_iWinCX, (float)g_iWinCY, 0.f, 1.f);
	m_fSizeX = pInfo->Get_Scale().x;
	m_fSizeY = pInfo->Get_Scale().y;
	m_fX = pInfo->Get_State(OBJ_INFO::STATE_POS).x;
	m_fY = pInfo->Get_State(OBJ_INFO::STATE_POS).y;

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CBackGround NativeConstruct - SetUp_Component");

	m_pTransformCom->SetUp_State(CTransform::STATE::STATE_POS, _vec3(m_fX - ((float)g_iWinCX * 0.5f), m_fY - ((float)g_iWinCY * 0.5f), 0.f));
	m_pTransformCom->SetUp_Scale(m_fSizeX, m_fSizeY, 1.f);

	return S_OK;
}

_int CMpBar::Tick(_float fTimeDelta)
{
	return _int();
}

_int CMpBar::LateTick(_float fTimeDelta)
{
	if (true == m_bControl)
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_UI, this)))
			MSG_MINUSONE("CPlayer LateTick - Add_RenderGroup");
	}

	return _int();
}

HRESULT CMpBar::Render()
{

	if (FAILED(__super::Render()))
		MSG_FAIL("CBackGround Render - __super");

	if (m_pRendererCom == nullptr)
		MSG_FAIL("CBackGround Render - m_pRendererCom is null");

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CBackGround Render - m_pVIBufferCom is null");

	if (m_pTextureCom == nullptr)
		MSG_FAIL("CBackGround Render - m_pTextureCom is null");

	m_pTransformCom->Bind_OnDevice();

	_mat tmp;
	D3DXMatrixIdentity(&tmp);

	m_pGraphicDevice->SetTransform(D3DTS_VIEW, &tmp);
	m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProject);
	m_pTextureCom->Bind_OnDevice();

	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pVIBufferCom->Render();

	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CMpBar::Notify(void * pMessage)
{
	CPlayer::PLAYER_STAT * pArg = (CPlayer::PLAYER_STAT*)pMessage;

	//m_bControl = pArg->bControl;
	m_iMaxMp = pArg->iMaxMp;
	m_iMp = pArg->iMp;

	_float fSizeX = _float(m_iMp) / _float(m_iMaxMp) * m_fSizeX;

	m_pTransformCom->SetUp_Scale(fSizeX, m_fSizeY, 1.f);
}

HRESULT CMpBar::SetUp_Component()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransformCom)))
		MSG_FAIL("CMpBar SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CMpBar SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferRect"), TEXT("ComVIBufferRect"), (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CMpBar SetUp_Component - Add_Component m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureMpBar"), TEXT("ComTextureUI"), (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CMpBar SetUp_Component - Add_Component m_pTextureCom");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CMpBar * CMpBar::Create(_pDevice pGraphicDevice)
{
	CMpBar* pInstance = new CMpBar(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed To CMpBar Create - NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CMpBar::Clone(void * pArg)
{
	CMpBar* pInstance = new CMpBar(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed To CMpBar Create - NativeConstruct");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMpBar::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}

_vec3 CMpBar::Get_MinPos()
{
	return _vec3();
}

_vec3 CMpBar::Get_MaxPos()
{
	return _vec3();
}

_vec3 CMpBar::Get_WorldPos()
{
	return _vec3();
}

_vec3 CMpBar::Get_Length()
{
	return _vec3();
}
