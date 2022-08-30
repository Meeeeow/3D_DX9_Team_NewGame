#include "..\public\VIBufferTerrain.h"

CVIBufferTerrain::CVIBufferTerrain(_pDevice pDevice)
	:CVIBuffer(pDevice)
{
}

CVIBufferTerrain::CVIBufferTerrain(const CVIBufferTerrain & rhs)
	: CVIBuffer(rhs), m_iNumVerticesX(rhs.m_iNumVerticesX), m_iNumVerticesZ(rhs.m_iNumVerticesZ)
{
}

HRESULT CVIBufferTerrain::NativeConstruct_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ, _float fSizeX /*= 1.f*/, _float fSizeZ /*= 1.f*/, _float fU /*= 1.f*/, _float fV /*= 1.f*/)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CVIBufferTerrain NativeConstruct_Prototype - __super");



	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;
	m_iNumVertices = iNumVerticesX * iNumVerticesZ;
	m_iStride = sizeof(VTXTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	if (FAILED(CreateVertexBuffer()))
		MSG_FAIL("CVIBufferTerrain NativeConstruct_Prototype - CreateVertexBuffer");

	VTXTEX* pVirtices = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVirtices, 0);

	for (_uint i = 0; i < iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < iNumVerticesX; ++j)
		{
			int iIndex = i * iNumVerticesX + j;
			float fPosX = j * fSizeX;
			float fPosZ = i * fSizeZ;
			pVirtices[iIndex].vPos = _vec3((float)fPosX, 0.f, (float)fPosZ);
			pVirtices[iIndex].vTexUV = _vec2((float)fPosX / (float)(iNumVerticesX - 1) * fU, (float)fPosZ / (float)(iNumVerticesZ - 1) * fV);
		}
	}

	m_pVB->Unlock();

	m_iNumPrimitive = (iNumVerticesX - 1) * 2 * (iNumVerticesZ - 1);
	m_iIndicesSize = sizeof(INDICES_TRI32);
	m_eFormat = D3DFORMAT::D3DFMT_INDEX32;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;

	if (FAILED(CreateIndexBuffer()))
		MSG_FAIL("CVIBufferTerrain NativeConstruct_Prototype - CreateIndexBuffer");

	INDICES_TRI32* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint iIndex = 0;
	for (_uint i = 0; i < iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < iNumVerticesX - 1; ++j)
		{
			//x°¡ Â¦¼ö
 			pIndices[iIndex]._1 = (iNumVerticesX * (i+1)) + j;
			pIndices[iIndex]._2 = (iNumVerticesX * i) + j + 1;
			pIndices[iIndex]._3 = (iNumVerticesX * i) + j;
			iIndex++;

			//x°¡ È¦¼ö
			pIndices[iIndex]._1 = pIndices[iIndex-1]._1;	
			pIndices[iIndex]._2 = (iNumVerticesX * (i + 1)) + j + 1;
			pIndices[iIndex]._3 = pIndices[iIndex - 1]._2;
			iIndex++;
			int i = 0;
		}
	}

	m_pIB->Unlock();

	if (FAILED(Create_VertexPosArr()))
		MSG_FAIL("CVIBufferTerrain NativeConstruct_Prototype - Create_VertexPosArr");
	
	return S_OK;
}

HRESULT CVIBufferTerrain::NativeConstruct_Prototype(const _tchar * pHeightMapPath)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CVIBufferTerrain NativeConstruct_Prototype - __super");

	HANDLE hFile;
	
	hFile = CreateFile(pHeightMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == 0)
		MSG_FAIL("CVIBufferTerrain NativeConstruct_Prototype - hFile is null");

	BITMAPFILEHEADER fh;
	_ulong dwByte;
	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);

	BITMAPINFOHEADER ih;
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);
	
	_uint iNumVerticesX = m_iNumVerticesX = ih.biWidth;
	_uint iNumVerticesZ = m_iNumVerticesZ = ih.biHeight;

	_ulong*		pPixel = new _ulong[iNumVerticesX * iNumVerticesZ];
	ReadFile(hFile, pPixel, sizeof(_ulong) * iNumVerticesX * iNumVerticesZ, &dwByte, nullptr);

	CloseHandle(hFile);

	m_iNumVertices = iNumVerticesX * iNumVerticesZ;

	if (m_iNumVertices > 10000000)
		MSG_FAIL("CVIBufferTerrain::NativeConstruct_Prototype - m_iNumVertices > 10000000");

	m_iStride = sizeof(VTXTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	if (FAILED(CreateVertexBuffer()))
		MSG_FAIL("CVIBufferTerrain NativeConstruct_Prototype - CreateVertexBuffer");

	VTXTEX* pVirtices = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVirtices, 0);

	for (_uint i = 0; i < iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < iNumVerticesX; ++j)
		{
			int iIndex = i * iNumVerticesX + j;
			pVirtices[iIndex].vPos = _vec3((float)j, (pPixel[iIndex] & 0x000000ff) / 15.f, (float)i);
			pVirtices[iIndex].vTexUV = _vec2((float)j / (float)(iNumVerticesX - 1) * 10.f, (float)i / (float)(iNumVerticesZ - 1) * 10.f);
		}
	}

	m_pVB->Unlock();

	m_iNumPrimitive = (iNumVerticesX - 1) * 2 * (iNumVerticesZ - 1);
	m_iIndicesSize = sizeof(INDICES_TRI32);
	m_eFormat = D3DFORMAT::D3DFMT_INDEX32;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;

	if (FAILED(CreateIndexBuffer()))
		MSG_FAIL("CVIBufferTerrain NativeConstruct_Prototype - CreateIndexBuffer");

	INDICES_TRI32* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint iIndex = 0;
	for (_uint i = 0; i < iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < iNumVerticesX - 1; ++j)
		{
			//x°¡ Â¦¼ö
			pIndices[iIndex]._1 = (iNumVerticesX * (i + 1)) + j;
			pIndices[iIndex]._2 = (iNumVerticesX * i) + j + 1;
			pIndices[iIndex]._3 = (iNumVerticesX * i) + j;
			iIndex++;

			//x°¡ È¦¼ö
			pIndices[iIndex]._1 = pIndices[iIndex - 1]._1;
			pIndices[iIndex]._2 = (iNumVerticesX * (i + 1)) + j + 1;
			pIndices[iIndex]._3 = pIndices[iIndex - 1]._2;
			iIndex++;
			int i = 0;
		}
	}

	m_pIB->Unlock();

	Safe_Delete_Arr(pPixel);

	if (FAILED(Create_VertexPosArr()))
		MSG_FAIL("CVIBufferTerrain NativeConstruct_Prototype - Create_VertexPosArr");

	return S_OK;
}

HRESULT CVIBufferTerrain::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CVIBufferTerrain NativeConstruct_Prototype - __super");



	m_iNumVertices = 4;
	m_iStride = sizeof(VTXTEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	if (FAILED(CreateVertexBuffer()))
		MSG_FAIL("CVIBufferTerrain NativeConstruct_Prototype - CreateVertexBuffer");

	VTXTEX* pVertices = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPos = _vec3(-0.5f, 0.f, 0.5f);
	pVertices[0].vTexUV = _vec2(0.f, 0.f);

	pVertices[1].vPos = _vec3(0.5f, 0.f, 0.5f);
	pVertices[1].vTexUV = _vec2(1.f, 0.f);

	pVertices[2].vPos = _vec3(0.5f, 0.f, -0.5f);
	pVertices[2].vTexUV = _vec2(1.f, 1.f);

	pVertices[3].vPos = _vec3(-0.5f, 0.f, -0.5f);
	pVertices[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	m_iNumPrimitive = 2;
	m_iIndicesSize = sizeof(INDICES_TRI32);
	m_eFormat = D3DFORMAT::D3DFMT_INDEX32;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;

	if (FAILED(CreateIndexBuffer()))
		MSG_FAIL("CVIBufferTerrain NativeConstruct_Prototype - CreateIndexBuffer");

	INDICES_TRI32* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._1 = 0; pIndices[0]._2 = 1; pIndices[0]._3 = 2;
	pIndices[1]._1 = 0; pIndices[1]._2 = 2; pIndices[1]._3 = 3;

	m_pIB->Unlock();

	if (FAILED(Create_VertexPosArr()))
		MSG_FAIL("CVIBufferTerrain NativeConstruct_Prototype - Create_VertexPosArr");

	return S_OK;
}

HRESULT CVIBufferTerrain::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CVIBufferTerrain NativeConstruct - __super");

	return S_OK;
}

_bool CVIBufferTerrain::Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, _vec3 * pOut)
{
	_float fU = 0.f, fV = 0.f, fDist = 0.f;

	for (int i = 0; i < m_iNumVerticesZ - 1; ++i)
	{
		for (int j = 0; j < m_iNumVerticesX - 1; ++j)
		{
			int iIndex = j + i* m_iNumVerticesX;

			_vec3 vIndices[4] = {
				m_pVerticesPosArr[iIndex + m_iNumVerticesX],
				m_pVerticesPosArr[iIndex + m_iNumVerticesX + 1],
				m_pVerticesPosArr[iIndex + 1],
				m_pVerticesPosArr[iIndex]
			};

			if (D3DXIntersectTri(&vIndices[0], &vIndices[1], &vIndices[2], &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
			{
				*pOut = vMousePivot + vMouseRay * fDist;
				return true;
			}

			if (D3DXIntersectTri(&vIndices[0], &vIndices[2], &vIndices[3], &vMousePivot, &vMouseRay, &fU, &fV, &fDist))
			{
				*pOut = vMousePivot + vMouseRay * fDist;
				return true;
			}
		}
	}

	return false;
}

_float CVIBufferTerrain::Compute_Height(_vec3 vLocalPos)
{
	_uint iIndex = _uint(vLocalPos.x) + _uint(vLocalPos.z) * m_iNumVerticesX;
	if (iIndex >= m_iNumVertices)
		return 1.f;

	_vec3 vIndices[4] = {
		m_pVerticesPosArr[iIndex + m_iNumVerticesX],
		m_pVerticesPosArr[iIndex + m_iNumVerticesX + 1],
		m_pVerticesPosArr[iIndex + 1],
		m_pVerticesPosArr[iIndex]
	};

	_float fDistanceX = vLocalPos.x - vIndices[0].x;
	_float fDistanceZ = vIndices[0].z - vLocalPos.z;
	_plane planeWorld;

	//¿ìÃø »ó´ÜÀÌ´Ù
	if (fDistanceX > fDistanceZ)
		D3DXPlaneFromPoints(&planeWorld, &vIndices[0], &vIndices[1], &vIndices[2]);
	//ÁÂÃø ÇÏ´ÜÀÌ´Ù
	else
		D3DXPlaneFromPoints(&planeWorld, &vIndices[0], &vIndices[2], &vIndices[3]);

	_float fY = (-planeWorld.a * vLocalPos.x - planeWorld.c * vLocalPos.z - planeWorld.d) / planeWorld.b;

	return fY;
}

void CVIBufferTerrain::Free()
{
	__super::Free();
}

CComponent * CVIBufferTerrain::Clone(void * pArg)
{
	CComponent* pInstance = new CVIBufferTerrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CVIBufferTerrain Clone Failed");
	}

	return pInstance;
}

CVIBufferTerrain * CVIBufferTerrain::Create(_pDevice pDevice, _uint iNumVerticesX, _uint iNumVerticesZ, _float fSizeX, _float fSizeZ, _float fU, _float fV)
{
	CVIBufferTerrain* pInstance = new CVIBufferTerrain(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype(iNumVerticesX, iNumVerticesZ, fSizeX, fSizeZ,  fU, fV)))
	{
		Safe_Release(pInstance);
		MSGBOX("CVIBufferTerrain Create Failed");
	}

	return pInstance;
}

CVIBufferTerrain * CVIBufferTerrain::Create(_pDevice pGraphic_Device, const _tchar * pHeightMapPath)
{
	CVIBufferTerrain* pInstance = new CVIBufferTerrain(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct_Prototype(pHeightMapPath)))
	{
		Safe_Release(pInstance);
		MSGBOX("CVIBufferTerrain Create Failed");
	}

	return pInstance;
}

CVIBufferTerrain * CVIBufferTerrain::Create(_pDevice pGraphicDevice)
{
	CVIBufferTerrain* pInstance = new CVIBufferTerrain(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CVIBufferTerrain Create Failed");
	}

	return pInstance;
}
