#include "stdafx.h"
#include "..\public\BossSkeletonMagicCircle.h"
#include "GameInstance.h"
#include "VIBufferRectColor.h"
#include "BossSkeletonMagicCircleIn.h"

void CBossSkeletonMagicCircle::Free()
{
	__super::Free();

	//프로토타입 제외
	if (m_pIn != nullptr)
		m_pIn->Set_Dead();
	Safe_Release(m_pIn);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}

CGameObject * CBossSkeletonMagicCircle::Clone(void * pArg /*= nullptr*/)
{
	CGameObject* pInstance = new CBossSkeletonMagicCircle(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CBossSkeletonMagicCircle Clone Failed");
	}

	return pInstance;
}

CBossSkeletonMagicCircle * CBossSkeletonMagicCircle::Create(_pDevice pDevice)
{
	CBossSkeletonMagicCircle* pInstance = new CBossSkeletonMagicCircle(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CBossSkeletonMagicCircle Create Failed");
	}

	return pInstance;
}

_vec3 CBossSkeletonMagicCircle::Get_MinPos()
{
	return _vec3();
}

_vec3 CBossSkeletonMagicCircle::Get_MaxPos()
{
	return _vec3();
}

_vec3 CBossSkeletonMagicCircle::Get_WorldPos()
{
	return _vec3();
}

_vec3 CBossSkeletonMagicCircle::Get_Length()
{
	return _vec3();
}

CBossSkeletonMagicCircle::CBossSkeletonMagicCircle(_pDevice pDevice)
	:CEffect(pDevice)
{
}

CBossSkeletonMagicCircle::CBossSkeletonMagicCircle(const CBossSkeletonMagicCircle & rhs)
	: CEffect(rhs)
{
}

HRESULT CBossSkeletonMagicCircle::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBossSkeletonMagicCircle NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CBossSkeletonMagicCircle::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CBossSkeletonMagicCircle NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CBossSkeletonMagicCircle NativeConstruct - SetUp_Component");

	PLAY_SOUND(L"BossSkeletonMagicCircle.ogg", CSoundMgr::BOSS_SKELETON);

	return S_OK;
}

_int CBossSkeletonMagicCircle::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta, true)))
		MSG_MINUSONE("CBossSkeletonMagicCircle Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	return _int();
}

_int CBossSkeletonMagicCircle::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CBossSkeletonMagicCircle LateTick - __super");

	if (m_pRendererCom == nullptr)
		MSG_MINUSONE("CBossSkeletonMagicCircle::LateTick - m_pRendererCom == nullptr");

	BillBoarding(m_pTransformCom);

	Compute_Distance(m_pTransformCom);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_ALPHA, this)))
		MSG_MINUSONE("CBossSkeletonMagicCircle LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CBossSkeletonMagicCircle::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CBossSkeletonMagicCircle Render - __super");

	if (m_pRendererCom == nullptr)
		MSG_FAIL("CBossSkeletonMagicCircle Render - m_pRendererCom is null");

	if (m_pVIBufferCom == nullptr)
		MSG_FAIL("CBossSkeletonMagicCircle Render - m_pVIBufferCom is null");

	if (m_pTextureCom == nullptr)
		MSG_FAIL("CBossSkeletonMagicCircle Render - m_pTextureCom is null");

	if (FAILED(m_pTransformCom->Bind_OnDevice()))
		MSG_FAIL("CBossSkeletonMagicCircle Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTextureCom->Bind_OnDevice(_uint(Get_Frame()))))
		MSG_FAIL("CBossSkeletonMagicCircle Render - m_pTextureCom Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CBossSkeletonMagicCircle Render - Set_RenderState");

	if (FAILED(m_pVIBufferCom->Render()))
		MSG_FAIL("CBossSkeletonMagicCircle Render - m_pVIBufferCom Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CBossSkeletonMagicCircle Render - Release_RenderState");

	return S_OK;
}

HRESULT CBossSkeletonMagicCircle::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBossSkeletonMagicCircle Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
		MSG_FAIL("CBossSkeletonMagicCircle::Set_RenderState - SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)))
		MSG_FAIL("CBossSkeletonMagicCircle::Set_RenderState - SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)))
		MSG_FAIL("CBossSkeletonMagicCircle::Set_RenderState - SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)");

	return S_OK;
}

HRESULT CBossSkeletonMagicCircle::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBossSkeletonMagicCircle Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
		MSG_FAIL("CBossSkeletonMagicCircle::Set_RenderState - SetRenderState");

	return S_OK;
}

HRESULT CBossSkeletonMagicCircle::SetUp_Component(void* pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//트랜스폼 세팅하고 넘겨받은 좌표, 스케일 적용
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransformCom)))
		MSG_FAIL("CBossSkeletonMagicCircle SetUp_Component - Add_Component m_pTransformCom");

	if (pArg == nullptr)
		MSG_FAIL("CBossSkeletonMagicCircle SetUp_Component - pArg == nullptr");
	DESC tDesc = *(DESC*)(pArg);
	m_pTransformCom->SetUp_Scale(tDesc.vScale);
	m_pTransformCom->SetUp_State(CTransform::STATE_POS, tDesc.vPos);

	switch (tDesc.eType)
	{
	case TYPE_RED:
		if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferRectColorRed"), TEXT("ComVIBuffer"), (CComponent**)&m_pVIBufferCom)))
			MSG_FAIL("CBossSkeletonMagicCircle SetUp_Component - Add_Component PrototypeVIBufferRectColorRed");
		break;
	case TYPE_GREEN:
		if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferRectColorGreen"), TEXT("ComVIBuffer"), (CComponent**)&m_pVIBufferCom)))
			MSG_FAIL("CBossSkeletonMagicCircle SetUp_Component - Add_Component PrototypeVIBufferRectColorGreen");
		break;
	case TYPE_BLUE:
		if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferRectColorBlue"), TEXT("ComVIBuffer"), (CComponent**)&m_pVIBufferCom)))
			MSG_FAIL("CBossSkeletonMagicCircle SetUp_Component - Add_Component PrototypeVIBufferRectColorBlue");
		break;
	}

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRendererCom)))
		MSG_FAIL("CBossSkeletonMagicCircle SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_BOSS_SKELETON, TEXT("PrototypeTextureBossSkeletonMagicCircle"), pTagComTexture, (CComponent**)&m_pTextureCom)))
		MSG_FAIL("CBossSkeletonMagicCircle SetUp_Component - Add_Component m_pTextureCom");

	Set_FrameValue(13);

	//원 가운데 이펙트 생성
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, L"PrototypeBossSkeletonMagicCircleIn", L"LayerEffect", &tDesc, (void**)(&m_pIn))))
		MSG_FAIL("CBossSkeletonMagicCircle::SetUp_Component - Add_GameObjectToLayer PrototypeBossSkeletonMagicCircleIn");
	Safe_AddRef(m_pIn);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}
