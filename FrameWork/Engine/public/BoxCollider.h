#pragma once

#ifndef __ENGINE_COLLIDER_BOX_H__
#define __ENGINE_COLLIDER_BOX_H__

#include "Component.h"
#include "CamMatrix.h"
BEGIN(Engine)
class ENGINE_DLL CBoxCollider final : public CComponent
{
public:
	enum COLLIDER_PLANE { PLANE_FRONT, PLANE_BACK, PLANE_LEFT, PLANE_RIGHT, PLANE_TOP,PLANE_BOTTOM, PLANE_END };
private:
	explicit CBoxCollider(_pDevice pGraphicDevice);
	explicit CBoxCollider(const CBoxCollider& rhs);
	virtual ~CBoxCollider() DEFAULT;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct_Prototype(void* pArg);
	virtual HRESULT NativeConstruct_Prototype(_vec3 vColliderCenter, _vec3 vColliderSize, DWORD dwColor);
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta);
	virtual _int	LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	HRESULT			Tick_Mat(_mat matWorld);
	HRESULT			Set_DefaultRotation();
	void			Set_WorldMatrixFromObject(_mat matObjectWorldMat) { memcpy(&m_matColliderWorld, &matObjectWorldMat, sizeof(_mat)); }
	void			Update_VerticesArrToWorld();
public:
	const	_vec3	Get_MinPos() const { return m_vMinWorldPos; }
	const	_vec3	Get_MinPosProj() const { return m_vMinProjPos; }
	const	_vec3	Get_MaxPos() const { return m_vMaxWorldPos; }
	const	_vec3	Get_MaxPosProj() const { return m_vMaxProjPos; }
	const	_vec3	Get_VerticesPosArr(_uint iIndex) const { return m_vVerticesPosArr[iIndex]; }
	const	_vec3	Get_ColliderWolrdPos() const { return *(_vec3*)(&m_matColliderWorld.m[3][0]); }
	const	_mat	Get_ColliderWolrdMat() const { return m_matColliderWorld; }
	const	_plane	Get_PlaneDir(COLLIDER_PLANE ePlane) const { return m_planeDir[ePlane]; }
	_plane*			Get_PlaneDirArr() { return m_planeDir; }
	const	_vec3&	Get_ColliderCenter() const { return m_vColliderCenter; }
	_vec3*			Get_WorldPosArr() { return m_vWorldPosArr; }

private:
	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	HRESULT Create_VertexPosArr();
	HRESULT	Create_Plane();
	
private:
	_pVtxBuf			m_pVB = nullptr;
	_uint				m_iStride = 0;
	_uint				m_iNumVertices = 8;
	_ulong				m_dwFVF = 0;
	_vec3				m_vVerticesPosArr[8];
	_vec3				m_v2DPosArr[8];
	_vec3				m_vWorldPosArr[8];
	_pIdxBuf			m_pIB = nullptr;
	_uint				m_iNumPrimitive = 0;
	_uint				m_iIndicesSize = 0;
	D3DFORMAT			m_eFormat;
	D3DPRIMITIVETYPE	m_ePrimitiveType;
	_vec3				m_vMinWorldPos;
	_vec3				m_vMaxWorldPos;
	_vec3				m_vMinProjPos;
	_vec3				m_vMaxProjPos;
	_mat				m_matColliderWorld;
	_plane				m_planeDir[6];
	_vec3				m_vColliderCenter = _vec3(0.f, 0.f, 0.f);

public:
	static	CBoxCollider*	Create(_pDevice pGraphicDevice, void* pArg = nullptr);
	static	CBoxCollider*	Create_Tool(_pDevice pGraphicDevice,
		_vec3 vColliderCetner, _vec3 vColliderSize, DWORD dwColor, class CGameObject* pObj);
	virtual void			Free() override;
	virtual CComponent*		Clone(void* pArg) override;
};

END
#endif // !__ENGINE_COLLIDER_BOX_H__
