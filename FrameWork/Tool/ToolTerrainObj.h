#pragma once
#ifndef __TOOL_TERRAIN_OBJ_H__
#define __TOOL_TERRAIN_OBJ_H__

#include "ToolObj.h"

BEGIN(Engine)
class CRenderer;
class CVIBufferTerrain;
class CTransform;
class CTexture;
END

class CToolTerrainObj final :
	public CToolObj
{
public:
	explicit CToolTerrainObj(_pDevice pDevice);
	virtual ~CToolTerrainObj() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float fTimeDelta) override;
	virtual _int LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: //SET
	//터레인 버퍼 사용자가 원하는 대로 만들기 위해 기존 것 지우고 새로 만듬
	HRESULT Set_TerrainBuffer(OBJ_INFO* pObjInfo);
	//텍스쳐 인덱스 세팅
	void Set_TextureIndex(_int iIndex = 0) { m_iIndex = iIndex; }
	//오브젝트 이름 세팅
	void Set_ObjName(CString wstrObjName) { CToolObj::Set_ObjName(wstrObjName); }
	//와이어 프레임으로 변경
	void Set_WireFrame() { m_bRenderWireFrame = !m_bRenderWireFrame; }
	//부모와 자식 동시 행렬 세팅
	virtual HRESULT Set_Matrix(_mat matWorld) override;

public: //GET
	//지형 타기
	_vec3 Compute_Height(CTransform* pTransform);

private:
	HRESULT SetUp_Component();

private:
	CRenderer*					m_pRendererCom = nullptr;
	CVIBufferTerrain*			m_pVIBufferCom = nullptr;
	CTransform*					m_pTransformCom = nullptr;
	CTexture*					m_pTextureCom = nullptr;

private:
	_int m_iIndex = 0;
	bool m_bRenderWireFrame = false;

public:
	virtual void Free() override;
	static CToolTerrainObj* Create(_pDevice pDevice);

	// CToolObj을(를) 통해 상속됨
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};

#endif // !__CLIENT_TERRAIN_H__
