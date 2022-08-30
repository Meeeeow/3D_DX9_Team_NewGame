#include "stdafx.h"
#include "..\public\CardSelect.h"



CCardSelect::CCardSelect(_pDevice pGraphicDevice)
	: CGameObject(pGraphicDevice)
{
}

CCardSelect::CCardSelect(const CCardSelect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCardSelect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CCardSelect::NativeConstruct_Prototype - __super");

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CCardSelect::NativeConstruct_Prototype - SetUp_Component");


	return S_OK;
}

HRESULT CCardSelect::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CCardSelect::NativeConstruct - __super");

	return S_OK;
}

_int CCardSelect::Tick(_float fTimeDelta)
{
	return OBJ_NOEVENT;
}

_int CCardSelect::LateTick(_float fTimeDelta)
{

	m_pRenderer->Add_RenderGroup(CRenderer::GROUP_UI,this);
	return _int();
}

HRESULT CCardSelect::Render()
{
	m_pTransform->Bind_OnDevice();
	m_pTexture->Bind_OnDevice();


	_mat tmp;
	D3DXMatrixIdentity(&tmp);

	m_pGraphicDevice->SetTransform(D3DTS_VIEW, &tmp);
	m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProject);

	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pVIBuffer->Render();

	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CCardSelect::SetUp_Position(_uint iCardCategory, _uint iCardNumber)
{
	_int iMoveDistanceY = 192 + (iCardCategory * 48);
	_int iMoveDistanceX = 240 + (iCardNumber * 32);

	m_pTransform->SetUp_State(CTransform::STATE::STATE_POS, _vec3((FLOAT)iMoveDistanceX, (FLOAT)-iMoveDistanceY, 0.f));
}

HRESULT CCardSelect::SetUp_Component()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_BOSS_CIRCUS, L"PrototypeTextureCardSelect", pTagComTexture, (CComponent**)&m_pTexture)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pTexture");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pRendererCom");

	D3DXMatrixOrthoLH(&m_matProject, (float)g_iWinCX, (float)g_iWinCY, 0.f, 1.f);
	m_fSizeX = 32;
	m_fSizeY = 48;
	m_fX = (g_iWinCX/2.f) - (416/2.f);
	m_fY = (g_iWinCY/2.f) - (192/2.f);

	m_pTransform->SetUp_State(CTransform::STATE::STATE_POS, _vec3(240, -192, 0.f));
	m_pTransform->SetUp_Scale(m_fSizeX, m_fSizeY, 1.f);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_vec3 CCardSelect::Get_MinPos()
{
	return _vec3();
}

_vec3 CCardSelect::Get_MaxPos()
{
	return _vec3();
}

_vec3 CCardSelect::Get_WorldPos()
{
	return _vec3();
}

_vec3 CCardSelect::Get_Length()
{
	return _vec3();
}

CGameObject * CCardSelect::Clone(void * pArg)
{
	return nullptr;
}

void CCardSelect::Free()
{
	__super::Free();
	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pVIBuffer);
}

CCardSelect * CCardSelect::Create(_pDevice pGraphicDevice)
{
	CCardSelect* pInstance = new CCardSelect(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCardSelect::Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}
