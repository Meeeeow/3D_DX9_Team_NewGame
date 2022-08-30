#pragma once
#ifndef __ENGINE_VIBUFFER_RECT_YZ_H__
#define __ENGINE_VIBUFFER_RECT_YZ_H__

#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBufferRectYZ final : public CVIBuffer
{
public:
	// 벽이 바라보는 방향
	enum WALLSIDE_YZ { WS_RIGHT, WS_LEFT, WS_END};
public:
	explicit CVIBufferRectYZ(_pDevice pGraphicDevice);
	explicit CVIBufferRectYZ(const CVIBufferRectYZ& rhs);
	virtual ~CVIBufferRectYZ() DEFAULT;

public:
	// 1) 크기 1짜리 오른쪽을 보는 YZ 평면 한칸
	virtual HRESULT				NativeConstruct_Prototype() override;
	// 2) 크기 n짜리 보는 방향을 정하는 YZ 평면 Y*Z 칸
	virtual HRESULT				NativeConstruct_Prototype(WALLSIDE_YZ eSide
		, _float fIntervalY, _float fIntervalZ, _float fMultipleU, _float fMultipleV, _uint iNumVerticesY, _uint iNumVerticesZ);
	virtual HRESULT				NativeConstruct(void* pArg) override;

public:
	_bool						Get_PlaneDir() { return m_bRight; }
	virtual _bool				Picking(const _vec3& vMouseRay, const _vec3& vMousePivot, _vec3* pOut) override;

private:
	_uint						m_iNumVerticesY = 0;
	_uint						m_iNumVerticesZ = 0;
	_bool						m_bRight = true;
public:
	// 크기 1짜리 오른쪽을 보는 YZ 평면 한칸
	static	CVIBufferRectYZ*	Create(_pDevice pGraphicDevice);
	// 크기 n짜리 보는 방향을 정하는 YZ 평면 Y*Z 칸
	static	CVIBufferRectYZ*	Create(_pDevice pGraphicDevice, WALLSIDE_YZ eSide
		, _float fIntervalY, _float fIntervalZ, _float fMultipleU, _float fMultipleV, _uint iNumVerticesY, _uint iNumVerticesZ);

	virtual	void				Free() override;
	virtual CComponent*			Clone(void* pArg) override;

};

END
#endif // !__ENGINE_VIBUFFER_RECT_YZ_H__
