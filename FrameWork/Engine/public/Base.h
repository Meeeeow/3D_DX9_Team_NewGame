#pragma once
#ifndef __ENGINE_BASE_H__
#define __ENGINE_BASE_H__

#include "Engine_Defines.h"

/* ���� ����������Ʈ �������� ���� Ŭ������. */
/* ���۷����� ���� ����. */
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
