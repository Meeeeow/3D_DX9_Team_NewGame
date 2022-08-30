#pragma once
#ifndef __TOOL_CUBE_OBJ_H__
#define __TOOL_CUBE_OBJ_H__

#include "ToolObj.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;
class CVIBufferCube;
class CBoxCollider;
END

class CCubeObj :
	public CToolObj
{
private:
	explicit CCubeObj(_pDevice pDevice);
	virtual ~CCubeObj() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(OBJ_INFO* pInfo);
	virtual HRESULT Render() override;
	virtual _int	LateTick(_float fTimeDelta) override;

private:
	HRESULT SetUp_Component(OBJ_INFO* pInfo);

private:
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CVIBufferCube*	m_pVIBufferCom = nullptr;
	CBoxCollider* m_pBoxCollider = nullptr;

public:
	void Set_RectVIBufferSize(LPDIRECT3DDEVICE9 pGraphic_Device, _vec3* pSize);
	void Update_BoxColliderValues(OBJ_INFO* pInfo);

public:
	void ReSet_Rotation();
	void Set_Rotation(_int iValueX, _int iValueY, _int iValueZ);
	virtual HRESULT Set_Matrix(_mat mat) override;

public:
	// �ؽ��� ������Ʈ �� �ؽ��� ���� ��ȯ
	_uint Get_ImageCount();

	// ����Ʈ �ڽ� �ε��� ��ȣ�� �ؽ��� �ѹ� ����
	void Set_TextureNumber(_uint iTexNum) { m_iTextureNum = iTexNum; }

private:
	_uint			m_iTextureNum = 0;

public:
	static CCubeObj* Create(_pDevice pDevice, OBJ_INFO* pInfo);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;


	// CToolObj��(��) ���� ��ӵ�
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;

};

#endif // !__TOOL_RECT_OBJ_H__
