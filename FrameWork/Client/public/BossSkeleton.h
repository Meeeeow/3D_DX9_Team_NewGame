#pragma once
#ifndef __CLIENT_BOSS_SKELETON_H__
#define __CLIENT_BOSS_SKELETON_H__

#include "Monster.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
class CBoxCollider;
END

BEGIN(Client)
class CBossSkeleton final:
	public CMonster
{
public:
	typedef struct tagBossSkeletonDesc {
		OBJ_INFO tObjInfo;
	} DESC;
public: 
	enum FSM { FSM_IDLE, FSM_ATTACK, FSM_DIE, FSM_SPAWN, FSM_END};

public:
	typedef struct tagMonsterStat {
		tagMonsterStat() { ZeroMemory(this, sizeof(*this)); }

		_int	iHp;
		_int	iMaxHp;
		_int	iDmg;

	}MONSTER_STAT;

private:
	explicit CBossSkeleton(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CBossSkeleton(const CBossSkeleton& rhs);
	virtual ~CBossSkeleton() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual _bool Picking(const _vec3& vMouseRay, const _vec3& vMousePivot, CTransform* pOut) override;
public: //SET
	void Set_State(FSM eState);
	void Set_Idle();
	virtual HRESULT Sub_Hp(_uint iDmg) override;
	virtual void Set_PoisonHitTransform(CPoisonCubeHit::DESC * pDesc) override;

public: //GET

private:
	//SetUp
	HRESULT SetUp_Component(void *pArg);

	//Render
	HRESULT Set_RenderState();
	HRESULT Release_RenderState();

	//Tick
	//State 관리하는 함수
	HRESULT AI(_float fTimeDelta, CGameInstance* pGameInstance);
	//State에 맞게 값들 초기화 해주는 함수
	void Check_Frame();
	void Die(_float fTimeDelta);

	//FRAME
	virtual void Tick_Frame(_float fTimeDelta) override;
	void Tick_Framing();

	void Tick_FrameEnd();
	void FrameEnd_Spawn();

	//Pattern
	void SetUp_Skill();

	HRESULT SetUp_Phase1RedSkill(CGameInstance* pGameInstance);
	HRESULT SetUp_Phase1BlueSkill(CGameInstance* pGameInstance);
	HRESULT SetUp_Phase1GreenSkill(CGameInstance* pGameInstance);
	HRESULT SetUp_Phase2RedSkill(CGameInstance* pGameInstance);
	HRESULT SetUp_Phase2BlueSkill(CGameInstance* pGameInstance);
	HRESULT SetUp_Phase2GreenSkill(CGameInstance* pGameInstance);

	HRESULT Phase1RedSkill(_float fTimeDelta, CGameInstance* pGameInstance);
	HRESULT Phase1BlueSkill(_float fTimeDelta, CGameInstance* pGameInstance);
	HRESULT Phase1GreenSkill(_float fTimeDelta, CGameInstance* pGameInstance);
	HRESULT Phase2RedSkill(_float fTimeDelta, CGameInstance* pGameInstance);
	HRESULT Phase2BlueSkill(_float fTimeDelta, CGameInstance* pGameInstance);
	HRESULT Phase2GreenSkill(_float fTimeDelta, CGameInstance* pGameInstance);

	HRESULT Summon_MagicCircle(_int iType, _vec3 vScale, CGameInstance* pGameInstance);

	virtual HRESULT Tick_Collider(_float fTimeDelta) override;

	virtual void Collision(CGameObject* pColObj) override;

private: //Component
	CTransform*		m_pTransform = nullptr;
	CVIBufferRect*	m_pVIBuffer = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CTexture*		m_pTextureArr[FSM_END];
	CBoxCollider*	m_pBoxCollider = nullptr;

private: //Variable
	FSM				m_eCurState = FSM_SPAWN;
	FSM				m_ePreState = FSM_END;
	_bool			m_bStateLock = false;

	//플레이어 위치 추적을 위한 변수
	CTransform*		m_pTransformPlayer = nullptr;

	//스탯 변수
	MONSTER_STAT	m_tMonsterStat;

	//패턴마다 대기 시간을 위한 변수
	TIME			m_tIdleTime;
	_uint			m_iPatternIndex = 0;
	//마법진 패턴 끝날 때 삭제하기 위해 들고 있음
	//CGameObject*	m_pMagicCircle = nullptr;
	const _float	m_fMagicCircleCreateDistance = 2.f;

	//시작 시 보스 1페
	_bool			m_bBossPhase1 = true;

	//1페 빨강 마법진 패턴
	_bool			m_bPhase1RedSkill = false;
	TIME			m_tPhase1RedSkillStartTime;

	//1페 파랑 마법진 패턴
	_bool			m_bPhase1BlueSkill = false;
	TIME			m_tPhase1BlueSkillStartTime;

	//1페 초록 마법진 패턴
	_bool			m_bPhase1GreenSkill = false;
	TIME			m_tPhase1GreenSkillStartTime;

	//2페 빨강 마법진 패턴
	_bool			m_bPhase2RedSkill = false;
	TIME			m_tPhase2RedSkillStartTime;

	//2페 파랑 마법진 패턴
	_bool			m_bPhase2BlueSkill = false;
	TIME			m_tPhase2BlueSkillStartTime;

	//2페 초록 마법진 패턴
	_bool			m_bPhase2GreenSkill = false;
	TIME			m_tPhase2GreenSkillStartTime;
	TIME			m_tPhase2GreenSkillThornTime;
	_bool			m_bPhase2GreenSkillFirst = false;

public:
	static CBossSkeleton* Create(_pDevice pDevice);
	virtual void Free() override;
	virtual CGameObject * Clone(void *pArg = (void *)nullptr);

	// CMonster을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;


	// CMonster을(를) 통해 상속됨
	virtual const _vec3 & Get_BoxColliderCenter() const override;

	// CMonster을(를) 통해 상속됨
};
END
#endif // !__CLIENT_PLAYER_H__
