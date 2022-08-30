#pragma once
#ifndef __CLIENT_BOSS_CLOWN_H__
#define __CLIENT_BOSS_CLOWN_H__

#include "Monster.h"
#include "CardBoard.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
class CBoxCollider;
END

BEGIN(Client)
class CBossClown final : public CMonster
{
public:
	enum CLOWN_FSM { FSM_CLOWN_IDLE, FSM_CLOWN_DANCE, FSM_CLOWN_DODGE, FSM_CLOWN_ATTACK, FSM_CLOWN_DEAD, FSM_CLOWN_END};
#define CARDCOUNTMAX 52
public:
	typedef struct tagDeck {
		_uint	iCardTextureNum = 0;
		_uint	iCardObjectsNum = 0;
		_float	fShuffleCount = 7.f;
		_bool	bShuffle = false;
	}DECK;
private:
	explicit CBossClown(_pDevice pGraphicDevice);
	explicit CBossClown(const CBossClown& rhs);
	virtual ~CBossClown() DEFAULT;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual _bool Picking(const _vec3& vMouseRay, const _vec3& vMousePivot, CTransform* pOut) override;
private:
	HRESULT			SetUp_Component(void* pArg);
	void			Performance(_float fTimeDelta, CGameInstance* pGameInstance);
	HRESULT			Shuffle(_float fTimeDelta);
	HRESULT			Draw(CGameInstance* pGameInstance);
	HRESULT			Falling(CGameInstance* pGameInstance);
	HRESULT			Fall(_float fTimeDelta, CGameInstance* pGameInstance);
	void			Welcome_ToMyCircus();
	void			IsThis_An_Attack_QuestionMark(_vec3 vPos);
	void			Circus_Epilouge();
	void			Lost_Card(_bool bVisible);
	void			Lost_CardVisible();
	HRESULT			Dodge();
	virtual HRESULT Sub_Hp(_uint iDmg) override;
	_bool			Get_Card() { return m_bLostCardNumbers[(m_tCardDeck.iCardObjectsNum * 13) + m_tCardDeck.iCardTextureNum]; }
private:
	void			Set_State(CLOWN_FSM eNextState);
	void			Check_Frame();
	virtual void	Tick_Frame(_float fTimeDelta,CGameInstance* pGameInstance);
private:
	// Component
	CRenderer*		m_pRenderer = nullptr;
	CVIBufferRect*	m_pVIBuffer = nullptr;
	CTransform*		m_pTransform = nullptr;
	CTexture*		m_pTexture[FSM_CLOWN_END];
	CBoxCollider*	m_pBoxCollider = nullptr;
	CFrustum*		m_pFrustum = nullptr;
	CCardBoard*		m_pCardBoard = nullptr;

	// Clown Stat
	MONSTER_STAT	m_tClownStat;
	DECK			m_tCardDeck;
	_float			m_fDeadTime = 3.f;

	// Clown Intro
	_bool			m_bIntroSound = false;

	// Frame
	CLOWN_FSM		m_eCurState = FSM_CLOWN_DANCE;
	CLOWN_FSM		m_ePreState = FSM_CLOWN_END;

	// Lost Card
	_bool			m_bLostCardNumbers[CARDCOUNTMAX];

public:
	// CMonster을(를) 통해 상속됨
	virtual _vec3			Get_MinPos() override;
	virtual _vec3			Get_MaxPos() override;
	virtual _vec3			Get_WorldPos() override;
	virtual _vec3			Get_Length() override;
	virtual CGameObject*	Clone(void * pArg = (void*)nullptr) override;
	static CBossClown*		Create(_pDevice pGraphicDevice);
	virtual void			Free() override;
	virtual void			Collision(CGameObject* pColObj) override;
	virtual HRESULT			Tick_Collider(_float fTimeDelta) override;

	// CMonster을(를) 통해 상속됨
	virtual void Set_PoisonHitTransform(CPoisonCubeHit::DESC * pDesc) override;

	// CMonster을(를) 통해 상속됨
	virtual const _vec3 & Get_BoxColliderCenter() const override;
};
END
#endif // !__CLIENT_BOSS_CLOWN_H__
