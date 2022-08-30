#pragma once
#ifndef __CLIENT_WALL_SIDE_H__
#define __CLIENT_WALL_SIDE_H__

BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBufferRectYZ;
class CBoxCollider;
class CFrustum;
END

#include "WallBase.h"
BEGIN(Client)
class CWallSide final : public CWallBase
{
public:
	typedef struct tagWallSideDesc {
		OBJ_INFO tObjInfo;
	} DESC;

public:
	explicit CWallSide(_pDevice pGraphicDevice);
	explicit CWallSide(const CWallSide& rhs);
	virtual ~CWallSide() DEFAULT;


public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta)		override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render()					override;
	virtual HRESULT Tick_Collider(_float fTimeDelta) override;
	virtual void Collision(CGameObject * pColObj) override;
private:
	HRESULT			SetUp_Component();
	HRESULT			SetUp_Component(void* pArg);
	HRESULT Set_RenderState();
	HRESULT Release_RenderState();

private:
	CTransform*						m_pTransform	= nullptr;
	CTexture*						m_pTexture		= nullptr;
	CRenderer*						m_pRenderer		= nullptr;
	CVIBufferRectYZ*				m_pVIBuffer		= nullptr;
	CBoxCollider*					m_pBoxCollider	= nullptr;
	CFrustum*						m_pFrustum		= nullptr;

private:
	_uint							m_iTextureNum = 0;
	_float							m_fFrustumRadius = 1.5f;
	_float							m_fMaxScale = 1.f;

public:
	// CWallBase을(를) 통해 상속됨
	virtual _vec3			Get_MinPos() override;
	virtual _vec3			Get_MaxPos() override;
	virtual _vec3			Get_WorldPos() override;
	virtual _vec3			Get_Length() override;
	virtual CGameObject*	Clone(void * pArg = nullptr) override;
	static  CWallSide*		Create(_pDevice pGraphicDevice);
	virtual void			Free() override;
	virtual _plane			Get_Plane() override;
	virtual CBoxCollider::COLLIDER_PLANE Get_PlaneDir() override;

};
END
#endif // !__CLIENT_WALL_SIDE_H__
