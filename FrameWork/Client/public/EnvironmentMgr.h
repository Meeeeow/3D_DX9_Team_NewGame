#pragma once
#ifndef __CLIENT_ENVIRONMENT_MGR_H__
#define __CLIENT_ENVIRONMENT_MGR_H__

#include "Base.h"
BEGIN(Engine)
class CSubject;
class CObserver;
END

#include "Environment.h"
#define ENV_RAIN	0x0000000000000001
#define ENV_MAGMA	0x0000000000000010

BEGIN(Client)
class CEnvironmentMgr :
	public CBase
{
	DECLARE_SINGLETON(CEnvironmentMgr)

private:
	explicit CEnvironmentMgr();
	virtual ~CEnvironmentMgr() = default;

public:
	enum TYPE { TYPE_RAIN, TYPE_MAGMA, TYPE_END};

public:
	void	Add_Env(_ulong dwFlag);
	void	Remove_Env(_ulong dwFlag);

	HRESULT	SubScribe_Observer(CObserver* pObserver);
	HRESULT UnSubScribe_Observer(CObserver* pObserver);
	_ulong	Get_Env() { return m_dwFlag; }

private:
	_ulong					m_dwFlag = 0;
	CSubject*				m_pSubject = nullptr;

public:
	// CBase을(를) 통해 상속됨
	virtual void Free() override;

};
END
#endif // !__CLIENT_ENVIRONMENT_MGR_H__
