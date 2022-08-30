#include "stdafx.h"
#include "..\public\TempleCube.h"
#include "Collision.h"
#include "Player.h"


CTempleCube::CTempleCube(_pDevice pGraphicDevice)
	: CTileCube(pGraphicDevice)
{
}

CTempleCube::CTempleCube(const CTempleCube & rhs)
	: CTileCube(rhs)
{
}

HRESULT CTempleCube::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CTempleCube::NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CTempleCube::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CTempleCube::NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CTempleCube::NativeConstruct - SetUpComponent");


	return S_OK;
}

HRESULT CTempleCube::SetUp_Component(void * pArg)
{
	OBJ_INFO* pObjInfo = (OBJ_INFO*)pArg;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransform)))
		MSG_FAIL("CGrayBlock SetUp_Component - Add_Component m_pTransformCom");
	m_pTransform->Set_Matrix(pObjInfo->matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRenderer)))
		MSG_FAIL("CGrayBlock SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeFrustum"), TEXT("ComFrustum"), (CComponent**)&m_pFrustum)))
		MSG_FAIL("CGrayBlock SetUp_Component - Add_Component m_pFrustumCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferCube"), TEXT("ComVIBuffer"), (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CGrayBlock SetUp_Component - Add_Component m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, pTagPrototypeTextureTempleCube, TEXT("ComTexture"), (CComponent**)&m_pTexture)))
		MSG_FAIL("CGrayBlock SetUp_Component - Add_Component m_pTextureCom");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, pObjInfo->vColliderCenter, pObjInfo->vColliderSize, pObjInfo->dwColliderColor, this);

	m_fFrustumRadius = 14.f;
	m_plane[CBoxCollider::PLANE_FRONT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_FRONT);
	m_plane[CBoxCollider::PLANE_BACK] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_BACK);
	m_plane[CBoxCollider::PLANE_LEFT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_LEFT);
	m_plane[CBoxCollider::PLANE_RIGHT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_RIGHT);
	m_plane[CBoxCollider::PLANE_TOP] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_TOP);
	m_plane[CBoxCollider::PLANE_BOTTOM] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_BOTTOM);

	return S_OK;
}

CGameObject * CTempleCube::Clone(void * pArg)
{
	CTempleCube* pInstance = new CTempleCube(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CTempleCube::Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTempleCube::Collision(CGameObject * pColObj)
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
		if (CCollision::CubeCollision(pPlayerObject, (_plane*)m_pBoxCollider->Get_PlaneDirArr(), &vResultPos, &bOntheCube, &bUndertheCube, _vec3(0.f, 0.5f, 0.f)))
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

CTempleCube * CTempleCube::Create(_pDevice pGraphicDevice)
{
	CTempleCube* pInstance = new CTempleCube(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CTempleCube::Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTempleCube::Free()
{
	__super::Free();
}
