#pragma once
#ifndef __ENGINE_COMPONENT_H__
#define __ENGINE_COMPONENT_H__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract:
	public CBase
{
protected:
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

protected:
	LPDIRECT3DDEVICE9 m_pGraphicDevice = nullptr;
	_bool			  m_bClone = false;

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};
END
#endif // !__ENGINE_COMPONENT_H__
