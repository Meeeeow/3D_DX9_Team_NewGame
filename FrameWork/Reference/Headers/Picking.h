#pragma once
#ifndef __ENGINE_PICKING_H__
#define	__ENGINE_PICKING_H__

#include "Base.h"
BEGIN(Engine)
class CPicking :
	public CBase
{
	DECLARE_SINGLETON(CPicking)

private:
	explicit CPicking();
	virtual ~CPicking() = default;

public:
	HRESULT PreConvert();
	_bool Picking(const _tchar* szTransformTag, const _tchar* szBufferTag, class CGameObject* pTarget, _vec3* pOut);
	const _vec3& Get_MouseRay() const { return m_vMouseRay; }

private:
	_vec3			m_vMouseRay;
	_vec3			m_vMousePivot;

public:
	virtual void Free() override;
};
END
#endif // !__ENGINE_PICKING_H__
