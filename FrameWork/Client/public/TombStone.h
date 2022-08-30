#pragma once

#ifndef __TOMBSTONE_H__
#define __TOMBSTONE_H__

#include "Monster.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferCube;
class CTransform;
class CTexture;
class CBoxCollider;
END

BEGIN(Client)
class CTombStone final
	: public CMonster
{
public:
	enum STATE { STATE_IDLE, STATE_BROKEN, STATE_END };

private:
	explicit CTombStone(_pDevice pGraphicDevice);
	explicit CTombStone(const CTombStone& rhs);
	virtual ~CTombStone() = default;

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
	virtual void Collision(CGameObject* pColObj) override;
	virtual void Collision_Terrain(_float fTimeDelta, class CGameInstance* pInstance, class CTransform* pTransform);

private:
	void Particle(_float fTimeDelta);

private:
	void AI(_float fTimeDelta);

	void Spawn();

private:
	CTransform*		m_pTransformCom = nullptr;
	CVIBufferCube*	m_pVIBufferCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CBoxCollider*	m_pBoxColliderCom = nullptr;

	CTransform*		m_pPlayerTransformCom = nullptr;

private:
	TIME			m_tTimeHit;

	_bool			m_bInvincible = false;

	STATE			m_eState = STATE_IDLE;

	_float			m_fTimeSpawn = 0.f;
	_float			m_fTimeDelay = 5.f;

	_bool			m_bStopParticle = false;

	TIME			m_tParticle;

public:
	virtual void Free() override;
	static CTombStone* Create(_pDevice pDevice);
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
#endif // !__TOMBSTONE_H__
