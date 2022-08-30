#pragma once
#ifndef __ENGINE_VI_BUFFER_RECT_COLOR_H__
#define __ENGINE_VI_BUFFER_RECT_COLOR_H__

#include "VIBuffer.h"
BEGIN(Engine)

class ENGINE_DLL CVIBufferRectColor final:
	public CVIBuffer
{
public:
	typedef struct tagVertexInfo {
		_vec3 vSize;
		_vec3 vPos;
		_ulong dwColor;
	} VTX_INFO;

public:
	explicit CVIBufferRectColor(_pDevice pGraphicDevice);
	explicit CVIBufferRectColor(const CVIBufferRectColor& rhs);
	virtual ~CVIBufferRectColor() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void* pArg = nullptr);
	virtual HRESULT NativeConstruct(void* pArg) override;

public: //SET
	HRESULT Change_Color(D3DXCOLOR dwColor);
	virtual _bool				Picking(const _vec3& vMouseRay, const _vec3& vMousePivot, _vec3* pOut) override;

public:
	virtual void Free() override;
	virtual CComponent * Clone(void * pArg) override;
	static CVIBufferRectColor* Create(_pDevice pGraphicDevice, void* pArg = nullptr);
};
END
#endif // !__ENGINE_VI_BUFFER_RECT_H__
