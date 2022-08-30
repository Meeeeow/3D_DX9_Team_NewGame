#include "stdafx.h"
#include "RectYZObj.h"
#include "GameInstance.h"


CRectYZObj::CRectYZObj(_pDevice pGraphicDevice)
	: CToolObj(pGraphicDevice)
{
}

HRESULT CRectYZObj::NativeConstruct_Prototype(void * pArg)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CRectYZObj NativeConstruct_Prototype - __super");

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CRectYZObj NativeConstruct - SetUp_Component");

	m_pTransformCom->SetUp_State(CTransform::STATE::STATE_POS, _vec3(0.f, 0.f, 0.f));
	m_pTransformCom->SetUp_Scale(1.f, 1.f, 1.f);
	m_pBoxCollider = CBoxCollider::Create(m_pGraphicDevice);
	m_pVIBufferCom = CVIBufferRectYZ::Create(m_pGraphicDevice);

	return S_OK;
}

HRESULT CRectYZObj::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CRectYZObj NativeConstruct - __super");

	return S_OK;
}

_int CRectYZObj::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CRectYZObj Tick - __super");

	return _int();
}

_int CRectYZObj::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CRectYZObj LateTick - __super");

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_NONALPHA, this)))
		MSG_MINUSONE("CRectYZObj LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CRectYZObj::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CRectYZObj Render - __super");

	if (m_pTransformCom == nullptr
		|| m_pTextureCom == nullptr
		|| m_pVIBufferCom == nullptr)
		MSG_FAIL("CRectYZObj Render - m_pTransform m_pTexture m_pVIBuffer is null");

	m_pBoxCollider->Set_WorldMatrixFromObject(*m_pTransformCom->Get_WorldMatrixPtr());
	m_pBoxCollider->Tick(0.1f);

	m_pTransformCom->Bind_OnDevice();
	m_pTextureCom->Bind_OnDevice(m_iTextureIndex);
	if(m_bWireFrameRender)
		m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pVIBufferCom->Render();

	if(m_bWireFrameRender)
		m_pGraphicDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	if (m_pBoxCollider != nullptr)
		m_pBoxCollider->Render();

	return S_OK;
}

HRESULT CRectYZObj::Set_RectYZBuffer(OBJ_INFO * pInfo)
{
	if (m_pVIBufferCom != nullptr)
		Safe_Release(m_pVIBufferCom);
	if (m_pBoxCollider != nullptr)
		Safe_Release(m_pBoxCollider);

	Set_ObjInfo(pInfo);
	m_iTextureIndex = pInfo->iTextureIndex;

	m_pVIBufferCom = CVIBufferRectYZ::Create(m_pGraphicDevice, pInfo->eWallSide, pInfo->fSizeY, pInfo->fSizeZ,
		pInfo->fU, pInfo->fV, pInfo->iNumVerticesY, pInfo->iNumVerticesZ);
	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, pInfo->vColliderCenter, pInfo->vColliderSize,pInfo->dwColliderColor, this);

	m_pTransformCom->Set_Matrix(pInfo->matWorld);

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CRectYZObj Set_RectYZBuffer - m_pVIBufferCom");
	if (m_pBoxCollider == nullptr)
		MSG_FAIL("CRectYZObj Set_RectYZBuffer - m_pBoxCollider");

	return S_OK;
}

HRESULT CRectYZObj::Set_Matrix(_mat mat)
{
	__super::Set_Matrix(mat);
	m_pTransformCom->Set_Matrix(mat);
	return S_OK;
}


HRESULT CRectYZObj::SetUp_Component()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransformCom)))
		MSG_FAIL("CRectYZObj SetUp_Component - Add_Component m_pTransformCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CRectYZObj SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTextureWall"), TEXT("ComTextureWall"), (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CRectYZObj SetUp_Component - Add_Component m_pTextureCom");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CRectYZObj::Free()
{
	__super::Free();

	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}

CRectYZObj * CRectYZObj::Create(_pDevice pDevice)
{
	CRectYZObj* pInstance = new CRectYZObj(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CRectObj Create Failed");
	}

	return pInstance;
}

_vec3 CRectYZObj::Get_MinPos()
{
	return _vec3();
}

_vec3 CRectYZObj::Get_MaxPos()
{
	return _vec3();
}

_vec3 CRectYZObj::Get_WorldPos()
{
	return _vec3();
}

_vec3 CRectYZObj::Get_Length()
{
	return _vec3();
}
