#include "stdafx.h"
#include "..\public\TelekinesisManager.h"
#include "CrosshairMgr.h"
#include "Player.h"
#include "Collision.h"


IMPLEMENT_SINGLETON(CTelekinesisManager)

CTelekinesisManager::CTelekinesisManager()
{
}

void CTelekinesisManager::Set_MonsterTransform(CTransform * pTransform)
{
	if (m_bTelekinesis)
		return;

	m_pTargetTransform = pTransform;
	m_pTargetTransform->AddRef();
	m_bTelekinesis = true;
}

void CTelekinesisManager::Telekinesis(_float fTimeDelta, CTransform* pPlayerTransform)
{
	if (!m_bTelekinesis)
		return;

	m_fTelekinesis -= fTimeDelta;
	if (m_fTelekinesis <= 0.f)
	{
		Safe_Release(m_pTargetTransform);
		m_bTelekinesis = false;
		m_fTelekinesis = 5.f;
		return;
	}
	
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_vec3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POS);
	_vec3 vHeight = m_pTargetTransform->Get_State(CTransform::STATE_POS);
	_vec3 vRay = CCrosshairMgr::Get_Instance()->Get_Ray();

	D3DXVec3Normalize(&vRay, &vRay);

	vRay *= 7.5f;

	vRay += vPlayerPos;

	_vec3 vGoalPos = { vRay.x, vHeight.y, vRay.z };
	m_pTargetTransform->Chase_Vector(vGoalPos, fTimeDelta * 3.f);

	RELEASE_INSTANCE(CGameInstance);
}

void CTelekinesisManager::Free()
{
	
}
