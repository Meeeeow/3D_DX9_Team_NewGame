#include "stdafx.h"
#include "..\public\ThornObject.h"
#include "GameInstance.h"
#include "Terrain.h"
#include "InputDevice.h"
#include "PlayerTagMgr.h"
#include "Attack.h"

CThornObject::CThornObject(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CGameObject(pGraphicDevice)
{
}

CThornObject::CThornObject(const CThornObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CThornObject::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CThornObject NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CThornObject::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CThornObject NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CThornObject::NativeConstruct - SetUp_Component");

	return S_OK;
}

_int CThornObject::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CThornObject Tick - __super");

	Tick_Collider(fTimeDelta);

	return 0;
}

_int CThornObject::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CThornObject LateTick - __super");

	if (m_pRenderer == nullptr)
		MSG_MINUSONE("CThornObject LateTick - m_pRenderer == nullptr");

	if (m_pFrustum->Check_Culling(m_pTransform, m_pTransform->Get_Scale().x * 1.5f))
	{
		if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this)))
			MSG_FAIL("CThornObject::LateTick - Add_RenderGroup(CRenderer::GROUP_NONALPHA");
	}

	return _int();
}

HRESULT CThornObject::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CThornObject Render - __super");

	if (m_pTransform == nullptr ||
		m_pTexture == nullptr ||
		m_pVIBuffer == nullptr || 
		m_pBoxCollider == nullptr)
		MSG_FAIL("CThornObject::Render - Com is null");

	if (FAILED(m_pTransform->Bind_OnDevice()))
		MSG_FAIL("CThornObject::Render - m_pTransform->Bind_OnDevice");

	if (FAILED(m_pTexture->Bind_OnDevice(m_tFrame.iStart)))
		MSG_FAIL("CThornObject::Render - m_pTexture->Bind_OnDevice");

	if (FAILED(SetUp_RenderState()))
		MSG_FAIL("CThornObject::Render - Set_RenderState");

	if (FAILED(m_pVIBuffer->Render()))
		MSG_FAIL("CThornObject::Render - m_pVIBuffer->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CThornObject::Render - Release_RenderState");

	if (FAILED(m_pBoxCollider->Render()))
		MSG_FAIL("CThornObject::Render - m_pBoxCollider->Render");

	return S_OK;
}

_float CThornObject::Go_Up(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSGBOX("CThornObject::Go_Up - m_pTransform == nullptr");

	return m_pTransform->Go_Up(fTimeDelta);
}

_float CThornObject::Go_Down(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSGBOX("CThornObject::Go_Down - m_pTransform == nullptr");

	return m_pTransform->Go_Down(fTimeDelta);
}

HRESULT CThornObject::SetUp_Speed(_float fSpeed)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CThornObject::SetUp_Speed - m_pTransform == nullptr");

	m_pTransform->Set_Speed(fSpeed);

	return S_OK;
}

_vec3 CThornObject::Get_TransformState(CTransform::STATE eState)
{
	if (m_pTransform == nullptr)
		MSGBOX("CThornObject::Get_TransformState - m_pTransform == nullptr");

	return m_pTransform->Get_State(eState);
}

_vec3 CThornObject::Get_TransformScale()
{
	if (m_pTransform == nullptr)
		MSGBOX("CThornObject::Get_TransformScale - m_pTransform == nullptr");

	return m_pTransform->Get_Scale();
}

_bool CThornObject::Check_YIsZero()
{
	if (m_pTransform == nullptr)
		MSG_FALSE("CThornObject::Check_MaxYIsZero - m_pTransform == nullptr");

	if (m_pTransform->Get_State(CTransform::STATE_POS).y <= -10.f)
		return true;

	return false;
}

HRESULT CThornObject::Tick_Collider(_float fTimeDelta)
{
	if (m_pBoxCollider == nullptr)
		MSG_FAIL("CCollisionCubeObject::Tick_Collider - m_pBoxCollider == nullptr");

	m_pBoxCollider->Set_WorldMatrixFromObject(*m_pTransform->Get_WorldMatrixPtr());
	m_pBoxCollider->Tick(fTimeDelta);

	return S_OK;
}

HRESULT CThornObject::SetUp_Component(void * pArg)
{
	if (pArg == nullptr)
		MSG_FAIL("CThornObject::SetUp_Component - pArg == nullptr");

	DESC tDesc = *(DESC*)(pArg);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform
		, (CComponent**)&m_pTransform, &tDesc.tTransformDesc)))
		MSG_FAIL("CThornObject::SetUp_Component - pTagPrototypeTransform");
	m_pTransform->Set_Matrix(tDesc.tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CThornObject::SetUp_Component - pTagPrototypeRenderer");

	if (FAILED(Add_Component(tDesc.eTextureLevel, tDesc.tObjInfo.szTextureName, pTagComTexture, (CComponent**)&m_pTexture)))
		MSG_FAIL("CThornObject::SetUp_Component - szTextureName");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferThorn, pTagComVIBuffer, (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CThornObject::SetUp_Component - pTagPrototypeVIBufferThorn");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeFrustum, pTagComFrustum, (CComponent**)&m_pFrustum)))
		MSG_FAIL("CThornObject::SetUp_Component - pTagPrototypeFrustum");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, tDesc.tObjInfo.vColliderCenter, tDesc.tObjInfo.vColliderSize
	, tDesc.tObjInfo.dwColliderColor, this);

	return S_OK;
}

HRESULT CThornObject::SetUp_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CThornObject SetUp_RenderState - m_pGraphicDevice is null");

	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CThornObject::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CThornObject Release_RenderState - m_pGraphicDevice is null");

	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_CCW);
	return S_OK;
}

void CThornObject::Free()
{
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pFrustum);

	__super::Free();
}

_vec3 CThornObject::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxCollider->Get_MinPos();

	return vMinPos;
}

_vec3 CThornObject::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxCollider->Get_MaxPos();

	return vMaxPos;
}

_vec3 CThornObject::Get_WorldPos()
{
	return m_pBoxCollider->Get_ColliderWolrdPos();
}

_vec3 CThornObject::Get_Length()
{
	return m_pTransform->Get_Scale();
}

void CThornObject::Collision(CGameObject * pColObj)
{

}
