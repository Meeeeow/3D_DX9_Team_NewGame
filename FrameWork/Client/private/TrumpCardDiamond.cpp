#include "stdafx.h"
#include "..\public\TrumpCardDiamond.h"
#include "Player.h"
#include "TileCube.h"
#include "WallLook.h"
#include "WallSide.h"
#include "Collision.h"

CTrumpCardDiamond * CTrumpCardDiamond::Create(_pDevice pGraphicDevice)
{
	CTrumpCardDiamond* pInstance = new CTrumpCardDiamond(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CTrumpCardDiamond::Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTrumpCardDiamond::Clone(void * pArg)
{
	CTrumpCardDiamond* pInstance = new CTrumpCardDiamond(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CTrumpCardDiamond::Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrumpCardDiamond::Free()
{
	__super::Free();
}

void CTrumpCardDiamond::Collision(CGameObject * pColObj)
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	_uint iCurrentCarmeraID = pInstance->Get_CurrentCameraID();
	RELEASE_INSTANCE(CGameInstance);

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
}

CTrumpCardDiamond::CTrumpCardDiamond(_pDevice pGraphicDevice)
	: CTrumpCard(pGraphicDevice)
{
}

CTrumpCardDiamond::CTrumpCardDiamond(const CTrumpCardDiamond & rhs)
	: CTrumpCard(rhs)
{
}

HRESULT CTrumpCardDiamond::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CTrumpCardDiamond::NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CTrumpCardDiamond::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CTrumpCardDiamond::NativeConstruct - __super");

	if (FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CTrumpCardDiamond::NativeConstruct - SetUp_Component");


	if (!m_tDesc.bProjectile)
		Direction();

	if (m_tDesc.bProjectile)
		Projectile();

	return S_OK;
}

_int CTrumpCardDiamond::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Move(fTimeDelta);
	BillBoarding(m_pTransform);

	return __super::Tick(fTimeDelta);
}

_int CTrumpCardDiamond::LateTick(_float fTimeDelta)
{
	return __super::LateTick(fTimeDelta);
}

HRESULT CTrumpCardDiamond::Render()
{
	return __super::Render();
}

HRESULT CTrumpCardDiamond::Collision_Effect(CPlayer * pPlayerObject)
{
	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(pPlayerObject->Get_Component(pTagComTransform));

	if (pPlayerTransform != nullptr)
	{
		pPlayerTransform->SetUp_DebuffSpeed(((_float)m_tDesc.iTexutreNum / 2.f) * -1);
		pPlayerTransform->SetUp_DebuffTime((_float)m_tDesc.iTexutreNum / 2.5f);
	}
	m_bDead = true;
	return S_OK;
}

HRESULT CTrumpCardDiamond::Collision_NoPlayer()
{
	m_bDead = true;

	return S_OK;
}

HRESULT CTrumpCardDiamond::SetUp_Component(void * pArg)
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

	if (FAILED(Add_Component(LEVEL_BOSS_CIRCUS, pTagPrototypeTextureTrumpDiamond, pTagComTexture, (CComponent**)&m_pTexture)))
		MSG_FAIL("CTrumpCardSpade SetUp_Component - Add_Component m_pTexture");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeFrustum, pTagComFrustum, (CComponent**)&m_pFrustum)))
		MSG_FAIL("CTrumpCardSpade SetUp_Component");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, _vec3(0.f, 0.f, 0.f), _vec3(0.1f, 0.7f, 0.7f), D3DCOLOR_ARGB(255, 255, 0, 0), this);

	return S_OK;
}

