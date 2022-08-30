#pragma once
#ifndef __ENGINE_VIBUFFER_TERRAIN_H__
#define __ENGINE_VIBUFFER_TERRAIN_H__

#include "VIBuffer.h"
BEGIN(Engine)
class ENGINE_DLL CVIBufferTerrain final:
	public CVIBuffer
{
public:
	explicit CVIBufferTerrain(_pDevice pDevice);
	explicit CVIBufferTerrain(const CVIBufferTerrain& rhs);
	virtual ~CVIBufferTerrain() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ, _float fSizeX = 1.f, _float fSizeZ = 1.f, _float fU = 1.f, _float fV = 1.f);
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pHeightMapPath);
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _bool Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, _vec3 * pOut) override;

public:
	_float Compute_Height(_vec3 vLocalPos);

private:
	_int m_iNumVerticesX = 0;
	_int m_iNumVerticesZ = 0;

public:
	virtual void Free() override;
	virtual CComponent * Clone(void * pArg) override;
	static CVIBufferTerrain* Create(_pDevice pDevice, _uint iNumVerticesX, _uint iNumVerticesZ, _float fSizeX = 1.f, _float fSizeZ = 1.f, _float fU = 1.f, _float fV = 1.f);
	static CVIBufferTerrain* Create(_pDevice pGraphic_Device, const _tchar* pHeightMapPath);
	static CVIBufferTerrain* Create(_pDevice pGraphicDevice);
};
END
#endif // !__ENGINE_VIBUFFER_TERRAIN_H__
