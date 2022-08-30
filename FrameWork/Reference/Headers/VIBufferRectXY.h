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
	// 1) ũ�� 1¥�� �������� ���� YZ ��� ��ĭ
	virtual HRESULT				NativeConstruct_Prototype() override;
	// 2) ũ�� n¥�� ���� ������ ���ϴ� YZ ��� Y*Z ĭ
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
	// ũ�� 1¥�� �������� ���� YZ ��� ��ĭ
	static	CVIBufferRectXY*	Create(_pDevice pGraphicDevice);
	// ũ�� n¥�� ���� ������ ���ϴ� YZ ��� Y*Z ĭ
	static	CVIBufferRectXY*	Create(_pDevice pGraphicDevice, WALLLOOK_XY eSide
		, _float fIntervalX, _float fIntervalY, _float fMultipleU, _float fMultipleV, _uint iNumVerticesX, _uint iNumVerticesY);

	virtual	void				Free() override;
	virtual CComponent*			Clone(void* pArg) override;
};
END
#endif // !__ENGINE_VIBUFFER_RECT_XY_H__
