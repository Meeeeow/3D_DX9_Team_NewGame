#pragma once
#ifndef __CLIENT_POISON_CUBE_ATTACK_H__
#define __CLIENT_POISON_CUBE_ATTACK_H__

#include "Attack.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CBoxCollider;
END

BEGIN(Client)
class CPoisonCubeAttack :
	public CAttack
{
public:
	typedef struct tagBossSkeletonStageCircleAttack {
		CAttack::DESC	tSuperDesc;

	public:
		void Set_Parent(CGameObject* pParent) { this->pParent = pParent; }
		CGameObject* Clone_Parent() { if (pParent != nullptr) { Safe_AddRef(pParent); return pParent; } return nullptr; }
	private:
		CGameObject*	pParent;

	}DESC;

public:
	explicit CPoisonCubeAttack(_pDevice pDevice);
	explicit CPoisonCubeAttack(const CPoisonCubeAttack& rhs);
	virtual ~CPoisonCubeAttack() = default;

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
	virtual void Set_Dead() override;

private:

private:
	CGameObject*	m_pParent = nullptr;
	list<class CMonster*> m_listMonster;


public:
	static CPoisonCubeAttack* Create(_pDevice pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

};
END
#endif // !__CLIENT_CUBE_OBJECT_H__
