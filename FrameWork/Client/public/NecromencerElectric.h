#pragma once

#ifndef __NECROMENCERELECTRIC_H__
#define __NECROMENCERELECTRIC_H__

#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBufferRect;
class CBoxCollider;
END

BEGIN(Client)
class CNecromencerElectric final 
	: public CGameObject
{
public:
	explicit CNecromencerElectric(_pDevice pGraphicDevice);
	explicit CNecromencerElectric(const CNecromencerElectric& rhs);
	virtual ~CNecromencerElectric() = default;

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
	void Update_Frame(_float fTimeDelta);
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

public:
	virtual void Free() override;
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CNecromencerElectric* Create(_pDevice pGraphicDevice);

public:
	virtual void Collision(CGameObject* pColObj) override;
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;

};
END
#endif // !__NECROMENCERELECTRIC_H__
