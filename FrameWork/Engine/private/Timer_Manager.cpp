#include "..\public\Timer_Manager.h"
#include "Timer.h"

IMPLEMENT_SINGLETON(CTimer_Manager)

CTimer_Manager::CTimer_Manager()
{
}

HRESULT CTimer_Manager::Add_Timer(const _tchar * pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (pTimer != nullptr)
		return E_FAIL;

	pTimer = CTimer::Create();
	if (pTimer == nullptr)
		return E_FAIL;

	m_mapTimers.emplace(pTimerTag, pTimer);

	return S_OK;
}

_float CTimer_Manager::Compute_Timer(const _tchar * pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (pTimer == nullptr)
		return 0.f;

	return pTimer->Compute_Timer();
}

CTimer* CTimer_Manager::Find_Timer(const _tchar * pTimerTag)
{
	auto iter = find_if(m_mapTimers.begin(), m_mapTimers.end(), CTagFinder(pTimerTag));

	if (iter == m_mapTimers.end())
		return nullptr;

	return iter->second;
}

void CTimer_Manager::Free()
{
	for (auto& pair : m_mapTimers)
		Safe_Release(pair.second);

	m_mapTimers.clear();
}
