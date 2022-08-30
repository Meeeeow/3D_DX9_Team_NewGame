#pragma once
#ifndef __CLIENT_CUBE_OBJECT_H__
#define __CLIENT_CUBE_OBJECT_H__

BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBufferCube;
class CBoxCollider;
END

#include "GameObject.h"
BEGIN(Client)
class CCubeObject :
	public CGameObject
{

public:
	explicit CCubeObject(_pDevice pDevice);
	explicit CCubeObject(const CCubeObject& rhs);
	virtual ~CCubeObject() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET

	//TRANSFORM
	HRESULT Set_TransformState(CTransform::STATE eTransformState, const _vec3& vState);
	HRESULT Set_TransformScale(_vec3 vScale);
	HRESULT Set_TransformScale(_float fX, _float fY, _float fZ);
	_float	Go_Straight(_float fTimeDelta);
	_float	Go_Dir(_float fTimeDelta, _vec3 vDir);
	_float	Go_Up(_float fTimeDelta);
	_float  Go_Left(_float fTimeDelta);
	HRESULT Rotation_Axis(const _vec3& vAxis, _float fTimeDelta);
	HRESULT SetUp_Rotation(_vec3 vAxis, _float fDegree);
	HRESULT Set_RotateSpeed(_float fRotateSpeed);
	HRESULT Set_Speed(_float fSpeed);
	//포물선 운동 x,y, 내부에서 셋까지 해줌
	void	Move_Projectile(_vec3 vStartPos, _vec3 vProjectilePower, _float fProjectileTime, _float fDegree);
	//인자 - 시작 지점, 타겟 지점, 타겟까지 도달 시간, 던지는 각도 // 리턴 - 힘
	_vec3	Get_ProjectilePowerToTarget(_vec3 vStartPos, _vec3 vTargetPos, _float fProjectileMaxTime, _float fDegree);


	HRESULT Set_BoxColliderTransform(CBoxCollider* pBoxCollider);

	void	Set_FrameValue(_uint iStart) { m_tFrame.iStart = iStart; };
	void	Set_AlphaBlend(_bool bAlphaBlend) { m_bAlphaBlend = bAlphaBlend; }

public: //GET
	_vec3			Get_TransformState(CTransform::STATE eTransformState);
	_vec3			Get_TransformScale();
	const _mat*		Get_TransformMatPtr() const;
	const _int&		Get_FrameIndex() const { return m_tFrame.iStart; }
	void			Get_BottomBufferPos(_vec3* vPosArr);
	const _float&   Get_Speed() const { return m_pTransform->Get_Speed(); }

protected:
	virtual HRESULT Tick_Mat(CBoxCollider* pBoxCollider);

private:
	HRESULT SetUp_Component(void* pArg);

private:
	CTransform*		m_pTransform = nullptr;
	CTexture*		m_pTexture = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CVIBufferCube*	m_pVIBuffer = nullptr;
	CFrustum*		m_pFrustum = nullptr;

private:
	FRAME			m_tFrame;
	_bool			m_bAlphaBlend = false;

public:
	static CCubeObject* Create(_pDevice pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

	// CGameObject을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};
END
#endif // !__CLIENT_CUBE_OBJECT_H__
