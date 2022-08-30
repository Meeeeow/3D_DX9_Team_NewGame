#include "stdafx.h"
#include "..\public\GrayBlock.h"
#include "Collision.h"
#include "Player.h"



CGrayBlock::CGrayBlock(_pDevice pGraphicDevice)
	: CTileCube(pGraphicDevice)
{
}

CGrayBlock::CGrayBlock(const CGrayBlock & rhs)
	: CTileCube(rhs)
{
}

HRESULT CGrayBlock::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL(";");

	return S_OK;
}

HRESULT CGrayBlock::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL(";;");

	if (pArg == nullptr)
		SetUp_Component();
	else
		SetUp_Component(pArg);

	return S_OK;
}

HRESULT CGrayBlock::SetUp_Component()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransform)))
		MSG_FAIL("CGrayBlock SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRenderer)))
		MSG_FAIL("CGrayBlock SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeFrustum"), TEXT("ComFrustum"), (CComponent**)&m_pFrustum)))
		MSG_FAIL("CGrayBlock SetUp_Component - Add_Component m_pFrustumCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferCube"), TEXT("ComVIBuffer"), (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CGrayBlock SetUp_Component - Add_Component m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_BOSS_CIRCUS, TEXT("PrototypeTextureGrayBlock"), TEXT("ComTexture"), (CComponent**)&m_pTexture)))
		MSG_FAIL("CGrayBlock SetUp_Component - Add_Component m_pTextureCom");


	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, _vec3(0.f, 0.f, 0.f), _vec3(1.f, 1.f, 1.f), D3DCOLOR_ARGB(255, 255, 255, 255), this);

	m_pTransform->SetUp_Scale(_vec3(14.f, 2.f, 14.f));
	m_pTransform->SetUp_State(CTransform::STATE_POS, _vec3(42.f, 3.f, 20.f));

	m_fFrustumRadius = 0.5f;

	m_plane[CBoxCollider::PLANE_FRONT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_FRONT);
	m_plane[CBoxCollider::PLANE_BACK] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_BACK);
	m_plane[CBoxCollider::PLANE_LEFT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_LEFT);
	m_plane[CBoxCollider::PLANE_RIGHT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_RIGHT);
	m_plane[CBoxCollider::PLANE_TOP] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_TOP);
	m_plane[CBoxCollider::PLANE_BOTTOM] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_BOTTOM);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CGrayBlock::SetUp_Component(void * pArg)
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

	if (FAILED(Add_Component(LEVEL_BOSS_CIRCUS, TEXT("PrototypeTextureGrayBlock"), TEXT("ComTexture"), (CComponent**)&m_pTexture)))
		MSG_FAIL("CGrayBlock SetUp_Component - Add_Component m_pTextureCom");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, pObjInfo->vColliderCenter, pObjInfo->vColliderSize, pObjInfo->dwColliderColor, this);

	_vec3 vScale = m_pTransform->Get_Scale();
	if (vScale.x < vScale.z)
		m_fFrustumRadius = vScale.z / 2.f;
	else
		m_fFrustumRadius = vScale.x / 2.f;

	m_plane[CBoxCollider::PLANE_FRONT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_FRONT);
	m_plane[CBoxCollider::PLANE_BACK] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_BACK);
	m_plane[CBoxCollider::PLANE_LEFT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_LEFT);
	m_plane[CBoxCollider::PLANE_RIGHT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_RIGHT);
	m_plane[CBoxCollider::PLANE_TOP] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_TOP);
	m_plane[CBoxCollider::PLANE_BOTTOM] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_BOTTOM);

	return S_OK;
}


CGameObject * CGrayBlock::Clone(void * pArg)
{
	CGrayBlock* pInstance = new CGrayBlock(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CGrayBlock::Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CGrayBlock::Collision(CGameObject * pColObj)
{
	if (!m_bCollision)
		return;

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
	else if (eCurrentCamera == CAMERA_2D)
	{
		if (CCollision::RectCollision(pPlayerObject, this, (_vec3*)m_pBoxCollider->Get_WorldPosArr(), &vResultPos, &bOntheCube, &bUndertheCube, _vec3(0.f, 0.5f, 0.f)))
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

void CGrayBlock::Free()
{
	__super::Free();
}

CGrayBlock * CGrayBlock::Create(_pDevice pGraphicDevice)
{
	CGrayBlock* pInstance = new CGrayBlock(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CGrayBlock::Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}
