#include "stdafx.h"
#include "..\public\BaseBlock.h"


CBaseBlock::CBaseBlock(_pDevice pGraphicDevice)
	: CGameObject(pGraphicDevice)
{
}

CBaseBlock::CBaseBlock(const CBaseBlock & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBaseBlock::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBaseBlock NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CBaseBlock::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CBaseBlock NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CBaseBlock NativeConstruct - SetUp_Component");

	return S_OK;
}

_int CBaseBlock::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CBaseBlock Tick - __super");

	return OBJ_NOEVENT;
}

_int CBaseBlock::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CBaseBlock LateTick - __super");

	if (m_pRendererCom == nullptr)
		MSG_MINUSONE("CBaseBlock::LateTick - m_pRendererCom == nullptr");

	if (m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this))
		MSG_MINUSONE("CBaseBlock::LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CBaseBlock::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CBaseBlock Render - __super");

	if (m_pTransformCom == nullptr ||
		m_pTextureCom == nullptr ||
		m_pVIBufferCom == nullptr)
		MSG_FAIL("CBaseBlock::Render - Com is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CBaseBlock::Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureCom->Bind_OnDevice()))
		MSG_FAIL("CBaseBlock::Render - m_pTextureCom->Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CBaseBlock::Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CBaseBlock::Render - m_pVIBufferCom->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CBaseBlock::Render - Release_RenderState");

	return S_OK;
}

HRESULT CBaseBlock::SetUp_Component(void * pArg)
{
	OBJ_INFO tObjInfo = *(OBJ_INFO*)(pArg);

	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.fSpeed = 0.f;
	tTransformDesc.fDegree = 3.f;

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransformCom, &tTransformDesc)))
		MSG_FAIL("CBaseBlock::SetUp_Component - m_pTransformCom");
	m_pTransformCom->Set_Matrix(tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CBaseBlock::SetUp_Component - m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferCube, pTagComVIBuffer, (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CBaseBlock::SetUp_Component - m_pVIBufferCom");

	// TombStoneBase
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureTombStoneBase"), L"ComTexture", (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CBaseBlock::SetUp_Component - PrototypeTextureTombStoneBase");

	return S_OK;
}

HRESULT CBaseBlock::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBaseBlock::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CBaseBlock::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CBaseBlock::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CBaseBlock::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

HRESULT CBaseBlock::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBaseBlock::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CBaseBlock::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

void CBaseBlock::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

CBaseBlock * CBaseBlock::Create(_pDevice pDevice)
{
	CBaseBlock* pInstance = new CBaseBlock(pDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CBaseBlock Create - NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBaseBlock::Clone(void * pArg)
{
	CGameObject* pInstance = new CBaseBlock(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBaseBlock Clone - NativeConstruct");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_vec3 CBaseBlock::Get_MinPos()
{
	return _vec3();
}

_vec3 CBaseBlock::Get_MaxPos()
{
	return _vec3();
}

_vec3 CBaseBlock::Get_WorldPos()
{
	return _vec3();
}

_vec3 CBaseBlock::Get_Length()
{
	return _vec3();
}
