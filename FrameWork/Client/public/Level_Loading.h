#pragma once
#ifndef __CLIENT_LEVEL_LOADING_H__
#define __CLIENT_LEVEL_LOADING_H__	 

#include "..\..\Reference\Headers\Level.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CVIBufferRect;
class CTransform;
class CTexture;
END

BEGIN(Client)

class CLevel_Loading final:
	public CLevel
{
private:
	explicit CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT NativeConstruct(LEVEL eNextLevel, _uint iLevelID);
	virtual _int Tick(_float fTimeDelta)override;
	virtual HRESULT Render()override;
	
	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

private:
	HRESULT SetUp_Component();
	HRESULT Add_Component(LEVEL eLevel, const _tchar* pTagPrototype, CComponent** ppComponent, void* pArg = nullptr);

private:
	LEVEL				m_eLevel = LEVEL_END;
	class CLoader*		m_pLoader = nullptr;

	CVIBufferRect*		m_pVIBufferBlack = nullptr;
	CTransform*			m_pTransformBlack = nullptr;
	CTexture*			m_pTextureBlack = nullptr;

	CVIBufferRect*		m_pVIBufferLoading = nullptr;
	CTransform*			m_pTransformLoading = nullptr;
	CTexture*			m_pTextureLoading = nullptr;
	FRAME				m_tFrame;
	_mat				m_matProject; //직교투영용
public:
	static CLevel_Loading* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel, _uint iLevelID);
	virtual void Free() override;
};

END

#endif // !__CLIENT_LEVEL_LOGO_H__
