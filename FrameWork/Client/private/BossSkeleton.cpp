#include "stdafx.h"
#include "..\public\BossSkeleton.h"
#include "Player.h"
#include "PlayerTagMgr.h"
#include "BossSkeletonMagicCircle.h"
#include "DestroyableCube.h"
#include "EnvironmentMgr.h"
#include "BossSkeletonStageCubeAttack.h"
#include "BossSkeletonThorn.h"
#include "DmgFont.h"
#include "Attack.h"

#include <functional>

CBossSkeleton::CBossSkeleton(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CMonster(pGraphicDevice)
{
	ZeroMemory(m_pTextureArr, sizeof(m_pTextureArr));
}

CBossSkeleton::CBossSkeleton(const CBossSkeleton & rhs)
	:CMonster(rhs)
{
}

HRESULT CBossSkeleton::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBossSkeleton NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CBossSkeleton::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CBossSkeleton NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CBossSkeleton NativeConstruct - SetUp_Component");

	m_bStateLock = true;
	Check_Frame();

	m_tIdleTime.fDelay = 7.f;
	m_tIdleTime.fTime = 5.f;

	//1페 스킬 마법진 이후 대기시간
	m_tPhase1RedSkillStartTime.fDelay = 3.f;
	m_tPhase1BlueSkillStartTime.fDelay = 3.f;
	m_tPhase1GreenSkillStartTime.fDelay = 3.f;

	m_tPhase2RedSkillStartTime.fDelay = 2.f;
	m_tPhase2BlueSkillStartTime.fDelay = 2.f;
	m_tPhase2GreenSkillStartTime.fDelay = 2.f;
	m_tPhase2GreenSkillThornTime.fDelay = 10.f;


	return S_OK;
}

_int CBossSkeleton::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CBossSkeleton Tick - __super");

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//태그 때문에 항상 플레이어 트랜스폼 갱신 해줘야함
	m_pTransformPlayer = CPlayerTagMgr::Get_Instance()->Get_CurPlayerTransform();

	if (FAILED(AI(fTimeDelta, pGameInstance)))
		MSG_MINUSONE("CBossSkeleton::Tick - AI");

	Check_Frame();

	Die(fTimeDelta);

	if (FAILED(Phase1RedSkill(fTimeDelta, pGameInstance)))
		MSG_FAIL("CBossSkeleton::Tick - Phase1RedSkill");

	if (FAILED(Phase1BlueSkill(fTimeDelta, pGameInstance)))
		MSG_FAIL("CBossSkeleton::Tick - Phase1BlueSkill");

	if (FAILED(Phase1GreenSkill(fTimeDelta, pGameInstance)))
		MSG_FAIL("CBossSkeleton::Tick - Phase1GreenSkill");

	if (FAILED(Phase2RedSkill(fTimeDelta, pGameInstance)))
		MSG_FAIL("CBossSkeleton::Tick - Phase2RedSkill");

	if (FAILED(Phase2BlueSkill(fTimeDelta, pGameInstance)))
		MSG_FAIL("CBossSkeleton::Tick - Phase2BlueSkill");

	if (FAILED(Phase2GreenSkill(fTimeDelta, pGameInstance)))
		MSG_FAIL("CBossSkeleton::Tick - Phase2GreenSkill");

	//지형 타기
	Collision_Terrain(fTimeDelta, pGameInstance, m_pTransform);
	//빌보딩
	if (FAILED(BillBoarding(m_pTransform)))
		MSG_FAIL("CBossSkeleton::Tick - BillBoarding");

	if (FAILED(Tick_Collider(fTimeDelta)))
		MSG_FAIL("CBossSkeleton::Tick - Tick_Collider");

	RELEASE_INSTANCE(CGameInstance);


	return OBJ_NOEVENT;
}

_int CBossSkeleton::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CBossSkeleton LateTick - __super");

	if (m_pRenderer == nullptr)
		MSG_MINUSONE("CBossSkeleton::LateTick - m_pRenderer == nullptr");

	if (m_pRenderer->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this))
		MSG_MINUSONE("CBossSkeleton::LateTick - Add_RenderGroup");

	//체력이 절반 이하로 떨어지면 2페이즈
	if (m_tMonsterStat.iHp <= (m_tMonsterStat.iMaxHp >> 1) && m_bBossPhase1)
	{
		m_bBossPhase1 = false;
		m_iPatternIndex = 3;
		m_tIdleTime.fDelay = 9.f;
	}

	return _int();
}

HRESULT CBossSkeleton::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CBossSkeleton Render - __super");

	if (m_pTransform == nullptr ||
		m_pTextureArr == nullptr ||
		m_pBoxCollider == nullptr ||
		m_pVIBuffer == nullptr)
		MSG_FAIL("CBossSkeleton::Render - Com is null");

	if (FAILED(m_pTransform->Bind_OnDevice()))
		MSG_FAIL("CBossSkeleton::Render - m_pTransform Bind_OnDevice");

	if (FAILED(m_pTextureArr[m_eCurState]->Bind_OnDevice(Get_FrameIndex())))
		MSG_FAIL("CBossSkeleton::Render - m_pTextureArr[m_eCurState]->Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CBossSkeleton::Render - Set_RenderState");

	if (FAILED(m_pVIBuffer->Render()))
		MSG_FAIL("CBossSkeleton::Render - m_pVIBuffer->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CBossSkeleton::Render - Release_RenderState");

	if (FAILED(m_pBoxCollider->Render()))
		MSG_FAIL("CBossSkeleton::Render - m_pBoxCollider->Render");

	return S_OK;
}

_bool CBossSkeleton::Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, CTransform * pOut)
{
	return false;
}

void CBossSkeleton::Set_State(FSM eState)
{
	if (m_bStateLock)
		return;

	m_eCurState = eState;
}

void CBossSkeleton::Set_Idle()
{
	m_bStateLock = false;
	Set_State(FSM_IDLE);
}

HRESULT CBossSkeleton::Sub_Hp(_uint iDmg)
{
	m_tMonsterStat.iHp -= iDmg;
	if (m_tMonsterStat.iHp < 0)
	{
		m_tMonsterStat.iHp = 0;
		Set_Dead();
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CDmgFont::DESC tDesc;
	tDesc.iDmg = iDmg;

	_vec3 vBossPos = m_pTransform->Get_State(CTransform::STATE_POS);
	_vec3 vScale = m_pTransform->Get_Scale();
	tDesc.vPos = vBossPos + _vec3(-1.f * vScale.x * 0.5f, m_pTransform->Get_State(CTransform::STATE_UP).y, 0.f);
	tDesc.vScale = vScale;
	_int iRandX = rand() % _int(vScale.x);
	tDesc.vPos += _vec3(_float(iRandX), 0.f, 0.f);

	////데미지 폰트가 앞으로 나와야 안가림
	_vec3 vDmgFontLook;
	D3DXVec3Normalize(&vDmgFontLook,&(m_pTransformPlayer->Get_State(CTransform::STATE_POS) - vBossPos));
	tDesc.vPos += vDmgFontLook * 2.f;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeDmgFont, pTagLayerDmgFont, &tDesc)))
		MSG_FAIL("CBossSkeleton::Sub_Hp - pTagPrototypeDmgFont");

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CBossSkeleton::SetUp_Component(void *pArg)
{
	DESC tDesc = *(DESC*)(pArg);
	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.fSpeed = 10.f;
	tTransformDesc.fDegree = 3.f;
	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform, &tTransformDesc)))
		MSG_FAIL("CBossSkeleton::SetUp_Component - m_pTransform");
	m_pTransform->Set_Matrix(tDesc.tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CBossSkeleton::SetUp_Component - m_pRenderer");

	CVIBufferRect::SIZE_POS tSizePos;
	tSizePos.vPos = tDesc.tObjInfo.vVertexPos;
	tSizePos.vSize = _vec3(tDesc.tObjInfo.fSizeX, tDesc.tObjInfo.fSizeY, tDesc.tObjInfo.fSizeZ);
	m_pVIBuffer = CVIBufferRect::Create(m_pGraphicDevice, &tSizePos);
	m_mapComponent.emplace(pTagComVIBuffer, m_pVIBuffer);

	wstring wstrTextureName;

	wstrTextureName = tDesc.tObjInfo.szTextureName;
	wstrTextureName += L"Idle";
	if (FAILED(Add_Component(LEVEL_BOSS_SKELETON, wstrTextureName.c_str(), L"ComTextureIdle", (CComponent**)&m_pTextureArr[FSM_IDLE])))
		MSG_FAIL("CBossSkeleton::SetUp_Component - FSM_IDLE");

	wstrTextureName = tDesc.tObjInfo.szTextureName;
	wstrTextureName += L"Attack";
	if (FAILED(Add_Component(LEVEL_BOSS_SKELETON, wstrTextureName.c_str(), L"ComTextureAttack", (CComponent**)&m_pTextureArr[FSM_ATTACK])))
		MSG_FAIL("CBossSkeleton::SetUp_Component - FSM_ATTACK");

	wstrTextureName = tDesc.tObjInfo.szTextureName;
	wstrTextureName += L"Die";
	if (FAILED(Add_Component(LEVEL_BOSS_SKELETON, wstrTextureName.c_str(), L"ComTextureDie", (CComponent**)&m_pTextureArr[FSM_DIE])))
		MSG_FAIL("CBossSkeleton::SetUp_Component - FSM_DIE");

	wstrTextureName = tDesc.tObjInfo.szTextureName;
	wstrTextureName += L"Spawn";
	if (FAILED(Add_Component(LEVEL_BOSS_SKELETON, wstrTextureName.c_str(), L"ComTextureHit", (CComponent**)&m_pTextureArr[FSM_SPAWN])))
		MSG_FAIL("CBossSkeleton::SetUp_Component - FSM_HIT");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, tDesc.tObjInfo.vColliderCenter, 
		tDesc.tObjInfo.vColliderSize, tDesc.tObjInfo.dwColliderColor, this);
	m_mapComponent.emplace(pTagComCollider, m_pBoxCollider);

	m_tMonsterStat.iMaxHp = 100;
	m_tMonsterStat.iHp = m_tMonsterStat.iMaxHp;
	m_tMonsterStat.iDmg = 5;

	return S_OK;
}

HRESULT CBossSkeleton::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBossSkeleton::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CBossSkeleton::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CBossSkeleton::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CBossSkeleton::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

HRESULT CBossSkeleton::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBossSkeleton::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CBossSkeleton::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

HRESULT CBossSkeleton::AI(_float fTimeDelta, CGameInstance* pGameInstance)
{
	//패턴 중에는 시간 안올라가게
	if (m_bStateLock)
		return S_OK;

	//패턴마다 대기시간 만들자
	m_tIdleTime.fTime += fTimeDelta;
	if (m_tIdleTime.fDelay > m_tIdleTime.fTime)
		return S_OK;

	m_tIdleTime.fTime = 0.f;

	//1페일때는 2페패턴 안뜸
	if (m_iPatternIndex >= 3 && m_bBossPhase1)
		m_iPatternIndex = 0;

	//2페때는 1페 안뜸
	if (m_iPatternIndex >= 6 && !m_bBossPhase1)
		m_iPatternIndex = 3;

	m_bStateLock = true;

	//시연회를 위해서 패턴 순서대로 나오게 할 생각, 인덱스로 패턴 관리하자
	switch (m_iPatternIndex)
	{
	case 0:
		//빨강 마법진 1페
		if (FAILED(SetUp_Phase1RedSkill(pGameInstance)))
			MSG_FAIL("CBossSkeleton::AI - SetUp_Phase1RedSkill");
		break;
	case 1:
		//파랑 마법진 1페
		if (FAILED(SetUp_Phase1BlueSkill(pGameInstance)))
			MSG_FAIL("CBossSkeleton::AI - SetUp_Phase1BlueSkill");
		break;
	case 2:
		//초록 마법진 1페
		if (FAILED(SetUp_Phase1GreenSkill(pGameInstance)))
			MSG_FAIL("CBossSkeleton::AI - SetUp_Phase1GreenSkill");
		break;
	case 3:
		//빨강 마법진 2페
		if (FAILED(SetUp_Phase2RedSkill(pGameInstance)))
			MSG_FAIL("CBossSkeleton::AI - SetUp_Phase2RedSkill");
		break;
	case 4:
		//파랑 마법진 2페
		if (FAILED(SetUp_Phase2BlueSkill(pGameInstance)))
			MSG_FAIL("CBossSkeleton::AI - SetUp_Phase2BlueSkill");
		break;
	case 5:
		//초록 마법진 2페
		if (FAILED(SetUp_Phase2GreenSkill(pGameInstance)))
			MSG_FAIL("CBossSkeleton::AI - SetUp_Phase2GreenSkill");
		break;
	}

	return S_OK;
}

void CBossSkeleton::Check_Frame()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Client::CBossSkeleton::FSM_IDLE:
			m_tFrame.Set_Value(6);
			break;
		case Client::CBossSkeleton::FSM_ATTACK:
			m_tFrame.Set_Value(10);
			break;
		case Client::CBossSkeleton::FSM_DIE:
			m_tFrame.Set_Value(13);
			break;
		case Client::CBossSkeleton::FSM_SPAWN:
			m_tFrame.Set_Value(15);
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CBossSkeleton::Die(_float fTimeDelta)
{
	if (m_eCurState != FSM_DIE)
		return;

}

void CBossSkeleton::Tick_Frame(_float fTimeDelta)
{
	m_tFrame.fTime += fTimeDelta;
	if (m_tFrame.fTime >= m_tFrame.fDelay)
	{
		m_tFrame.fTime = 0.f;
		m_tFrame.iStart++;

		Tick_Framing();

		if (m_tFrame.iStart >= m_tFrame.iEnd)
		{
			m_tFrame.iStart = 0;
			Tick_FrameEnd();
		}
	}
}

void CBossSkeleton::Tick_FrameEnd()
{
	FrameEnd_Spawn();

	//debug
	if (m_eCurState == FSM_ATTACK)
		Set_Idle();
}

void CBossSkeleton::FrameEnd_Spawn()
{
	if (m_eCurState != FSM_SPAWN)
		return;

	Set_Idle();
}

void CBossSkeleton::SetUp_Skill()
{
	// 스킬 끝난 후 대기시간 이후 다른 스킬 사용하게
	m_iPatternIndex++;
	m_tIdleTime.fTime = 0.f;

	//모션 고정
	Set_State(FSM_ATTACK);
	m_bStateLock = true;
}

HRESULT CBossSkeleton::SetUp_Phase1RedSkill(CGameInstance* pGameInstance)
{
	//기본적인 준비
	SetUp_Skill();

	//스킬 시작
	m_bPhase1RedSkill = true;

	//마법진 소환
	Summon_MagicCircle(CBossSkeletonMagicCircle::TYPE_RED, _vec3(2.f, 2.f, 2.f), pGameInstance);

	return S_OK;
}

HRESULT CBossSkeleton::SetUp_Phase1BlueSkill(CGameInstance* pGameInstance)
{
	//기본적인 준비
	SetUp_Skill();

	//스킬 시작
	m_bPhase1BlueSkill = true;

	//마법진 소환
	Summon_MagicCircle(CBossSkeletonMagicCircle::TYPE_BLUE, _vec3(2.f, 2.f, 2.f), pGameInstance);

	return S_OK;
}

HRESULT CBossSkeleton::SetUp_Phase1GreenSkill(CGameInstance* pGameInstance)
{
	//기본적인 준비
	SetUp_Skill();

	//스킬 시작
	m_bPhase1GreenSkill = true;

	//마법진 소환
	Summon_MagicCircle(CBossSkeletonMagicCircle::TYPE_GREEN, _vec3(2.f, 2.f, 2.f), pGameInstance);

	return S_OK;
}

HRESULT CBossSkeleton::SetUp_Phase2RedSkill(CGameInstance* pGameInstance)
{
	//기본적인 준비
	SetUp_Skill();

	//스킬 시작
	m_bPhase2RedSkill = true;

	//마법진 소환
	Summon_MagicCircle(CBossSkeletonMagicCircle::TYPE_RED, _vec3(4.f, 4.f, 4.f), pGameInstance);
	

	return S_OK;
}

HRESULT CBossSkeleton::SetUp_Phase2BlueSkill(CGameInstance* pGameInstance)
{
	//기본적인 준비
	SetUp_Skill();

	//스킬 시작
	m_bPhase2BlueSkill = true;

	//마법진 소환
	Summon_MagicCircle(CBossSkeletonMagicCircle::TYPE_BLUE, _vec3(4.f, 4.f, 4.f), pGameInstance);

	return S_OK;
}

HRESULT CBossSkeleton::SetUp_Phase2GreenSkill(CGameInstance* pGameInstance)
{
	if (pGameInstance == nullptr)
		MSG_FAIL("CBossSkeleton::Phase2GreenSkill - m_pGameInstance == nullptr");

	//기본적인 준비
	SetUp_Skill();

	//스킬 시작
	m_bPhase2GreenSkill = true;
	m_bPhase2GreenSkillFirst = true;

	//마법진 소환
	Summon_MagicCircle(CBossSkeletonMagicCircle::TYPE_GREEN, _vec3(4.f, 4.f, 4.f), pGameInstance);

	return S_OK;
}

HRESULT CBossSkeleton::Phase1RedSkill(_float fTimeDelta, CGameInstance* pGameInstance)
{
	//해당 행동 중이 아님
	if (!m_bPhase1RedSkill)
		return S_OK;

	//대기 시간
	m_tPhase1RedSkillStartTime.fTime += fTimeDelta;
	if (m_tPhase1RedSkillStartTime.fDelay > m_tPhase1RedSkillStartTime.fTime)
		return S_OK;

	if (pGameInstance == nullptr)
		MSG_FAIL("CBossSkeleton::Phase1RedSkill - m_pGameInstance == nullptr");

	//플레이어 위치 파악해서 4방에서 조여들어오게
	_vec3 vPlayerPos = m_pTransformPlayer->Get_State(CTransform::STATE_POS);

	_vec3 vCubePos[4] = { vPlayerPos + _vec3(-32.f, 0.f, 0.f), vPlayerPos + _vec3(0.f, 0.f, 32.f),
		vPlayerPos + _vec3(32.f, 0.f, 0.f), vPlayerPos + _vec3(0.f, 0.f, -32.f) };

	//큐브 정보 세팅
	CBossSkeletonStageCubeAttack::DESC tDesc;
	tDesc.eType = CBossSkeletonStageCubeAttack::TYPE_NORMAL_WEAK;
	tDesc.fPower = 5.f;

	OBJ_INFO tObjInfo;
	tObjInfo.dwColliderColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	lstrcpy(tObjInfo.szTextureName, pTagPrototypeTextureBossSkeletonStageCubeAttack);
	tObjInfo.vColliderCenter = _vec3(0.f, 0.f, 0.f);
	tObjInfo.vColliderSize = _vec3(1.f, 1.f, 1.f);
	tObjInfo.SetUp_Scale(32.f, 32.f, 32.f);

	for (int i = 0; i < 4; ++i)
	{
		tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, vCubePos[i]);
		tObjInfo.SetUp_Rotation(_vec3(0.f, 1.f, 0.f), 90.f * i + 90.f);
		tDesc.tObjInfo = tObjInfo;

		if (pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_SKELETON, pTagPrototypeBossSkeletonStageCubeAttack, pTagLayerBossSkeletonCube, &tDesc))
			MSG_FAIL("CBossSkeleton::Tick - pTagPrototypeBossSkeletonThorn");
	}

	//빨강 패턴 끝
	m_bPhase1RedSkill = false;
	m_tPhase1RedSkillStartTime.fTime = 0.f;
	m_bStateLock = false;

	return S_OK;
}

HRESULT CBossSkeleton::Phase1BlueSkill(_float fTimeDelta, CGameInstance* pGameInstance)
{
	//해당 행동 중이 아님
	if (!m_bPhase1BlueSkill)
		return S_OK;

	m_tPhase1BlueSkillStartTime.Tick_Time(fTimeDelta);
	if (!m_tPhase1BlueSkillStartTime.Check_Time())
		return S_OK;

	if (pGameInstance == nullptr)
		MSG_FAIL("CBossSkeleton::Phase2GreenSkill - m_pGameInstance == nullptr");

	//모든 타일 일정 시간 동안 용암으로 변경
	CEnvironmentMgr::Get_Instance()->Add_Env(ENV_MAGMA);
	CEnvironmentMgr::Get_Instance()->Remove_Env(ENV_MAGMA);

	m_bPhase1BlueSkill = false;
	m_tPhase1BlueSkillStartTime.fTime = 0.f;
	m_bStateLock = false;

	return S_OK;
}

HRESULT CBossSkeleton::Phase1GreenSkill(_float fTimeDelta, CGameInstance* pGameInstance)
{
	//해당 행동 중이 아님
	if (!m_bPhase1GreenSkill)
		return S_OK;

	m_tPhase1GreenSkillStartTime.Tick_Time(fTimeDelta);
	if (!m_tPhase1GreenSkillStartTime.Check_Time())
		return S_OK;

	if (pGameInstance == nullptr)
		MSG_FAIL("CBossSkeleton::Phase2GreenSkill - m_pGameInstance == nullptr");

	//가시 소환
	CBossSkeletonThorn::DESC tDesc;
	tDesc.tSuperDesc.eTextureLevel = LEVEL_BOSS_SKELETON;
	tDesc.tSuperDesc.tTransformDesc.fSpeed = 10.f;
	tDesc.fDelay = 1.f;
	OBJ_INFO tObjInfo;
	tObjInfo.dwColliderColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	lstrcpy(tObjInfo.szTextureName, pTagPrototypeTextureBossSkeletonThorn);
	tObjInfo.vColliderCenter = _vec3(0.f, 0.5f, 0.f);
	tObjInfo.vColliderSize = _vec3(1.f, 1.f, 1.f);
	tObjInfo.SetUp_Scale(3.f, 6.f, 3.f);

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			_vec3 vPlayerPos = m_pTransformPlayer->Get_State(CTransform::STATE_POS);
			vPlayerPos.y = 0.f;
			_vec3 vThornPos = vPlayerPos + _vec3(i * 3.f, 0.f, j * 3.f)
				- _vec3(7.5f, 6.f, 7.5f);
			tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, vThornPos);
			tDesc.tSuperDesc.tObjInfo = tObjInfo;
			if (pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeBossSkeletonThorn, pTagLayerMonsterAttack, &tDesc))
				MSG_FAIL("CBossSkeleton::Tick - pTagPrototypeBossSkeletonThorn");
		}
	}

	m_tPhase1GreenSkillStartTime.fTime = 0.f;
	m_bPhase1GreenSkill = false;
	m_bStateLock = false;

	return S_OK;
}

HRESULT CBossSkeleton::Phase2RedSkill(_float fTimeDelta, CGameInstance* pGameInstance)
{
	//해당 행동 중이 아님
	if (!m_bPhase2RedSkill)
		return S_OK;

	m_tPhase2RedSkillStartTime.Tick_Time(fTimeDelta);
	if (!m_tPhase2RedSkillStartTime.Check_Time())
		return S_OK;


	if (pGameInstance == nullptr)
		MSG_FAIL("CBossSkeleton::Phase2GreenSkill - m_pGameInstance == nullptr");

	//플레이어 위치 추적하는 바위 소환
	CBossSkeletonStageCubeAttack::DESC tDesc;
	tDesc.eType = CBossSkeletonStageCubeAttack::TYPE_NORMAL_STONE;
	tDesc.tObjInfo.dwColliderColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tDesc.tObjInfo.SetUp_Scale(32.f, 32.f, 32.f);
	_vec3 vPos = m_pTransform->Get_State(CTransform::STATE_POS);
	vPos.y += tDesc.tObjInfo.Get_Scale().y;
	tDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, vPos);

	lstrcpy(tDesc.tObjInfo.szTextureName, pTagPrototypeTextureBossSkeletonStageCubeAttack);
	tDesc.tObjInfo.vColliderCenter = _vec3(0.f, 0.f, 0.f);
	tDesc.tObjInfo.vColliderSize = _vec3(1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_SKELETON, pTagPrototypeBossSkeletonStageCubeAttack
		, pTagLayerMonsterAttack, &tDesc)))
		MSG_MINUSONE("CBossSkeleton::Tick - Add_GameObjectToLayer ");

	m_tPhase2RedSkillStartTime.fTime = 0.f;
	m_bPhase2RedSkill = false;
	m_bStateLock = false;

	return S_OK;
}

HRESULT CBossSkeleton::Phase2BlueSkill(_float fTimeDelta, CGameInstance* pGameInstance)
{
	//해당 행동 중이 아님
	if (!m_bPhase2BlueSkill)
		return S_OK;

	m_tPhase2BlueSkillStartTime.Tick_Time(fTimeDelta);
	if (!m_tPhase2BlueSkillStartTime.Check_Time())
		return S_OK;

	if (pGameInstance == nullptr)
		MSG_FAIL("CBossSkeleton::Phase2GreenSkill - m_pGameInstance == nullptr");

	//용암 4개 소환
	//플레이어 위치 파악해서 4방에서 조여들어오게
	_vec3 vPlayerPos = m_pTransformPlayer->Get_State(CTransform::STATE_POS);

	_vec3 vCubePos[4] = { vPlayerPos + _vec3(-32.f, 0.f, 0.f), vPlayerPos + _vec3(0.f, 0.f, 32.f),
		vPlayerPos + _vec3(32.f, 0.f, 0.f), vPlayerPos + _vec3(0.f, 0.f, -32.f) };

	//큐브 정보 세팅
	CBossSkeletonStageCubeAttack::DESC tDesc;
	tDesc.eType = CBossSkeletonStageCubeAttack::TYPE_MAGMA;
	tDesc.fPower = 2.5f;

	OBJ_INFO tObjInfo;
	tObjInfo.dwColliderColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	lstrcpy(tObjInfo.szTextureName, pTagPrototypeTextureBossSkeletonStageCubeAttack);
	tObjInfo.vColliderCenter = _vec3(0.f, 0.f, 0.f);
	tObjInfo.vColliderSize = _vec3(1.f, 1.f, 1.f);
	tObjInfo.SetUp_Scale(32.f, 32.f, 32.f);

	for (int i = 0; i < 4; ++i)
	{
		tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, vCubePos[i]);
		tObjInfo.SetUp_Rotation(_vec3(0.f, 1.f, 0.f), 90.f * i + 90.f);
		tDesc.tObjInfo = tObjInfo;

		if (pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_SKELETON, pTagPrototypeBossSkeletonStageCubeAttack, pTagLayerBossSkeletonCube, &tDesc))
			MSG_FAIL("CBossSkeleton::Tick - pTagPrototypeBossSkeletonThorn");
	}

	m_tPhase2BlueSkillStartTime.fTime = 0.f;
	m_bPhase2BlueSkill = false;
	m_bStateLock = false;

	return S_OK;
}

HRESULT CBossSkeleton::Phase2GreenSkill(_float fTimeDelta, CGameInstance* pGameInstance)
{
	//해당 행동 중이 아님
	if (!m_bPhase2GreenSkill)
		return S_OK;

	m_tPhase2GreenSkillStartTime.Tick_Time(fTimeDelta);
	if (!m_tPhase2GreenSkillStartTime.Check_Time())
		return S_OK;

	if (pGameInstance == nullptr)
		MSG_FAIL("CBossSkeleton::Phase2GreenSkill - m_pGameInstance == nullptr");

	if (m_bPhase2GreenSkillFirst)
	{
		//플레이어 위치로 물 던짐
		CBossSkeletonStageCubeAttack::DESC tDesc;
		tDesc.eType = CBossSkeletonStageCubeAttack::TYPE_WET;
		tDesc.fPower = 10.f;

		OBJ_INFO tObjInfo;
		tObjInfo.dwColliderColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		lstrcpy(tObjInfo.szTextureName, pTagPrototypeTextureBossSkeletonStageCubeAttack);
		tObjInfo.vColliderCenter = _vec3(0.f, 0.f, 0.f);
		tObjInfo.vColliderSize = _vec3(1.f, 1.f, 1.f);
		tObjInfo.SetUp_Scale(4.f, 4.f, 4.f);
		tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, m_pTransform->Get_State(CTransform::STATE_POS) + _vec3(0.f, 2.f, 0.f));
		tDesc.tObjInfo = tObjInfo;

		if (pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_SKELETON, pTagPrototypeBossSkeletonStageCubeAttack, pTagLayerBossSkeletonCube, &tDesc))
			MSG_FAIL("CBossSkeleton::Tick - pTagPrototypeBossSkeletonThorn");

		m_bPhase2GreenSkillFirst = false;
	}

	m_tPhase2GreenSkillThornTime.Tick_Time(fTimeDelta);
	if (!m_tPhase2GreenSkillThornTime.Check_Time())
		return S_OK;

	//일정 시간 뒤에 가시소환 + 용암
	CBossSkeletonThorn::DESC tThornDesc;
	tThornDesc.tSuperDesc.eTextureLevel = LEVEL_BOSS_SKELETON;
	tThornDesc.tSuperDesc.tTransformDesc.fSpeed = 10.f;
	tThornDesc.fDelay = 1.f;
	OBJ_INFO tObjInfoThorn;
	tObjInfoThorn.dwColliderColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	lstrcpy(tObjInfoThorn.szTextureName, pTagPrototypeTextureBossSkeletonThorn);
	tObjInfoThorn.vColliderCenter = _vec3(0.f, 0.5f, 0.f);
	tObjInfoThorn.vColliderSize = _vec3(1.f, 1.f, 1.f);
	tObjInfoThorn.SetUp_Scale(3.f, 6.f, 3.f);

	for (int i = 0; i < 17; ++i)
	{
		for (int j = 0; j < 17; ++j)
		{
			_vec3 vPlayerPos = m_pTransformPlayer->Get_State(CTransform::STATE_POS);
			vPlayerPos.y = 0.f;
			_vec3 vThornPos = vPlayerPos + _vec3(i * 3.f, 0.f, j * 3.f)
				- _vec3(25.5f, 6.f, 25.5f);
			tObjInfoThorn.SetUp_State(OBJ_INFO::STATE_POS, vThornPos);
			tThornDesc.tSuperDesc.tObjInfo = tObjInfoThorn;
			if (pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeBossSkeletonThorn, pTagLayerMonsterAttack, &tThornDesc))
				MSG_FAIL("CBossSkeleton::Tick - pTagPrototypeBossSkeletonThorn");
		}
	}

	CEnvironmentMgr::Get_Instance()->Add_Env(ENV_MAGMA);
	CEnvironmentMgr::Get_Instance()->Remove_Env(ENV_MAGMA);
	

	m_tPhase2GreenSkillStartTime.fTime = 0.f;
	m_tPhase2GreenSkillThornTime.fTime = 0.f;
	m_bPhase2GreenSkill = false;
	m_bStateLock = false;

	return S_OK;
}

HRESULT CBossSkeleton::Summon_MagicCircle(_int iType, _vec3 vScale, CGameInstance* pGameInstance)
{
	if (pGameInstance == nullptr)
		MSG_FAIL("CBossSkeleton::Summon_MagicCircle - m_pGameInstance == nullptr");

	//마법진 소환
	CBossSkeletonMagicCircle::DESC tDesc;
	tDesc.eType = CBossSkeletonMagicCircle::TYPE(iType);
	_vec3 vBossPos = m_pTransform->Get_State(CTransform::STATE_POS);
	//플레이어 방향으로
	_vec3 vBossToPlayerDir = m_pTransformPlayer->Get_State(CTransform::STATE_POS) - vBossPos;
	tDesc.vPos = vBossPos + *D3DXVec3Normalize(&vBossToPlayerDir, &vBossToPlayerDir) * m_fMagicCircleCreateDistance;
	tDesc.vScale = vScale * 8.f;
	tDesc.vPos.y += tDesc.vScale.y * 0.75f;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"PrototypeBossSkeletonMagicCircle",
		L"LayerEffect", &tDesc)))
		MSG_FAIL("CBossSkeleton::SetUp_Phase1RedSkill - Add_GameObjectToLayer");

	return S_OK;
}

HRESULT CBossSkeleton::Tick_Collider(_float fTimeDelta)
{
	if (m_pBoxCollider == nullptr || m_pTransform == nullptr)
		MSG_FAIL("CBossSkeleton::Tick_Collider  m_pBoxCollider == nullptr || m_pTransform == nullptr");

	m_pBoxCollider->Set_WorldMatrixFromObject(*m_pTransform->Get_WorldMatrixPtr());
	m_pBoxCollider->Tick(fTimeDelta);

	return S_OK;
}

void CBossSkeleton::Collision(CGameObject * pColObj)
{
	CAttack* pAttack = dynamic_cast<CAttack*>(pColObj);

	DO;
	//충돌한게 공격인 경우
	if (pAttack != nullptr)
	{
		_int iDmg = pAttack->Get_Dmg();
		//데미지가 있으면
		if (iDmg > 0)
			Sub_Hp(iDmg);

		//빙결 속성 공격인 경우
		if (pAttack->Get_Freeze())
		{

		}

		pAttack->Set_Dead();
	}
	WHILE;

}

void CBossSkeleton::Tick_Framing()
{
}

CBossSkeleton * CBossSkeleton::Create(_pDevice pDevice)
{
	CBossSkeleton* pInstance = new CBossSkeleton(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CBossSkeleton Create Failed");
	}

	return pInstance;
}

void CBossSkeleton::Free()
{
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTransform);
	Safe_Release(m_pVIBuffer);

	for (auto& pTexture : m_pTextureArr)
		Safe_Release(pTexture);

	__super::Free();
}

CGameObject * CBossSkeleton::Clone(void * pArg)
{
	CGameObject* pInstance = new CBossSkeleton(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CBossSkeleton Clone Failed");
	}

	return pInstance;
}

_vec3 CBossSkeleton::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxCollider->Get_MinPos();
	return vMinPos;
}

_vec3 CBossSkeleton::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxCollider->Get_MaxPos();
	return vMaxPos;
}

_vec3 CBossSkeleton::Get_WorldPos()
{
	return m_pBoxCollider->Get_ColliderWolrdPos();

}

_vec3 CBossSkeleton::Get_Length()
{
	return m_pTransform->Get_Scale();

}

const _vec3 & CBossSkeleton::Get_BoxColliderCenter() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pBoxCollider->Get_ColliderCenter();
}

void CBossSkeleton::Set_PoisonHitTransform(CPoisonCubeHit::DESC * pDesc)
{
	memcpy(&pDesc->tObjInfo.matWorld, m_pTransform->Get_WorldMatrixPtr(), sizeof(_mat));
}
