#pragma once
#ifndef __TOOL_BASE_LINE_H__
#define __TOOL_BASE_LINE_H__


BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;
class CVIBufferCube;
END

#include "ToolObj.h"

class CBaseLine : public CToolObj
{
public:
	enum TYPE { TYPE_X, TYPE_Y, TYPE_Z, TYPE_END };
private:
	explicit CBaseLine(_pDevice pDevice);
	virtual ~CBaseLine() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(TYPE eType);
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render() override;

public: //SET
	HRESULT SetUp_Scale(_vec3 vScale);
	HRESULT SetUp_Scale(_float fX, _float fY, _float fZ);

private:
	HRESULT SetUp_Component(TYPE eType);

private:
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CVIBufferCube*		m_pVIBuffer = nullptr;
	CTexture*			m_pTexture = nullptr;
	LPD3DXLINE			m_pLine = nullptr;
public:

public:
	static CBaseLine* Create(_pDevice pDevice, TYPE eType);
	virtual void Free() override;


	// CToolObj을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;

	virtual _vec3 Get_MaxPos() override;

	virtual _vec3 Get_WorldPos() override;

	virtual _vec3 Get_Length() override;

};

#endif // !__TOOL_RECT_OBJ_H__
