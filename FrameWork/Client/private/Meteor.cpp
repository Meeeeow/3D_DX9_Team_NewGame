#include "stdafx.h"
#include "..\public\Meteor.h"
#include "Monster.h"
#include "Attack.h"
#include "Explosion.h"
#include "TileCube.h"
#include "CameraTargetBack.h"

CMeteor::CMeteor(_pDevice pGraphicDevice)
	: CCollisionCubeObject(pGraphicDevice)
{
}

CMeteor::CMeteor(const CMeteor & rhs)
	: CCollisionCubeObject(rhs)
{
}

HRESULT CMeteor::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CMeteor::NativeConstruct_Prototype - __super");
	return S_OK;
}

HRESULT CMeteor::NativeConstruct(void * pArg)
{
	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CMeteor::NativeConstruct - SetUp_Component");

	return S_OK;
}

_int CMeteor::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Move(fTimeDelta);
	TextureChange();

	return _int();
}

_int CMeteor::LateTick(_float fTimeDelta)
{
	if (m_pFrustum->Check_Culling(m_pTransform, m_fFrustumRadius))
		m_pRenderer->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this);

	return _int();
}

HRESULT CMeteor::Render()
{
	m_pTransform->Bind_OnDevice();
	m_pTexture->Bind_OnDevice(m_iTextureIndex);

	m_pVIBuffer->Render();
	m_pBoxCollider->Render();

	return S_OK;
}

HRESULT CMeteor::SetUp_Component(void * pArg)
{
	m_tDesc = *(DESC*)pArg;
	CTransform::TRANSFORM_DESC m_tTransformDesc;
	m_tTransformDesc.fDegree = 33.f + (rand() % 4);
	m_tTransformDesc.fSpeed = 10.f + (rand() % 4);

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeTransform"), TEXT("ComTransform"), (CComponent**)&m_pTransform,&m_tTransformDesc)))
		MSG_FAIL("CRedBlock SetUp_Component - Add_Component m_pTransformCom");
	m_pTransform->SetUp_State(CTransform::STATE_POS, m_tDesc.vPos);
	m_pTransform->SetUp_Scale(_vec3(3.f, 3.f, 3.f));

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeRenderer"), TEXT("ComRenderer"), (CComponent**)&m_pRenderer)))
		MSG_FAIL("CRedBlock SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeFrustum"), TEXT("ComFrustum"), (CComponent**)&m_pFrustum)))
		MSG_FAIL("CRedBlock SetUp_Component - Add_Component m_pFrustumCom");

	if (FAILED(Add_Component(LEVEL_STATIC, TEXT("PrototypeVIBufferCube"), TEXT("ComVIBuffer"), (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CRedBlock SetUp_Component - Add_Component m_pVIBufferCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTextureMeteor, TEXT("ComTexture"), (CComponent**)&m_pTexture)))
		MSG_FAIL("CRedBlock SetUp_Component - Add_Component m_pTextureCom");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, _vec3(0.f,0.f,0.f), _vec3(1.f,1.f,1.f), D3DCOLOR_ARGB(255,255,0,0), this);

	m_fFrustumRadius = 14.f;
	m_plane[CBoxCollider::PLANE_FRONT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_FRONT);
	m_plane[CBoxCollider::PLANE_BACK] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_BACK);
	m_plane[CBoxCollider::PLANE_LEFT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_LEFT);
	m_plane[CBoxCollider::PLANE_RIGHT] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_RIGHT);
	m_plane[CBoxCollider::PLANE_TOP] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_TOP);
	m_plane[CBoxCollider::PLANE_BOTTOM] = m_pBoxCollider->Get_PlaneDir(CBoxCollider::PLANE_BOTTOM);


	m_tDesc.vDir = m_tDesc.vTargetPos - m_tDesc.vPos;
	m_fMaxRange = D3DXVec3Length(&m_tDesc.vDir);
	D3DXVec3Normalize(&m_tDesc.vDir, &m_tDesc.vDir);


	return S_OK;
}

void CMeteor::Move(_float fTimeDelta)
{
	m_pTransform->Rotation_Axis(_vec3(0.f, 1.f, 0.f), fTimeDelta);
	_float fDistance = m_pTransform->Go_Dir(fTimeDelta, m_tDesc.vDir);

	m_fRange += fDistance;

	if (m_fRange >= m_fMaxRange + 20.f)
		Set_Dead();
}

void CMeteor::TextureChange()
{
	++m_iTextureIndex;
	if (m_iTextureIndex > 1)
		m_iTextureIndex = 0;
}

CMeteor * CMeteor::Create(_pDevice pGraphicDevice)
{
	CMeteor* pInstance = new CMeteor(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CMeteor::Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CMeteor * CMeteor::Clone(void * pArg)
{
	CMeteor* pInstance = new CMeteor(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CMeteor::Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMeteor::Free()
{
	Safe_Release(m_pTexture);
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTransform);
	Safe_Release(m_pVIBuffer);
	__super::Free();
}

void CMeteor::Collision(CGameObject * pColObj)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CMonster* pMonster = dynamic_cast<CMonster*>(pColObj);
	CTileCube* pTileCube = dynamic_cast<CTileCube*>(pColObj);
	if (pMonster != nullptr)
	{
		CExplosion::DESC tEffect;
		tEffect.vPos = m_pTransform->Get_State(CTransform::STATE_POS);
		tEffect.vSize = _vec3(8.f,8.f,8.f);

		pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("PrototypeExplosion"), L"LayerEffect", &tEffect);

		CAttack::DESC tAttack;
		tAttack.bDestroy = true;
		tAttack.iDmg = 12;
		tAttack.vPos = m_pTransform->Get_State(CTransform::STATE_POS);
		tAttack.vScale = m_pTransform->Get_Scale();
		pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeAttack, pTagLayerPlayerAttack, &tAttack);
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
		CSoundMgr::Get_Instance()->PlaySound(L"METEORBOOM.wav", CSoundMgr::EFFECT);
		Set_Dead();
	}
	if (pTileCube != nullptr)
	{
		CExplosion::DESC tEffect;
		tEffect.vPos = m_pTransform->Get_State(CTransform::STATE_POS);
		tEffect.vSize = _vec3(8.f, 8.f, 8.f);

		pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("PrototypeExplosion"), L"LayerEffect", &tEffect);

		CAttack::DESC tAttack;
		tAttack.bDestroy = true;
		tAttack.iDmg = 12;
		tAttack.vPos = m_pTransform->Get_State(CTransform::STATE_POS);
		tAttack.vScale = m_pTransform->Get_Scale();
		pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeAttack, pTagLayerPlayerAttack, &tAttack);
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
		CSoundMgr::Get_Instance()->PlaySound(L"METEORBOOM.wav", CSoundMgr::EFFECT);
		Set_Dead();
	}

	if (pTileCube != nullptr || pMonster != nullptr)
	{
		CCameraTargetBack* pCamera = dynamic_cast<CCameraTargetBack*>(pGameInstance->Get_CurrentCamera());
		pCamera->Set_Quake();
	}
	RELEASE_INSTANCE(CGameInstance);
}

_vec3 CMeteor::Get_MinPos()
{
	return m_pBoxCollider->Get_MinPos();
}

_vec3 CMeteor::Get_MaxPos()
{
	return m_pBoxCollider->Get_MaxPos();
}

_vec3 CMeteor::Get_WorldPos()
{
	return m_pBoxCollider->Get_ColliderWolrdPos();
}

HRESULT CMeteor::Tick_Collider(_float fTimeDelta)
{
	m_pBoxCollider->Set_WorldMatrixFromObject(*m_pTransform->Get_WorldMatrixPtr());
	m_pBoxCollider->Tick(fTimeDelta);

	return S_OK;
}
