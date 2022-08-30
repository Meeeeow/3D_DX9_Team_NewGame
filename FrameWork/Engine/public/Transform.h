#pragma once
#ifndef __ENGINE_TRANSFOMR_H__
#define __ENGINE_TRANSFOMR_H__

#include "Component.h"
BEGIN(Engine)
class ENGINE_DLL CTransform :
	public CComponent
{
public:
	enum STATE {STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POS, STATE_END};

	typedef struct tagTransformDesc
	{
		tagTransformDesc() { ZeroMemory(this, sizeof(tagTransformDesc)); }
		_float fSpeed;
		_float fDegree;
	}TRANSFORM_DESC;

public:
	explicit CTransform(_pDevice pGraphicDevice);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public: //GET
	_vec3 Get_State(STATE eState) const {
		return *(_vec3*)(&m_matWorld.m[eState][0]);
	}

	_vec3 Get_Scale() const {
		return _vec3(D3DXVec3Length(&Get_State(STATE_RIGHT)),
			D3DXVec3Length(&Get_State(STATE_UP)),
			D3DXVec3Length(&Get_State(STATE_LOOK)));
	}

	const _mat* Get_WorldMatrixPtr() const { return &m_matWorld; }

	_float Get_DistanceTarget(CTransform* pTransform);
	const _float& Get_Speed() const { return m_tTransformDesc.fSpeed; }
public: //SET
	void SetUp_State(STATE eState, const _vec3& vState) {
		memcpy(&m_matWorld.m[eState][0], &vState, sizeof(_vec3));
	}
	//�� ���� ������ �ش� �迡 ���缭 �������·� �������
	void SetUp_LookOrtho(const _vec3& vLook);
	void SetUp_UpOrtho(const _vec3& vUp);
	//�� ���� ������ �ش� �迡 ���缭 �������·� �������(y���� 0, 1, 0)
	void SetUp_LookOrthoWithoutY(const _vec3& vLook);
	void SetUp_Scale(_float fScaleX, _float fScaleY, _float fScaleZ);
	void SetUp_Scale(_vec3 vScale);
	void SetUp_Rotation(const _vec3& vAxis, _float fDegree);
	void Set_Matrix(_mat matWorld) { m_matWorld = matWorld; }
	void Set_Speed(_float fSpeed) { m_tTransformDesc.fSpeed = fSpeed; }
	void Set_RotateSpeed(_float fRotateSpeed) { m_tTransformDesc.fDegree = fRotateSpeed; }

public: // ADD
	void Add_Position(_vec3 vPos);

public:
	//8���� �̵�
	_float	Go_Straight(_float fTimeDelta);
	_float	Go_Backward(_float fTimeDelta);
	_float	Go_Right(_float fTimeDelta);
	_float	Go_Left(_float fTimeDelta);
	_float	Go_StraightRight(_float fTimeDelta);
	_float	Go_StraightLeft(_float fTimeDelta);
	_float	Go_BackwardRight(_float fTimeDelta);
	_float	Go_BackwardLeft(_float fTimeDelta);

	_float	Go_Up(_float fTimeDelta);
	_float	Go_Down(_float fTimeDelta);
	_float	Go_Dir(_float fTimeDelta, _vec3 vDir);

	//��Ʈ ����(�����ϰ� ���� ���Ȳ�� ���ǹ� �ɾ ���� �ϱ� ���� ȣ���ϰ� �� ���Ŀ� ȣ��)
	void Invert_Rectangle();

	//����, �̵��� y ��ġ�� �����ϰ� ��ȯ, ������ � y
	_float	Jump(_float fJumpPos, _float fJumpPower, _float fJumpTime);

	//������ � x,y, ���ο��� �±��� ����, ProjectileTime�� ƽ�� �����ؼ� �������ָ��
	void	Projectile(_vec3 vStartPos, _vec3 vProjectilePower, _float fProjectileTime, _float fDegree);
	//���� - ���� ����, Ÿ�� ����, Ÿ�ٱ��� ���� �ð�, ������ ���� // ���� - ��
	_vec3	Get_ProjectilePowerToTarget(_vec3 vStartPos, _vec3 vTargetPos, _float fProjectileMaxTime, _float fDegree);

	_float	Fall(_float fFallPos, _float fFallTime);
	void	Rotation_Axis(_vec3 vAxis, _float fTimeDelta);

	void Chase_Target(CTransform* pTargetTransform, _float fTimeDelta, _float fDistance = 1.f);
	void Chase_Vector(_vec3 vTargetPos, _float fTimeDelta, _float fDistance = 1.f);
	void Look_Target(CTransform* pTargetTransform);
	void Look_TargetByLandObject(CTransform * pTargetTransform);

	HRESULT Bind_OnDevice();


	// ������Ÿ�� �� ����
	void ReSet_Rotation();
	void Prototype_Rotation(_int iValueX, _int iValueY, _int iValueZ);

public: // �����
	void SetUp_DebuffTime(_float fDebuffTime) { m_fDebuffTime = fDebuffTime; }
	void SetUp_DebuffSpeed(_float fDebuffSpeed) { m_fDebuffSpeed = fDebuffSpeed; }
	void Tick_Debuff(_float fTimeDelta);
private:
	_mat m_matWorld;
	TRANSFORM_DESC m_tTransformDesc;

	// �����
	_float	m_fDebuffSpeed = 0.f;
	_float	m_fDebuffTime = 0.f;
	_uint	m_iDebuffTick = 0;

	//������ ��� ���� �߷°�
	const _vec3 m_vGraivty = _vec3(0.f, -9.8f, 0.f);

public:
	virtual void Free() override;
	virtual CComponent * Clone(void * pArg) override;
	static CTransform* Create(_pDevice pGraphicDevice);
};
END
#endif // !__ENGINE_TRANSFOMR_H__
