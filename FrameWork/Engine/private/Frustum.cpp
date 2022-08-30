#include "..\public\Frustum.h"
#include "BoxCollider.h"
#include "Transform.h"

CFrustum::CFrustum(_pDevice pGraphicDevice)
	: CComponent(pGraphicDevice)
{
}


HRESULT CFrustum::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CFrustum NativeConstruct_Prototype - __super");


	return S_OK;
}

HRESULT CFrustum::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CFrustum NativeConstruct - __super");


	return S_OK;
}

HRESULT CFrustum::Make_Frustum()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CFrustum Make_Frustum - GraphicDevice is nullptr");

	_mat matProj;
	_mat matInvProj;
	m_pGraphicDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matInvProj, nullptr, &matProj);

	_mat matView;
	_mat matInvView;
	m_pGraphicDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInvView, nullptr, &matView);

	_mat matInverse = matInvProj * matInvView;
	
	m_vPoint[0] = { -1.f ,  1.f ,  0.f };
	m_vPoint[1] = { 1.f  ,  1.f ,  0.f };
	m_vPoint[2] = { 1.f  , -1.f ,  0.f };
	m_vPoint[3] = { -1.f , -1.f ,  0.f };
	m_vPoint[4] = { -1.f ,  1.f ,  1.f };
	m_vPoint[5] = { 1.f  ,  1.f ,  1.f };
	m_vPoint[6] = { 1.f  , -1.f ,  1.f };
	m_vPoint[7] = { -1.f , -1.f ,  1.f };

	for (int i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matInverse);
	// 안에서 밖을 바라보는 평면
	D3DXPlaneFromPoints(&m_vPlane[0], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]); // Near
	D3DXPlaneFromPoints(&m_vPlane[1], &m_vPoint[5], &m_vPoint[4], &m_vPoint[7]); // Far
	D3DXPlaneFromPoints(&m_vPlane[2], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]); // Left
	D3DXPlaneFromPoints(&m_vPlane[3], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]); // Right
	D3DXPlaneFromPoints(&m_vPlane[4], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]); // Top
	D3DXPlaneFromPoints(&m_vPlane[5], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]); // Bottom

	return S_OK;
}

_bool CFrustum::Check_Culling(CTransform * pTransform, _float fRadius)
{
	if (FAILED(Make_Frustum()))
		MSG_FALSE("CFrustum Check_Culling - Make_Frustum CTransform");
	if (pTransform == nullptr)
		MSG_FALSE("CFrustum Check_Culling - CTransform is nullptr");

	_float fDist = 0.f;
	
	for (int i = 0; i < 6; ++i)
	{
		_vec3 vTransformPos = pTransform->Get_State(CTransform::STATE_POS);
		fDist = D3DXPlaneDotCoord(&m_vPlane[i], &vTransformPos);
		if (fDist > fRadius)
			return false;
	}
	return true;
}

_bool CFrustum::Check_Culling(class CTransform* pTransform, class CBoxCollider* pCollider)
{
	if (FAILED(Make_Frustum()))
		MSG_FALSE("CFrustum Check_Culling - Make_Frustum CBoxCollider");
	if (pCollider == nullptr ||
		pTransform == nullptr)
		MSG_FALSE("CFrustum Check_Culling - CBoxCollider or CTransform is nullptr");

	_float	fDist;
	_uint	iTrueCnt = 0;
	for (int i = 0; i < 8; ++i)
	{
		iTrueCnt = 0;
		for (int j = 0; j < 6; ++j)
		{
			_vec3 vLocal = pCollider->Get_VerticesPosArr(i);
			D3DXVec3TransformCoord(&vLocal, &vLocal, pTransform->Get_WorldMatrixPtr());
			fDist = D3DXPlaneDotCoord(&m_vPlane[j], &vLocal);
			if (fDist < 0)
				++iTrueCnt;

			if (iTrueCnt == 6)
				return true;
		}
	}
	return false;
}

_bool CFrustum::Check_Culling(CVIBuffer * pBuffer)
{
	if (FAILED(Make_Frustum()))
		MSG_FALSE("CFrustum Check_Culling - CVIBuffer");

	return false;
}

void CFrustum::Free()
{
	__super::Free();
}

CComponent * CFrustum::Clone(void * pArg)
{
	AddRef();
	return this;
}

CFrustum * CFrustum::Create(_pDevice pGraphicDevice)
{
	CFrustum* pInstance = new CFrustum(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CFrustum Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}
