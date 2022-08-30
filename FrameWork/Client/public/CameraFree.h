#pragma once
#ifndef __CLIENT_CAMERA_FREE_H__
#define __CLIENT_CAMERA_FREE_H__

#include "Camera.h"

BEGIN(Client)
class CCameraFree final : public CCamera
{
public:
	explicit CCameraFree(_pDevice pDevice);
	explicit CCameraFree(const CCameraFree& rhs);
	virtual ~CCameraFree() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Cam_SyncReset() override;
private:
	HRESULT SetUp_Component();

public:
	virtual void Free() override;
	static CCameraFree* Create(_pDevice pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;

	// CCamera을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};
END
#endif // !__CLIENT_CAMERA_FREE_H__
