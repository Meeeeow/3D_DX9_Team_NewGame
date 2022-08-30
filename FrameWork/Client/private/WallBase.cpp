#include "stdafx.h"
#include "..\public\WallBase.h"



CWallBase::CWallBase(_pDevice pGraphicDevice)
	: CGameObject(pGraphicDevice)
{
}

CWallBase::CWallBase(const CWallBase & rhs)
	: CGameObject(rhs)
	, m_ePlaneDir(rhs.m_ePlaneDir)
	, m_planeCollision(rhs.m_planeCollision)
{
}
