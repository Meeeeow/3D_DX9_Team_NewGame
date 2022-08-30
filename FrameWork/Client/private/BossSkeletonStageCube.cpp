#include "stdafx.h"
#include "..\public\BossSkeletonStageCube.h"
#include "Attack.h"
#include "Player.h"
#include "BossSkeletonStageCubeObserver.h"
#include "Frustum.h"
#include "Collision.h"

CBossSkeletonStageCube::CBossSkeletonStageCube(_pDevice pDevice)
	: CTileCube(pDevice)
{
}

CBossSkeletonStageCube::CBossSkeletonStageCube(const CBossSkeletonStageCube & rhs)
	: CTileCube(rhs)
{
}

HRESULT CBossSkeletonStageCube::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBossSkeletonStageCube NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CBossSkeletonStageCube::NativeConstruct(void * pArg)
{
	//여기서 셋업 컴포넌트 알아서 함
	DESC tDesc = *(DESC*)(pArg);

	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CBossSkeletonStageCube NativeConstruct - __super");

	if (FAILED(SetUp_Component(&tDesc)))
		MSG_FAIL("CBossSkeletonStageCube NativeConstruct - SetUp_Component");

	Clear_Env();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_SKELETON, pTagPrototypeBossSkeletonStageCubeObserver, pTagLayerObserver, this, (void**)&m_pObserver)))
		MSG_FAIL("CBossSkeletonStageCube::NativeConstruct - Add_GameObjectToLayer pTagPrototypeBossSkeletonStageCubeObserver");

	Set_State(FSM_NORMAL);

	Safe_AddRef(m_pObserver);

	Check_Frame(0.01f);

	RELEASE_INSTANCE(CGameInstance);

	m_tTimeIceRemain.fDelay = 5.f;
	m_tTimeRain.fDelay = 5.f;
	m_tTimeMagma.fDelay = 5.f;

	return S_OK;
}

_int CBossSkeletonStageCube::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CBossSkeletonStageCube Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	Tick_Rain(fTimeDelta);
	Tick_FsmIce(fTimeDelta);
	Tick_Magma(fTimeDelta);

	Tick_Collider(fTimeDelta);

	Check_Frame(fTimeDelta);

	return 0;
}

_int CBossSkeletonStageCube::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CBossSkeletonStageCube LateTick - __super");

	return _int();
}

HRESULT CBossSkeletonStageCube::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CBossSkeletonStageCube Render - __super");

	return S_OK;
}

void CBossSkeletonStageCube::Set_State(FSM eState)
{
	m_eCurState = eState;

	if (m_eCurState == FSM_ICE)
		PLAY_SOUND(L"BossSkeletonStageCubeIce.ogg", CSoundMgr::BOSS_SKELETON_STAGE_CUBE);

	if (m_eCurState == FSM_MAGMA)
		PLAY_SOUND(L"BossSkeletonStageCubeMagma.ogg", CSoundMgr::BOSS_SKELETON_STAGE_CUBE);

}

void CBossSkeletonStageCube::Set_TutorialMagma()
{
	m_bTutorialMagma = true;
}

void CBossSkeletonStageCube::Clear_Env()
{
	for (auto& bEnv : m_bEnvArr)
		bEnv = false;
}

void CBossSkeletonStageCube::Set_Env(CEnvironmentMgr::TYPE eType)
{
	m_bEnvArr[eType] = true;
	if (eType == CEnvironmentMgr::TYPE_MAGMA)
	{
		if (m_ePreState != FSM_ICE)
			Set_State(FSM_MAGMA);
	}
}

void CBossSkeletonStageCube::Collision(CGameObject * pColObj)
{
	CAttack* pAttack = dynamic_cast<CAttack*>(pColObj);

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
			//큐브가 패턴 일반 타입인 경우
			if (m_eCurState == FSM_WEAK_NORMAL)
			{
				Set_State(FSM_DEAD_NORMAL);
				break;
			}

			//큐브가 얼음인 경우
			if (m_eCurState == FSM_ICE)
			{
				Set_State(FSM_DEAD_ICE);
				break;
			}
		}

		//빙결 속성 공격인 경우
		if (pAttack->Get_Freeze())
		{
			//얼어야함
			if (m_eCurState == FSM_WET)
			{
				Set_State(FSM_ICE);
				break;
			}

			//식어야함
			if (m_eCurState == FSM_MAGMA)
			{
				Set_State(FSM_NORMAL);
				break;
			}

		}

		pAttack->Set_Attack(true);
		pAttack->Set_Dead();
	}
	WHILE;


	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pColObj);

	DO;
	//플레이어 아니면 빠짐
	if (pPlayer == nullptr)
		break;

	CGameInstance* pGameInstnace = GET_INSTANCE(CGameInstance);
	_uint iCurrentCameraID = pGameInstnace->Get_CurrentCameraID();
	RELEASE_INSTANCE(CGameInstance);

	//2D카메라면 빠짐
	if (iCurrentCameraID == CAMERA_2D)
		break;

	_vec3	vResultPos = _vec3(0.f, 0.f, 0.f);
	_bool	bOnTheCube = false;
	_bool	bUnderTheCube = false;

	if (CCollision::CubeCollision(pPlayer, (_plane*)m_pBoxCollider->Get_PlaneDirArr(), &vResultPos, &bOnTheCube, &bUnderTheCube, _vec3(0.f,0.5f,0.f)))
	{
		pPlayer->Set_Transform(vResultPos);
		if (bOnTheCube)
		{
			pPlayer->On_theCube();
			CTransform* pPlayerTransform = dynamic_cast<CTransform*>(pPlayer->Get_Component(pTagComTransform));
			if (pPlayerTransform == nullptr)
				MSG_VOID("CBossSkeletonStageCube::Collision - pPlayerTransform == nullptr");

			if (m_eCurState == FSM_WET)
			{
				pPlayerTransform->SetUp_DebuffSpeed(5.f);
				pPlayerTransform->SetUp_DebuffTime(1.f);
			}

			if (m_eCurState == FSM_MAGMA)
			{
				pPlayer->Sub_Hp(1);
			}
		}

		if (bUnderTheCube)
			pPlayer->Under_theCube();
	}


	WHILE;

	Check_Frame(0.01f);
}

HRESULT CBossSkeletonStageCube::Check_Frame(_float fTimeDelta)
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Client::CBossSkeletonStageCube::FSM_NORMAL:
			Set_FrameValue(0);
			break;
		case Client::CBossSkeletonStageCube::FSM_WEAK_NORMAL:
			Set_FrameValue(1);
			break;
		case Client::CBossSkeletonStageCube::FSM_WET:
			Set_FrameValue(2);
			break;
		case Client::CBossSkeletonStageCube::FSM_ICE:
			Clear_Env();
			m_tTimeIceRemain.fTime = 0.f;
			Set_FrameValue(3);
			break;
		case Client::CBossSkeletonStageCube::FSM_MAGMA:
			m_tTimeMagma.fTime = 0.f;
			Set_FrameValue(4);
			break;
		case Client::CBossSkeletonStageCube::FSM_DEAD_NORMAL:
			Set_Dead();
			break;
		case Client::CBossSkeletonStageCube::FSM_DEAD_ICE:
			Set_Dead();
			break;
		}

		m_ePreState = m_eCurState;
	}
	

	return S_OK;
}

HRESULT CBossSkeletonStageCube::Tick_Rain(_float fTimeDelta)
{
	if (!m_bEnvArr[CEnvironmentMgr::TYPE_RAIN])
		return S_OK;

	if (m_eCurState == FSM_WET)
	{
		m_tTimeRain.Tick_Time(fTimeDelta);
		if (m_tTimeRain.Check_Time())
		{
			m_tTimeRain.fTime = 0.f;
			Set_State(FSM_NORMAL);
		}
	}

	if (m_eCurState != FSM_NORMAL /*&& m_eCurState != FSM_MAGMA*/)
		return S_OK;

	int iRand = rand() % 2000;
	if (iRand == 0)
	{
		m_tTimeRain.fTime = 0.f;
		if (m_eCurState == FSM_NORMAL)
			Set_State(FSM_WET);
		//else if (m_eCurState == FSM_MAGMA)
		//	Set_State(FSM_NORMAL);
	}

	return S_OK;
}

HRESULT CBossSkeletonStageCube::Tick_FsmIce(_float fTimeDelta)
{
	if (m_eCurState != FSM_ICE)
		return S_OK;

	m_tTimeIceRemain.Tick_Time(fTimeDelta);
	if (m_tTimeIceRemain.Check_Time())
	{
		m_tTimeIceRemain.fTime = 0.f;
		Set_State(FSM_NORMAL);
	}

	return S_OK;
}

HRESULT CBossSkeletonStageCube::Tick_Magma(_float fTimeDelta)
{
	if (m_eCurState != FSM_MAGMA || m_bTutorialMagma)
		return S_OK;

	m_tTimeMagma.Tick_Time(fTimeDelta);
	if (m_tTimeMagma.Check_Time())
	{
		m_tTimeMagma.fTime = 0.f;
		Set_State(FSM_NORMAL);
	}

	return S_OK;
}

HRESULT CBossSkeletonStageCube::SetUp_Component(void * pArg)
{
	DESC tDesc = *(DESC*)(pArg);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform)))
		MSG_FAIL("CBossSkeletonStageCube::SetUp_Component - pTagPrototypeTransform");
	m_pTransform->Set_Matrix(tDesc.tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, tDesc.tObjInfo.szTextureName, pTagComTexture, (CComponent**)&m_pTexture)))
		MSG_FAIL("CBossSkeletonStageCube::SetUp_Component - pTagPrototypeTextureBossSkeletonStageCube");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CBossSkeletonStageCube::SetUp_Component - pTagPrototypeRenderer");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferCube, pTagComVIBuffer, (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CBossSkeletonStageCube::SetUp_Component - pTagPrototypeVIBufferCube");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeFrustum, pTagComFrustum, (CComponent**)&m_pFrustum)))
		MSG_FAIL("CBossSkeletonStageCube::SetUp_Component - pTagPrototypeTransform");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, tDesc.tObjInfo.vColliderCenter
		, tDesc.tObjInfo.vColliderSize, tDesc.tObjInfo.dwColliderColor, this);

	Set_FrustrumRadius(m_pTransform->Get_Scale().x * 1.5f);

	return S_OK;
}

CBossSkeletonStageCube * CBossSkeletonStageCube::Create(_pDevice pDevice)
{
	CBossSkeletonStageCube* pInstance = new CBossSkeletonStageCube(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CBossSkeletonStageCube Create Failed");
	}

	return pInstance;
}

CGameObject * CBossSkeletonStageCube::Clone(void * pArg)
{
	CBossSkeletonStageCube* pInstance = new CBossSkeletonStageCube(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CBossSkeletonStageCube Clone Failed");
	}

	return pInstance;
}

void CBossSkeletonStageCube::Free()
{
	CEnvironmentMgr::Get_Instance()->UnSubScribe_Observer(m_pObserver);
	Safe_Release(m_pObserver);
	__super::Free();
}
