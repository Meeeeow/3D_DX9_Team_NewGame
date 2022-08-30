#include "stdafx.h"
#include "CubeObj.h"
#include "GameInstance.h"

CCubeObj::CCubeObj(_pDevice pDevice)
	:CToolObj(pDevice)
{
}

HRESULT CCubeObj::NativeConstruct_Prototype(OBJ_INFO * pInfo)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CCubeObj NativeConstruct_Prototype - __super");

	if (FAILED(Set_ObjInfo(pInfo)))
		MSG_FAIL("CCubeObj NativeConstruct_Prototype - Set_ObjInfo");

	if (FAILED(SetUp_Component(pInfo)))
		MSG_FAIL("CRectObj NativeConstruct - SetUp_Component");

	m_iTextureNum = pInfo->iTextureIndex;

	Update_BoxColliderValues(pInfo);

	Set_Matrix(pInfo->matWorld);
	
	
	return S_OK;
}

HRESULT CCubeObj::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CCubeObj Render - __super");

	if (m_pTransformCom == nullptr
		|| m_pTextureCom == nullptr
		|| m_pVIBufferCom == nullptr)
		MSG_FAIL("CCubeObj Render - m_pTransform m_pTexture m_pVIBuffer is null");

	m_pBoxCollider->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxCollider->Tick(0.1f);

	m_pTransformCom->Bind_OnDevice();
	m_pTextureCom->Bind_OnDevice(m_iTextureNum);
	m_pVIBufferCom->Render();

	if (m_pBoxCollider != nullptr)
		m_pBoxCollider->Render();

	return S_OK;
}

_int CCubeObj::LateTick(_float fTimeDelta)
{
	if (0 > __super::LateTick(fTimeDelta))
		return -1;

	m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this);

	return _int();
}

HRESULT CCubeObj::SetUp_Component(OBJ_INFO* pInfo)
{
	if (FAILED(Add_Component(LEVEL_STATIC, pInfo->szTextureName, L"ComTexture", (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CCubeObj SetUp_Component - Add_Component m_pTexture");

	if (FAILED(Add_Component(LEVEL_STATIC, L"PrototypeTransform", L"ComTransform", (CComponent**)&m_pTransformCom)))
		MSG_FAIL("CCubeObj SetUp_Component - Add_Component m_pTransform");

	if (FAILED(Add_Component(LEVEL_STATIC, L"PrototypeRenderer", L"ComRenderer", (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CCubeObj SetUp_Component - Add_Component m_pRenderer");

	if (FAILED(Add_Component(LEVEL_STATIC, L"PrototypeVIBufferCube", L"ComVIBuffer", (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CCubeObj SetUp_Component - Add_Component m_pVIBuffer");

	return S_OK;
}

void CCubeObj::Set_RectVIBufferSize(LPDIRECT3DDEVICE9 pGraphic_Device, _vec3 * pSize)
{
	if (m_pVIBufferCom != nullptr)
		Safe_Release(m_pVIBufferCom);

	if (m_pGraphicDevice == nullptr)
		m_pGraphicDevice = pGraphic_Device;

	m_pVIBufferCom = CVIBufferCube::Create(m_pGraphicDevice, pSize);
}

void CCubeObj::Update_BoxColliderValues(OBJ_INFO * pInfo)
{
	if (m_pBoxCollider != nullptr)
		Safe_Release(m_pBoxCollider);

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, pInfo->vColliderCenter, pInfo->vColliderSize, pInfo->dwColliderColor, this);
}

void CCubeObj::ReSet_Rotation()
{
	if (m_pTransformCom == nullptr)
		MSG_VOID("CRectObj ReSet_Rotation - Point Check");

	m_pTransformCom->ReSet_Rotation();
}

void CCubeObj::Set_Rotation(_int iValueX, _int iValueY, _int iValueZ)
{
	if (m_pTransformCom == nullptr)
		MSG_VOID("CRectObj Set_RotationX - Point Check");

	m_pTransformCom->Prototype_Rotation(iValueX, iValueY, iValueZ);
}

HRESULT CCubeObj::Set_Matrix(_mat mat)
{
	__super::Set_Matrix(mat);

	m_pTransformCom->Set_Matrix(mat);

	return S_OK;
}

_uint CCubeObj::Get_ImageCount()
{
	if (nullptr == m_pTextureCom)
		MSG_MINUSONE("Failed To CRectObj Get_ImageCount - Pointer Check");

	return m_pTextureCom->Get_ImageCount();
}

CCubeObj * CCubeObj::Create(_pDevice pDevice, OBJ_INFO * pInfo)
{
	CCubeObj* pInstance = new CCubeObj(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype(pInfo)))
	{
		Safe_Release(pInstance);
		MSGBOX("CCubeObj Create Failed");
	}

	return pInstance;
}

CGameObject * CCubeObj::Clone(void * pArg)
{
	// ¾È¾¸
	CGameObject* pInstance = new CCubeObj(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CRectObj Create Failed");
	}

	return pInstance;
}

void CCubeObj::Free()
{
	__super::Free();
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}

_vec3 CCubeObj::Get_MinPos()
{
	return _vec3();
}

_vec3 CCubeObj::Get_MaxPos()
{
	return _vec3();
}

_vec3 CCubeObj::Get_WorldPos()
{
	return _vec3();
}

_vec3 CCubeObj::Get_Length()
{
	return _vec3();
}
