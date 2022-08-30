#include "..\public\ComponentManager.h"

IMPLEMENT_SINGLETON(CComponentManager)

CComponentManager::CComponentManager()
{
}

HRESULT CComponentManager::Reserve_Container(_uint iNumLevels)
{
	if (m_mapComponentArr != nullptr)
		MSG_FAIL("CComponentManager Reserve_Container - m_mapComponentArr is Not null");

	m_mapComponentArr = new PROTOTYPES[iNumLevels];
	m_iNumLevels = iNumLevels;

	return S_OK;
}

HRESULT CComponentManager::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	auto& iter = find_if(m_mapComponentArr[iLevelIndex].begin(), m_mapComponentArr[iLevelIndex].end()
		, CTagFinder(pPrototypeTag));
	if (iLevelIndex >= m_iNumLevels
		|| pPrototype == nullptr
		|| m_mapComponentArr[iLevelIndex].end() != iter)
		MSG_FAIL("CComponentManager - Add_Prototypes");

	m_mapComponentArr[iLevelIndex].emplace(pPrototypeTag, pPrototype);

	return S_OK;
}

CComponent * CComponentManager::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (iLevelIndex >= m_iNumLevels)
		MSG_NULL("CComponentManager Clone_Component - iLevelIndex >= m_iNumLevels");

	auto& iter = find_if(m_mapComponentArr[iLevelIndex].begin(), m_mapComponentArr[iLevelIndex].end(), CTagFinder(pPrototypeTag));
	if (iter == m_mapComponentArr[iLevelIndex].end())
		MSG_NULL("CComponentManager Clone_Component - iter is end");

	return iter->second->Clone(pArg);
}

HRESULT CComponentManager::Clear_ComponentContainer(_uint iNumLevels)
{
	if (iNumLevels >= m_iNumLevels)
		MSG_FAIL("CComponentManager::Clear_Container - iNumLevels >= m_iNumLevels");

	for (auto& pair : m_mapComponentArr[iNumLevels])
		Safe_Release(pair.second);

	m_mapComponentArr[iNumLevels].clear();
	
	return S_OK;
}

void CComponentManager::Free()
{
	for (size_t i = 0; i < m_iNumLevels; ++i)
	{
		Clear_ComponentContainer(_uint(i));
	}

	Safe_Delete_Arr(m_mapComponentArr);
}