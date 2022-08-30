#include "..\public\VIBufferRectColor.h"
#include "Graphic_Device.h"

CVIBufferRectColor::CVIBufferRectColor(_pDevice pGraphicDevice)
	:CVIBuffer(pGraphicDevice)
{
}

CVIBufferRectColor::CVIBufferRectColor(const CVIBufferRectColor & rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CVIBufferRectColor::NativeConstruct_Prototype(void* pArg)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CVIBufferRectColor NativeConstruct_Prototype - __super");

	m_iNumVertices = 4;
	m_iStride = sizeof(VTXTEXCOL);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE;

	if (FAILED(__super::CreateVertexBuffer()))
		MSG_FAIL("CVIBufferRectColor NativeConstruct_Prototype - __super");

	VTXTEXCOL* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	if (nullptr == pArg)
	{
		pVertices[0].vPos = _vec3(-0.5f, 0.5f, 0.f);
		pVertices[0].vTexUV = _vec2(0.f, 0.f);
		pVertices[0].dwColor = 0xffffffff;

		pVertices[1].vPos = _vec3(0.5f, 0.5f, 0.f);
		pVertices[1].vTexUV = _vec2(1.f, 0.f);
		pVertices[1].dwColor = 0xffffffff;

		pVertices[2].vPos = _vec3(0.5f, -0.5f, 0.f);
		pVertices[2].vTexUV = _vec2(1.f, 1.f);
		pVertices[2].dwColor = 0xffffffff;

		pVertices[3].vPos = _vec3(-0.5f, -0.5f, 0.f);
		pVertices[3].vTexUV = _vec2(0.f, 1.f);
		pVertices[3].dwColor = 0xffffffff;
	}
	else
	{
		VTX_INFO tVtxInfo = *(VTX_INFO*)pArg;
		_float fSizeX = tVtxInfo.vSize.x * 0.5f;
		_float fSizeY = tVtxInfo.vSize.y * 0.5f;
		_ulong dwColor = tVtxInfo.dwColor;

		pVertices[0].vPos = _vec3(-fSizeX, fSizeY, 0.f) + tVtxInfo.vPos;
		pVertices[0].vTexUV = _vec2(0.f, 0.f);
		pVertices[0].dwColor = dwColor;

		pVertices[1].vPos = _vec3(fSizeX, fSizeY, 0.f) + tVtxInfo.vPos;
		pVertices[1].vTexUV = _vec2(1.f, 0.f);
		pVertices[1].dwColor = dwColor;

		pVertices[2].vPos = _vec3(fSizeX, -fSizeY, 0.f) + tVtxInfo.vPos;
		pVertices[2].vTexUV = _vec2(1.f, 1.f);
		pVertices[2].dwColor = dwColor;

		pVertices[3].vPos = _vec3(-fSizeX, -fSizeY, 0.f) + tVtxInfo.vPos;
		pVertices[3].vTexUV = _vec2(0.f, 1.f);
		pVertices[3].dwColor = dwColor;
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
		MSG_FAIL("CVIBufferRectColor::NativeConstruct_Prototype - __super::Create_VertexPosArr");

	return S_OK;
}

HRESULT CVIBufferRectColor::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CVIBufferRectColor NativeConstruct - __super");

	return S_OK;
}

HRESULT CVIBufferRectColor::Change_Color(D3DXCOLOR dwColor)
{
	VTXTEXCOL* pVertices = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].dwColor = dwColor;
	pVertices[1].dwColor = dwColor;
	pVertices[2].dwColor = dwColor;
	pVertices[3].dwColor = dwColor;

	m_pVB->Unlock();

	return S_OK;
}

_bool CVIBufferRectColor::Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, _vec3 * pOut)
{
	INDICES_TRI16 pIndices[2];

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	_float fV, fU, fDist, fDistMin = -1.f;

	for (auto& index : pIndices)
	{
		if (D3DXIntersectTri(&m_pVerticesPosArr[index._1],
			&m_pVerticesPosArr[index._2], &m_pVerticesPosArr[index._3], &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
		{
			if (fDistMin < 0.f || fDistMin > fDist)
			{
				fDistMin = fDist;
				*pOut = vMousePivot + vMouseRay * fDist;
			}
		}
	}

	if (fDistMin < 0.f)
		return false;
	else
		return true;
}

void CVIBufferRectColor::Free()
{
	__super::Free();
}

CComponent * CVIBufferRectColor::Clone(void * pArg)
{
	CComponent* pInstance = new CVIBufferRectColor(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CVIBufferRectColor Clone Failed");
	}

	return pInstance;
}

CVIBufferRectColor * CVIBufferRectColor::Create(_pDevice pGraphicDevice, void* pArg)
{
	CVIBufferRectColor* pInstance = new CVIBufferRectColor(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CVIBufferRectColor Create Failed");
	}


	return pInstance;
}
