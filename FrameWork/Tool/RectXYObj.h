#pragma once
#ifndef __TOOL_RECT_XY_OBJ_H__
#define __TOOL_RECT_XY_OBJ_H__


BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;
class CVIBufferRectXY;
class CBoxCollider;
END


#include "ToolObj.h"

class CRectXYObj : public CToolObj
{
public:
	explicit CRectXYObj(_pDevice pGraphicDevice);
	virtual ~CRectXYObj() DEFAULT;

public:
	virtual HRESULT NativeConstruct_Prototype(void* pArg = nullptr);
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Set_RectXYBuffer(OBJ_INFO* pInfo);
	void	Set_TextureIndex(_int iIndex) { m_iTextureIndex = iIndex; }
	void	Do_WireFrameRendering() { m_bWireFrameRender = !m_bWireFrameRender; }
	virtual HRESULT Set_Matrix(_mat mat) override;
private:
	HRESULT SetUp_Component();

private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CVIBufferRectXY*	m_pVIBufferCom = nullptr;
	CBoxCollider*		m_pBoxCollider = nullptr;

private:
	_int				m_iTextureIndex = 0;
	_bool				m_bWireFrameRender = false;

public:
	virtual void Free() override;
	static CRectXYObj* Create(_pDevice pDevice);

	// CToolObj을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};


#endif // !__TOOL_RECT_XY_OBJ_H__
