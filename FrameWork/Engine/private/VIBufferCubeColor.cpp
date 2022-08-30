#include "..\public\VIBufferCubeColor.h"



CVIBufferCubeColor::CVIBufferCubeColor(_pDevice pGraphicDevice)
	: CVIBuffer(pGraphicDevice)
{
}

CVIBufferCubeColor::CVIBufferCubeColor(const CVIBufferCubeColor & rhs)
	: CVIBuffer(rhs)
	, m_tDesc(rhs.m_tDesc)
{
}


HRESULT CVIBufferCubeColor::NativeConstruct_Prototype(DESC tCubeInfo)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CVIBufferCubeColor::NativeConstruct_Prototype - __super");

 	m_tDesc = tCubeInfo;

	m_iNumVertices = 8;
	m_iStride = sizeof(VTXCUBECOL);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	if (FAILED(__super::CreateVertexBuffer()))
		return E_FAIL;

	_vec3 vSize = tCubeInfo.vSize / 2.f;
	

	VTXCUBECOL* pVertices = nullptr;
	
	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPos = _vec3(-vSize.x,  vSize.y, -vSize.z);
	pVertices[1].vPos = _vec3( vSize.x,  vSize.y, -vSize.z);
	pVertices[2].vPos = _vec3( vSize.x, -vSize.y, -vSize.z);
	pVertices[3].vPos = _vec3(-vSize.x, -vSize.y, -vSize.z);
	pVertices[4].vPos = _vec3(-vSize.x,  vSize.y,  vSize.z);
	pVertices[5].vPos = _vec3( vSize.x,  vSize.y,  vSize.z);
	pVertices[6].vPos = _vec3( vSize.x, -vSize.y,  vSize.z);
	pVertices[7].vPos = _vec3(-vSize.x, -vSize.y,  vSize.z);

	for (int i = 0; i < 8; ++i)
		pVertices[i].dwColor = tCubeInfo.dwColor;

	m_pVB->Unlock();

	m_iNumPrimitive = 12;
	m_iIndicesSize = sizeof(INDICES_TRI16);
	m_eFormat = D3DFMT_INDEX16;
	m_ePrimitiveType = D3DPT_TRIANGLELIST;

	if (FAILED(CreateIndexBuffer()))
		return E_FAIL;

	INDICES_TRI16* pIndices = nullptr;

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

	if (FAILED(Create_VertexPosArr()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBufferCubeColor::NativeConstruct(void * pArg)
{
	if(FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CVIBufferCubeColor::NativeConstruct - __super");

	

	return S_OK;
}

void CVIBufferCubeColor::AlphaChange()
{
	VTXCUBECOL* pVertices = nullptr;

	_ulong dwColor = D3DCOLOR_ARGB((int)m_fAlpha, m_tDesc.iRed, m_tDesc.iGreen, m_tDesc.iBlue);

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	m_fAlpha -= m_fIncreasePower;

	for (int i = 0; i < 8; ++i)
		pVertices[i].dwColor = dwColor;

	if (m_fAlpha <= 0 || m_fAlpha >= 255)
		m_fIncreasePower *= -1;

	m_pVB->Unlock();
}


CVIBufferCubeColor * CVIBufferCubeColor::Create(_pDevice pGraphicDevice, DESC tCubeInfo)
{
	CVIBufferCubeColor* pInstance = new CVIBufferCubeColor(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype(tCubeInfo)))
	{
		MSGBOX("CVIBufferCubeColor::Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBufferCubeColor::Clone(void * pArg)
{
	CVIBufferCubeColor* pInstance = new CVIBufferCubeColor(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CVIBufferCubeColor::Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBufferCubeColor::Free()
{
	__super::Free();
}
