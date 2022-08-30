#pragma once
#ifndef __TOOL_UI_OBJ_H__
#define __TOOL_UI_OBJ_H__


BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;
class CVIBufferRect;
END

#include "ToolObj.h"

class CUIObj :
	public CToolObj
{
private:
	explicit CUIObj(_pDevice pDevice);
	virtual ~CUIObj() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(void* pArg);
	virtual HRESULT Render() override;
	virtual _int	Tick(_float fTimeDelta) override;
	virtual _int	LateTick(_float fTimeDelta) override;

public: //SET
	virtual HRESULT Set_Matrix(_mat matrix) override;

private:
	HRESULT SetUp_Component();

private:
	CTransform*		m_pTransform = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CTexture*		m_pTexture = nullptr;
	CVIBufferRect*	m_pVIBuffer = nullptr;


public:
	virtual void Free() override;
	//Argument로 텍스쳐 문자열 받아오면 될듯
	static CUIObj* Create(_pDevice pDevice, void* pArg);

	// CToolObj을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};

#endif // !__TOOL_RECT_OBJ_H__
