#include "stdafx.h"
#include "TextureSaver.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CTextureSaver)

CTextureSaver::CTextureSaver()
{
}


CTextureSaver::~CTextureSaver()
{
}

HRESULT CTextureSaver::PathInfoExtraction(const wstring wstrPath, list<PATH_INFO*>& rlistPathInfo)
{
	CFileFind fileFind;

	size_t iPathLength = wstrPath.length();

	wstring wstrFilePath = wstrPath + L"/*.*";

	BOOL bContinue = fileFind.FindFile(wstrFilePath.c_str());
	while (bContinue)
	{
		bContinue = fileFind.FindNextFile();
		if (fileFind.IsDots())
			continue;
		else if (fileFind.IsDirectory())
			PathInfoExtraction(fileFind.GetFilePath().GetString(), rlistPathInfo);
		else
		{
			// ���� �۾��ؾ� �ϴ� ȯ��. 

			//�ý��� ������ ���. 
			if (fileFind.IsSystem())
				continue;

			PATH_INFO* pPathInfo = new PATH_INFO;

			TCHAR szPath[MAX_PATH] = L"";

			//���� ��� ����
			lstrcpy(szPath, fileFind.GetFilePath().GetString());

			//���ϸ� �߶󳾰���
			PathRemoveFileSpec(szPath);

			// ���� ���� ������ 
			pPathInfo->dwCount = FileCountFunc(szPath);

			//���ϸ� ���� Ȯ���� ������. 
			wstring wstrTextureName = fileFind.GetFileTitle().GetString();
			wstring wstrExtension = fileFind.GetFileName().GetString();
			size_t iExtensionIndex = wstrExtension.find('.');
			// ���� Tile0�̶�� -> Tile + %d = Tile%d �� ����� ����. 
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.length() - 1) + L"%d";
			// ���� Tile0.png ��� .png�� �߶���
			wstrExtension = wstrExtension.substr(iExtensionIndex, wstrExtension.length() - iExtensionIndex);
			wstrTextureName += wstrExtension;

			//q59��° �ٿ��� �߶� ��� + ���ϸ� ��ġ�� ? ������ �ϼ�/. 
			PathCombine(szPath, szPath, wstrTextureName.c_str());

			lstrcpy(pPathInfo->szFilePath, ConvertRelativePath(szPath));

			// ��ο��� �ѹ� ©�󳻸�? ������ƮŰ ���� �����ҽ� �׷��� ��Ģ ���� ����. 
			PathRemoveFileSpec(szPath);
			lstrcpy(pPathInfo->szStateKey, PathFindFileName(szPath));

			// �ѹ��� �߶󳻸�? �װ� ������Ʈ Ű 
			PathRemoveFileSpec(szPath);
			lstrcpy(pPathInfo->szObjectKey, PathFindFileName(szPath));

			rlistPathInfo.emplace_back(pPathInfo);
			bContinue = FALSE;
		}

	}

	return S_OK;
}

int CTextureSaver::FileCountFunc(const wstring & wstrPath)
{
	CFileFind find;  // ���� ��� �����ϴ� ��ü

	wstring wstrFilePath = wstrPath + L"\\*.*";

	// ���� ���� ���� �ľ�. 
	BOOL bContinue = find.FindFile(wstrFilePath.c_str());
	int iCnt = 0;
	while (bContinue)
	{
		// ������ ��� �� ���ϵ� ã�Ƴ� 
		bContinue = find.FindNextFile();
		// ���� ��
		if (find.IsSystem() || find.IsDots())
			continue;;
		++iCnt;
	}
	return iCnt;
}

CString CTextureSaver::ConvertRelativePath(const CString & wstrFilePath)
{
	TCHAR szCurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurDir);

	TCHAR szRelativePath[MAX_PATH]{};
	PathRelativePathTo(szRelativePath, szCurDir, FILE_ATTRIBUTE_DIRECTORY, wstrFilePath.GetString(), FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

HRESULT CTextureSaver::Create_TextureDat()
{
	//C:/Users/SeokRae/Documents/Visual Studio 2015/Projects/FrameWork/Client/bin/Resource/Texture
	if (FAILED(PathInfoExtraction(L"../Client/bin/Resource/Texture", m_listPathInfo)))
		MSG_FAIL("CTextureSaver Create_TextureDat - PathInfoExtraction");

	TCHAR szBuf[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Client\\bin\\Resource\\Data\\Texture\\Texture.dat");

	HANDLE hFile = CreateFile(szBuf, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		MSG_FAIL("CTextureSaver Create_TextureDat - hFile is INVALID_HANDLE_VALUE");

	_ulong dwNum;
	
	for (auto& pPathInfo : m_listPathInfo)
		WriteFile(hFile, pPathInfo, sizeof(PATH_INFO), &dwNum, nullptr);

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CTextureSaver::Add_TexturePrototype()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	for (auto& pPathInfo : m_listPathInfo)
	{
		CString wstrPrototypeTag = L"Prototype";
		wstrPrototypeTag += pPathInfo->szObjectKey;
		wstrPrototypeTag += pPathInfo->szStateKey;
		_tchar szBuf[MAX_PATH]{};
		lstrcpy(szBuf, wstrPrototypeTag.GetString());
		
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, szBuf
			, CTexture::Create(pGameInstance->Get_Device(), pPathInfo->szFilePath, pPathInfo->dwCount))))
			MSG_FAIL("CTextureSaver Add_TexturePrototype - Add_Prototype");
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CTextureSaver::Clear_List()
{
	for (auto pPathInfo : m_listPathInfo)
		Safe_Delete(pPathInfo);
	m_listPathInfo.clear();

	return S_OK;
}

void CTextureSaver::Free()
{
	Clear_List();
}
