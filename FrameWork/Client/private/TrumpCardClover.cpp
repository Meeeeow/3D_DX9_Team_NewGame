#include "stdafx.h"
#include "..\public\TrumpCardClover.h"
#include "Player.h"
#include "WallLook.h"
#include "WallSide.h"
#include "Collision.h"
#include "TileCube.h"

CGameObject * CTrumpCardClover::Clone(void * pArg)
{
	CTrumpCardClover* pInstance = new CTrumpCardClover(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CTrumpCardClover::Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CTrumpCardClover * CTrumpCardClover::Create(_pDevice pGraphicDevice)
{
	CTrumpCardClover* pInstance = new CTrumpCardClover(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CTrumpCardClover::Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrumpCardClover::Free()
{
	__super::Free();
}

CTrumpCardClover::CTrumpCardClover(_pDevice pGraphicDevice)
	: CTrumpCard(pGraphicDevice)
{
}

CTrumpCardClover::CTrumpCardClover(const CTrumpCardClover & rhs)
	: CTrumpCard(rhs)
{
}

HRESULT CTrumpCardClover::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CTrumpCardClover::NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CTrumpCardClover::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CTrumpCardClover::NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CTrumpCardClover::NativeConstruct - SetUp_Component");

	if (!m_tDesc.bProjectile)
		Direction();

	if (m_tDesc.bProjectile)
		Projectile();

	return S_OK;
}

_int CTrumpCardClover::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	


	return __super::Tick(fTimeDelta);
}

_int CTrumpCardClover::LateTick(_float fTimeDelta)
{
	return __super::LateTick(fTimeDelta);
}

HRESULT CTrumpCardClover::Render()
{
	return __super::Render();
}

HRESULT CTrumpCardClover::SetUp_Component(void * pArg)
{
	memcpy(&m_tDesc, (CTrumpCard::DESC*)pArg, sizeof(CTrumpCard::DESC));

	CTransform::TRANSFORM_DESC tTransDesc;
	tTransDesc.fDegree = 5.f;
	tTransDesc.fSpeed = 25.f;

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform, &tTransDesc)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pTransformCom");
	m_pTransform->SetUp_State(CTransform::STATE_POS, m_tDesc.vPos);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pRendererCom");

	if(FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect,pTagComVIBuffer,(CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pVIBuffer");

	if (FAILED(Add_Component(LEVEL_BOSS_CIRCUS, pTagPrototypeTextureTrumpClover, pTagComTexture, (CComponent**)&m_pTexture)))
		MSG_FAIL("CTrumpCardClover SetUp_Component - Add_Component m_pTexture");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeFrustum, pTagComFrustum, (CComponent**)&m_pFrustum)))
		MSG_FAIL("CTrumpCardClover SetUp_Component");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, _vec3(0.f, 0.f, 0.f), _vec3(0.1f, 0.7f, 0.7f), D3DCOLOR_ARGB(255, 0, 0, 0), this);
	
	return S_OK;
}

void CTrumpCardClover::Collision(CGameObject* pCollisionObject)
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	_uint iCurrentCarmeraID = pInstance->Get_CurrentCameraID();


	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pCollisionObject);
	if (pPlayer != nullptr)
		Collision_Effect(pPlayer);

	if (iCurrentCarmeraID != CAMERA_2D)
	{
		CTileCube* pTileCube = dynamic_cast<CTileCube*>(pCollisionObject);
		if (pTileCube != nullptr)
			Collision_NoPlayer();

		CWallSide* pWallSide = dynamic_cast<CWallSide*>(pCollisionObject);
		if (pWallSide != nullptr)
			Collision_NoPlayer();

		CWallLook* pWallLook = dynamic_cast<CWallLook*>(pCollisionObject);
		if (pWallLook != nullptr)
			Collision_NoPlayer();
	}
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CTrumpCardClover::Collision_Effect(class CPlayer * pPlayerObject)
{

	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(pPlayerObject->Get_Component(pTagComTransform));
	m_bDead = true;

	pPlayerTransform->SetUp_DebuffSpeed(_float(m_tDesc.iTexutreNum + 1));
	pPlayerTransform->SetUp_DebuffTime(3.f);

	return S_OK;
}

HRESULT CTrumpCardClover::Collision_NoPlayer()
{
	m_bDead = true;
	return S_OK;
}

