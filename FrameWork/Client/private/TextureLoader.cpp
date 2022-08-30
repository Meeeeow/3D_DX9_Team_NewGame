#include "stdafx.h"
#include "..\public\TextureLoader.h"

IMPLEMENT_SINGLETON(CTextureLoader)


CTextureLoader::CTextureLoader()
{
}

HRESULT CTextureLoader::Load_Texture()
{
	TCHAR szBuf[MAX_PATH]{};
	lstrcat(szBuf, L"..\\bin\\Resource\\Data\\Texture\\Texture.dat");

	HANDLE hFile = CreateFile(szBuf, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		MSG_FAIL("CTextureLoader Load_Texture - hFile is INVALID_HANDLE_VALUE");

	_ulong dwByte = 0;
	PATH_INFO* pPathInfo = nullptr;
	Clear_List();
	while (true)
	{
		pPathInfo = new PATH_INFO;
		ReadFile(hFile, pPathInfo, sizeof(PATH_INFO), &dwByte, nullptr);
		
		if (dwByte == 0)
		{
			Safe_Delete(pPathInfo);
			break;
		}

		m_listPathInfo.emplace_back(pPathInfo);
	}

	CloseHandle(hFile);

	return S_OK;
}

void CTextureLoader::Free()
{
	Clear_List();
}

void CTextureLoader::Clear_List()
{
	for (auto& pPathInfo : m_listPathInfo)
		Safe_Delete(pPathInfo);

	m_listPathInfo.clear();
}
