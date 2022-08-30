#include "stdafx.h"
#include "..\public\Attack.h"


CAttack::CAttack(_pDevice pDevice)
	: CGameObject(pDevice)
{
}

CAttack::CAttack(const CAttack & rhs)
	:CGameObject(rhs)
{
}

HRESULT CAttack::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CAttack NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CAttack::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CAttack NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CAttack::NativeConstruct - SetUp_Component");

	if (FAILED(m_pBoxCollider->Tick_Mat(*m_pTransform->Get_WorldMatrixPtr())))
		MSG_FAIL("CAttack::NativeConstruct - m_pBoxCollider->Tick_Mat");
	
	return S_OK;
}

_int CAttack::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CAttack Tick - __super");

	if (m_iDead > 1)
		Set_Dead();

	if (m_bDead)
		return OBJ_DEAD;

	Tick_Dead();

	return 0;
}

_int CAttack::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CAttack LateTick - __super");

	if (m_pRenderer == nullptr)
		MSG_MINUSONE("CAttack::LateTick - m_pRenderer == nullptr");

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this)))
		MSG_MINUSONE("CAttack::LateTick - m_pRenderer->Add_RenderGroup");

	return _int();
}

HRESULT CAttack::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CAttack Render - __super");

	if (m_pTransform == nullptr ||
		m_pBoxCollider == nullptr)
		MSG_FAIL("CAttack Render - com is null");

	if (FAILED(m_pTransform->Bind_OnDevice()))
		MSG_FAIL("CAttack Render - m_pTransform->Bind_OnDevice");

	if (FAILED(m_pBoxCollider->Render()))
		MSG_FAIL("CAttack Render - m_pBoxCollider->Render");

	return S_OK;
}

HRESULT CAttack::SetUp_Scale(_vec3 vScale)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CAttack::SetUp_Scale - m_pTransform == nullptr");

	m_pTransform->SetUp_Scale(vScale);

	return S_OK;
}

_vec3 CAttack::Get_Scale()
{
	if (m_pTransform == nullptr)
		MSGBOX("CAttack::Get_Scale - m_pTransform == nullptr");

	return m_pTransform->Get_Scale();
}

void CAttack::Collision(CGameObject * pColObj)
{
	int i = 0;
}

void CAttack::Tick_Dead()
{
	m_iDead++;
}

HRESULT CAttack::Tick_Collider_BossSkeletonStageCircleAttack(_float fTimeDelta)
{
	m_pBoxCollider->Set_WorldMatrixFromObject(*m_pTransform->Get_WorldMatrixPtr());
	m_pBoxCollider->Tick(fTimeDelta);

	return S_OK;
}

_float CAttack::Go_Forward(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSGBOX("CAttack::Go_Forward - m_pTransform == nullptr");

	return m_pTransform->Go_Straight(fTimeDelta);
}

_float CAttack::Go_Right(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSGBOX("CAttack::Go_Right - m_pTransform == nullptr");

	return m_pTransform->Go_Right(fTimeDelta);
}

_float CAttack::Go_Left(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSGBOX("CAttack::Go_Left - m_pTransform == nullptr");

	return m_pTransform->Go_Left(fTimeDelta);
}

_float CAttack::Go_Back(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSGBOX("CAttack::Go_Back - m_pTransform == nullptr");

	return m_pTransform->Go_Backward(fTimeDelta);
}

HRESULT CAttack::SetUp_Component(void * pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_tDesc = *(DESC*)pArg;

	//트랜스폼 세팅
	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform)))
		MSG_FAIL("CAttack::SetUp_Component - Add_Component m_pTransform");
	m_pTransform->SetUp_State(CTransform::STATE_POS, m_tDesc.vPos);
	m_pTransform->SetUp_Scale(m_tDesc.vScale);
	m_pTransform->SetUp_LookOrthoWithoutY(m_tDesc.vLook);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CAttack::SetUp_Component - Add_Component m_pRenderer");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, _vec3(0.f, 0.f, 0.f), _vec3(1.f, 1.f, 1.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f), this);
	m_mapComponent.emplace(pTagComCollider, m_pBoxCollider);
	m_pBoxCollider->AddRef();

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CAttack * CAttack::Create(_pDevice pDevice)
{
	CAttack* pInstance = new CAttack(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CAttack Create Failed");
	}

	return pInstance;
}

CGameObject * CAttack::Clone(void * pArg)
{
	CAttack* pInstance = new CAttack(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CAttack Clone Failed");
	}

	return pInstance;
}

void CAttack::Free()
{
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pBoxCollider);
	__super::Free();
}

_vec3 CAttack::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxCollider->Get_MinPos();

	return vMinPos;
}

_vec3 CAttack::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxCollider->Get_MaxPos();

	return vMaxPos;
}

_vec3 CAttack::Get_WorldPos()
{
	return m_pBoxCollider->Get_ColliderWolrdPos();
}

_vec3 CAttack::Get_Length()
{
	return m_pTransform->Get_Scale();
}

HRESULT CAttack::Tick_Collider(_float fTimeDelta)
{
	m_pBoxCollider->Set_WorldMatrixFromObject(*m_pTransform->Get_WorldMatrixPtr());
	m_pBoxCollider->Tick(fTimeDelta);

	return S_OK;
}
