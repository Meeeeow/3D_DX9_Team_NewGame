#include "stdafx.h"
#include "..\public\CrosshairMgr.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Monster.h"
#include "TelekinesisManager.h"


IMPLEMENT_SINGLETON(CCrosshairMgr)

typedef unordered_map<const _tchar*, class CLayer*> LAYERS;

CCrosshairMgr::CCrosshairMgr()
{
}

void CCrosshairMgr::Free()
{
	Safe_Release(m_pCrosshair);
}

_bool CCrosshairMgr::Picking_Crosshair(const _tchar* szTransformTag, const _tchar* szBufferTag, const _vec3& vBasePos, _vec3* pOut)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	LAYERS mapLayer = pGameInstance->Get_CurrentLayers();

	_float fMinDist = -1.f;

	for (auto& pair : mapLayer)
	{
		for (auto& pGameObj : pair.second->Get_ObjectList())
		{
			if (!pGameObj->Check_Component(szBufferTag))
				continue;

			if (m_pCrosshair->Picking(szTransformTag, szBufferTag, pGameObj, pOut))
			{
				_float fDist = D3DXVec3Length(&(*pOut - vBasePos));
				if (fMinDist < 0.f)
					fMinDist = fDist;
				else if (fMinDist > fDist)
					fMinDist = fDist;
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	if (fMinDist < 0.f)
		return false;
	else
		return true;
}

_bool CCrosshairMgr::Picking_Crosshair_Telekinesis(const _tchar * szTransformTag, const _tchar * szBufferTag, const _vec3 & vBasePos)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	LAYERS mapLayer = pGameInstance->Get_CurrentLayers();
	CTransform* pTransform = nullptr;
	_float fMinDist = -1.f;
	_vec3  vDistance;
	for (auto& pair : mapLayer)
	{
		for (auto& pGameObj : pair.second->Get_ObjectList())
		{
			CMonster* pMonster = dynamic_cast<CMonster*>(pGameObj);
			if (!pGameObj->Check_Component(szBufferTag))
				continue;

			if (pMonster == nullptr)
				continue;

			
			if (m_pCrosshair->Picking(szTransformTag, szBufferTag, pMonster))
			{
				_float fDist = D3DXVec3Length(&(vDistance - vBasePos));
				if (fMinDist < 0.f)
				{
					if(pTransform == nullptr)
						pTransform = dynamic_cast<CTransform*>(pMonster->Get_Component(pTagComTransform));
					fMinDist = fDist;
				}
				else if (fMinDist > fDist)
				{
					pTransform = dynamic_cast<CTransform*>(pMonster->Get_Component(pTagComTransform));

					fMinDist = fDist;
				}
			}
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	if (fMinDist < 0.f || pTransform == nullptr)
		return false;
	else
	{
		CTelekinesisManager::Get_Instance()->Set_MonsterTransform(pTransform);
		return true;
	}
}


void CCrosshairMgr::Set_Crosshair(CCrosshair * pCrosshair)
{
	m_pCrosshair = pCrosshair;
	Safe_AddRef(m_pCrosshair);
}
