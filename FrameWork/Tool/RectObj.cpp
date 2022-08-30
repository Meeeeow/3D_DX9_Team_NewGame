#include "stdafx.h"
#include "RectObj.h"
#include "GameInstance.h"

CRectObj::CRectObj(_pDevice pDevice)
	:CToolObj(pDevice)
{
}

HRESULT CRectObj::NativeConstruct_Prototype(OBJ_INFO* pInfo)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CRectObj NativeConstruct_Prototype - __super");

	if (FAILED(Set_ObjInfo(pInfo)))
		MSG_FAIL("CRectObj NativeConstruct - Set_ObjInfo");


	m_iTextureNum = pInfo->iTextureIndex;


	if (FAILED(SetUp_Component(pInfo)))
		MSG_FAIL("CRectObj NativeConstruct - SetUp_Component");

	Update_BoxColliderValues(pInfo);


	Set_Matrix(pInfo->matWorld);

	return S_OK;
}

HRESULT CRectObj::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CRectObj Render - __super");

	if (m_pTransformCom == nullptr || 
		m_pTextureCom == nullptr || 
		m_pVIBufferCom == nullptr)
		MSG_FAIL("CRectObj Render - NullCheck");

	m_pBoxCollider->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxCollider->Tick(0.1f);

	m_pTransformCom->Bind_OnDevice();
	m_pTextureCom->Bind_OnDevice(m_iTextureNum);
	m_pVIBufferCom->Render();

	if (m_pBoxCollider != nullptr)
		m_pBoxCollider->Render();

	return S_OK;
}

_int CRectObj::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this);

	return _int();
}

HRESULT CRectObj::SetUp_Component(OBJ_INFO* pInfo)
{
	if (FAILED(Add_Component(LEVEL_STATIC, pInfo->szTextureName, L"ComTexture", (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CRectObj SetUp_Component - Add_Component m_pTexture");

	if (FAILED(Add_Component(LEVEL_STATIC, L"PrototypeTransform", L"ComTransform", (CComponent**)&m_pTransformCom)))
		MSG_FAIL("CRectObj SetUp_Component - Add_Component m_pTransform");

	if (FAILED(Add_Component(LEVEL_STATIC, L"PrototypeRenderer", L"ComRenderer", (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CRectObj SetUp_Component - Add_Component m_pRenderer");

	CVIBufferRect::SIZE_POS tSizePos;
	tSizePos.vPos = pInfo->vVertexPos;
	tSizePos.vSize = _vec3(pInfo->fSizeX, pInfo->fSizeY, pInfo->fSizeZ);
	m_pVIBufferCom = CVIBufferRect::Create(m_pGraphicDevice, &tSizePos);
	/*if (FAILED(Add_Component(LEVEL_STATIC, L"PrototypeVIBufferRect", L"ComVIBuffer", (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CRectObj SetUp_Component - Add_Component PrototypeVIBufferRect");*/

	return S_OK;
}

void CRectObj::Set_RectVIBufferSize(LPDIRECT3DDEVICE9 pGraphic_Device, CVIBufferRect::SIZE_POS* tSizePos)
{
	if (m_pVIBufferCom != nullptr)
		Safe_Release(m_pVIBufferCom);

	if (m_pGraphicDevice == nullptr)
		m_pGraphicDevice = pGraphic_Device;

	m_pVIBufferCom = CVIBufferRect::Create(m_pGraphicDevice, tSizePos);
}

void CRectObj::Update_BoxColliderValues(OBJ_INFO * pInfo)
{
	if (m_pBoxCollider != nullptr)
		Safe_Release(m_pBoxCollider);

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, pInfo->vColliderCenter, pInfo->vColliderSize, pInfo->dwColliderColor, this);
}

void CRectObj::ReSet_Rotation()
{
	if (m_pTransformCom == nullptr)
		MSG_VOID("CRectObj ReSet_Rotation - Point Check");

	m_pTransformCom->ReSet_Rotation();
}

void CRectObj::Set_Rotation(_int iValueX, _int iValueY, _int iValueZ)
{
	if (m_pTransformCom == nullptr)
		MSG_VOID("CRectObj Set_RotationX - Point Check");

	m_pTransformCom->Prototype_Rotation(iValueX, iValueY, iValueZ);
}

HRESULT CRectObj::Set_Matrix(_mat mat)
{
	if (FAILED(__super::Set_Matrix(mat)))
		MSG_FAIL("CRectObj::Set_Matrix - __super::Set_Matrix");

	m_pTransformCom->Set_Matrix(mat);

	return S_OK;
}

_uint CRectObj::Get_ImageCount()
{
	if (nullptr == m_pTextureCom)
		MSG_MINUSONE("Failed To CRectObj Get_ImageCount - Pointer Check");

	return m_pTextureCom->Get_ImageCount();
}

void CRectObj::Free()
{
	__super::Free();
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}

_vec3 CRectObj::Get_MinPos()
{
	return _vec3();
}

_vec3 CRectObj::Get_MaxPos()
{
	return _vec3();
}

_vec3 CRectObj::Get_WorldPos()
{
	return _vec3();
}

_vec3 CRectObj::Get_Length()
{
	return _vec3();
}

CGameObject * CRectObj::Clone(void * pArg /*= nullptr*/)
{
	// ¾È¾¸


	//CGameObject* pInstance = new CRectObj(*this);

	//if (FAILED(pInstance->NativeConstruct(pArg)))
	//{
	//	Safe_Release(pInstance);
	//	MSGBOX("CRectObj Create Failed");
	//}

	return nullptr;
}

CRectObj * CRectObj::Create(_pDevice pDevice, OBJ_INFO* pInfo)
{
	CRectObj* pInstance = new CRectObj(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype(pInfo)))
	{
		Safe_Release(pInstance);
		MSGBOX("CRectObj Create Failed");
	}

	return pInstance;
}
