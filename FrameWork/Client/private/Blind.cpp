#include "stdafx.h"
#include "..\public\Blind.h"
#include "Graphic_Device.h"

CBlind::CBlind(_pDevice pGraphicDevice)
	: CEffect(pGraphicDevice)
{
}

CBlind::CBlind(const CBlind & rhs)
	: CEffect(rhs)
{
}

HRESULT CBlind::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CBlind::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	SetUp_Component();
	return S_OK;
}

_int CBlind::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	
	return _int();
}

_int CBlind::LateTick(_float fTimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::GROUP_SCREENEFFECT, this);

	m_fTime -= fTimeDelta;
	if (m_fTime < 0.f)
		m_bDead = true;

	return _int();
}

HRESULT CBlind::Render()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_uint iNumCurrentCameraID = pGameInstance->Get_CurrentCameraID();
	RELEASE_INSTANCE(CGameInstance);

	m_pTransform->Bind_OnDevice();

	m_pTexture->Bind_OnDevice();
	if(iNumCurrentCameraID == CAMERA_2D)
		m_pTexture->Bind_OnDevice(1);


	_mat tmp;
	D3DXMatrixIdentity(&tmp);

	m_pGraphicDevice->SetTransform(D3DTS_VIEW, &tmp);
	m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProject);

	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 100);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pVIBuffer->Render();

	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);
	return S_OK;

}

HRESULT CBlind::SetUp_Component()
{
	m_bDead = false;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTextureBlindEffect, pTagComTexture, (CComponent**)&m_pTexture)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pTexture");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pRendererCom");

	D3DXMatrixOrthoLH(&m_matProject, (float)g_iWinCX, (float)g_iWinCY, 0.f, 1.f);
	m_fSizeX = (float)g_iWinCX;
	m_fSizeY = (float)g_iWinCY;
	m_fX = float(g_iWinCX >> 1);
	m_fY = float(g_iWinCY >> 1);

	m_pTransform->SetUp_State(CTransform::STATE::STATE_POS, _vec3(m_fX - (m_fSizeX * 0.5f), -m_fY + (m_fSizeY * 0.5f), 0.f));
	m_pTransform->SetUp_Scale(m_fSizeX, m_fSizeY, 1.f);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CBlind * CBlind::Create(_pDevice pGraphicDevice)
{
	CBlind* pInstance = new CBlind(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBlind::Clone(void * pArg )
{
	CBlind* pInstance = new CBlind(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBlind::Free()
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTransform);

	__super::Free();
}
