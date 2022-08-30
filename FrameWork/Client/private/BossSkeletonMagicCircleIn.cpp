#include "stdafx.h"
#include "..\public\BossSkeletonMagicCircleIn.h"
#include "GameInstance.h"
#include "VIBufferRectColor.h"

void CBossSkeletonMagicCircleIn::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}

CGameObject * CBossSkeletonMagicCircleIn::Clone(void * pArg /*= nullptr*/)
{
	CGameObject* pInstance = new CBossSkeletonMagicCircleIn(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CBossSkeletonMagicCircleIn Clone Failed");
	}

	return pInstance;
}

CBossSkeletonMagicCircleIn * CBossSkeletonMagicCircleIn::Create(_pDevice pDevice)
{
	CBossSkeletonMagicCircleIn* pInstance = new CBossSkeletonMagicCircleIn(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CBossSkeletonMagicCircleIn Create Failed");
	}

	return pInstance;
}

_vec3 CBossSkeletonMagicCircleIn::Get_MinPos()
{
	return _vec3();
}

_vec3 CBossSkeletonMagicCircleIn::Get_MaxPos()
{
	return _vec3();
}

_vec3 CBossSkeletonMagicCircleIn::Get_WorldPos()
{
	return _vec3();
}

_vec3 CBossSkeletonMagicCircleIn::Get_Length()
{
	return _vec3();
}

CBossSkeletonMagicCircleIn::CBossSkeletonMagicCircleIn(_pDevice pDevice)
	:CEffect(pDevice)
{
}

CBossSkeletonMagicCircleIn::CBossSkeletonMagicCircleIn(const CBossSkeletonMagicCircleIn & rhs)
	: CEffect(rhs)
{
}

HRESULT CBossSkeletonMagicCircleIn::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBossSkeletonMagicCircleIn NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CBossSkeletonMagicCircleIn::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CBossSkeletonMagicCircleIn NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CBossSkeletonMagicCircleIn NativeConstruct - SetUp_Component");

	return S_OK;
}

_int CBossSkeletonMagicCircleIn::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CBossSkeletonMagicCircleIn Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	m_dwColor.a += 0.f;
	if (m_dwColor.a >= 1.f)
		m_dwColor.a = 0.f;

	m_pVIBufferCom->Change_Color(m_dwColor);

	return _int();
}

_int CBossSkeletonMagicCircleIn::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CBossSkeletonMagicCircleIn LateTick - __super");

	if (m_pRendererCom == nullptr)
		MSG_MINUSONE("CBossSkeletonMagicCircleIn::LateTick - m_pRendererCom == nullptr");

	BillBoarding(m_pTransformCom);

	Compute_Distance(m_pTransformCom);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_ALPHA, this)))
		MSG_MINUSONE("CBossSkeletonMagicCircleIn LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CBossSkeletonMagicCircleIn::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CBossSkeletonMagicCircleIn Render - __super");

	if (m_pRendererCom == nullptr)
		MSG_FAIL("CBossSkeletonMagicCircleIn Render - m_pRendererCom is null");

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CBossSkeletonMagicCircleIn Render - m_pVIBufferCom is null");

	if (m_pTextureCom == nullptr)
		MSG_FAIL("CBossSkeletonMagicCircleIn Render - m_pTextureCom is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CBossSkeletonMagicCircleIn Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureCom->Bind_OnDevice(_uint(Get_Frame()))))
		MSG_FAIL("CBossSkeletonMagicCircleIn Render - m_pTextureCom Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CBossSkeletonMagicCircleIn Render - Set_RenderState");

	m_pVIBufferCom->Change_Color(D3DXCOLOR(0.f, 0.f, 0.f, 0.f));
	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CBossSkeletonMagicCircleIn Render - m_pVIBufferCom Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CBossSkeletonMagicCircleIn Render - Release_RenderState");

	return S_OK;
}

HRESULT CBossSkeletonMagicCircleIn::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBossSkeletonMagicCircleIn Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
		MSG_FAIL("CBossSkeletonMagicCircleIn::Set_RenderState - SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)))
		MSG_FAIL("CBossSkeletonMagicCircleIn::Set_RenderState - SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)))
		MSG_FAIL("CBossSkeletonMagicCircleIn::Set_RenderState - SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)");

	return S_OK;
}

HRESULT CBossSkeletonMagicCircleIn::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBossSkeletonMagicCircleIn Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
		MSG_FAIL("CBossSkeletonMagicCircleIn::Set_RenderState - SetRenderState");

	return S_OK;
}

HRESULT CBossSkeletonMagicCircleIn::SetUp_Component(void* pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//트랜스폼 세팅하고 넘겨받은 좌표, 스케일 적용
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransformCom)))
		MSG_FAIL("CBossSkeletonMagicCircleIn SetUp_Component - Add_Component m_pTransformCom");

	if (pArg == nullptr)
		MSG_FAIL("CBossSkeletonMagicCircleIn SetUp_Component - pArg == nullptr");
	DESC tDesc = *(DESC*)(pArg);
	m_pTransformCom->SetUp_Scale(tDesc.vScale);
	m_pTransformCom->SetUp_State(CTransform::STATE_POS, tDesc.vPos);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferRectColor"), TEXT("ComVIBuffer"), (CComponent**)&m_pVIBufferCom)))
		MSG_FAIL("CBossSkeletonMagicCircleIn SetUp_Component - Add_Component m_pVIBufferCom");

	switch (tDesc.eType)
	{
	case TYPE_RED:
		m_dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 0.f);
		m_pVIBufferCom->Change_Color(m_dwColor);
		break;
	case TYPE_GREEN:
		m_dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 0.f);
		m_pVIBufferCom->Change_Color(m_dwColor);
		break;
	case TYPE_BLUE:
		m_dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 0.f);
		m_pVIBufferCom->Change_Color(m_dwColor);
		break;
	}

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CBossSkeletonMagicCircleIn SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_BOSS_SKELETON, TEXT("PrototypeTextureBossSkeletonMagicCircleIn"), pTagComTexture, (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CBossSkeletonMagicCircleIn SetUp_Component - Add_Component m_pTextureCom");

	Set_FrameValue(1);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}
