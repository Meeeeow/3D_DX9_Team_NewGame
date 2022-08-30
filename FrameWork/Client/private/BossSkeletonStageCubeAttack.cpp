#include "stdafx.h"
#include "..\public\BossSkeletonStageCubeAttack.h"
#include "Attack.h"
#include "Player.h"
#include "BossSkeletonStageCubeObserver.h"
#include "PlayerTagMgr.h"
#include "BossSkeletonStageCircle.h"
#include "Collision.h"

CBossSkeletonStageCubeAttack::CBossSkeletonStageCubeAttack(_pDevice pDevice)
	: CBossSkeletonStageCube(pDevice)
{
}

CBossSkeletonStageCubeAttack::CBossSkeletonStageCubeAttack(const CBossSkeletonStageCubeAttack & rhs)
	: CBossSkeletonStageCube(rhs)
{
}

HRESULT CBossSkeletonStageCubeAttack::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBossSkeletonStageCubeAttack NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CBossSkeletonStageCubeAttack::NativeConstruct(void * pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//여기서 셋업 컴포넌트 알아서 함
	DESC tDesc = *(DESC*)(pArg);
	__super::DESC tSuperDesc;
	Set_State(tDesc.eType);
	tSuperDesc.tObjInfo = tDesc.tObjInfo;

	if (FAILED(__super::NativeConstruct(&tSuperDesc)))
		MSG_FAIL("CBossSkeletonStageCubeAttack NativeConstruct - __super");

	//돌 공격인 경우
	if (m_eType == TYPE_NORMAL_STONE)
	{
		//크기키움
		m_vStartPos = m_pTransform->Get_State(CTransform::STATE_POS);
		m_vStartPos.y += 20.f;
		CTransform * pPlayerTransform = CPlayerTagMgr::Get_Instance()->Get_CurPlayerTransform();
		if (pPlayerTransform == nullptr)
			MSG_FAIL("CBossSkeletonStageCubeAttack::NativeConstruct - pPlayerTransform == nullptr");

		_vec3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POS);
		vPlayerPos.y += 10.f;

		m_vProjectilePower = m_pTransform->Get_ProjectilePowerToTarget(m_vStartPos, vPlayerPos, m_fStoneProjectileMaxTime, m_fStoneProjectileDegree);
	}

	if (m_eType == TYPE_SMALL_STONE)
	{
		_vec3 vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
		D3DXVec3Normalize(&vLook, &vLook);

		m_vStartPos = m_pTransform->Get_State(CTransform::STATE_POS);
		m_vProjectilePower = vLook * tDesc.fPower;

		m_pTransform->Set_RotateSpeed(tDesc.fPower);

		m_fNX = (rand() % 1000) / 1000.f + 0.5f;
	}

	if (m_eType == TYPE_WET)
	{
		if (tDesc.bNotBoss)
		{
			m_vStartPos = m_pTransform->Get_State(CTransform::STATE_POS);

			m_vProjectilePower = m_pTransform->Get_ProjectilePowerToTarget(m_vStartPos, tDesc.vTargetPos, 4.f, 30.f);

			m_tIceTime.fDelay = 12.f;
		}
		else
		{
			m_vStartPos = m_pTransform->Get_State(CTransform::STATE_POS);
			m_vStartPos.y += 20.f;
			CTransform * pPlayerTransform = CPlayerTagMgr::Get_Instance()->Get_CurPlayerTransform();
			if (pPlayerTransform == nullptr)
				MSG_FAIL("CBossSkeletonStageCubeAttack::NativeConstruct - pPlayerTransform == nullptr");

			_vec3 vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POS);
			vPlayerPos.y += 10.f;

			m_vProjectilePower = m_pTransform->Get_ProjectilePowerToTarget(m_vStartPos, vPlayerPos, m_fWetProjectileMaxTime, m_fWetProjectileDegree);

			m_tIceTime.fDelay = 12.f;
		}

	}

	if (m_eType == TYPE_NORMAL_WEAK)
	{
		m_pTransform->Set_Speed(tDesc.fPower);
	}

	if (m_eType == TYPE_MAGMA)
	{
		m_pTransform->Set_Speed(tDesc.fPower);
	}

	Set_FrameValue(_uint(m_eType));
	Set_FrustrumRadius(Get_TransformScale().x * 1.5f);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_int CBossSkeletonStageCubeAttack::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CBossSkeletonStageCubeAttack Tick - __super");

	if (m_bDead)
	{
		if (m_eType == TYPE_NORMAL_WEAK)
			PLAY_SOUND(L"BossSkeletonStageCubeAttackNormalWeakDead.wav", CSoundMgr::BOSS_SKELETON_STAGE_CUBE);

		return OBJ_DEAD;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//충돌 체크용
	Tick_Collider(fTimeDelta);

	Tick_Stone(fTimeDelta, pGameInstance);
	Tick_SmallStone(fTimeDelta, pGameInstance);
	Tick_Wet(fTimeDelta, pGameInstance);
	Tick_NormalWeak(fTimeDelta, pGameInstance);
	Tick_Magma(fTimeDelta, pGameInstance);

	RELEASE_INSTANCE(CGameInstance);
	return 0;
}

_int CBossSkeletonStageCubeAttack::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CBossSkeletonStageCubeAttack LateTick - __super");

	return _int();
}

HRESULT CBossSkeletonStageCubeAttack::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CBossSkeletonStageCubeAttack Render - __super");

	return S_OK;
}

void CBossSkeletonStageCubeAttack::Set_State(TYPE eType)
{
	m_eType = eType;
	Set_FrameValue(eType);
}

void CBossSkeletonStageCubeAttack::Set_Env(CEnvironmentMgr::TYPE eType)
{
}

void CBossSkeletonStageCubeAttack::Collision(CGameObject * pColObj)
{
	CAttack* pAttack = dynamic_cast<CAttack*>(pColObj);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	DO;
	//충돌한게 공격인 경우
	if (pAttack != nullptr)
	{
		//충돌한 공격이 단일 타겟 공격인데 이미 누굴 공격한 경우
		if (pAttack->Get_SingleAttack() && pAttack->Get_Dead())
			break;

		//충돌한 공격 속성에 오브젝트 파괴가 있는 경우
		if (pAttack->Get_Destroy())
		{
			if (m_eType == TYPE_NORMAL_WEAK)
			{
				Set_Dead();
			}

			if (m_eType == TYPE_NORMAL_STONE)
			{
				Dead_EventNormalStone(0.01f, pGameInstance);
			}
		}

		//빙결 속성 공격인 경우
		if (pAttack->Get_Freeze())
		{
			//얼고 움직임 멈춤
			if (m_eType == TYPE_WET)
			{
				Set_State(TYPE_ICE);
			}

			if (m_eType == TYPE_MAGMA)
			{
				Set_State(TYPE_NORMAL_WEAK);
			}
		}

		pAttack->Set_Attack(true);
	}
	WHILE;


	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pColObj);

	DO;
	//플레이어 아니면 빠짐
	if (pPlayer == nullptr)
		break;

	_uint iCurrentCameraID = pGameInstance->Get_CurrentCameraID();

	//2D카메라면 빠짐
	if (iCurrentCameraID == CAMERA_2D)
		break;

	_vec3		vResultPos = _vec3(0.f, 0.f, 0.f);
	_bool		bOnTheCube = false;
	_bool		bUnderTheCube = false;
	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(pPlayer->Get_Component(pTagComTransform));
	if (pPlayerTransform == nullptr)
		MSG_VOID("CBossSkeletonStageCube::Collision - pPlayerTransform == nullptr");

	//물 타일이면 그냥 데미지, 디버프 입히고 삭제
	if (m_eType == TYPE_WET)
	{
		pPlayerTransform->SetUp_DebuffSpeed(5.f);
		pPlayerTransform->SetUp_DebuffTime(1.f);
		pPlayer->Sub_Hp(5);
		Set_Dead();
		break;
	}

	//얼음 타입이면 타야함
	if (m_eType == TYPE_ICE)
	{
		_vec3 vColliderCenter = pPlayer->Get_ColliderCenter();
		if (CCollision::CubeCollision(pPlayer, (_plane*)m_pBoxCollider->Get_PlaneDirArr(), &vResultPos, &bOnTheCube, &bUnderTheCube, vColliderCenter))
		{
			pPlayer->Set_Transform(vResultPos);
			if (bOnTheCube)
				pPlayer->On_theCube();

			if (bUnderTheCube)
				pPlayer->Under_theCube();
		}

		break;
	}

	if (m_eType == TYPE_NORMAL_WEAK)
	{
		pPlayer->Sub_Hp(10);
		Set_Dead();
		break;
	}

	if (m_eType == TYPE_MAGMA)
	{
		pPlayer->Sub_Hp(15);
		Set_Dead();
		break;
	}

	WHILE;

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CBossSkeletonStageCubeAttack::Tick_Stone(_float fDeltaTime, CGameInstance* pGameInstance)
{
	if (m_eType != TYPE_NORMAL_STONE)
		return S_OK;

	//포물선 운동
	m_fStoneProjectileTime += fDeltaTime;
	m_pTransform->Projectile(m_vStartPos, m_vProjectilePower, m_fStoneProjectileTime, m_fStoneProjectileDegree);

	//바닥 체크 bottom 이 0보다 작아지면 부숴지게
	_vec3 vMinPos = Get_MinPos();
	//부숴질때 파편들 소환해야함
	if (vMinPos.y <= 0.f)
		Dead_EventNormalStone(fDeltaTime, pGameInstance);

	return S_OK;
}

HRESULT CBossSkeletonStageCubeAttack::Tick_SmallStone(_float fTimeDelta, CGameInstance* pGameInstance)
{
	if (m_eType != TYPE_SMALL_STONE)
		return S_OK;

	//포물선 운동
	m_fStoneProjectileTime += fTimeDelta * m_fNX;
	m_pTransform->Projectile(m_vStartPos, m_vProjectilePower, m_fStoneProjectileTime, m_fStoneProjectileDegree);
	m_pTransform->Rotation_Axis(_vec3(1.f, 1.f, 0.f), fTimeDelta* 10.f);

	//바닥 체크 bottom 이 0보다 작아지면 부숴지게
	_vec3 vMinPos = Get_MinPos();
	//부숴질때 파동 소환해야함
	if (vMinPos.y <= 0.5f)
	{
		_vec3 vPos = m_pTransform->Get_State(CTransform::STATE_POS);
		vPos.y = 0.1f;
		m_pTransform->SetUp_State(CTransform::STATE_POS, vPos);
		Dead_EventSmallStone(fTimeDelta, pGameInstance);
	}

	return S_OK;
}

HRESULT CBossSkeletonStageCubeAttack::Tick_Wet(_float fTimeDelta, CGameInstance * pGameInstance)
{
	//얼음인 동안 포물선 이동 안함, 얼음 지속시간 체크해서 얼음 해제
	if (m_eType == TYPE_ICE)
	{
		m_tIceTime.Tick_Time(fTimeDelta);
		if (m_tIceTime.Check_Time())
		{
			m_tIceTime.fTime = 0.f;
			Set_State(TYPE_WET);
		}
		return S_OK;
	}

	if (m_eType != TYPE_WET)
		return S_OK;

	//포물선 운동
	m_fWetProjectileTime += fTimeDelta;
	m_pTransform->Projectile(m_vStartPos, m_vProjectilePower, m_fWetProjectileTime, m_fWetProjectileDegree);

	//바닥 체크 bottom 이 0보다 작아지면 부숴지게
	_vec3 vMinPos = Get_MinPos();
	//부숴질때 파편들 소환해야함
	if (vMinPos.y <= 0.f)
		Set_Dead();

	return S_OK;
}

HRESULT CBossSkeletonStageCubeAttack::Tick_NormalWeak(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (m_eType != TYPE_NORMAL_WEAK)
		return S_OK;

	m_fNormalWeakDist += m_pTransform->Go_Straight(fTimeDelta);
	//일단 거리를 32씩 두었으니 그보다 3 큰 35로 해봄
	if (m_fNormalWeakDist >= 35.f)
		Set_Dead();



	return S_OK;
}

HRESULT CBossSkeletonStageCubeAttack::Tick_Magma(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (m_eType != TYPE_MAGMA)
		return S_OK;

	m_fNormalWeakDist += m_pTransform->Go_Straight(fTimeDelta);
	//일단 거리를 32씩 두었으니 그보다 3 큰 35로 해봄
	if (m_fNormalWeakDist >= 35.f)
		Set_Dead();

	return S_OK;
}

void CBossSkeletonStageCubeAttack::Dead_EventNormalStone(_float fTimeDelta, CGameInstance* pGameInstance)
{
	//12방향으로 10개씩 랜덤하게 작은 돌맹이 사출
	_float fBaseAngle;
	for (size_t i = 0; i < 12; ++i)
	{
		fBaseAngle = 30.f * i;

		//10개씩 사출 힘, 각도 랜덤
		for (size_t j = 0; j < 10; j++)
		{
			int iRandAngle = rand() % 30;
			int iRandPower = rand() % 50;
			_float fAngle = fBaseAngle + iRandAngle;
			_float fPower = (_float)iRandPower;

			DESC tDesc;
			tDesc.eType = TYPE_SMALL_STONE;
			tDesc.fPower = fPower;
			tDesc.tObjInfo.dwColliderColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
			tDesc.tObjInfo.vColliderCenter = _vec3(0.f, 0.f, 0.f);
			tDesc.tObjInfo.vColliderSize = _vec3(0.f, 0.f, 0.f);
			lstrcpy(tDesc.tObjInfo.szTextureName, pTagPrototypeTextureBossSkeletonStageCubeAttack);
			tDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, m_pTransform->Get_State(CTransform::STATE_POS));
			tDesc.tObjInfo.SetUp_Rotation(_vec3(0.f, 1.f, 0.f), fAngle);

			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_SKELETON, pTagPrototypeBossSkeletonStageCubeAttack, pTagLayerMonsterAttack, &tDesc)))
				MSG_VOID("CBossSkeletonStageCubeAttack::Dead_EventNormalStone - pTagPrototypeBossSkeletonStageCubeAttack");
		}
	}

	PLAY_SOUND(L"BossSkeletonStageCubeAttackNormalStoneDead.ogg", CSoundMgr::BOSS_SKELETON_STAGE_CUBE);
	Set_Dead();
}

void CBossSkeletonStageCubeAttack::Dead_EventSmallStone(_float fTimeDelta, CGameInstance* pGameInstance)
{
	CBossSkeletonStageCircle::DESC tDesc;
	tDesc.eTextureLevel = LEVEL_BOSS_SKELETON;
	tDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, m_pTransform->Get_State(CTransform::STATE_POS));
	tDesc.tObjInfo.SetUp_Scale(1.f, 1.f, 1.f);
	tDesc.tObjInfo.SetUp_XRotation(_vec3(1.f, 0.f, 0.f), 90.f);
	lstrcpy(tDesc.tObjInfo.szTextureName, pTagPrototypeTextureBossSkeletonStageCircle);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_SKELETON, pTagPrototypeBossSkeletonStageCircle, 
		pTagLayerNonCollision, &tDesc)))
		MSG_VOID("CBossSkeletonStageCubeAttack::Dead_EventSmallStone - pTagPrototypeBossSkeletonStageCircle");

	Set_Dead();
}

CBossSkeletonStageCubeAttack * CBossSkeletonStageCubeAttack::Create(_pDevice pDevice)
{
	CBossSkeletonStageCubeAttack* pInstance = new CBossSkeletonStageCubeAttack(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CBossSkeletonStageCubeAttack Create Failed");
	}

	return pInstance;
}

CGameObject * CBossSkeletonStageCubeAttack::Clone(void * pArg)
{
	CBossSkeletonStageCubeAttack* pInstance = new CBossSkeletonStageCubeAttack(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CBossSkeletonStageCubeAttack Clone Failed");
	}

	return pInstance;
}

void CBossSkeletonStageCubeAttack::Free()
{
	__super::Free();
}
