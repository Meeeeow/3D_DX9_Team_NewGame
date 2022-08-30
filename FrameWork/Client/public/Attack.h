#pragma once
#ifndef __CLIENT_ATTACK_H__
#define __CLIENT_ATTACK_H__

BEGIN(Engine)
class CTransform;
class CRenderer;
class CBoxCollider;
END

#include "GameObject.h"
BEGIN(Client)
class CAttack :
	public CGameObject
{
public:
	typedef struct tagAttackDesc {
		tagAttackDesc() {
			ZeroMemory(this, sizeof(tagAttackDesc));

			vLook = _vec3(0.f, 0.f, 1.f);
			vScale = _vec3(1.f, 1.f, 1.f);
		}

		_vec3	vPos;
		_vec3	vScale;
		//룩 기준으로 회전 정렬(일단은 y축 고려하지 않겠음 필요하면 추후 수정)
		_vec3	vLook;
		_uint	iDmg;
		_bool	bDestroy;
		_bool	bFreeze;
		_bool	bPoision;
		_bool	bSingleAttack;

	}DESC;
public:
	explicit CAttack(_pDevice pDevice);
	explicit CAttack(const CAttack& rhs);
	virtual ~CAttack() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET
	void Set_Attack(_bool bAttack) { m_bAttack = bAttack; }

	HRESULT SetUp_Scale(_vec3 vScale);

	void SetUp_Speed(_float fSpeed) { m_pTransform->Set_Speed(fSpeed); }

public: //GET
	_bool Get_Poision() { return m_tDesc.bPoision; }
	_bool Get_Freeze() { return m_tDesc.bFreeze; }
	_bool Get_Destroy() { return m_tDesc.bDestroy; }
	_uint Get_Dmg() { return m_tDesc.iDmg; }
	_bool Get_SingleAttack() { return m_tDesc.bSingleAttack; }
	_bool Get_Attack() { return m_bAttack; }

	_vec3 Get_Scale();

public:
	virtual void Collision(CGameObject* pColObj) override;
	virtual void Tick_Dead();

protected:
	HRESULT Tick_Collider_BossSkeletonStageCircleAttack(_float fTimeDelta);

	_float Go_Forward(_float fTimeDelta);
	_float Go_Right(_float fTimeDelta);
	_float Go_Left(_float fTimeDelta);
	_float Go_Back(_float fTimeDelta);

private:
	HRESULT SetUp_Component(void* pArg);

private:
	CTransform*		m_pTransform = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CBoxCollider*	m_pBoxCollider = nullptr;

	DESC			m_tDesc;
	_bool			m_bAttack = false;
	_uint			m_iDead = 0;

public:
	static CAttack* Create(_pDevice pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

	// CGameObject을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;

	virtual HRESULT Tick_Collider(_float fTimeDelta) override;

};
END
#endif // !__CLIENT_CUBE_OBJECT_H__
