#include "stdafx.h"
#include "..\public\CrossBlock.h"


CCrossBlock::CCrossBlock(_pDevice pGraphicDevice)
	: CGameObject(pGraphicDevice)
{
}

CCrossBlock::CCrossBlock(const CCrossBlock & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCrossBlock::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CCrossBlock NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CCrossBlock::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CCrossBlock NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CCrossBlock NativeConstruct - SetUp_Component");

	return S_OK;
}

_int CCrossBlock::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CCrossBlock Tick - __super");

	return OBJ_NOEVENT;
}

_int CCrossBlock::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CCrossBlock LateTick - __super");

	if (m_pRendererCom == nullptr)
		MSG_MINUSONE("CCrossBlock::LateTick - m_pRendererCom == nullptr");

	if (m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this))
		MSG_MINUSONE("CCrossBlock::LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CCrossBlock::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CCrossBlock Render - __super");

	if (m_pTransformCom == nullptr ||
		m_pTextureCom	== nullptr ||
		m_pVIBufferCom	== nullptr)
		MSG_FAIL("CCrossBlock::Render - Com is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CCrossBlock::Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureCom->Bind_OnDevice()))
		MSG_FAIL("CCrossBlock::Render - m_pTextureCom->Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CCrossBlock::Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CCrossBlock::Render - m_pVIBufferCom->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CCrossBlock::Render - Release_RenderState");

	return S_OK;
}

HRESULT CCrossBlock::SetUp_Component(void * pArg)
{
	OBJ_INFO tObjInfo = *(OBJ_INFO*)(pArg);

	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.fSpeed = 0.f;
	tTransformDesc.fDegree = 3.f;

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransformCom, &tTransformDesc)))
		MSG_FAIL("CCrossBlock::SetUp_Component - m_pTransformCom");
	m_pTransformCom->Set_Matrix(tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CCrossBlock::SetUp_Component - m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferCube, pTagComVIBuffer, (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CCrossBlock::SetUp_Component - m_pVIBufferCom");

	// TombStoneCross
	if (FAILED(Add_Component(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureTombStoneCross"), L"ComTexture", (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CCrossBlock::SetUp_Component - PrototypeTextureTombStoneCross");

	return S_OK;
}

HRESULT CCrossBlock::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CCrossBlock::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CCrossBlock::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, 150)))
		MSG_FAIL("CCrossBlock::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CCrossBlock::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

HRESULT CCrossBlock::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CCrossBlock::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CCrossBlock::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

void CCrossBlock::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}

CCrossBlock * CCrossBlock::Create(_pDevice pDevice)
{
	CCrossBlock* pInstance = new CCrossBlock(pDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CCrossBlock Create - NativeConstruct_Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCrossBlock::Clone(void * pArg)
{
	CGameObject* pInstance = new CCrossBlock(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CCrossBlock Clone - NativeConstruct");
		Safe_Release(pInstance);
	}

	return pInstance;
}

_vec3 CCrossBlock::Get_MinPos()
{
	return _vec3();
}

_vec3 CCrossBlock::Get_MaxPos()
{
	return _vec3();
}

_vec3 CCrossBlock::Get_WorldPos()
{
	return _vec3();
}

_vec3 CCrossBlock::Get_Length()
{
	return _vec3();
}
