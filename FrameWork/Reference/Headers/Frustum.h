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

	// 컬링을 수행하기 위해 절도체를 생성하는 과정입니다. 반복해서 이루어져야 합니다.
	HRESULT			Make_Frustum();

	/* 	Transform의 WorldMatrix의 Position + 가상의 구의 반지름으로 계산합니다.
		Radius 값은 Object 내부에서 정해주는게 좋습니다.*/ //true일때 렌더
	_bool			Check_Culling(class CTransform* pTransform, _float fRadius);

	// Collider의 각 정점을 기준으로 계산합니다.
	_bool			Check_Culling(class CTransform* pTransform, class CBoxCollider* pCollider);

	// 추천하지 않습니다.
	_bool			Check_Culling(class CVIBuffer* pBuffer);
private:
	_vec3	m_vPoint[8]; // 투영 공간의 점 8개
	_plane	m_vPlane[6]; // 투영 공간의 점에 Inv Proj * Inv View하여 만든 6개 평면

public:
	virtual void		Free() override;
	virtual CComponent* Clone(void * pArg) override;
	static	CFrustum*	Create(_pDevice pGraphicDevice);
};

END
#endif // !__ENGINE_FRUSTUM_H__
