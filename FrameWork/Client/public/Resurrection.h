#pragma once

#ifndef __RESURRECTION_H__
#define __RESURRECTION_H__

#include "Effect.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
END

BEGIN(Client)
class CResurrection final :
	public CEffect
{
public:
	typedef struct tagDesc {
		_vec3 vPos;
		_vec3 vScale;
	}DESC;

private:
	explicit CResurrection(_pDevice pGraphicDevice);
	explicit CResurrection(const CResurrection& rhs);
	virtual ~CResurrection() = default;

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
	CRenderer*					m_pRendererCom = nullptr;
	CVIBufferRect*				m_pVIBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;

public:
	virtual void Free();
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CResurrection* Create(_pDevice pGraphicDevice);

};
END
#endif // !__RESURRECTION_H__
