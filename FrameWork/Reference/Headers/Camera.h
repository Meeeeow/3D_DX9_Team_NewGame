#pragma once
#ifndef __ENGINE_CAMERA_H__
#define __ENGINE_CAMERA_H__

#include "GameObject.h"
BEGIN(Engine)
class ENGINE_DLL CCamera abstract:
	public CGameObject
{
public:
	typedef struct tagInitCamera
	{
		_vec3 vEye = _vec3(0.f, 0.f, 0.f);
		_vec3 vAt = _vec3(0.f, 0.f, 0.f);
		_vec3 vAxis = _vec3(0.f, 1.f, 0.f);

		_float fAspect = 1.f;
		_float fFovY = 45.f;
		_float fNear = 0.2f;
		_float fFar = 300.f;
	}INIT_CAMERA;
public:
	explicit CCamera(_pDevice pDevice);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;

protected:
	class CTransform*	m_pTransform = nullptr;
	INIT_CAMERA			m_tInitCam{};

public:
	void Set_MatProject(_float fAspect, _float fFovY, _float fNear, _float fFar);

protected:
	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, void* pArg);
	HRESULT Synchronize_CameraState();

public:
	virtual HRESULT Cam_SyncReset() PURE;

public:
	virtual void Free() override;
	virtual CGameObject * Clone(void * pArg = nullptr)PURE;
};
END
#endif // !__ENGINE_CAMERA_H__
