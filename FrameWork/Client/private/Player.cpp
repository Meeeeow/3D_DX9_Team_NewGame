#include "stdafx.h"
#include "..\public\Player.h"
#include "GameInstance.h"
#include "Terrain.h"
#include "WallBase.h"
#include "InputDevice.h"
#include "PlayerTagMgr.h"
#include "Observer.h"
#include "Layer.h"
#include "Collision.h"
#include "Attack.h"
#include "TileCube.h"
#include "MagicMissile.h"
#include "CrosshairMgr.h"
#include "ResurrectionManager.h"
#include "TelekinesisManager.h"
#include "Dagger.h"
#include "LockOn.h"
#include "Portal.h"
#include "PoisonCube.h"
#include "PlayerShield.h"
#include "BloodScreen.h"
#include "BossSkeletonStageCube.h"
#include "RunWet.h"


_float CPlayer::m_fTagCoolTime = 0.f;
typedef unordered_map<const _tchar*, class CLayer*> LAYERS;

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{
	ZeroMemory(m_pTextureComArr, sizeof(CTexture*) * FSM_END);
}

CPlayer::CPlayer(const CPlayer & rhs)
	:CGameObject(rhs)
{
	ZeroMemory(m_pTextureComArr, sizeof(CTexture*) * FSM_END);
}

HRESULT CPlayer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CPlayer NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CPlayer::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CPlayer NativeConstruct - __super");

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//플레이어 스탯 넘겨주기
	memcpy(&m_tPlayerStat, pArg, sizeof(PLAYER_STAT));
	if (m_tPlayerStat.eType == TYPE_END)
		MSG_FAIL("CPlayer::NativeConstruct - m_tPlayerStat.eType == TYPE_END");

	if (FAILED(CPlayerTagMgr::Get_Instance()->Set_Player(m_tPlayerStat.eType, this)))
		MSG_FAIL("CPlayer::NativeConstruct - Set_Player");
	CPlayerTagMgr::Get_Instance()->Set_MaxTagCoolTime(m_fTagCoolTimeMax);

	//첫 캐릭터만 따로 처리
	if (m_tPlayerStat.eType == TYPE_WARRIOR)
		m_bControl = true;

	//스킬 설정
	Ready_SkillSet();
	
	if (FAILED(SetUp_Component()))
		MSG_FAIL("CPlayer NativeConstruct - SetUp_Component");

	Check_Frame(0.1f);

	PLAYER_STAT* pObj = (PLAYER_STAT*)pArg;

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, pObj->tObj.vColliderCenter, pObj->tObj.vColliderSize, pObj->tObj.dwColliderColor, this);

	RELEASE_INSTANCE(CGameInstance);

	m_tSkillCoolTime.fCooltime1 = m_tPlayerSkill.fCooltime1;
	m_tSkillCoolTime.fCooltime2 = m_tPlayerSkill.fCooltime2;
	m_tSkillCoolTime.fCooltime3 = m_tPlayerSkill.fCooltime3;

	m_iSkillMPCostArr[0] = 1;
	m_iSkillMPCostArr[1] = 20;
	m_iSkillMPCostArr[2] = 30;

	m_tTimeWarriorSkill3.fDelay = 1.f;

	return S_OK;
}

_int CPlayer::Tick(_float fTimeDelta)
{
	Tick_SkillCoolTime(fTimeDelta);

	//3명의 플레이어 중 컨트롤 가질 수 있는 친구는 1명
	if (m_bControl == false && m_bChangeIn == false && m_bChangeOut == false)
		return OBJ_NOEVENT;

	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CPlayer Tick - __super");

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	Tick_TagCoolTime(fTimeDelta);
	m_pTransformCom->Tick_Debuff(fTimeDelta);

	if (pInstance->Key_Down(DIK_9))
		pInstance->Set_ColliderRenderState(false);
	if (pInstance->Key_Down(DIK_0))
		pInstance->Set_ColliderRenderState(true);

	if (pInstance->Get_CurrentCameraID() == CAMERA_BACK)
	{
		//키보드 입력
		Check_KeyBoard(fTimeDelta, pInstance);
		//마우스 입력
		Check_Mouse(fTimeDelta, pInstance);
		Tick_ChangeIn(fTimeDelta);
		Tick_ChangeOut(fTimeDelta);
	}
	else if (pInstance->Get_CurrentCameraID() == CAMERA_2D)
	{
		Check_KeyBoard2D(fTimeDelta, pInstance);
		Tick_ChangeIn2D(fTimeDelta);
		Tick_ChangeOut2D(fTimeDelta);
	}
	Check_Frame(fTimeDelta);
	//텍스쳐 프레임 갱신
	Tick_Frame(fTimeDelta, pInstance);

	//y 좌표 이동
	//지형 태우기
	Collision_Terrain(fTimeDelta, pInstance);
	//점프
	Jump(fTimeDelta,pInstance);
	Run(fTimeDelta, pInstance);
	Fall(fTimeDelta, pInstance);

	//스킬
	Tick_WarriorSkill2(fTimeDelta, pInstance);
	Tick_WarriorSkill3(fTimeDelta, pInstance);



	m_pBoxCollider->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxCollider->Tick(fTimeDelta);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

_int CPlayer::LateTick(_float fTimeDelta)
{
	//3명의 플레이어 중 컨트롤 가질 수 있는 친구는 1명 // 들어오는 친구랑 나가는 친구는 위치 고정이 되면 안됨
	if (m_bControl == false && m_bChangeIn == false && m_bChangeOut == false)
	{
		//컨트롤 하는 친구와 위치 동기화
		CTransform* pCurCtrlPlayerTransform = CPlayerTagMgr::Get_Instance()->Get_CurPlayerTransform();

		//모든 스테이트를 받아 넣어줄건데 혹시나 플레이어 스케일이 각자 다를까봐 스케일만 따로 꺼내옴
		_vec3 vScale = m_pTransformCom->Get_Scale();

		m_pTransformCom->Set_Matrix(*pCurCtrlPlayerTransform->Get_WorldMatrixPtr());
		m_pTransformCom->SetUp_Scale(vScale.x, vScale.y, vScale.z);

		return OBJ_NOEVENT;
	}

	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CPlayer LateTick - __super");


	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_NONALPHA, this)))
		MSG_MINUSONE("CPlayer LateTick - Add_RenderGroup");

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	Check_Camera(fTimeDelta, pInstance);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
}

HRESULT CPlayer::Render()
{
	//3명의 플레이어 중 컨트롤 가질 수 있는 친구는 1명
	if (m_bControl == false && m_bChangeOut == false && m_bChangeIn == false)
		return S_OK;

	if (FAILED(__super::Render()))
		MSG_FAIL("CPlayer Render - __super");

	if (m_pRendererCom == nullptr)
		MSG_FAIL("CPlayer Render - m_pRendererCom is null");

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CPlayer Render - m_pVIBufferCom is null");

	if (m_pTextureComArr == nullptr)
		MSG_FAIL("CPlayer Render - m_pTextureCom is null");

	//트랜스폼 스케일 돌려야함
	if (m_eDirKey == DIR_KEY::DIR_SW || m_eDirKey == DIR_KEY::DIR_W || m_eDirKey == DIR_KEY::DIR_NW)
		m_pTransformCom->Invert_Rectangle();

	
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	//pInstance->Draw_Font(wstr.c_str(), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	m_pTransformCom->Bind_OnDevice();

	//트랜스폼 스케일 돌려야함
	if (m_eDirKey == DIR_KEY::DIR_SW || m_eDirKey == DIR_KEY::DIR_W || m_eDirKey == DIR_KEY::DIR_NW)
		m_pTransformCom->Invert_Rectangle();

	m_pTextureComArr[m_eCurState]->Bind_OnDevice(m_tFrame.iStart);
	SetUp_RenderState();

	m_pVIBufferCom->Render();

	m_pBoxCollider->Render();

	RELEASE_INSTANCE(CGameInstance);

	Release_RenderState();
	return S_OK;
}

void CPlayer::Add_Bound(_vec3 vBoundPos)
{
	m_pTransformCom->Add_Position(vBoundPos);
}

void CPlayer::Set_Control(_bool bControl)
{
	//현재 컨트롤 중이다가 교체 되는 경우
	if (m_bControl && !bControl)
	{
		m_bChangeOut = true;
	}
	//앞으로 컨트롤 할 친구
	else if (!m_bControl && bControl)
	{
		m_bChangeIn = true;
		m_fTagCoolTime = 0.f;
		_vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
		_vec3 vDir = m_pTransformCom->Get_State(CTransform::STATE_LOOK) + m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		vPos -= m_fChangeTargetMaxDist * (*D3DXVec3Normalize(&vDir, &vDir));
		m_pTransformCom->SetUp_State(CTransform::STATE_POS, vPos);
	}
}

void CPlayer::Sub_Hp(_uint iDmg)
{
	//무적 상태일때 무시
	if (m_bInvincible)
		return;

	//컨트롤인 친구만 체력 담
	if (!m_bControl)
		return;

	m_tPlayerStat.iHp -= iDmg;
	if (m_tPlayerStat.iHp <= 0)
		m_tPlayerStat.iHp = 0;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CBloodScreen::DESC tBloodScreenDesc;
	tBloodScreenDesc.tSuperDesc.eTextureLevel = LEVEL_STATIC;
	D3DXMatrixIdentity(&tBloodScreenDesc.tSuperDesc.tObjInfo.matWorld);
	tBloodScreenDesc.tSuperDesc.tObjInfo.SetUp_Scale(_float(g_iWinCX), _float(g_iWinCY), 1.f);
	tBloodScreenDesc.tSuperDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, _vec3(_float(g_iWinCX>>1), _float(g_iWinCY>>1), 0.f));
	lstrcpy(tBloodScreenDesc.tSuperDesc.tObjInfo.szTextureName, pTagPrototypeTextureBloodScreen);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeBloodScreen
		, pTagLayerNonCollision, &tBloodScreenDesc)))
		MSG_VOID("CPlayer::Sub_Hp - pTagPrototypeBloodScreen");
	RELEASE_INSTANCE(CGameInstance);

	Call_StatObserverChange();
}

_bool CPlayer::Sub_Mp(_uint iMp)
{
	if (m_tPlayerStat.iMp >= (_int)iMp)
	{
		m_tPlayerStat.iMp -= (_int)iMp;
		Call_StatObserverChange();
		return true;
	}

	return false;
}

void CPlayer::Set_State(STATE_KEY eStateKey, DIR_KEY eDirKey)
{
	if (m_bStateLock && !m_bJump)
		return;

	if (!m_bJump || m_eStateKey != STATE_JUMP)
		m_eStateKey = eStateKey;

	m_eDirKey = eDirKey;

	switch (m_eStateKey)
	{
		//대기 상태
	case Client::CPlayer::STATE_IDLE:
		Set_CurState(FSM_IDLE_N);
		break;

		//점프
	case Client::CPlayer::STATE_JUMP:
	{
		switch (m_eDirKey)
		{
		case Client::CPlayer::DIR_N:
			Set_CurState(FSM_SKILL_N);
			break;
		case Client::CPlayer::DIR_NE:
			Set_CurState(FSM_SKILL_NE);
			break;
		case Client::CPlayer::DIR_E:
			Set_CurState(FSM_SKILL_E);
			break;
		case Client::CPlayer::DIR_SE:
			Set_CurState(FSM_SKILL_SE);
			break;
		case Client::CPlayer::DIR_S:
			Set_CurState(FSM_SKILL_S);
			break;
		case Client::CPlayer::DIR_SW:
			Set_CurState(FSM_SKILL_SE);
			break;
		case Client::CPlayer::DIR_W:
			Set_CurState(FSM_SKILL_E);
			break;
		case Client::CPlayer::DIR_NW:
			Set_CurState(FSM_SKILL_NE);
			break;
		case Client::CPlayer::DIR_END:
			Set_CurState(FSM_SKILL_N);
			break;
		}
	}
	break;

		//달리기
	case Client::CPlayer::STATE_RUN:
		switch (m_eDirKey)
		{
		case Client::CPlayer::DIR_N:
			Set_CurState(FSM_RUN_N);
			break;
		case Client::CPlayer::DIR_NE:
			Set_CurState(FSM_RUN_NE);
			break;
		case Client::CPlayer::DIR_E:
			Set_CurState(FSM_RUN_E);
			break;
		case Client::CPlayer::DIR_SE:
			Set_CurState(FSM_RUN_SE);
			break;
		case Client::CPlayer::DIR_S:
			Set_CurState(FSM_RUN_S);
			break;
		case Client::CPlayer::DIR_SW:
			Set_CurState(FSM_RUN_SE);
			break;
		case Client::CPlayer::DIR_W:
			Set_CurState(FSM_RUN_E);
			break;
		case Client::CPlayer::DIR_NW:
			Set_CurState(FSM_RUN_NE);
			break;
		case Client::CPlayer::DIR_END:
			Set_CurState(FSM_RUN_N);
			break;
		}
		break;

		//스킬 1
	case Client::CPlayer::STATE_SKILL1:

		if (m_tPlayerStat.eType == TYPE_THIEF)
		{
			Set_CurState(FSM_THROW);
		}
		else
		{
			switch (m_eDirKey)
			{
			case Client::CPlayer::DIR_N:
				Set_CurState(FSM_SKILL_N);
				break;
			case Client::CPlayer::DIR_NE:
				Set_CurState(FSM_SKILL_NE);
				break;
			case Client::CPlayer::DIR_E:
				Set_CurState(FSM_SKILL_E);
				break;
			case Client::CPlayer::DIR_SE:
				Set_CurState(FSM_SKILL_SE);
				break;
			case Client::CPlayer::DIR_S:
				Set_CurState(FSM_SKILL_S);
				break;
			case Client::CPlayer::DIR_SW:
				Set_CurState(FSM_SKILL_SE);
				break;
			case Client::CPlayer::DIR_W:
				Set_CurState(FSM_SKILL_E);
				break;
			case Client::CPlayer::DIR_NW:
				Set_CurState(FSM_SKILL_NE);
				break;
			case Client::CPlayer::DIR_END:
				Set_CurState(FSM_SKILL_N);
				break;
			}
		}
		
		break;

		//스킬 2
	case Client::CPlayer::STATE_SKILL2:
		//전사 2스킬은 모션을 달리기로 할듯
		if (m_tPlayerStat.eType == TYPE_WARRIOR)
		{
			switch (m_eDirKey)
			{
			case Client::CPlayer::DIR_N:
				Set_CurState(FSM_RUN_N);
				break;
			case Client::CPlayer::DIR_NE:
				Set_CurState(FSM_RUN_NE);
				break;
			case Client::CPlayer::DIR_E:
				Set_CurState(FSM_RUN_E);
				break;
			case Client::CPlayer::DIR_SE:
				Set_CurState(FSM_RUN_SE);
				break;
			case Client::CPlayer::DIR_S:
				Set_CurState(FSM_RUN_S);
				break;
			case Client::CPlayer::DIR_SW:
				Set_CurState(FSM_RUN_SE);
				break;
			case Client::CPlayer::DIR_W:
				Set_CurState(FSM_RUN_E);
				break;
			case Client::CPlayer::DIR_NW:
				Set_CurState(FSM_RUN_NE);
				break;
			case Client::CPlayer::DIR_END:
				Set_CurState(FSM_RUN_N);
				break;
			}
		}
		//도적
		else if (m_tPlayerStat.eType == TYPE_THIEF)
		{
			Set_CurState(FSM_THROW);
			break;
		}
		//법사
		else
		{
			switch (m_eDirKey)
			{
			case Client::CPlayer::DIR_N:
				Set_CurState(FSM_SKILL_N);
				break;
			case Client::CPlayer::DIR_NE:
				Set_CurState(FSM_SKILL_NE);
				break;
			case Client::CPlayer::DIR_E:
				Set_CurState(FSM_SKILL_E);
				break;
			case Client::CPlayer::DIR_SE:
				Set_CurState(FSM_SKILL_SE);
				break;
			case Client::CPlayer::DIR_S:
				Set_CurState(FSM_SKILL_S);
				break;
			case Client::CPlayer::DIR_SW:
				Set_CurState(FSM_SKILL_SE);
				break;
			case Client::CPlayer::DIR_W:
				Set_CurState(FSM_SKILL_E);
				break;
			case Client::CPlayer::DIR_NW:
				Set_CurState(FSM_SKILL_NE);
				break;
			case Client::CPlayer::DIR_END:
				Set_CurState(FSM_SKILL_N);
				break;
			}
		}
		break;
		//스킬 3
	case Client::CPlayer::STATE_SKILL3:
		if (m_tPlayerStat.eType == TYPE_WARRIOR)
		{
			switch (m_eDirKey)
			{
			case Client::CPlayer::DIR_N:
				Set_CurState(FSM_SKILL_N);
				break;
			case Client::CPlayer::DIR_NE:
				Set_CurState(FSM_SKILL_NE);
				break;
			case Client::CPlayer::DIR_E:
				Set_CurState(FSM_SKILL_E);
				break;
			case Client::CPlayer::DIR_SE:
				Set_CurState(FSM_SKILL_SE);
				break;
			case Client::CPlayer::DIR_S:
				Set_CurState(FSM_SKILL_S);
				break;
			case Client::CPlayer::DIR_SW:
				Set_CurState(FSM_SKILL_SE);
				break;
			case Client::CPlayer::DIR_W:
				Set_CurState(FSM_SKILL_E);
				break;
			case Client::CPlayer::DIR_NW:
				Set_CurState(FSM_SKILL_NE);
				break;
			case Client::CPlayer::DIR_END:
				Set_CurState(FSM_SKILL_N);
				break;
			}
		}
		//도적
		else if (m_tPlayerStat.eType == TYPE_THIEF)
		{

		}
		//법사
		else
		{
			switch (m_eDirKey)
			{
			case Client::CPlayer::DIR_N:
				Set_CurState(FSM_SKILL_N);
				break;
			case Client::CPlayer::DIR_NE:
				Set_CurState(FSM_SKILL_NE);
				break;
			case Client::CPlayer::DIR_E:
				Set_CurState(FSM_SKILL_E);
				break;
			case Client::CPlayer::DIR_SE:
				Set_CurState(FSM_SKILL_SE);
				break;
			case Client::CPlayer::DIR_S:
				Set_CurState(FSM_SKILL_S);
				break;
			case Client::CPlayer::DIR_SW:
				Set_CurState(FSM_SKILL_SE);
				break;
			case Client::CPlayer::DIR_W:
				Set_CurState(FSM_SKILL_E);
				break;
			case Client::CPlayer::DIR_NW:
				Set_CurState(FSM_SKILL_NE);
				break;
			case Client::CPlayer::DIR_END:
				Set_CurState(FSM_SKILL_N);
				break;
			}
		}
		break;
	}
}

void CPlayer::Set_CurState(PLAYER_FSM eFSM)
{

	m_eCurState = eFSM;
}

void CPlayer::Set_Idle()
{
	m_bStateLock = false;
	Set_State(STATE_KEY::STATE_IDLE, DIR_KEY::DIR_N);
	Check_Frame(0.01f);
}

void CPlayer::Set_JumpEnd()
{
	m_bJump = false;
	m_bJumpUp = false;
}

const _vec3 & CPlayer::Get_ColliderCenter() const
{
	return m_pBoxCollider->Get_ColliderCenter();
}

HRESULT CPlayer::SetUp_Component()
{
	CTransform::TRANSFORM_DESC tTransDesc;
	tTransDesc.fDegree = 5.f;
	tTransDesc.fSpeed = 10.f;
	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransformCom, &tTransDesc)))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PlayerBuffer"), pTagComVIBuffer, (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pVIBufferCom");

	// 스텟 서브젝트 추가
	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeSubject, TEXT("ComStatSubject"), (CComponent**)&m_pStatSubjectCom)))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pStatSubjectCom");

	// 스킬 서브젝트 추가
	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeSubject, TEXT("ComSkillSubject"), (CComponent**)&m_pSkillSubjectCom)))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pSkillSubjectCom");

	switch (m_tPlayerStat.eType)
	{
	case TYPE_WARRIOR:
		if (FAILED(Ready_TextureWarrior()))
			MSG_FAIL("CPlayer::SetUp_Component - Ready_TextureWarrior");
		break;
	case TYPE_MAGE:
		if (FAILED(Ready_TextureMage()))
			MSG_FAIL("CPlayer::SetUp_Component - Ready_TextureMage");
		break;
	case TYPE_THIEF:
		if (FAILED(Ready_TextureThief()))
			MSG_FAIL("CPlayer::SetUp_Component - Ready_TextureThief");
		break;
	}

	return S_OK;
}

HRESULT CPlayer::Ready_TextureWarrior()
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureWarriorRunE"), TEXT("ComTextureRunE"), (CComponent**)&m_pTextureComArr[FSM_RUN_E])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureWarriorRunE");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureWarriorRunN"), TEXT("ComTextureRunN"), (CComponent**)&m_pTextureComArr[FSM_RUN_N])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureWarriorRunN");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureWarriorRunNE"), TEXT("ComTextureRunNE"), (CComponent**)&m_pTextureComArr[FSM_RUN_NE])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureWarriorRunNE");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureWarriorRunSE"), TEXT("ComTextureRunSE"), (CComponent**)&m_pTextureComArr[FSM_RUN_SE])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureWarriorRunSE");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureWarriorRunS"), TEXT("ComTextureRunS"), (CComponent**)&m_pTextureComArr[FSM_RUN_S])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureWarriorRunS");


	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureWarriorSkillE"), TEXT("ComTextureSkillE"), (CComponent**)&m_pTextureComArr[FSM_SKILL_E])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureWarriorSkillE");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureWarriorSkillS"), TEXT("ComTextureSkillS"), (CComponent**)&m_pTextureComArr[FSM_SKILL_S])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureWarriorSkillS");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureWarriorSkillN"), TEXT("ComTextureSkillN"), (CComponent**)&m_pTextureComArr[FSM_SKILL_N])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureWarriorSkillN");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureWarriorSkillNE"), TEXT("ComTextureSkillNE"), (CComponent**)&m_pTextureComArr[FSM_SKILL_NE])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureWarriorSkillNE");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureWarriorSkillSE"), TEXT("ComTextureSkillSE"), (CComponent**)&m_pTextureComArr[FSM_SKILL_SE])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureWarriorSkillSE");


	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureWarriorIdleE"), TEXT("ComTextureIdleE"), (CComponent**)&m_pTextureComArr[FSM_IDLE_E])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureWarriorIdleE");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureWarriorIdleS"), TEXT("ComTextureIdleS"), (CComponent**)&m_pTextureComArr[FSM_IDLE_S])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureWarriorIdleS");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureWarriorIdleN"), TEXT("ComTextureIdleN"), (CComponent**)&m_pTextureComArr[FSM_IDLE_N])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureWarriorIdleN");

	return S_OK;
}

HRESULT CPlayer::Ready_TextureMage()
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureMageRunE"), TEXT("ComTextureRunE"), (CComponent**)&m_pTextureComArr[FSM_RUN_E])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureMageRunE");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureMageRunN"), TEXT("ComTextureRunN"), (CComponent**)&m_pTextureComArr[FSM_RUN_N])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureMageRunN");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureMageRunNE"), TEXT("ComTextureRunNE"), (CComponent**)&m_pTextureComArr[FSM_RUN_NE])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureMageRunNE");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureMageRunSE"), TEXT("ComTextureRunSE"), (CComponent**)&m_pTextureComArr[FSM_RUN_SE])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureMageRunSE");
	
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureMageRunS"), TEXT("ComTextureRunS"), (CComponent**)&m_pTextureComArr[FSM_RUN_S])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureMageRunS");


	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureMageSkillE"), TEXT("ComTextureSkillE"), (CComponent**)&m_pTextureComArr[FSM_SKILL_E])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureMageSkillE");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureMageSkillS"), TEXT("ComTextureSkillS"), (CComponent**)&m_pTextureComArr[FSM_SKILL_S])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureMageSkillS");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureMageSkillN"), TEXT("ComTextureSkillN"), (CComponent**)&m_pTextureComArr[FSM_SKILL_N])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureMageSkillN");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureMageSkillNE"), TEXT("ComTextureSkillNE"), (CComponent**)&m_pTextureComArr[FSM_SKILL_NE])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureMageSkillNE");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureMageSkillSE"), TEXT("ComTextureSkillSE"), (CComponent**)&m_pTextureComArr[FSM_SKILL_SE])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureMageSkillSE");


	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureMageIdleE"), TEXT("ComTextureIdleE"), (CComponent**)&m_pTextureComArr[FSM_IDLE_E])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureMageIdleE");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureMageIdleS"), TEXT("ComTextureIdleS"), (CComponent**)&m_pTextureComArr[FSM_IDLE_S])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureMageIdleS");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureMageIdleN"), TEXT("ComTextureIdleN"), (CComponent**)&m_pTextureComArr[FSM_IDLE_N])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureMageIdleN");

	return S_OK;
}

HRESULT CPlayer::Ready_TextureThief()
{
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureThiefRunE"), TEXT("ComTextureRunE"), (CComponent**)&m_pTextureComArr[FSM_RUN_E])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureThiefRunE");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureThiefRunN"), TEXT("ComTextureRunN"), (CComponent**)&m_pTextureComArr[FSM_RUN_N])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureThiefRunN");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureThiefRunNE"), TEXT("ComTextureRunNE"), (CComponent**)&m_pTextureComArr[FSM_RUN_NE])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureThiefRunNE");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureThiefRunSE"), TEXT("ComTextureRunSE"), (CComponent**)&m_pTextureComArr[FSM_RUN_SE])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureThiefRunSE");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureThiefRunS"), TEXT("ComTextureRunS"), (CComponent**)&m_pTextureComArr[FSM_RUN_S])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureThiefRunS");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureThiefSkillE"), TEXT("ComTextureSkillE"), (CComponent**)&m_pTextureComArr[FSM_SKILL_E])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureThiefSkillE");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureThiefSkillS"), TEXT("ComTextureSkillS"), (CComponent**)&m_pTextureComArr[FSM_SKILL_S])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureThiefSkillS");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureThiefSkillN"), TEXT("ComTextureSkillN"), (CComponent**)&m_pTextureComArr[FSM_SKILL_N])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureThiefSkillN");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureThiefSkillNE"), TEXT("ComTextureSkillNE"), (CComponent**)&m_pTextureComArr[FSM_SKILL_NE])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureThiefSkillNE");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureThiefSkillSE"), TEXT("ComTextureSkillSE"), (CComponent**)&m_pTextureComArr[FSM_SKILL_SE])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureThiefSkillSE");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureThiefIdleE"), TEXT("ComTextureIdleE"), (CComponent**)&m_pTextureComArr[FSM_IDLE_E])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureThiefIdleE");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureThiefIdleS"), TEXT("ComTextureIdleS"), (CComponent**)&m_pTextureComArr[FSM_IDLE_S])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureThiefIdleS");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureThiefIdleN"), TEXT("ComTextureIdleN"), (CComponent**)&m_pTextureComArr[FSM_IDLE_N])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureThiefIdleN");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureThiefThrow"), TEXT("ComTextureThrow"), (CComponent**)&m_pTextureComArr[FSM_THROW])))
		MSG_FAIL("CPlayer SetUp_Component - Add_Component m_pTextureCom PrototypeTextureThiefIdleN");

	return S_OK;
}

HRESULT CPlayer::SetUp_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CPlayer SetUp_RenderState - m_pGraphicDevice is null");

	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CPlayer::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CPlayer Release_RenderState - m_pGraphicDevice is null");

	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_CCW);
	 
	return S_OK;
}

void CPlayer::Ready_SkillSet()
{
	m_tPlayerSkill.eType = m_tPlayerStat.eType;

	switch (m_tPlayerSkill.eType)
	{
	case Client::CPlayer::TYPE_WARRIOR:
		m_tPlayerSkill.fCooltime1 = 3.f;
		m_tPlayerSkill.fCooltime2 = 5.f;
		m_tPlayerSkill.fCooltime3 = 7.f;
		break;
	case Client::CPlayer::TYPE_MAGE:
		m_tPlayerSkill.fCooltime1 = 2.f;
		m_tPlayerSkill.fCooltime2 = 4.f;
		m_tPlayerSkill.fCooltime3 = 8.f;
		break;
	case Client::CPlayer::TYPE_THIEF:
		m_tPlayerSkill.fCooltime1 = 1.f;
		m_tPlayerSkill.fCooltime2 = 3.f;
		m_tPlayerSkill.fCooltime3 = 5.f;
		break;
	}
}

HRESULT CPlayer::Ready_Observers() // 구독후 한번 호출이 필요한 옵저버들의 호출
{
	Call_StatObserverChange();
	Call_SkillObserverChange();
	return S_OK;
}

HRESULT CPlayer::Check_KeyBoard(_float fTimeDelta, CGameInstance* pInstance)
{
	Set_State(STATE_KEY::STATE_IDLE, DIR_KEY::DIR_END);

	//이동 키 관련(wasd, 점프)
	Check_MoveKey(fTimeDelta, pInstance);

	//태그 키 관련
	Check_TagKey(fTimeDelta, pInstance);

	//스킬 키 관련
	Check_SkillKey(fTimeDelta, pInstance);

	return S_OK;
}

HRESULT CPlayer::Check_MoveKey(_float fTimeDelta, CGameInstance* pInstance)
{
	//점프 제외 다른 행동불가 상태일때 무브키 안먹음, 점프말고 이동하면서 할 수 있는게 생기면 추가필요
	if (m_bStateLock && !m_bJump)
		return S_OK;

	if (pInstance->Key_Pressing(DIK_W))
	{
		Set_State(STATE_KEY::STATE_RUN, DIR_KEY::DIR_N);
	}

	if (pInstance->Key_Pressing(DIK_S))
	{
		Set_State(STATE_KEY::STATE_RUN, DIR_KEY::DIR_S);
	}

	if (pInstance->Key_Pressing(DIK_A))
	{
		if (m_eDirKey == DIR_N)
			Set_State(STATE_KEY::STATE_RUN, DIR_NW);
		else if (m_eDirKey == DIR_S)
			Set_State(STATE_KEY::STATE_RUN, DIR_SW);
		else
			Set_State(STATE_KEY::STATE_RUN, DIR_W);
	}

	if (pInstance->Key_Pressing(DIK_D))
	{
		if (m_eDirKey == DIR_N)
			Set_State(STATE_KEY::STATE_RUN, DIR_NE);
		else if (m_eDirKey == DIR_S)
			Set_State(STATE_KEY::STATE_RUN, DIR_SE);
		else
			Set_State(STATE_KEY::STATE_RUN, DIR_KEY::DIR_E);
	}

	if (!m_bStateLock && m_tPlayerStat.eType == TYPE::TYPE_THIEF)
		if (pInstance->Key_Down(DIK_SPACE))
		{
			//점프 관련 변수들 초기화
			m_bJump = true;
			m_bJumpUp = true;
			m_bCollisionTileCube = false;
			Set_State(STATE_KEY::STATE_JUMP, m_eDirKey);
			m_bStateLock = true;
			m_fJumpHighest = 0.f;
			_vec3 vJumpStartPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
			m_fJumpPosY = vJumpStartPos.y;
			m_fJumpTime = 0.f;
		}

	if (pInstance->Key_Down(DIK_K))
	{
		m_tPlayerStat.iHp -= 5;
		Call_StatObserverChange();
	}
	if (pInstance->Key_Down(DIK_L))
	{
		m_tPlayerStat.iMp -= 5;
		Call_StatObserverChange();
	}
	if (pInstance->Key_Down(DIK_N))
	{
		CResurrectionManager::Get_Instance()->Resurrection();
	}
	////////////////////////////////////////////////////////////////

	
	
	return S_OK;
}

HRESULT CPlayer::Check_TagKey(_float fTimeDelta, CGameInstance * pInstance)
{
	if (m_bStateLock)
		return S_OK;

	//전사로 변경
	if (pInstance->Key_Down(DIK_1))
	{
		if (FAILED(CPlayerTagMgr::Get_Instance()->Change_Player(TYPE::TYPE_WARRIOR)))
			MSG_FAIL("CPlayer::Check_TagKey - Change_Player");
	}

	//법사로 변경
	if (pInstance->Key_Down(DIK_2))
	{
		if (FAILED(CPlayerTagMgr::Get_Instance()->Change_Player(TYPE::TYPE_MAGE)))
			MSG_FAIL("CPlayer::Check_TagKey - Change_Player");
	}

	//도적으로 변경
	if (pInstance->Key_Down(DIK_3))
	{
		if (FAILED(CPlayerTagMgr::Get_Instance()->Change_Player(TYPE::TYPE_THIEF)))
			MSG_FAIL("CPlayer::Check_TagKey - Change_Player");
	}

	return S_OK;
}

HRESULT CPlayer::Check_SkillKey(_float fTimeDelta, CGameInstance * pGameInstance)
{
	//보호막 사용중일때 키 때면
	if (m_bWarriorSkill3 && !pGameInstance->Key_Pressing(DIK_R))
		End_WarriorSkill3();

	//행동락 걸려잇을때 다른 행동 못하게 막기
	if (m_bStateLock)
		return S_OK;

	//전사 스킬 제작중
	if (m_tPlayerStat.eType == TYPE_WARRIOR)
	{
		if (m_tPlayerSkill.fCooltime1 <= m_tSkillCoolTime.fCooltime1)
		{
			if (pGameInstance->Key_Down(DIK_Q) && Sub_Mp(m_iSkillMPCostArr[0]))
			{
				Set_State(STATE_KEY::STATE_SKILL1, m_eDirKey);
				m_bStateLock = true;
				m_bWarriorSkill1 = true;
				m_tSkillCoolTime.fCooltime1 = 0.f;
				Call_SkillObserverUse(0);
			}
		}


		if (m_tPlayerSkill.fCooltime2 <= m_tSkillCoolTime.fCooltime2)
			if (pGameInstance->Key_Down(DIK_E) && Sub_Mp(m_iSkillMPCostArr[1]))
		{
			Set_State(STATE_KEY::STATE_SKILL2, m_eDirKey);
			SetUp_WarriorSkill2();
			m_tSkillCoolTime.fCooltime2 = 0.f;
			Call_SkillObserverUse(1);
		}

		if (m_tPlayerSkill.fCooltime3 <= m_tSkillCoolTime.fCooltime3)
			if (pGameInstance->Key_Down(DIK_R) && Sub_Mp(m_iSkillMPCostArr[2]))
		{
			Set_State(STATE_KEY::STATE_SKILL3, m_eDirKey);
			SetUp_WarriorSkill3();
		}
	}

	//법사 빙결 마법
	if (m_tPlayerStat.eType == TYPE_MAGE)
	{
		if (m_tPlayerSkill.fCooltime1 <= m_tSkillCoolTime.fCooltime1)
			if (pGameInstance->Key_Down(DIK_Q) && Sub_Mp(m_iSkillMPCostArr[0]))
			{
				Set_State(STATE_KEY::STATE_SKILL1, m_eDirKey);
				m_bStateLock = true;
				m_bMageSkill1 = true;
				m_tSkillCoolTime.fCooltime1 = 0.f;
				Call_SkillObserverUse(0);
			}

		if (m_tPlayerSkill.fCooltime2 <= m_tSkillCoolTime.fCooltime2)
			if (pGameInstance->Key_Down(DIK_E))
			{
				Set_State(STATE_KEY::STATE_SKILL2, m_eDirKey);
				m_bStateLock = true;
				m_bMageSkill2 = true;
			}

		if (m_tPlayerSkill.fCooltime3 <= m_tSkillCoolTime.fCooltime3)
			if (pGameInstance->Key_Down(DIK_R) && Sub_Mp(m_iSkillMPCostArr[2]))
			{
				Set_State(STATE_KEY::STATE_SKILL3, m_eDirKey);
				m_bStateLock = true;
				m_bMageSkill3 = true;
				m_tSkillCoolTime.fCooltime3 = 0.f;
				Call_SkillObserverUse(2);
			}

	}

	//도적
	if (m_tPlayerStat.eType == TYPE_THIEF)
	{
		if (m_tPlayerSkill.fCooltime1 <= m_tSkillCoolTime.fCooltime1)
			if (pGameInstance->Key_Down(DIK_Q) && Sub_Mp(m_iSkillMPCostArr[0]))
		{
			Set_State(STATE_KEY::STATE_SKILL1, m_eDirKey);
			SetUp_ThiefSkill1();
			m_tSkillCoolTime.fCooltime1 = 0.f;
			Call_SkillObserverUse(0);
		}

		if (m_tPlayerSkill.fCooltime2 <= m_tSkillCoolTime.fCooltime2)
			if (pGameInstance->Key_Down(DIK_E) && Sub_Mp(m_iSkillMPCostArr[1]))
		{
			Set_State(STATE_KEY::STATE_SKILL2, m_eDirKey);
			SetUp_ThiefSkill2();
			m_tSkillCoolTime.fCooltime2 = 0.f;
			Call_SkillObserverUse(1);
		}
	}

	if (pGameInstance->Key_Down(DIK_4))
		m_tPlayerStat.iHp = m_tPlayerStat.iMaxHp;

	if (pGameInstance->Key_Down(DIK_5))
		m_tPlayerStat.iMp = m_tPlayerStat.iMaxMp;

	Check_Frame(fTimeDelta);
	return S_OK;
}

HRESULT CPlayer::Check_Camera(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (pGameInstance->Key_Down(DIK_7))
		pGameInstance->Change_Camera(CAMERA_BACK);

	if (pGameInstance->Key_Down(DIK_8))
		pGameInstance->Change_Camera(CAMERA_2D);

	return S_OK;
}

HRESULT CPlayer::Check_Mouse(_float fTimeDelta, CGameInstance* pInstance)
{
	_long dwMouse = 0;
	if (dwMouse = pInstance->Get_DIMouseMoveState(CInputDevice::DIMM_X))
		m_pTransformCom->Rotation_Axis(_vec3(0.f, 1.f, 0.f), fTimeDelta * dwMouse);



	CTelekinesisManager::Get_Instance()->Telekinesis(fTimeDelta, m_pTransformCom);

	return S_OK;
}

HRESULT CPlayer::Check_KeyBoard2D(_float fTimeDelta, CGameInstance * pInstance)
{
	Set_State2D(STATE_KEY::STATE_IDLE, DIR_KEY::DIR_END);

	m_pTransformCom->SetUp_Rotation(_vec3(0.f, 1.f, 0.f), 0);

	Check_MoveKey2D(fTimeDelta, pInstance);

	Check_TagKey2D(fTimeDelta, pInstance);

	Check_SkillKey2D(fTimeDelta, pInstance);

	return S_OK;
}

HRESULT CPlayer::Check_MoveKey2D(_float fTimeDelta, CGameInstance * pInstance)
{

	if (pInstance->Key_Pressing(DIK_A))
		Set_State2D(STATE_KEY::STATE_RUN, DIR_KEY::DIR_W);

	if (pInstance->Key_Pressing(DIK_D))
		Set_State2D(STATE_KEY::STATE_RUN, DIR_KEY::DIR_E);

	if (m_eStateKey == STATE_RUN)
	{
		m_fJumpSpeed += 0.0085f;
		if (m_fJumpSpeed >= m_fJumpSpeedMax)
			m_fJumpSpeed = m_fJumpSpeedMax;
	}
	else if (m_eStateKey == STATE_JUMP)
	{
		m_fJumpSpeed -= 0.0012f;
		if (m_fJumpSpeed < 0.f)
			m_fJumpSpeed = 0.f;
	}
	else if (m_eStateKey == STATE_IDLE)
		m_fJumpSpeed = 0.f;

	if (!m_bStateLock && m_tPlayerStat.eType == TYPE::TYPE_THIEF)
		if (pInstance->Key_Down(DIK_SPACE))
		{
			//점프 관련 변수들 초기화
			m_bJump = true;
			m_bJumpUp = true;
			Set_State2D(STATE_KEY::STATE_JUMP, m_eDirKey);
			m_bCollisionTileCube = false;
			m_bStateLock = true;
			m_fJumpHighest = 0.f;
			m_fReturnPos = Get_WorldPos();
			_vec3 vJumpStartPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
			m_fJumpPosY = vJumpStartPos.y;
			m_fJumpTime = 0.f;
		}

	return S_OK;
}

HRESULT CPlayer::Check_TagKey2D(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (m_bStateLock)
		return S_OK;

	//전사로 변경
	if (pGameInstance->Key_Down(DIK_1))
	{
		if (FAILED(CPlayerTagMgr::Get_Instance()->Change_Player(TYPE::TYPE_WARRIOR)))
			MSG_FAIL("CPlayer::Check_TagKey2D - Change_Player");
	}

	//법사로 변경
	if (pGameInstance->Key_Down(DIK_2))
	{
		if (FAILED(CPlayerTagMgr::Get_Instance()->Change_Player(TYPE::TYPE_MAGE)))
			MSG_FAIL("CPlayer::Check_TagKey2D - Change_Player");
	}

	//도적으로 변경
	if (pGameInstance->Key_Down(DIK_3))
	{
		if (FAILED(CPlayerTagMgr::Get_Instance()->Change_Player(TYPE::TYPE_THIEF)))
			MSG_FAIL("CPlayer::Check_TagKey2D - Change_Player");
	}

	return S_OK;
}

HRESULT CPlayer::Check_SkillKey2D(_float fTimeDelta, CGameInstance * pGameInstance)
{
	//전사 스킬 제작중
	if (m_tPlayerStat.eType == TYPE_WARRIOR)
	{
		if (pGameInstance->Key_Down(DIK_Q))
		{
			Set_State2D(STATE_KEY::STATE_SKILL1, m_eDirKey);
			m_bStateLock = true;
			m_bWarriorSkill1 = true;
		}
	}

	//법사 빙결 마법
	if (m_tPlayerStat.eType == TYPE_MAGE)
	{
		if (pGameInstance->Key_Down(DIK_Q))
		{
			Set_State2D(STATE_KEY::STATE_SKILL1, m_eDirKey);
			m_bStateLock = true;
			m_bMageSkill1 = true;
		}
	}



	Check_Frame(fTimeDelta);
	return S_OK;
}

void CPlayer::Set_State2D(STATE_KEY eStateKey, DIR_KEY eDirKey)
{
	if (m_bStateLock)
		return;

	m_eStateKey = eStateKey;
	m_eDirKey = eDirKey;
	switch (eStateKey)
	{
	case Client::CPlayer::STATE_IDLE:
		Set_CurState(FSM_IDLE_E);
		break;
	case Client::CPlayer::STATE_RUN:
		switch (eDirKey)
		{
		case Client::CPlayer::DIR_E:
			Set_CurState(FSM_RUN_E);
			break;
		case Client::CPlayer::DIR_W:
			Set_CurState(FSM_RUN_E);
			break;
		case Client::CPlayer::DIR_END:
			Set_CurState(FSM_RUN_E);
			break;
		}
		break;
	case Client::CPlayer::STATE_SKILL1:
		switch (eDirKey)
		{
		case Client::CPlayer::DIR_E:
			Set_CurState(FSM_SKILL_E);
			if (m_tPlayerStat.eType == TYPE_THIEF)
				Set_CurState(FSM_THROW);
			break;
		case Client::CPlayer::DIR_W:
			Set_CurState(FSM_SKILL_E);
			if (m_tPlayerStat.eType == TYPE_THIEF)
				Set_CurState(FSM_THROW);
			break;
		case Client::CPlayer::DIR_END:
			Set_CurState(FSM_SKILL_E);
			if (m_tPlayerStat.eType == TYPE_THIEF)
				Set_CurState(FSM_THROW);
			break;
		}
		break;
	case Client::CPlayer::STATE_JUMP:
		switch (eDirKey)
		{
		case Client::CPlayer::DIR_E:
			Set_CurState(FSM_SKILL_E);
			break;
		case Client::CPlayer::DIR_W:
			Set_CurState(FSM_SKILL_E);
			break;
		case Client::CPlayer::DIR_END:
			Set_CurState(FSM_SKILL_E);
			break;
		}
		break;
	}
}


void CPlayer::Tick_ChangeOut2D(_float fTimeDelta)
{
	if (!m_bChangeOut)
		return;

	m_fChangeTargetDist += m_pTransformCom->Go_Straight(fTimeDelta);
	//목표 위치까지 이동했음
	if (m_fChangeTargetDist >= m_fChangeTargetMaxDist)
	{
		m_bChangeOut = false;
		m_bControl = false;
		m_fChangeTargetDist = 0.f;

		// Stat옵저버
		if (nullptr == m_pStatSubjectCom)
			MSG_VOID("SubjectCom is Null");

		Call_StatObserverChange();
		Call_SkillObserverChange();
	}

	return;
}

void CPlayer::Tick_ChangeIn2D(_float fTimeDelta)
{
	if (!m_bChangeIn)
		return;

	m_fChangeTargetDist += m_pTransformCom->Go_StraightRight(fTimeDelta);
	//목표 위치까지 이동했음
	if (m_fChangeTargetDist >= m_fChangeTargetMaxDist)
	{
		m_bChangeIn = false;
		m_bControl = true;
		CPlayerTagMgr::Get_Instance()->Set_Changing(false);
		m_fChangeTargetDist = 0.f;

		// Stat옵저버
		if (nullptr == m_pStatSubjectCom)
			MSG_VOID("SubjectCom is Null");

		Call_StatObserverChange();
		Call_SkillObserverChange();
	}

	return;
}

void CPlayer::Tick_Frame(_float fTimeDelta, CGameInstance* pGameInstance)
{
	m_tFrame.fTime += fTimeDelta;
	if (m_tFrame.fDelay <= m_tFrame.fTime)
	{
		m_tFrame.fTime = 0.f;
		m_tFrame.iStart++;

		Tick_Framing(pGameInstance);

		if (m_tFrame.iStart >= m_tFrame.iEnd)
		{
			m_tFrame.iStart = 0;

			Tick_FrameEnd(pGameInstance);

		}
	}
}

void CPlayer::Tick_Framing(CGameInstance* pGameInstance)
{
	Tick_FramingJump();
	Tick_FramingRunWet();

	Tick_FramingWarriorSkill1(pGameInstance);
	Tick_FramingMageSkill1(pGameInstance);
	Tick_FramingMageSkill2(pGameInstance);
	Tick_FramingMageSkill3(pGameInstance);
	Tick_FramingThiefSkill1(pGameInstance);
	Tick_FramingThiefSkill2(pGameInstance);

}

void CPlayer::Tick_FramingJump()
{
	if (!m_bJump)
		return;

	if (m_bJumpUp)
		m_tFrame.iStart = 0;
}

void CPlayer::Tick_FramingRunWet()
{
	if (!m_bRunWet || m_eStateKey != STATE_RUN)
		return;

	if (m_tFrame.iStart == 1 || m_tFrame.iStart == 4)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		CRunWet::DESC tDesc;
		tDesc.matWorld = *m_pTransformCom->Get_WorldMatrixPtr();
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeRunWet, pTagLayerNonCollision, &tDesc)))
			MSG_VOID("CPlayer::Tick_FramingRunWet - pTagPrototypeRunWet");

		RELEASE_INSTANCE(CGameInstance);
		return;
	}

	//if (m_tFrame.iStart == 4)
	//{
	//	return;
	//}
}

void CPlayer::Tick_FramingWarriorSkill1(CGameInstance* pGameInstance)
{
	if (!m_bWarriorSkill1 || m_tFrame.iStart != 1)
		return;

	CAttack::DESC tDesc;
	tDesc.bDestroy = true;
	tDesc.iDmg = 10;
	//2D기준
	if (pGameInstance->Get_CurrentCameraID() == CAMERA_2D)
	{
		tDesc.vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
		tDesc.vPos.x += 1;
		tDesc.vPos.y += 1;
		tDesc.vLook = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		tDesc.vScale = _vec3(1.f, 1.f, 1.f);
	}
	//3D기준
	else
	{
		tDesc.vPos = m_pTransformCom->Get_State(CTransform::STATE_POS) + m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		tDesc.vPos.y += 0.5f;
		tDesc.vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		tDesc.vScale = _vec3(3.f, 1.f, 3.f);
	}

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeAttack, pTagLayerPlayerAttack, &tDesc)))
		MSG_VOID("CPlayer::Tick_FramingWarriorSkill1 - pGameInstance->Add_GameObjectToLayer pTagPrototypeAttack");
}

void CPlayer::Tick_FramingMageSkill1(CGameInstance * pGameInstance)
{
	if (!m_bMageSkill1 || m_tFrame.iStart != 2)
		return;

	CMagicMissile::DESC tDesc;
	tDesc.iTextureNum = 1;
	tDesc.vPos = m_pTransformCom->Get_State(CTransform::STATE_POS);
	tDesc.vPos += m_pTransformCom->Get_State(CTransform::STATE_LOOK) * 0.1f + m_pTransformCom->Get_State(CTransform::STATE_UP)* 0.6f;
	tDesc.tTransformDesc.fSpeed = 30.f;

	_vec3 vTargetPos;
	//피킹 성공
	if (CCrosshairMgr::Get_Instance()->Picking_Crosshair(pTagComTransform, pTagComVIBuffer,m_pTransformCom->Get_State(CTransform::STATE_POS) ,&vTargetPos))
	{
		tDesc.vTargetPos = vTargetPos;
	}
	//피킹 실패시
	else
	{
		_vec3 vTargetDir = CCrosshairMgr::Get_Instance()->Get_Ray();
		tDesc.vTargetPos = tDesc.vPos + *D3DXVec3Normalize(&vTargetDir, &vTargetDir) * 1.f;
	}

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeMagicMissile, pTagLayerPlayerAttack, &tDesc)))
		MSG_VOID("CPlayer::Tick_FramingWarriorSkill1 - pGameInstance->Add_GameObjectToLayer pTagPrototypeMagicMissile");
}

void CPlayer::Tick_FramingThiefSkill1(CGameInstance * pGaemInstance)
{
	if (!m_bThiefSkill1 || m_tFrame.iStart != 7)
		return;

	CDagger::DESC tDaggerDesc;

	tDaggerDesc.tSuperDesc.tSuperDesc.eTextureLevel = LEVEL_STATIC;

	OBJ_INFO tObjInfo;
	tObjInfo.dwColliderColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tObjInfo.vColliderCenter = _vec3(0.f, 0.f, 0.f);
	tObjInfo.vColliderSize = _vec3(1.f, 1.f, 1.f);
	lstrcpy(tObjInfo.szTextureName, pTagPrototypeTextureDagger);
	tObjInfo.matWorld = *m_pTransformCom->Get_WorldMatrixPtr();
	tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, tObjInfo.Get_State(OBJ_INFO::STATE_POS) + tObjInfo.Get_State(OBJ_INFO::STATE_UP) * 0.6f);
	tDaggerDesc.tSuperDesc.tSuperDesc.tObjInfo = tObjInfo;
	tDaggerDesc.tSuperDesc.tSuperDesc.tTransformDesc.fDegree = 1.f;
	tDaggerDesc.tSuperDesc.tSuperDesc.tTransformDesc.fSpeed = 15.f;

	_vec3 vTargetPos;
	//피킹 성공
	if (CCrosshairMgr::Get_Instance()->Picking_Crosshair(pTagComTransform, pTagComVIBuffer, m_pTransformCom->Get_State(CTransform::STATE_POS), &vTargetPos))
	{
		tDaggerDesc.vTargetPos = vTargetPos;
	}
	//피킹 실패시
	else
	{
		_vec3 vTargetDir = CCrosshairMgr::Get_Instance()->Get_Ray();
		tDaggerDesc.vTargetPos = tObjInfo.Get_State(OBJ_INFO::STATE_POS) + *D3DXVec3Normalize(&vTargetDir, &vTargetDir) * 1.f;
	}

	if (FAILED(pGaemInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeDagger, pTagLayerPlayerAttack, &tDaggerDesc)))
		MSG_VOID("CPlayer::Tick_FramingWarriorSkill1 - pGameInstance->Add_GameObjectToLayer pTagPrototypeDagger");
}
void CPlayer::Tick_FramingMageSkill2(CGameInstance * pGameInstance)
{
	if (!m_bMageSkill2 || m_tFrame.iStart == 2)
		return;



	if (CCrosshairMgr::Get_Instance()->Picking_Crosshair_Telekinesis(pTagComTransform, pTagComVIBuffer, m_pTransformCom->Get_State(CTransform::STATE_POS)))
		CSoundMgr::Get_Instance()->PlaySound(L"TELEKINESIS.wav", CSoundMgr::MAGE_SKILL);
}
void CPlayer::Tick_FramingMageSkill3(CGameInstance * pGameInstance)
{
	if (!m_bMageSkill3 || m_tFrame.iStart != 2)
		return;

	CLockOn::DESC tDesc;
	tDesc.pObject = this;
	tDesc.mat = *m_pTransformCom->Get_WorldMatrixPtr();
	tDesc.fTransformRotation = 10.f;
	tDesc.fTransformSpeed = 5.5f;
	CSoundMgr::Get_Instance()->PlaySound(L"METEOR.wav",CSoundMgr::MAGE_SKILL);
	pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeLockOn, pTagLayerLockOn, &tDesc);
}

void CPlayer::Tick_FramingThiefSkill2(CGameInstance * pGaemInstance)
{
	if (!m_bThiefSkill2 || m_tFrame.iStart != 7)
		return;

	CPoisonCube::DESC tPoisonCubeDesc;

	tPoisonCubeDesc.tTransformDesc.fDegree = 1.f;
	lstrcpy(tPoisonCubeDesc.tObjInfo.szTextureName, pTagPrototypeTexturePoisonCube);
	tPoisonCubeDesc.tObjInfo.iTextureIndex = 0;
	D3DXMatrixIdentity(&tPoisonCubeDesc.tObjInfo.matWorld);
	tPoisonCubeDesc.tObjInfo.SetUp_Scale(0.5f, 0.5f, 0.5f);
	tPoisonCubeDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, m_pTransformCom->Get_State(CTransform::STATE_POS) + _vec3(0.f, 1.f, 0.f));

	_vec3 vTargetPos;
	//피킹 성공
	if (CCrosshairMgr::Get_Instance()->Picking_Crosshair(pTagComTransform, pTagComVIBuffer, m_pTransformCom->Get_State(CTransform::STATE_POS), &vTargetPos))
	{
		tPoisonCubeDesc.vTargetPos = vTargetPos;
	}
	//피킹 실패시
	else
	{
		tPoisonCubeDesc.bRayFail = true;
		_vec3 vTargetDir = CCrosshairMgr::Get_Instance()->Get_Ray();
		tPoisonCubeDesc.vTargetPos = tPoisonCubeDesc.tObjInfo.Get_State(OBJ_INFO::STATE_POS) + *D3DXVec3Normalize(&vTargetDir, &vTargetDir) * 5.f;
	}

	if (FAILED(pGaemInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypePoisonCube, pTagLayerNonCollision, &tPoisonCubeDesc)))
		MSG_VOID("CPlayer::Tick_FramingWarriorSkill1 - pGameInstance->Add_GameObjectToLayer pTagPrototypeDagger");

}

void CPlayer::Tick_FrameEnd(CGameInstance* pGameInstance)
{
	Tick_FrameEndJump();
	Tick_FrameEndWarriorSkill1();
	Tick_FrameEndMageSkill1();
	Tick_FrameEndThiefSkill1();
	Tick_FrameEndMageSkill2();
	Tick_FrameEndMageSkill3();
	Tick_FrameEndThiefSkill2();
}

void CPlayer::Tick_FrameEndJump()
{
	if (!m_bJump)
		return;

	m_tFrame.iStart = 1;
}

void CPlayer::Tick_FrameEndWarriorSkill1()
{
	if (!m_bWarriorSkill1)
		return;

	m_bWarriorSkill1 = false;
	Set_Idle();
}

void CPlayer::Tick_FrameEndMageSkill1()
{
	if (!m_bMageSkill1)
		return;

	m_bMageSkill1 = false;
	Set_Idle();
}

void CPlayer::Tick_FrameEndThiefSkill1()
{
	if (!m_bThiefSkill1)
		return;

	m_bThiefSkill1 = false;
	Set_Idle();
}
void CPlayer::Tick_FrameEndMageSkill2()
{
	if (!m_bMageSkill2)
		return;

	m_bMageSkill2 = false;
	Set_Idle();
}
void CPlayer::Tick_FrameEndMageSkill3()
{
	if (!m_bMageSkill3)
		return;

	m_tFrame.iStart = 3;
}

void CPlayer::Tick_FrameEndThiefSkill2()
{
	if (!m_bThiefSkill2)
		return;

	m_bThiefSkill2 = false;
	Set_Idle();
}

void CPlayer::Check_Frame(_float fTimeDelta)
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_tPlayerStat.eType)
		{
			//WARRIOR
		case TYPE::TYPE_WARRIOR:
		{

			_float fDelay = 0.1f;
			if (m_bWarriorSkill2)
				fDelay = 0.05f;

			switch (m_eCurState)
			{
			case Client::CPlayer::FSM_IDLE_N:
				m_tFrame.Set_Value(1);
				break;
			case Client::CPlayer::FSM_IDLE_E:
				m_tFrame.Set_Value(1);
				break;
			case Client::CPlayer::FSM_IDLE_S:
				m_tFrame.Set_Value(1);
				break;
			case Client::CPlayer::FSM_RUN_N:
				m_tFrame.Set_Value(6, 0, fDelay);
				break;
			case Client::CPlayer::FSM_RUN_NE:
				m_tFrame.Set_Value(6, 0, fDelay);
				break;
			case Client::CPlayer::FSM_RUN_E:
				m_tFrame.Set_Value(6, 0, fDelay);
				break;
			case Client::CPlayer::FSM_RUN_SE:
				m_tFrame.Set_Value(6, 0, fDelay);
				break;
			case Client::CPlayer::FSM_RUN_S:
				m_tFrame.Set_Value(6, 0, fDelay);
				break;
			case Client::CPlayer::FSM_SKILL_N:
				m_tFrame.Set_Value(2);
				break;
			case Client::CPlayer::FSM_SKILL_NE:
				m_tFrame.Set_Value(2);
				break;
			case Client::CPlayer::FSM_SKILL_E:
				m_tFrame.Set_Value(2);
				break;
			case Client::CPlayer::FSM_SKILL_SE:
				m_tFrame.Set_Value(2);
				break;
			case Client::CPlayer::FSM_SKILL_S:
				m_tFrame.Set_Value(2);
				break;
			}

			break;


		}
		//MAGE
		case TYPE::TYPE_MAGE:
			switch (m_eCurState)
			{

			case Client::CPlayer::FSM_IDLE_N:
				m_tFrame.Set_Value(4);
				break;
			case Client::CPlayer::FSM_IDLE_E:
				m_tFrame.Set_Value(4);
				break;
			case Client::CPlayer::FSM_IDLE_S:
				m_tFrame.Set_Value(4);
				break;
			case Client::CPlayer::FSM_RUN_N:
				m_tFrame.Set_Value(7);
				break;
			case Client::CPlayer::FSM_RUN_NE:
				m_tFrame.Set_Value(7);
				break;
			case Client::CPlayer::FSM_RUN_E:
				m_tFrame.Set_Value(7);
				break;
			case Client::CPlayer::FSM_RUN_SE:
				m_tFrame.Set_Value(7);
				break;
			case Client::CPlayer::FSM_RUN_S:
				m_tFrame.Set_Value(7);
				break;
			case Client::CPlayer::FSM_SKILL_N:
				m_tFrame.Set_Value(6);
				break;
			case Client::CPlayer::FSM_SKILL_NE:
				m_tFrame.Set_Value(6);
				break;
			case Client::CPlayer::FSM_SKILL_E:
				m_tFrame.Set_Value(6);
				break;
			case Client::CPlayer::FSM_SKILL_SE:
				m_tFrame.Set_Value(6);
				break;
			case Client::CPlayer::FSM_SKILL_S:
				m_tFrame.Set_Value(6);
				break;
			}

			break;

			//THIEF
		case TYPE::TYPE_THIEF:
		{
			_uint iJumpFrameStart = 0;
			if (!m_bJumpUp)
				iJumpFrameStart = 1;

			switch (m_eCurState)
			{
			case Client::CPlayer::FSM_IDLE_N:
				m_tFrame.Set_Value(1);
				break;
			case Client::CPlayer::FSM_IDLE_E:
				m_tFrame.Set_Value(1);
				break;
			case Client::CPlayer::FSM_IDLE_S:
				m_tFrame.Set_Value(1);
				break;
			case Client::CPlayer::FSM_RUN_N:
				m_tFrame.Set_Value(6);
				break;
			case Client::CPlayer::FSM_RUN_NE:
				m_tFrame.Set_Value(6);
				break;
			case Client::CPlayer::FSM_RUN_E:
				m_tFrame.Set_Value(6);
				break;
			case Client::CPlayer::FSM_RUN_SE:
				m_tFrame.Set_Value(6);
				break;
			case Client::CPlayer::FSM_RUN_S:
				m_tFrame.Set_Value(6);
				break;
			case Client::CPlayer::FSM_SKILL_N:
				m_tFrame.Set_Value(2, iJumpFrameStart);
				break;
			case Client::CPlayer::FSM_SKILL_NE:
				m_tFrame.Set_Value(2, iJumpFrameStart);
				break;
			case Client::CPlayer::FSM_SKILL_E:
				m_tFrame.Set_Value(2, iJumpFrameStart);
				break;
			case Client::CPlayer::FSM_SKILL_SE:
				m_tFrame.Set_Value(2, iJumpFrameStart);
				break;
			case Client::CPlayer::FSM_SKILL_S:
				m_tFrame.Set_Value(2, iJumpFrameStart);
				break;
			case Client::CPlayer::FSM_THROW:
				m_tFrame.Set_Value(9, 0, 0.025f);
				break;
			}

			break;
		}
		}
		m_ePreState = m_eCurState;
	}
}

void CPlayer::Tick_TagCoolTime(_float fTimeDelta)
{
	//태그 쿨타임 갱신
	m_fTagCoolTime += fTimeDelta;
	if (m_fTagCoolTimeMax <= m_fTagCoolTime)
		m_fTagCoolTime = m_fTagCoolTimeMax;
}

void CPlayer::Tick_ChangeOut(_float fTimeDelta)
{
	if (!m_bChangeOut)
		return;

	m_fChangeTargetDist += m_pTransformCom->Go_BackwardRight(fTimeDelta);
	//목표 위치까지 이동했음
	if (m_fChangeTargetDist >= m_fChangeTargetMaxDist)
	{
		m_bChangeOut = false;
		m_bControl = false;
		m_fChangeTargetDist = 0.f;

		// Stat옵저버
		if (nullptr == m_pStatSubjectCom)
			MSG_VOID("SubjectCom is Null");

		Call_StatObserverChange();
		Call_SkillObserverChange();
	}

	return;
}

void CPlayer::Tick_ChangeIn(_float fTimeDelta)
{
	if (!m_bChangeIn)
		return;

	m_fChangeTargetDist += m_pTransformCom->Go_StraightRight(fTimeDelta);
	//목표 위치까지 이동했음
	if (m_fChangeTargetDist >= m_fChangeTargetMaxDist)
	{
		m_bChangeIn = false;
		m_bControl = true;
		CPlayerTagMgr::Get_Instance()->Set_Changing(false);
		m_fChangeTargetDist = 0.f;

		// Stat옵저버
		if (nullptr == m_pStatSubjectCom)
			MSG_VOID("SubjectCom is Null");

		Call_StatObserverChange();
		Call_SkillObserverChange();
	}

	return;
}

void CPlayer::Collision_Terrain(_float fTimeDelta, class CGameInstance* pInstance)
{
	CTerrain* pTerrain = (CTerrain*)pInstance->Get_GameObjPtr(pInstance->Get_CurrentLevel(), L"LayerTerrain", 0);

	if (pTerrain == nullptr)
	{
		m_bCollisionTerrain = false;
		return;
	}

	_vec3 vTerrainPos = pTerrain->Compute_Height(m_pTransformCom);
	_vec3 vPlayerPos = m_pTransformCom->Get_State(CTransform::STATE_POS);

	//플레이어가 지형 가까이 있거나 지형보다 밑에 있으면 땡김
	if (vPlayerPos.y - vTerrainPos.y < 1.f)
	{
		m_pTransformCom->SetUp_State(CTransform::STATE_POS, vTerrainPos);
		m_bCollisionTerrain = true;
		m_bCollisionTileCube = false;
		m_bFall = false;
		m_fFallPosY = Get_WorldPos().y;
		m_fFallTime = 0.f;
	}
	else
		m_bCollisionTerrain = false;


}

void CPlayer::Jump(_float fTimeDelta, class CGameInstance* pGameInstance)
{
	if (!m_bJump)
		return;

	m_fJumpTime += fTimeDelta;
	

	_float fJumpHighest; 

	//2D기준
	if (pGameInstance->Get_CurrentCameraID() == CAMERA_2D)
		fJumpHighest = m_pTransformCom->Jump(m_fJumpPosY, m_fJumpPower, m_fJumpTime);
	//3D기준
	else
		fJumpHighest = m_pTransformCom->Jump(m_fJumpPosY, m_f3DJumpPower, m_fJumpTime);

	if (pGameInstance->Get_CurrentCameraID() == CAMERA_2D)
	{
		if (m_eDirKey == DIR_E)
			m_pTransformCom->Add_Position(_vec3(m_fJumpSpeed, 0.f, 0.f));
		else if (m_eDirKey == DIR_W)
			m_pTransformCom->Add_Position(_vec3(-m_fJumpSpeed, 0.f, 0.f));
	}

	//아직 점프로 올라가는 중
	if (m_fJumpHighest <= fJumpHighest)
	{
		m_fJumpHighest = fJumpHighest;
	}
	//점프 내려가는 중
	else
	{
		m_bJumpUp = false;
		//내려가는 중에 땅 만남
		if (m_bCollisionTerrain || m_bCollisionTileCube)
		{
			m_bJump = false;
			m_bStateLock = false;
		}
	}
}

void CPlayer::Run(_float fTimeDelta, class CGameInstance* pGameInstance)
{
	if (m_eStateKey != STATE_RUN && !m_bJump)
		return;

	if (m_bChangeIn || m_bChangeOut)
		return;

	switch (m_eDirKey)
	{
	case Client::CPlayer::DIR_N:
		m_pTransformCom->Go_Straight(fTimeDelta);
		break;
	case Client::CPlayer::DIR_NE:
		m_pTransformCom->Go_StraightRight(fTimeDelta);
		break;
	case Client::CPlayer::DIR_E:
		m_pTransformCom->Go_Right(fTimeDelta);
		break;
	case Client::CPlayer::DIR_SE:
		m_pTransformCom->Go_BackwardRight(fTimeDelta);
		break;
	case Client::CPlayer::DIR_S:
		m_pTransformCom->Go_Backward(fTimeDelta);
		break;
	case Client::CPlayer::DIR_SW:
		m_pTransformCom->Go_BackwardLeft(fTimeDelta);
		break;
	case Client::CPlayer::DIR_W:
		m_pTransformCom->Go_Left(fTimeDelta);
		break;
	case Client::CPlayer::DIR_NW:
		m_pTransformCom->Go_StraightLeft(fTimeDelta);
		break;
	}
}

void CPlayer::Fall(_float fTimeDelta, class CGameInstance* pGameInstance)
{
	if (m_bJump || m_bCollisionTerrain || m_bCollisionTileCube || m_bChangeIn)
	{
		m_fFallPosY = Get_WorldPos().y;
		m_fFallTime = 0.f;
		m_bFall = false;
		m_bCollisionTileCube = false;
		return;
	}

	

	if (!m_bCollisionTerrain && !m_bCollisionTileCube)
		m_bFall = true;

	if (m_bFall)
	{
		m_fFallTime += fTimeDelta;
		m_pTransformCom->Fall(m_fFallPosY, m_fFallTime);
	}
}

void CPlayer::Tick_SkillCoolTime(_float fTimeDelta)
{
	m_tSkillCoolTime.fCooltime1 += fTimeDelta;
	m_tSkillCoolTime.fCooltime2 += fTimeDelta;
	m_tSkillCoolTime.fCooltime3 += fTimeDelta;
}

void CPlayer::SetUp_WarriorSkill2()
{
	m_bStateLock = true;
	m_bWarriorSkill2 = true;
	m_fWarriorSkill2DashDist = 0.f;
	m_fWarriorSkill2DashOriginalSpeed = m_pTransformCom->Get_Speed();
	m_pTransformCom->Set_Speed(m_fWarriorSkill2DashSpeed);

}

void CPlayer::Tick_WarriorSkill2(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (!m_bWarriorSkill2)
		return;

	//특정 거리까지 돌진
	m_fWarriorSkill2DashDist += m_pTransformCom->Go_Straight(fTimeDelta);
	if (m_fWarriorSkill2DashDist >= m_fWarriorSkill2DashDistMax)
	{
		m_fWarriorSkill2DashDist = 0.f;
		m_bWarriorSkill2 = false;
		Set_Idle();
		m_pTransformCom->Set_Speed(m_fWarriorSkill2DashOriginalSpeed);
	}
}

void CPlayer::SetUp_WarriorSkill3()
{
	m_bStateLock = true;
	m_bWarriorSkill3 = true;
	m_bInvincible = true;
	m_tTimeWarriorSkill3.fTime = 0.f;
	m_tSkillCoolTime.fCooltime3 = 0.f;
	Call_SkillObserverUse(2);

	CPlayerShield::DESC tDesc;
	D3DXMatrixIdentity(&tDesc.tObjInfo.matWorld);
	_vec3 vScale = m_pTransformCom->Get_Scale();
	tDesc.tObjInfo.SetUp_Scale(vScale.x, vScale.y, vScale.z);
	tDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, m_pTransformCom->Get_State(CTransform::STATE_POS) + _vec3(0.f, 0.5f, 0.f));
	lstrcpy(tDesc.tObjInfo.szTextureName, pTagPrototypeTexturePlayerShield);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypePlayerShield, pTagLayerNonCollision, &tDesc, (void**)&m_pShield)))
		MSG_VOID("CPlayer::SetUp_WarriorSkill3 - pTagPrototypePlayerShield");
	if (m_pShield == nullptr)
		MSG_VOID("CPlayer::SetUp_WarriorSkill3 - m_pShield == nullptr");
	Safe_AddRef(m_pShield);
	RELEASE_INSTANCE(CGameInstance);
}

void CPlayer::Tick_WarriorSkill3(_float fTimeDelta, CGameInstance * pGameInstance)
{
	if (!m_bWarriorSkill3)
		return;

	m_tTimeWarriorSkill3.Tick_Time(fTimeDelta);
	if (m_tTimeWarriorSkill3.Check_Time())
		End_WarriorSkill3();
}

void CPlayer::End_WarriorSkill3()
{
	m_bInvincible = false;
	m_pShield->Set_Dead();
	Safe_Release(m_pShield);
	m_pShield = nullptr;
	m_bWarriorSkill3 = false;
	Set_Idle();
}

void CPlayer::SetUp_ThiefSkill1()
{
	m_bStateLock = true;
	m_bThiefSkill1 = true;
	PLAY_SOUND(L"PlayerDagger.wav", CSoundMgr::THIEF_SKILL);
}

void CPlayer::SetUp_ThiefSkill2()
{
	m_bStateLock = true;
	m_bThiefSkill2 = true;
}

void CPlayer::Call_SkillObserverUse(_uint iSkillNum)
{
	const list<CObserver*>& pObserverList = m_pSkillSubjectCom->Get_ObserverList();
	auto& iter = pObserverList.begin();
	if (iter == pObserverList.end())
		return;

	advance(iter, iSkillNum);

	(*iter)->Notify();
}

void CPlayer::Call_SkillObserverChange()
{
	m_tPlayerSkill.bControl = m_bControl;

	m_pSkillSubjectCom->Notify(&m_tPlayerSkill);
}

void CPlayer::On_theCube()
{
	m_bCollisionTileCube = true;
	if (m_bJump)
	{
		m_bJump = false;
		m_bJumpUp = false;
		m_bStateLock = false;
	}
	m_bFall = false;
}

void CPlayer::Under_theCube()
{
	m_bJump = false;
	m_bFall = true;
	m_bStateLock = false;
}

void CPlayer::Lock_Off()
{
	if (!m_bMageSkill3)
		return;

	m_bMageSkill3 = false;
	Set_Idle();
}

void CPlayer::Collision(CGameObject * pColObj)
{
	//물 밟고있을때만 파티클 생성하기 위해 항상 초기화
	m_bRunWet = false;

	CPortal* pPortal = dynamic_cast<CPortal*>(pColObj);

	if (pPortal != nullptr)
		return;

	CAttack* pAttack = dynamic_cast<CAttack*>(pColObj);

	if (pAttack != nullptr)
	{
		//싱글 공격인데 이미 공격한 경우 아무일도 안일어나야함
		if (pAttack->Get_SingleAttack() && pAttack->Get_Attack())
		{

		}
		else if (pAttack->Get_Dmg() > 0)
		{
			pAttack->Set_Attack(true);
			Sub_Hp(pAttack->Get_Dmg());
		}

		pAttack->Set_Dead();
	}

	//젖은 타일 위로가면 파티클 생성
	CBossSkeletonStageCube* pBossSkeletonStageCube = dynamic_cast<CBossSkeletonStageCube*>(pColObj);
	DO;
	if (pBossSkeletonStageCube == nullptr)
		break;

	//젖은 타일에서만 생성함
	if (pBossSkeletonStageCube->Get_FSM() != CBossSkeletonStageCube::FSM_WET)
		break;

	//파티클 생성을 위한 불변수 초기화
	m_bRunWet = true;
	WHILE;
}

_mat * CPlayer::Get_WorldMatrix()
{
	return (_mat*)(m_pTransformCom->Get_WorldMatrixPtr());
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	CPlayer* pInstance = new CPlayer(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CPlayer Create Failed");
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pStatSubjectCom);
	Safe_Release(m_pSkillSubjectCom);
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pShield);

	for (auto& pTexture : m_pTextureComArr)
		Safe_Release(pTexture);
}

CGameObject * CPlayer::Clone(void * pArg)
{
	CGameObject* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CPlayer Clone Failed");
	}

	return pInstance;
}

unsigned long CPlayer::AddRef()
{
	return CBase::AddRef();
}

Engine::_vec3 CPlayer::Get_MinPos()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	_uint iCurrentCameraId = pInstance->Get_CurrentCameraID();
	RELEASE_INSTANCE(CGameInstance);
	_vec3 vMinPos;
	if (iCurrentCameraId == CAMERA_BACK)
		vMinPos = m_pBoxCollider->Get_MinPos();
	else if (iCurrentCameraId == CAMERA_2D)
		vMinPos = m_pBoxCollider->Get_MinPosProj();


	return vMinPos;
}

Engine::_vec3 CPlayer::Get_MaxPos()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	_uint iCurrentCameraId = pInstance->Get_CurrentCameraID();
	RELEASE_INSTANCE(CGameInstance);
	_vec3 vMaxPos;
	
	if (iCurrentCameraId == CAMERA_BACK)
		vMaxPos = m_pBoxCollider->Get_MaxPos();
	else if (iCurrentCameraId == CAMERA_2D)
		vMaxPos = m_pBoxCollider->Get_MaxPosProj();
	
	return vMaxPos;
}

_vec3 CPlayer::Get_WorldPos()
{
	return m_pBoxCollider->Get_ColliderWolrdPos();
}

Engine::_vec3 CPlayer::Get_Length()
{
	return m_pTransformCom->Get_Scale();
}

HRESULT CPlayer::Tick_Collider(_float fTimeDelta)
{
	m_pBoxCollider->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxCollider->Tick(fTimeDelta);

	return S_OK;
}
