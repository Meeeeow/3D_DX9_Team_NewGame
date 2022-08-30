#include "stdafx.h"
#include "..\public\TombStoneManager.h"


IMPLEMENT_SINGLETON(CTombStoneManager)
CTombStoneManager::CTombStoneManager()
{
}

HRESULT CTombStoneManager::Add_Shield(CShield * pShield)
{
	if (nullptr == pShield)
		MSG_FAIL("CTombStoneManager Add_Shield - Nullptr");

	m_pShield = pShield;

	return S_OK;
}

HRESULT CTombStoneManager::Sub_Shield()
{
	m_pShield = nullptr;

	return S_OK;
}

HRESULT CTombStoneManager::Add_TombStone(CObserver * pObserver)
{
	if (nullptr == pObserver)
		MSG_FAIL("CTombStoneManager Add_TombStone - Null");

	m_listObserver.emplace_back(pObserver);

	++m_iIdleTombStone;

	return S_OK;
}

HRESULT CTombStoneManager::Sub_TombStone(CObserver * pObserver)
{
	auto iter = find(m_listObserver.begin(), m_listObserver.end(), pObserver);

	if (m_listObserver.end() != iter)
	{
		pObserver->Release();
		m_listObserver.erase(iter);
	}

	return S_OK;
}

void CTombStoneManager::Broken()
{
	--m_iIdleTombStone;

	if (0 >= m_iIdleTombStone)
	{
		m_pShield->Broken();
	}
}

void CTombStoneManager::Repair()
{
	for (auto& pObserver : m_listObserver)
		pObserver->Notify(nullptr);
}

_vec3 CTombStoneManager::Get_ShieldPos()
{
	if (m_pShield == nullptr)
		return _vec3();

	return m_pShield->Get_ShieldPos();
}

void CTombStoneManager::Free()
{
	m_listObserver.clear();
	m_pShield = nullptr;
}
