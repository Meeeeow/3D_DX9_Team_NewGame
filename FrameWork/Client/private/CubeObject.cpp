#include "stdafx.h"
#include "..\public\CubeObject.h"
#include "Frustum.h"
#include "Transform.h"

CCubeObject::CCubeObject(_pDevice pDevice)
	: CGameObject(pDevice)
{
}

CCubeObject::CCubeObject(const CCubeObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCubeObject::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CCubeObject NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CCubeObject::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CCubeObject NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CCubeObject::NativeConstruct - SetUp_Component");


	return S_OK;
}

_int CCubeObject::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CCubeObject Tick - __super");

	return 0;
}

_int CCubeObject::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CCubeObject LateTick - __super");

	if (m_pRenderer == nullptr)
		MSG_MINUSONE("CCubeObject::LateTick - m_pRenderer == nullptr");

	if (m_pFrustum == nullptr)
		MSG_MINUSONE("CCubeObject::LateTick - m_pFrustum == nullptr");

	_float fRadius = m_pTransform->Get_Scale().x * 1.5f;
	if (m_pFrustum->Check_Culling(m_pTransform, fRadius))
	{
		//알파 블랜딩
		if (m_bAlphaBlend)
		{
			if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::GROUP_ALPHA, this)))
				MSG_MINUSONE("CCubeObject::LateTick - m_pRenderer->Add_RenderGroup");
		}
		else
		{
			if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this)))
				MSG_MINUSONE("CCubeObject::LateTick - m_pRenderer->Add_RenderGroup");
		}
	}


	return _int();
}

HRESULT CCubeObject::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CCubeObject Render - __super");

	if (m_pTexture == nullptr ||
		m_pTransform == nullptr ||
		m_pVIBuffer == nullptr)
		MSG_FAIL("CCubeObject Render - com is null");

	if (FAILED(m_pTexture->Bind_OnDevice(m_tFrame.iStart)))
		MSG_FAIL("CCubeObject Render - m_pTexture->Bind_OnDevice");

	if (FAILED(m_pTransform->Bind_OnDevice()))
		MSG_FAIL("CCubeObject Render - m_pTransform->Bind_OnDevice");

	if (FAILED(m_pVIBuffer->Render()))
		MSG_FAIL("CCubeObject Render - m_pVIBuffer->Render");

	return S_OK;
}

HRESULT CCubeObject::Set_TransformState(CTransform::STATE eTransformState, const _vec3 & vState)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CCubeObject::Set_TransformState - m_pTransform == nullptr");

	m_pTransform->SetUp_State(eTransformState, vState);

	return S_OK;
}

HRESULT CCubeObject::Set_TransformScale(_vec3 vScale)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CCubeObject::Set_TransformScale - m_pTransform == nullptr");

	m_pTransform->SetUp_Scale(vScale);

	return S_OK;
}

HRESULT CCubeObject::Set_TransformScale(_float fX, _float fY, _float fZ)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CCubeObject::Set_TransformScale - m_pTransform == nullptr");

	m_pTransform->SetUp_Scale(fX, fY, fZ);

	return S_OK;
}

_float CCubeObject::Go_Straight(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
	{
		MSGBOX("CCubeObject::Go_Straight - m_pTransform == nullptr");
		return -1.f;
	}

	return m_pTransform->Go_Straight(fTimeDelta);
}

_float CCubeObject::Go_Dir(_float fTimeDelta, _vec3 vDir)
{
	if (m_pTransform == nullptr)
		MSGBOX("CCubeObject::Go_Dir - m_pTransform == nullptr");
	
	return m_pTransform->Go_Dir(fTimeDelta, vDir);
}

_float CCubeObject::Go_Up(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSGBOX("CCubeObject::Go_Up - m_pTransform == nullptr");

	return m_pTransform->Go_Up(fTimeDelta);
}

_float CCubeObject::Go_Left(_float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSGBOX("CCubeObject::Go_Left - m_pTransform == nullptr");

	return m_pTransform->Go_Left(fTimeDelta);
}

HRESULT CCubeObject::Rotation_Axis(const _vec3& vAxis, _float fTimeDelta)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CCubeObject::Rotation_Axis - m_pTransform == nullptr");

	m_pTransform->Rotation_Axis(vAxis, fTimeDelta);

	return S_OK;
}

HRESULT CCubeObject::SetUp_Rotation(_vec3 vAxis, _float fDegree)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CCubeObject::SetUp_Rotation - m_pTransform == nullptr");

	m_pTransform->SetUp_Rotation(vAxis, fDegree);

	return S_OK;
}

HRESULT CCubeObject::Set_RotateSpeed(_float fRotateSpeed)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CCubeObject::Set_RotateSpeed - m_pTransform == nullptr");

	m_pTransform->Set_RotateSpeed(fRotateSpeed);

	return S_OK;
}

HRESULT CCubeObject::Set_Speed(_float fSpeed)
{
	if (m_pTransform == nullptr)
		MSG_FAIL("CCubeObject::Set_RotateSpeed - m_pTransform == nullptr");

	m_pTransform->Set_Speed(fSpeed);

	return S_OK;
}

void CCubeObject::Move_Projectile(_vec3 vStartPos, _vec3 vProjectilePower, _float fProjectileTime, _float fDegree)
{
	if (m_pTransform == nullptr)
		MSG_VOID("CCubeObject::Move_Projectile - m_pTransform == nullptr");

	return m_pTransform->Projectile(vStartPos, vProjectilePower, fProjectileTime, fDegree);
}

_vec3 CCubeObject::Get_ProjectilePowerToTarget(_vec3 vStartPos, _vec3 vTargetPos, _float fProjectileMaxTime, _float fDegree)
{
	if (m_pTransform == nullptr)
		MSGBOX("CCubeObject::Get_ProjectilePowerToTarget - m_pTransform == nullptr");

	return m_pTransform->Get_ProjectilePowerToTarget(vStartPos, vTargetPos, fProjectileMaxTime, fDegree);
}

HRESULT CCubeObject::Set_BoxColliderTransform(CBoxCollider * pBoxCollider)
{
	if (pBoxCollider == nullptr)
		MSG_FAIL("CCubeObject::Set_BoxColliderTransform - pBoxCollider == nullptr");

	if (m_pTransform == nullptr)
		MSG_FAIL("CCubeObject::Set_BoxColliderTransform - m_pTransform == nullptr ");

	pBoxCollider->Set_WorldMatrixFromObject(*m_pTransform->Get_WorldMatrixPtr());
	return S_OK;
}

_vec3 CCubeObject::Get_TransformState(CTransform::STATE eTransformState)
{
	if (m_pTransform == nullptr)
	{
		MSGBOX("CCubeObject::Get_TransformState - m_pTransform == nullptr");
		return _vec3();
	}

	return m_pTransform->Get_State(eTransformState);
}

_vec3 CCubeObject::Get_TransformScale()
{
	if (m_pTransform == nullptr)
		MSGBOX("CCubeObject::Get_TransformScale - m_pTransform == nullptr");

	return m_pTransform->Get_Scale();
}

const _mat * CCubeObject::Get_TransformMatPtr() const
{
	if (m_pTransform == nullptr)
		MSGBOX("CCubeObject::Get_TransformMatPtr - m_pTransform == nullptr");

	return m_pTransform->Get_WorldMatrixPtr();
}

void CCubeObject::Get_BottomBufferPos(_vec3 * vPosArr)
{
	if (m_pVIBuffer == nullptr)
		MSG_VOID("CCubeObject::Get_BottomBufferPos - m_pVIBuffer == nullptr");

	m_pVIBuffer->Get_BottomBufferPos(vPosArr);
	return;
}

HRESULT CCubeObject::SetUp_Component(void * pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	OBJ_INFO tObjInfo = *(OBJ_INFO*)pArg;

	//텍스쳐 이름
	wstring wstrTextureName = tObjInfo.szTextureName;

	//트랜스폼 세팅
	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform)))
		MSG_FAIL("CCubeObject::SetUp_Component - Add_Component m_pTransform");
	m_pTransform->Set_Matrix(tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CCubeObject::SetUp_Component - Add_Component m_pRenderer");

	if (FAILED(Add_Component(LEVEL_STATIC, wstrTextureName.c_str(), L"ComTexture", (CComponent**)&m_pTexture)))
		MSG_FAIL("CCubeObject::SetUp_Component - Add_Component m_pTexture");

	m_tFrame.iStart = tObjInfo.iTextureIndex;

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferCube, pTagComVIBuffer, (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CCubeObject::SetUp_Component - Add_Component m_pVIBuffer");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeFrustum, pTagComFrustum, (CComponent**)&m_pFrustum)))
		MSG_FAIL("CCubeObject::SetUp_Component - Add_Component m_pFrustum");

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CCubeObject::Tick_Mat(CBoxCollider * pBoxCollider)
{
	if (pBoxCollider == nullptr)
		MSG_FAIL("CCubeObject::Tick_Mat - pBoxCollider == nullptr");

	if (FAILED(pBoxCollider->Tick_Mat(*m_pTransform->Get_WorldMatrixPtr())))
		MSG_FAIL("CCubeObject::Tick_Mat - pBoxCollider->Tick_Mat");

	return S_OK;
}

CCubeObject * CCubeObject::Create(_pDevice pDevice)
{
	CCubeObject* pInstance = new CCubeObject(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CCubeObject Create Failed");
	}

	return pInstance;
}

CGameObject * CCubeObject::Clone(void * pArg)
{
	CCubeObject* pInstance = new CCubeObject(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CCubeObject Clone Failed");
	}

	return pInstance;
}

void CCubeObject::Free()
{
	Safe_Release(m_pTransform);
	Safe_Release(m_pTexture);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pFrustum);
	__super::Free();
}

_vec3 CCubeObject::Get_MinPos()
{
	return _vec3();
}

_vec3 CCubeObject::Get_MaxPos()
{
	return _vec3();
}

_vec3 CCubeObject::Get_WorldPos()
{
	return _vec3();
}

_vec3 CCubeObject::Get_Length()
{
	return m_pTransform->Get_Scale();
}


