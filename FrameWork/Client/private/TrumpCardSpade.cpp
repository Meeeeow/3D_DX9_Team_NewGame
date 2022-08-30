#include "stdafx.h"
#include "..\public\TrumpCardSpade.h"
#include "Player.h"
#include "TileCube.h"
#include "WallLook.h"
#include "WallSide.h"
#include "Collision.h"

CTrumpCardSpade::CTrumpCardSpade(_pDevice pGraphicDevice)
	: CTrumpCard(pGraphicDevice)
{
}

CTrumpCardSpade::CTrumpCardSpade(const CTrumpCardSpade & rhs)
	: CTrumpCard(rhs)
{
}

HRESULT CTrumpCardSpade::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CTrumpCardSpade::NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CTrumpCardSpade::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CTrumpCardSpade::NativeConstruct - __super");

	if(FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CTrumpCardSpade::NativeConstruct - __super");

	if (!m_tDesc.bProjectile)
		Direction();

	if (m_tDesc.bProjectile)
		Projectile();

	return S_OK;
}

_int CTrumpCardSpade::Tick(_float fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	
	Move(fTimeDelta);
	BillBoarding(m_pTransform);
	return __super::Tick(fTimeDelta);
}

_int CTrumpCardSpade::LateTick(_float fTimeDelta)
{
	return __super::LateTick(fTimeDelta);
}

HRESULT CTrumpCardSpade::Render()
{
	return __super::Render();
}

HRESULT CTrumpCardSpade::Collision_Effect()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeEffectBlind, pTagLayerCircusEffect)))
		MSG_FAIL("CTrumpCardSpade::Collision_Effect Add_GameObjectToLayer - pTagPrototypeEffectBlind");
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CTrumpCardSpade::Collision_NoPlayer()
{
	m_bDead = true;
	return S_OK;
}

HRESULT CTrumpCardSpade::SetUp_Component(void * pArg)
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

	if (FAILED(Add_Component(LEVEL_BOSS_CIRCUS, pTagPrototypeTextureTrumpSpade, pTagComTexture, (CComponent**)&m_pTexture)))
		MSG_FAIL("CTrumpCardSpade SetUp_Component - Add_Component m_pTexture");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeFrustum, pTagComFrustum, (CComponent**)&m_pFrustum)))
		MSG_FAIL("CTrumpCardSpade SetUp_Component");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, _vec3(0.f, 0.f, 0.f), _vec3(0.1f, 0.7f, 0.7f), D3DCOLOR_ARGB(255, 0, 0, 0), this);

	return S_OK;
}


CGameObject * CTrumpCardSpade::Clone(void * pArg)
{
	CTrumpCardSpade* pInstance = new CTrumpCardSpade(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CTrumpCardSpade::Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTrumpCardSpade * CTrumpCardSpade::Create(_pDevice pGraphicDevice)
{
	CTrumpCardSpade* pInstance = new CTrumpCardSpade(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CTrumpCardSpade::Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrumpCardSpade::Free()
{
	__super::Free();
}

void CTrumpCardSpade::Collision(CGameObject * pColObj)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_uint iCurrentCameraID = pGameInstance->Get_CurrentCameraID();
	RELEASE_INSTANCE(CGameInstance);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pColObj);
	if (pPlayer != nullptr)
		Collision_Effect();

	if (iCurrentCameraID != CAMERA_2D)
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
