#include "stdafx.h"
#include "..\public\CardBoard.h"
#include "CardSelect.h"

CCardBoard::CCardBoard(_pDevice pGraphicDevice)
	: CGameObject(pGraphicDevice)
{
}

CCardBoard::CCardBoard(const CCardBoard & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCardBoard::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CCardBoard::NativeConstruct_Prototype - __super");

	if(FAILED(SetUp_Component()))
		MSG_FAIL("CCardBoard::NativeConstruct_Prototype - SetUp_Component");


	return S_OK;
}

HRESULT CCardBoard::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CCardBoard::NativeConstruct - __super");

	return S_OK;
}

_int CCardBoard::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;


	return OBJ_NOEVENT;
}

_int CCardBoard::LateTick(_float fTimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::GROUP_UI, this);
	m_pCardSelect->LateTick(fTimeDelta);
	return _int();
}

HRESULT CCardBoard::Render()
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

void CCardBoard::SetUp_CardSelectPoision(_uint iCardCategory, _uint iCardNumber)
{
	m_pCardSelect->SetUp_Position(iCardCategory, iCardNumber);
}

HRESULT CCardBoard::SetUp_Component()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_BOSS_CIRCUS, L"PrototypeTextureCardBoard", pTagComTexture, (CComponent**)&m_pTexture)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pTexture");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pRendererCom");

	D3DXMatrixOrthoLH(&m_matProject, (float)g_iWinCX, (float)g_iWinCY, 0.f, 1.f);
	m_fSizeX = 416;
	m_fSizeY = 192;
	m_fX = (g_iWinCX / 2.f) - (416 / 2.f);
	m_fY = (g_iWinCY / 2.f) - (192 / 2.f);


	m_pTransform->SetUp_State(CTransform::STATE::STATE_POS, _vec3(m_fX, -m_fY, 0.f));
	m_pTransform->SetUp_Scale(m_fSizeX, m_fSizeY, 1.f);

	RELEASE_INSTANCE(CGameInstance);

	m_pCardSelect = CCardSelect::Create(m_pGraphicDevice);

	return S_OK;
}

_vec3 CCardBoard::Get_MinPos()
{
	return _vec3();
}

_vec3 CCardBoard::Get_MaxPos()
{
	return _vec3();
}

_vec3 CCardBoard::Get_WorldPos()
{
	return _vec3();
}

_vec3 CCardBoard::Get_Length()
{
	return _vec3();
}

CGameObject * CCardBoard::Clone(void * pArg )
{
	CCardBoard* pInstance = new CCardBoard(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CCardBoard::Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCardBoard::Free()
{
	__super::Free();
	Safe_Release(m_pCardSelect);
	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pVIBuffer);
}

CCardBoard * CCardBoard::Create(_pDevice pGraphicDevice)
{
	CCardBoard* pInstance = new CCardBoard(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCardBoard::Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}
