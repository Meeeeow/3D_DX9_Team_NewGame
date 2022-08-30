#include "stdafx.h"
#include "..\public\TempleCubeDestroyable.h"
#include "Player.h"
#include "Collision.h"
#include "Attack.h"

CTempleCubeDestroyable::CTempleCubeDestroyable(_pDevice pGraphicDevice)
	: CTileCube(pGraphicDevice)
{
}

CTempleCubeDestroyable::CTempleCubeDestroyable(const CTempleCubeDestroyable & rhs)
	: CTileCube(rhs)
{
}

HRESULT CTempleCubeDestroyable::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CTempleCubeDestroyable::NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CTempleCubeDestroyable::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CTempleCubeDestroyable::NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CTempleCubeDestroyable::NativeConstruct - SetUpComponent");


	return S_OK;
}


HRESULT CTempleCubeDestroyable::SetUp_Component(void * pArg)
{
	OBJ_INFO* pObjInfo = (OBJ_INFO*)pArg;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransform)))
		MSG_FAIL("CTempleCubeDestroyable SetUp_Component - Add_Component m_pTransformCom");
	m_pTransform->Set_Matrix(pObjInfo->matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRenderer)))
		MSG_FAIL("CTempleCubeDestroyable SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeFrustum"), TEXT("ComFrustum"), (CComponent**)&m_pFrustum)))
		MSG_FAIL("CTempleCubeDestroyable SetUp_Component - Add_Component m_pFrustumCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferCube"), TEXT("ComVIBuffer"), (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CTempleCubeDestroyable SetUp_Component - Add_Component m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_GAMEPLAY, pTagPrototypeTextureTempleBrokenCube, TEXT("ComTexture"), (CComponent**)&m_pTexture)))
		MSG_FAIL("CTempleCubeDestroyable SetUp_Component - Add_Component m_pTextureCom");

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

CTempleCubeDestroyable * CTempleCubeDestroyable::Create(_pDevice pGraphicDevice)
{
	CTempleCubeDestroyable* pInstance = new CTempleCubeDestroyable(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CTempleCubeDestroyable::Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTempleCubeDestroyable::Clone(void * pArg)
{
	CTempleCubeDestroyable* pInstance = new CTempleCubeDestroyable(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CTempleCubeDestroyable::Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTempleCubeDestroyable::Collision(CGameObject * pColObj)
{
	CPlayer* pPlayerObject = dynamic_cast<CPlayer*>(pColObj);
	if (pPlayerObject != nullptr)
	{
		_vec3 vResultPos = { 0.f,0.f,0.f };
		_bool bOntheCube = false;
		_bool bUndertheCube = false;
		if (CCollision::CubeCollision(pPlayerObject, (_plane*)m_pBoxCollider->Get_PlaneDirArr(), &vResultPos, &bOntheCube, &bUndertheCube, _vec3(0.f, 0.5f, 0.f)))
		{
			pPlayerObject->Set_Transform(vResultPos);
			if (bOntheCube)
				pPlayerObject->On_theCube();
			if (bUndertheCube)
				pPlayerObject->Under_theCube();
		}
	}
	CAttack* pAttack = dynamic_cast<CAttack*>(pColObj);

	if (pAttack != nullptr)
	{
		if (pAttack->Get_Destroy())
		{
			m_iTextureIndex += 1;
			if (m_iTextureIndex > 9)
			{
				m_iTextureIndex = 9;
				Set_Dead();
			}

			pAttack->Set_Dead();
		}
	}
}

void CTempleCubeDestroyable::Free()
{
	__super::Free();
}
