#include "stdafx.h"
#include "ToolViewCamera.h"
#include "Form.h"

CToolViewCamera::CToolViewCamera(LPDIRECT3DDEVICE9 pGraphicDevice)
	: CCamera(pGraphicDevice)
{
}

CToolViewCamera::CToolViewCamera(const CToolViewCamera & rhs)
	: CCamera(rhs)
{
}

HRESULT CToolViewCamera::NativeConstruct_Prototype()
{
	if (FAILED(SetUp_Componenet()))
		MSG_FAIL("CToolViewCamera SetUp_Componenet");
	if (FAILED(SetUp_Initialize()))
		MSG_FAIL("CToolViewCamera SetUp_Initialize");
	if (FAILED(Synchronize_CameraState()))
		MSG_FAIL("CToolViewCamera Synchronize_CameraState");

	return S_OK;
}

HRESULT CToolViewCamera::NativeConstruct(void * pArg)
{
	if (FAILED(SetUp_Componenet()))
		MSG_FAIL("CToolViewCamera SetUp_Componenet");
	if (FAILED(SetUp_Initialize()))
		MSG_FAIL("CToolViewCamera SetUp_Initialize");
	if (FAILED(Synchronize_CameraState()))
		MSG_FAIL("CToolViewCamera Synchronize_CameraState");

	return S_OK;
}

_int CToolViewCamera::Tick(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		return -1;

	if (m_bAction)
	{
		D3DVIEWPORT9 tViewPort;
		m_pGraphicDevice->GetViewport(&tViewPort);
		POINT pt = { (LONG)tViewPort.Width >> 1, (LONG)tViewPort.Height >> 1 };
		ClientToScreen(m_hViewWnd, &pt);
		SetCursorPos(pt.x, pt.y);

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		

		if (pGameInstance->Key_Pressing(DIK_UPARROW))
			m_pTransform->Go_Straight(fTimeDelta);
		if (pGameInstance->Key_Pressing(DIK_LEFTARROW))
			m_pTransform->Go_Left(fTimeDelta);
		if (pGameInstance->Key_Pressing(DIK_DOWNARROW))
			m_pTransform->Go_Backward(fTimeDelta);
		if (pGameInstance->Key_Pressing(DIK_RIGHTARROW))
			m_pTransform->Go_Right(fTimeDelta);

		if (pGameInstance->Key_Pressing(DIK_4))
			pGameInstance->Set_ColliderRenderState(true);
		if (pGameInstance->Key_Pressing(DIK_5))
			pGameInstance->Set_ColliderRenderState(false);
		if (pGameInstance->Key_Down(DIK_LSHIFT))
			m_pTransform->Set_Speed(30.f);
		if (pGameInstance->Key_Up(DIK_LSHIFT))
			m_pTransform->Set_Speed(15.f);

		_long Mouse = 0;

		if (Mouse = pGameInstance->Get_DIMouseMoveState(CInputDevice::DIMM_X))
			m_pTransform->Rotation_Axis(_vec3(0.f, 1.f, 0.f), fTimeDelta * Mouse * 0.1f);

		if (Mouse = pGameInstance->Get_DIMouseMoveState(CInputDevice::DIMM_Y))
			m_pTransform->Rotation_Axis(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * Mouse * 0.1f);

		if (Mouse = pGameInstance->Get_DIMouseMoveState(CInputDevice::DIMM_WHEEL))
		{
			m_tInitCam.fFovY += Mouse / 90.f;
		}

		if (m_pForm != nullptr)
			m_pForm->Set_CamPosToScreen();
	}

	return __super::Tick(fTimeDelta);
}

_int CToolViewCamera::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	return _int();
}

HRESULT CToolViewCamera::SetUp_Componenet()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	CTransform::TRANSFORM_DESC tTransDesc{};
	tTransDesc.fDegree = 5.f;
	tTransDesc.fSpeed = 15.f;

	if (FAILED(CCamera::Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), &tTransDesc)))
		MSG_FAIL("CToolViewCamera SetUp_Component - Add_Component ComTransform");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CToolViewCamera::SetUp_Initialize()
{
	m_bAction = false;
	m_tInitCam.vEye = { 0.f, 0.f, -1.f };
	m_tInitCam.vAt = { 0.f,0.f,0.f };
	m_tInitCam.fFovY = 90.f;
	m_tInitCam.fAspect = (_float)g_iWinCX / (_float)g_iWinCY;
	m_tInitCam.fNear = 0.1f;
	m_tInitCam.fFar = 500.f;
	m_tInitCam.vAxis = { 0.f , 1.f , 0.f };
	return S_OK;
}

HRESULT CToolViewCamera::Synchronize_CameraState()
{
	_vec3 vLook = m_tInitCam.vAt - m_tInitCam.vEye;
	D3DXVec3Normalize(&vLook, &vLook);
	m_pTransform->SetUp_State(CTransform::STATE::STATE_LOOK, vLook);

	_vec3 vRight = *D3DXVec3Cross(&vRight, &_vec3(0.f, 1.f, 0.f), &vLook);
	m_pTransform->SetUp_State(CTransform::STATE::STATE_RIGHT, vRight);

	_vec3 vUp = *D3DXVec3Cross(&vUp, &vLook, &vRight);
	m_pTransform->SetUp_State(CTransform::STATE::STATE_UP, vUp);

	m_pTransform->SetUp_State(CTransform::STATE::STATE_POS, m_tInitCam.vEye);

	return S_OK;
}

HRESULT CToolViewCamera::SetUp_InitialState(_vec3 vPos, _vec3 vAt)
{
	m_pTransform->SetUp_State(CTransform::STATE_POS, vPos);
	
	_vec3 vLook = vAt - vPos;
	D3DXVec3Normalize(&vLook, &vLook);
	m_pTransform->SetUp_State(CTransform::STATE_LOOK, vLook);

	_vec3 vRight;
	D3DXVec3Cross(&vRight, &_vec3(0.f, 1.f, 0.f), &vLook);
	m_pTransform->SetUp_State(CTransform::STATE_RIGHT, vRight);

	_vec3 vUp;
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	m_pTransform->SetUp_State(CTransform::STATE_UP, vUp);

	return S_OK;
}

_vec3 CToolViewCamera::Get_CameraPos()
{
	return m_pTransform->Get_State(CTransform::STATE_POS);
}

_float CToolViewCamera::Get_CameraFov()
{
	return m_tInitCam.fFovY;
}

HRESULT CToolViewCamera::Set_CamPos(const _vec3 * vPos)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CToolViewCamera::Set_CamPos - m_pTransform == nullptr");

	m_pTransform->SetUp_State(CTransform::STATE_POS, *vPos);
	return S_OK;
}

CToolViewCamera * CToolViewCamera::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CToolViewCamera* pInstance = new CToolViewCamera(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CToolViewCamera Create - NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CToolViewCamera::Clone(void * pArg)
{
	CToolViewCamera* pInstance = new CToolViewCamera(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CToolViewCamera Clone - NativeConstruct");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CToolViewCamera::Free()
{
	Safe_Release(m_pTransform);
	__super::Free();
}

_vec3 CToolViewCamera::Get_MinPos()
{
	return _vec3();
}

_vec3 CToolViewCamera::Get_MaxPos()
{
	return _vec3();
}

_vec3 CToolViewCamera::Get_WorldPos()
{
	return _vec3();
}

_vec3 CToolViewCamera::Get_Length()
{
	return _vec3();
}

HRESULT CToolViewCamera::Cam_SyncReset()
{
	return E_NOTIMPL;
}
