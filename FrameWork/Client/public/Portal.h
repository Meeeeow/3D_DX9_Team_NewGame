#pragma once
#ifndef __CLIENT_PORTAL_H__
#define __CLIENT_PORTAL_H__

BEGIN(Engine)
class CRenderer;
class CVIBufferCubeColor;
class CTransform;
class CFrustum;
class CBoxCollider;
END


#include "GameObject.h"
BEGIN(Client)
class CPortal final : public CGameObject
{
public:
	typedef struct tagProtalInfo {
		LEVEL						eNextLevel = LEVEL_END;
		_vec3						vPos = _vec3(0.f, 0.f, 0.f);
		_vec3						vScale = _vec3(1.f, 1.f, 1.f);
		_vec3						vTrans = _vec3(0.f, 0.f, 0.f);
		wchar_t						szBufferName[64];
	}DESC;

public:
	explicit CPortal(_pDevice pGraphicDevice);
	explicit CPortal(const CPortal& rhs);
	virtual ~CPortal() DEFAULT;

public:
	virtual HRESULT	NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	void			AlphaChange();
	
private:
	HRESULT			SetUp_Component(void* pArg);
	HRESULT			SetUp_RenderState();
	HRESULT			Release_RenderState();

private:
	CTransform*			m_pTransform = nullptr;
	CRenderer*			m_pRenderer = nullptr;
	CBoxCollider*		m_pBoxCollider = nullptr;
	CVIBufferCubeColor*	m_pVIBuffer = nullptr;
	CFrustum*			m_pFrustum = nullptr;
	DESC				m_tPortalInfo;
	_plane				m_plane[6];
	_bool				m_bUse = false;

public:
	virtual void	Collision(CGameObject* pColObj) override;
	virtual HRESULT Tick_Collider(_float fTimeDelta) override;

public:
	// CGameObject을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	static CPortal*	Create(_pDevice pGraphicDevice);
	virtual void	Free() override;
};
END

#endif // !__CLIENT_PORTAL_H__
