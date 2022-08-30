#pragma once
#ifndef __CLIENT_TEXTURE_LOADER_H__
#define __CLIENT_TEXTURE_LOADER_H__

#include "Base.h"
BEGIN(Client)
class CTextureLoader final:
	public CBase
{
	DECLARE_SINGLETON(CTextureLoader)

private:
	explicit CTextureLoader();
	virtual ~CTextureLoader() = default;

public:
	HRESULT Load_Texture();

private:
	list<PATH_INFO*> m_listPathInfo;

public:
	virtual void Free() override;
	void Clear_List();
};
END
#endif // !__ENGINE_TEXTURE_LOADER_H__
