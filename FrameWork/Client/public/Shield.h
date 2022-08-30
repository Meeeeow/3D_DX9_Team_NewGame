#pragma once

#ifndef __SHIELD_H__
#define __SHIELD_H__

#include "Effect.h"
#include "Necromencer.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferCube;
class CTransform;
class CTexture;
class CFrustum;
END

BEGIN(Client)
class CShield final :
	public CEffect
{
private:
	explicit CShield(_pDevice pGraphicDevice);
	explicit CShield(const CShield& rhs);
	virtual ~CShield() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Set_RenderState();
	HRESULT Release_RenderState();
	HRESULT SetUp_Component();

public:
	void Broken();
	void Add_Master(CNecromencer* pObj) { m_pMaster = pObj; }

public:
	_vec3 Get_ShieldPos() { return m_pTransformCom->Get_State(CTransform::STATE_POS); }

private:
	CRenderer*					m_pRendererCom = nullptr;
	CVIBufferCube*				m_pVIBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;
	CFrustum*					m_pFrustumCom = nullptr;
	CGameObject*				m_pShieldCube = nullptr;

private:
	CNecromencer*				m_pMaster = nullptr;

public:
	static CShield* Create(_pDevice pGraphicDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free();
};
END
#endif // !__SHIELD_H__
