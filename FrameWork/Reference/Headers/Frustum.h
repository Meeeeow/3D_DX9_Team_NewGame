#pragma once
#ifndef __ENGINE_FRUSTUM_H__
#define __ENGINE_FRUSTUM_H__

#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CFrustum final : public CComponent
{
public:
	explicit CFrustum(_pDevice pGraphicDevice);
	virtual ~CFrustum() DEFAULT;


public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

	// �ø��� �����ϱ� ���� ����ü�� �����ϴ� �����Դϴ�. �ݺ��ؼ� �̷������ �մϴ�.
	HRESULT			Make_Frustum();

	/* 	Transform�� WorldMatrix�� Position + ������ ���� ���������� ����մϴ�.
		Radius ���� Object ���ο��� �����ִ°� �����ϴ�.*/ //true�϶� ����
	_bool			Check_Culling(class CTransform* pTransform, _float fRadius);

	// Collider�� �� ������ �������� ����մϴ�.
	_bool			Check_Culling(class CTransform* pTransform, class CBoxCollider* pCollider);

	// ��õ���� �ʽ��ϴ�.
	_bool			Check_Culling(class CVIBuffer* pBuffer);
private:
	_vec3	m_vPoint[8]; // ���� ������ �� 8��
	_plane	m_vPlane[6]; // ���� ������ ���� Inv Proj * Inv View�Ͽ� ���� 6�� ���

public:
	virtual void		Free() override;
	virtual CComponent* Clone(void * pArg) override;
	static	CFrustum*	Create(_pDevice pGraphicDevice);
};

END
#endif // !__ENGINE_FRUSTUM_H__
