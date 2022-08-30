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
	//트랜스폼에 OBJ_INFO를 이용해 값 채우기
	HRESULT Set_ObjInfo(void* pArg, CTransform* pTransform); 
	//OBJ_INFO 세팅
	HRESULT Set_ObjInfo(OBJ_INFO* pObjInfo);
	//오브젝트 이름 따로 세팅하기 위해 만듬
	HRESULT Set_ObjName(CString wstrObjName) { lstrcpy(m_pInfo->szObjName, wstrObjName.GetString()); return S_OK; }

	void Set_ObjInfoTextureIndex(_int iIndex) { m_pInfo->iTextureIndex = iIndex; }

	//부모에 obj_info가 있고 자식에 Transform이 있어서 생긴 문제를 해결하기 위해 만든 함수, virtual을 통해 자식이 먼저 호출되게해 둘다 동시에
	//초기화 할 수 있도록 만들었다.
	virtual HRESULT Set_Matrix(_mat matWorld);

public: //GET
	const OBJ_INFO* Get_ObjInfo() const { return m_pInfo; }
	OBJ_INFO*		Get_ObjInfoEx() { return m_pInfo; }
public:
	virtual void Free() override;

	//순수가상함수 구현 필요는 없다
	virtual CGameObject* Clone(void* pArg) override;
protected:
	OBJ_INFO* m_pInfo = nullptr;

};

#endif // !__TOOL_TOOL_OBJ_H__
