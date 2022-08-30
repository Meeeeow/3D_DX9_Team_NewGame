#pragma once
#ifndef __CLIENT_TIP_OBJECT_H__
#define __CLIENT_TIP_OBJECT_H__


BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBuffer;
END
#include "GameObject.h"

BEGIN(Client)
class CTipObject :
	public CGameObject
{
public:
	typedef struct tagTipObjectDesc {
		tagTipObjectDesc()
		{
			ZeroMemory(this, sizeof(tagTipObjectDesc));
		}

		OBJ_INFO		tObjInfo;
		const _tchar*	pTagPrototypeVIBuffer;
		LEVEL			eTextureLevel;
		_bool			bBlend;

	}DESC;

public:
	explicit CTipObject(_pDevice pDevice);
	explicit CTipObject(const CTipObject& rhs);
	virtual ~CTipObject() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET

	//TRANSFORM
	HRESULT Set_TransformState(CTransform::STATE eTransformState, const _vec3& vState);
	HRESULT Set_TransformScale(_vec3 vScale);
	HRESULT Set_TransformScale(_float fX, _float fY, _float fZ);
	HRESULT Set_LookOrtho(const _vec3& vLook);
	HRESULT Set_UpOrtho(const _vec3& vUp);
	_float	Go_Straight(_float fTimeDelta);
	_float	Go_Up(_float fTimeDelta);
	_float	Go_Right(_float fTimeDelta);
	_float	Go_Left(_float fTimeDelta);
	_float	Go_Dir(_float fTimeDelta, _vec3 vDir);
	HRESULT Rotation_Axis(const _vec3& vAxis, _float fTimeDelta);
	HRESULT SetUp_Rotation(_vec3 vAxis, _float fDegree);
	HRESULT Set_RotateSpeed(_float fRotateSpeed);
	HRESULT Set_Speed(_float fSpeed);
	virtual void	Add_Pos(_vec3 vPos);
	void Sub_Pos(_vec3 vPos);
	//포물선 운동 x,y, 내부에서 셋까지 해줌
	void	Move_Projectile(_vec3 vStartPos, _vec3 vProjectilePower, _float fProjectileTime, _float fDegree);
	//인자 - 시작 지점, 타겟 지점, 타겟까지 도달 시간, 던지는 각도 // 리턴 - 힘
	_vec3	Get_ProjectilePowerToTarget(_vec3 vStartPos, _vec3 vTargetPos, _float fProjectileMaxTime, _float fDegree);
	HRESULT Set_BoxColliderTransform(CBoxCollider* pBoxCollider);

	//Frame
	void	Set_FrameValue(_uint iStart) { m_tFrame.iStart = iStart; };

	//Render
	void	Set_AlphaRef(_uint iAlphaRef) { m_iAlphaRef = iAlphaRef; }

	//BoxCollider
	HRESULT	Tick_Collider(_float fTimeDelta, CBoxCollider* pBoxCollider);

public: //GET
	_vec3	Get_TransformState(CTransform::STATE eTransformState);
	_vec3	Get_TransformScale();
	const _bool& Get_Blend() const { return m_bBlend; }

protected:
	virtual HRESULT Set_RenderState();
	virtual HRESULT Release_RenderState();

private:
	HRESULT SetUp_Component(void* pArg);

private:
	CTransform*		m_pTransform = nullptr;
	CTexture*		m_pTexture = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CVIBuffer*		m_pVIBuffer = nullptr;

private:
	FRAME					m_tFrame;
	_int					m_iAlphaRef = 150;
	_bool					m_bBlend;
	_mat					m_matProject;

public:
	virtual CGameObject* Clone(void* pArg = nullptr)override;
	static CTipObject* Create(_pDevice pDevice, DESC* tDesc);
	virtual void Free() override;

	// CGameObject을(를) 통해 상속됨
	virtual _vec3 Get_MinPos()override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};
END
#endif // !__CLIENT_CUBE_OBJECT_H__
