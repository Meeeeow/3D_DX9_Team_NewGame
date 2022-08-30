#include "..\public\Collision.h"
#include "Layer.h"
#include "GameObject.h"

_bool CCollision::CollisionSpherebySphere(const _vec3 & vCenter1, _float fRadius1, const _vec3 & vCenter2, _float fRadius2)
{
	_float fDistanceBetweenCenter = D3DXVec3Length(&(vCenter1 - vCenter2));

	if (fRadius1 + fRadius2 >= fDistanceBetweenCenter)
		return true;

	return false;
}

_bool CCollision::CollisionAABB(const _vec3 & vMin1, const _vec3 vMax1, const _vec3 & vMin2, const _vec3 & vMax2)
{
	if (vMax1.x < vMin2.x || vMin1.x > vMax2.x)
		return false;
	if (vMax1.y < vMin2.y || vMin1.y > vMax2.y) 
		return false;
	if (vMax1.z < vMin2.z || vMin1.z > vMax2.z) 
		return false;

	return true;
}

_bool CCollision::CollisionAABB2D(const _vec3 & vMin1, const _vec3 vMax1, const _vec3 & vMin2, const _vec3 & vMax2, const _vec3& vProjMax2, const _vec3& vPos1, const _vec3& vPos2)
{
	_mat matProj = CCamMatrix::Get_Instance()->Get_CamProj();
	_mat matView = CCamMatrix::Get_Instance()->Get_CamView();

	_vec3 vLocalMin1 = vMin1;
	_vec3 vLocalMax1 = vMax1;
	_vec3 vLocalMin2 = vMin2;
	_vec3 vLocalMax2 = vMax2;
	_vec3 vProjectionMax2 = vProjMax2;
	_vec3 vWorldPos1 = vPos1;
	_vec3 vWorldPos2 = vPos2;

	TransformVP(&vLocalMax1, matView, matProj);
	TransformVP(&vLocalMin1, matView, matProj);
	TransformVP(&vLocalMax2, matView, matProj);
	TransformVP(&vLocalMin2, matView, matProj);
	TransformVP(&vWorldPos1, matView, matProj);
	TransformVP(&vWorldPos2, matView, matProj);

	if (vWorldPos2.z > vWorldPos1.z)
	{
		if (D3DXVec3Length(&vProjectionMax2) != 0)
		{
			TransformVP(&vProjectionMax2, matView, matProj);
			_float fHeigthDistance = (vLocalMax2.y - vProjectionMax2.y) / 2.f;
			if (fHeigthDistance >= 0.f)
				vLocalMax2.y -= fHeigthDistance;
			else
				vLocalMax2 = vProjectionMax2;
		}
	}
	else
	{
		if (D3DXVec3Length(&vProjectionMax2) != 0)
		{
			TransformVP(&vProjectionMax2, matView, matProj);
			_float fHeigthDistance = (vLocalMax2.y - vProjectionMax2.y) / 2.f;
			if (fHeigthDistance <= 0.f)
				vLocalMax2 = vProjectionMax2;
		}
	}
	

	if (vLocalMax1.x < vLocalMin2.x || vLocalMin1.x > vLocalMax2.x)
		return false;
	if (vLocalMax1.y < vLocalMin2.y || vLocalMin1.y > vLocalMax2.y)
		return false;

	return true;
}

void CCollision::Check_LayerCollision(CLayer * pFirstLayer, CLayer * pSecondLayer, _float fTimeDelta, _uint iCurrentCameraNum)
{
	for (auto& pFirstObj : pFirstLayer->Get_ObjectList())
	{
		//충돌 안하는 오브젝트
		if (!pFirstObj->Get_Collision())
			continue;

		//틱에서 생성되서 틱 못타고 바로 충돌로 들어오는 객체들 대비
		pFirstObj->Tick_Collider(fTimeDelta);

		for (auto& pSecondObj : pSecondLayer->Get_ObjectList())
		{
			//충돌 안하는 오브젝트
			if (!pSecondObj->Get_Collision())
				continue;
			//틱에서 생성되서 틱 못타고 바로 충돌로 들어오는 객체들 대비
			pSecondObj->Tick_Collider(fTimeDelta);

			_vec3 pFirstMinPos = pFirstObj->Get_MinPos();
			_vec3 pFirstMaxPos = pFirstObj->Get_MaxPos();
			_vec3 pSecondMinPos = pSecondObj->Get_MinPos();
			_vec3 pSecondMaxPos = pSecondObj->Get_MaxPos();

			//충돌 났음 3D
			if (iCurrentCameraNum == 0)
			{
				if (CollisionAABB(pFirstObj->Get_MinPos(), pFirstObj->Get_MaxPos(), pSecondObj->Get_MinPos(), pSecondObj->Get_MaxPos()))
				{
					pFirstObj->Collision(pSecondObj);
					pSecondObj->Collision(pFirstObj);
				}
			}
			else if (iCurrentCameraNum == 1)
			{
				if (CollisionAABB2D(pFirstObj->Get_MinPos(), pFirstObj->Get_MaxPos(), pSecondObj->Get_MinPos(), pSecondObj->Get_MaxPos(), pSecondObj->Get_MaxProjPos(), pFirstObj->Get_WorldPos(), pSecondObj->Get_WorldPos()))
				{
					pFirstObj->Collision(pSecondObj);
					pSecondObj->Collision(pFirstObj);
				}
			}
		}
	}
}

_vec3 CCollision::WallCollisionResult(const _vec3& vTargetPos, const _vec3& vCallerPos, const _vec3& vTargetSize, const _vec3& vCallerSize)
{
	float iMoveX = 0.f, iMoveZ = 0.f;
	_vec3 vResultVector = { 0.f, 0.f, 0.f };
	float fDistanceX = abs(vTargetPos.x - vCallerPos.x);
	float fDistanceZ = abs(vTargetPos.z - vCallerPos.z);
	float fSumX = (float)((vTargetSize.x + vCallerSize.x) / 2);
	float fSumZ = (float)((vTargetSize.z + vCallerSize.z) / 2);

	if (fSumX > fDistanceX && fSumZ > fDistanceZ)
	{
		iMoveX = fSumX - fDistanceX;
		iMoveZ = fSumZ - fDistanceZ;
		if (iMoveX < iMoveZ)
		{
			if (vTargetPos.x > vCallerPos.x)
				vResultVector.x = -fDistanceX / 2;
			else
				vResultVector.x = fDistanceX / 2;
		}
		else
		{
			if (vTargetPos.z > vCallerPos.z)
				vResultVector.z = -fDistanceZ / 2;
			else
				vResultVector.z = fDistanceZ / 2;
		}
	}
	return vResultVector;
}

_vec3 CCollision::WallCollisionResult(const _plane& plTargetPlane, CBoxCollider::COLLIDER_PLANE ePlaneDir, const _vec3& vTargetPos, const _vec3& vTargetSize
	, const _vec3& vCallerMinPos, const _vec3& vCallerMaxPos, const _vec3& vCallerPos, const _vec3& vCallerSize)
{
	_vec3	vResultVector = vCallerPos;
	_float	fResult = 0.f;

	if (ePlaneDir == CBoxCollider::PLANE_FRONT)
	{
		fResult = (-plTargetPlane.a * vCallerMaxPos.x - plTargetPlane.b * vCallerMaxPos.y - plTargetPlane.d) / plTargetPlane.c;
		fResult -= vCallerSize.z / 2.f;
		vResultVector.z = fResult;
	}
	else if (ePlaneDir == CBoxCollider::PLANE_LEFT)
	{
		fResult = (-plTargetPlane.b * vCallerMinPos.y - plTargetPlane.c * vCallerMinPos.z - plTargetPlane.d) / plTargetPlane.a;
		fResult -= vCallerSize.x / 2.f;
		vResultVector.x = fResult;
	}
	else if (ePlaneDir == CBoxCollider::PLANE_BACK)
	{
		fResult = (-plTargetPlane.a * vCallerMaxPos.x - plTargetPlane.b * vCallerMaxPos.y - plTargetPlane.d) / plTargetPlane.c;
		fResult += vCallerSize.z / 2.f;
		vResultVector.z = fResult;
	}
	else if (ePlaneDir == CBoxCollider::PLANE_RIGHT)
	{
		fResult = (-plTargetPlane.b * vCallerMinPos.y - plTargetPlane.c * vCallerMinPos.z - plTargetPlane.d) / plTargetPlane.a;
		fResult += vCallerSize.x / 2.f;
		vResultVector.x = fResult;
	}

	return vResultVector;
}

_bool CCollision::PlaneCheck(const _plane & plTargetPlane, const _vec3 & vCheckPos)
{

	float fResult = (plTargetPlane.a * vCheckPos.x) + (plTargetPlane.b * vCheckPos.y) + (plTargetPlane.c * vCheckPos.z) + plTargetPlane.d;

	if (fResult > 0.f)
		return true;
	else
		return false;
}

_vec3 CCollision::CubeCollisionResult(const _plane& plTargetPlane, CBoxCollider::COLLIDER_PLANE ePlaneDir
	, const _vec3& vObjectMin, const _vec3& vObjectMax, const _vec3& vObjectPos, const _vec3& vObjectSize)
{
	_vec3	vResultVector = vObjectPos;
	_float	fResult = 0.f;

	if (ePlaneDir == CBoxCollider::PLANE_FRONT)
	{
		fResult = (-plTargetPlane.a * vObjectMax.x - plTargetPlane.b * vObjectMax.y - plTargetPlane.d) / plTargetPlane.c;
		fResult -= vObjectSize.z / 2.f;
		vResultVector.z = fResult;
	}
	else if (ePlaneDir == CBoxCollider::PLANE_BACK)
	{
		fResult = (-plTargetPlane.a * vObjectMin.x - plTargetPlane.b * vObjectMin.y - plTargetPlane.d) / plTargetPlane.c;
		fResult += vObjectSize.z / 2.f;
		vResultVector.z = fResult;
	}
	else if (ePlaneDir == CBoxCollider::PLANE_LEFT)
	{
		fResult = (-plTargetPlane.b * vObjectMax.y - plTargetPlane.c * vObjectMax.z - plTargetPlane.d) / plTargetPlane.a;
		fResult -= vObjectSize.x / 2.f;
		vResultVector.x = fResult;
	}
	else if (ePlaneDir == CBoxCollider::PLANE_RIGHT)
	{
		fResult = (-plTargetPlane.b * vObjectMin.y - plTargetPlane.c * vObjectMin.z - plTargetPlane.d) / plTargetPlane.a;
		fResult += vObjectSize.x / 2.f;
		vResultVector.x = fResult;
	}
	else if (ePlaneDir == CBoxCollider::PLANE_BOTTOM)
	{
		fResult = (-plTargetPlane.a * vObjectMax.x - plTargetPlane.c * vObjectMax.z - plTargetPlane.d) / plTargetPlane.b;
		fResult -= vObjectSize.y / 2.f;
		vResultVector.y = fResult;
	}
	else if (ePlaneDir == CBoxCollider::PLANE_TOP)
	{
		fResult = (-plTargetPlane.a * vObjectMin.x - plTargetPlane.c * vObjectMin.z - plTargetPlane.d) / plTargetPlane.b;
		fResult += vObjectSize.y / 2.f;
		vResultVector.y = fResult;
	}
	return vResultVector;
}

_bool CCollision::CubeCollision(CGameObject* pTarget, _plane CallerPlane[], _vec3* vTargetReturnPos, _bool* bOntheCube, _bool* bUnderCube, _vec3 vColliderCenter)
{
	_vec3 vTargetPos = pTarget->Get_WorldPos();
	vTargetPos += vColliderCenter;
	_vec3 vTargetSize = pTarget->Get_Length();
	_vec3 vTargetMin = pTarget->Get_MinPos();
	_vec3 vTargetMax = pTarget->Get_MaxPos();


	_bool Front = PlaneCheck(CallerPlane[CBoxCollider::PLANE_FRONT], vTargetPos);
	_bool Back = PlaneCheck(CallerPlane[CBoxCollider::PLANE_BACK], vTargetPos);
	_bool Left = PlaneCheck(CallerPlane[CBoxCollider::PLANE_LEFT], vTargetPos);
	_bool Right = PlaneCheck(CallerPlane[CBoxCollider::PLANE_RIGHT], vTargetPos);
	_bool Top = PlaneCheck(CallerPlane[CBoxCollider::PLANE_TOP], vTargetPos);
	_bool Bottom = PlaneCheck(CallerPlane[CBoxCollider::PLANE_BOTTOM], vTargetPos);

	_vec3 vResult = { 0.f , 0.f , 0.f };

	if (Front)
		vResult = CubeCollisionResult(CallerPlane[CBoxCollider::PLANE_FRONT], CBoxCollider::PLANE_FRONT
			, vTargetMin, vTargetMax, vTargetPos, vTargetSize);
	if (Back)
		vResult = CubeCollisionResult(CallerPlane[CBoxCollider::PLANE_BACK], CBoxCollider::PLANE_BACK
			, vTargetMin, vTargetMax, vTargetPos, vTargetSize);
	if( Left)
		vResult = CubeCollisionResult(CallerPlane[CBoxCollider::PLANE_LEFT], CBoxCollider::PLANE_LEFT
			, vTargetMin, vTargetMax, vTargetPos, vTargetSize);
	if (Right)
		vResult = CubeCollisionResult(CallerPlane[CBoxCollider::PLANE_RIGHT], CBoxCollider::PLANE_RIGHT
			, vTargetMin, vTargetMax, vTargetPos, vTargetSize);
	if (Top)
		vResult = CubeCollisionResult(CallerPlane[CBoxCollider::PLANE_TOP], CBoxCollider::PLANE_TOP
			, vTargetMin, vTargetMax, vTargetPos, vTargetSize);
	if (Bottom)
		vResult = CubeCollisionResult(CallerPlane[CBoxCollider::PLANE_BOTTOM], CBoxCollider::PLANE_BOTTOM
			, vTargetMin, vTargetMax, vTargetPos, vTargetSize);



	if (Top)
		*bOntheCube = true;
	if (Bottom)
		*bUnderCube = true;

	vResult -= vColliderCenter;
	*vTargetReturnPos = vResult;
	return true;
}

void CCollision::RectCheck(const _vec3& vTargetPos, const _vec3& vTargetMaxSize, const _vec3& vTargetMinSize
	, const _vec3& vCallerPos, const _vec3& vCallerMaxSize, const _vec3& vCallerMinSize, _vec3 vCallerWorldArr[]
	, _bool* bLeft, _bool* bRight, _bool* bTop, _bool* bBottom)
{
	// Top
	if (vTargetPos.y > (vCallerPos.y - vCallerMinSize.y)
		&& vTargetPos.x < (vCallerPos.x + vCallerMaxSize.x)
		&& vTargetPos.x > (vCallerPos.x - vCallerMinSize.x))
			*bTop = true;
	// Bottom
	if (vTargetPos.y + vTargetMaxSize.y < vCallerPos.y - vCallerMinSize.y
		&& vTargetPos.x - vTargetMinSize.x < vCallerPos.x
		&& vTargetPos.x + vTargetMaxSize.x > vCallerPos.x)
		*bBottom = true;
	// Right
	if (vTargetPos.x > vCallerPos.x + vCallerMaxSize.x
		&& vTargetPos.y > vCallerPos.y - vCallerMinSize.y
		&& vTargetPos.y < vCallerPos.y + vCallerMaxSize.y)
		*bRight = true;
	// Left
	if (vTargetPos.x < vCallerPos.x - vCallerMinSize.x
		&& vTargetPos.y > vCallerPos.y - vCallerMinSize.y
		&& vTargetPos.y < vCallerPos.y + vCallerMaxSize.y)
		*bLeft = true;
}

_bool CCollision::RectCollision(CGameObject * pTarget, CGameObject * pCaller, _vec3 vCallerWorldArr[], _vec3 * vTargetReturnPos, _bool * bOntheCube, _bool * bUndertheCube, _vec3 vColliderCenter)
{
	_mat matCurrentView = CCamMatrix::Get_Instance()->Get_CamView();
	_mat matCurrentProj = CCamMatrix::Get_Instance()->Get_CamProj();
	_bool bLeft = false; _bool bRight = false; _bool bTop = false; _bool bBottom = false;
	// 타겟의 크기를 투영 공간까지 내린다.
	_vec3 vTargetPos = pTarget->Get_WorldPos(); _vec3 vTargetMinPos = pTarget->Get_MinPos(); _vec3 vTargetMaxPos = pTarget->Get_MaxPos();
	vTargetPos += vColliderCenter;
	TransformVP(&vTargetPos, matCurrentView, matCurrentProj); TransformVP(&vTargetMinPos, matCurrentView, matCurrentProj); TransformVP(&vTargetMaxPos, matCurrentView, matCurrentProj);
	TransformVP(&vCallerWorldArr[1], matCurrentView, matCurrentProj);  TransformVP(&vCallerWorldArr[4], matCurrentView, matCurrentProj); TransformVP(&vCallerWorldArr[5], matCurrentView, matCurrentProj);
	_vec3 vTargetMaxSize = { vTargetMaxPos.x - vTargetPos.x , vTargetMaxPos.y - vTargetPos.y , vTargetMaxPos.z - vTargetPos.z };
	_vec3 vTargetMinSize = { vTargetPos.x - vTargetMinPos.x , vTargetPos.y - vTargetMinPos.y , vTargetPos.z - vTargetMinPos.z };
	_vec3 vTargetNormalSize = { vTargetMaxPos.x - vTargetMinPos.x , vTargetMaxPos.y - vTargetMinPos.y , vTargetMaxPos.z - vTargetMinPos.z };

	// 콜러의 크기를 투영 공간까지 내린다.
	_vec3 vCallerPos = pCaller->Get_WorldPos(); _vec3 vCallerMinPos = pCaller->Get_MinPos(); _vec3 vCallerMaxPos = pCaller->Get_MaxPos(); _vec3 vCallerProjMaxPos = pCaller->Get_MaxProjPos();
	TransformVP(&vCallerPos, matCurrentView, matCurrentProj); TransformVP(&vCallerMinPos, matCurrentView, matCurrentProj); TransformVP(&vCallerMaxPos, matCurrentView, matCurrentProj); TransformVP(&vCallerProjMaxPos, matCurrentView, matCurrentProj);

	if (vTargetPos.z < vCallerPos.z)
	{
		if (vCallerProjMaxPos.y < vCallerMaxPos.y)
		{
			_float fWidthDistance = (vCallerMaxPos.x - vCallerProjMaxPos.x) / 2.f;
			_float fHeightDistance = (vCallerMaxPos.y - vCallerProjMaxPos.y) / 2.f;
			vCallerMaxPos.x -= fWidthDistance;
			vCallerMaxPos.y -= fHeightDistance;
		}
		else
			vCallerMaxPos = vCallerProjMaxPos;
	}
	else if (vTargetPos.z > vCallerPos.z)
	{
		_float fWidthDistance = (vCallerMaxPos.x - vCallerProjMaxPos.x) / 2.f;
		_float fHeightDistance = (vCallerMaxPos.y - vCallerProjMaxPos.y) / 2.f;

	}
	
	

	_vec3 vCallerMaxSize = { vCallerMaxPos.x - vCallerPos.x , vCallerMaxPos.y - vCallerPos.y , vCallerMaxPos.z - vCallerPos.z };
	_vec3 vCallerMinSize = { vCallerPos.x - vCallerMinPos.x , vCallerPos.y - vCallerMinPos.y , vCallerPos.z - vCallerMinPos.z };
	_vec3 vCallerNormalSize = { vCallerMaxPos.x - vCallerMinPos.x , vCallerMaxPos.y - vCallerMinPos.y  , vCallerMaxPos.z - vCallerMinPos.z };
	_vec3 vResultPos = vTargetPos;
	RectCheck(vTargetPos, vTargetMaxSize, vTargetMinSize,
		vCallerPos, vCallerMaxSize, vCallerMinSize, vCallerWorldArr,&bLeft, &bRight, &bTop, &bBottom);



	if (bTop)
		vResultPos.y = { vCallerMaxPos.y + (vTargetMinSize.y / 1.f) };
	if (bBottom)
		vResultPos.y = { vCallerMinPos.y - (vTargetMaxSize.y / 1.f) };
	if (bRight)
		vResultPos.x = { vCallerMaxPos.x + (vTargetMinSize.x / 1.f) };
	if (bLeft)
		vResultPos.x = { vCallerMinPos.x - (vTargetMinSize.x / 1.f) };


	if (vCallerPos.z < vTargetPos.z)
	{
		_float fZDistance = 0.f;
		fZDistance = vTargetPos.z - vCallerPos.z;
		vResultPos.y += fZDistance * vCallerPos.y;
	}


	if (bTop)
		*bOntheCube = true;
	if (bBottom)
		*bUndertheCube = true;

	TransformToWorld(&vResultPos, matCurrentView, matCurrentProj);
	vResultPos -= vColliderCenter;
	
	*vTargetReturnPos = vResultPos;

	return true;
}

void CCollision::TransformVP(_vec3* vWorldPos, const _mat& matView, const _mat& matProj)
{
	_vec3 vTransformPos = *vWorldPos;
	D3DXVec3TransformCoord(&vTransformPos, &vTransformPos, &matView);
	D3DXVec3TransformCoord(&vTransformPos, &vTransformPos, &matProj);
	*vWorldPos = vTransformPos;
}

void CCollision::TransformToWorld(_vec3 * vProjPos, const _mat & matView, const _mat & matProj)
{
	_vec3 vTransformPos = *vProjPos;
	_mat matInvProj, matInvView;

	D3DXMatrixInverse(&matInvProj, nullptr, &matProj);
	D3DXMatrixInverse(&matInvView, nullptr, &matView);

	D3DXVec3TransformCoord(&vTransformPos, &vTransformPos, &matInvProj);
	D3DXVec3TransformCoord(&vTransformPos, &vTransformPos, &matInvView);
	*vProjPos = vTransformPos;

}
