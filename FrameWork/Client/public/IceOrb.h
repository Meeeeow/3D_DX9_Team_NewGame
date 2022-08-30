#pragma once

#ifndef __ICEORB_H__
#define __ICEORB_H__

BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBufferRect;
END

#include "GameObject.h"

BEGIN(Client)
class CIceOrb final 
	: public CGameObject
{
private:
	explicit CIceOrb(_pDevice pGraphicDevice);
	explicit CIceOrb(const CIceOrb& rhs);
	virtual ~CIceOrb() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT SetUp_Component(void * pArg);

	HRESULT Set_RenderState();
	HRESULT Release_RenderState();

private:
	HRESULT BillBoarding(CTransform * pTransform);

private:
	void Shot(_float fTimeDelta);

private:
	CRenderer*			m_pRendererCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CVIBufferRect*		m_pVIBufferCom = nullptr;

	CTransform*			m_pPlayerTransformCom = nullptr;

private:
	_int				m_iShotCount = 10;

	_float				m_fShotTime = 0.f;
	_float				m_fDelayTime = 0.3f;

public:
	virtual void Free() override;
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CIceOrb* Create(_pDevice pGraphicDevice);

public:
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};
END
#endif // !__ICEORB_H__
