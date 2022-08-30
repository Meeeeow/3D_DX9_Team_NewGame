#include "stdafx.h"
#include "..\public\CollisionRect.h"
#include "Player.h"
#include "PlayerTagMgr.h"
#include "Attack.h"
#include "DmgFont.h"
#include "BossSkeletonStageCircleAttack.h"


CCollisionRect::CCollisionRect(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CRectObject(pGraphicDevice)
{
}

CCollisionRect::CCollisionRect(const CCollisionRect & rhs)
	: CRectObject(rhs)
{
}

HRESULT CCollisionRect::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CCollisionRect NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CCollisionRect::NativeConstruct(void * pArg)
{
	DESC tDesc = *(DESC*)(pArg);

	CRectObject::DESC tSuperDesc = tDesc.tSuperDesc;
	if (FAILED(__super::NativeConstruct(&tSuperDesc)))
		MSG_FAIL("CCollisionRect NativeConstruct - __super");

	OBJ_INFO tObjInfo = tDesc.tSuperDesc.tObjInfo;
	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, tObjInfo.vColliderCenter
		, tObjInfo.vColliderSize, tObjInfo.dwColliderColor, this);

	return S_OK;
}

_int CCollisionRect::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CCollisionRect Tick - __super");

	return 0;
}

_int CCollisionRect::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CCollisionRect LateTick - __super");

	return _int();
}

HRESULT CCollisionRect::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CCollisionRect Render - __super");

	if (m_pBoxCollider == nullptr)
		MSG_FAIL("CCollisionRect Render - m_pBoxCollider == nullptr");

	if (FAILED(m_pBoxCollider->Render()))
		MSG_FAIL("CCollisionRect Render - m_pBoxCollider->Render");

	return S_OK;
}

HRESULT CCollisionRect::Tick_Collider(_float fTimeDelta)
{
	return CRectObject::Tick_Collider(fTimeDelta, m_pBoxCollider);
}

void CCollisionRect::Free()
{
	Safe_Release(m_pBoxCollider);
	__super::Free();
}

_vec3 CCollisionRect::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxCollider->Get_MinPos();

	return vMinPos;
}

_vec3 CCollisionRect::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxCollider->Get_MaxPos();

	return vMaxPos;
}

_vec3 CCollisionRect::Get_WorldPos()
{
	return m_pBoxCollider->Get_ColliderWolrdPos();
}
