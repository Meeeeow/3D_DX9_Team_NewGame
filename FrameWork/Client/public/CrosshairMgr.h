#pragma once
#ifndef __CLIENT_CROSSHAIR_MGR_H__
#define __CLIENT_CROSSHAIR_MGR_H__

#include "Base.h"
#include "Crosshair.h"

class CCrosshairMgr :
	public CBase
{
	DECLARE_SINGLETON(CCrosshairMgr)

private:
	explicit CCrosshairMgr();
	virtual ~CCrosshairMgr() = default;

public:
	const _vec3 Get_Ray() const { return m_vCrosshairRay; }
	const _vec3 Get_Pivot() const { return m_vCrosshairPivot; }
	void Set_RayToMgr(_vec3 vRay) { m_vCrosshairRay = vRay; }
	void Set_PivotToMgr(_vec3 vPivot) { m_vCrosshairPivot = vPivot; }

	_bool Picking_Crosshair(const _tchar* szTransformTag, const _tchar* szBufferTag, const _vec3& vBasePos, _vec3* pOut);
	_bool Picking_Crosshair_Telekinesis(const _tchar* szTransformTag, const _tchar* szBufferTag, const _vec3& vBasePos);
	void Set_Crosshair(CCrosshair* pCrosshair);

private:
	_vec3 m_vCrosshairPivot = _vec3(0.f, 0.f, 0.f);
	_vec3 m_vCrosshairRay;
	CCrosshair* m_pCrosshair = nullptr;

public:
	virtual void Free() override;
};

#endif // !__CLIENT_CROSSHAIR_MGR_H__
