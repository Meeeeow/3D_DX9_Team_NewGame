#include "stdafx.h"
#include "..\public\CameraFree.h"
#include "GameInstance.h"

void CCameraFree::Free()
{
	__super::Free();
}

CCameraFree * CCameraFree::Create(_pDevice pDevice)
{
	CCameraFree* pInstance = new CCameraFree(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CCameraFree Create Failed");
	}

	return pInstance;
}

CGameObject * CCameraFree::Clone(void * pArg /*= nullptr*/)
{
	CGameObject* pInstance = new CCameraFree(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CCameraFree Clone Failed");
	}

	return pInstance;
}

_vec3 CCameraFree::Get_MinPos()
{
	return _vec3();
}

_vec3 CCameraFree::Get_MaxPos()
{
	return _vec3();
}

_vec3 CCameraFree::Get_WorldPos()
{
	return _vec3();
}

_vec3 CCameraFree::Get_Length()
{
	return _vec3();
}

CCameraFree::CCameraFree(_pDevice pDevice)
	:CCamera(pDevice)
{
}

CCameraFree::CCameraFree(const CCameraFree & rhs)
	:CCamera(rhs)
{
}

HRESULT CCameraFree::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CCameraFree NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CCameraFree::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CCameraFree NativeConstruct - __super");

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CCameraFree NativeConstruct - SetUp_Component");

	if (FAILED(CCamera::Synchronize_CameraState()))
		MSG_FAIL("CCameraFree NativeConstruct - Synchronize_CameraState");

	return S_OK;
}

_int CCameraFree::Tick(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSG_MINUSONE("CCameraFree Tick - m_pTransform");

	D3DVIEWPORT9		ViewPort;
	m_pGraphicDevice->GetViewport(&ViewPort);

	/*POINT		ptMouse = { (LONG)ViewPort.Width >> 1, (LONG)ViewPort.Height >> 1 };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);*/

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	if (pInstance->Key_Pressing(DIK_UP))
		m_pTransform->Go_Straight(fTimeDelta);

	if (pInstance->Key_Pressing(DIK_DOWN))
		m_pTransform->Go_Backward(fTimeDelta);

	if (pInstance->Key_Pressing(DIK_LEFT))
		m_pTransform->Go_Left(fTimeDelta);

	if (pInstance->Key_Pressing(DIK_RIGHT))
		m_pTransform->Go_Right(fTimeDelta);

	_long		MouseMove = 0;

	if (MouseMove = pInstance->Get_DIMouseMoveState(CInputDevice::DIMM_X))
		m_pTransform->Rotation_Axis(_vec3(0.f, 1.f, 0.f), fTimeDelta * MouseMove * 0.2f);

	if (MouseMove = pInstance->Get_DIMouseMoveState(CInputDevice::DIMM_Y))
		m_pTransform->Rotation_Axis(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.2f);


	RELEASE_INSTANCE(CGameInstance);

	return __super::Tick(fTimeDelta);
}

_int CCameraFree::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	return _int();
}

HRESULT CCameraFree::Cam_SyncReset()
{
	return S_OK;
}

HRESULT CCameraFree::SetUp_Component()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	CTransform::TRANSFORM_DESC tTransDesc{};
	tTransDesc.fDegree = 5.f;
	tTransDesc.fSpeed = 5.f;
	
	if(FAILED(CCamera::Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), &tTransDesc)))
		MSG_FAIL("CCameraFree SetUp_Component - Add_Component ComTransform");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}
