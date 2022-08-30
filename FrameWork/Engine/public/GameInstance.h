#pragma once
#ifndef __ENGINE_GAME_INSTANCE_H__
#define __ENGINE_GAME_INSTANCE_H__

#include "ComponentManager.h"
#include "InputDevice.h"
#include "ObjectManager.h"
BEGIN(Engine)

class ENGINE_DLL CGameInstance final:
public CBase
{
	DECLARE_SINGLETON(CGameInstance)
	typedef unordered_map<const _tchar*, class CLayer*> LAYERS;
private:
	explicit CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT		Initialize_Engine(HINSTANCE hInst, HWND hWnd, _uint iNumLevels);
	HRESULT		Initialize_Camera(_uint iNumCameras);
	_int		Tick_Engine(_float fTimeDelta);
	_int		Tick_ToolEngine(_float fTimeDelta);
	void		Clear_Container(_uint iLevelIndex);
	static void Release_Engine();
	HWND		Get_Wnd() { return m_hWnd; }
	LAYERS		Get_CurrentLayers();

public: 
	void		Set_ColliderRenderState(_bool bRender);

public: // Camera
	HRESULT			Add_CameraToContainer(_uint iLevelIndex, const _tchar* pPrototypeTag, _uint iCameraLevel, void* pArg = nullptr);
	HRESULT			Camera_Clear();
	HRESULT			Change_Camera(_uint iCameraLevel);
	class CCamera*	Get_CurrentCamera();
	_uint			Get_CurrentCameraID() { return m_pObjMgr->Get_CurrentCameraID(); }
	
public: //Picking
	HRESULT PreConvert();
	_bool Picking(const _tchar* szTransformTag, const _tchar* szBufferTag, class CGameObject* pTarget, _vec3* pOut);
	const _vec3& Get_MouseRay() const;

public: //Component Manager
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);
	HRESULT		Clear_ComponentContainer(_uint iNumLevels);

public: //Timer Manager
	HRESULT Add_Timer(const _tchar* pTimerTag);
	_float Compute_Timer(const _tchar* pTimerTag);

public: //Graphic Device
	HRESULT Ready_Graphic_Device(HWND hWnd, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppOut);
	void Render_Begin();
	void Render_End(HWND hWND = nullptr);
	void Draw_Font(const _tchar * pText, D3DXCOLOR FontColor, _vec3* vPos = nullptr, _vec3 vScale = _vec3(0.5f, 1.f, 1.f));
	_pDevice Get_Device();
	LPD3DXLINE Get_Line();

public: //Level Manager
	HRESULT Change_Level(class CLevel* pLevel);
	HRESULT Render();
	_int	Get_CurrentLevel();

public: //GameObject Manager
	HRESULT Add_Prototype(const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObjectToLayer(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg = nullptr, void** ppOut = nullptr);
	HRESULT Add_GameObjectToLayer(_uint iLevelIndex, const _tchar* pLayerTag, CGameObject* pGameObj);
	HRESULT Clear(_uint iLevelIndex);
	HRESULT Clear(_uint iLevelIndex, const _tchar* pLayerTag);
	HRESULT Add_CollisionTag(_uint iFirstLevel, const _tchar* pFirstTag, _uint iSecondLevel, const _tchar* pSecondTag);
	HRESULT Clear_CollisionTag();
	void	Set_NotTick(_bool bNotTick);

	class CGameObject* Get_GameObjPtr(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndex);
	const _bool&		Get_NotTick() const;

	/* For.Input_Device */
public:
	_bool Key_Pressing(_ubyte byKeyID);
	_bool Key_Down(_ubyte byKeyID);
	_bool Key_Up(_ubyte byKeyID);

	_bool Mouse_Pressing(CInputDevice::DIMB eMouseButton);
	_bool Mouse_Up(CInputDevice::DIMB eMouseButton);
	_bool Mouse_Down(CInputDevice::DIMB eMouseButton);

	_byte Get_DIKeyState(_ubyte byKeyID);
	_byte Get_DIMouseButtonState(CInputDevice::DIMB eMouseButton);
	_long Get_DIMouseMoveState(CInputDevice::DIMM eMouseMove);

	/* For.CamMatrix */
public:
	HRESULT Ready_CamMatrix();
	const _mat& Get_CamWorld() const;
	const _mat& Get_CamView() const;
	const _mat& Get_CamProj() const;

private:
	class CTimer_Manager*			m_pTimerManager = nullptr;
	class CGraphic_Device*			m_pGraphicDevice = nullptr;
	class CLevel_Manager*			m_pLevelManager = nullptr;
	class CObjectManager*			m_pObjMgr = nullptr;
	class CComponentManager*		m_pComponentMgr = nullptr;
	class CInputDevice*				m_pInputDevice = nullptr;
	class CPicking*					m_pPicking = nullptr;
	class CCamMatrix*				m_pCamMatrix = nullptr;
	class CColliderRenderManager*	m_pColliderRenderMgr = nullptr;
	HWND							m_hWnd;

	_uint						m_iLevelMax;

public:
	virtual void Free() override;
};

END

#endif // !__ENGINE_GAME_INSTANCE_H__
