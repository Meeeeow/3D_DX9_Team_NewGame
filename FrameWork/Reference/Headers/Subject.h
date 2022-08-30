#pragma once

#ifndef __SUBJECT_H__
#define __SUBJECT_H__

#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CSubject final : 
	public CComponent
{
private:
	explicit CSubject(_pDevice pGraphicDevice);
	virtual ~CSubject() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	virtual HRESULT Subscribe(class CObserver* pObserver);
	virtual HRESULT UnSubscribe(class CObserver* pObserver);
	virtual void Notify(void* pMessage = nullptr);

private:
	list<class CObserver*>				m_listObservers;
	typedef list<class CObserver*>		OBSERVERS;

public:
	const OBSERVERS& Get_ObserverList() const { return m_listObservers; }

public:
	static CSubject* Create(_pDevice pGraphicDevice);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END

#endif // !__SUBJECT_H__
