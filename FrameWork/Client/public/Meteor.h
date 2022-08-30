#pragma once
#ifndef __CLIENT_METEOR_H__
#define __CLIENT_METEOR_H__

BEGIN(Engine)
class CBoxCollider;
class CTransform;
class CTexture;
class CRenderer;
class CVIBufferCube;
class CFrustum;
END

#include "CollisionCubeObject.h"
BEGIN(Client)
class CMeteor final : public CCollisionCubeObject
{
public:
	typedef struct tagMeteorInfo{
		_vec3 vPos;
		_vec3 vTargetPos;
		_vec3 vDir;
	}DESC;
public:
	explicit CMeteor(_pDevice pGraphicDevice);
	explicit CMeteor(const CMeteor& rhs);
	virtual ~CMeteor() DEFAULT;

public:
	virtual	HRESULT		NativeConstruct_Prototype() override;
	virtual HRESULT		NativeConstruct(void* pArg) override;
	virtual _int		Tick(_float fTimeDelta) override;
	virtual _int		LateTick(_float fTimeDelta) override;
	virtual HRESULT		Render() override;
	virtual _plane		Get_Plane(CBoxCollider::COLLIDER_PLANE ePlane) { return m_plane[ePlane]; }

private:
	HRESULT				SetUp_Component(void* pArg);
	void				Move(_float fTimeDelta);
	void				TextureChange();
private:
	CTransform*		m_pTransform = nullptr;
	CTexture*		m_pTexture = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CVIBufferCube*	m_pVIBuffer = nullptr;
	CFrustum*		m_pFrustum = nullptr;
	CBoxCollider*	m_pBoxCollider = nullptr;

	_int			m_iTextureIndex = 0;
	_float			m_fFrustumRadius = 14.f;
	_float			m_fRange = 0.f;
	_float			m_fMaxRange = 0.f;
	DESC			m_tDesc;
	_plane			m_plane[CBoxCollider::PLANE_END];
	
public:
	static CMeteor*		Create(_pDevice pGraphicDevice);
	virtual CMeteor*	Clone(void* pArg) override;
	virtual void		Free() override;
	virtual void		Collision(CGameObject* pColObj) override;
	virtual _vec3		Get_MinPos() override;
	virtual _vec3		Get_MaxPos() override;
	virtual _vec3		Get_WorldPos() override;
	virtual HRESULT		Tick_Collider(_float fTimeDelta) override;
};
END
#endif // !__CLIENT_METEOR_H__
