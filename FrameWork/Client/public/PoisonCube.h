#pragma once
#ifndef __CLIENT_POISON_CUBE_H__
#define __CLIENT_POISON_CUBE_H__

BEGIN(Engine)
class CBoxCollider;
END

#include "CubeObject.h"
BEGIN(Client)
class CPoisonCube final:
	public CCubeObject
{
public:
	typedef struct tagPoisonCubeDesc {
		tagPoisonCubeDesc()
		{
			ZeroMemory(this, sizeof(tagPoisonCubeDesc));
		}

		OBJ_INFO					tObjInfo;
		_vec3						vTargetPos;
		CTransform::TRANSFORM_DESC	tTransformDesc;
		_bool						bRayFail;
	} DESC;
public:
	explicit CPoisonCube(_pDevice pDevice);
	explicit CPoisonCube(const CPoisonCube& rhs);
	virtual ~CPoisonCube() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET

public: //GET

private:
	HRESULT SetUp_TransformDesc(CTransform::TRANSFORM_DESC tTransformDesc);

	void	Tick_Move(_float fTimeDelta, CGameInstance* pGameInstance);
	void	Tick_Rotate(_float fTimeDelta, CGameInstance* pGameInstance);
	void	Tick_Particle(_float fTimeDelta, CGameInstance* pGameInstance);
	void	Tick_WaitDead(_float fTimeDelta, CGameInstance* pGameInstance);

	void	Dead_Event(_float fTimeDelta, CGameInstance* pGameInstance);

	void	Set_RenderState();
	void	Release_RenderState();

private:
	_vec3			m_vTargetPos = _vec3(0.f, 0.f, 0.f);
	_vec3			m_vStartPos = _vec3(0.f, 0.f, 0.f);
	TIME			m_tTimeProjectile;
	const _float	m_fProjectileDegree = 30.f;
	_vec3			m_vProjectilePower = _vec3(0.f, 0.f, 0.f);

	_bool			m_bRender = true;
	_bool			m_bParticle = false;
	TIME			m_tTimeParticle;

	_uint			m_iCount = 0;
	const _uint		m_iMaxCount = 5;

	TIME			m_tTimeWaitDead;
	_bool			m_bWaitDead = false;

public:
	static CPoisonCube* Create(_pDevice pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};
END
#endif // !__CLIENT_CUBE_OBJECT_H__
