#pragma once
#ifndef __ENGINE_VI_BUFFER_THORN_H__
#define __ENGINE_VI_BUFFER_THORN_H__

#include "VIBuffer.h"
BEGIN(Engine)

class ENGINE_DLL CVIBufferThorn final:
	public CVIBuffer
{
public:
	typedef struct tagVIBufferRectPosSize {
		_vec3 vSize;
		_vec3 vPos;
	} SIZE_POS;

public:
	explicit CVIBufferThorn(_pDevice pGraphicDevice);
	explicit CVIBufferThorn(const CVIBufferThorn& rhs);
	virtual ~CVIBufferThorn() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void* pArg = nullptr);
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _bool				Picking(const _vec3& vMouseRay, const _vec3& vMousePivot, _vec3* pOut) override;

public:
	virtual void Free() override;
	virtual CComponent * Clone(void * pArg) override;
	static CVIBufferThorn* Create(_pDevice pGraphicDevice, void* pArg = nullptr);
};
END
#endif // !__ENGINE_VI_BUFFER_RECT_H__
