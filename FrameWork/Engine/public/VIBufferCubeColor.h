#pragma once
#ifndef __ENGINE_VI_BUFFER_CUBE_COLOR_H__
#define __ENGINE_VI_BUFFER_CUBE_COLOR_H__

#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBufferCubeColor final : public CVIBuffer
{
public:
	typedef struct tagCubeInfo{
		_ulong	dwColor = D3DCOLOR_ARGB(255,255,255,255);
		_vec3	vSize = _vec3(1.f, 1.f, 1.f);
		_int	iRed = 255, iBlue = 255, iGreen = 255;
	}DESC;
public:
	explicit CVIBufferCubeColor(_pDevice pGraphicDevice);
	explicit CVIBufferCubeColor(const CVIBufferCubeColor& rhs);
	virtual ~CVIBufferCubeColor() DEFAULT;


public:
	virtual	HRESULT	NativeConstruct_Prototype(DESC tCubeInfo);
	virtual HRESULT NativeConstruct(void* pArg) override;

	void			AlphaChange();
private:
	_float			m_fAlpha = 255.f;
	DESC			m_tDesc;
	_float			m_fIncreasePower = 13.5f;
public:
	static CVIBufferCubeColor*	Create(_pDevice pGraphicDevice, DESC tCubeInfo);
	virtual CComponent*			Clone(void* pArg) override;
	virtual void				Free() override;
};
END

#endif // !__ENGINE_VI_BUFFER_CUBE_COLOR_H__
