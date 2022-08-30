#pragma once
#ifndef __TOOL_RECT_OBJ_H__
#define __TOOL_RECT_OBJ_H__


BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;
class CVIBufferRect;
class CBoxCollider;
END

#include "ToolObj.h"

class CRectObj : public CToolObj
{
private:
	explicit CRectObj(_pDevice pDevice);
	virtual ~CRectObj() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(OBJ_INFO* pInfo);
	virtual HRESULT Render() override;
	virtual _int LateTick(_float fTimeDelta) override;

private:
	HRESULT SetUp_Component(OBJ_INFO* pInfo);

private:
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CVIBufferRect*		m_pVIBufferCom = nullptr;
	CBoxCollider*		m_pBoxCollider = nullptr;

public:
	void Set_RectVIBufferSize(LPDIRECT3DDEVICE9 pGraphic_Device, CVIBufferRect::SIZE_POS* tSizePos);
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
	_uint				m_iTextureNum = 0;

public:
	static CRectObj* Create(_pDevice pDevice, OBJ_INFO* pInfo);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;

	// CToolObj��(��) ���� ��ӵ�
	virtual _vec3 Get_MinPos() override;
	virtual _vec3 Get_MaxPos() override;
	virtual _vec3 Get_WorldPos() override;
	virtual _vec3 Get_Length() override;
};

#endif // !__TOOL_RECT_OBJ_H__
