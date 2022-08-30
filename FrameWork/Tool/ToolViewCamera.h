#pragma once
#ifndef __TOOL_CAMERA_H__
#define __TOOL_CAMERA_H__

BEGIN(Engine)
class CTransform;
class CCamera;
END

#include "Camera.h"

class CToolViewCamera final : public CCamera
{
public:
	CToolViewCamera(LPDIRECT3DDEVICE9 pGraphicDevice);
	CToolViewCamera(const CToolViewCamera& rhs);
	~CToolViewCamera() = default;

public:
	virtual HRESULT				NativeConstruct_Prototype() override;
	virtual HRESULT				NativeConstruct(void* pArg) override;
	virtual _int				Tick(_float fTimeDelta) override;
	virtual _int				LateTick(_float fTimeDelta) override;
	HRESULT						SetUp_Componenet();
	HRESULT						SetUp_Initialize();
	HRESULT						Synchronize_CameraState();
	HRESULT						SetUp_InitialState(_vec3 vPos, _vec3 vLook);
	void						Setup_Form(class CForm* pForm) { m_pForm = pForm; }
public:
	void						Do_CameraAction() { m_bAction = !m_bAction; }
	bool						Get_CameraAction() { return m_bAction; }
	void						Sync_Window(HWND hWnd) { m_hViewWnd = hWnd; }
	_vec3						Get_CameraPos();
	_float						Get_CameraFov();
	HRESULT						Set_CamPos(const _vec3* vPos);
	HWND						m_hViewWnd;
	class CForm*				m_pForm = nullptr;
public:
	static	CToolViewCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual CGameObject*		Clone(void * pArg = nullptr) override;
	virtual void				Free() override;
private:
	bool					m_bAction = false;


	// CCamera을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;


	// CCamera을(를) 통해 상속됨
	virtual HRESULT Cam_SyncReset() override;

};


#endif // !__TOOL_CAMERA_H__
