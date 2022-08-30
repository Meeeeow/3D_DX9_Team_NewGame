#pragma once
#ifndef __CLIENT_BOSS_SKELETON_STAGE_CIRCLE_ATTACK_H__
#define __CLIENT_BOSS_SKELETON_STAGE_CIRCLE_ATTACK_H__

#include "Attack.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CBoxCollider;
END

BEGIN(Client)
class CBossSkeletonStageCircleAttack :
	public CAttack
{
public:
	enum TYPE { TYPE_RIGHT, TYPE_FORWARD, TYPE_LEFT, TYPE_BACK, TYPE_END};

	typedef struct tagBossSkeletonStageCircleAttack {
		CAttack::DESC	tSuperDesc;
		TYPE			eType;
		_float			fSpeed;

	public:
		void Set_Parent(CGameObject* pParent) { this->pParent = pParent; }
		CGameObject* Clone_Parent() { if (pParent != nullptr) { Safe_AddRef(pParent); return pParent; } return nullptr; }
	private:
		CGameObject*	pParent;

	}DESC;

public:
	explicit CBossSkeletonStageCircleAttack(_pDevice pDevice);
	explicit CBossSkeletonStageCircleAttack(const CBossSkeletonStageCircleAttack& rhs);
	virtual ~CBossSkeletonStageCircleAttack() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Collision(CGameObject* pColObj) override;
	virtual void Tick_Dead() override;
	virtual HRESULT Tick_Collider(_float fTimeDelta) override;

private:
	HRESULT Tick_Right(_float fTimeDelta, CGameInstance* pGameInstance);
	HRESULT Tick_Left(_float fTimeDelta, CGameInstance* pGameInstance);
	HRESULT Tick_Forward(_float fTimeDelta, CGameInstance* pGameInstance);
	HRESULT Tick_Back(_float fTimeDelta, CGameInstance* pGameInstance);

	HRESULT Tick_Scale();

private:
	CGameObject*	m_pParent = nullptr;
	TYPE			m_eType;

	_float			m_fDist = 0.f;
	const _float	m_fDistMax = 3.f;
	_float			m_fScale = 1.f;

public:
	static CBossSkeletonStageCircleAttack* Create(_pDevice pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};
END
#endif // !__CLIENT_CUBE_OBJECT_H__
