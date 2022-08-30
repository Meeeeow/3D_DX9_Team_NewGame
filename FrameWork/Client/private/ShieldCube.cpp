#include "stdafx.h"
#include "..\public\ShieldCube.h"
#include "Player.h"
#include "Collision.h"
#include "CamMatrix.h"


CShieldCube::CShieldCube(_pDevice pGraphicDevice)
	: CTileCube(pGraphicDevice)
{
}

CShieldCube::CShieldCube(const CShieldCube & rhs)
	: CTileCube(rhs)
{
}

HRESULT CShieldCube::Render()
{
	if (m_pRenderer == nullptr)
		MSG_FAIL("CShieldCube Render - m_pRendererCom is null");

	if (m_pVIBuffer == nullptr)
		MSG_FAIL("CShieldCube Render - m_pVIBufferCom is null");

	if (FAILED(m_pTransform->Bind_OnDevice()))
		MSG_FAIL("CShieldCube Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pBoxCollider->Render()))
		MSG_FAIL("CShieldCube Render - m_pBoxCollider Render");


	return S_OK;
}

HRESULT CShieldCube::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CShieldCube::NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CShieldCube::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CShieldCube::NativeConstruct - __super");

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CShieldCube::NativeConstruct - SetUp_Componet");

	OBJ_INFO* pObj = (OBJ_INFO*)pArg;

	m_pTransform->SetUp_Scale(pObj->Get_Scale());
	m_pTransform->SetUp_State(CTransform::STATE_POS, pObj->Get_State(OBJ_INFO::STATE_POS));

	return S_OK;
}

HRESULT CShieldCube::SetUp_Component()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransform)))
		MSG_FAIL("CShieldCube SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRenderer)))
		MSG_FAIL("CShieldCube SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeFrustum"), TEXT("ComFrustum"), (CComponent**)&m_pFrustum)))
		MSG_FAIL("CShieldCube SetUp_Component - Add_Component m_pFrustumCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferCube"), TEXT("ComVIBuffer"), (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CShieldCube SetUp_Component - Add_Component m_pVIBufferCom");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, _vec3(0.f, 0.f, 0.f), _vec3(1.f, 1.f, 1.f), D3DCOLOR_ARGB(255, 255, 255, 255), this);

	m_plane[CBoxCollider::PLANE_FRONT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_FRONT);
	m_plane[CBoxCollider::PLANE_BACK] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_BACK);
	m_plane[CBoxCollider::PLANE_LEFT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_LEFT);
	m_plane[CBoxCollider::PLANE_RIGHT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_RIGHT);
	m_plane[CBoxCollider::PLANE_TOP] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_TOP);
	m_plane[CBoxCollider::PLANE_BOTTOM] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_BOTTOM);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CGameObject * CShieldCube::Clone(void * pArg)
{
	CShieldCube* pInstance = new CShieldCube(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CClayCube Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CShieldCube * CShieldCube::Create(_pDevice pGraphicDevice)
{
	CShieldCube* pInstance = new CShieldCube(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CClayCube Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShieldCube::Free()
{
	__super::Free();
}

void CShieldCube::Collision(CGameObject * pColObj)
{
	CPlayer* pPlayerObject = dynamic_cast<CPlayer*>(pColObj);
	if (pPlayerObject == nullptr)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_uint eCurrentCamera = pGameInstance->Get_CurrentCameraID();
	RELEASE_INSTANCE(CGameInstance);


	_vec3 vResultPos = { 0.f,0.f,0.f };
	_bool bOntheCube = false;
	_bool bUndertheCube = false;
	if (eCurrentCamera == CAMERA_BACK)
	{
		if (CCollision::CubeCollision(pPlayerObject, (_plane*)m_pBoxCollider->Get_PlaneDirArr(), &vResultPos, &bOntheCube, &bUndertheCube))
		{
			pPlayerObject->Set_Transform(vResultPos);
			if (bOntheCube)
				pPlayerObject->On_theCube();
			if (bUndertheCube)
				pPlayerObject->Under_theCube();
		}
	}
	else if (eCurrentCamera == CAMERA_2D)
	{
		if (CCollision::RectCollision(pPlayerObject, this, (_vec3*)m_pBoxCollider->Get_WorldPosArr(), &vResultPos, &bOntheCube, &bUndertheCube))
		{
			pPlayerObject->Set_Transform(vResultPos);
			if (bOntheCube)
				pPlayerObject->On_theCube();
			if (bUndertheCube)
				pPlayerObject->Under_theCube();
		}
	}
	else
		return;
}
