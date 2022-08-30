#include "stdafx.h"
#include "..\public\CollisionCubeObject.h"


CCollisionCubeObject::CCollisionCubeObject(_pDevice pDevice)
	: CCubeObject(pDevice)
{
}

CCollisionCubeObject::CCollisionCubeObject(const CCollisionCubeObject & rhs)
	: CCubeObject(rhs)
{
}

HRESULT CCollisionCubeObject::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CCollisionCubeObject NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CCollisionCubeObject::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CCollisionCubeObject NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CCollisionCubeObject::NativeConstruct - SetUp_Component");

	
	return S_OK;
}

_int CCollisionCubeObject::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CCollisionCubeObject Tick - __super");

	if (FAILED(__super::Tick_Mat(m_pBoxCollider)))
		MSG_MINUSONE("CCollisionCubeObject Tick - __super Tick_Mat");

	if (FAILED(m_pBoxCollider->Set_DefaultRotation()))
		MSG_MINUSONE("CCollisionCubeObject::Tick - m_pBoxCollider->Set_DefaultRotation");

	return 0;
}

_int CCollisionCubeObject::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CCollisionCubeObject LateTick - __super");

	return _int();
}

HRESULT CCollisionCubeObject::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CCollisionCubeObject Render - __super");

	if (m_pBoxCollider == nullptr)
		MSG_FAIL("CCollisionCubeObject Render - com is null");

	if (FAILED(m_pBoxCollider->Render()))
		MSG_FAIL("CCollisionCubeObject Render - m_pBoxCollider->Render");

	return S_OK;
}

HRESULT CCollisionCubeObject::Set_TransformState(CTransform::STATE eTransformState, const _vec3 & vState)
{
	return __super::Set_TransformState(eTransformState, vState);
}

HRESULT CCollisionCubeObject::Set_TransformScale(_vec3 vScale)
{
	return __super::Set_TransformScale(vScale);
}

HRESULT CCollisionCubeObject::Set_TransformScale(_float fX, _float fY, _float fZ)
{
	return __super::Set_TransformScale(fX, fY, fZ);
}

_float CCollisionCubeObject::Go_Straight(_float fTimeDelta)
{
	return __super::Go_Straight(fTimeDelta);
}

HRESULT CCollisionCubeObject::Rotation_Axis(const _vec3& vAxis, _float fTimeDelta)
{
	return __super::Rotation_Axis(vAxis, fTimeDelta);
}

HRESULT CCollisionCubeObject::Set_RotateSpeed(_float fRotateSpeed)
{
	return __super::Set_RotateSpeed(fRotateSpeed);
}

HRESULT CCollisionCubeObject::Set_Speed(_float fSpeed)
{
	return __super::Set_Speed(fSpeed);
}

_vec3 CCollisionCubeObject::Get_TransformState(CTransform::STATE eTransformState)
{
	return __super::Get_TransformState(eTransformState);
}

HRESULT CCollisionCubeObject::SetUp_Component(void * pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	OBJ_INFO tObjInfo = *(OBJ_INFO*)pArg;

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, tObjInfo.vColliderCenter, tObjInfo.vColliderSize, tObjInfo.dwColliderColor, this);
	m_mapComponent.emplace(pTagComCollider, m_pBoxCollider);
	if (m_pBoxCollider == nullptr)
		MSG_FAIL("CCollisionCubeObject::SetUp_Component - m_pBoxCollider == nullptr");

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CCollisionCubeObject * CCollisionCubeObject::Create(_pDevice pDevice)
{
	CCollisionCubeObject* pInstance = new CCollisionCubeObject(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CCollisionCubeObject Create Failed");
	}

	return pInstance;
}

CGameObject * CCollisionCubeObject::Clone(void * pArg)
{
	CCollisionCubeObject* pInstance = new CCollisionCubeObject(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CCollisionCubeObject Clone Failed");
	}

	return pInstance;
}

void CCollisionCubeObject::Free()
{
	Safe_Release(m_pBoxCollider);
	__super::Free();
}

_vec3 CCollisionCubeObject::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxCollider->Get_MinPos();
	return vMinPos;
}

_vec3 CCollisionCubeObject::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxCollider->Get_MaxPos();
	return vMaxPos;
}

_vec3 CCollisionCubeObject::Get_WorldPos()
{
	return m_pBoxCollider->Get_ColliderWolrdPos();
}

HRESULT CCollisionCubeObject::Tick_Collider(_float fTimeDelta)
{
	if (m_pBoxCollider == nullptr)
		MSG_FAIL("CCollisionCubeObject::Tick_Collider - m_pBoxCollider == nullptr");

	Set_BoxColliderTransform(m_pBoxCollider);
	m_pBoxCollider->Tick(fTimeDelta);

	return S_OK;

}
