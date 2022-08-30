#pragma once

#ifndef __BLACKHOLE_H__
#define __BLACKHOLE_H__

#include "Effect.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
END

BEGIN(Client)
class CBlackHole final
	: public CEffect
{
private:
	explicit CBlackHole(_pDevice pGraphicDevice);
	explicit CBlackHole(const CBlackHole& rhs);
	virtual ~CBlackHole() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Set_RenderState();
	HRESULT Release_RenderState();
	HRESULT SetUp_Component();

private:
	void Reduction(_float fTimeDelta);

private:
	CRenderer*					m_pRendererCom = nullptr;
	CVIBufferRect*				m_pVIBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;

private:
	_vec3						m_vSize;

	_float						m_fTime = 0.f;
	_float						m_fDelay = 0.3f;

public:
	virtual void Free();
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CBlackHole* Create(_pDevice pGraphicDevice);

};
END
#endif // !__BLACKHOLE_H__
