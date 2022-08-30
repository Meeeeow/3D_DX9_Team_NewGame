#pragma once
#ifndef __CLIENT_MAGIC_MISSILE_H__
#define __CLIENT_MAGIC_MISSILE_H__

BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBufferCube;
class CBoxCollider;
END

#include "CollisionCubeObject.h"
BEGIN(Client)
class CMagicMissile :
	public CCollisionCubeObject
{
public:
	typedef struct tagMagicMissileDesc {
		tagMagicMissileDesc() {
			ZeroMemory(this, sizeof(tagMagicMissileDesc));

			tTransformDesc.fDegree = 900.f;
			tTransformDesc.fSpeed = 5.f;
		}
		_vec3 vPos;
		_vec3 vTargetPos;
		_uint iTextureNum;
		CTransform::TRANSFORM_DESC tTransformDesc;
	} DESC;

public:
	explicit CMagicMissile(_pDevice pDevice);
	explicit CMagicMissile(const CMagicMissile& rhs);
	virtual ~CMagicMissile() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET

public: //GET

private:
	//Tick
	HRESULT Move(_float fTimeDelta);
	HRESULT RotateZ(_float fTimeDelta);
	void	Tick_Particle(_float fTimeDelta);

private:
	_float m_fRange = 0.f;
	const _float m_fMaxRange = 80.f;

	TIME	m_tTimeParticle;

public:
	static CMagicMissile* Create(_pDevice pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
	virtual void Collision(CGameObject* pColObj) override;
};
END
#endif // !__CLIENT_CUBE_OBJECT_H__
