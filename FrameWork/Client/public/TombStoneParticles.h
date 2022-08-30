#pragma once

#ifndef __TOMBSTONEPARTICLES_H__
#define __TOMBSTONEPARTICLES_H__

#include "Effect.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
END

BEGIN(Client)
class CTombStoneParticles final
	: public CGameObject
{
public:
	typedef struct tagTombStoneParticlesDesc {
		_vec3 vTargetPos;
		_vec3 vStartPos;
		_vec3 vScale;
	}DESC;

private:
	explicit CTombStoneParticles(_pDevice pGraphicDevice);
	explicit CTombStoneParticles(const CTombStoneParticles& rhs);
	virtual ~CTombStoneParticles() = default;

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
	void BillBoarding();

	void Move(_float fTimeDelta);

	void Set_StartPos(void* pArg);

private:
	CRenderer*					m_pRendererCom = nullptr;
	CVIBufferRect*				m_pVIBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;

	CTransform*					m_pMoveTransformCom = nullptr;

	CTransform*					m_pMiddleTransformCom = nullptr;

private:
	_vec3						m_vSize;

	_float						m_fTime = 0.f;
	_float						m_fDelay = 0.3f;

	_vec3						m_vTargetPos;
	_vec3						m_vDir;

	_float						m_fMaxLength = 0.5f;
	_float						m_fAngle = 0.f;

public:
	virtual void Free();
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CTombStoneParticles* Create(_pDevice pGraphicDevice);

public:
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;

};
END
#endif // !__TOMBSTONEPARTICLES_H__
