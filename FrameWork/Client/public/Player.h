#pragma once
#ifndef __CLIENT_PLAYER_H__
#define __CLIENT_PLAYER_H__

#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
class CSubject;
class CBoxCollider;
END

BEGIN(Client)
class CPlayer final:
	public CGameObject
{
public:
	enum TYPE { TYPE_WARRIOR, TYPE_MAGE, TYPE_THIEF, TYPE_END};
	enum PLAYER_FSM { FSM_IDLE_N, FSM_IDLE_E, FSM_IDLE_S, 
		FSM_RUN_N, FSM_RUN_NE, FSM_RUN_E, FSM_RUN_SE, FSM_RUN_S, 
		FSM_SKILL_N, FSM_SKILL_NE, FSM_SKILL_E, FSM_SKILL_SE, FSM_SKILL_S,
		FSM_THROW,
		FSM_END };

	enum STATE_KEY { STATE_IDLE, STATE_RUN, STATE_SKILL1, STATE_SKILL2, STATE_SKILL3, STATE_JUMP, STATE_END};
	enum DIR_KEY { DIR_N, DIR_NE, DIR_E, DIR_SE, DIR_S, DIR_SW, DIR_W, DIR_NW, DIR_END };

public:
	typedef struct tagPlayerStat {
		tagPlayerStat() { ZeroMemory(this, sizeof(*this)); }

		TYPE	eType;
		_bool	bControl;
		_int	iHp;
		_int	iMaxHp;
		_int	iMp;
		_int	iMaxMp;
		_int	iDmg;
		
		OBJ_INFO tObj;

	}PLAYER_STAT;

	typedef struct tagPlayerSkiil {
		tagPlayerSkiil() { ZeroMemory(this, sizeof(*this)); }

		TYPE	eType;
		_bool	bControl;
		_int	iSkillNum;
		_float	fCooltime1;
		_float	fCooltime2;
		_float	fCooltime3;
	}PLAYER_SKILL;

	typedef struct tagPlayerSkillCoolTime {
		tagPlayerSkillCoolTime() { ZeroMemory(this, sizeof(tagPlayerSkillCoolTime)); }
		_float fCooltime1;
		_float fCooltime2;
		_float fCooltime3;
	}SKILL_COOLTIME;

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

#pragma region PublicFunction


public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	// 다른 객체에서 충돌 받을 경우
	void	Add_Bound(_vec3 vBoundPos);
public: //SET
	void Set_Control(_bool bControl);
	void Sub_Hp(_uint iDmg);
	//마나 소모량 안되면 false 리턴
	_bool Sub_Mp(_uint iMp);

	//스테이트
	void Set_State(STATE_KEY eStateKey, DIR_KEY eDirKey);
	void Set_CurState(PLAYER_FSM eFSM);
	void Set_Idle();
	void Set_JumpEnd();

public: //GET
	const _vec3& Get_ColliderCenter() const;

public:
	// 구독후 한번 호출이 필요한 옵저버들의 호출
	HRESULT Ready_Observers();
#pragma endregion

#pragma region PublicVariable
public:
	static _float	m_fTagCoolTime;
	const _float	m_fTagCoolTimeMax = 2.f;

	//디버그용변수
	_float m_fTest;
#pragma endregion

#pragma region PrivateFunction
	private:
	HRESULT SetUp_Component();
	HRESULT Ready_TextureWarrior();
	HRESULT Ready_TextureMage();
	HRESULT Ready_TextureThief();

	HRESULT SetUp_RenderState();
	HRESULT Release_RenderState();

	// 스킬 타입 및 쿨타임 설정
	void Ready_SkillSet();



private:
	//Tick
	//키보드 입력 체크
#pragma region CheckKeyboard
	HRESULT Check_KeyBoard(_float fTimeDelta, class CGameInstance* pInstance);
	HRESULT Check_MoveKey(_float fTimeDelta, class CGameInstance* pInstance);
	HRESULT Check_TagKey(_float fTimeDelta, class CGameInstance* pInstance);
	HRESULT Check_SkillKey(_float fTimeDelta, class CGameInstance* pGameInstance);
	HRESULT Check_Camera(_float fTimeDelta, class CGameInstance* pGameInstance);
#pragma endregion

	//마우스 입력 체크
	HRESULT Check_Mouse(_float fTimeDelta, class CGameInstance* pInstance);

	// 2D 시점 키보드 입력 체크
#pragma region 2DKeyboardCheck
	HRESULT	Check_KeyBoard2D(_float fTimeDelta, class CGameInstance* pInstance);
	HRESULT Check_MoveKey2D(_float fTimeDelta, class CGameInstance* pInstance);
	HRESULT Check_TagKey2D(_float fTimeDelta, class CGameInstance* pGameInstance);
	HRESULT Check_SkillKey2D(_float fTimeDelta, class CGameInstance* pGameInstance);
	void	Set_State2D(STATE_KEY eStateKey, DIR_KEY eDirKey);
	void	Tick_ChangeOut2D(_float fTimeDelta);
	void	Tick_ChangeIn2D(_float fTimeDelta);
#pragma endregion

//프레임 증가
#pragma region Frame
	void Tick_Frame(_float fTimeDelta, class CGameInstance* pGameInstance);
	void Tick_Framing(class CGameInstance* pGameInstance);
	void Tick_FramingJump();
	void Tick_FramingRunWet();
	void Tick_FramingWarriorSkill1(class CGameInstance* pGameInstance);
	void Tick_FramingMageSkill1(class CGameInstance* pGaemInstance);
	void Tick_FramingThiefSkill1(class CGameInstance* pGaemInstance);
	void Tick_FramingMageSkill2(class CGameInstance* pGameInstance);
	void Tick_FramingMageSkill3(class CGameInstance* pGameInstance);
	void Tick_FramingThiefSkill2(class CGameInstance* pGaemInstance);

	void Tick_FrameEnd(class CGameInstance* pGameInstance);
	void Tick_FrameEndJump();
	void Tick_FrameEndWarriorSkill1();
	void Tick_FrameEndMageSkill1();
	void Tick_FrameEndThiefSkill1();
	void Tick_FrameEndMageSkill2();
	void Tick_FrameEndMageSkill3();
	void Tick_FrameEndThiefSkill2();
	void Check_Frame(_float fTimeDelta);
#pragma endregion
	
//태그 시스템
#pragma region Tag
	void Tick_TagCoolTime(_float fTimeDelta);
	//교체 아웃
	void Tick_ChangeOut(_float fTimeDelta);
	//교체 인
	void Tick_ChangeIn(_float fTimeDelta);
#pragma endregion

//기본 동작
#pragma region BasicMovement
	void Collision_Terrain(_float fTimeDelta, class CGameInstance* pInstance);
	void Jump(_float fTimeDelta, class CGameInstance* pGameInstance);
	void Run(_float fTimeDelta, class CGameInstance* pGameInstance);
	void Fall(_float fTimeDelta, class CGameInstance* pGameInstance);
#pragma endregion

	//플레이어 스킬
#pragma region PlayerSkill
	void Tick_SkillCoolTime(_float fTimeDelta);

	void SetUp_WarriorSkill2();
	void Tick_WarriorSkill2(_float fTimeDelta, class CGameInstance* pGameInstance);

	void SetUp_WarriorSkill3();
	void Tick_WarriorSkill3(_float fTimeDelta, class CGameInstance* pGameInstance);
	void End_WarriorSkill3();

	void SetUp_ThiefSkill1();

	void SetUp_ThiefSkill2();

#pragma endregion

	// 스텟 옵저버 호출
	void Call_StatObserverChange() { m_tPlayerStat.bControl = m_bControl; m_pStatSubjectCom->Notify(&m_tPlayerStat); }
	// 스킬 옵저버 호출
	void Call_SkillObserverUse(_uint iSkillNum);
	void Call_SkillObserverChange();
#pragma endregion

public: 
	// 타일 위에 있을 때 호출 할 함수
	void	On_theCube();
	void	Under_theCube();

	// 점프 업!
	_bool	Get_JumpUp() { return m_bJumpUp; }
	// 외부에서 StateLock 컨트롤 하는 변수
	void	Set_StateLock(_bool bStateLock) { m_bStateLock = bStateLock; }

	// Lock On 에서 조절 할 함수
	void	Lock_Off();
	//충돌
	virtual void Collision(CGameObject* pColObj) override;


	void	Set_Transform(const _vec3& vPos) { m_pTransformCom->SetUp_State(CTransform::STATE_POS, vPos); }

	//WorldMatrix Ptr로 받기
	_mat*	Get_WorldMatrix();

private:
#pragma region Component
	CRenderer*					m_pRendererCom = nullptr;
	CVIBufferRect*				m_pVIBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	// 스텟 서브젝트
	CSubject*					m_pStatSubjectCom = nullptr;
	// 스킬 서브젝트 
	CSubject*					m_pSkillSubjectCom = nullptr;

	CBoxCollider*				m_pBoxCollider = nullptr;
	CTexture*					m_pTextureComArr[FSM_END];
#pragma endregion

private:

//플레이어 상태
#pragma region PlayerState
	PLAYER_STAT			m_tPlayerStat;
	PLAYER_SKILL		m_tPlayerSkill;
	SKILL_COOLTIME		m_tSkillCoolTime;
	_uint				m_iSkillMPCostArr[3];
	FRAME				m_tFrame;
	_bool				m_bStateLock = false;			//플레이어 행동 중일때 다른 행동 입력 못하게 방지용
	_bool				m_bDirLock = false;				//플레이어 방향 전환 컨트롤 용
	PLAYER_FSM			m_eCurState = FSM_IDLE_N;
	PLAYER_FSM			m_ePreState = FSM_END;
	STATE_KEY			m_eStateKey = STATE_KEY::STATE_END;
	DIR_KEY				m_eDirKey = DIR_KEY::DIR_END;
#pragma endregion

//태그 관련 변수
#pragma region Tag
	_bool				m_bControl = false;				//현재 컨트롤 중
	_bool				m_bChangeOut = false;			//태그 시 나가는 친구
	_bool				m_bChangeIn = false;			//태그 시 들어오는 친구
	_float				m_fChangeTargetDist = 0.f;		//태그 시 이동거리
	const _float		m_fChangeTargetMaxDist = 3.5f;	//태그 시 이동거리
#pragma endregion

	//무적 관련 변수
	_bool				m_bInvincible = false;			//무적

//점프 관련 변수
#pragma region Jump
	_bool				m_bJump = false;				//점프 중
	_float				m_fJumpPower = 10.f;			//점프 파워
	_float				m_f3DJumpPower = 20.f;			//3D 상태일때의 점프 힘
	_float				m_fJumpPosY = 0.f;				//점프 시작 위치
	_float				m_fJumpTime = 0.f;				//점프 시작 후 시간이 얼마나 지났나
	_float				m_fJumpHighest = 0.f;			//점프 최고 높이 측정 (점프 올라갈때 내려갈때 모션 다르게 주기 위함)
	_float				m_fJumpSpeed = 0.f;				//2D 시점에서 점프 시 속도를 조절하는 용도
	_float				m_fJumpSpeedMax = 0.25f;		//2D 시점에서 점프 시 최고 속도 양
	_vec3				m_fReturnPos = { 0.f,0.f,0.f };	//2D 시점에서 낙하 시 돌아올 위치
	_bool				m_bJumpUp = false;				//점프 내려올때 모션 바꾸기용
#pragma endregion

//낙하 관련 변수
#pragma region Fall
														//낙하 관련 변수
	_bool				m_bFall = false;
	_float				m_fFallPosY = 0.f;
	_float				m_fFallTime = 0.f;
	//플레이어가 지형을 탔는가
	_bool				m_bCollisionTerrain = false;	//지형 충돌 체크
														//플레이어가 큐브를 탔는가
	_bool				m_bCollisionTileCube = false;	//타일 충돌 체크
#pragma endregion

	//전사 1스킬 공격해서 오브젝트 파괴 or 데미지
	_bool				m_bWarriorSkill1 = false;		//전사 첫 스킬 시작 변수

//전사 2스킬 돌진 공격
#pragma region WarriorSkill2
	//전사 2스킬 시작 변수
	_bool				m_bWarriorSkill2 = false;
	//돌진 거리
	const _float		m_fWarriorSkill2DashDistMax = 10.f;
	_float				m_fWarriorSkill2DashDist = 0.f;
	//돌진 속도
	const _float		m_fWarriorSkill2DashSpeed = 30.f;
	//원래의 속도 담아줄 변수
	_float				m_fWarriorSkill2DashOriginalSpeed = 0.f;
#pragma endregion

//전사 3스킬 방어
#pragma region WarriorSkill3
	//일정 시간 동안 방어막 펼침
	_bool				m_bWarriorSkill3 = false;
	TIME				m_tTimeWarriorSkill3;
	CGameObject*		m_pShield = nullptr;

#pragma endregion

	//법사 1스킬 빙결 오브젝트 발사
	_bool				m_bMageSkill1 = false;

	//법사 2스킬 염동력
	_bool				m_bMageSkill2 = false;
	_bool				m_bTelekinesis = false;
	//법사 3스킬 메떼오
	_bool				m_bMageSkill3 = false;

	//도적 스킬
#pragma region ThiefSkill
	//도적 1스킬 단검 투척
	_bool	m_bThiefSkill1 = false;

	//도적 2스킬 독 투척
	_bool	m_bThiefSkill2 = false;
#pragma endregion

	_bool				m_bRunWet = false;

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual void Free() override;
	virtual CGameObject * Clone(void *pArg = (void *)nullptr);

	//디버깅용
	virtual unsigned long AddRef();

	// CGameObject을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
	virtual HRESULT Tick_Collider(_float fTimeDelta) override;

};
END
#endif // !__CLIENT_PLAYER_H__
