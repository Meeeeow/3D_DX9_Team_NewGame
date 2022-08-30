#pragma once
#ifndef __ENGINE_TEXTUER_H__
#define __ENGINE_TEXTUER_H__

#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CTexture final:
	public CComponent
{
public:
	enum TYPE { TYPE_GENERIC, TYPE_CUBE, TYPE_END};
private:
	explicit CTexture(_pDevice pGraphicDevice);
	virtual ~CTexture() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* szFilePath, _uint iNum, TYPE eType);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT			Bind_OnDevice(_uint iIndex = 0);
public:
	_uint Get_ImageCount() { return m_iNumTextures; }

private:
	vector<_pBaseTexture>			m_vectorTexture;
	typedef vector<_pBaseTexture>	TEXTURES;
	_uint			m_iNumTextures = 0;

public:
	static CTexture* Create(_pDevice pGraphicDevice, const _tchar* szFilePath, _uint iNum = 1, TYPE eType = TYPE_GENERIC);
	virtual CComponent * Clone(void * pArg) override;
	virtual void Free() override;
};

END
#endif // !__ENGINE_TEXTUER_H__
