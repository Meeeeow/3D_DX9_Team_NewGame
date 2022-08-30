#include "..\public\Layer.h"
#include "GameObject.h"
#include "BoxCollider.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Add_GameObject(CGameObject * pObj)
{
	if (pObj == nullptr)
	{
		MSGBOX("Add GameObject Fail");
		return E_FAIL;
	}

	m_listObj.emplace_back(pObj);
	return S_OK;
}

HRESULT CLayer::NativeConstruct()
{
	return S_OK;
}

_int CLayer::Tick(_float fTimeDelta)
{
	auto& iter = m_listObj.begin();
	for (; iter != m_listObj.end(); )
	{
		_int iResult = (*iter)->Tick(fTimeDelta);

		if (iResult < 0)
			MSG_MINUSONE("CLayer::Tick - iResult < 0");

		if (iResult == OBJ_DEAD)
		{
			Safe_Release(*iter);
			iter = m_listObj.erase(iter);
		}
		else
			iter++;

	}

	return 0;
}

_int CLayer::LateTick(_float fTimeDelta)
{
	for (auto& pObj : m_listObj)
	{
		if (pObj != nullptr)
			if (0 > pObj->LateTick(fTimeDelta))
				return -1;
	}
	return 0;
}

CGameObject * CLayer::Get_GameObjPtr(_int iIndex)
{
	if (m_listObj.size() <= iIndex)
		return nullptr;

	auto& iter = m_listObj.begin();
	for (int i = 0; i < iIndex; ++i)
		iter++;
	return (*iter);
}

CLayer * CLayer::Create()
{
	CLayer* pInstance = new CLayer;

	if (FAILED(pInstance->NativeConstruct()))
	{
		Safe_Release(pInstance);
		MSGBOX("Fail to Create CLayer");
	}

	return pInstance;
}

void CLayer::Free()
{
	for (auto& pObj : m_listObj)
		Safe_Release(pObj);

	m_listObj.clear();
}