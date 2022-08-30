#include "stdafx.h"
#include "..\public\TipMgr.h"
#include "GameObject.h"
#include "BlackScreen.h"
#include "TipObject.h"

IMPLEMENT_SINGLETON(CTipMgr)

CTipMgr::CTipMgr()
{
}


CTipMgr::~CTipMgr()
{
}

void CTipMgr::SetUp_Mgr()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	m_pRenderer = dynamic_cast<CRenderer*>(pGameInstance->Clone_Component(LEVEL_STATIC, pTagPrototypeRenderer));
	if (m_pRenderer == nullptr)
		MSG_VOID("CTipMgr::SetUp_Mgr - m_pRenderer == nullptr");

	RELEASE_INSTANCE(CGameInstance);
}

void CTipMgr::Tick(_float fTimeDelta)
{
	if (!m_bTick)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Key_Down(DIK_RETURN))
	{
		pGameInstance->Set_NotTick(false);
		Set_Out();
	}
	RELEASE_INSTANCE(CGameInstance);

	for (auto& pObj : m_listObj)
	{
		if (!m_bIn && !m_bOut)
			pObj->Tick(fTimeDelta);

		if (!pObj->Get_Blend())
			m_pRenderer->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_UI_LATE, pObj);
		else
			m_pRenderer->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_UI_ALPHA, pObj);
	}

	Tick_In(fTimeDelta);
	Tick_Out(fTimeDelta);

}

void CTipMgr::Tick_In(_float fTimeDelta)
{
	if (!m_bIn)
		return;

	if (m_bFirstBounce)
	{
		m_fFallTime += fTimeDelta * 12.f;
		//스타트는 0이라 가정, 힘도 1이라 가정 
		m_fPreY = m_fY;
		m_fY = m_fFallTime * 40.f - 0.5f * GRAVITY * m_fFallTime * m_fFallTime;
	}

	if (m_fY <= 0.f && m_bFirstBounce)
	{
		m_bFirstBounce = false;
		m_bIn = false;
	}

	if (m_bFirstIn)
	{
		m_fFallTime += fTimeDelta * 16.f;
		m_fPreY = m_fY;
		m_fY = m_fStartY - 0.5f * GRAVITY * m_fFallTime * m_fFallTime;
	}

	if (0.f >= m_fY && m_bFirstIn)
	{
		m_bFirstIn = false;
		m_bFirstBounce = true;

		m_fFallTime = 0.f;
	}
	
	for (auto& pObj : m_listObj)
		pObj->Add_Pos(_vec3(0.f, m_fY - m_fPreY, 0.f));
}

void CTipMgr::Tick_Out(_float fTimeDelta)
{
	if (!m_bOut)
		return;

	if (m_fOutY > _float(g_iWinCY))
	{
		m_bUp = false;
		m_bOut = false;
		Tick_End(fTimeDelta);
	}

	if (m_bUp)
	{
		m_fOutFallTime += fTimeDelta * 12.f;
		m_fOutPreY = m_fOutY;
		m_fOutY = m_fOutFallTime * m_fOutFallTime * GRAVITY;
	}

	if (m_fOutPreY <= m_fOutY)
	{
		m_bDown = false;
		m_bUp = true;
	}

	if (m_bDown)
	{
		m_fOutFallTime += fTimeDelta * 6.f;
		m_fOutPreY = m_fOutY;
		m_fOutY = 0.5f * GRAVITY * m_fOutFallTime * m_fOutFallTime - m_fOutFallTime * 80.f;
	}

	for (auto& pObj : m_listObj)
		pObj->Add_Pos(_vec3(0.f, m_fOutY - m_fOutPreY, 0.f));
}

void CTipMgr::Tick_End(_float fTimeDelta)
{
	m_bTick = false;
	Clear_Container();
}

void CTipMgr::Clear_Container()
{
	for (auto& pObj : m_listObj)
		Safe_Release(pObj);
	m_listObj.clear();
}

void CTipMgr::Add_Obj(CTipObject * pObj)
{
	if (pObj == nullptr)
		MSG_VOID("CTipMgr::Add_Obj - pObj == nullptr");

	m_listObj.emplace_back(pObj);
	Safe_AddRef(pObj);

	m_bTick = true;

	m_bIn = true;
	m_fFallTime = 0.f;
	m_fY = m_fStartY;
	m_fPreY = m_fStartY;
	m_bFirstIn = true;
	m_bFirstBounce = false;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Set_NotTick(true);
	RELEASE_INSTANCE(CGameInstance);
}

void CTipMgr::Set_Out()
{
	m_bOut = true;
	m_bDown = true;
	m_bUp = false;
	m_fOutFallTime = 0.f;
	m_fOutY = 0.f;
	m_fOutPreY = 0.f;
}

void CTipMgr::Free()
{
	Clear_Container();

	Safe_Release(m_pRenderer);
}
