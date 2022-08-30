#pragma once
#ifndef __CLIENT_TILE_CUBE_H__
#define __CLIENT_TILE_CUBE_H__

#include "GameObject.h"

BEGIN(Engine)
class CBoxCollider;
class CTransform;
class CTexture;
class CRenderer;
class CVIBufferCube;
class CFrustum;
END


BEGIN(Client)
class CTileCube abstract : public CGameObject
{
public:
	explicit CTileCube(_pDevice pGraphicDevice);
	explicit CTileCube(const CTileCube& rhs);
	virtual ~CTileCube() DEFAULT;


public:
	virtual HRESULT		NativeConstruct_Prototype() override;
	virtual HRESULT		NativeConstruct(void* pArg) override;
	virtual _int		Tick(_float fTimeDelta) override;
	virtual _int		LateTick(_float fTimeDelta) override;
	virtual HRESULT		Render() override;

	virtual HRESULT		SetUp_RenderState();
	virtual HRESULT		Release_RenderState();

	virtual _vec3		Compute_Height(CTransform* pObjectTransform);
	virtual _plane		Get_Plane(CBoxCollider::COLLIDER_PLANE ePlane) { return m_plane[ePlane]; }

public: //SET
	void Set_TextureIndex(_uint iTextureIndex) { m_iTextureIndex = iTextureIndex; }

protected:
	CBoxCollider*		m_pBoxCollider	= nullptr;
	CTransform*			m_pTransform	= nullptr;
	CTexture*			m_pTexture		= nullptr;
	CRenderer*			m_pRenderer		= nullptr;
	CVIBufferCube*		m_pVIBuffer		= nullptr;
	CFrustum*			m_pFrustum		= nullptr;
	_plane				m_plane[6];

	_float				m_fFrustumRadius = 0.5f;
	_uint				m_iTextureIndex = 0;
	_bool				m_bCollision = false;

public:
	// CGameObject을(를) 통해 상속됨
	virtual _vec3	Get_MinPos() override;
	virtual _vec3	Get_MaxPos() override;
	virtual _vec3	Get_WorldPos() override;
	virtual _vec3	Get_Length() override;
	virtual _vec3	Get_MaxProjPos() override;
	virtual void	Free();
	virtual HRESULT Tick_Collider(_float fTimeDelta) override;
	virtual void	Collision(CGameObject* pColObj) PURE;

};

END
#endif // !__CLIENT_TILE_CUBE_H__
