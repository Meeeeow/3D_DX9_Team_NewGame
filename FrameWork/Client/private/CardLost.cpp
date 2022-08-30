#include "stdafx.h"
#include "..\public\CardLost.h"
#include "BossClown.h"

_vec3 CCardLost::Get_MinPos()
{
	return _vec3();
}

_vec3 CCardLost::Get_MaxPos()
{
	return _vec3();
}

_vec3 CCardLost::Get_WorldPos()
{
	return _vec3();
}

_vec3 CCardLost::Get_Length()
{
	return _vec3();
}

CGameObject * CCardLost::Clone(void * pArg)
{
	CCardLost* pInstance = new CCardLost(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CCardLost::Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CCardLost * CCardLost::Create(_pDevice pGraphicDevice)
{
	CCardLost* pInstance = new CCardLost(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCardLost::Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCardLost::Free()
{
	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pVIBuffer);
	__super::Free();
}

CCardLost::CCardLost(_pDevice pGraphicDevice)
	: CGameObject(pGraphicDevice)
{
}

CCardLost::CCardLost(const CCardLost & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCardLost::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CCardLost::NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CCardLost::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CCardLost::NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CCardLost::NativeConstruct - SetUp_Component");

	return S_OK;
}

_int CCardLost::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CBossClown* pMonster = dynamic_cast<CBossClown*>(pGameInstance->Get_GameObjPtr(LEVEL_BOSS_CIRCUS, pTagLayerMonster, 0));
	RELEASE_INSTANCE(CGameInstance);
	if (pMonster == nullptr)
		Set_Dead();

	return _int();
}

_int CCardLost::LateTick(_float fTimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::GROUP_UI_LATE, this);

	return _int();
}

HRESULT CCardLost::Render()
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

HRESULT CCardLost::SetUp_Component(void * pArg)
{
	if (pArg == nullptr)
		return S_OK;

	m_tDesc = *(DESC*)pArg;

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_BOSS_CIRCUS, L"PrototypeTextureCardLost", pTagComTexture, (CComponent**)&m_pTexture)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pTexture");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pRendererCom");

	D3DXMatrixOrthoLH(&m_matProject, (float)g_iWinCX, (float)g_iWinCY, 0.f, 1.f);
	m_fSizeX = 32;
	m_fSizeY = 48;
	

	_int iMoveDistanceY = 192 + (m_tDesc.iCardObjectNum  * 48);
	_int iMoveDistanceX = 240 + (m_tDesc.iCardTextureNum * 32);

	m_pTransform->SetUp_State(CTransform::STATE::STATE_POS, _vec3(iMoveDistanceX, -iMoveDistanceY, 0.f));
	m_pTransform->SetUp_Scale(m_fSizeX, m_fSizeY, 1.f);

	return S_OK;
}
