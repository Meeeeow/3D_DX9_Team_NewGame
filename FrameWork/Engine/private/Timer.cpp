#include "..\public\Timer.h"

CTimer::CTimer()
{
}

HRESULT CTimer::Ready_Timer()
{
	QueryPerformanceFrequency(&m_tCpuTick);
	QueryPerformanceCounter(&m_tTick);
	QueryPerformanceCounter(&m_tBegin);
	QueryPerformanceCounter(&m_tEnd);

	return S_OK;
}

_float CTimer::Compute_Timer()
{
	QueryPerformanceCounter(&m_tEnd);

	//1초마다 cputick 갱신
	if (m_tEnd.QuadPart - m_tTick.QuadPart >= m_tCpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_tCpuTick);
		m_tTick = m_tEnd;
	}

	_float fTime = (float)(m_tEnd.QuadPart - m_tBegin.QuadPart) / m_tCpuTick.QuadPart;
	m_tBegin = m_tEnd;

	return fTime;
}

void CTimer::Free()
{
}

CTimer * CTimer::Create()
{
	CTimer* pTimer = new CTimer;

	if (FAILED(pTimer->Ready_Timer()))
		Safe_Delete(pTimer);

	return pTimer;
}
