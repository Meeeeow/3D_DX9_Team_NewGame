#include "stdafx.h"
#include "..\public\EnvironmentMgr.h"
#include "Rain.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CEnvironmentMgr)

CEnvironmentMgr::CEnvironmentMgr()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pSubject = dynamic_cast<CSubject*>(pGameInstance->Clone_Component(LEVEL_STATIC, pTagPrototypeSubject));
	if (m_pSubject == nullptr)
		MSGBOX("CEnvironmentMgr::CEnvironmentMgr - m_pSubject == nullptr");

	m_dwFlag = 0;

	RELEASE_INSTANCE(CGameInstance);
}

void CEnvironmentMgr::Add_Env(_ulong dwFlag)
{
	m_dwFlag |= dwFlag;

	m_pSubject->Notify(&m_dwFlag);
}

void CEnvironmentMgr::Remove_Env(_ulong dwFlag)
{
	m_dwFlag ^= dwFlag;

	m_pSubject->Notify(&m_dwFlag);
}

HRESULT CEnvironmentMgr::SubScribe_Observer(CObserver * pObserver)
{
	return m_pSubject->Subscribe(pObserver);
}

HRESULT CEnvironmentMgr::UnSubScribe_Observer(CObserver * pObserver)
{
	return m_pSubject->UnSubscribe(pObserver);
}

void CEnvironmentMgr::Free()
{
	Safe_Release(m_pSubject);
}
