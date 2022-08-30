#pragma once
#ifndef __CLIENT_BOSS_SEKELETON_THORN_H__
#define __CLIENT_BOSS_SEKELETON_THORN_H__

#include "ThornObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferThorn;
class CTransform;
class CTexture;
class CFrustum;
class CBoxCollider;
END

BEGIN(Client)
class CBossSkeletonThorn final :
	public CThornObject
{
public:
	typedef struct tagBossSkeletonThornDesc {
		CThornObject::DESC tSuperDesc;
		_float fDelay;
	} DESC;

public:
	explicit CBossSkeletonThorn(LPDIRECT3DDEVICE9 pGraphicDevice);
	explicit CBossSkeletonThorn(const CBossSkeletonThorn& rhs);
	virtual ~CBossSkeletonThorn() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET

public: //GET

private:
	void Dead_Event(CGameInstance* pGameInstance);

	//Tick
	HRESULT Tick_Up(_float fTimeDelta, CGameInstance* pGameInstance);
	HRESULT Tick_Down(_float fTimeDelta, CGameInstance* pGameInstance);

private:
	const _float		m_fMaxDist = 6.f;
	_float				m_fDist = 0.f;
	_bool				m_bUp = true;

	TIME				m_tTimeWait;
	const _float		m_fDownSpeed = 2.f;

public:
	virtual void Free() override;
	static CBossSkeletonThorn* Create(_pDevice pDevice);
	virtual CGameObject * Clone(void *pArg = (void *)nullptr) override;
	virtual void Collision(CGameObject* pColObj) override;
};
END
#endif // !__CLIENT_PLAYER_H__
