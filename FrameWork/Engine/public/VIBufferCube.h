#pragma once
#ifndef __ENGINE_VI_BUFFER_CUBE_H__
#define __ENGINE_VI_BUFFER_CUBE_H__

#include "VIBuffer.h"
BEGIN(Engine)

class ENGINE_DLL CVIBufferCube final:
	public CVIBuffer
{
public:
	explicit CVIBufferCube(_pDevice pGraphicDevice);
	explicit CVIBufferCube(const CVIBufferCube& rhs);
	virtual ~CVIBufferCube() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void* pArg = nullptr);
	virtual HRESULT NativeConstruct(void* pArg) override;

	virtual _bool Picking(const _vec3& vMouseRay, const _vec3& vMousePivot, _vec3* pOut) override;

public:
	void Get_BottomBufferPos(_vec3* vPosArr);

public:
	virtual void Free() override;
	virtual CComponent * Clone(void * pArg) override;
	static CVIBufferCube* Create(_pDevice pGraphicDevice, void* pArg = nullptr);
};
END
#endif // !__ENGINE_VI_BUFFER_RECT_H__
