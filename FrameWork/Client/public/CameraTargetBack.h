#pragma once
#ifndef __CLIENT_CAMERA_TARGET_BACK_H__
#define __CLIENT_CAMERA_TARGET_BACK_H__

#include "Camera.h"
class CCameraTargetBack final:
	public CCamera
{
public:
	typedef struct tagCameraTargetBackDescription {
		tagCameraTargetBackDescription()
		{
			ZeroMemory(this, sizeof(*this));
		}

		float fDistance;
	}CAM_TARGET_BACK_DESC;
private:
	explicit CCameraTargetBack(_pDevice pDevice);
	explicit CCameraTargetBack(const CCameraTargetBack& rhs);
	virtual ~CCameraTargetBack() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Cam_SyncReset() override;
public: //SET
	//�ֵ� ���۷���, ������ ���۷��� �� ����, ī�޶� Ÿ�� ����
	HRESULT Set_Target(CGameObject* pGameObj);
	//���� ī�޶� �ʿ��� ���� ����
	HRESULT Set_CamTargetBackDesc(CAM_TARGET_BACK_DESC tCamTargetBackDesc);
	//ī�޶� ���� ��Ű�� Ǯ��
	void Set_FixCam(bool bFix) { m_bFixCam = bFix; }
	void Set_Quake() { m_bQuake = true; }
	void Move_Quake(_float fTimeDelta);
public: //GET

private:
	HRESULT SetUp_Component();

	//Tick
	HRESULT Move_toTarget(_float fTimeDelta, CGameInstance* pGameInstance);
	HRESULT Move_Free(_float fTimeDelta);

private:
	CGameObject*	m_pTarget = nullptr;
	CAM_TARGET_BACK_DESC		m_tCamTargetBackDesc;

	_bool			m_bTargetCamera = true;
	_bool			m_bFixCam = false;
	_bool			m_bQuake = false;

	_float			m_fQuakeTime = 1.f;

	_long			m_dwMouseYPos = 0;

public:
	virtual void Free() override;
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CCameraTargetBack* Create(_pDevice pDevice);

	// CCamera��(��) ���� ��ӵ�
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};

#endif // !__CLIENT_CAMERA_TARGET_BACK_H__
