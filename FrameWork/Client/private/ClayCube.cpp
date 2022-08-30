#include "stdafx.h"
#include "..\public\ClayCube.h"
#include "Player.h"
#include "Collision.h"
#include "CamMatrix.h"

CClayCube::CClayCube(_pDevice pGraphicDevice)
	: CTileCube(pGraphicDevice)
{
}

CClayCube::CClayCube(const CClayCube & rhs)
	: CTileCube(rhs)
{
}

HRESULT CClayCube::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CClayCube::NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CClayCube::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CClayCube::NativeConstruct - __super");

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CClayCube::NativeConstruct - SetUp_Componet");

	m_pTransform->SetUp_Scale(_vec3(3.f, 3.f, 3.f));
	m_pTransform->SetUp_State(CTransform::STATE_POS, _vec3(5.f, 5.f, 8.f));
	
	return S_OK;
}

HRESULT CClayCube::SetUp_Component()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransform)))
		MSG_FAIL("CClayCube SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRenderer)))
		MSG_FAIL("CClayCube SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeFrustum"), TEXT("ComFrustum"), (CComponent**)&m_pFrustum)))
		MSG_FAIL("CClayCube SetUp_Component - Add_Component m_pFrustumCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferCube"), TEXT("ComVIBuffer"), (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CClayCube SetUp_Component - Add_Component m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, TEXT("PrototypeTextureClay"), TEXT("ComTexture"), (CComponent**)&m_pTexture)))
		MSG_FAIL("CClayCube SetUp_Component - Add_Component m_pTextureCom");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice,_vec3(0.f,0.f,0.f),_vec3(1.f,1.f,1.f),D3DCOLOR_ARGB(255, 255, 255, 255),this);

	m_plane[CBoxCollider::PLANE_FRONT ] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_FRONT);
	m_plane[CBoxCollider::PLANE_BACK] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_BACK);
	m_plane[CBoxCollider::PLANE_LEFT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_LEFT);
	m_plane[CBoxCollider::PLANE_RIGHT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_RIGHT);
	m_plane[CBoxCollider::PLANE_TOP] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_TOP);
	m_plane[CBoxCollider::PLANE_BOTTOM] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_BOTTOM);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CClayCube::SetUp_Component(void * pArg)
{
	return S_OK;
}

CGameObject * CClayCube::Clone(void * pArg)
{
	CClayCube* pInstance = new CClayCube(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CClayCube Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CClayCube * CClayCube::Create(_pDevice pGraphicDevice)
{
	CClayCube* pInstance = new CClayCube(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CClayCube Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CClayCube::Free()
{
	__super::Free();
}

void CClayCube::Collision(CGameObject * pColObj)
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
		if (CCollision::CubeCollision(pPlayerObject, (_plane*)m_pBoxCollider->Get_PlaneDirArr(),&vResultPos, &bOntheCube,&bUndertheCube))
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
		if (CCollision::RectCollision(pPlayerObject, this,(_vec3*)m_pBoxCollider->Get_WorldPosArr() ,&vResultPos, &bOntheCube, &bUndertheCube))
		{
			if (pPlayerObject->Get_JumpUp())
				return;

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
