#include "stdafx.h"
#include "..\public\BossSkeletonStageCircleAttack.h"
#include "Player.h"

CBossSkeletonStageCircleAttack::CBossSkeletonStageCircleAttack(_pDevice pDevice)
	: CAttack(pDevice)
{
}

CBossSkeletonStageCircleAttack::CBossSkeletonStageCircleAttack(const CBossSkeletonStageCircleAttack & rhs)
	: CAttack(rhs)
{
}

HRESULT CBossSkeletonStageCircleAttack::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBossSkeletonStageCircleAttack NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CBossSkeletonStageCircleAttack::NativeConstruct(void * pArg)
{
	DESC tDesc = *(DESC*)(pArg);

	m_eType = tDesc.eType;
	m_pParent = tDesc.Clone_Parent();

	if (m_eType == TYPE_END)
		MSG_FAIL("CBossSkeletonStageCircleAttack::NativeConstruct - m_eType == TYPE_END");

	if (FAILED(__super::NativeConstruct(&tDesc.tSuperDesc)))
		MSG_FAIL("CBossSkeletonStageCircleAttack NativeConstruct - __super");

	SetUp_Speed(tDesc.fSpeed);
	
	return S_OK;
}

_int CBossSkeletonStageCircleAttack::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CBossSkeletonStageCircleAttack Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	switch (m_eType)
	{
	case Client::CBossSkeletonStageCircleAttack::TYPE_RIGHT:
		Tick_Right(fTimeDelta, pGameInstance);
		break;
	case Client::CBossSkeletonStageCircleAttack::TYPE_FORWARD:
		Tick_Forward(fTimeDelta, pGameInstance);
		break;
	case Client::CBossSkeletonStageCircleAttack::TYPE_LEFT:
		Tick_Left(fTimeDelta, pGameInstance);
		break;
	case Client::CBossSkeletonStageCircleAttack::TYPE_BACK:
		Tick_Back(fTimeDelta, pGameInstance);
		break;
	}

	m_fScale += 0.1f;

	if (m_pParent->Get_Dead() /*|| m_fDist >= m_fDistMax*/ )
		Set_Dead();

	RELEASE_INSTANCE(CGameInstance);
	return 0;
}

_int CBossSkeletonStageCircleAttack::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CBossSkeletonStageCircleAttack LateTick - __super");

	return _int();
}

HRESULT CBossSkeletonStageCircleAttack::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CBossSkeletonStageCircleAttack Render - __super");

	return S_OK;
}

void CBossSkeletonStageCircleAttack::Collision(CGameObject * pColObj)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pColObj);

	DO;
	if (pPlayer != nullptr)
	{
		pPlayer->Sub_Hp(10);
		m_pParent->Set_Dead();
		Set_Dead();
	}
	WHILE;
}

void CBossSkeletonStageCircleAttack::Tick_Dead()
{
}

HRESULT CBossSkeletonStageCircleAttack::Tick_Collider(_float fTimeDelta)
{
	Tick_Collider_BossSkeletonStageCircleAttack(fTimeDelta);

	return S_OK;
}

HRESULT CBossSkeletonStageCircleAttack::Tick_Right(_float fTimeDelta, CGameInstance * pGameInstance)
{
	m_fDist += Go_Right(fTimeDelta);

	return Tick_Scale();
}

HRESULT CBossSkeletonStageCircleAttack::Tick_Left(_float fTimeDelta, CGameInstance * pGameInstance)
{
	m_fDist += Go_Left(fTimeDelta);

	return Tick_Scale();
}

HRESULT CBossSkeletonStageCircleAttack::Tick_Forward(_float fTimeDelta, CGameInstance * pGameInstance)
{
	m_fDist += Go_Forward(fTimeDelta);

	return Tick_Scale();
}

HRESULT CBossSkeletonStageCircleAttack::Tick_Back(_float fTimeDelta, CGameInstance * pGameInstance)
{
	m_fDist += Go_Back(fTimeDelta);

	return Tick_Scale();
}

HRESULT CBossSkeletonStageCircleAttack::Tick_Scale()
{
	_vec3 vScale = Get_Scale();
	if (m_eType == TYPE_LEFT || m_eType == TYPE_RIGHT)
		vScale.z = m_fScale;
	else
		vScale.x = m_fScale;

	if (FAILED(SetUp_Scale(vScale)))
		MSG_FAIL("CBossSkeletonStageCircleAttack::Tick_Right - SetUp_Scale");

	return S_OK;
}

CBossSkeletonStageCircleAttack * CBossSkeletonStageCircleAttack::Create(_pDevice pDevice)
{
	CBossSkeletonStageCircleAttack* pInstance = new CBossSkeletonStageCircleAttack(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CBossSkeletonStageCircleAttack Create Failed");
	}

	return pInstance;
}

CGameObject * CBossSkeletonStageCircleAttack::Clone(void * pArg)
{
	CBossSkeletonStageCircleAttack* pInstance = new CBossSkeletonStageCircleAttack(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CBossSkeletonStageCircleAttack Clone Failed");
	}

	return pInstance;
}

void CBossSkeletonStageCircleAttack::Free()
{
	Safe_Release(m_pParent);
	__super::Free();
}