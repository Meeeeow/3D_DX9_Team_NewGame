#include "..\public\Conversion.h"


HRESULT CConversion::CharToWChar(const _char * szBuf, _tchar* pOut)
{
	if (szBuf == nullptr)
		MSG_FAIL("CConversion CharToWChar - szBuf is null");

	size_t cn = 0;
	size_t iLength = strlen(szBuf) + 1;
	mbstowcs_s(&cn, pOut, iLength, szBuf, iLength);
	return S_OK;
}

_char * CConversion::WCharToChar(const _tchar * szBuf)
{
	if (szBuf == nullptr)
		MSG_NULL("CConversion WCharToChar - szBuf is null");

	size_t cn = 0;
	size_t iLength = lstrlen(szBuf) + 1;
	_char* szResult = new _char[iLength];
	wcstombs_s(&cn, szResult, iLength, szBuf, iLength);

	return szResult;
}
