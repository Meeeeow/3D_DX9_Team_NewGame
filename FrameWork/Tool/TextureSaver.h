#pragma once
#ifndef __TOOL_TEXTURE_SAVER_H__
#define __TOOL_TEXTURE_SAVER_H__

#include "Base.h"
class CTextureSaver final : public CBase
{
	DECLARE_SINGLETON(CTextureSaver)
	
private:
	CTextureSaver();
	~CTextureSaver();

public:
	HRESULT Create_TextureDat();
	HRESULT Add_TexturePrototype();
	HRESULT Clear_List();

private:
	HRESULT PathInfoExtraction(const wstring wstrPath, list<PATH_INFO*>& rlistPathInfo);
	int FileCountFunc(const wstring& wstrPath);
	CString ConvertRelativePath(const CString& wstrFilePath);

private:
	list<PATH_INFO*> m_listPathInfo;

public:
	virtual void Free() override;
};

#endif // !__TOOL_TEXTURE_SAVER_H__
