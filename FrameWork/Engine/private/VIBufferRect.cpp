#include "..\public\VIBufferRect.h"
#include "Graphic_Device.h"

CVIBufferRect::CVIBufferRect(_pDevice pGraphicDevice)
	:CVIBuffer(pGraphicDevice)
{
}

CVIBufferRect::CVIBufferRect(const CVIBufferRect & rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CVIBufferRect::NativeConstruct_Prototype(void* pArg)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CVIBufferRect NativeConstruct_Prototype - __super");

	m_iNumVertices = 4;
	m_iStride = sizeof(VTXTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	if (FAILED(__super::CreateVertexBuffer()))
		MSG_FAIL("CVIBufferRect NativeConstruct_Prototype - __super");

	VTXTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	if (nullptr == pArg)
	{
		pVertices[0].vPos = _vec3(-0.5f, 0.5f, 0.f);
		pVertices[0].vTexUV = _vec2(0.f, 0.f);

		pVertices[1].vPos = _vec3(0.5f, 0.5f, 0.f);
		pVertices[1].vTexUV = _vec2(1.f, 0.f);

		pVertices[2].vPos = _vec3(0.5f, -0.5f, 0.f);
		pVertices[2].vTexUV = _vec2(1.f, 1.f);

		pVertices[3].vPos = _vec3(-0.5f, -0.5f, 0.f);
		pVertices[3].vTexUV = _vec2(0.f, 1.f);
	}
	else
	{
		SIZE_POS tSizePos;
		tSizePos = *(SIZE_POS*)(pArg);

		_float fSizeX = tSizePos.vSize.x * 0.5f;
		_float fSizeY = tSizePos.vSize.y * 0.5f;
		_float fPosX = tSizePos.vPos.x;
		_float fPosY = tSizePos.vPos.y;
		_float fPosZ = tSizePos.vPos.z;

		m_vColliderCenter = _vec3(fPosX, fPosY, fPosZ);

		pVertices[0].vPos = _vec3(-fSizeX + fPosX, fSizeY + fPosY, fPosZ);
		pVertices[0].vTexUV = _vec2(0.f, 0.f);

		pVertices[1].vPos = _vec3(fSizeX + fPosX, fSizeY + fPosY, fPosZ);
		pVertices[1].vTexUV = _vec2(1.f, 0.f);

		pVertices[2].vPos = _vec3(fSizeX + fPosX, -fSizeY + fPosY, fPosZ);
		pVertices[2].vTexUV = _vec2(1.f, 1.f);

		pVertices[3].vPos = _vec3(-fSizeX + fPosX, -fSizeY + fPosY, fPosZ);
		pVertices[3].vTexUV = _vec2(0.f, 1.f);
	}

	m_pVB->Unlock();

	m_iNumPrimitive = 2;
	m_iIndicesSize = sizeof(INDICES_TRI16);
	m_eFormat = D3DFORMAT::D3DFMT_INDEX16;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;

	CreateIndexBuffer();

	INDICES_TRI16* pIndices = nullptr;
	m_pIB->Lock(0, 0, (void**)(&pIndices), 0);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	m_pIB->Unlock();

	if (FAILED(__super::Create_VertexPosArr()))
		MSG_FAIL("CVIBufferRect::NativeConstruct_Prototype - __super::Create_VertexPosArr");

	return S_OK;
}

HRESULT CVIBufferRect::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CVIBufferRect NativeConstruct - __super");

	return S_OK;
}

_bool CVIBufferRect::Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, _vec3 * pOut)
{
	INDICES_TRI16 pIndices[2];

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	_float fU, fV, fDist, fDistMin = -1.f;

	for (auto& index : pIndices)
	{
		if (D3DXIntersectTri(&m_pVerticesPosArr[index._1],
			&m_pVerticesPosArr[index._2], &m_pVerticesPosArr[index._3], &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
		{
			if (fDistMin < 0.f || fDistMin > fDist)
			{
				fDistMin = fDist;
				*pOut = vMousePivot + vMouseRay * fDist + m_vColliderCenter;
			}
		}
	}

	if (fDistMin < 0.f)
		return false;
	else
		return true;
}

HRESULT CVIBufferRect::Decrease_Top(_float fRatio)
{
	if (m_pVB == nullptr)
		MSG_FAIL("CVIBufferRect::Decrease_Top - m_pVB == nullptr");

	VTXTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPos.y = 0.5f - fRatio;
	pVertices[0].vTexUV.y = fRatio;

	pVertices[1].vPos.y = 0.5f - fRatio;
	pVertices[1].vTexUV.y = fRatio;

	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBufferRect::Reset()
{
	if (m_pVB == nullptr)
		MSG_FAIL("CVIBufferRect::Decrease_Top - m_pVB == nullptr");

	VTXTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPos.y = m_pVerticesPosArr[0].y;
	pVertices[0].vTexUV.y = 0.f;

	pVertices[1].vPos.y = m_pVerticesPosArr[1].y;
	pVertices[1].vTexUV.y = 0.f;

	m_pVB->Unlock();

	return S_OK;
}

void CVIBufferRect::Free()
{
	__super::Free();
}

CComponent * CVIBufferRect::Clone(void * pArg)
{
	CComponent* pInstance = new CVIBufferRect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CVIBufferRect Clone Failed");
	}

	return pInstance;
}

CVIBufferRect * CVIBufferRect::Create(_pDevice pGraphicDevice, void* pArg)
{
	CVIBufferRect* pInstance = new CVIBufferRect(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CVIBufferRect Create Failed");
	}


	return pInstance;
}
