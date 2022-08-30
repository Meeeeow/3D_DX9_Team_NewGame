#include "..\public\VIBuffer.h"



CVIBuffer::CVIBuffer(_pDevice pGraphicDevice)
	:CComponent(pGraphicDevice)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs), m_pVB(rhs.m_pVB), m_iStride(rhs.m_iStride), m_iNumVertices(rhs.m_iNumVertices)
	, m_pIB(rhs.m_pIB), m_dwFVF(rhs.m_dwFVF), m_iNumPrimitive(rhs.m_iNumPrimitive), m_iIndicesSize(rhs.m_iIndicesSize)
	, m_eFormat(rhs.m_eFormat), m_ePrimitiveType(rhs.m_ePrimitiveType), m_pVerticesPosArr(rhs.m_pVerticesPosArr)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CVIBuffer NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CVIBuffer::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CVIBuffer NativeConstruct - __super");

	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CVIBuffer Render - m_pGraphicDevice is null");

	m_pGraphicDevice->SetStreamSource(0, m_pVB, 0, m_iStride);
	m_pGraphicDevice->SetFVF(m_dwFVF);
	m_pGraphicDevice->SetIndices(m_pIB);
	m_pGraphicDevice->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, m_iNumVertices, 0, m_iNumPrimitive);

	return S_OK;
}

_bool CVIBuffer::Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, _vec3 * pOut)
{
	return false;
}

HRESULT CVIBuffer::CreateVertexBuffer()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CVIBuffer CreateVertexBuffer - m_pGraphicDevice is null");

	if(FAILED(m_pGraphicDevice->CreateVertexBuffer(m_iNumVertices * m_iStride, 0, m_dwFVF, D3DPOOL_MANAGED
		, &m_pVB, nullptr)))
		MSG_FAIL("CVIBuffer CreateVertexBuffer - CreateVertexBuffer");

	return S_OK;
}

HRESULT CVIBuffer::CreateIndexBuffer()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CVIBuffer CreateIndexBuffer - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->CreateIndexBuffer(m_iNumPrimitive * m_iIndicesSize, 0, m_eFormat, D3DPOOL_MANAGED
		, &m_pIB, nullptr)))
		MSG_FAIL("CVIBuffer CreateVertexBuffer - CreateVertexBuffer");

	return S_OK;
}

HRESULT CVIBuffer::Create_VertexPosArr()
{
	m_pVerticesPosArr = new _vec3[m_iNumVertices];

	void* pVertices = nullptr;
	m_pVB->Lock(0, 0, &pVertices, 0);

	for (_uint i = 0; i < m_iNumVertices; ++i)
		m_pVerticesPosArr[i] = *(_vec3*)((_byte*)pVertices + i * m_iStride);

	m_pVB->Unlock();

	return S_OK;
}

void CVIBuffer::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	if (m_bClone == false)
		Safe_Delete_Arr(m_pVerticesPosArr);

	__super::Free();
}
