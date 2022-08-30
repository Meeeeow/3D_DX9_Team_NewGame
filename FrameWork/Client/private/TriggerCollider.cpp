#include "stdafx.h"
#include "..\public\TriggerCollider.h"
#include "Player.h"

CTriggerCollider::CTriggerCollider(_pDevice pDevice)
	: CGameObject(pDevice)
{
}

CTriggerCollider::CTriggerCollider(const CTriggerCollider & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTriggerCollider::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CTriggerCollider NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CTriggerCollider::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CTriggerCollider NativeConstruct - __super");

	DESC tDesc = *(DESC*)(pArg);
	if (FAILED(SetUp_Component(&tDesc.tObjInfo)))
		MSG_FAIL("CTriggerCollider::NativeConstruct - SetUp_Component");
	
	m_vTargetPos = tDesc.vTargetPos;
	m_eTuto = tDesc.eTuto;
	m_bTrans = tDesc.bTrans;

	return S_OK;
}

_int CTriggerCollider::Tick(_float fTimeDelta)
{
	if (0 > (__super::Tick(fTimeDelta)))
		MSG_MINUSONE("CTriggerCollider Tick - __super");

	if (FAILED(Tick_Collider(fTimeDelta)))
		MSG_MINUSONE("CTriggerCollider::Tick - Tick_Collider");

	return 0;
}

_int CTriggerCollider::LateTick(_float fTimeDelta)
{
	if (0 > (__super::LateTick(fTimeDelta)))
		MSG_MINUSONE("CTriggerCollider LateTick - __super");

	if (m_pRenderer == nullptr)
		MSG_MINUSONE("CTriggerCollider LateTick - m_pRenderer == nullptr");

	if (FAILED(m_pRenderer->Add_RenderGroup(CRenderer::RENDER_GROUP::GROUP_NONALPHA, this)))
		MSG_FAIL("CTriggerCollider LateTick - Add_RenderGroup");

	return _int();
}

HRESULT CTriggerCollider::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CTriggerCollider Render - __super");

	if (m_pBoxCollider == nullptr)
		MSG_FAIL("CTriggerCollider Render - com is null");

	if (FAILED(m_pBoxCollider->Render()))
		MSG_FAIL("CTriggerCollider Render - m_pBoxCollider->Render");

	return S_OK;
}

HRESULT CTriggerCollider::SetUp_Component(void * pArg)
{
	OBJ_INFO tObjInfo = *(OBJ_INFO*)(pArg);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform)))
		MSG_FAIL("CTriggerCollider::SetUp_Component - pTagPrototypeTransform");
	m_pTransform->Set_Matrix(tObjInfo.matWorld);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CTriggerCollider::SetUp_Component - pTagPrototypeRenderer");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, tObjInfo.vColliderCenter, tObjInfo.vColliderSize, tObjInfo.dwColliderColor, this);
	if (m_pBoxCollider == nullptr)	
		MSG_FAIL("CTriggerCollider::SetUp_Component - m_pBoxCollider == nullptr");

	return S_OK;
}

CTriggerCollider * CTriggerCollider::Create(_pDevice pDevice)
{
	CTriggerCollider* pInstance = new CTriggerCollider(pDevice);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		Safe_Release(pInstance);
		MSGBOX("CTriggerCollider Create Failed");
	}

	return pInstance;
}

CGameObject * CTriggerCollider::Clone(void * pArg)
{
	CTriggerCollider* pInstance = new CTriggerCollider(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		Safe_Release(pInstance);
		MSGBOX("CTriggerCollider Clone Failed");
	}

	return pInstance;
}

void CTriggerCollider::Free()
{
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pTransform);
	Safe_Release(m_pRenderer);
	__super::Free();
}

void CTriggerCollider::Collision(CGameObject * pColObj)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pColObj);

	DO;
	if (pPlayer == nullptr)
		break;

	CTransform* pTransform = dynamic_cast<CTransform*>(pPlayer->Get_Component(pTagComTransform));
	if (pTransform == nullptr)
		break;

	if (m_bTrans)
	{
		pPlayer->Set_Pos(m_vTargetPos, pTransform);
		pPlayer->Set_Idle();
		pPlayer->Set_JumpEnd();
		pPlayer->On_theCube();
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		if (pGameInstance->Get_CurrentCameraID() == CAMERA_2D)
			pGameInstance->Change_Camera(CAMERA_BACK);
		RELEASE_INSTANCE(CGameInstance);
	}

	if (m_eTuto != CTutorialMgr::TUTORIAL_END)
	{
		CTutorialMgr::Get_Instance()->Set_TutorialToolTip(m_eTuto);
		//CTutorialMgr::Get_Instance()->Set_Tutorial(m_eTuto);
	}

	WHILE;
}

_vec3 CTriggerCollider::Get_MinPos()
{
	_vec3 vMinPos = m_pBoxCollider->Get_MinPos();
	return vMinPos;
}

_vec3 CTriggerCollider::Get_MaxPos()
{
	_vec3 vMaxPos = m_pBoxCollider->Get_MaxPos();
	return vMaxPos;
}

_vec3 CTriggerCollider::Get_WorldPos()
{
	return m_pBoxCollider->Get_ColliderWolrdPos();
}

HRESULT CTriggerCollider::Tick_Collider(_float fTimeDelta)
{
	if (m_pBoxCollider == nullptr)
		MSG_FAIL("CTriggerCollider::Tick_Collider - m_pBoxCollider == nullptr");

	m_pBoxCollider->Set_WorldMatrixFromObject(*m_pTransform->Get_WorldMatrixPtr());
	m_pBoxCollider->Tick(fTimeDelta);

	return S_OK;

}

_vec3 CTriggerCollider::Get_Length()
{
	return m_pTransform->Get_Scale();
}
