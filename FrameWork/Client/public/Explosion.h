#pragma once
#ifndef __CLIENT_EXPLOSION_H__
#define __CLIENT_EXPLOSION_H__

#include "Effect.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
END

BEGIN(Client)

class CExplosion final:
	public CEffect
{
public:
	typedef struct tagExplosionDesc {
		_vec3 vPos;
		_vec3 vSize;
	}DESC;
public:
	explicit CExplosion(_pDevice pDevice);
	explicit CExplosion(const CExplosion& rhs);
	virtual ~CExplosion() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET

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
public:
	virtual void Free() override;
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CExplosion* Create(_pDevice pDevice);
};

END
#endif // !__CLIENT_TERRAIN_H__
