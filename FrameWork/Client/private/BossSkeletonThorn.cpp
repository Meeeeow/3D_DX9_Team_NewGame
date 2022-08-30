#include "stdafx.h"
#include "GameInstance.h"
#include "Terrain.h"
#include "InputDevice.h"
#include "PlayerTagMgr.h"
#include "Attack.h"
#include "..\public\BossSkeletonThorn.h"

CBossSkeletonThorn::CBossSkeletonThorn(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CThornObject(pGraphicDevice)
{
}

CBossSkeletonThorn::CBossSkeletonThorn(const CBossSkeletonThorn & rhs)
	: CThornObject(rhs)
{
}

HRESULT CBossSkeletonThorn::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBossSkeletonThorn NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CBossSkeletonThorn::NativeConstruct(void * pArg)
{
	DESC tDesc = *(DESC*)(pArg);

	if (FAILED(__super::NativeConstruct(&tDesc.tSuperDesc)))
		MSG_FAIL("CBossSkeletonThorn NativeConstruct - __super");

	m_tTimeWait.fDelay = tDesc.fDelay;

	return S_OK;
}

_int CBossSkeletonThorn::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CBossSkeletonThorn Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	Tick_Up(fTimeDelta, pGameInstance);
	Tick_Down(fTimeDelta, pGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	return 0;
}

_int CBossSkeletonThorn::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CBossSkeletonThorn LateTick - __super");

	return _int();
}

HRESULT CBossSkeletonThorn::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CBossSkeletonThorn Render - __super");

	return S_OK;
}

void CBossSkeletonThorn::Dead_Event(CGameInstance* pGameInstance)
{
	Set_Dead();

	CAttack::DESC tDesc;
	tDesc.iDmg = 10;
	tDesc.vPos = Get_TransformState(CTransform::STATE_POS);
	tDesc.vScale = Get_TransformScale();
	tDesc.vPos.y += tDesc.vScale.y;
	tDesc.vScale.y *= 2.f;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeAttack, pTagLayerMonsterAttack, &tDesc)))
		MSG_VOID("CBossSkeletonThorn::Dead_Event - pTagPrototypeAttack");

}

HRESULT CBossSkeletonThorn::Tick_Up(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (!m_bUp)
		return S_OK;

	m_fDist += Go_Up(fTimeDelta);
	if (m_fDist >= m_fMaxDist)
	{
		m_bUp = false;
		SetUp_Speed(m_fDownSpeed);
	}

	return S_OK;
}

HRESULT CBossSkeletonThorn::Tick_Down(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (m_bUp)
		return S_OK;

	m_tTimeWait.Tick_Time(fTimeDelta);
	if (!m_tTimeWait.Check_Time())
		return S_OK;

	Go_Down(fTimeDelta);
	
	if (Check_YIsZero())
		Set_Dead();

	return S_OK;
}

void CBossSkeletonThorn::Free()
{
	__super::Free();
}

CBossSkeletonThorn * CBossSkeletonThorn::Create(_pDevice pDevice)
{
	CBossSkeletonThorn* pInstance = new CBossSkeletonThorn(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CMagicMissile Create Failed");
	}

	return pInstance;
}

CGameObject * CBossSkeletonThorn::Clone(void * pArg)
{
	CGameObject* pInstance = new CBossSkeletonThorn(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CBossSkeletonThorn Clone Failed");
	}

	return pInstance;
}

void CBossSkeletonThorn::Collision(CGameObject * pColObj)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pColObj);
	//충돌한 오브젝트가 플레이어면
	if (pPlayer != nullptr)
	{
		Dead_Event(pGameInstance);
	}

	RELEASE_INSTANCE(CGameInstance);
}
