#include "stdafx.h"
#include "..\public\PlayerShield.h"
#include "Player.h"
#include "Collision.h"
#include "CamMatrix.h"


CPlayerShield::CPlayerShield(_pDevice pGraphicDevice)
	: CCubeObject(pGraphicDevice)
{
}

CPlayerShield::CPlayerShield(const CPlayerShield & rhs)
	: CCubeObject(rhs)
{
}

HRESULT CPlayerShield::Render()
{
	SetUp_RenderState();

	if (FAILED(__super::Render()))
		MSG_FAIL("CPlayerShield::Render - __super::Render");

	Release_RenderState();

	return S_OK;
}

_int CPlayerShield::Tick(_float fTimeDelta)
{
	if (FAILED(__super::Tick(fTimeDelta)))
		MSG_FAIL("CPlayerShield::Tick - __super::Tick");

	if (m_bDead)
		return OBJ_DEAD;

	return _int();
}

_int CPlayerShield::LateTick(_float fTimeDelta)
{
	if (FAILED(__super::LateTick(fTimeDelta)))
		MSG_FAIL("CPlayerShield::LateTick - __super::LateTick");

	return _int();
}

HRESULT CPlayerShield::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CPlayerShield::NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CPlayerShield::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CPlayerShield::NativeConstruct - __super");

	Set_AlphaBlend(true);

	return S_OK;
}

void CPlayerShield::SetUp_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("CPlayerShield::SetUp_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
		MSG_VOID("CPlayerShield::SetUp_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)))
		MSG_VOID("CPlayerShield::SetUp_RenderState - SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)))
		MSG_VOID("CPlayerShield::SetUp_RenderState - SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)");
}

void CPlayerShield::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("CPlayerShield::Release_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
		MSG_VOID("CPlayerShield::Release_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");
}

CGameObject * CPlayerShield::Clone(void * pArg)
{
	CPlayerShield* pInstance = new CPlayerShield(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CPlayerShield Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CPlayerShield * CPlayerShield::Create(_pDevice pGraphicDevice)
{
	CPlayerShield* pInstance = new CPlayerShield(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CPlayerShield Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayerShield::Free()
{
	__super::Free();
}