#pragma once
#ifndef __CLIENT_MONSTER_H__
#define __CLIENT_MONSTER_H__

#include "Observer.h"
#include "PoisonCubeHit.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferRect;
class CTransform;
class CTexture;
END

BEGIN(Client)
class CMonster abstract:
	public CObserver
{
public:
	typedef struct tagMonsterStat {
		tagMonsterStat() { ZeroMemory(this, sizeof(*this)); }
		_int	iHp;
		_int	iMaxHp;
		_int	iDmg;

	}MONSTER_STAT;

public:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


	virtual _bool	Picking(const _vec3& vMouseRay, const _vec3& vMousePivot, CTransform* pOut);
public: //SET
	void Set_Frame(_int iEnd, wstring wstrObjectKey, wstring wstrStateKey, wstring wstrDir, _int iStart = 0
		, _float fTime = 0.f, _float fDelay = 0.1f);
	virtual HRESULT Sub_Hp(_uint iDmg);
	virtual void Set_Poison(TIME tTimePoisonTick, TIME tTimePoisonEnd, TIME tTimePoisonCreateHitParticle, _int iPoisonDmg);
	virtual void Set_PoisonHitTransform(CPoisonCubeHit::DESC* pDesc)PURE;

public: //GET
	const FRAME& Get_Frame() const { return m_tFrame; }
	const _int& Get_FrameIndex() const { return m_tFrame.iStart; }
	const _bool& Get_Poison() const { return m_bPoison; }
	virtual const _vec3& Get_BoxColliderCenter() const PURE;

protected:
	//프레임 증가
	virtual void	Tick_Frame(_float fTimeDelta);
	void			Tick_Poison(_float fTimeDelta);
	_bool Check_Dead();

	virtual void Collision_Terrain(_float fTimeDelta, class CGameInstance* pInstance, class CTransform* pTransform);
	virtual HRESULT BillBoarding(class CTransform* pTransform);

	virtual HRESULT SetUp_RenderState();
	virtual HRESULT Release_RenderState();

protected:
	FRAME				m_tFrame;
	MONSTER_STAT		m_tStat;

	_bool				m_bPoison = false;
	TIME				m_tTimePoisonTick;
	TIME				m_tTimePoisonEnd;
	_int				m_iPoisonDmg = 0;

	TIME				m_tTimePoisonCreateHitParticle;
	_bool				m_bCreateRight = false;
public:
	virtual void Free() override;
	virtual CGameObject * Clone(void *pArg = (void *)nullptr)PURE;
	virtual void Collision(CGameObject* pColObj) override;

public:
	virtual void Notify(void* pMessage = nullptr) override;

};
END
#endif // !__CLIENT_PLAYER_H__
