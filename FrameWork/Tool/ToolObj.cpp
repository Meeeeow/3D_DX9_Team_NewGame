#include "stdafx.h"
#include "ToolObj.h"

CToolObj::CToolObj(_pDevice pDevice)
	:CGameObject(pDevice)
{
}

HRESULT CToolObj::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CToolObj NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CToolObj::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CToolObj Render - __super");


	return S_OK;
}

_int CToolObj::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;
		
	return _int();
}

HRESULT CToolObj::Set_ObjInfo(void * pArg, CTransform* pTransform)
{
	if (pArg == nullptr)
		MSG_FAIL("CToolObj Set_ObjInfo - pArg is null");

	m_pInfo = (OBJ_INFO*)pArg;

	pTransform->Set_Matrix(m_pInfo->matWorld);

	return S_OK;
}

HRESULT CToolObj::Set_ObjInfo(OBJ_INFO * pObjInfo)
{
	if (m_pInfo != nullptr)
		Safe_Delete(m_pInfo);

	m_pInfo = pObjInfo;

	return S_OK;
}

HRESULT CToolObj::Set_Matrix(_mat matWorld)
{
	m_pInfo->matWorld = matWorld;
	return S_OK;
}

void CToolObj::Free()
{
	Safe_Delete(m_pInfo);

	__super::Free();
}

CGameObject * CToolObj::Clone(void * pArg)
{
	return nullptr;
}
