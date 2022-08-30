#include "stdafx.h"
#include "..\public\Camera2D.h"

CCamera2D::CCamera2D(_pDevice pGraphicDevice)
	: CCamera(pGraphicDevice)
{
}

CCamera2D::CCamera2D(const CCamera2D & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera2D::NativeConstruct_Prototype()
{
	if (__super::NativeConstruct_Prototype())
		MSG_FAIL("CCamera2D NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CCamera2D::NativeConstruct(void * pArg)
{
	if(__super::NativeConstruct(pArg))
		MSG_FAIL("CCamera2D NativeConstruct - __super");

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CCamera2D NativeConstruct - SetUp_Component");

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CGameObject* pTargetObject = pGameInstance->Get_GameObjPtr(LEVEL_STATIC, pTagLayerPlayer, 0);
	if(FAILED(Set_TargetObject(pTargetObject)))
		MSG_FAIL("CCamera2D NativeConstruct - Set_TargetObject");

	if(FAILED(Synchronize_CameraInfo()))
		MSG_FAIL("CCamera2D NativeConstruct - Synchronize_CameraInfo");

	RELEASE_INSTANCE(CGameInstance);

	ShowCursor(FALSE);

	return S_OK;
}

_int CCamera2D::Tick(_float fTimeDelta)
{
	Chase_Target(fTimeDelta);
		

	return __super::Tick(fTimeDelta);
}

_int CCamera2D::LateTick(_float fTimeDelta)
{

	return __super::LateTick(fTimeDelta);
}

HRESULT CCamera2D::SetUp_Component()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTransform::TRANSFORM_DESC tTransformDesc{};
	tTransformDesc.fDegree = 1.f;
	tTransformDesc.fSpeed = 1.f;

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, &tTransformDesc)))
		MSG_FAIL("CCamera2D SetUp_Component - Add_Component");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CCamera2D::Set_TargetObject(CGameObject * pTargetObejct)
{
	if (pTargetObejct == nullptr)
		MSG_FAIL("CCamera2D Set_TargetObject - pTargetObejct is nullptr");

	if (m_pTargetObject != nullptr)
		Safe_Release(m_pTargetObject);

	m_pTargetObject = pTargetObejct;
	Safe_AddRef(pTargetObejct);

	return S_OK;
}

HRESULT CCamera2D::Chase_Target(_float fTimeDelta)
{
	if (m_pTargetObject == nullptr)
		MSG_FAIL("CCamera2D Chase_Target - m_pTargetObject is nullptr");

	CTransform* pTargetObjectsTransformCom = dynamic_cast<CTransform*>(m_pTargetObject->Get_Component(pTagComTransform));

	if(pTargetObjectsTransformCom == nullptr)
		MSG_FAIL("CCamera2D Chase_Target - pTargetObjectsTransformCom is nullptr");

	_vec3 vTargetPos = pTargetObjectsTransformCom->Get_State(CTransform::STATE_POS);
	_vec3 vCameraPos = m_pTransform->Get_State(CTransform::STATE_POS);

	vCameraPos.x = vTargetPos.x + 4;
	
	m_pTransform->SetUp_State(CTransform::STATE_POS, vCameraPos);

	return S_OK;
}

HRESULT CCamera2D::Synchronize_CameraInfo()
{
	if (m_pTargetObject == nullptr)
		MSG_FAIL("CCamera2D Synchronize_CameraInfo - m_pTargetObject is nullptr");

	CTransform* pTargetObjectsTransformCom = dynamic_cast<CTransform*>(m_pTargetObject->Get_Component(pTagComTransform));

	if (pTargetObjectsTransformCom == nullptr)
		MSG_FAIL("CCamera2D Chase_Target - pTargetObjectsTransformCom is nullptr");

	_vec3 vTargetPos = pTargetObjectsTransformCom->Get_State(CTransform::STATE_POS);
	_vec3 vCameraPos = { vTargetPos.x + 4 , vTargetPos.y + 4, vTargetPos.z - 25 };
	vTargetPos.x = vCameraPos.x;
	vTargetPos.y = vCameraPos.y;

	_vec3 vLook		= vTargetPos - vCameraPos;
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vRight	= *D3DXVec3Cross(&vRight, &_vec3(0.f, 1.f, 0.f), &vLook);
	_vec3 vUp		= *D3DXVec3Cross(&vUp, &vLook, &vRight);


	m_pTransform->SetUp_State(CTransform::STATE_LOOK, vLook);
	m_pTransform->SetUp_State(CTransform::STATE_RIGHT, vRight);
	m_pTransform->SetUp_State(CTransform::STATE_UP, vUp);
	m_pTransform->SetUp_State(CTransform::STATE_POS, vCameraPos);


	
	m_tInitCam.fFovY = 35.f;
	m_tInitCam.fFar = 200.f;
	m_tInitCam.fNear = 0.2f;
	m_tInitCam.fAspect = g_iWinCX / (_float)g_iWinCY;

	return S_OK;
}

HRESULT CCamera2D::Cam_SyncReset()
{
	if (FAILED(Synchronize_CameraInfo()))
		MSG_FAIL("Camera Reset Failed");

	return S_OK;
}


_vec3 CCamera2D::Get_MinPos()
{
	return _vec3();
}

_vec3 CCamera2D::Get_MaxPos()
{
	return _vec3();
}

_vec3 CCamera2D::Get_WorldPos()
{
	return _vec3();
}

_vec3 CCamera2D::Get_Length()
{
	return _vec3();
}

CGameObject * CCamera2D::Clone(void * pArg)
{
	CGameObject* pInstance = new CCamera2D(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CCamera2D Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CCamera2D * CCamera2D::Create(_pDevice pGraphicDevice)
{
	CCamera2D* pInstance = new CCamera2D(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCamera2D Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera2D::Free()
{
	Safe_Release(m_pTargetObject);
	__super::Free();
}
