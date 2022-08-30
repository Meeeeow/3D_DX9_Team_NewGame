#include "stdafx.h"
#include "..\public\TrumpCard.h"
#include "Player.h"

CTrumpCard::CTrumpCard(_pDevice pGraphicDevice)
	: CEffect(pGraphicDevice)
{
}

CTrumpCard::CTrumpCard(const CTrumpCard & rhs)
	: CEffect(rhs)
{
}

HRESULT CTrumpCard::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CTrumpCard::NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CTrumpCard::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CTrumpCard::NativeConstruct - __super");

	return S_OK;
}

_int CTrumpCard::Tick(_float fTimeDelta)
{
	m_tDesc.fDuration -= fTimeDelta;
	if (m_tDesc.fDuration <= 0.f)
		m_bDead = true;

	return OBJ_NOEVENT;
}

_int CTrumpCard::LateTick(_float fTimeDelta)
{
	if (m_pFrustum->Check_Culling(m_pTransform, 1.5f))
		m_pRenderer->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this);

	if(!m_tDesc.bProjectile)
		Move(fTimeDelta);
	if (m_tDesc.bProjectile)
		Parabola(fTimeDelta);

	BillBoarding(m_pTransform);

	return 0;
}

HRESULT CTrumpCard::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CTrumpCard Render - __super");

	if (m_pTexture == nullptr ||
		m_pTransform == nullptr ||
		m_pVIBuffer == nullptr)
		MSG_FAIL("CTrumpCard Render - nullptr");

	if (FAILED(m_pTexture->Bind_OnDevice(m_tDesc.iTexutreNum)))
		MSG_FAIL("CTrumpCard Render - BoxCollider");

	if (FAILED(m_pTransform->Bind_OnDevice()))
		MSG_FAIL("CTrumpCard Render - Transform");

	Set_RenderState();

	if (FAILED(m_pVIBuffer->Render()))
		MSG_FAIL("CTrumpCard Render - VIBuffer");

	if (FAILED(m_pBoxCollider->Render()))
		MSG_FAIL("CTrumpCard Render - BoxCollider");

	Release_RenderState();

	return S_OK;
}

HRESULT CTrumpCard::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CTrumpCard Set_RenderState - m_pGraphicDevice is null");

	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CTrumpCard::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CTrumpCard Release_RenderState - m_pGraphicDevice is null");

	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

void CTrumpCard::Direction()
{
	_vec3 vLook = m_tDesc.vTargetPos - m_tDesc.vPos, vUp = { 0.f, 1.f, 0.f }, vRight;
	D3DXVec3Normalize(&vLook, &vLook);
	m_tDesc.vDir = vLook * m_tDesc.fPower;
	m_pTransform->SetUp_State(CTransform::STATE_LOOK, vLook);
	D3DXVec3Cross(&vRight, &vUp, &vLook);
	m_pTransform->SetUp_State(CTransform::STATE_RIGHT, vRight);
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	m_pTransform->SetUp_State(CTransform::STATE_UP, vUp);
}

void CTrumpCard::Projectile()
{
	m_vProjectileDir = m_pTransform->Get_ProjectilePowerToTarget(m_tDesc.vPos, m_tDesc.vTargetPos, 2.5f, m_fDegreeProjectile);
}

void CTrumpCard::Move(_float fTimeDelta)
{
	m_pTransform->Go_Dir(fTimeDelta, m_tDesc.vDir);
}

void CTrumpCard::Parabola(_float fTimeDelta)
{
	m_fTimeProjectile += fTimeDelta;
	m_pTransform->Projectile(m_tDesc.vPos, m_vProjectileDir, m_fTimeProjectile, m_fDegreeProjectile);

	if (m_tDesc.vTargetPos.y - 3.f > m_tDesc.vPos.y)
		Set_Dead();
}

void CTrumpCard::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pFrustum);
	__super::Free();
}

_vec3 CTrumpCard::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxCollider->Get_MinPos();
	
	return vMinPos;
}

_vec3 CTrumpCard::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxCollider->Get_MaxPos();

	return vMaxPos;
}

_vec3 CTrumpCard::Get_WorldPos()
{
	return m_pBoxCollider->Get_ColliderWolrdPos();
}

_vec3 CTrumpCard::Get_Length()
{
	return m_pTransform->Get_Scale();
}

HRESULT CTrumpCard::Tick_Collider(_float fTimeDelta)
{
	m_pBoxCollider->Set_WorldMatrixFromObject(*m_pTransform->Get_WorldMatrixPtr());
	m_pBoxCollider->Tick(fTimeDelta);

	return S_OK;
}
