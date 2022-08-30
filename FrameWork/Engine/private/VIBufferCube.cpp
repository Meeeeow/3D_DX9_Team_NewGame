#include "..\public\VIBufferCube.h"
#include "Graphic_Device.h"

CVIBufferCube::CVIBufferCube(_pDevice pGraphicDevice)
	:CVIBuffer(pGraphicDevice)
{
}

CVIBufferCube::CVIBufferCube(const CVIBufferCube & rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CVIBufferCube::NativeConstruct_Prototype(void* pArg)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CVIBufferCube NativeConstruct_Prototype - __super");

	m_iNumVertices = 8;

	m_iStride = sizeof(VTXCUBETEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	if (FAILED(__super::CreateVertexBuffer()))
		return E_FAIL;

	VTXCUBETEX*			pVertices = nullptr;

	m_pVB->Lock(0, /*m_iStride * m_iNumVertices*/0, (void**)&pVertices, 0);

	if (nullptr == pArg)
	{
		pVertices[0].vPos = _vec3(-0.5f, 0.5f, -0.5f);
		pVertices[1].vPos = _vec3(0.5f, 0.5f, -0.5f);
		pVertices[2].vPos = _vec3(0.5f, -0.5f, -0.5f);
		pVertices[3].vPos = _vec3(-0.5f, -0.5f, -0.5f);
		pVertices[4].vPos = _vec3(-0.5f, 0.5f, 0.5f);
		pVertices[5].vPos = _vec3(0.5f, 0.5f, 0.5f);
		pVertices[6].vPos = _vec3(0.5f, -0.5f, 0.5f);
		pVertices[7].vPos = _vec3(-0.5f, -0.5f, 0.5f);
	}
	else
	{
		_vec3 vSize;
		vSize = *(_vec3*)pArg;
		_float fSizeX = vSize.x * 0.5f;
		_float fSizeY = vSize.y * 0.5f;
		_float fSizeZ = vSize.z * 0.5f;

		pVertices[0].vPos = _vec3(-fSizeX, fSizeY, -fSizeZ);
		pVertices[1].vPos = _vec3(fSizeX, fSizeY, -fSizeZ);
		pVertices[2].vPos = _vec3(fSizeX, -fSizeY, -fSizeZ);
		pVertices[3].vPos = _vec3(-fSizeX, -fSizeY, -fSizeZ);
		pVertices[4].vPos = _vec3(-fSizeX, fSizeY, fSizeZ);
		pVertices[5].vPos = _vec3(fSizeX, fSizeY, fSizeZ);
		pVertices[6].vPos = _vec3(fSizeX, -fSizeY, fSizeZ);
		pVertices[7].vPos = _vec3(-fSizeX, -fSizeY, fSizeZ);
	}

	for (_uint i = 0; i < 8; ++i)
		pVertices[i].vTexUV = pVertices[i].vPos;

	m_pVB->Unlock();

	m_iNumPrimitive = 12;
	m_iIndicesSize = sizeof(INDICES_TRI16);
	m_eFormat = D3DFMT_INDEX16;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;

	if (FAILED(CVIBuffer::CreateIndexBuffer()))
		return E_FAIL;

	INDICES_TRI16*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	/* +X */
	pIndices[0]._1 = 1; pIndices[0]._2 = 5; pIndices[0]._3 = 6;
	pIndices[1]._1 = 1; pIndices[1]._2 = 6; pIndices[1]._3 = 2;

	/* -X */
	pIndices[2]._1 = 4; pIndices[2]._2 = 0; pIndices[2]._3 = 3;
	pIndices[3]._1 = 4; pIndices[3]._2 = 3; pIndices[3]._3 = 7;

	/* +Y */
	pIndices[4]._1 = 4; pIndices[4]._2 = 5; pIndices[4]._3 = 1;
	pIndices[5]._1 = 4; pIndices[5]._2 = 1; pIndices[5]._3 = 0;

	/* -Y */
	pIndices[6]._1 = 3; pIndices[6]._2 = 2; pIndices[6]._3 = 6;
	pIndices[7]._1 = 3; pIndices[7]._2 = 6; pIndices[7]._3 = 7;

	/* +Z */
	pIndices[8]._1 = 5; pIndices[8]._2 = 4; pIndices[8]._3 = 7;
	pIndices[9]._1 = 5; pIndices[9]._2 = 7; pIndices[9]._3 = 6;

	/* -Z */
	pIndices[10]._1 = 0; pIndices[10]._2 = 1; pIndices[10]._3 = 2;
	pIndices[11]._1 = 0; pIndices[11]._2 = 2; pIndices[11]._3 = 3;

	m_pIB->Unlock();

	if (FAILED(__super::Create_VertexPosArr()))
		MSG_FAIL("CVIBufferCube::NativeConstruct_Prototype - __super::Create_VertexPosArr");

	return S_OK;
}

HRESULT CVIBufferCube::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CVIBufferCube NativeConstruct - __super");

	return S_OK;
}

_bool CVIBufferCube::Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, _vec3 * pOut)
{
	_float fU = 0.f, fV = 0.f, fDist = 0.f;

	//큐브 면 중 가장 가까운 친구 찾아서 보내야함
	
	_float fDistMin = -1.f;

	INDICES_TRI16 pIndices[12];

	// +X
	pIndices[0]._1 = 1; pIndices[0]._2 = 5; pIndices[0]._3 = 6;
	pIndices[1]._1 = 1; pIndices[1]._2 = 6; pIndices[1]._3 = 2;

	/* -X */
	pIndices[2]._1 = 4; pIndices[2]._2 = 0; pIndices[2]._3 = 3;
	pIndices[3]._1 = 4; pIndices[3]._2 = 3; pIndices[3]._3 = 7;

	/* +Y */
	pIndices[4]._1 = 4; pIndices[4]._2 = 5; pIndices[4]._3 = 1;
	pIndices[5]._1 = 4; pIndices[5]._2 = 1; pIndices[5]._3 = 0;

	/* -Y */
	pIndices[6]._1 = 3; pIndices[6]._2 = 2; pIndices[6]._3 = 6;
	pIndices[7]._1 = 3; pIndices[7]._2 = 6; pIndices[7]._3 = 7;

	/* +Z */
	pIndices[8]._1 = 5; pIndices[8]._2 = 4; pIndices[8]._3 = 7;
	pIndices[9]._1 = 5; pIndices[9]._2 = 7; pIndices[9]._3 = 6;

	/* -Z */
	pIndices[10]._1 = 0; pIndices[10]._2 = 1; pIndices[10]._3 = 2;
	pIndices[11]._1 = 0; pIndices[11]._2 = 2; pIndices[11]._3 = 3;

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

void CVIBufferCube::Get_BottomBufferPos(_vec3 * vPosArr)
{
	vPosArr[0] = m_pVerticesPosArr[0];
	vPosArr[1] = m_pVerticesPosArr[1];
	vPosArr[2] = m_pVerticesPosArr[2];
	vPosArr[3] = m_pVerticesPosArr[3];
}

void CVIBufferCube::Free()
{
	__super::Free();
}

CComponent * CVIBufferCube::Clone(void * pArg)
{
	CComponent* pInstance = new CVIBufferCube(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CVIBufferCube Clone Failed");
	}

	return pInstance;
}

CVIBufferCube * CVIBufferCube::Create(_pDevice pGraphicDevice, void* pArg)
{
	CVIBufferCube* pInstance = new CVIBufferCube(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CVIBufferCube Create Failed");
	}

	return pInstance;
}
