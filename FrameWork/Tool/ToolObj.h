#pragma once
#ifndef __TOOL_TOOL_OBJ_H__
#define __TOOL_TOOL_OBJ_H__

#include "GameObject.h"
#include "BoxCollider.h"

BEGIN(Engine)
class CTransform;
class CBoxCollider;
END

class CToolObj abstract:
	public CGameObject
{
public:
	explicit CToolObj(_pDevice pDevice);
	virtual ~CToolObj() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT Render() override;
	virtual _int LateTick(_float fTimeDelta) override;

public: //SET
	//Ʈ�������� OBJ_INFO�� �̿��� �� ä���
	HRESULT Set_ObjInfo(void* pArg, CTransform* pTransform); 
	//OBJ_INFO ����
	HRESULT Set_ObjInfo(OBJ_INFO* pObjInfo);
	//������Ʈ �̸� ���� �����ϱ� ���� ����
	HRESULT Set_ObjName(CString wstrObjName) { lstrcpy(m_pInfo->szObjName, wstrObjName.GetString()); return S_OK; }

	void Set_ObjInfoTextureIndex(_int iIndex) { m_pInfo->iTextureIndex = iIndex; }

	//�θ� obj_info�� �ְ� �ڽĿ� Transform�� �־ ���� ������ �ذ��ϱ� ���� ���� �Լ�, virtual�� ���� �ڽ��� ���� ȣ��ǰ��� �Ѵ� ���ÿ�
	//�ʱ�ȭ �� �� �ֵ��� �������.
	virtual HRESULT Set_Matrix(_mat matWorld);

public: //GET
	const OBJ_INFO* Get_ObjInfo() const { return m_pInfo; }
	OBJ_INFO*		Get_ObjInfoEx() { return m_pInfo; }
public:
	virtual void Free() override;

	//���������Լ� ���� �ʿ�� ����
	virtual CGameObject* Clone(void* pArg) override;
protected:
	OBJ_INFO* m_pInfo = nullptr;

};

#endif // !__TOOL_TOOL_OBJ_H__
