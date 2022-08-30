#include "..\public\ObjectManager.h"
#include "Layer.h"
#include "GameObject.h"
#include "Observer.h"
#include "Collision.h"
#include "Camera.h"

IMPLEMENT_SINGLETON(CObjectManager)

typedef unordered_map<const _tchar*, class CLayer*> LAYERS;
typedef unordered_map<const _tchar*, class CGameObject*> PROTOTYPES;


CObjectManager::CObjectManager()
{
}

HRESULT CObjectManager::Reserve_Container(_uint iLevelNumber)
{
	if (m_mapLayerArr != nullptr)
	{
		MSGBOX("Reserve_Cotainter Failed");
		return E_FAIL;
	}

	m_mapLayerArr = new LAYERS[iLevelNumber];

	m_iLevelNumber = iLevelNumber;

	return S_OK;
}



HRESULT CObjectManager::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	auto& iter = find_if(m_mapProtoType.begin(), m_mapProtoType.end(), CTagFinder(pPrototypeTag));

	if (iter != m_mapProtoType.end())
	{
		MSGBOX("Add_Prototype Failed");
		return E_FAIL;
	}

	m_mapProtoType.emplace(pPrototypeTag, pPrototype);

	return S_OK;
}

HRESULT CObjectManager::Add_GameObjectToLayer(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pLayerTag, void * pArg, void ** ppOut)
{
	if (m_iLevelNumber <= iLevelIndex)
	{
		MSGBOX("Add_GameObjectToLayer Failed - Over LevelNumber");
		return E_FAIL;
	}

	auto& iterProtoType = find_if(m_mapProtoType.begin(), m_mapProtoType.end(), CTagFinder(pPrototypeTag));

	if (iterProtoType == m_mapProtoType.end())
	{
		MSGBOX("Add_GameObjectToLayer Failed - No ProtoType");
		return E_FAIL;
	}
	 

	CGameObject* pObj = iterProtoType->second->Clone(pArg);
	if (pObj == nullptr)
	{
		MSGBOX("Add_GameObjectToLayer Failed - Clone Nullptr");
		return E_FAIL;
	}

	// 생성된 오브젝트 반환용
	if(ppOut!=nullptr)
		*ppOut = pObj;

	//레이어 없으면 생성
	auto& iterLayer = find_if(m_mapLayerArr[iLevelIndex].begin(), m_mapLayerArr[iLevelIndex].end(), CTagFinder(pLayerTag));
	if (iterLayer == m_mapLayerArr[iLevelIndex].end())
	{
		CLayer* pLayer = CLayer::Create();
		if (pLayer == nullptr)
		{
			MSGBOX("Add_GameObjectToLayer Failed - CLayer Nullptr");
			return E_FAIL;
		}

		if (FAILED(pLayer->Add_GameObject(pObj)))
		{
			MSGBOX("Add_GameObjectToLayer Failed - pLayer Add Failed");
			return E_FAIL;
		}

		m_mapLayerArr[iLevelIndex].emplace(pLayerTag, pLayer);
	}
	//레이어 있음
	else
	{
		if (FAILED(iterLayer->second->Add_GameObject(pObj)))
		{
			MSGBOX("Add_GameObjectToLayer Failed - pLayer Add Failed");
			return E_FAIL;
		}
	}

	return S_OK;
}

//클론해서 추가하는 것이 아닌 오브젝트를 직접 받아 추가하는 함수
HRESULT CObjectManager::Add_GameObjectToLayer(_uint iLevelIndex, const _tchar * pLayerTag, CGameObject * pGameObj)
{
	if (iLevelIndex >= m_iLevelNumber)
		MSG_FAIL("CObjectManager::Add_GameObjectToLayer - iLevelIndex >= m_iLevelNumber");

	if (pGameObj == nullptr)
		MSG_FAIL("CObjectManager::Add_GameObjectToLayer - pGameObj == nullptr");

	auto& iter = find_if(m_mapLayerArr[iLevelIndex].begin(), m_mapLayerArr[iLevelIndex].end(), CTagFinder(pLayerTag));
	//레이어 없으면 직접 생성
	if (iter == m_mapLayerArr[iLevelIndex].end())
	{
		CLayer* pLayer = CLayer::Create();
		
		if (FAILED(pLayer->Add_GameObject(pGameObj)))
			MSG_FAIL("CObjectManager::Add_GameObjectToLayer - pLayer->Add_GameObject pGameObj");
		m_mapLayerArr[iLevelIndex].emplace(pLayerTag, pLayer);
	}
	//있으면 삽입
	else
	{
		if (FAILED(iter->second->Add_GameObject(pGameObj)))
			MSG_FAIL("CObjectManager::Add_GameObjectToLayer - pLayer->Add_GameObject pGameObj");
	}

	return S_OK;
}

_int CObjectManager::Tick(_float fTimeDelta)
{
	if (m_bNotTick)
		return 0;

	for (_uint i = 0; i < m_iLevelNumber; ++i)
	{
		for (auto& pair : m_mapLayerArr[i])
		{
			if (pair.second->Tick(fTimeDelta) < 0)
				MSG_MINUSONE("CObjectManager::Tick - pair.second->Tick(fTimeDelta) < 0");
		}
	}

	return 0;
}

HRESULT CObjectManager::Collision_Object(_float fTimeDelta)
{
	if (m_bNotTick)
		return S_OK;

	for (auto& pCollisionTag : m_listCollsionTag)
	{
		//등록만 해두고 아직 레이어에 추가 안된 경우 있을 수 있으니 값 못찾으면 그냥 컨티뉴
		auto& iterFirst = find_if(m_mapLayerArr[pCollisionTag->iFirstLevel].begin()
			, m_mapLayerArr[pCollisionTag->iFirstLevel].end(), CTagFinder(pCollisionTag->pFirstTag));
		if (iterFirst == m_mapLayerArr[pCollisionTag->iFirstLevel].end())
			continue;

		auto& iterSecond = find_if(m_mapLayerArr[pCollisionTag->iSecondLevel].begin(), m_mapLayerArr[pCollisionTag->iSecondLevel].end()
			, CTagFinder(pCollisionTag->pSecondTag));
		if (iterSecond == m_mapLayerArr[pCollisionTag->iSecondLevel].end())
			continue;
		
		CCollision::Check_LayerCollision(iterFirst->second, iterSecond->second, fTimeDelta, Get_CurrentCameraID());
	}
	return S_OK;
}

_int CObjectManager::LateTick(_float fTimeDelta)
{
	for (_uint i = 0; i < m_iLevelNumber; ++i)
	{
		for (auto& pair : m_mapLayerArr[i])
		{
			if (0 > pair.second->LateTick(fTimeDelta))
				return -1;
		}
	}

	return 0;
}

HRESULT CObjectManager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iLevelNumber)
		MSG_FAIL("CObjectManager::Clear - iLevelIndex >= m_iLevelNumber");

	for (auto& pair : m_mapLayerArr[iLevelIndex])
		Safe_Release(pair.second);

	m_mapLayerArr[iLevelIndex].clear();
	
	return S_OK;
}

HRESULT CObjectManager::Clear(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (iLevelIndex >= m_iLevelNumber)
		MSG_FAIL("CObjectManager::Clear - iLevelIndex >= m_iLevelNumber");

	auto& iter = find_if(m_mapLayerArr[iLevelIndex].begin(), m_mapLayerArr[iLevelIndex].end(), CTagFinder(pLayerTag));
	if (iter == m_mapLayerArr[iLevelIndex].end())
		MSG_FAIL("CObjectManager::Clear - iter == m_mapLayerArr[iLevelIndex].end()");

	Safe_Release(iter->second);
	m_mapLayerArr[iLevelIndex].erase(iter);
	return S_OK;
}

LAYERS CObjectManager::Get_CurrentLayer(_uint iLevelIndex)
{
	if (iLevelIndex > m_iLevelNumber)
		return LAYERS();

	return m_mapLayerArr[iLevelIndex];
}

HRESULT CObjectManager::Reserve_CameraCnt(_uint iNumCameras)
{
	if (!m_vecCamera.empty())
	{
		for_each(m_vecCamera.begin(), m_vecCamera.end(), Safe_Release<CCamera*>);
		m_iCurrentCamera = 0;
		m_iMaximumCamera = 0;
		m_vecCamera.clear();
	}
	m_vecCamera.reserve(iNumCameras);

	m_iMaximumCamera = iNumCameras;
	return S_OK;
}

HRESULT CObjectManager::Add_CameraToContainer(_uint iLevelIndex, const _tchar* pPrototypeTag, _uint iCameraLevel, void* pArg)
{
	if (iLevelIndex >= m_iLevelNumber)
		MSG_FAIL("CObjectManager Add_CameraToContainer - LevelIndex too big");

	if (iCameraLevel >= m_iMaximumCamera)
		MSG_FAIL("CObjectManager Add_CameraToContainer - CameraLevel too big");

	auto& ProtoTypeiter = find_if(m_mapProtoType.begin(), m_mapProtoType.end(), CTagFinder(pPrototypeTag));

	if (ProtoTypeiter == m_mapProtoType.end())
	{
		MSGBOX("CObjectManager Add_CameraToContainer - No ProtoType");
		return E_FAIL;
	}


	CCamera* pObj = dynamic_cast<CCamera*>(ProtoTypeiter->second->Clone(pArg));
	if (pObj == nullptr)
		MSG_FAIL("CObjectManager Add_CameraToContainer - Camera is nullptr");
	
	
	m_vecCamera.emplace_back(pObj);


	return S_OK;
}

_int CObjectManager::Camera_Tick(_float fTimeDelta)
{
	if (m_vecCamera.size() == 0
		|| m_vecCamera[m_iCurrentCamera] == nullptr)
		return 0;

	return m_vecCamera[m_iCurrentCamera]->Tick(fTimeDelta);
}

_int CObjectManager::Camera_LateTick(_float fTimeDelta)
{
	if (m_vecCamera.size() == 0
		|| m_vecCamera[m_iCurrentCamera] == nullptr)
		return 0;

	return m_vecCamera[m_iCurrentCamera]->LateTick(fTimeDelta);
}

HRESULT CObjectManager::Camera_Clear()
{
	for_each(m_vecCamera.begin(), m_vecCamera.end(), Safe_Release<CCamera*>);
	m_vecCamera.clear();
	m_iCurrentCamera = 0;
	m_iMaximumCamera = 0;

	return S_OK;
}

HRESULT CObjectManager::Camera_Change(_uint iNumCamera)
{
	if (m_iMaximumCamera <= iNumCamera)
		MSG_FAIL("CObjectManager Camera_Change - iNumCamera too big");

	if (m_iCurrentCamera == iNumCamera)
		return S_OK;
	

	m_iCurrentCamera = iNumCamera;
	m_vecCamera[m_iCurrentCamera]->Cam_SyncReset();

	return S_OK;
}

HRESULT CObjectManager::Add_CollisionTag(COLLISION_TAG * pCollisionTag)
{
	if (pCollisionTag == nullptr)
		MSG_FAIL("CObjectManager::Add_CollisionTag - pCollisionTag == nullptr");

	m_listCollsionTag.emplace_back(pCollisionTag);

	return S_OK;
}

HRESULT CObjectManager::Clear_CollisionTag()
{
	for (auto& pCollisionTag : m_listCollsionTag)
	{
		Safe_Delete(pCollisionTag);
	}
	m_listCollsionTag.clear();
	return S_OK;
}

CGameObject * CObjectManager::Get_GameObjPtr(_uint iLevelIndex, const _tchar * pLayerTag, _uint iIndex)
{
	if (iLevelIndex >= m_iLevelNumber)
		return nullptr;

	auto& iter = find_if(m_mapLayerArr[iLevelIndex].begin(), m_mapLayerArr[iLevelIndex].end(), CTagFinder(pLayerTag));
	if (iter == m_mapLayerArr[iLevelIndex].end())
		return nullptr;

	return iter->second->Get_GameObjPtr(iIndex);
}

void CObjectManager::Free()
{
	for (_uint i = 0; i < m_iLevelNumber; ++i)
	{
		for (auto& pair : m_mapLayerArr[i])
			Safe_Release(pair.second);
		m_mapLayerArr[i].clear();
	}

	for (auto& pair : m_mapProtoType)
		Safe_Release(pair.second);
	m_mapProtoType.clear();

	Safe_Delete_Arr(m_mapLayerArr);
	Camera_Clear();
	Clear_CollisionTag();
}
