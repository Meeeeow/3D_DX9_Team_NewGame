#pragma once

#ifndef __NECROMENCERICE_H__
#define __NECROMENCERICE_H__

#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBufferRect;
class CBoxCollider;
END

BEGIN(Client)
class CNecromencerIce final
	: public CGameObject
{
public:
	typedef struct tagIceDESC {
		_vec3 vTargetPos;
		OBJ_INFO* pObj;
	}DESC;

private:
	explicit CNecromencerIce(_pDevice pGraphicDevice);
	explicit CNecromencerIce(const CNecromencerIce& rhs);
	virtual ~CNecromencerIce() = default;

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
	_vec3				m_vLook;
	_float				m_fMaxDist = 0.f;

public:
	virtual void Free() override;
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CNecromencerIce* Create(_pDevice pGraphicDevice);

public:
	virtual void Collision(CGameObject* pColObj) override;
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};
END
#endif // !__NECROMENCERICE_H__
