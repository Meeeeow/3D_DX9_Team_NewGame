#include "stdafx.h"
#include "..\public\LoadingDatFile.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CLoadingDatFile)

CLoadingDatFile::CLoadingDatFile()
{
}

HRESULT CLoadingDatFile::Ready_LoadingDatFile()
{
	TCHAR szBuf[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szBuf);
	wstring strBuf = szBuf;
	wstring wstrTemp = L"\\";

	//거꾸로 돌면서 \\ 찾고 삭제 -> 폴더 하나 날림
	for (size_t i = strBuf.length() - 1; i > 0; --i)
	{
		//같은 경우
		//if (0 == lstrcmp(&strBuf[i], L"\\"))
		if (strBuf[i] == wstrTemp[0])
		{
			//찾았으니 해당 인덱스의 앞 인덱스 까지( \\ \\ 중 2번째 \\ 찾았으니 앞에것도 무시)
			strBuf = strBuf.substr(0, i);
			break;
		}
	}
	lstrcpy(szBuf, strBuf.c_str());
	lstrcat(szBuf, L"\\bin\\Resource\\Data\\Scene\\");
	strBuf = szBuf;

	//씬네임 하드코딩 필요! 여기다 추가할 것
	list<wstring> listSceneName;
	listSceneName.emplace_back(wstring(L"Intro"));
	listSceneName.emplace_back(wstring(L"Test"));
	listSceneName.emplace_back(wstring(L"UI"));
	listSceneName.emplace_back(wstring(pTagBossSkeletonSceneName));
	listSceneName.emplace_back(wstring(pTagBossClownScnenName));
	listSceneName.emplace_back(wstring(pTagBossNecromencerSceneName));
	listSceneName.emplace_back(wstring(L"PlayerBufferScene"));
	

	for (auto& wstrSceneName : listSceneName)
	{
		wstring wstrSceneDatName = strBuf + wstrSceneName + L".dat";
		
		HANDLE hFile = CreateFile(wstrSceneDatName.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (hFile == INVALID_HANDLE_VALUE)
			MSG_FAIL("CLoadingDatFile::Ready_LoadingDatFile - hFile == INVALID_HANDLE_VALUE");

		while (true)
		{
			OBJ_INFO* pObjInfo = new OBJ_INFO;
			_ulong dwByte;
			ReadFile(hFile, pObjInfo, sizeof(OBJ_INFO), &dwByte, nullptr);

			if (dwByte <= 0)
			{
				Safe_Delete(pObjInfo);
				break;
			}

			//맵 검색해서 이미 있으면 거기에 추가, 없으면 만듬
			auto& iter = m_mapScene.find(wstrSceneName);
			//없음
			if (iter == m_mapScene.end())
			{
				vector<OBJ_INFO*> vectorObjInfo;
				vectorObjInfo.emplace_back(pObjInfo);
				m_mapScene.emplace(wstrSceneName, vectorObjInfo);
			}
			//있음
			else
				iter->second.emplace_back(pObjInfo);
		}
	}

	return S_OK;
}

size_t CLoadingDatFile::Get_VectorSize(wstring wstrSceneName)
{
	auto& iter = m_mapScene.find(wstrSceneName);
	if (iter == m_mapScene.end())
	{
		MSGBOX("CLoadingDatFile::Get_VectorSize - iter == m_mapScene.end");
		return 0;
	}

	return iter->second.size();
}

const OBJ_INFO * CLoadingDatFile::Get_ObjInfo(wstring wstrSceneName, _uint iIndex) const
{
	auto& iter = m_mapScene.find(wstrSceneName);
	if (iter == m_mapScene.end())
		MSG_NULL("CLoadingDatFile::Get_ObjInfo - iter == m_mapScene.end");

	if (iIndex >= iter->second.size())
		MSG_NULL("CLoadingDatFile::Get_ObjInfo - iIndex >= iter->second.size");

	return iter->second[iIndex];
}

void CLoadingDatFile::Free()
{
	for (auto& pair : m_mapScene)
	{
		for (auto& pObjInfo : pair.second)
			Safe_Delete(pObjInfo);
		pair.second.clear();
		pair.second.shrink_to_fit();
	}
	m_mapScene.clear();
}