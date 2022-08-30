#pragma once
#ifndef __CLIENT_BLACK_SCREEN_H__
#define __CLIENT_BLACK_SCREEN_H__

#include "Effect.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
END

BEGIN(Client)

class CBlackScreen final :
	public CEffect
{
public:
	typedef struct tagBlackScreenDesc {
		_uint iTextureNum = 10;
	} DESC;

public:
	explicit CBlackScreen(_pDevice pDevice);
	explicit CBlackScreen(const CBlackScreen& rhs);
	virtual ~CBlackScreen() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET
	void Set_TextureNum(_uint iTextureNum) { m_iTextureNum = iTextureNum; }

public: //GET

private:
	HRESULT Set_RenderState();
	HRESULT Release_RenderState();
	HRESULT SetUp_Component(void* pArg);

private:
	CRenderer*					m_pRendererCom = nullptr;
	CVIBufferRect*				m_pVIBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;

private:
	_mat						m_matProject;
	_uint						m_iTextureNum = 10;

public:
	virtual void Free() override;
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CBlackScreen* Create(_pDevice pDevice);

	// CEffect을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};

END
#endif // !__CLIENT_TERRAIN_H__
