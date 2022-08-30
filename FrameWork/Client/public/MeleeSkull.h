#pragma once

#ifndef __MELEESKULL_H__
#define __MELEESKULL_H__

#include "Monster.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
class CBoxCollider;
END	

BEGIN(Client)
class CMeleeSkull final :
	public CMonster
{
public:
	enum STATE { STATE_IDLE, STATE_MOVE, STATE_ATTACK, STATE_HIT, STATE_DEATH, STATE_DIE, STATE_SPAWN, STATE_END };

private:
	explicit CMeleeSkull(_pDevice pGraphicDevice);
	explicit CMeleeSkull(const CMeleeSkull& rhs);
	virtual ~CMeleeSkull() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual _bool Picking(const _vec3& vMouseRay, const _vec3& vMousePivot, CTransform* pOut) override;
private:
	HRESULT SetUp_Component(void* pArg);

	HRESULT Set_RenderState();
	HRESULT Release_RenderState();

private:
	void Set_State(STATE eState);
	void Set_Idle();

	void Check_Frame();
	virtual void Tick_Frame(_float fTimeDelta) override;

	virtual void Collision(CGameObject* pColObj) override;

	virtual HRESULT Sub_Hp(_uint iDmg);

private:
	void AI();

	void Move(_float fTimeDelta);
	void Attack();

	void Go_Necromencer(_float fTimeDelta);

	void Retreat(_float fTimeDelta);
	void Set_RetreatPos();

public:
	void Hit(_float fTimeDelta);

private:
	CTransform*		m_pTransformCom = nullptr;
	CVIBufferRect*	m_pVIBufferCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CTexture*		m_pTextureComArr[STATE_END];
	CBoxCollider*	m_pBoxColliderCom = nullptr;

	CTransform*		m_pPlayerTransformCom = nullptr;

private:
	STATE			m_ePreState = STATE_END;
	STATE			m_eCurState = STATE_SPAWN;

	_bool			m_bStateLock = true;
	_bool			m_bMove = false;

	TIME			m_tTimeHit;

	// 부활
	_bool			m_bResurrection = false;
	_bool			m_bDie = false;
	_bool			m_bOnce = false;

	// 키메라 온
	_bool			m_bChimera = false;
	_mat			m_matTarget;

	_bool			m_bClone = false;

	// 후퇴
	_bool			m_bRetreat = false;

	_float			m_fDist = 0.f;

	_vec3			m_vRetreatDir;

	TIME			m_tRetreatCount;

public:
	virtual void Free() override;
	static CMeleeSkull* Create(_pDevice pDevice);
	virtual CGameObject * Clone(void * pArg = (void*)nullptr) override;

public:
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;

public:
	virtual void Notify(void* pMessage = nullptr) override;

	virtual HRESULT Tick_Collider(_float fTimeDelta) override;


	// CMonster을(를) 통해 상속됨
	virtual void Set_PoisonHitTransform(CPoisonCubeHit::DESC * pDesc) override;


	// CMonster을(를) 통해 상속됨
	virtual const _vec3 & Get_BoxColliderCenter() const override;

};
END
#endif // !__MELEESKULL_H__
