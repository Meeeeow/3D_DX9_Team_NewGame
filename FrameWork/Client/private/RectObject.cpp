#include "stdafx.h"
#include "..\public\RectObject.h"
#include "Frustum.h"

CRectObject::CRectObject(_pDevice pDevice)
	: CGameObject(pDevice)
{
}

CRectObject::CRectObject(const CRectObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRectObject::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CRectObject NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CRectObject::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CRectObject NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CRectObject::NativeConstruct - SetUp_Component");

	return S_OK;
}

_int CRectObject::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CRectObject Tick - __super");

	return 0;
}

_int CRectObject::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CRectObject LateTick - __super");

	if (m_pRenderer == nullptr)
		MSG_MINUSONE("CRectObject::LateTick - m_pRenderer == nullptr");

	if (m_pFrustum == nullptr)
		MSG_MINUSONE("CRectObject::LateTick - m_pFrustum == nullptr");

	_float fRadius = m_pTransform->Get_Scale().x * 1.5f;
	if (m_pFrustum->Check_Culling(m_pTransform, fRadius))
	{
		if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this)))
			MSG_MINUSONE("CRectObject::LateTick - m_pRenderer->Add_RenderGroup");
	}


	return _int();
}

HRESULT CRectObject::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CRectObject Render - __super");

	if (m_pTexture == nullptr ||
		m_pTransform == nullptr ||
		m_pVIBuffer == nullptr)
		MSG_FAIL("CRectObject Render - com is null");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CDagger::Render - Set_RenderState");

	if (FAILED(m_pTexture->Bind_OnDevice(m_tFrame.iStart)))
		MSG_FAIL("CRectObject Render - m_pTexture->Bind_OnDevice");

	if (FAILED(m_pTransform->Bind_OnDevice()))
		MSG_FAIL("CRectObject Render - m_pTransform->Bind_OnDevice");

	if (FAILED(m_pVIBuffer->Render()))
		MSG_FAIL("CRectObject Render - m_pVIBuffer->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CDagger::Render - Release_RenderState");

	return S_OK;
}

HRESULT CRectObject::Set_TransformState(CTransform::STATE eTransformState, const _vec3 & vState)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CRectObject::Set_TransformState - m_pTransform == nullptr");

	m_pTransform->SetUp_State(eTransformState, vState);

	return S_OK;
}

HRESULT CRectObject::Set_TransformScale(_vec3 vScale)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CRectObject::Set_TransformScale - m_pTransform == nullptr");

	m_pTransform->SetUp_Scale(vScale);

	return S_OK;
}

HRESULT CRectObject::Set_TransformScale(_float fX, _float fY, _float fZ)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CRectObject::Set_TransformScale - m_pTransform == nullptr");

	m_pTransform->SetUp_Scale(fX, fY, fZ);

	return S_OK;
}

HRESULT CRectObject::Set_LookOrtho(const _vec3 & vLook)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CRectObject::Set_LookOrtho - m_pTransform == nullptr");

	m_pTransform->SetUp_LookOrtho(vLook);

	return S_OK;
}

HRESULT CRectObject::Set_UpOrtho(const _vec3 & vUp)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CRectObject::Set_UpOrtho - m_pTransform == nullptr");

	m_pTransform->SetUp_UpOrtho(vUp);

	return S_OK;
}

_float CRectObject::Go_Straight(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSGBOX("CRectObject::Go_Straight - m_pTransform == nullptr");

	return m_pTransform->Go_Straight(fTimeDelta);
}

_float CRectObject::Go_Up(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSGBOX("CRectObject::Go_Up - m_pTransform == nullptr");

	return m_pTransform->Go_Up(fTimeDelta);
}

_float CRectObject::Go_Right(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSGBOX("CRectObject::Go_Right - m_pTransform == nullptr");

	return m_pTransform->Go_Right(fTimeDelta);
}

_float CRectObject::Go_Left(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSGBOX("CRectObject::Go_Left - m_pTransform == nullptr");

	return m_pTransform->Go_Left(fTimeDelta);
}

_float CRectObject::Go_Dir(_float fTimeDelta, _vec3 vDir)
{
	if (m_pTransform == nullptr)
		MSGBOX("CRectObject::Go_Dir - m_pTransform == nullptr");
	
	return m_pTransform->Go_Dir(fTimeDelta, vDir);
}

HRESULT CRectObject::Rotation_Axis(const _vec3& vAxis, _float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CRectObject::Rotation_Axis - m_pTransform == nullptr");

	m_pTransform->Rotation_Axis(vAxis, fTimeDelta);

	return S_OK;
}

HRESULT CRectObject::SetUp_Rotation(_vec3 vAxis, _float fDegree)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CRectObject::SetUp_Rotation - m_pTransform == nullptr");

	m_pTransform->SetUp_Rotation(vAxis, fDegree);

	return S_OK;
}

HRESULT CRectObject::Set_RotateSpeed(_float fRotateSpeed)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CRectObject::Set_RotateSpeed - m_pTransform == nullptr");

	m_pTransform->Set_RotateSpeed(fRotateSpeed);

	return S_OK;
}

HRESULT CRectObject::Set_Speed(_float fSpeed)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CRectObject::Set_RotateSpeed - m_pTransform == nullptr");

	m_pTransform->Set_Speed(fSpeed);

	return S_OK;
}

void CRectObject::Move_Projectile(_vec3 vStartPos, _vec3 vProjectilePower, _float fProjectileTime, _float fDegree)
{
	if (m_pTransform == nullptr)
		MSG_VOID("CRectObject::Move_Projectile - m_pTransform == nullptr");

	return m_pTransform->Projectile(vStartPos, vProjectilePower, fProjectileTime, fDegree);
}

_vec3 CRectObject::Get_ProjectilePowerToTarget(_vec3 vStartPos, _vec3 vTargetPos, _float fProjectileMaxTime, _float fDegree)
{
	if (m_pTransform == nullptr)
		MSGBOX("CRectObject::Get_ProjectilePowerToTarget - m_pTransform == nullptr");

	return m_pTransform->Get_ProjectilePowerToTarget(vStartPos, vTargetPos, fProjectileMaxTime, fDegree);
}

HRESULT CRectObject::Set_BoxColliderTransform(CBoxCollider * pBoxCollider)
{
	if (pBoxCollider == nullptr)
		MSG_FAIL("CRectObject::Set_BoxColliderTransform - pBoxCollider == nullptr");

	if (m_pTransform == nullptr)
		MSG_FAIL("CRectObject::Set_BoxColliderTransform - m_pTransform == nullptr ");

	pBoxCollider->Set_WorldMatrixFromObject(*m_pTransform->Get_WorldMatrixPtr());
	return S_OK;
}

HRESULT CRectObject::Tick_Collider(_float fTimeDelta, CBoxCollider * pBoxCollider)
{
	if (pBoxCollider == nullptr || m_pTransform == nullptr)
		MSG_FAIL("CRectObject::Tick_Collider - pBoxCollider == nullptr || m_pTransform == nullptr");

	pBoxCollider->Set_WorldMatrixFromObject(*m_pTransform->Get_WorldMatrixPtr());
	pBoxCollider->Tick(fTimeDelta);

	return S_OK;
}

_vec3 CRectObject::Get_TransformState(CTransform::STATE eTransformState)
{
	if (m_pTransform == nullptr)
	{
		MSGBOX("CRectObject::Get_TransformState - m_pTransform == nullptr");
		return _vec3();
	}

	return m_pTransform->Get_State(eTransformState);
}

_vec3 CRectObject::Get_TransformScale()
{
	if (m_pTransform == nullptr)
		MSGBOX("CRectObject::Get_TransformScale - m_pTransform == nullptr");

	return m_pTransform->Get_Scale();
}

HRESULT CRectObject::SetUp_Component(void * pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	DESC tDesc = *(DESC*)pArg;

	//텍스쳐 이름
	wstring wstrTextureName = tDesc.tObjInfo.szTextureName;

	//트랜스폼 세팅
	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform, &tDesc.tTransformDesc)))
		MSG_FAIL("CRectObject::SetUp_Component - Add_Component m_pTransform");
	m_pTransform->Set_Matrix(tDesc.tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CRectObject::SetUp_Component - Add_Component m_pRenderer");

	if (FAILED(Add_Component(tDesc.eTextureLevel, wstrTextureName.c_str(), pTagComTexture, (CComponent**)&m_pTexture)))
		MSG_FAIL("CRectObject::SetUp_Component - Add_Component m_pTexture");

	m_tFrame.iStart = tDesc.tObjInfo.iTextureIndex;

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CRectObject::SetUp_Component - Add_Component m_pVIBuffer");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeFrustum, pTagComFrustum, (CComponent**)&m_pFrustum)))
		MSG_FAIL("CRectObject::SetUp_Component - Add_Component m_pFrustum");

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CRectObject::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CRectObject::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
		MSG_FAIL("CRectObject::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)m_iAlphaRef)))
		MSG_FAIL("CRectObject::Set_RenderState - D3DRS_ALPHAREF, m_iAlphaRef)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
		MSG_FAIL("CRectObject::Set_RenderState - D3DRS_ALPHAFUNC, D3DCMP_GREATER)");

	return S_OK;
}

HRESULT CRectObject::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CRectObject::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CRectObject::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	return S_OK;
}

void CRectObject::Free()
{
	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pFrustum);
	__super::Free();
}

_vec3 CRectObject::Get_Length()
{
	return m_pTransform->Get_Scale();
}


