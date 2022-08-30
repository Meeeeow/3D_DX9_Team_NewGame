#pragma once
#ifndef __ENGINE_TEXTURE_LOADER_H__
#define __ENGINE_TEXTURE_LOADER_H__

#include "Base.h"
BEGIN(Engine)
class CTextureLoader final:
	public CBase
{
	DECLARE_SINGLETON(CTextureLoader)

private:
	explicit CTextureLoader();
	virtual ~CTextureLoader();

public:
	//HRESULT Save_Texture();
	//HRESULT Load_Texture();

public:
	virtual void Free() override;
};
END
#endif // !__ENGINE_TEXTURE_LOADER_H__
