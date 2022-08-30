#pragma once
#ifndef __ENGINE_VIBUFFER_RECT_XY_H__
#define __ENGINE_VIBUFFER_RECT_XY_H__

#include "VIBuffer.h"
BEGIN(Engine)

class ENGINE_DLL CVIBufferRectXY final : public CVIBuffer
{
public:
	enum WALLLOOK_XY { WL_FRONT , WL_BACK , WL_END };
public:
	explicit CVIBufferRectXY(_pDevice pGraphicDevice);
	explicit CVIBufferRectXY(const CVIBufferRectXY& rhs);
	virtual ~CVIBufferRectXY() DEFAULT;

public:
	// 1) 크기 1짜리 오른쪽을 보는 YZ 평면 한칸
	virtual HRESULT				NativeConstruct_Prototype() override;
	// 2) 크기 n짜리 보는 방향을 정하는 YZ 평면 Y*Z 칸
	virtual HRESULT				NativeConstruct_Prototype(WALLLOOK_XY eSide
		, _float fIntervalX, _float fIntervalY, _float fMultipleU, _float fMultipleV, _uint iNumVerticesX, _uint iNumVerticesY);
	virtual HRESULT				NativeConstruct(void* pArg) override;

	virtual _bool				Picking(const _vec3& vMouseRay, const _vec3& vMousePivot, _vec3* pOut) override;

public:
	_bool						Get_PlaneDir() { return m_bFront; }

private:
	_uint						m_iNumVerticesX = 0;
	_uint						m_iNumVerticesY = 0;
	_bool						m_bFront = true;

public:
	// 크기 1짜리 오른쪽을 보는 YZ 평면 한칸
	static	CVIBufferRectXY*	Create(_pDevice pGraphicDevice);
	// 크기 n짜리 보는 방향을 정하는 YZ 평면 Y*Z 칸
	static	CVIBufferRectXY*	Create(_pDevice pGraphicDevice, WALLLOOK_XY eSide
		, _float fIntervalX, _float fIntervalY, _float fMultipleU, _float fMultipleV, _uint iNumVerticesX, _uint iNumVerticesY);

	virtual	void				Free() override;
	virtual CComponent*			Clone(void* pArg) override;
};
END
#endif // !__ENGINE_VIBUFFER_RECT_XY_H__
