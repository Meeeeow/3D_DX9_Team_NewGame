#include "stdafx.h"
#include "..\public\LockOn.h"
#include "Player.h"
#include "CameraTargetBack.h"
#include "Camera2D.h"
#include "Meteor.h"

CLockOn * CLockOn::Create(_pDevice pGraphicDevice)
{
	CLockOn* pInstance = new CLockOn(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CLockOn::Clone(void * pArg)
{
	CLockOn* pInstance = new CLockOn(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLockOn::Free()
{
	__super::Free();
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTexture);
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
}

_vec3 CLockOn::Get_MinPos()
{
	return _vec3();
}

_vec3 CLockOn::Get_MaxPos()
{
	return _vec3();
}

_vec3 CLockOn::Get_WorldPos()
{
	return _vec3();
}

_vec3 CLockOn::Get_Length()
{
	return m_pTransform->Get_Scale();
}

CLockOn::CLockOn(_pDevice pGraphicDevice)
	: CGameObject(pGraphicDevice)
{
}

CLockOn::CLockOn(const CLockOn & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLockOn::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSGBOX("CLockOn::NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CLockOn::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSGBOX("CLockOn::NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSGBOX("CLockOn::NativeConstruct - SetUp_Component");

	return S_OK;
}

_int CLockOn::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	Control_Check(fTimeDelta,pGameInstance);

	Move(fTimeDelta,pGameInstance);
	Fire(fTimeDelta,pGameInstance);


	RELEASE_INSTANCE(CGameInstance);
	return _int();
}

_int CLockOn::LateTick(_float fTimeDelta)
{
	m_pRenderer->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this);

	return _int();
}

HRESULT CLockOn::Render()
{
	m_pTransform->Bind_OnDevice();
	m_pTexture->Bind_OnDevice();

	SetUp_RenderState();
	m_pVIBuffer->Render();
	Release_RenderState();

	return S_OK;
}

void CLockOn::Move(_float fTimeDelta, CGameInstance* pGameInstance)
{
	if (!m_bControl)
		return;
	m_tDesc.pObject->On_theCube();
	m_tDesc.pObject->Set_StateLock(true);

	if (pGameInstance->Key_Pressing(DIK_W))
		m_pTransform->Go_Straight(fTimeDelta);
	if (pGameInstance->Key_Pressing(DIK_S))
		m_pTransform->Go_Backward(fTimeDelta);
	if (pGameInstance->Key_Pressing(DIK_A))
		m_pTransform->Go_Left(fTimeDelta);
	if (pGameInstance->Key_Pressing(DIK_D))
		m_pTransform->Go_Right(fTimeDelta);

	_long dwMouse = 0;
	if (dwMouse = pGameInstance->Get_DIMouseMoveState(CInputDevice::DIMM_X))
		m_pTransform->Rotation_Axis(_vec3(0.f, 1.f, 0.f), fTimeDelta * dwMouse);

}

void CLockOn::Control_Check(_float fTimeDelta, CGameInstance* pGameInstance)
{
	m_fControlTime -= fTimeDelta;
	if (m_fControlTime <= 0.f && m_bControl)
	{
		CCameraTargetBack* pCameraBack = dynamic_cast<CCameraTargetBack*>(pGameInstance->Get_CurrentCamera());
		CCamera2D* pCamera2D = dynamic_cast<CCamera2D*>(pGameInstance->Get_CurrentCamera());
		if (pCameraBack != nullptr)
			pCameraBack->Set_Target(m_tDesc.pObject);
		if (pCamera2D != nullptr)
			pCamera2D->Set_TargetObject(m_tDesc.pObject);

		m_bControl = false;
		m_tDesc.pObject->Lock_Off();
		m_tDesc.pObject->Set_StateLock(false);
		Safe_Release(m_tDesc.pObject);
	}
}

void CLockOn::Fire(_float fTimeDelta, CGameInstance* pGameInstance)
{
	if (m_bControl)
		return;

	m_fFireTime -= fTimeDelta;
	m_fFireWait += fTimeDelta;
	if (m_fFireTime <= 0.f)
	{
		m_bDead = true;
		return;
	}
	if (m_fFireWait >= 0.15f)
	{
		m_fFireWait = 0.f;
		CMeteor::DESC tDesc;
		tDesc.vPos = m_pTransform->Get_State(CTransform::STATE_POS);
		tDesc.vPos.y += 45.f;
		tDesc.vPos.x -= rand() % 50;
		tDesc.vPos.x += rand() % 50;
		tDesc.vPos.z -= rand() % 50;
		tDesc.vPos.z += rand() % 50;
		
		tDesc.vTargetPos = m_pTransform->Get_State(CTransform::STATE_POS);

		pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeMeteor, pTagLayerPlayerAttack, &tDesc);
	}
}

HRESULT CLockOn::SetUp_Component(void * pArg)
{
	m_tDesc = *(DESC*)pArg;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTransform::TRANSFORM_DESC tTransformDesc;
	tTransformDesc.fSpeed = m_tDesc.fTransformSpeed;
	tTransformDesc.fDegree = m_tDesc.fTransformRotation;
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransform,&tTransformDesc)))
		MSG_FAIL("CPortal SetUp_Component - Add_Component m_pTransformCom");
	m_pTransform->Set_Matrix(m_tDesc.mat);
	m_pTransform->Add_Position(_vec3(0.f, 0.5f, 0.f));
	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRenderer)))
		MSG_FAIL("CPortal SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferXZ"), TEXT("ComVIBuffer"), (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CPortal SetUp_Component - Add_Component m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTextureLockOn, TEXT("ComTexture"), (CComponent**)&m_pTexture)))
		MSG_FAIL("CPortal SetUp_Component - Add_Component m_pTexture");

	CCamera2D* pCamera2D = dynamic_cast<CCamera2D*>(pGameInstance->Get_CurrentCamera());
	if (pCamera2D != nullptr)
		pGameInstance->Change_Camera(CAMERA_BACK);
	CCameraTargetBack* pCameraBack = dynamic_cast<CCameraTargetBack*>(pGameInstance->Get_CurrentCamera());
	if (pCameraBack != nullptr)
		pCameraBack->Set_Target(this);

	RELEASE_INSTANCE(CGameInstance);

	m_tDesc.pObject->Set_StateLock(true);
	Safe_AddRef(m_tDesc.pObject);
	return S_OK;
}

HRESULT CLockOn::SetUp_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}

HRESULT CLockOn::Release_RenderState()
{
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}
