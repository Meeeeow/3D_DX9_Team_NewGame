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
	// �ٸ� ��ü���� �浹 ���� ���
	void	Add_Bound(_vec3 vBoundPos);
public: //SET
	void Set_Control(_bool bControl);
	void Sub_Hp(_uint iDmg);
	//���� �Ҹ� �ȵǸ� false ����
	_bool Sub_Mp(_uint iMp);

	//������Ʈ
	void Set_State(STATE_KEY eStateKey, DIR_KEY eDirKey);
	void Set_CurState(PLAYER_FSM eFSM);
	void Set_Idle();
	void Set_JumpEnd();

public: //GET
	const _vec3& Get_ColliderCenter() const;

public:
	// ������ �ѹ� ȣ���� �ʿ��� ���������� ȣ��
	HRESULT Ready_Observers();
#pragma endregion

#pragma region PublicVariable
public:
	static _float	m_fTagCoolTime;
	const _float	m_fTagCoolTimeMax = 2.f;

	//����׿뺯��
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

	// ��ų Ÿ�� �� ��Ÿ�� ����
	void Ready_SkillSet();



private:
	//Tick
	//Ű���� �Է� üũ
#pragma region CheckKeyboard
	HRESULT Check_KeyBoard(_float fTimeDelta, class CGameInstance* pInstance);
	HRESULT Check_MoveKey(_float fTimeDelta, class CGameInstance* pInstance);
	HRESULT Check_TagKey(_float fTimeDelta, class CGameInstance* pInstance);
	HRESULT Check_SkillKey(_float fTimeDelta, class CGameInstance* pGameInstance);
	HRESULT Check_Camera(_float fTimeDelta, class CGameInstance* pGameInstance);
#pragma endregion

	//���콺 �Է� üũ
	HRESULT Check_Mouse(_float fTimeDelta, class CGameInstance* pInstance);

	// 2D ���� Ű���� �Է� üũ
#pragma region 2DKeyboardCheck
	HRESULT	Check_KeyBoard2D(_float fTimeDelta, class CGameInstance* pInstance);
	HRESULT Check_MoveKey2D(_float fTimeDelta, class CGameInstance* pInstance);
	HRESULT Check_TagKey2D(_float fTimeDelta, class CGameInstance* pGameInstance);
	HRESULT Check_SkillKey2D(_float fTimeDelta, class CGameInstance* pGameInstance);
	void	Set_State2D(STATE_KEY eStateKey, DIR_KEY eDirKey);
	void	Tick_ChangeOut2D(_float fTimeDelta);
	void	Tick_ChangeIn2D(_float fTimeDelta);
#pragma endregion

//������ ����
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
	
//�±� �ý���
#pragma region Tag
	void Tick_TagCoolTime(_float fTimeDelta);
	//��ü �ƿ�
	void Tick_ChangeOut(_float fTimeDelta);
	//��ü ��
	void Tick_ChangeIn(_float fTimeDelta);
#pragma endregion

//�⺻ ����
#pragma region BasicMovement
	void Collision_Terrain(_float fTimeDelta, class CGameInstance* pInstance);
	void Jump(_float fTimeDelta, class CGameInstance* pGameInstance);
	void Run(_float fTimeDelta, class CGameInstance* pGameInstance);
	void Fall(_float fTimeDelta, class CGameInstance* pGameInstance);
#pragma endregion

	//�÷��̾� ��ų
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

	// ���� ������ ȣ��
	void Call_StatObserverChange() { m_tPlayerStat.bControl = m_bControl; m_pStatSubjectCom->Notify(&m_tPlayerStat); }
	// ��ų ������ ȣ��
	void Call_SkillObserverUse(_uint iSkillNum);
	void Call_SkillObserverChange();
#pragma endregion

public: 
	// Ÿ�� ���� ���� �� ȣ�� �� �Լ�
	void	On_theCube();
	void	Under_theCube();

	// ���� ��!
	_bool	Get_JumpUp() { return m_bJumpUp; }
	// �ܺο��� StateLock ��Ʈ�� �ϴ� ����
	void	Set_StateLock(_bool bStateLock) { m_bStateLock = bStateLock; }

	// Lock On ���� ���� �� �Լ�
	void	Lock_Off();
	//�浹
	virtual void Collision(CGameObject* pColObj) override;


	void	Set_Transform(const _vec3& vPos) { m_pTransformCom->SetUp_State(CTransform::STATE_POS, vPos); }

	//WorldMatrix Ptr�� �ޱ�
	_mat*	Get_WorldMatrix();

private:
#pragma region Component
	CRenderer*					m_pRendererCom = nullptr;
	CVIBufferRect*				m_pVIBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	// ���� ������Ʈ
	CSubject*					m_pStatSubjectCom = nullptr;
	// ��ų ������Ʈ 
	CSubject*					m_pSkillSubjectCom = nullptr;

	CBoxCollider*				m_pBoxCollider = nullptr;
	CTexture*					m_pTextureComArr[FSM_END];
#pragma endregion

private:

//�÷��̾� ����
#pragma region PlayerState
	PLAYER_STAT			m_tPlayerStat;
	PLAYER_SKILL		m_tPlayerSkill;
	SKILL_COOLTIME		m_tSkillCoolTime;
	_uint				m_iSkillMPCostArr[3];
	FRAME				m_tFrame;
	_bool				m_bStateLock = false;			//�÷��̾� �ൿ ���϶� �ٸ� �ൿ �Է� ���ϰ� ������
	_bool				m_bDirLock = false;				//�÷��̾� ���� ��ȯ ��Ʈ�� ��
	PLAYER_FSM			m_eCurState = FSM_IDLE_N;
	PLAYER_FSM			m_ePreState = FSM_END;
	STATE_KEY			m_eStateKey = STATE_KEY::STATE_END;
	DIR_KEY				m_eDirKey = DIR_KEY::DIR_END;
#pragma endregion

//�±� ���� ����
#pragma region Tag
	_bool				m_bControl = false;				//���� ��Ʈ�� ��
	_bool				m_bChangeOut = false;			//�±� �� ������ ģ��
	_bool				m_bChangeIn = false;			//�±� �� ������ ģ��
	_float				m_fChangeTargetDist = 0.f;		//�±� �� �̵��Ÿ�
	const _float		m_fChangeTargetMaxDist = 3.5f;	//�±� �� �̵��Ÿ�
#pragma endregion

	//���� ���� ����
	_bool				m_bInvincible = false;			//����

//���� ���� ����
#pragma region Jump
	_bool				m_bJump = false;				//���� ��
	_float				m_fJumpPower = 10.f;			//���� �Ŀ�
	_float				m_f3DJumpPower = 20.f;			//3D �����϶��� ���� ��
	_float				m_fJumpPosY = 0.f;				//���� ���� ��ġ
	_float				m_fJumpTime = 0.f;				//���� ���� �� �ð��� �󸶳� ������
	_float				m_fJumpHighest = 0.f;			//���� �ְ� ���� ���� (���� �ö󰥶� �������� ��� �ٸ��� �ֱ� ����)
	_float				m_fJumpSpeed = 0.f;				//2D �������� ���� �� �ӵ��� �����ϴ� �뵵
	_float				m_fJumpSpeedMax = 0.25f;		//2D �������� ���� �� �ְ� �ӵ� ��
	_vec3				m_fReturnPos = { 0.f,0.f,0.f };	//2D �������� ���� �� ���ƿ� ��ġ
	_bool				m_bJumpUp = false;				//���� �����ö� ��� �ٲٱ��
#pragma endregion

//���� ���� ����
#pragma region Fall
														//���� ���� ����
	_bool				m_bFall = false;
	_float				m_fFallPosY = 0.f;
	_float				m_fFallTime = 0.f;
	//�÷��̾ ������ ���°�
	_bool				m_bCollisionTerrain = false;	//���� �浹 üũ
														//�÷��̾ ť�긦 ���°�
	_bool				m_bCollisionTileCube = false;	//Ÿ�� �浹 üũ
#pragma endregion

	//���� 1��ų �����ؼ� ������Ʈ �ı� or ������
	_bool				m_bWarriorSkill1 = false;		//���� ù ��ų ���� ����

//���� 2��ų ���� ����
#pragma region WarriorSkill2
	//���� 2��ų ���� ����
	_bool				m_bWarriorSkill2 = false;
	//���� �Ÿ�
	const _float		m_fWarriorSkill2DashDistMax = 10.f;
	_float				m_fWarriorSkill2DashDist = 0.f;
	//���� �ӵ�
	const _float		m_fWarriorSkill2DashSpeed = 30.f;
	//������ �ӵ� ����� ����
	_float				m_fWarriorSkill2DashOriginalSpeed = 0.f;
#pragma endregion

//���� 3��ų ���
#pragma region WarriorSkill3
	//���� �ð� ���� �� ��ħ
	_bool				m_bWarriorSkill3 = false;
	TIME				m_tTimeWarriorSkill3;
	CGameObject*		m_pShield = nullptr;

#pragma endregion

	//���� 1��ų ���� ������Ʈ �߻�
	_bool				m_bMageSkill1 = false;

	//���� 2��ų ������
	_bool				m_bMageSkill2 = false;
	_bool				m_bTelekinesis = false;
	//���� 3��ų �޶���
	_bool				m_bMageSkill3 = false;

	//���� ��ų
#pragma region ThiefSkill
	//���� 1��ų �ܰ� ��ô
	_bool	m_bThiefSkill1 = false;

	//���� 2��ų �� ��ô
	_bool	m_bThiefSkill2 = false;
#pragma endregion

	_bool				m_bRunWet = false;

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDevice);
	virtual void Free() override;
	virtual CGameObject * Clone(void *pArg = (void *)nullptr);

	//������
	virtual unsigned long AddRef();

	// CGameObject��(��) ���� ��ӵ�
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
	virtual HRESULT Tick_Collider(_float fTimeDelta) override;

};
END
#endif // !__CLIENT_PLAYER_H__
