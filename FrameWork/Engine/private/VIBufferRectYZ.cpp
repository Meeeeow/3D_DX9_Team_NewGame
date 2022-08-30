#include "..\public\VIBufferRectYZ.h"



CVIBufferRectYZ::CVIBufferRectYZ(_pDevice pGraphicDevice)
	: CVIBuffer(pGraphicDevice)
{
}

CVIBufferRectYZ::CVIBufferRectYZ(const CVIBufferRectYZ & rhs)
	: CVIBuffer(rhs)
	, m_iNumVerticesY(rhs.m_iNumVerticesY)
	, m_iNumVerticesZ(rhs.m_iNumVerticesZ)
	, m_bRight(rhs.m_bRight)
{
}

HRESULT CVIBufferRectYZ::NativeConstruct_Prototype()
{
	if(FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CVIBufferRectYZ NativeConstruct_Prototype - __super");

	m_iNumVerticesY = 2;
	m_iNumVerticesZ = 2;
	m_iNumVertices = 4;
	m_iStride = sizeof(VTXTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	if (FAILED(CreateVertexBuffer()))
		MSG_FAIL("CVIBufferRectYZ NativeConstruct_Prototype - CreateVertexBuffer");

	VTXTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPos = _vec3(0.f, 0.5f, -0.5f);
	pVertices[0].vTexUV = _vec2(0.f, 0.f);

	pVertices[1].vPos = _vec3(0.f, 0.5f, 0.5f);
	pVertices[1].vTexUV = _vec2(1.f, 0.f);

	pVertices[2].vPos = _vec3(0.f, -0.5f, 0.5f);
	pVertices[2].vTexUV = _vec2(1.f, 1.f);

	pVertices[3].vPos = _vec3(0.f, -0.5f, -0.5f);
	pVertices[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	m_iNumPrimitive = 2;
	m_iIndicesSize = sizeof(INDICES_TRI16);
	m_eFormat = D3DFORMAT::D3DFMT_INDEX16;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;

	if(FAILED(CreateIndexBuffer()))
		MSG_FAIL("CVIBufferRectYZ NativeConstruct_Prototype - CreateIndexBuffer");


	INDICES_TRI16* pIndices = nullptr;
	m_pIB->Lock(0, 0, (void**)(&pIndices), 0);

	pIndices[0]._1 = 0;
	pIndices[0]._2 = 1;
	pIndices[0]._3 = 2;

	pIndices[1]._1 = 0;
	pIndices[1]._2 = 2;
	pIndices[1]._3 = 3;

	m_pIB->Unlock();


	if(FAILED(Create_VertexPosArr()))
		MSG_FAIL("CVIBufferRectYZ NativeConstruct_Prototype - Create_VertexPosArr");

	m_bRight = true;

	return S_OK;
}

HRESULT CVIBufferRectYZ::NativeConstruct_Prototype(WALLSIDE_YZ eSide, _float fIntervalY, _float fIntervalZ, _float fMultipleU, _float fMultipleV, _uint iNumVerticesY, _uint iNumVerticesZ)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CVIBufferRectYZ NativeConstruct_Prototype - __super");


	m_iNumVerticesY = iNumVerticesY;
	m_iNumVerticesZ = iNumVerticesZ;
	m_iNumVertices	= m_iNumVerticesY * m_iNumVerticesZ;

	m_iStride = sizeof(VTXTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	if (FAILED(CreateVertexBuffer()))
		MSG_FAIL("CVIBufferRectYZ NativeConstruct_Prototype - CreateVertexBuffer, 2) ");

	VTXTEX*	pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);
	for (_uint i = 0; i < m_iNumVerticesY; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesZ; ++j)
		{
			_uint iIndex = i * m_iNumVerticesZ + j;

			pVertices[iIndex].vPos = _vec3(0, fIntervalY * i, fIntervalZ * j);
			pVertices[iIndex].vTexUV = _vec2((j / (_float)iNumVerticesZ - 1) * fMultipleU,
				(i / (_float)iNumVerticesY - 1) * fMultipleV);
		}
	}
	m_pVB->Unlock();

	m_iNumPrimitive = (iNumVerticesY - 1) * (iNumVerticesZ - 1) * 2;
	m_iIndicesSize = sizeof(INDICES_TRI32);
	m_eFormat = D3DFMT_INDEX32;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;

	if (FAILED(CreateIndexBuffer()))
		MSG_FAIL("CVIBufferRectYZ NativeConstruct_Prototype - CreateIndexBuffer, 2) ");

	INDICES_TRI32* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint iIndicesIndex = 0;

	if (eSide == WALLSIDE_YZ::WS_RIGHT)
	{
		m_bRight = true;
		for (_uint i = 0; i < iNumVerticesY - 1; ++i)
		{
			for (_uint j = 0; j < iNumVerticesZ - 1; ++j)
			{
				_uint iIndex = i * iNumVerticesZ + j;

				_uint iIndices[4] = { iIndex + iNumVerticesZ,
					iIndex + iNumVerticesZ + 1,
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
	else if (eSide == WALLSIDE_YZ::WS_LEFT)
	{
		m_bRight = false;
		for (_uint i = 0; i < iNumVerticesY - 1; ++i)
		{
			for (_uint j = 0; j < iNumVerticesZ - 1; ++j)
			{
				_uint iIndex = i * iNumVerticesZ + j;

				_uint iIndices[4] = { iIndex + iNumVerticesZ,
					iIndex + iNumVerticesZ + 1,
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
		MSG_FAIL("CVIBufferRectYZ NativeConstruct_Prototype - WALLSIDE_END, 2) ");

	m_pIB->Unlock();

	if (FAILED(Create_VertexPosArr()))
		MSG_FAIL("CVIBufferRectYZ NativeConstruct_Prototype - Create_VertexPosArr, 2) ");

	return S_OK;
}

HRESULT CVIBufferRectYZ::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CVIBufferRectYZ NativeConstruct - __super");

	return S_OK;
}

_bool CVIBufferRectYZ::Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, _vec3 * pOut)
{
	_uint iIndicesIndex = 0;
	INDICES_TRI32* pIndices = new INDICES_TRI32[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVerticesY - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumVerticesZ - 1; ++j)
		{
			_uint iIndex = i * m_iNumVerticesZ + j;

			if (iIndicesIndex > 1000)
				int a = 0;

			_uint iIndices[4] = { iIndex + m_iNumVerticesZ,
				iIndex + m_iNumVerticesZ + 1,
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

CVIBufferRectYZ * CVIBufferRectYZ::Create(_pDevice pGraphicDevice)
{
	CVIBufferRectYZ* pInstance = new CVIBufferRectYZ(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CVIBufferRectYZ Create Failed - 1) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CVIBufferRectYZ * CVIBufferRectYZ::Create(_pDevice pGraphicDevice, WALLSIDE_YZ eSide, _float fIntervalY, _float fIntervalZ, _float fMultipleU, _float fMultipleV, _uint iNumVerticesY, _uint iNumVerticesZ)
{
	CVIBufferRectYZ* pInstance = new CVIBufferRectYZ(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype(eSide, fIntervalY,fIntervalZ,fMultipleU,fMultipleV, iNumVerticesY, iNumVerticesZ)))
	{
		MSGBOX("CVIBufferRectYZ Create Failed - 4) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBufferRectYZ::Free()
{
	__super::Free();
}

CComponent * CVIBufferRectYZ::Clone(void * pArg)
{
	CVIBufferRectYZ* pInstance = new CVIBufferRectYZ(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CVIBufferRectYZ Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}
