#pragma once
#ifndef __ENGINE_COLLISION_H__
#define __ENGINE_COLLISION_H__

#include "Base.h"

#include "BoxCollider.h"

BEGIN(Engine)
class ENGINE_DLL CCollision
{
public:
	static _bool	CollisionSpherebySphere(const _vec3 & vCenter1, _float fRadius1, const _vec3 & vCenter2, _float fRadius2);
	static _bool	CollisionAABB(const _vec3& vMin1, const _vec3 vMax1, const _vec3& vMin2, const _vec3& vMax2);
	static _bool	CollisionAABB2D(const _vec3& vMin1, const _vec3 vMax1, const _vec3& vMin2, const _vec3& vMax2, const _vec3& vProjMax2, const _vec3& vPos1, const _vec3& vPos2);
	static void		Check_LayerCollision(class CLayer* pFirstLayer, class CLayer* pSecondLayer, _float fTimeDelta, _uint iCurrentCameraNum);
	// 바운드
	static _vec3 WallCollisionResult(const _vec3& vTargetPos, const _vec3& vCallerPos,const _vec3& vTargetSize, const _vec3& vCallerSize);
	// 딱 붙
	static _vec3 WallCollisionResult(const _plane& plTargetPlane, CBoxCollider::COLLIDER_PLANE ePlaneDir, const _vec3& vTargetPos, const _vec3& vTargetSize
		,const _vec3& vCallerMinPos , const _vec3& vCallerMaxPos, const _vec3& vCallerPos,const _vec3& vCallerSize);
	

	static _bool PlaneCheck(const _plane& plTargetPlane, const _vec3& vCheckPos);
	// 큐브 6면 다 검사해야함
	static _vec3 CubeCollisionResult(const _plane& plTargetPlane, CBoxCollider::COLLIDER_PLANE ePlaneDir
		, const _vec3& vObjectMin, const _vec3& vObjectMax, const _vec3& vObjectPos,const _vec3& vObjectSize);

	static _bool CubeCollision(CGameObject* pTarget, _plane CallerPlane[], _vec3* vTargetReturnPos, _bool* bOntheCube, _bool* bUnderCube, _vec3 vColliderCenter = _vec3(0.f, 0.f, 0.f));

	static void  RectCheck(const _vec3& vTargetPos, const _vec3& vTargetMaxSize, const _vec3& vTargetMinSize 
		, const _vec3& vCallerPos, const _vec3& vCallerMaxSize, const _vec3& vCallerMinSize, _vec3 vCallerWorldArr[]
		, _bool* bLeft, _bool* bRight, _bool* bTop, _bool* bBottom);
	static _bool RectCollision(CGameObject* pTarget, CGameObject* pCaller, _vec3 vCallerWorldArr[], _vec3* vTargetReturnPos, _bool* bOntheCube, _bool* bUndertheCube, _vec3 vColliderCenter = _vec3(0.f,0.f,0.f));

	static void  TransformVP(_vec3* vWorldPos, const _mat& matView, const _mat& matProj);
	static void  TransformToWorld(_vec3* vProjPos, const _mat& matView, const _mat& matProj);
};
END
#endif // !__ENGINE_COLLISION_H__
