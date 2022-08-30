#include "..\public\GameInstance.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "ObjectManager.h"
#include "Picking.h"
#include "CamMatrix.h"
#include "Collision.h"
#include "ColliderRenderManager.h"

IMPLEMENT_SINGLETON(CGameInstance)
typedef unordered_map<const _tchar*, class CLayer*> LAYERS;
CGameInstance::CGameInstance()
	:m_pTimerManager(CTimer_Manager::Get_Instance()), m_pGraphicDevice(CGraphic_Device::Get_Instance()), m_pLevelManager(CLevel_Manager::Get_Instance())
	, m_pObjMgr(CObjectManager::Get_Instance()), m_pComponentMgr(CComponentManager::Get_Instance()), m_pInputDevice(CInputDevice::Get_Instance())
	, m_pPicking(CPicking::Get_Instance()), m_pCamMatrix(CCamMatrix::Get_Instance()) , m_pColliderRenderMgr(CColliderRenderManager::Get_Instance())
{
	Safe_AddRef(m_pCamMatrix);
	Safe_AddRef(m_pPicking);
	Safe_AddRef(m_pTimerManager);
	Safe_AddRef(m_pLevelManager);
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pObjMgr);
	Safe_AddRef(m_pComponentMgr);
	Safe_AddRef(m_pInputDevice);
	Safe_AddRef(m_pColliderRenderMgr);
}

HRESULT CGameInstance::Add_Timer(const _tchar * pTimerTag)
{
	if (m_pTimerManager == nullptr)
		MSG_FAIL("CGameInstance Add_Timer - m_pTimerManager is null");

	return m_pTimerManager->Add_Timer(pTimerTag);
}

_float CGameInstance::Compute_Timer(const _tchar * pTimerTag)
{
	if (m_pTimerManager == nullptr)
		MSG_MINUSONE("CGameInstance Compute_Timer - m_pTimerManager is null");

	return m_pTimerManager->Compute_Timer(pTimerTag);
}

HRESULT CGameInstance::Ready_Graphic_Device(HWND hWnd, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppOut)
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CGameInstance Ready_Graphic_Device - m_pGraphicDevice is null");

	return m_pGraphicDevice->Ready_Graphic_Device(hWnd, iWinCX, iWinCY, ppOut);
}

void CGameInstance::Render_Begin()
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("CGameInstance Render_Begin - m_pGraphicDevice is null");

	m_pGraphicDevice->Render_Begin();
}

void CGameInstance::Render_End(HWND hWND)
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("CGameInstance Render_End - m_pGraphicDevice is null");

	m_pGraphicDevice->Render_End(hWND);
}

void CGameInstance::Draw_Font(const _tchar * pText, D3DXCOLOR FontColor, _vec3* vPos, _vec3 vScale)
{
	if (m_pGraphicDevice == nullptr)
		MSG_VOID("CGameInstance Draw_Font - m_pGraphicDevice is null");

	m_pGraphicDevice->Draw_Font(pText, FontColor, vPos, vScale);
}

_pDevice CGameInstance::Get_Device()
{
	if (m_pGraphicDevice == nullptr)
		MSG_NULL("CGameInstance Get_Device - m_pGraphicDevice is null");

	return m_pGraphicDevice->Get_Device();
}

LPD3DXLINE CGameInstance::Get_Line()
{
	if (m_pGraphicDevice == nullptr)
		MSG_NULL("CGameInstance::Get_Line - m_pGraphicDevice == nullptr");

	return m_pGraphicDevice->Get_Line();
}

HRESULT CGameInstance::Change_Level(CLevel * pLevel)
{
	if (m_pLevelManager == nullptr)
		MSG_FAIL("CGameInstance Change_Level - m_pLevelManager is null");

	return m_pLevelManager->Change_Level(pLevel);
}

HRESULT CGameInstance::Render()
{
	if (m_pLevelManager == nullptr)
		MSG_FAIL("CGameInstance Render - m_pLevelManager is null");

	return m_pLevelManager->Render();
}

_int CGameInstance::Get_CurrentLevel()
{
	if (m_pLevelManager == nullptr)
		MSG_MINUSONE("CGameInstance::Get_CurrentLevel - m_pLevelManager == nullptr");

	return m_pLevelManager->Get_CurrentLevel();
}

HRESULT CGameInstance::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (m_pObjMgr == nullptr)
		MSG_FAIL("CGameInstance Add_Prototype - m_pObjMgr is null");

	return m_pObjMgr->Add_Prototype(pPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Add_GameObjectToLayer(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pLayerTag, void * pArg, void** ppOut)
{
	if (m_pObjMgr == nullptr)
		MSG_FAIL("CGameInstance Add_GameObjectToLayer - m_pObjMgr is null");

	return m_pObjMgr->Add_GameObjectToLayer(iLevelIndex, pPrototypeTag, pLayerTag, pArg, ppOut);
}

HRESULT CGameInstance::Add_GameObjectToLayer(_uint iLevelIndex, const _tchar * pLayerTag, CGameObject * pGameObj)
{
	if (m_pObjMgr == nullptr)
		MSG_FAIL("CGameInstance Add_GameObjectToLayer - m_pObjMgr is null");

	return m_pObjMgr->Add_GameObjectToLayer( iLevelIndex,  pLayerTag,  pGameObj);
}

CGameObject * CGameInstance::Get_GameObjPtr(_uint iLevelIndex, const _tchar * pLayerTag, _uint iIndex)
{
	if (m_pObjMgr == nullptr)
		return nullptr;

	return m_pObjMgr->Get_GameObjPtr(iLevelIndex, pLayerTag, iIndex);
}

const _bool & CGameInstance::Get_NotTick() const
{
	if (m_pObjMgr == nullptr)
		MSG_FALSE("CGameInstance::Get_NotTick - m_pObjMgr == nullptr");

	return m_pObjMgr->Get_NotTick();
}

HRESULT CGameInstance::Clear(_uint iLevelIndex)
{
	if (m_pObjMgr == nullptr)
		MSG_FAIL("CGameInstance Clear - m_pObjMgr is null");

	return m_pObjMgr->Clear(iLevelIndex);
}

HRESULT CGameInstance::Clear(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (m_pObjMgr == nullptr)
		MSG_FAIL("CGameInstance Clear - m_pObjMgr is null");

	return m_pObjMgr->Clear(iLevelIndex, pLayerTag);
}

HRESULT CGameInstance::Add_CollisionTag(_uint iFirstLevel, const _tchar* pFirstTag, _uint iSecondLevel, const _tchar* pSecondTag)
{
	if (m_pObjMgr == nullptr)
		MSG_FAIL("CGameInstance::Add_CollisionTag - m_pObjMgr == nullptr");

	COLLISION_TAG* pCollisionTag = new COLLISION_TAG;
	pCollisionTag->Set_CollisionValue(iFirstLevel, pFirstTag, iSecondLevel, pSecondTag);

	return m_pObjMgr->Add_CollisionTag(pCollisionTag);
}

HRESULT CGameInstance::Clear_CollisionTag()
{
	if (m_pObjMgr == nullptr)
		MSG_FAIL("CGameInstance::Clear_CollisionTag - m_pObjMgr == nullptr");

	return m_pObjMgr->Clear_CollisionTag();
}

void CGameInstance::Set_NotTick(_bool bNotTick)
{
	if (m_pObjMgr == nullptr)
		MSG_VOID("CGameInstance::Set_NotTick - m_pObjMgr == nullptr");

	return m_pObjMgr->Set_NotTick(bNotTick);
}

_bool CGameInstance::Key_Pressing(_ubyte byKeyID)
{
	if (m_pInputDevice == nullptr)
		MSG_FALSE("CGameInstance Key_Pressing - m_pInputDevice is null");

	return m_pInputDevice->Key_Pressing(byKeyID);
}

_bool CGameInstance::Key_Down(_ubyte byKeyID)
{
	if (m_pInputDevice == nullptr)
		MSG_FALSE("CGameInstance Key_Down - m_pInputDevice is null");

	return m_pInputDevice->Key_Down(byKeyID);
}

_bool CGameInstance::Key_Up(_ubyte byKeyID)
{
	if (m_pInputDevice == nullptr)
		MSG_FALSE("CGameInstance Key_Up - m_pInputDevice is null");

	return m_pInputDevice->Key_Up(byKeyID);
}

_bool CGameInstance::Mouse_Pressing(CInputDevice::DIMB eMouseButton)
{
	if (m_pInputDevice == nullptr)
		MSG_FALSE("CGameInstance Mouse_Pressing - m_pInputDevice is null");

	return m_pInputDevice->Mouse_Pressing(eMouseButton);
}

_bool CGameInstance::Mouse_Up(CInputDevice::DIMB eMouseButton)
{
	if (m_pInputDevice == nullptr)
		MSG_FALSE("CGameInstance Mouse_Up - m_pInputDevice is null");

	return m_pInputDevice->Mouse_Up(eMouseButton);
}

_bool CGameInstance::Mouse_Down(CInputDevice::DIMB eMouseButton)
{
	if (m_pInputDevice == nullptr)
		MSG_FALSE("CGameInstance Mouse_Down - m_pInputDevice is null");

	return m_pInputDevice->Mouse_Down(eMouseButton);
}

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	if (m_pInputDevice == nullptr)
		MSG_MINUSONE("CGameInstance Get_DIKeyState - m_pInputDevice is null");

	return m_pInputDevice->Get_DIKeyState(byKeyID);
}

_byte CGameInstance::Get_DIMouseButtonState(CInputDevice::DIMB eMouseButton)
{
	if (m_pInputDevice == nullptr)
		MSG_MINUSONE("CGameInstance Get_DIMouseButtonState - m_pInputDevice is null");

	return  m_pInputDevice->Get_DIMouseButtonState(eMouseButton);
}

_long CGameInstance::Get_DIMouseMoveState(CInputDevice::DIMM eMouseMove)
{
	if (m_pInputDevice == nullptr)
		MSG_MINUSONE("CGameInstance Get_DIMouseMoveState - m_pInputDevice is null");

	return m_pInputDevice->Get_DIMouseMoveState(eMouseMove);
}

HRESULT CGameInstance::Ready_CamMatrix()
{
	if (m_pCamMatrix == nullptr)
		MSG_FAIL("CGameInstance::Ready_CamMatrix - m_pCamMatrix == nullptr");

	return m_pCamMatrix->Set_Device(m_pGraphicDevice->Get_Device());
}

const _mat & CGameInstance::Get_CamWorld() const
{
	if (m_pCamMatrix == nullptr)
		MSGBOX("CGameInstance::Get_CamWorld - m_pCamMatrix == nullptr");

	return m_pCamMatrix->Get_CamWorld();
}

const _mat & CGameInstance::Get_CamView() const
{
	if (m_pCamMatrix == nullptr)
		MSGBOX("CGameInstance::Get_CamView - m_pCamMatrix == nullptr");

	return m_pCamMatrix->Get_CamView();
}

const _mat & CGameInstance::Get_CamProj() const
{
	if (m_pCamMatrix == nullptr)
		MSGBOX("CGameInstance::Get_CamProj - m_pCamMatrix == nullptr");

	return m_pCamMatrix->Get_CamProj();
}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst, HWND hWnd, _uint iNumLevels)
{
	if (m_pObjMgr == nullptr || m_pComponentMgr == nullptr || m_pInputDevice == nullptr)
	{
		MSGBOX("Initialize_Engine - Mgr is Nullptr");
		return E_FAIL;
	}

	m_iLevelMax = iNumLevels;

	if (FAILED(m_pObjMgr->Reserve_Container(iNumLevels)))
	{
		MSGBOX("Initialize_Engine - Reserve_Container Fail");
		return E_FAIL;
	}

	if (FAILED(m_pComponentMgr->Reserve_Container(iNumLevels)))
		MSG_FAIL("Initialize_Engine - Reserve_Container Fail");

	if (FAILED(m_pInputDevice->NativeConstruct(hInst, hWnd)))
		MSG_FAIL("CGameInstance Initialize_Engine - m_pInputDevice NativeConstruct");

	m_hWnd = hWnd;
	
	return S_OK;
}

HRESULT CGameInstance::Initialize_Camera(_uint iNumCameras)
{
	return m_pObjMgr->Reserve_CameraCnt(iNumCameras);
}

_int CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevelManager ||
		nullptr == m_pObjMgr || 
		nullptr == m_pInputDevice)
		MSG_MINUSONE("Tick_Engine - ObjMgr or LevelMgr or m_pInputDevice is Nullptr");

	_uint iLevel = Get_CurrentLevel();

	if (FAILED(m_pInputDevice->Update_InputDeviceState()))
		MSG_MINUSONE("CGameInstance Tick_Engine - m_pInputDevice Update_InputDeviceState");

	if (0 > m_pObjMgr->Camera_Tick(fTimeDelta))
		MSG_MINUSONE("CGameInstance Tick_Engine - m_pObjMgr Camera_Tick");

	//카메라 이동이 끝난 이후에 저장
	if (0 > m_pCamMatrix->Save_CamMatrix(fTimeDelta))
		MSG_MINUSONE("CGameInstance::Tick_Engine - m_pCamMatrix->Save_CamMatrix");

	//레벨 로딩일 경우 틱 안하기
	if (m_iLevelMax - 1 != iLevel)
		if (0 > m_pObjMgr->Tick(fTimeDelta))
			MSG_MINUSONE("CGameInstance Tick_Engine - m_pObjMgr Tick");

	if (FAILED(m_pPicking->PreConvert()))
		MSG_MINUSONE("CGameInstance Tick_Engine - m_pPicking PreConvert");

	if (FAILED(m_pObjMgr->Collision_Object(fTimeDelta)))
		MSG_MINUSONE("CGameInstance Tick_Engine - m_pObjMgr->Collision_Object ");

	//레벨 로딩일 경우 레이트 틱 안하기
	if (m_iLevelMax - 1 != iLevel)
		if (0 > m_pObjMgr->LateTick(fTimeDelta))
			MSG_MINUSONE("CGameInstance Tick_Engine - m_pObjMgr LateTick");

	if (0 > m_pObjMgr->Camera_LateTick(fTimeDelta))
		MSG_MINUSONE("CGameInstance Tick_Engine - m_pObjMgr Camera_LateTick");

	if (0 > m_pLevelManager->Tick(fTimeDelta))
		MSG_MINUSONE("CGameInstance Tick_Engine - m_pLevelManager Tick");

	return 0;
}

_int CGameInstance::Tick_ToolEngine(_float fTimeDelta)
{
	//순서 주의
	//키보드 마우스 초기화
	if (FAILED(m_pInputDevice->Update_InputDeviceState()))
		MSG_MINUSONE("CGameInstance Tick_Engine - m_pInputDevice Update_InputDeviceState");

	//오브젝트 틱(카메라 위치 변경)
	if (0 > m_pObjMgr->Tick(fTimeDelta))
		MSG_MINUSONE("CGameInstance Tick_Engine - m_pObjMgr Tick");

	if (FAILED(m_pPicking->PreConvert()))
		MSG_MINUSONE("CGameInstance Tick_Engine - m_pPicking PreConvert");

	if (0 > m_pObjMgr->LateTick(fTimeDelta))
		MSG_MINUSONE("CGameInstance Tick_Engine - m_pObjMgr LateTick");


	return 0;
}

void CGameInstance::Clear_Container(_uint iLevelIndex)
{
	if (m_pObjMgr == nullptr)
	{
		MSGBOX("Clear_Container - ObjMgr is Nullptr");
		return;
	}

	m_pObjMgr->Clear(iLevelIndex);
}

void CGameInstance::Release_Engine()
{
	//게임 인스턴스는 무조건 가장 앞에(Free에서 매니저들 지워주기 때문)
	if (0 != CGameInstance::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed To Release CGameInstance ");

	if(0 != CColliderRenderManager::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed To Release CColliderRenderManager");

	if (0 != CPicking::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed To Release CPicking ");

	if (0 != CCamMatrix::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed To Release CCamMatrix ");

	if (0 != CInputDevice::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed To Release InputDevice");



	//순서 주의
	
	if (0 != CLevel_Manager::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed To Release LevelManager ");

	if (0 != CTimer_Manager::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed To Release TimeManager ");

	if (0 != CObjectManager::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed To Release CObjectManager ");

	if (0 != CComponentManager::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed To Release CComponentManager ");

	if (0 != CGraphic_Device::Get_Instance()->Destroy_Instance())
		MSGBOX("Failed To Release CGraphic_Device ");
}

LAYERS CGameInstance::Get_CurrentLayers()
{
	_uint iIndex = m_pLevelManager->Get_CurrentLevel();
	return m_pObjMgr->Get_CurrentLayer(iIndex);
}

void CGameInstance::Set_ColliderRenderState(_bool bRender)
{
	m_pColliderRenderMgr->Set_ColliderRenderState(bRender);
}

HRESULT CGameInstance::Add_CameraToContainer(_uint iLevelIndex, const _tchar * pPrototypeTag, _uint iCameraLevel, void * pArg)
{
	return m_pObjMgr->Add_CameraToContainer(iLevelIndex,pPrototypeTag,iCameraLevel,pArg);
}

HRESULT CGameInstance::Camera_Clear()
{
	return m_pObjMgr->Camera_Clear();
}

HRESULT CGameInstance::Change_Camera(_uint iCameraLevel)
{
	return m_pObjMgr->Camera_Change(iCameraLevel);
}

CCamera * CGameInstance::Get_CurrentCamera()
{
	return m_pObjMgr->Get_CurrentCamera();
}

HRESULT CGameInstance::PreConvert()
{
	if (m_pPicking == nullptr)
		MSG_FAIL("CGameInstance PreConvert - m_pPicking is null");

	return m_pPicking->PreConvert();
}

_bool CGameInstance::Picking(const _tchar * szTransformTag, const _tchar * szBufferTag, CGameObject * pTarget, _vec3 * pOut)
{
	if (m_pPicking == nullptr)
		MSG_FALSE("CGameInstance Picking - m_pPicking is null");

	return m_pPicking->Picking(szTransformTag, szBufferTag, pTarget, pOut);
}

const _vec3 & CGameInstance::Get_MouseRay() const
{
	if (m_pPicking == nullptr)
		MSGBOX("CGameInstance::Get_MouseRay - m_pPicking == nullptr");

	return m_pPicking->Get_MouseRay();
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (m_pComponentMgr == nullptr)
		MSG_FAIL("CGameInstance - Add_Prototype");

	return m_pComponentMgr->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (m_pComponentMgr == nullptr)
		MSG_NULL("CGameInstance - Clone_Component");

	return m_pComponentMgr->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

HRESULT CGameInstance::Clear_ComponentContainer(_uint iNumLevels)
{
	if (m_pComponentMgr == nullptr)
		MSG_FAIL("CGameInstance - Clone_Component");

	return m_pComponentMgr->Clear_ComponentContainer(iNumLevels);
}

void CGameInstance::Free()
{
	Safe_Release(m_pCamMatrix);
	Safe_Release(m_pPicking);
	Safe_Release(m_pTimerManager);
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pLevelManager);
	Safe_Release(m_pObjMgr);
	Safe_Release(m_pComponentMgr);
	Safe_Release(m_pInputDevice);
	Safe_Release(m_pColliderRenderMgr);
}