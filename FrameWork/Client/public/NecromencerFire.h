#pragma once

#ifndef __NECROMENCERFIRE_H__
#define __NECROMENCERFIRE_H__

BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBufferRect;
class CBoxCollider;
END

#include "GameObject.h"

BEGIN(Client)
class CNecromencerFire final 
	: public CGameObject
{
public:
	typedef struct tagFireDESC {
		_vec3 vTargetPos;
		OBJ_INFO* pObj;
	}DESC;

private:
	explicit CNecromencerFire(_pDevice pGraphicDevice);
	explicit CNecromencerFire(const CNecromencerFire& rhs);
	virtual ~CNecromencerFire() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT SetUp_Component(void * pArg);

	HRESULT Set_RenderState();
	HRESULT Release_RenderState();

private:
	void Tick_Frame(_float fTimeDelta);

private:
	HRESULT Move(_float fTimeDelta);
	HRESULT BillBoarding(CTransform * pTransform);

public:
	HRESULT Tick_Collider(_float fTimeDelta);

private:
	CRenderer*			m_pRendererCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CVIBufferRect*		m_pVIBufferCom = nullptr;
	CBoxCollider*		m_pBoxColliderCom = nullptr;

private:
	FRAME				m_tFrame;
	_vec3				m_vLook;

	_float				m_fDist = 0.f;

public:
	static CNecromencerFire* Create(_pDevice pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
	virtual void Collision(CGameObject* pColObj) override;

public:
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;

};
END
#endif // !__NECROMENCERFIRE_H__
