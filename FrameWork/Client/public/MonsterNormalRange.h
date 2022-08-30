#pragma once
#ifndef __CLIENT_MONSTER_NORMAL_RANGE_H__
#define __CLIENT_MONSTER_NORMAL_RANGE_H__

#include "Monster.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
class CBoxCollider;
END

BEGIN(Client)
class CMonsterNormalRange final:
	public CMonster
{
public: 
	enum MONSTER_FSM { FSM_IDLE, FSM_MOVE, FSM_ATTACK, FSM_DIE, FSM_HIT, FSM_END};

public:
	typedef struct tagMonsterStat {
		tagMonsterStat() { ZeroMemory(this, sizeof(*this)); }

		_int	iHp;
		_int	iMaxHp;
		_int	iDmg;

	}MONSTER_STAT;

private:
	explicit CMonsterNormalRange(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CMonsterNormalRange(const CMonsterNormalRange& rhs);
	virtual ~CMonsterNormalRange() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET
	void Set_State(MONSTER_FSM eState);
	void Set_Idle();
	virtual void Set_PoisonHitTransform(CPoisonCubeHit::DESC * pDesc) override;

	virtual	HRESULT Sub_Hp(_uint iDmg) override;

public: //GET

private:
	//SetUp
	HRESULT SetUp_Component(void *pArg);

	//Render
	HRESULT Set_RenderState();
	HRESULT Release_RenderState();

	//Tick
	//State 관리하는 함수
	void AI();
	//State에 맞게 값들 초기화 해주는 함수
	void Check_Frame();
	void Move(_float fTimeDelta);
	void Hit(_float fTimeDelta);
	virtual void Tick_Frame(_float fTimeDelta) override;
	virtual _bool Picking(const _vec3& vMouseRay, const _vec3& vMousePivot, CTransform* pOut) override;
private: //Component
	CTransform*		m_pTransform = nullptr;
	CVIBufferRect*	m_pVIBuffer = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CTexture*		m_pTextureArr[FSM_END];
	CBoxCollider*	m_pBoxCollider = nullptr;

private: //Variable
	MONSTER_FSM		m_eCurState = FSM_IDLE;
	MONSTER_FSM		m_ePreState = FSM_END;
	_bool			m_bStateLock = false;
	TIME			m_tTimeHit;

	//플레이어 위치 추적을 위한 변수
	CTransform*		m_pTransformPlayer = nullptr;

	//스탯 변수
	MONSTER_STAT	m_tMonsterStat;

public:
	static CMonsterNormalRange* Create(_pDevice pDevice);
	virtual void Free() override;
	virtual CGameObject * Clone(void *pArg = (void *)nullptr);

	// CMonster을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
	virtual void Collision(CGameObject* pColObj) override;


	// CMonster을(를) 통해 상속됨
	virtual const _vec3 & Get_BoxColliderCenter() const override;

};
END
#endif // !__CLIENT_PLAYER_H__
