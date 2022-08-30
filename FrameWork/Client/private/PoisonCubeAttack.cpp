#include "stdafx.h"
#include "..\public\PoisonCubeAttack.h"
#include "Player.h"
#include "Monster.h"
#include "TombStone.h"

CPoisonCubeAttack::CPoisonCubeAttack(_pDevice pDevice)
	: CAttack(pDevice)
{
}

CPoisonCubeAttack::CPoisonCubeAttack(const CPoisonCubeAttack & rhs)
	: CAttack(rhs)
{
}

HRESULT CPoisonCubeAttack::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CPoisonCubeAttack NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CPoisonCubeAttack::NativeConstruct(void * pArg)
{
	DESC tDesc = *(DESC*)(pArg);

	m_pParent = tDesc.Clone_Parent();

	if (FAILED(__super::NativeConstruct(&tDesc.tSuperDesc)))
		MSG_FAIL("CPoisonCubeAttack NativeConstruct - __super");

	return S_OK;
}

_int CPoisonCubeAttack::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CPoisonCubeAttack Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	if (m_pParent->Get_Dead())
		return OBJ_DEAD;

	return 0;
}

_int CPoisonCubeAttack::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CPoisonCubeAttack LateTick - __super");

	return _int();
}

HRESULT CPoisonCubeAttack::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CPoisonCubeAttack Render - __super");

	return S_OK;
}

void CPoisonCubeAttack::Collision(CGameObject * pColObj)
{
	CMonster* pMonster = dynamic_cast<CMonster*>(pColObj);
	CTombStone* pTombStone = dynamic_cast<CTombStone*>(pColObj);
	DO;
	if (pMonster == nullptr)
		break;

	//묘비는 중독 안걸림
	if (pTombStone != nullptr)
		break;
	
	auto& iter = find(m_listMonster.begin(), m_listMonster.end(), pMonster);
	//이미 한 번 맞은 친구면 ㅌㅌ
	if (iter != m_listMonster.end())
		break;

	//한 번 맞으면 다시 안맞게하기 위한 준비 작업
	m_listMonster.emplace_back(pMonster);
	Safe_AddRef(pMonster);

	TIME tTimePoisonTick, tTimePoisonEnd, tTimePoisonCreateParticleHit;
	tTimePoisonTick.fDelay = 1.f;
	tTimePoisonEnd.fDelay = 6.f;
	tTimePoisonCreateParticleHit.fDelay = 0.7f;

	pMonster->Set_Poison(tTimePoisonTick, tTimePoisonEnd, tTimePoisonCreateParticleHit, 2);

	WHILE;
}

void CPoisonCubeAttack::Tick_Dead()
{
}

HRESULT CPoisonCubeAttack::Tick_Collider(_float fTimeDelta)
{
	Tick_Collider_BossSkeletonStageCircleAttack(fTimeDelta);

	return S_OK;
}

void CPoisonCubeAttack::Set_Dead()
{
}

CPoisonCubeAttack * CPoisonCubeAttack::Create(_pDevice pDevice)
{
	CPoisonCubeAttack* pInstance = new CPoisonCubeAttack(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CPoisonCubeAttack Create Failed");
	}

	return pInstance;
}

CGameObject * CPoisonCubeAttack::Clone(void * pArg)
{
	CPoisonCubeAttack* pInstance = new CPoisonCubeAttack(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CPoisonCubeAttack Clone Failed");
	}

	return pInstance;
}

void CPoisonCubeAttack::Free()
{
	Safe_Release(m_pParent);

	for (auto& pMonster : m_listMonster)
		Safe_Release(pMonster);

	m_listMonster.clear();

	__super::Free();
}