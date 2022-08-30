#include "stdafx.h"
#include "..\public\CameraTargetBack.h"

CCameraTargetBack::CCameraTargetBack(_pDevice pDevice)
	:CCamera(pDevice)
{
}

CCameraTargetBack::CCameraTargetBack(const CCameraTargetBack & rhs)
	:CCamera(rhs)
{
}

HRESULT CCameraTargetBack::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CCameraTargetBack::NativeConstruct_Prototype - __super::NativeConstruct_Prototype");

	return S_OK;
}

HRESULT CCameraTargetBack::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CCameraTargetBack NativeConstruct - __super");

	//컴포넌트 부착
	if (FAILED(SetUp_Component()))
		MSG_FAIL("CCameraTargetBack NativeConstruct - SetUp_Component");

	//카메라 시작값 세팅
	if (FAILED(CCamera::Synchronize_CameraState()))
		MSG_FAIL("CCameraTargetBack NativeConstruct - Synchronize_CameraState");

	//카메라 타겟 세팅(플레이어)
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CGameObject* pPlayer = pGameInstance->Get_GameObjPtr(LEVEL_STATIC, pTagLayerPlayer, 0);
	if (pPlayer == nullptr)
		MSG_FAIL("CCameraTargetBack::NativeConstruct - pPlayer == nullptr");

	if (FAILED(Set_Target(pPlayer)))
		MSG_FAIL("CCameraTargetBack NativeConstruct - Set_Target");

	//카메라 필요한 기본 정보들
	CAM_TARGET_BACK_DESC tCamTargetBackDesc;
	tCamTargetBackDesc.fDistance = 100.f;
	if (FAILED(Set_CamTargetBackDesc(tCamTargetBackDesc)))
		MSG_FAIL("CCameraTargetBack NativeConstruct - Set_Target");

	ShowCursor(FALSE);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

_int CCameraTargetBack::Tick(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSG_MINUSONE("CCameraTargetBack Tick - m_pTransform");

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	DO;

	if (pInstance->Get_NotTick())
		break;

	//debug 임시코드
	//Set_MatProject(g_iWinCX / _float(g_iWinCY), 45.f, 0.2f, 150.f);

	D3DVIEWPORT9		ViewPort;
	m_pGraphicDevice->GetViewport(&ViewPort);

	//POINT		ptMouse = { (LONG)ViewPort.Width >> 1, (LONG)ViewPort.Height >> 1 };
	//ClientToScreen(g_hWnd, &ptMouse);
	//SetCursorPos(ptMouse.x, ptMouse.y);


#ifdef _DEBUG
	//휠키 누르면 카메라 바뀜
	if (pInstance->Get_DIMouseButtonState(CInputDevice::DIMB::DIMB_WHEEL))
		m_bTargetCamera = !m_bTargetCamera;
#endif // _DEBUG

	//카메라 고정중에는 이동하지 않기
	if (m_bFixCam)
	{

	}
	//카메라 고정 해제
	else
	{
		if (m_bTargetCamera)
		{


			if (FAILED(Move_toTarget(fTimeDelta, pInstance)))
				MSG_FAIL("CCameraTargetBack::Tick - Move_toTarget");
		}
		else
		{
			if (FAILED(Move_Free(fTimeDelta)))
				MSG_FAIL("CCameraTargetBack::Tick - Move_Free");
			
			_long		MouseMove = 0;

			if (MouseMove = pInstance->Get_DIMouseMoveState(CInputDevice::DIMM_X))
				m_pTransform->Rotation_Axis(_vec3(0.f, 1.f, 0.f), fTimeDelta * MouseMove * 0.2f);

			if (MouseMove = pInstance->Get_DIMouseMoveState(CInputDevice::DIMM_Y))
				m_pTransform->Rotation_Axis(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.2f);
		}


	}

	WHILE;

	RELEASE_INSTANCE(CGameInstance);

	return __super::Tick(fTimeDelta);
}

_int CCameraTargetBack::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	return _int();
}

HRESULT CCameraTargetBack::Cam_SyncReset()
{
	return S_OK;
}

HRESULT CCameraTargetBack::Set_Target(CGameObject * pGameObj)
{
	if (pGameObj == nullptr)
		MSG_FAIL("CCameraTargetBack::Set_Target - pGameObj == nullptr");

	if (m_pTarget != nullptr)
		Safe_Release(m_pTarget);

	Safe_AddRef(pGameObj);
	m_pTarget = pGameObj;
	return S_OK;
}

HRESULT CCameraTargetBack::Set_CamTargetBackDesc(CAM_TARGET_BACK_DESC tCamTargetBackDesc)
{
	m_tCamTargetBackDesc = tCamTargetBackDesc;
	return S_OK;
}

void CCameraTargetBack::Move_Quake(_float fTimeDelta)
{
	_uint iRand1 = rand() % 3;
	_uint iRand2 = rand() % 3;

	_float fPositivePower = 0.15f;
	_float fNegativePower = -0.15f;
	fPositivePower *= iRand1;
	fNegativePower *= iRand2;

	m_pTransform->Add_Position(_vec3(fNegativePower, fPositivePower, fNegativePower));

	m_fQuakeTime -= fTimeDelta;
	if (m_fQuakeTime <= 0.f)
	{
		m_fQuakeTime = 1.f;
		m_bQuake = false;
	}
}

HRESULT CCameraTargetBack::SetUp_Component()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	CTransform::TRANSFORM_DESC tTransDesc{};
	tTransDesc.fDegree = 5.f;
	tTransDesc.fSpeed = 5.f;

	if (FAILED(CCamera::Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), &tTransDesc)))
		MSG_FAIL("CCameraTargetBack SetUp_Component - Add_Component ComTransform");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

//타겟의 트랜스폼을 받아 룩벡터를 이용해 각도를 조절하고 추적하는 함수
HRESULT CCameraTargetBack::Move_toTarget(_float fTimeDelta, CGameInstance* pGameInstance)
{
	if (m_pTarget == nullptr)
		MSG_FAIL("CCameraTargetBack::Move_toTarget - m_pTarget == nullptr");

	CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pTarget->Get_Component(L"ComTransform"));
	if (pTargetTransform == nullptr)
		MSG_FAIL("CCameraTargetBack::Move_toTarget - pTargetTransform == nullptr");
	_vec3 vLook = pTargetTransform->Get_State(CTransform::STATE_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);
	m_pTransform->SetUp_LookOrtho(vLook);


	m_dwMouseYPos += pGameInstance->Get_DIMouseMoveState(CInputDevice::DIMM_Y);
	m_pTransform->Rotation_Axis(m_pTransform->Get_State(CTransform::STATE_RIGHT), 0.01f * m_dwMouseYPos);

	_vec3 vTargetPos = pTargetTransform->Get_State(CTransform::STATE_POS);
	vTargetPos.y += 1.f;
	m_pTransform->SetUp_State(CTransform::STATE_POS, vTargetPos);
	_float fBackDistance;
	if (m_dwMouseYPos < 0)
		fBackDistance = 50.f + m_dwMouseYPos * 0.05f;
	else
		fBackDistance = 50.f;

	m_pTransform->Go_Backward(0.02f * fBackDistance);
	if (m_bQuake)
		Move_Quake(fTimeDelta);
	return S_OK;
}

HRESULT CCameraTargetBack::Move_Free(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Key_Pressing(DIK_UP))
		m_pTransform->Go_Straight(fTimeDelta);

	if (pGameInstance->Key_Pressing(DIK_DOWN))
		m_pTransform->Go_Backward(fTimeDelta);

	if (pGameInstance->Key_Pressing(DIK_LEFT))
		m_pTransform->Go_Left(fTimeDelta);

	if (pGameInstance->Key_Pressing(DIK_RIGHT))
		m_pTransform->Go_Right(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CGameObject * CCameraTargetBack::Clone(void * pArg /*= nullptr*/)
{
	CGameObject* pInstance = new CCameraTargetBack(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CCameraTargetBack Clone Failed");
	}

	return pInstance;
}

CCameraTargetBack * CCameraTargetBack::Create(_pDevice pDevice)
{
	CCameraTargetBack* pInstance = new CCameraTargetBack(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CCameraTargetBack Create Failed");
	}

	return pInstance;
}

_vec3 CCameraTargetBack::Get_MinPos()
{
	return _vec3();
}

_vec3 CCameraTargetBack::Get_MaxPos()
{
	return _vec3();
}

_vec3 CCameraTargetBack::Get_WorldPos()
{
	return _vec3();
}

_vec3 CCameraTargetBack::Get_Length()
{
	return _vec3();
}



void CCameraTargetBack::Free()
{
	Safe_Release(m_pTarget);
	__super::Free();
}