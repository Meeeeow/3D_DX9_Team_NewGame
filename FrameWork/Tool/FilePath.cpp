#include "stdafx.h"
#include "FilePath.h"

CString CFilePath::Get_FileNameWithoutExtension(CString wstrCurFilePath)
{
	CString wstrFileName = PathFindFileName(wstrCurFilePath);
	_tchar szFileName[MAX_PATH] = L"";
	lstrcpy(szFileName, wstrFileName.GetString());
	PathRemoveExtension(szFileName);
	wstrFileName = szFileName;
	return wstrFileName;
}
