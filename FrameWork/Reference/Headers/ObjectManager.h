#pragma once
#ifndef __ENGINE_OBJECT_MANAGER_H__
#define __ENGINE_OBJECT_MANAGER_H__

#include "Base.h"
BEGIN(Engine)

class CObjectManager final:
	public CBase
{
	DECLARE_SINGLETON(CObjectManager)

private:
	explicit CObjectManager();
	virtual ~CObjectManager() = default;

private:
	typedef unordered_map<const _tchar*, class CLayer*> LAYERS;
	typedef unordered_map<const _tchar*, class CGameObject*> PROTOTYPES;

public:
	HRESULT Reserve_Container(_uint iLevelNumber);
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObjectToLayer(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg, void** ppOut);
	HRESULT Add_GameObjectToLayer(_uint iLevelIndex, const _tchar* pLayerTag, CGameObject* pGameObj);
	_int Tick(_float fTimeDelta);
	HRESULT Collision_Object(_float fTimeDelta);
	_int LateTick(_float fTimeDelta);
	HRESULT Clear(_uint iLevelIndex);
	HRESULT Clear(_uint iLevelIndex, const _tchar* pLayerTag);
	LAYERS	Get_CurrentLayer(_uint iLevelIndex);

public: // Camera Container
	HRESULT			Reserve_CameraCnt(_uint iNumCameras);
	HRESULT			Add_CameraToContainer(_uint iLevelIndex, const _tchar* pPrototypeTag,_uint iCameraLevel, void* pArg);
	_int			Camera_Tick(_float fTimeDelta);
	_int			Camera_LateTick(_float fTimeDelta);
	HRESULT			Camera_Clear();
	HRESULT			Camera_Change(_uint iNumCamera);
	class CCamera*	Get_CurrentCamera() { return m_vecCamera[m_iCurrentCamera]; }
	_uint			Get_CurrentCameraID() { return m_iCurrentCamera; }
public: //SET
	HRESULT Add_CollisionTag(COLLISION_TAG* pCollisionTag);
	HRESULT Clear_CollisionTag();
	void	Set_NotTick(_bool bNotTick) { m_bNotTick = bNotTick; }

public: //GET
	class CGameObject* Get_GameObjPtr(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex);
	const _bool&		Get_NotTick() const { return m_bNotTick; }

private:
	unordered_map<const _tchar*, class CLayer*>* m_mapLayerArr = nullptr;
	list<COLLISION_TAG*> m_listCollsionTag;

private:
	unordered_map<const _tchar*, class CGameObject*> m_mapProtoType;

private:
	vector<class CCamera*>	m_vecCamera;
	_uint					m_iMaximumCamera = 0;
	_uint					m_iCurrentCamera = 0;


private:
	_uint	m_iLevelNumber = 0;
	_bool	m_bNotTick = false;

public:
	virtual void Free() override;
};

END
#endif // !__ENGINE_OBJECT_MANAGER_H__
