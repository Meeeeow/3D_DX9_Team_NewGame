#include "stdafx.h"
#include "..\public\Portal.h"
#include "VIBufferCubeColor.h"
#include "Player.h"
#include "Level_Loading.h"

CPortal::CPortal(_pDevice pGraphicDevice)
	: CGameObject(pGraphicDevice)
{
}

CPortal::CPortal(const CPortal & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPortal::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CPortal::NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CPortal::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CPortal::NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CPortal::NativeConstruct - SetUp_Component");

	return S_OK;
}

_int CPortal::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	AlphaChange();

	return OBJ_NOEVENT;
}

_int CPortal::LateTick(_float fTimeDelta)
{

	if (m_pFrustum->Check_Culling(m_pTransform, 10.f))
		m_pRenderer->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this);

	return 0;
}

HRESULT CPortal::Render()
{
	m_pTransform->Bind_OnDevice();
	SetUp_RenderState();
	m_pGraphicDevice->SetTexture(0, nullptr);
	m_pVIBuffer->Render();
	Release_RenderState();
	m_pBoxCollider->Render();

	return S_OK;
}

void CPortal::AlphaChange()
{
	m_pVIBuffer->AlphaChange();
}

HRESULT CPortal::SetUp_Component(void * pArg)
{
	m_tPortalInfo = *(DESC*)pArg;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransform)))
		MSG_FAIL("CPortal SetUp_Component - Add_Component m_pTransformCom");
	m_pTransform->SetUp_State(CTransform::STATE_POS, m_tPortalInfo.vPos);
	m_pTransform->SetUp_Scale(m_tPortalInfo.vScale);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRenderer)))
		MSG_FAIL("CPortal SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeFrustum"), TEXT("ComFrustum"), (CComponent**)&m_pFrustum)))
		MSG_FAIL("CPortal SetUp_Component - Add_Component m_pFrustumCom");

	if (FAILED(Add_Component(LEVEL_STATIC, m_tPortalInfo.szBufferName, TEXT("ComVIBuffer"), (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CPortal SetUp_Component - Add_Component m_pVIBufferCom");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, _vec3(0.f, 0.f, 0.f), _vec3(1.f, 1.f, 1.f), D3DCOLOR_ARGB(255,255,255,255), this);

	m_plane[CBoxCollider::PLANE_FRONT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_FRONT);
	m_plane[CBoxCollider::PLANE_BACK] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_BACK);
	m_plane[CBoxCollider::PLANE_LEFT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_LEFT);
	m_plane[CBoxCollider::PLANE_RIGHT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_RIGHT);
	m_plane[CBoxCollider::PLANE_TOP] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_TOP);
	m_plane[CBoxCollider::PLANE_BOTTOM] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_BOTTOM);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPortal::SetUp_RenderState()
{
	//m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE);
	//m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 0);
	//m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_NONE);

	//return S_OK;
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CExplosion Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
		MSG_FAIL("CExplosion::Set_RenderState - SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)))
		MSG_FAIL("CExplosion::Set_RenderState - SetRenderState(D3DRS_SRCBLEND, D3DBLEND::D3DBLEND_SRCALPHA)");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)))
		MSG_FAIL("CExplosion::Set_RenderState - SetRenderState(D3DRS_DESTBLEND, D3DBLEND::D3DBLEND_INVSRCALPHA)");

	return S_OK;
}

HRESULT CPortal::Release_RenderState()
{
	/*m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_CCW);
	return S_OK;*/
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CExplosion Set_RenderState - m_pGraphicDevice is null");

	if (FAILED(m_pGraphicDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
		MSG_FAIL("CExplosion::Set_RenderState - SetRenderState");

	return S_OK;
}

void CPortal::Collision(CGameObject * pColObj)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pColObj);
	_uint iCurrentCamera = pGameInstance->Get_CurrentCameraID();

	if (pPlayer != nullptr && iCurrentCamera == CAMERA_BACK)
	{
		if (!m_bUse)
		{
			CLevel* pLevel = CLevel_Loading::Create(m_pGraphicDevice, m_tPortalInfo.eNextLevel, LEVEL_LOADING);
			if (pLevel == nullptr)
				MSGBOX("pLevel is nullptr");

			if (D3DXVec3Length(&m_tPortalInfo.vTrans) != 0)
			{
				pPlayer->Set_Pos(m_tPortalInfo.vTrans, dynamic_cast<CTransform*>(pPlayer->Get_Component(pTagComTransform)));
				pPlayer->On_theCube();
			}

			if (FAILED(pGameInstance->Change_Level(pLevel)))
				MSGBOX("Failed to Portal");

			Set_Dead();
			m_bUse = true;
		}
	}
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CPortal::Tick_Collider(_float fTimeDelta)
{
	m_pBoxCollider->Set_WorldMatrixFromObject(*m_pTransform->Get_WorldMatrixPtr());
	m_pBoxCollider->Tick(fTimeDelta);

	m_plane[CBoxCollider::PLANE_FRONT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_FRONT);
	m_plane[CBoxCollider::PLANE_BACK] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_BACK);
	m_plane[CBoxCollider::PLANE_LEFT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_LEFT);
	m_plane[CBoxCollider::PLANE_RIGHT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_RIGHT);
	m_plane[CBoxCollider::PLANE_TOP] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_TOP);
	m_plane[CBoxCollider::PLANE_BOTTOM] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_BOTTOM);

	return S_OK;
}

_vec3 CPortal::Get_MinPos()
{
	return m_pBoxCollider->Get_MinPos();;
}

_vec3 CPortal::Get_MaxPos()
{
	return m_pBoxCollider->Get_MaxPos();
}

_vec3 CPortal::Get_WorldPos()
{
	return m_pBoxCollider->Get_ColliderWolrdPos();
}

_vec3 CPortal::Get_Length()
{
	return m_pTransform->Get_Scale();
}

CGameObject * CPortal::Clone(void * pArg)
{
	CPortal* pInstance = new CPortal(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CPortal::Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CPortal * CPortal::Create(_pDevice pGraphicDevice)
{
	CPortal* pInstance = new CPortal(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CPortal::Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPortal::Free()
{
	Safe_Release(m_pFrustum);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pTransform);
	__super::Free();
}
