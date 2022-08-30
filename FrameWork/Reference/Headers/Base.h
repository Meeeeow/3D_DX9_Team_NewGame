#pragma once
#ifndef __ENGINE_BASE_H__
#define __ENGINE_BASE_H__

#include "Engine_Defines.h"

/* 현재 엔진프로젝트 내에서만 사용될 클래스다. */
/* 레퍼런스에 대한 관리. */
BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
public:
	CBase();
	virtual ~CBase() = default;
public:
	virtual unsigned long AddRef();
	unsigned long Release();

private:
	unsigned long		m_dwRefCnt = 0;

public:
	virtual void Free() = 0;

};

END

#endif // !__ENGINE_BASE_H__
