#include "..\public\Subject.h"

#include "Observer.h"


CSubject::CSubject(_pDevice pGraphicDevice)
	: CComponent(pGraphicDevice)
{
}

HRESULT CSubject::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("Failed To CSubject NativeConstruct_Prototype - super");

	return S_OK;
}

HRESULT CSubject::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CSubject::Subscribe(CObserver * pObserver)
{
	auto iter = find(m_listObservers.begin(), m_listObservers.end(), pObserver);

	if (m_listObservers.end() == iter)
	{
		m_listObservers.emplace_back(pObserver);
		pObserver->AddRef();
	}

	return S_OK;
}

HRESULT CSubject::UnSubscribe(CObserver * pObserver)
{
	auto iter = find(m_listObservers.begin(), m_listObservers.end(), pObserver);

	if (m_listObservers.end() != iter)
	{
		pObserver->Release();
		m_listObservers.erase(iter);
	}

	return S_OK;
}

void CSubject::Notify(void * pMessage)
{
	for (auto& pObserver : m_listObservers)
		pObserver->Notify(pMessage);
}

CSubject * CSubject::Create(_pDevice pGraphicDevice)
{
	CSubject* pInstance = new CSubject(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed To Csubject Create - NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CSubject::Clone(void * pArg)
{
	CSubject* pInstance = new CSubject(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed To Csubject Create - NativeConstruct");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubject::Free()
{
	__super::Free();

	for (auto& pObserver : m_listObservers)
		Safe_Release(pObserver);

	m_listObservers.clear();
}
