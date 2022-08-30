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
			// 내가 작업해야 하는 환경. 

			//시스템 파일일 경우. 
			if (fileFind.IsSystem())
				continue;

			PATH_INFO* pPathInfo = new PATH_INFO;

			TCHAR szPath[MAX_PATH] = L"";

			//현재 경로 얻어옴
			lstrcpy(szPath, fileFind.GetFilePath().GetString());

			//파일명 잘라낼거임
			PathRemoveFileSpec(szPath);

			// 파일 갯수 셀거임 
			pPathInfo->dwCount = FileCountFunc(szPath);

			//파일명 얻어옴 확장자 제외한. 
			wstring wstrTextureName = fileFind.GetFileTitle().GetString();
			wstring wstrExtension = fileFind.GetFileName().GetString();
			size_t iExtensionIndex = wstrExtension.find('.');
			// 만약 Tile0이라면 -> Tile + %d = Tile%d 로 만드는 거임. 
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.length() - 1) + L"%d";
			// 만약 Tile0.png 라면 .png를 잘라내자
			wstrExtension = wstrExtension.substr(iExtensionIndex, wstrExtension.length() - iExtensionIndex);
			wstrTextureName += wstrExtension;

			//q59번째 줄에서 잘라낸 경로 + 파일명 합치면 ? 절대경로 완성/. 
			PathCombine(szPath, szPath, wstrTextureName.c_str());

			lstrcpy(pPathInfo->szFilePath, ConvertRelativePath(szPath));

			// 경로에서 한번 짤라내면? 스테이트키 지금 수업소스 그렇게 규칙 잡혀 있음. 
			PathRemoveFileSpec(szPath);
			lstrcpy(pPathInfo->szStateKey, PathFindFileName(szPath));

			// 한번더 잘라내면? 그게 오브젝트 키 
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
	CFileFind find;  // 파일 경로 제어하는 객체

	wstring wstrFilePath = wstrPath + L"\\*.*";

	// 파일 존재 유무 파악. 
	BOOL bContinue = find.FindFile(wstrFilePath.c_str());
	int iCnt = 0;
	while (bContinue)
	{
		// 동일한 경로 내 파일들 찾아냄 
		bContinue = find.FindNextFile();
		// 갯수 셈
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
