#include "stdafx.h"
#include "..\public\MonsterColliderManager.h"

IMPLEMENT_SINGLETON(CMonsterColliderManager)
CMonsterColliderManager::CMonsterColliderManager()
{
	ZeroMemory(m_pColliderDat, sizeof(m_pColliderDat));
}

OBJ_INFO * CMonsterColliderManager::Get_ColliderDat(COLLIDER eDat)
{
	return m_pColliderDat[eDat];
}

void CMonsterColliderManager::Add_ColliderDat(COLLIDER eDat, OBJ_INFO * pObj)
{
	m_pColliderDat[eDat] = new OBJ_INFO(*pObj);
}

void CMonsterColliderManager::Free()
{
	for (_uint i = 0; i < COLLIDER_END; ++i)
	{
		Safe_Delete(m_pColliderDat[i]);
	}
}
