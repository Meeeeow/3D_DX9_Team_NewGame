#pragma once
#ifndef __ENGINE_COLLIDER_RENDER_MANAGER_H__
#define __ENGINE_COLLIDER_RENDER_MANAGER_H__
#include "Base.h"

BEGIN(Engine)
class CColliderRenderManager final : public CBase
{
	DECLARE_SINGLETON(CColliderRenderManager)

public:
	explicit CColliderRenderManager();
	virtual ~CColliderRenderManager() DEFAULT;

	_bool		Get_ColliderRenderState() { return m_bColliderRender; }
	void		Set_ColliderRenderState(_bool bControl) { m_bColliderRender = bControl; }

private:
	_bool		m_bColliderRender = false;
public:
	virtual void Free() override;
};

END
#endif // !__ENGINE_COLLIDER_RENDER_MANAGER_H__
