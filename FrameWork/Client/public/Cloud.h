#pragma once

#ifndef __CLOUD_H__
#define __CLOUD_H__

BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBufferRect;
END

#include "GameObject.h"

BEGIN(Client)
class CCloud final
	: public CGameObject
{
public:
	enum STATE { STATE_CHARGE, STATE_SHOT, STATE_END };

private:
	explicit CCloud(_pDevice pGraphicDevice);
	explicit CCloud(const CCloud& rhs);
	virtual ~CCloud() = default;

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
	HRESULT Move(_float fTimeDelta);
	void Shot(_float fTimeDelta);

private:
	void Update_Frame(_float fTimeDelta);

private:
	CRenderer*			m_pRendererCom = nullptr;
	CTexture*			m_pTextureCom1= nullptr;
	CTexture*			m_pTextureCom2 = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CVIBufferRect*		m_pVIBufferCom = nullptr;

	CTransform*			m_pPlayerTransformCom = nullptr;

private:
	_bool				m_bShot = false;
	_bool				m_bChase = true;

	_float				m_fChargeTime = 0.f;
	_float				m_fShotTime = 3.f;

	FRAME				m_tFram;

	STATE				m_eCurState = STATE_CHARGE;

public:
	virtual void Free() override;
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CCloud* Create(_pDevice pGraphicDevice);

public:
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;

};
END
#endif // !__CLOUD_H__
