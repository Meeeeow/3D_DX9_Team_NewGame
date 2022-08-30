#pragma once
#ifndef __CLIENT_RUN_WET_PARTICLE_H__
#define __CLIENT_RUN_WET_PARTICLE_H__

#include "CubeObject.h"

BEGIN(Engine)
END

BEGIN(Client)
class CRunWetParticle final:
	public CCubeObject
{
public:
	typedef struct tagRunWetParticleDesc {
		tagRunWetParticleDesc()
		{
			ZeroMemory(this, sizeof(tagRunWetParticleDesc));
		}

		OBJ_INFO					tObjInfo;
		_float						fPower;
		_float						fPowerY;
	} DESC;

private:
	explicit CRunWetParticle(_pDevice pDevice);
	explicit CRunWetParticle(const CRunWetParticle& rhs);
	virtual ~CRunWetParticle() = default;

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

	void	Set_RenderState();
	void	Release_RenderState();

private:
	_vec3			m_vStartPos = _vec3(0.f, 0.f, 0.f);
	TIME			m_tTimeProjectile;
	_vec3			m_vProjectilePower = _vec3(0.f, 0.f, 0.f);
	_float			m_fRandomTime;
	_float			m_fProjectileDegree;

public:
	static CRunWetParticle* Create(_pDevice pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END
#endif // !__CLIENT_CUBE_OBJECT_H__
