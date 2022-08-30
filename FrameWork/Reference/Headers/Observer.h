#pragma once

#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include "GameObject.h"
BEGIN(Engine)
class ENGINE_DLL CObserver abstract : 
	public CGameObject
{
public:
	explicit CObserver(_pDevice pGraphicDevice);
	explicit CObserver(const CObserver& rhs);
	virtual ~CObserver() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_float fTimeDelta);
	virtual _int LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void Notify(void* pMessage = nullptr) = 0;

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;

};

END

#endif // !__OBSERVER_H__
