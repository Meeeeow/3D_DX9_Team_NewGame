#pragma once
#ifndef __CLIENT_CAMERA_2D_H__
#define __CLIENT_CAMERA_2D_H__

#include "Camera.h"

BEGIN(Client)
class CCamera2D final : public CCamera
{
public:
	explicit CCamera2D(_pDevice pGraphicDevice);
	explicit CCamera2D(const CCamera2D& rhs);
	virtual ~CCamera2D() DEFAULT;

public:
	virtual	HRESULT	NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta)		override;
	virtual _int	LateTick(_float fTimeDelta)	override;

public:
	HRESULT			SetUp_Component();
	HRESULT			Set_TargetObject(CGameObject* pTargetObejct);
	HRESULT			Chase_Target(_float fTimeDelta);
	HRESULT			Synchronize_CameraInfo();
	virtual HRESULT Cam_SyncReset() override;
	void			Set_FixCam(_bool bCameraControl) { m_bSetFixCam = bCameraControl; }
private:
	CGameObject*	m_pTargetObject = nullptr;
	_bool			m_bSetFixCam = false;
public:
	// CCamera을(를) 통해 상속됨
	virtual _vec3			Get_MinPos() override;
	virtual _vec3			Get_MaxPos() override;
	virtual _vec3			Get_WorldPos() override;
	virtual _vec3			Get_Length() override;
	virtual CGameObject*	Clone(void * pArg = nullptr) override;
	static	CCamera2D*		Create(_pDevice pGraphicDevice);
	virtual void			Free() override;
};
END
#endif // !__CLIENT_CAMERA_2D_H__
