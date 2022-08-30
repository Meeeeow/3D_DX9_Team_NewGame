#include "..\public\Level_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
{
}

HRESULT CLevel_Manager::Change_Level(CLevel * pLevel)
{
	if (pLevel == nullptr)
		return E_FAIL;

	if (m_pCurLevel != nullptr)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		pGameInstance->Clear_Container(m_pCurLevel->Get_LevelID());
		pGameInstance->Clear_ComponentContainer(m_pCurLevel->Get_LevelID());
		RELEASE_INSTANCE(CGameInstance);
	}

	Safe_Release(m_pCurLevel);

	m_pCurLevel = pLevel;

	return S_OK;
}

_int CLevel_Manager::Tick(_float fTimeDelta)
{
	if (m_pCurLevel == nullptr)
		return -1;

	return m_pCurLevel->Tick(fTimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (m_pCurLevel == nullptr)
		return E_FAIL;

	return m_pCurLevel->Render();
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pCurLevel);
}
