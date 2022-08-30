#pragma once
#ifndef __CLIENT_COLLISION_CUBE_H__
#define __CLIENT_COLLISION_CUBE_H__

BEGIN(Engine)
class CBoxCollider;
END

#include "CubeObject.h"
BEGIN(Client)
class CCollisionCubeObject :
	public CCubeObject
{

public:
	explicit CCollisionCubeObject(_pDevice pDevice);
	explicit CCollisionCubeObject(const CCollisionCubeObject& rhs);
	virtual ~CCollisionCubeObject() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET
	HRESULT Set_TransformState(CTransform::STATE eTransformState, const _vec3& vState);
	HRESULT Set_TransformScale(_vec3 vScale);
	HRESULT Set_TransformScale(_float fX, _float fY, _float fZ);
	_float	Go_Straight(_float fTimeDelta);
	HRESULT Rotation_Axis(const _vec3& vAxis, _float fTimeDelta);
	HRESULT Set_RotateSpeed(_float fRotateSpeed);
	HRESULT Set_Speed(_float fSpeed);

public: //GET
	_vec3	Get_TransformState(CTransform::STATE eTransformState);

private:
	HRESULT SetUp_Component(void* pArg);

private:
	CBoxCollider*	m_pBoxCollider = nullptr;

public:
	static CCollisionCubeObject* Create(_pDevice pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual HRESULT Tick_Collider(_float fTimeDelta) override;

};
END
#endif // !__CLIENT_CUBE_OBJECT_H__
