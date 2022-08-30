#include "..\public\VIBufferThorn.h"
#include "Graphic_Device.h"

CVIBufferThorn::CVIBufferThorn(_pDevice pGraphicDevice)
	:CVIBuffer(pGraphicDevice)
{
}

CVIBufferThorn::CVIBufferThorn(const CVIBufferThorn & rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CVIBufferThorn::NativeConstruct_Prototype(void* pArg)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CVIBufferThorn NativeConstruct_Prototype - __super");

	m_iNumVertices = 5;
	m_iStride = sizeof(VTXCUBETEX);
	m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	if (FAILED(__super::CreateVertexBuffer()))
		MSG_FAIL("CVIBufferThorn NativeConstruct_Prototype - __super");

	VTXCUBETEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	_vec3 vSize = _vec3(0.5f, 0.5f, 0.5f);
	_vec3 vPos = _vec3(0.f, 0.f, 0.f);

	if (nullptr != pArg)
	{
		SIZE_POS tSizePos = *(SIZE_POS*)(pArg);
		vPos = tSizePos.vPos;
		vSize = tSizePos.vSize * 0.5f;
	}

	pVertices[0].vPos = _vec3(-vSize.x + vPos.x, vPos.y, -vSize.z + vPos.z);
	pVertices[1].vPos = _vec3(vSize.x + vPos.x, vPos.y, -vSize.z + vPos.z);
	pVertices[2].vPos = _vec3(vSize.x + vPos.x, vPos.y, vSize.z + vPos.z);
	pVertices[3].vPos = _vec3(-vSize.x + vPos.x, vPos.y, vSize.z + vPos.z);
	pVertices[4].vPos = _vec3(vPos.x, vPos.y + vSize.y * 2.f, vPos.z);

	for (_uint i = 0; i < m_iNumVertices; ++i)
		pVertices[i].vTexUV = pVertices[i].vPos;

	m_pVB->Unlock();

	m_iNumPrimitive = 6;
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

	pIndices[2]._1 = 4;
	pIndices[2]._2 = 0;
	pIndices[2]._3 = 3;

	pIndices[3]._1 = 4;
	pIndices[3]._2 = 1;
	pIndices[3]._3 = 0;

	pIndices[4]._1 = 4;
	pIndices[4]._2 = 2;
	pIndices[4]._3 = 1;

	pIndices[5]._1 = 4;
	pIndices[5]._2 = 3;
	pIndices[5]._3 = 2;

	m_pIB->Unlock();

	if (FAILED(__super::Create_VertexPosArr()))
		MSG_FAIL("CVIBufferThorn::NativeConstruct_Prototype - __super::Create_VertexPosArr");

	return S_OK;
}

HRESULT CVIBufferThorn::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CVIBufferThorn NativeConstruct - __super");

	return S_OK;
}

_bool CVIBufferThorn::Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, _vec3 * pOut)
{
	return false;
}

void CVIBufferThorn::Free()
{
	__super::Free();
}

CComponent * CVIBufferThorn::Clone(void * pArg)
{
	CComponent* pInstance = new CVIBufferThorn(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CVIBufferThorn Clone Failed");
	}

	return pInstance;
}

CVIBufferThorn * CVIBufferThorn::Create(_pDevice pGraphicDevice, void* pArg)
{
	CVIBufferThorn* pInstance = new CVIBufferThorn(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CVIBufferThorn Create Failed");
	}


	return pInstance;
}
