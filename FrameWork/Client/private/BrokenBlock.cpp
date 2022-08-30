#include "stdafx.h"
#include "..\public\BrokenBlock.h"
#include "Player.h"
#include "Collision.h"
#include "Attack.h"


CBrokenBlock::CBrokenBlock(_pDevice pGraphicDevice)
	: CTileCube(pGraphicDevice)
{
}

CBrokenBlock::CBrokenBlock(const CBrokenBlock & rhs)
	: CTileCube(rhs)
	, m_iDurabillity(rhs.m_iDurabillity)
{
}

HRESULT CBrokenBlock::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBrokenBlock::NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CBrokenBlock::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CBrokenBlock::NativeConstruct - __super");

	if(FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CBrokenBlock::NativeConstruct - SetUp_Component");


	return S_OK;
}

HRESULT CBrokenBlock::SetUp_Component()
{
	return S_OK;
}

HRESULT CBrokenBlock::SetUp_Component(void * pArg)
{
	OBJ_INFO* pObjInfo = (OBJ_INFO*)pArg;

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransform)))
		MSG_FAIL("CExplosion SetUp_Component - Add_Component m_pTransformCom");
	m_pTransform->Set_Matrix(pObjInfo->matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRenderer)))
		MSG_FAIL("CExplosion SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeFrustum"), TEXT("ComFrustum"), (CComponent**)&m_pFrustum)))
		MSG_FAIL("CExplosion SetUp_Component - Add_Component m_pFrustumCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferCube"), TEXT("ComVIBuffer"), (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CExplosion SetUp_Component - Add_Component m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_BOSS_CIRCUS, TEXT("PrototypeTextureBrokenBlock"), TEXT("ComTexture"), (CComponent**)&m_pTexture)))
		MSG_FAIL("CExplosion SetUp_Component - Add_Component m_pTextureCom");

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

CGameObject * CBrokenBlock::Clone(void * pArg)
{
	CBrokenBlock* pInstance = new CBrokenBlock(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBrokenBlock::Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBrokenBlock::Collision(CGameObject * pColObj)
{
	if (!m_bCollision)
		return;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pPlayerObject = dynamic_cast<CPlayer*>(pColObj);
	CAttack* pPlayerAttack = dynamic_cast<CAttack*>(pColObj);
	_uint eCurrentCamera = pGameInstance->Get_CurrentCameraID();

	if (pPlayerObject != nullptr)
	{
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
	}

	if (pPlayerAttack != nullptr)
	{
		if (pPlayerAttack->Get_Destroy())
		{
			m_iDurabillity -= 1;
			if (m_iDurabillity <= 0)
				Set_Dead();
		}
	}
	RELEASE_INSTANCE(CGameInstance);

}

void CBrokenBlock::Free()
{
	__super::Free();
}

CBrokenBlock * CBrokenBlock::Create(_pDevice pGraphicDevice)
{
	CBrokenBlock* pInstance = new CBrokenBlock(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CBrokenBlock::Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}
