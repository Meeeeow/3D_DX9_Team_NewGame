#include "stdafx.h"
#include "..\public\BossSkeletonStageCircle.h"
#include "Player.h"
#include "PlayerTagMgr.h"
#include "Attack.h"
#include "DmgFont.h"
#include "BossSkeletonStageCircleAttack.h"


CBossSkeletonStageCircle::CBossSkeletonStageCircle(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CEffect(pGraphicDevice)
{
}

CBossSkeletonStageCircle::CBossSkeletonStageCircle(const CBossSkeletonStageCircle & rhs)
	: CEffect(rhs)
{
}

HRESULT CBossSkeletonStageCircle::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBossSkeletonStageCircle NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CBossSkeletonStageCircle::NativeConstruct(void * pArg)
{
	DESC tDesc = *(DESC*)(pArg);

	if (FAILED(__super::NativeConstruct(&tDesc)))
		MSG_FAIL("CBossSkeletonStageCircle NativeConstruct - __super");

	if (FAILED(SetUp_Component(&tDesc)))
		MSG_FAIL("CBossSkeletonStageCircle::NativeConstruct - SetUp_Component");

	//파동에 맞게 공격 범위 설정해줘야함
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CBossSkeletonStageCircleAttack::DESC tAttackDesc;
	tAttackDesc.eType = CBossSkeletonStageCircleAttack::TYPE::TYPE_LEFT;
	tAttackDesc.fSpeed = 2.f;
	tAttackDesc.Set_Parent(this);
	tAttackDesc.tSuperDesc.iDmg = 10;
	tAttackDesc.tSuperDesc.vPos = m_pTransform->Get_State(CTransform::STATE_POS) + _vec3(-0.5f, 0.f, 0.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC
		, pTagPrototypeBossSkeletonStageCircleAttack, pTagLayerMonsterAttack, &tAttackDesc)))
		MSG_FAIL("CBossSkeletonStageCircle::NativeConstruct - pTagPrototypeBossSkeletonStageCircleAttack");

	tAttackDesc.eType = CBossSkeletonStageCircleAttack::TYPE::TYPE_RIGHT;
	tAttackDesc.tSuperDesc.vPos = m_pTransform->Get_State(CTransform::STATE_POS) + _vec3(0.5f, 0.f, 0.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC
		, pTagPrototypeBossSkeletonStageCircleAttack, pTagLayerMonsterAttack, &tAttackDesc)))
		MSG_FAIL("CBossSkeletonStageCircle::NativeConstruct - pTagPrototypeBossSkeletonStageCircleAttack");

	tAttackDesc.eType = CBossSkeletonStageCircleAttack::TYPE::TYPE_FORWARD;
	tAttackDesc.tSuperDesc.vPos = m_pTransform->Get_State(CTransform::STATE_POS) + _vec3(0.f, 0.f, 0.5f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC
		, pTagPrototypeBossSkeletonStageCircleAttack, pTagLayerMonsterAttack, &tAttackDesc)))
		MSG_FAIL("CBossSkeletonStageCircle::NativeConstruct - pTagPrototypeBossSkeletonStageCircleAttack");

	tAttackDesc.eType = CBossSkeletonStageCircleAttack::TYPE::TYPE_BACK;
	tAttackDesc.tSuperDesc.vPos = m_pTransform->Get_State(CTransform::STATE_POS) + _vec3(0.f, 0.f, -0.5f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC
		, pTagPrototypeBossSkeletonStageCircleAttack, pTagLayerMonsterAttack, &tAttackDesc)))
		MSG_FAIL("CBossSkeletonStageCircle::NativeConstruct - pTagPrototypeBossSkeletonStageCircleAttack");

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_int CBossSkeletonStageCircle::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CBossSkeletonStageCircle Tick - __super");

	if (m_bDead)
		return OBJ_DEAD;

	m_vScale += _vec3(0.1f, 0.1f, 0.1f);
	if (m_vScale.x >= 10.f)
		Set_Dead();

	m_pTransform->SetUp_Scale(m_vScale);

	return 0;
}

_int CBossSkeletonStageCircle::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CBossSkeletonStageCircle LateTick - __super");

	if (m_pRenderer == nullptr)
		MSG_MINUSONE("CExplosion::LateTick - m_pRendererCom == nullptr");

	Compute_Distance(m_pTransform);

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_ALPHA, this)))
		MSG_MINUSONE("CExplosion LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CBossSkeletonStageCircle::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CBossSkeletonStageCircle Render - __super");

	if (m_pTransform == nullptr ||
		m_pTexture == nullptr ||
		m_pVIBuffer == nullptr)
		MSG_FAIL("CBossSkeletonStageCircle::Render - Com is null");

	if (FAILED(m_pTransform->Bind_OnDevice()))
		MSG_FAIL("CBossSkeletonStageCircle::Render - m_pTransform->Bind_OnDevice");

	if (FAILED(m_pTexture->Bind_OnDevice(Get_Frame())))
		MSG_FAIL("CBossSkeletonStageCircle::Render - m_pTexture->Bind_OnDevice");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CBossSkeletonStageCircle::Render - Set_RenderState");

	m_pVIBuffer->Change_Color(D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	if (FAILED(m_pVIBuffer->Render()))
		MSG_FAIL("CBossSkeletonStageCircle::Render - m_pVIBuffer->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CBossSkeletonStageCircle::Render - Release_RenderState");

	return S_OK;
}

HRESULT CBossSkeletonStageCircle::SetUp_Component(void * pArg)
{
	DESC tDesc = *(DESC*)(pArg);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform)))
		MSG_FAIL("CBossSkeletonStageCircle::SetUp_Component - pTagPrototypeTransform");
	m_pTransform->Set_Matrix(tDesc.tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CBossSkeletonStageCircle::SetUp_Component - pTagPrototypeRenderer");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRectColor, pTagComVIBuffer, (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CBossSkeletonStageCircle::SetUp_Component - pTagPrototypeVIBufferRectColor");
	m_pVIBuffer->Change_Color(D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	if (FAILED(Add_Component(tDesc.eTextureLevel, pTagPrototypeTextureBossSkeletonStageCircle, pTagComTexture, (CComponent**)&m_pTexture)))
		MSG_FAIL("CBossSkeletonStageCircle::SetUp_Component - pTagPrototypeTextureBossSkeletonStageCircle");

	return S_OK;
}

HRESULT CBossSkeletonStageCircle::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBossSkeletonStageCircle::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
		MSG_FAIL("CBossSkeletonStageCircle::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)))
		MSG_FAIL("CBossSkeletonStageCircle::Set_RenderState - SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)))
		MSG_FAIL("CBossSkeletonStageCircle::Set_RenderState - SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)");

	return S_OK;
}

HRESULT CBossSkeletonStageCircle::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CBossSkeletonStageCircle::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
		MSG_FAIL("CBossSkeletonStageCircle::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

CBossSkeletonStageCircle * CBossSkeletonStageCircle::Create(_pDevice pDevice)
{
	CBossSkeletonStageCircle* pInstance = new CBossSkeletonStageCircle(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CBossSkeletonStageCircle Create Failed");
	}

	return pInstance;
}

void CBossSkeletonStageCircle::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	
	__super::Free();
}

CGameObject * CBossSkeletonStageCircle::Clone(void * pArg)
{
	CGameObject* pInstance = new CBossSkeletonStageCircle(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CBossSkeletonStageCircle Clone Failed");
	}

	return pInstance;
}