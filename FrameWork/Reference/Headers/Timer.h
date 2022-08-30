#pragma once
#ifndef __ENGINE_TIMER_H__
#define __ENGINE_TIMER_H__

#include "Base.h"
BEGIN(Engine)

class CTimer :
	public CBase
{
public:
	explicit CTimer();
	virtual ~CTimer() = default;

public:
	HRESULT Ready_Timer();
	_float Compute_Timer();

public:
	virtual void Free() override;
	static CTimer* Create();

private:
	LARGE_INTEGER m_tCpuTick;
	LARGE_INTEGER m_tTick;
	LARGE_INTEGER m_tBegin;
	LARGE_INTEGER m_tEnd;
};

END

#endif // !__ENGINE_TIMER_H__
