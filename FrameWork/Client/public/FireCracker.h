#pragma once
#ifndef __CLIENT_FIRE_CRACKER_H__
#define __CLIENT_FIRE_CRACKER_H__

#include "CubeObject.h"

class CFireCracker final : public CCubeObject
{
public:
	typedef struct tagPoisonCubeParticleDesc {
		tagPoisonCubeParticleDesc()
		{
			ZeroMemory(this, sizeof(tagPoisonCubeParticleDesc));
		}

		OBJ_INFO					tObjInfo;
		_float						fPower;
	} DESC;

public:
	explicit CFireCracker(_pDevice pGraphicDevice);
	explicit CFireCracker(const CFireCracker& rhs);
	virtual ~CFireCracker() DEFAULT;


public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET

public: //GET

private:
	void	Tick_Move(_float fTimeDelta, CGameInstance* pGameInstance);
	void	Tick_Rotate(_float fTimeDelta, CGameInstance* pGameInstance);

	void	Set_RenderState();
	void	Release_RenderState();

private:
	_vec3			m_vTargetPos = _vec3(0.f, 0.f, 0.f);
	_vec3			m_vStartPos = _vec3(0.f, 0.f, 0.f);
	TIME			m_tTimeProjectile;
	const _float	m_fProjectileDegree = 65.f;
	_vec3			m_vProjectilePower = _vec3(0.f, 0.f, 0.f);
	_float			m_fRandomTime;

public:
	static CFireCracker*	Create(_pDevice pDevice);
	virtual CGameObject*	Clone(void* pArg = nullptr) override;
	virtual void			Free() override;
};


#endif // !__CLIENT_FIRE_CRACKER_H__
