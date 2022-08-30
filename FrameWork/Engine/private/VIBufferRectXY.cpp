#include "..\public\VIBufferRectXY.h"

CVIBufferRectXY::CVIBufferRectXY(_pDevice pGraphicDevice)
	: CVIBuffer(pGraphicDevice)
{

}

CVIBufferRectXY::CVIBufferRectXY(const CVIBufferRectXY & rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesX(rhs.m_iNumVerticesX)
	, m_iNumVerticesY(rhs.m_iNumVerticesY)
{

}

HRESULT CVIBufferRectXY::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CVIBufferRectYZ NativeConstruct_Prototype - __super");

	m_iNumVerticesX = 2;
	m_iNumVerticesY = 2;
	m_iNumVertices = 4;
	m_iStride = sizeof(VTXTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	if (FAILED(CreateVertexBuffer()))
		MSG_FAIL("CVIBufferRectYZ NativeConstruct_Prototype - CreateVertexBuffer");

	VTXTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPos = _vec3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexUV = _vec2(0.f, 0.f);

	pVertices[1].vPos = _vec3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexUV = _vec2(1.f, 0.f);

	pVertices[2].vPos = _vec3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexUV = _vec2(1.f, 1.f);

	pVertices[3].vPos = _vec3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	m_iNumPrimitive = 2;
	m_iIndicesSize = sizeof(INDICES_TRI16);
	m_eFormat = D3DFORMAT::D3DFMT_INDEX16;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;

	if (FAILED(CreateIndexBuffer()))
		MSG_FAIL("CVIBufferRectXY NativeConstruct_Prototype - CreateIndexBuffer");


	INDICES_TRI16* pIndices = nullptr;
	m_pIB->Lock(0, 0, (void**)(&pIndices), 0);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	m_pIB->Unlock();

	if (FAILED(Create_VertexPosArr()))
		MSG_FAIL("CVIBufferRectXY NativeConstruct_Prototype - Create_VertexPosArr");


	m_bFront = true;

	return S_OK;
}

HRESULT CVIBufferRectXY::NativeConstruct_Prototype(WALLLOOK_XY eSide, _float fIntervalX, _float fIntervalY, _float fMultipleU, _float fMultipleV, _uint iNumVerticesX, _uint iNumVerticesY)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CVIBufferRectYZ NativeConstruct_Prototype - __super");


	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesY = iNumVerticesY;
	m_iNumVertices = m_iNumVerticesX * m_iNumVerticesY;

	m_iStride = sizeof(VTXTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	if (FAILED(CreateVertexBuffer()))
		MSG_FAIL("CVIBufferRectYZ NativeConstruct_Prototype - CreateVertexBuffer, 2) ");

	VTXTEX*	pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);
	for (_uint i = 0; i < iNumVerticesY; ++i)
	{
		for (_uint j = 0; j < iNumVerticesX; ++j)
		{
			_uint iIndex = i * iNumVerticesX + j;

			pVertices[iIndex].vPos = _vec3(fIntervalX * j, fIntervalY * i, 0);
			pVertices[iIndex].vTexUV = _vec2((j / (_float)iNumVerticesX - 1) * fMultipleU,
				(i / (_float)iNumVerticesY - 1) * fMultipleV);
		}
	}
	m_pVB->Unlock();

	m_iNumPrimitive = (iNumVerticesY - 1) * (iNumVerticesX - 1) * 2;
	m_iIndicesSize = sizeof(INDICES_TRI32);
	m_eFormat = D3DFMT_INDEX32;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;

	if (FAILED(CreateIndexBuffer()))
		MSG_FAIL("CVIBufferRectYZ NativeConstruct_Prototype - CreateIndexBuffer, 2) ");

	INDICES_TRI32* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint iIndicesIndex = 0;

	if (eSide == WALLLOOK_XY::WL_FRONT)
	{
		m_bFront = true;
		for (_uint i = 0; i < iNumVerticesY - 1; ++i)
		{
			for (_uint j = 0; j < iNumVerticesX - 1; ++j)
			{
				_uint iIndex = i * iNumVerticesX + j;

				_uint iIndices[4] = { iIndex + iNumVerticesX,
					iIndex + iNumVerticesX + 1,
					iIndex + 1,
					iIndex };

				pIndices[iIndicesIndex]._1 = iIndices[0];
				pIndices[iIndicesIndex]._2 = iIndices[1];
				pIndices[iIndicesIndex]._3 = iIndices[2];
				++iIndicesIndex;

				pIndices[iIndicesIndex]._1 = iIndices[0];
				pIndices[iIndicesIndex]._2 = iIndices[2];
				pIndices[iIndicesIndex]._3 = iIndices[3];
				++iIndicesIndex;
			}
		}
	}
	else if (eSide == WALLLOOK_XY::WL_BACK)
	{
		m_bFront = false;
		for (_uint i = 0; i < iNumVerticesY - 1; ++i)
		{
			for (_uint j = 0; j < iNumVerticesX - 1; ++j)
			{
				_uint iIndex = i * iNumVerticesX + j;

				_uint iIndices[4] = { iIndex + iNumVerticesX,
					iIndex + iNumVerticesX + 1,
					iIndex + 1,
					iIndex };

				pIndices[iIndicesIndex]._1 = iIndices[2];
				pIndices[iIndicesIndex]._2 = iIndices[1];
				pIndices[iIndicesIndex]._3 = iIndices[0];
				++iIndicesIndex;

				pIndices[iIndicesIndex]._1 = iIndices[3];
				pIndices[iIndicesIndex]._2 = iIndices[2];
				pIndices[iIndicesIndex]._3 = iIndices[0];
				++iIndicesIndex;
			}
		}
	}
	else
		MSG_FAIL("CVIBufferRectXY NativeConstruct_Prototype - WALLLOOK_END , 2) ");

	m_pIB->Unlock();

	if (FAILED(Create_VertexPosArr()))
		MSG_FAIL("CVIBufferRectXY NativeConstruct_Prototype - Create_VertexPosArr, 2) ");

	return S_OK;
}

HRESULT CVIBufferRectXY::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CVIBufferRectXY NativeConstruct - __super");

	return S_OK;
}

_bool CVIBufferRectXY::Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, _vec3 * pOut)
{
	_uint iIndicesIndex = 0;
	INDICES_TRI32* pIndices = new INDICES_TRI32[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVerticesY - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * m_iNumVerticesX + j;

			if (iIndicesIndex > 1000)
				int a = 0;

			_uint iIndices[4] = { iIndex + m_iNumVerticesX,
				iIndex + m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex };

			pIndices[iIndicesIndex]._1 = iIndices[0];
			pIndices[iIndicesIndex]._2 = iIndices[1];
			pIndices[iIndicesIndex]._3 = iIndices[2];
			++iIndicesIndex;

			pIndices[iIndicesIndex]._1 = iIndices[0];
			pIndices[iIndicesIndex]._2 = iIndices[2];
			pIndices[iIndicesIndex]._3 = iIndices[3];
			++iIndicesIndex;
		}
	}

	_float fU, fV, fDist, fDistMin = -1.f;
	for (_uint i = 0; i < iIndicesIndex; ++i)
	{
		if (D3DXIntersectTri(&m_pVerticesPosArr[pIndices[i]._1],
			&m_pVerticesPosArr[pIndices[i]._2], &m_pVerticesPosArr[pIndices[i]._3], &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
		{
			if (fDistMin < 0.f || fDistMin > fDist)
			{
				fDistMin = fDist;
				*pOut = vMousePivot + vMouseRay * fDist;
			}
		}
	}

	Safe_Delete_Arr(pIndices);

	if (fDistMin < 0.f)
		return false;
	else
		return true;
}

CVIBufferRectXY * CVIBufferRectXY::Create(_pDevice pGraphicDevice)
{
	CVIBufferRectXY* pInstance = new CVIBufferRectXY(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CVIBufferRectXY Create Failed - 1) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CVIBufferRectXY * CVIBufferRectXY::Create(_pDevice pGraphicDevice, WALLLOOK_XY eSide, _float fIntervalX, _float fIntervalY, _float fMultipleU, _float fMultipleV, _uint iNumVerticesX, _uint iNumVerticesY)
{
	CVIBufferRectXY* pInstance = new CVIBufferRectXY(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype(eSide, fIntervalX, fIntervalY, fMultipleU, fMultipleV, iNumVerticesX, iNumVerticesY)))
	{
		MSGBOX("CVIBufferRectXY Create Failed - 2) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBufferRectXY::Free()
{
	__super::Free();
}

CComponent * CVIBufferRectXY::Clone(void * pArg)
{
	CVIBufferRectXY* pInstance = new CVIBufferRectXY(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CVIBufferRectYZ Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}
