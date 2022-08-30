#pragma once

#ifndef __ENGINE_OBSERVER_COM_H__
#define __ENGINE_OBSERVER_COM_H__

#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CObserverCom abstract : 
	public CComponent
{
public:
	explicit CObserverCom(_pDevice pGraphicDevice);
	explicit CObserverCom(const CObserverCom& rhs);
	virtual ~CObserverCom() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int Tick(_float fTimeDelta);
	virtual _int LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void Notify(void* pMessage = nullptr) = 0;

public:
	virtual CComponent* Clone(void* pArg) PURE;
	virtual void Free() override;

};

END

#endif // !__OBSERVER_H__
