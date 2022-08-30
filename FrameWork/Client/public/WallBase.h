#pragma once
#ifndef __CLIENT_WALL_BASE_H__
#define __CLIENT_WALL_BASE_H__

#include "GameObject.h"

class CWallBase abstract : public CGameObject
{
public:
	explicit CWallBase(_pDevice pGraphicDevice);
	explicit CWallBase(const CWallBase& rhs);
	virtual ~CWallBase() DEFAULT;

public:
	virtual _plane							Get_Plane() PURE;
	virtual CBoxCollider::COLLIDER_PLANE	Get_PlaneDir() PURE;

protected:
	_plane							m_planeCollision;
	CBoxCollider::COLLIDER_PLANE	m_ePlaneDir;
};


#endif // !__CLIENT_WALL_BASE_H__
