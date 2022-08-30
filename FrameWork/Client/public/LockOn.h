#pragma once
#ifndef __CLIENT_LOCK_ON_H__
#define __CLIENT_LOCK_ON_H__


BEGIN(Engine)
class CRenderer;
class CVIBufferTerrain;
class CTexture;
class CTransform;
class CBoxCollider;
END

#include "GameObject.h"
BEGIN(Client)
class CLockOn final : public CGameObject
{
public:
	typedef struct tagLockOn {
		_mat	mat;
		_float	fTransformSpeed = 1.f;
		_float	fTransformRotation = 35.f;
		class CPlayer* pObject = nullptr;
	}DESC;
public:
	explicit CLockOn(_pDevice pGraphicDevice);
	explicit CLockOn(const CLockOn& rhs);
	virtual ~CLockOn() DEFAULT;

public:
	virtual	HRESULT	NativeConstruct_Prototype() override;
	virtual HRESULT	NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void			Move(_float fTimeDelta, CGameInstance* pGameInstance);
	void			Control_Check(_float fTimeDelta, CGameInstance* pGameInstance);
	void			Fire(_float fTimeDelta, CGameInstance* pGameInstance);

private:
	HRESULT			SetUp_Component(void* pArg);
	HRESULT			SetUp_RenderState();
	HRESULT			Release_RenderState();

private:
	CVIBufferTerrain*	m_pVIBuffer = nullptr;
	CTexture*			m_pTexture = nullptr;
	CTransform*			m_pTransform = nullptr;
	CRenderer*			m_pRenderer = nullptr;

	_bool				m_bControl = true;
	_float				m_fControlTime = 13.f;
	_float				m_fFireTime = 5.f;
	_float				m_fFireWait = 0.f;
	DESC				m_tDesc;

public:
	static	CLockOn*		Create(_pDevice pGraphicDevice);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;

	// CGameObject을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};
END
#endif // !__CLIENT_LOCK_ON_H__
