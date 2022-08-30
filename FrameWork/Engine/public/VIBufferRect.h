#pragma once
#ifndef __ENGINE_VI_BUFFER_RECT_H__
#define __ENGINE_VI_BUFFER_RECT_H__

#include "VIBuffer.h"
BEGIN(Engine)

class ENGINE_DLL CVIBufferRect final:
	public CVIBuffer
{
public:
	typedef struct tagVIBufferRectPosSize {
		_vec3 vSize;
		_vec3 vPos;
	} SIZE_POS;

public:
	explicit CVIBufferRect(_pDevice pGraphicDevice);
	explicit CVIBufferRect(const CVIBufferRect& rhs);
	virtual ~CVIBufferRect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void* pArg = nullptr);
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _bool				Picking(const _vec3& vMouseRay, const _vec3& vMousePivot, _vec3* pOut) override;

public: //SET
	//위에서 아래로 UV, POS 감소시킴 최대 0까지 비율 넣어주면 됨
	HRESULT Decrease_Top(_float fRatio);
	//vPos 변경시킨거 다시 되돌려놓음
	HRESULT Reset();

public: //GET

private:
	_vec3 m_vColliderCenter = _vec3(0.f, 0.f, 0.f);

public:
	virtual void Free() override;
	virtual CComponent * Clone(void * pArg) override;
	static CVIBufferRect* Create(_pDevice pGraphicDevice, void* pArg = nullptr);
};
END
#endif // !__ENGINE_VI_BUFFER_RECT_H__
