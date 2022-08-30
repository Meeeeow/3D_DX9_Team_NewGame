#pragma once
#ifndef __CLIENT_TRUMP_CARD_H__
#define __CLIENT_TRUMP_CARD_H__
#include "Effect.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
class CBoxCollider;
class CFrustum;
END

BEGIN(Client)
class CTrumpCard abstract : public CEffect
{
public:
	typedef struct TAGCARDEFFECTDESC
	{
		TAGCARDEFFECTDESC() {
			ZeroMemory(this, sizeof(TAGCARDEFFECTDESC));
			fDuration = 30.f;
			bProjectile = false;
			fPower = 2.f;
		}
		_vec3	vPos;
		_vec3	vTargetPos;
		_vec3	vDir;
		_uint	iTexutreNum;
		_float	fPower;
		_bool	bProjectile;
		_float	fDuration;
	}DESC;
public:
	explicit CTrumpCard(_pDevice pGraphicDevice);
	explicit CTrumpCard(const CTrumpCard& rhs);
	virtual ~CTrumpCard() DEFAULT;

public:
	virtual HRESULT	NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Set_RenderState();
	virtual HRESULT Release_RenderState();
protected:
	virtual void	Direction();
	virtual void	Projectile();
	virtual void	Move(_float fTimeDelta);
	virtual void	Parabola(_float fTimeDelta);
protected:
	_vec3			m_vProjectileDir;
	_uint			m_iProjectileCount = 0;
	_float			m_fDegreeProjectile = 30.f;
	_float			m_fTimeProjectile = 0.f;
protected:
	CRenderer*		m_pRenderer = nullptr;
	CVIBufferRect*	m_pVIBuffer = nullptr;
	CTransform*		m_pTransform = nullptr;
	CTexture*		m_pTexture = nullptr;
	CBoxCollider*	m_pBoxCollider = nullptr;
	CFrustum*		m_pFrustum = nullptr;

	DESC			m_tDesc;
public:
	virtual void	Free()			override;
	virtual _vec3	Get_MinPos()	override;
	virtual _vec3	Get_MaxPos()	override;
	virtual _vec3	Get_WorldPos()	override;
	virtual _vec3	Get_Length()	override;
	virtual HRESULT	Tick_Collider(_float fTimeDelta) override;
};
END

#endif // !__CLIENT_TRUMP_CARD_H__
