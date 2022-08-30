#include "stdafx.h"
#include "..\public\TipObject.h"

CTipObject::CTipObject(_pDevice pDevice)
	: CGameObject(pDevice)
{
}

CTipObject::CTipObject(const CTipObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTipObject::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CTipObject NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CTipObject::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CTipObject NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CTipObject::NativeConstruct - SetUp_Component");

	D3DXMatrixOrthoLH(&m_matProject, (float)g_iWinCX, (float)g_iWinCY, 0.f, 1.f);

	_vec3 vOriPos = m_pTransform->Get_State(CTransform::STATE_POS);
	m_pTransform->SetUp_State(CTransform::STATE::STATE_POS, _vec3(vOriPos.x - ((float)g_iWinCX * 0.5f), vOriPos.y - ((float)g_iWinCY * 0.5f), 0.f));

	return S_OK;
}

_int CTipObject::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CTipObject Tick - __super");

	return 0;
}

HRESULT CTipObject::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CTipObject Render - __super");

	if (m_pTexture == nullptr ||
		m_pTransform == nullptr ||
		m_pVIBuffer == nullptr)
		MSG_FAIL("CTipObject Render - com is null");

	if (FAILED(Set_RenderState()))
		MSG_FAIL("CTipObject::Render - Set_RenderState");

	if (FAILED(m_pTexture->Bind_OnDevice(m_tFrame.iStart)))
		MSG_FAIL("CTipObject Render - m_pTexture->Bind_OnDevice");

	if (FAILED(m_pTransform->Bind_OnDevice()))
		MSG_FAIL("CTipObject Render - m_pTransform->Bind_OnDevice");

	if (FAILED(m_pVIBuffer->Render()))
		MSG_FAIL("CTipObject Render - m_pVIBuffer->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CTipObject::Render - Release_RenderState");

	return S_OK;
}

HRESULT CTipObject::Set_TransformState(CTransform::STATE eTransformState, const _vec3 & vState)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CTipObject::Set_TransformState - m_pTransform == nullptr");

	m_pTransform->SetUp_State(eTransformState, vState);

	return S_OK;
}

HRESULT CTipObject::Set_TransformScale(_vec3 vScale)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CTipObject::Set_TransformScale - m_pTransform == nullptr");

	m_pTransform->SetUp_Scale(vScale);

	return S_OK;
}

HRESULT CTipObject::Set_TransformScale(_float fX, _float fY, _float fZ)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CTipObject::Set_TransformScale - m_pTransform == nullptr");

	m_pTransform->SetUp_Scale(fX, fY, fZ);

	return S_OK;
}

HRESULT CTipObject::Set_LookOrtho(const _vec3 & vLook)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CTipObject::Set_LookOrtho - m_pTransform == nullptr");

	m_pTransform->SetUp_LookOrtho(vLook);

	return S_OK;
}

HRESULT CTipObject::Set_UpOrtho(const _vec3 & vUp)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CTipObject::Set_UpOrtho - m_pTransform == nullptr");

	m_pTransform->SetUp_UpOrtho(vUp);

	return S_OK;
}

_float CTipObject::Go_Straight(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSGBOX("CTipObject::Go_Straight - m_pTransform == nullptr");

	return m_pTransform->Go_Straight(fTimeDelta);
}

_float CTipObject::Go_Up(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSGBOX("CTipObject::Go_Up - m_pTransform == nullptr");

	return m_pTransform->Go_Up(fTimeDelta);
}

_float CTipObject::Go_Right(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSGBOX("CTipObject::Go_Right - m_pTransform == nullptr");

	return m_pTransform->Go_Right(fTimeDelta);
}

_float CTipObject::Go_Left(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSGBOX("CTipObject::Go_Left - m_pTransform == nullptr");

	return m_pTransform->Go_Left(fTimeDelta);
}

_float CTipObject::Go_Dir(_float fTimeDelta, _vec3 vDir)
{
	if (m_pTransform == nullptr)
		MSGBOX("CTipObject::Go_Dir - m_pTransform == nullptr");
	
	return m_pTransform->Go_Dir(fTimeDelta, vDir);
}

HRESULT CTipObject::Rotation_Axis(const _vec3& vAxis, _float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CTipObject::Rotation_Axis - m_pTransform == nullptr");

	m_pTransform->Rotation_Axis(vAxis, fTimeDelta);

	return S_OK;
}

HRESULT CTipObject::SetUp_Rotation(_vec3 vAxis, _float fDegree)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CTipObject::SetUp_Rotation - m_pTransform == nullptr");

	m_pTransform->SetUp_Rotation(vAxis, fDegree);

	return S_OK;
}

HRESULT CTipObject::Set_RotateSpeed(_float fRotateSpeed)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CTipObject::Set_RotateSpeed - m_pTransform == nullptr");

	m_pTransform->Set_RotateSpeed(fRotateSpeed);

	return S_OK;
}

HRESULT CTipObject::Set_Speed(_float fSpeed)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CTipObject::Set_RotateSpeed - m_pTransform == nullptr");

	m_pTransform->Set_Speed(fSpeed);

	return S_OK;
}

void CTipObject::Add_Pos(_vec3 vPos)
{
	_vec3 vTransPos = m_pTransform->Get_State(CTransform::STATE_POS);
	vTransPos += vPos;
	m_pTransform->SetUp_State(CTransform::STATE_POS, vTransPos);
}

void CTipObject::Sub_Pos(_vec3 vPos)
{
	_vec3 vTransPos = m_pTransform->Get_State(CTransform::STATE_POS);
	vTransPos -= vPos;
	m_pTransform->SetUp_State(CTransform::STATE_POS, vTransPos);
}

void CTipObject::Move_Projectile(_vec3 vStartPos, _vec3 vProjectilePower, _float fProjectileTime, _float fDegree)
{
	if (m_pTransform == nullptr)
		MSG_VOID("CTipObject::Move_Projectile - m_pTransform == nullptr");

	return m_pTransform->Projectile(vStartPos, vProjectilePower, fProjectileTime, fDegree);
}

_vec3 CTipObject::Get_ProjectilePowerToTarget(_vec3 vStartPos, _vec3 vTargetPos, _float fProjectileMaxTime, _float fDegree)
{
	if (m_pTransform == nullptr)
		MSGBOX("CTipObject::Get_ProjectilePowerToTarget - m_pTransform == nullptr");

	return m_pTransform->Get_ProjectilePowerToTarget(vStartPos, vTargetPos, fProjectileMaxTime, fDegree);
}

HRESULT CTipObject::Set_BoxColliderTransform(CBoxCollider * pBoxCollider)
{
	if (pBoxCollider == nullptr)
		MSG_FAIL("CTipObject::Set_BoxColliderTransform - pBoxCollider == nullptr");

	if (m_pTransform == nullptr)
		MSG_FAIL("CTipObject::Set_BoxColliderTransform - m_pTransform == nullptr ");

	pBoxCollider->Set_WorldMatrixFromObject(*m_pTransform->Get_WorldMatrixPtr());
	return S_OK;
}

HRESULT CTipObject::Tick_Collider(_float fTimeDelta, CBoxCollider * pBoxCollider)
{
	if (pBoxCollider == nullptr || m_pTransform == nullptr)
		MSG_FAIL("CTipObject::Tick_Collider - pBoxCollider == nullptr || m_pTransform == nullptr");

	pBoxCollider->Set_WorldMatrixFromObject(*m_pTransform->Get_WorldMatrixPtr());
	pBoxCollider->Tick(fTimeDelta);

	return S_OK;
}

_vec3 CTipObject::Get_TransformState(CTransform::STATE eTransformState)
{
	if (m_pTransform == nullptr)
	{
		MSGBOX("CTipObject::Get_TransformState - m_pTransform == nullptr");
		return _vec3();
	}

	return m_pTransform->Get_State(eTransformState);
}

_vec3 CTipObject::Get_TransformScale()
{
	if (m_pTransform == nullptr)
		MSGBOX("CTipObject::Get_TransformScale - m_pTransform == nullptr");

	return m_pTransform->Get_Scale();
}

HRESULT CTipObject::SetUp_Component(void * pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	DESC tDesc = *(DESC*)pArg;

	m_bBlend = tDesc.bBlend;

	//트랜스폼 세팅
	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform)))
		MSG_FAIL("CTipObject::SetUp_Component - Add_Component m_pTransform");
	m_pTransform->Set_Matrix(tDesc.tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CTipObject::SetUp_Component - Add_Component m_pRenderer");

	if (FAILED(Add_Component(tDesc.eTextureLevel, tDesc.tObjInfo.szTextureName, pTagComTexture, (CComponent**)&m_pTexture)))
		MSG_FAIL("CTipObject::SetUp_Component - Add_Component m_pTexture");

	m_tFrame.iStart = tDesc.tObjInfo.iTextureIndex;

	if (FAILED(Add_Component(LEVEL_STATIC, tDesc.pTagPrototypeVIBuffer, pTagComVIBuffer, (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CTipObject::SetUp_Component - Add_Component m_pVIBuffer");

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CTipObject::Set_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CTipObject::Set_RenderState - m_pGraphicDevice == nullptr");

	_mat tmp;
	D3DXMatrixIdentity(&tmp);

	m_pGraphicDevice->SetTransform(D3DTS_VIEW, &tmp);
	m_pGraphicDevice->SetTransform(D3DTS_PROJECTION, &m_matProject);

	if (m_bBlend)
	{
		if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
			MSG_FAIL("CTipObject::Set_RenderState - SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)");

		if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)))
			MSG_FAIL("CTipObject::Set_RenderState - SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)");

		if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)))
			MSG_FAIL("CTipObject::Set_RenderState - SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)");
	}
	else
	{
		if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
			MSG_FAIL("CTipObject::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

		if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)m_iAlphaRef)))
			MSG_FAIL("CTipObject::Set_RenderState - D3DRS_ALPHAREF, m_iAlphaRef)");

		if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
			MSG_FAIL("CTipObject::Set_RenderState - D3DRS_ALPHAFUNC, D3DCMP_GREATER)");
	}


	return S_OK;
}

HRESULT CTipObject::Release_RenderState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CTipObject::Set_RenderState - m_pGraphicDevice == nullptr");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
		MSG_FAIL("CTipObject::Set_RenderState - m_pGraphicDevice->SetRenderState D3DRS_ALPHATESTENABLE, TRUE");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
		MSG_FAIL("CTipObject::Set_RenderState - SetRenderState");

	return S_OK;
}

CGameObject * CTipObject::Clone(void * pArg)
{
	return nullptr;
}

CTipObject * CTipObject::Create(_pDevice pDevice, DESC * tDesc)
{
	CTipObject* pInstance = new CTipObject(pDevice);

	if (FAILED(pInstance->NativeConstruct(tDesc)))
	{
		Safe_Release(pInstance);
		MSGBOX("CBossSkeletonStageCubeAttack Create Failed");
	}

	return pInstance;
}

void CTipObject::Free()
{
	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pVIBuffer);
	__super::Free();
}

_vec3 CTipObject::Get_MinPos()
{
	return _vec3();
}

_vec3 CTipObject::Get_MaxPos()
{
	return _vec3();
}

_vec3 CTipObject::Get_WorldPos()
{
	return _vec3();
}

_vec3 CTipObject::Get_Length()
{
	return m_pTransform->Get_Scale();
}


