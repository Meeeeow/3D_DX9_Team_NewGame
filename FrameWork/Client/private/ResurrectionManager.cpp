#include "stdafx.h"
#include "..\public\ResurrectionManager.h"


IMPLEMENT_SINGLETON(CResurrectionManager)
CResurrectionManager::CResurrectionManager()
{
}

HRESULT CResurrectionManager::Add_Monster(CObserver * pObserver)
{
	if (nullptr == pObserver)
		MSG_FAIL("CResurrectionManager Add_Monster - Null");

	auto iter = find(m_listObserver.begin(), m_listObserver.end(), pObserver);

	if(iter == m_listObserver.end())
		m_listObserver.emplace_back(pObserver);

	return S_OK;
}

HRESULT CResurrectionManager::Sub_Monster(CObserver * pObserver)
{
	auto iter = find(m_listObserver.begin(), m_listObserver.end(), pObserver);

	if (m_listObserver.end() != iter)
	{
		m_listObserver.erase(iter);
	}

	return S_OK;
}

void CResurrectionManager::Resurrection()
{
	for (auto& pObserver : m_listObserver)
		pObserver->Notify(nullptr);
}

void CResurrectionManager::Chimera(OBJ_INFO* pObj)
{
	for (auto& pObserver : m_listObserver)
		pObserver->Notify(pObj);
}

void CResurrectionManager::Free()
{
	m_listObserver.clear();
}
