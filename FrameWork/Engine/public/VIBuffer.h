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
	/* 정점 배열의 메모리공간을 대표하는 컴 객체다. */
	_pVtxBuf			m_pVB = nullptr;
	_uint				m_iStride = 0; /* 정점하나의 바이트 크기. */
	_uint				m_iNumVertices = 0; /* 이 모델을 그리기위한 정점의 갯수.  */
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
