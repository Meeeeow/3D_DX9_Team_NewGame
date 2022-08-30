#pragma once
#ifndef __ENGINE_TIMER_MANGER_H__
#define __ENGINE_TIMER_MANGER_H__

#include "Base.h"

BEGIN(Engine)

class CTimer_Manager final:
	public CBase
{
	DECLARE_SINGLETON(CTimer_Manager)

private:
	explicit CTimer_Manager();
	virtual ~CTimer_Manager() = default;

public:
	HRESULT Add_Timer(const _tchar* pTimerTag);
	_float Compute_Timer(const _tchar* pTimerTag);

private:
	class CTimer* Find_Timer(const _tchar* pTimerTag);	

private:
	unordered_map<const _tchar*, class CTimer*> m_mapTimers;
	typedef unordered_map<const _tchar*, class CTimer*> TIMERS;

public:
	virtual void Free() override;
};

END

#endif // !__ENGINE_TIMER_MANGER_H__
