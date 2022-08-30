#pragma once
#ifndef __CLIENT_WALL_LOOK_H__
#define __CLIENT_WALL_LOOK_H__

#include "WallBase.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBufferRectXY;
class CBoxCollider;
class CFrustum;
END

BEGIN(Engine)
class CWallLook final : public CWallBase
{
public:
	typedef struct tagWallLookDesc {
		OBJ_INFO tObjInfo;
	} DESC;
public:
	explicit CWallLook(_pDevice pGraphicDevice);
	explicit CWallLook(const CWallLook& rhs);
	virtual ~CWallLook() DEFAULT;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Tick_Collider(_float fTimeDelta) override;
	virtual void Collision(CGameObject * pColObj) override;
private:
	HRESULT			SetUp_Component();
	HRESULT			SetUp_Component(void* pArg);
	HRESULT Set_RenderState();
	HRESULT Release_RenderState();

private:
	CTransform*						m_pTransform = nullptr;
	CTexture*						m_pTexture = nullptr;
	CRenderer*						m_pRenderer = nullptr;
	CVIBufferRectXY*				m_pVIBuffer = nullptr;
	CBoxCollider*					m_pBoxCollider = nullptr;
	CFrustum*						m_pFrustum = nullptr;

private:
	_uint							m_iTextureNum = 0;
	_float							m_fMaxScale = 1.f;
	
public:
	// CWallBase을(를) 통해 상속됨
	virtual _vec3			Get_MinPos() override;
	virtual _vec3			Get_MaxPos() override;
	virtual _vec3			Get_WorldPos() override;
	virtual _vec3			Get_Length() override;
	virtual CGameObject*	Clone(void * pArg = nullptr) override;
	static	CWallLook*		Create(_pDevice pGraphicDevice);
	virtual void			Free() override;
	virtual _plane			Get_Plane() override;
	virtual CBoxCollider::COLLIDER_PLANE Get_PlaneDir() override;

};

END
#endif // !__CLIENT_WALL_LOOK_H__
