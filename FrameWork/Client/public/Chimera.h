#pragma once

#ifndef __CHIMERA_H__
#define __CHIMERA_H__

#include "Monster.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
class CBoxCollider;
class CSubject;
END

BEGIN(Client)
class CChimera final
	: public CMonster
{
public:
	enum STATE { STATE_IDLE, STATE_MOVE, STATE_ATTACK, STATE_CAST, STATE_DEATH, STATE_END };

private:
	explicit CChimera(_pDevice pGraphicDevice);
	explicit CChimera(const CChimera& rhs);
	virtual ~CChimera() = default;

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
	void	Shokan_Watashino_Potaru();
private:
	void Set_State(STATE eState);

	void Set_Idle();

	void Check_Frame();

	virtual void Tick_Frame(_float fTimeDelta) override;

	virtual void Collision(CGameObject* pColObj) override;

	virtual HRESULT Sub_Hp(_uint iDmg);

private:
	void AI(_float fTimeDelta);

	void Move(_float fTimeDelta);

private:
	void Fire();

	void Ice();

	void Eletric();

	void Resurrection();

	void DarkOrb();

	void Attack();

	void Blink();

private:
	CTransform*		m_pTransformCom = nullptr;
	CVIBufferRect*	m_pVIBufferCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CTexture*		m_pTextureComArr[STATE_END];
	CBoxCollider*	m_pBoxColliderCom = nullptr;
	CSubject*		m_pSubjectCom = nullptr;

	CTransform*		m_pPlayerTransformCom = nullptr;

private:
	TIME			m_tTimeHit;
	STATE			m_ePreState = STATE_END;
	STATE			m_eCurState = STATE_IDLE;

	_bool			m_bStateLock = false;

	TIME			m_tDarkOrb;
	TIME			m_tPattern;

	_bool			m_bFire = false;
	_bool			m_bBlink = false;

	_int			m_iPatternNum = 0;
	_int			m_iShotCount = 0;

public:
	virtual void Free() override;
	static CChimera* Create(_pDevice pDevice);
	virtual CGameObject * Clone(void * pArg = (void*)nullptr) override;

public:
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;

public:
	virtual void Notify(void* pMessage = nullptr) override;

	HRESULT Tick_Collider(_float fTimeDelta);


	// CMonster을(를) 통해 상속됨
	virtual void Set_PoisonHitTransform(CPoisonCubeHit::DESC * pDesc) override;


	// CMonster을(를) 통해 상속됨
	virtual const _vec3 & Get_BoxColliderCenter() const override;

};
END
#endif // !__CHIMERA_H__
