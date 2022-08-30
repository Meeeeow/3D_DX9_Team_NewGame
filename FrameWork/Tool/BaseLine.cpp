#include "stdafx.h"
#include "BaseLine.h"
#include "GameInstance.h"

CBaseLine::CBaseLine(_pDevice pDevice)
	:CToolObj(pDevice)
{
}

HRESULT CBaseLine::NativeConstruct_Prototype(TYPE eType)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBaseLine NativeConstruct_Prototype - __super");

	//로드한 데이터 부모 클래스에 보관
	OBJ_INFO* pObjInfo = new OBJ_INFO;
	D3DXMatrixIdentity(&pObjInfo->matWorld);
	if(FAILED(__super::Set_ObjInfo(pObjInfo)))
		MSG_FAIL("CBaseLine::NativeConstruct_Prototype - Set_ObjInfo");

	if (FAILED(SetUp_Component(eType)))
		MSG_FAIL("CBaseLine NativeConstruct - SetUp_Component");

	return S_OK;
}

HRESULT CBaseLine::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CBaseLine NativeConstruct - __super");

	return S_OK;
}

HRESULT CBaseLine::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CBaseLine Render - __super");

	if (m_pTransformCom == nullptr)
		MSG_FAIL("CBaseLine Render - NullCheck");

	m_pTexture->Bind_OnDevice(Get_ObjInfo()->iTextureIndex);
	m_pTransformCom->Bind_OnDevice();
	m_pVIBuffer->Render();

	return S_OK;
}

HRESULT CBaseLine::SetUp_Scale(_vec3 vScale)
{
	if (m_pTransformCom == nullptr)
		MSG_FAIL("CBaseLine::SetUp_Scale - m_pTransformCom == nullptr");

	m_pTransformCom->SetUp_Scale(vScale);

	return S_OK;
}

HRESULT CBaseLine::SetUp_Scale(_float fX, _float fY, _float fZ)
{
	if (m_pTransformCom == nullptr)
		MSG_FAIL("CBaseLine::SetUp_Scale - m_pTransformCom == nullptr");

	m_pTransformCom->SetUp_Scale(fX, fY, fZ);

	return S_OK;
}

HRESULT CBaseLine::SetUp_Component(TYPE eType)
{
	//타입 받아와서 각각 사이즈 텍스쳐 다르게 만들기
	switch (eType)
	{
	case CBaseLine::TYPE_X:
		m_pVIBuffer = CVIBufferCube::Create(m_pGraphicDevice, _vec3(10000.f, 0.1f, 0.1f));
		Set_ObjInfoTextureIndex(4);
		break;
	case CBaseLine::TYPE_Y:
		m_pVIBuffer = CVIBufferCube::Create(m_pGraphicDevice, _vec3(0.1f, 10000.f, 0.1f));
		Set_ObjInfoTextureIndex(5);
		break;
	case CBaseLine::TYPE_Z:
		m_pVIBuffer = CVIBufferCube::Create(m_pGraphicDevice, _vec3(0.1f, 0.1f, 10000.f));
		Set_ObjInfoTextureIndex(6);
		break;

	}

	if (FAILED(Add_Component(LEVEL_STATIC, L"PrototypeTextureCube", L"ComTexture", (CComponent**)&m_pTexture)))
		MSG_FAIL("CBaseLine SetUp_Component - Add_Component m_pTexture");

	if (FAILED(Add_Component(LEVEL_STATIC, L"PrototypeTransform", L"ComTransform", (CComponent**)&m_pTransformCom)))
		MSG_FAIL("CBaseLine SetUp_Component - Add_Component m_pTransform");

	if (FAILED(Add_Component(LEVEL_STATIC, L"PrototypeRenderer", L"ComRenderer", (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CBaseLine SetUp_Component - Add_Component m_pRenderer");


	return S_OK;
}
void CBaseLine::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}

_vec3 CBaseLine::Get_MinPos()
{
	return _vec3();
}

_vec3 CBaseLine::Get_MaxPos()
{
	return _vec3();
}

_vec3 CBaseLine::Get_WorldPos()
{
	return _vec3();
}

_vec3 CBaseLine::Get_Length()
{
	return _vec3();
}

CBaseLine * CBaseLine::Create(_pDevice pDevice, TYPE eType)
{
	CBaseLine* pInstance = new CBaseLine(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype(eType)))
	{
		Safe_Release(pInstance);
		MSGBOX("CBaseLine Create Failed");
	}

	return pInstance;
}
