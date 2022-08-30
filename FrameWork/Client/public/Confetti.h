#pragma once
#ifndef __CLIENT_CONFETTI_H__
#define __CLIENT_CONFETTI_H__

#include "CubeObject.h"

class CConfetti final : public CCubeObject
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
	explicit CConfetti(_pDevice pGraphicDevice);
	explicit CConfetti(const CConfetti& rhs);
	virtual ~CConfetti() DEFAULT;

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
	static CConfetti*		Create(_pDevice pGraphicDevice);
	virtual CGameObject*	Clone(void* pArg = nullptr) override;
	virtual void			Free() override;
};

#endif // !__CLIENT_CONFETTI_H__