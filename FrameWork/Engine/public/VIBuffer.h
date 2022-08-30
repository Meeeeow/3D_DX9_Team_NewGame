#pragma once
#ifndef __ENGINE_VI_BUFFER_H__
#define __ENGINE_VI_BUFFER_H__

#include "Component.h"
BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract:
	public CComponent
{

protected:
	explicit CVIBuffer(_pDevice pGraphicDevice);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render();

	virtual _bool Picking(const _vec3& vMouseRay, const _vec3& vMousePivot, _vec3* pOut);

protected:
	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	HRESULT Create_VertexPosArr();

protected:
	/* ���� �迭�� �޸𸮰����� ��ǥ�ϴ� �� ��ü��. */
	_pVtxBuf			m_pVB = nullptr;
	_uint				m_iStride = 0; /* �����ϳ��� ����Ʈ ũ��. */
	_uint				m_iNumVertices = 0; /* �� ���� �׸������� ������ ����.  */
	_ulong				m_dwFVF = 0;
	_vec3*				m_pVerticesPosArr = nullptr;

	_pIdxBuf			m_pIB = nullptr;
	_uint				m_iNumPrimitive = 0;
	_uint				m_iIndicesSize = 0;
	D3DFORMAT			m_eFormat;
	D3DPRIMITIVETYPE	m_ePrimitiveType;


public:
	virtual void Free() override;
	virtual CComponent * Clone(void * pArg)= 0;
};

END
#endif // !__ENGINE_VI_BUFFER_H__
