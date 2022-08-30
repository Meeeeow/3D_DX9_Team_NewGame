#pragma once
#ifndef __CONVERSION_H__
#define __CONVERSION_H__

#include "Base.h"
BEGIN(Engine)
class ENGINE_DLL CConversion
{
public:
	static HRESULT CharToWChar(const _char* szBuf, _tchar* pOut);
	static _char* WCharToChar(const _tchar* szBuf);
};
END
#endif // !__CONVERSION_H__
