#include "stdafx.h"
#include "..\public\TrumpCardHeart.h"
#include "Collision.h"
#include "Player.h"
#include "TileCube.h"
#include "WallLook.h"
#include "WallSide.h"
#include "Attack.h"
CTrumpCardHeart * CTrumpCardHeart::Create(_pDevice pGraphicDevice)
{
	CTrumpCardHeart* pInstance = new CTrumpCardHeart(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CTrumpCardHeart::Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTrumpCardHeart::Clone(void * pArg)
{
	CTrumpCardHeart* pInstance = new CTrumpCardHeart(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CTrumpCardHeart::Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrumpCardHeart::Free()
{
	__super::Free();
}

void CTrumpCardHeart::Collision(CGameObject * pColObj)
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	_uint iCurrentCarmeraID = pInstance->Get_CurrentCameraID();


	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pColObj);
	if (pPlayer != nullptr)
		Collision_Effect(pPlayer);

	if (iCurrentCarmeraID != CAMERA_2D)
	{
		CTileCube* pTileCube = dynamic_cast<CTileCube*>(pColObj);
		if (pTileCube != nullptr)
			Collision_NoPlayer();

		CWallSide* pWallSide = dynamic_cast<CWallSide*>(pColObj);
		if (pWallSide != nullptr)
			Collision_NoPlayer();

		CWallLook* pWallLook = dynamic_cast<CWallLook*>(pColObj);
		if (pWallLook != nullptr)
			Collision_NoPlayer();
	}
	RELEASE_INSTANCE(CGameInstance);
}

CTrumpCardHeart::CTrumpCardHeart(_pDevice pGraphicDevice)
	: CTrumpCard(pGraphicDevice)
{
}

CTrumpCardHeart::CTrumpCardHeart(const CTrumpCardHeart & rhs)
	: CTrumpCard(rhs)
{
}

HRESULT CTrumpCardHeart::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CTrumpCardHeart::NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CTrumpCardHeart::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CTrumpCardHeart::NativeConstruct - __super");

	if(FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CTrumpCardHeart::NativeConstruct - SetUp_Component");

	if (!m_tDesc.bProjectile)
		Direction();

	if (m_tDesc.bProjectile)
		Projectile();

	return S_OK;
}

_int CTrumpCardHeart::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Move(fTimeDelta);
	BillBoarding(m_pTransform);
	return __super::Tick(fTimeDelta);
}

_int CTrumpCardHeart::LateTick(_float fTimeDelta)
{
	return __super::LateTick(fTimeDelta);
}

HRESULT CTrumpCardHeart::Render()
{
	return __super::Render();
}

HRESULT CTrumpCardHeart::Collision_Effect(CPlayer* pPlayerObject)
{
	CAttack::DESC tDesc;
	tDesc.iDmg = m_tDesc.iTexutreNum;
	tDesc.vPos = m_pBoxCollider->Get_ColliderWolrdPos();
	tDesc.vScale = Get_Length();
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeAttack, pTagLayerMonsterAttack, &tDesc);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CTrumpCardHeart::Collision_NoPlayer()
{
	return S_OK;
}

HRESULT CTrumpCardHeart::SetUp_Component(void * pArg)
{
	memcpy(&m_tDesc, (CTrumpCard::DESC*)pArg, sizeof(CTrumpCard::DESC));

	CTransform::TRANSFORM_DESC tTransDesc;
	tTransDesc.fDegree = 5.f;
	tTransDesc.fSpeed = 25.f;

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform, &tTransDesc)))
		MSG_FAIL("CTrumpCardSpade SetUp_Component - Add_Component m_pTransformCom");
	m_pTransform->SetUp_State(CTransform::STATE_POS, m_tDesc.vPos);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CTrumpCardSpade SetUp_Component - Add_Component m_pRendererCom");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CTrumpCardSpade SetUp_Component - Add_Component m_pVIBuffer");

	if (FAILED(Add_Component(LEVEL_BOSS_CIRCUS, pTagPrototypeTextureTrumpHeart, pTagComTexture, (CComponent**)&m_pTexture)))
		MSG_FAIL("CTrumpCardSpade SetUp_Component - Add_Component m_pTexture");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeFrustum, pTagComFrustum, (CComponent**)&m_pFrustum)))
		MSG_FAIL("CTrumpCardSpade SetUp_Component");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, _vec3(0.f, 0.f, 0.f), _vec3(0.1f, 0.7f, 0.7f), D3DCOLOR_ARGB(255, 255, 0, 0), this);

	return S_OK;
}

