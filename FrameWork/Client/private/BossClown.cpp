#include "stdafx.h"
#include "..\public\BossClown.h"
#include "Player.h"
#include "TrumpCard.h"
#include "Attack.h"
#include "DmgFont.h"
#include "Camera2D.h"
#include "CameraTargetBack.h"
#include "FireCracker.h"
#include "Portal.h"
#include "CardLost.h"
#include "TutorialMgr.h"

CBossClown::CBossClown(_pDevice pGraphicDevice)
	: CMonster(pGraphicDevice)
{
	ZeroMemory(m_pTexture, sizeof(m_pTexture));
}

CBossClown::CBossClown(const CBossClown & rhs)
	: CMonster(rhs)
{
}

HRESULT CBossClown::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		MSG_FAIL("CBossClown::NativeConstruct_Prototype - __super");

	return S_OK;
}

HRESULT CBossClown::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		MSG_FAIL("CBossClown::NativeConstruct - __super");

	if(FAILED(SetUp_Component(pArg)))
		MSG_FAIL("CBossClown::NativeConstruct - SetUp_Component");

	return S_OK;
}

_int CBossClown::Tick(_float fTimeDelta)
{
	if (m_bDead)
	{
		Circus_Epilouge();
		return OBJ_DEAD;
	}

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	Performance(fTimeDelta,pGameInstance);

	BillBoarding(m_pTransform);
	Check_Frame();

	Tick_Frame(fTimeDelta,pGameInstance);
	
	RELEASE_INSTANCE(CGameInstance);

	return OBJ_NOEVENT;
}

_int CBossClown::LateTick(_float fTimeDelta)
{
	if (m_pFrustum->Check_Culling(m_pTransform, 25.f))
		m_pRenderer->Add_RenderGroup(CRenderer::GROUP_NONALPHA, this);

	m_pCardBoard->LateTick(fTimeDelta);

	return 0;
}

HRESULT CBossClown::Render()
{
	if (FAILED(__super::Render()))
		MSG_FAIL("CBossClown Render - __super");

	if (m_pTransform == nullptr ||
		m_pTexture == nullptr ||
		m_pBoxCollider == nullptr ||
		m_pVIBuffer == nullptr)
		MSG_FAIL("CBossClown::Render - Com is null");


	if (!m_bIntroSound)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"CIRCUS_INTRO.wav", CSoundMgr::CLOWN_INTRO);
		Welcome_ToMyCircus();
		m_bIntroSound = true;
	}


	if (FAILED(m_pTransform->Bind_OnDevice()))
		MSG_FAIL("CBossClown::Render - m_pTransformCom Bind_OnDevice");

	if (FAILED(m_pTexture[m_eCurState]->Bind_OnDevice(m_tFrame.iStart)))
		MSG_FAIL("CBossClown::Render - m_pTexture[m_eCurState]->Bind_OnDevice");

	if (FAILED(SetUp_RenderState()))
		MSG_FAIL("m_pTransform::Render - SetUp_RenderState");

	if (FAILED(m_pVIBuffer->Render()))
		MSG_FAIL("CBossClown::Render - m_pVIBufferCom->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CBossClown::Render - Release_RenderState");

	if (FAILED(m_pBoxCollider->Render()))
		MSG_FAIL("CBossClown::Render - m_pBoxColliderCom->Render");

	return S_OK;
}

_bool CBossClown::Picking(const _vec3 & vMouseRay, const _vec3 & vMousePivot, CTransform * pOut)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_GameObjPtr(LEVEL_STATIC, pTagLayerPlayer, 0));
	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(pPlayer->Get_Component(pTagComTransform));
	RELEASE_INSTANCE(CGameInstance);

	_vec3 vPlayerPos = pPlayer->Get_WorldPos();
	_vec3 vClownPos = Get_WorldPos();

	_float fDistance = D3DXVec3Length(&(vPlayerPos - vClownPos));

	if (abs(fDistance) >= 20.f)
		return false;

	_vec3 vPos;
	if (m_pVIBuffer->Picking(vMouseRay, vMousePivot, &vPos))
	{
		pOut = dynamic_cast<CTransform*>(Get_Component(pTagComTransform));
		return true;
	}
	return false;
}


HRESULT CBossClown::SetUp_Component(void * pArg)
{
	OBJ_INFO tObjInfo = *(OBJ_INFO*)pArg;
	CTransform::tagTransformDesc tTransformDesc;
	tTransformDesc.fDegree = 5.f;
	tTransformDesc.fSpeed = 0.f;

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, pTagComTransform, (CComponent**)&m_pTransform, &tTransformDesc)))
		MSG_FAIL("CBossClown::SetUp_Component - m_pTransform");

	_vec3 vStatePos = tObjInfo.Get_State(tagObjectInfo::STATE_POS);
	vStatePos.y += 5;
	m_pTransform->SetUp_State(CTransform::STATE_POS,vStatePos);
	m_pTransform->SetUp_Scale(10.f, 10.f, 1.f);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeRenderer, pTagComRenderer, (CComponent**)&m_pRenderer)))
		MSG_FAIL("CBossClown::SetUp_Component - m_pRenderer");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, pTagComVIBuffer, (CComponent**)&m_pVIBuffer)))
		MSG_FAIL("CBossClown::SetUp_Component - m_pVIBuffer");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeFrustum, pTagComFrustum, (CComponent**)&m_pFrustum)))
		MSG_FAIL("CBossClown::SetUp_Component - m_pFrustum");

	wstring wstrTextureName;

	wstrTextureName = tObjInfo.szTextureName;
	wstrTextureName += L"Idle";
	if(FAILED(Add_Component(LEVEL_BOSS_CIRCUS, wstrTextureName.c_str(), L"ComTextureIdle", (CComponent**)&m_pTexture[FSM_CLOWN_IDLE])))
		MSG_FAIL("CBossClown::SetUp_Component - m_pTexture Idle");

	wstrTextureName = tObjInfo.szTextureName;
	wstrTextureName += L"Dance";
	if (FAILED(Add_Component(LEVEL_BOSS_CIRCUS, wstrTextureName.c_str(), L"ComTextureDance", (CComponent**)&m_pTexture[FSM_CLOWN_DANCE])))
		MSG_FAIL("CBossClown::SetUp_Component - m_pTexture Dance");

	wstrTextureName = tObjInfo.szTextureName;
	wstrTextureName += L"Dodge";
	if (FAILED(Add_Component(LEVEL_BOSS_CIRCUS, wstrTextureName.c_str(), L"ComTextureDodge", (CComponent**)&m_pTexture[FSM_CLOWN_DODGE])))
		MSG_FAIL("CBossClown::SetUp_Component - m_pTexture Dodge");

	wstrTextureName = tObjInfo.szTextureName;
	wstrTextureName += L"Attack";
	if (FAILED(Add_Component(LEVEL_BOSS_CIRCUS, wstrTextureName.c_str(), L"ComTextureAttack", (CComponent**)&m_pTexture[FSM_CLOWN_ATTACK])))
		MSG_FAIL("CBossClown::SetUp_Component - m_pTexture Attack");

	wstrTextureName = tObjInfo.szTextureName;
	wstrTextureName += L"Dead";
	if (FAILED(Add_Component(LEVEL_BOSS_CIRCUS, wstrTextureName.c_str(), L"ComTextureDead", (CComponent**)&m_pTexture[FSM_CLOWN_DEAD])))
		MSG_FAIL("CBossClown::SetUp_Component - m_pTexture Dead");

	m_pBoxCollider = CBoxCollider::Create_Tool(m_pGraphicDevice, _vec3(0.f,0.f,0.f), _vec3(1.f,1.f,3.f), tObjInfo.dwColliderColor, this);

	m_pCardBoard = CCardBoard::Create(m_pGraphicDevice);

	for (int i = 0; i < CARDCOUNTMAX; i++)
		m_bLostCardNumbers[i] = false;

	return S_OK;
}

void CBossClown::Performance(_float fTimeDelta, CGameInstance* pGameInstance)
{
	Shuffle(fTimeDelta);
	Fall(fTimeDelta, pGameInstance);
}

HRESULT CBossClown::Shuffle(_float fTimeDelta)
{
	_int iExit = 0;
	for (int i = 0; i < CARDCOUNTMAX; ++i)
		if (m_bLostCardNumbers[i])
			++iExit;

	if (iExit == CARDCOUNTMAX)
		return S_OK;

	if (m_tCardDeck.bShuffle && m_eCurState == FSM_CLOWN_IDLE)
	{
		m_tCardDeck.iCardObjectsNum += rand() % 4;
		if (m_tCardDeck.iCardObjectsNum >= 4)
			m_tCardDeck.iCardObjectsNum -= 4;

		m_tCardDeck.iCardTextureNum += rand() % 13;
		if (m_tCardDeck.iCardTextureNum >= 13)
			m_tCardDeck.iCardTextureNum -= 13;

		if (Get_Card())
			Shuffle(fTimeDelta);

		m_tCardDeck.fShuffleCount -= fTimeDelta;
		m_pCardBoard->SetUp_CardSelectPoision(m_tCardDeck.iCardObjectsNum, m_tCardDeck.iCardTextureNum);
		if (m_tCardDeck.fShuffleCount <= 0.f)
		{
			m_tCardDeck.fShuffleCount = 7.f;
			m_tCardDeck.bShuffle = false;
			Set_State(FSM_CLOWN_ATTACK);
		}
	}

	return S_OK;
}

HRESULT CBossClown::Draw(CGameInstance* pGameInstance)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_GameObjPtr(LEVEL_STATIC, pTagLayerPlayer, 0));
	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(pPlayer->Get_Component(pTagComTransform));
	CTrumpCard::DESC tDesc;
	tDesc.vPos = m_pTransform->Get_State(CTransform::STATE_POS);
	tDesc.vTargetPos = pPlayerTransform->Get_State(CTransform::STATE_POS);
	tDesc.vTargetPos.y += 1.0f;
	tDesc.iTexutreNum = m_tCardDeck.iCardTextureNum;
	Lost_Card(false);
	CSoundMgr::Get_Instance()->PlaySound(L"CIRCUS_ATTACK.wav", CSoundMgr::CLOWN_ATTACK);
	if (m_tCardDeck.iCardObjectsNum == 0)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeCircusCardSpade, pTagLayerMonsterAttack, &tDesc)))
			MSG_FAIL("CBossClown::Draw Add_GameObjectToLayer - pTagPrototypeCircusCardClover");
	}
	else if (m_tCardDeck.iCardObjectsNum == 1)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeCircusCardHeart, pTagLayerMonsterAttack, &tDesc)))
			MSG_FAIL("CBossClown::Draw Add_GameObjectToLayer - pTagPrototypeCircusCardSpade");
	}
	else if (m_tCardDeck.iCardObjectsNum == 2)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeCircusCardClover, pTagLayerMonsterAttack, &tDesc)))
			MSG_FAIL("CBossClown::Draw Add_GameObjectToLayer - pTagPrototypeCircusCardHeart");
	}
	else if (m_tCardDeck.iCardObjectsNum == 3)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeCircusCardDiamond, pTagLayerMonsterAttack, &tDesc)))
			MSG_FAIL("CBossClown::Draw Add_GameObjectToLayer - pTagPrototypeCircusCardDiamond");
	}

	return S_OK;
}

HRESULT CBossClown::Falling(CGameInstance* pGameInstance)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_GameObjPtr(LEVEL_STATIC, pTagLayerPlayer, 0));
	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(pPlayer->Get_Component(pTagComTransform));
	CTrumpCard::DESC tDesc;
	tDesc.bProjectile = true;
	tDesc.vPos = m_pTransform->Get_State(CTransform::STATE_POS);
	tDesc.vTargetPos = pPlayerTransform->Get_State(CTransform::STATE_POS);
	tDesc.vTargetPos.y += 1.0f;
	tDesc.iTexutreNum = m_tCardDeck.iCardTextureNum;
	Lost_Card(false);
	CSoundMgr::Get_Instance()->PlaySound(L"CIRCUS_NEW_ACTOR.wav", CSoundMgr::CLOWN_NEWACTOR);
	if (m_tCardDeck.iCardObjectsNum == 0)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeCircusCardSpade, pTagLayerMonsterAttack, &tDesc)))
			MSG_FAIL("CBossClown::Draw Add_GameObjectToLayer - pTagPrototypeCircusCardClover");
	}
	else if (m_tCardDeck.iCardObjectsNum == 1)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeCircusCardHeart, pTagLayerMonsterAttack, &tDesc)))
			MSG_FAIL("CBossClown::Draw Add_GameObjectToLayer - pTagPrototypeCircusCardSpade");
	}
	else if (m_tCardDeck.iCardObjectsNum == 2)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeCircusCardSpade, pTagLayerMonsterAttack, &tDesc)))
			MSG_FAIL("CBossClown::Draw Add_GameObjectToLayer - pTagPrototypeCircusCardHeart");
	}
	else if (m_tCardDeck.iCardObjectsNum == 3)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeCircusCardDiamond, pTagLayerMonsterAttack, &tDesc)))
			MSG_FAIL("CBossClown::Draw Add_GameObjectToLayer - pTagPrototypeCircusCardDiamond");
	}


	return S_OK;
}

HRESULT CBossClown::Fall(_float fTimeDelta, CGameInstance* pGameInstance)
{
	if (m_pTransform->Get_State(CTransform::STATE_POS).x >= 123.f || m_bDead)
		return S_OK;

	CCameraTargetBack* pCameraBack = dynamic_cast<CCameraTargetBack*>(pGameInstance->Get_CurrentCamera());
	if (pCameraBack != nullptr)
		pGameInstance->Change_Camera(CAMERA_2D);

	m_pTransform->Add_Position(_vec3(0.f, -0.15f, 0.f));
	Set_State(CLOWN_FSM::FSM_CLOWN_DEAD);
	CSoundMgr::Get_Instance()->PlaySound(L"CIRCUS_DEAD.wav", CSoundMgr::CLOWN_DEAD);
	m_fDeadTime -= fTimeDelta;
	CCamera2D* pCamera2D = dynamic_cast<CCamera2D*>(pGameInstance->Get_CurrentCamera());

	if (m_fDeadTime <= 0.5f)
	{
		Set_Dead();
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_GameObjPtr(LEVEL_STATIC, pTagLayerPlayer, 0));
		pCamera2D->Set_TargetObject(pPlayer);
		pCamera2D->Cam_SyncReset();
		pPlayer->Set_Pos(_vec3(8.f, 5.f, 4.f), dynamic_cast<CTransform*>(pPlayer->Get_Component(pTagComTransform)));
		pGameInstance->Change_Camera(CAMERA_BACK);

		return S_OK; 
	}

	if (pCamera2D != nullptr)
	{
		pCamera2D->Set_TargetObject(this);
		pCamera2D->Cam_SyncReset();
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_GameObjPtr(LEVEL_STATIC, pTagLayerPlayer, 0));
		//pPlayer->Set_Pos(_vec3(134.f, 6.f, 4.f), dynamic_cast<CTransform*>(pPlayer->Get_Component(pTagComTransform)));
	}
	
	return S_OK;
}

void CBossClown::Welcome_ToMyCircus()
{
	CGameInstance * pGameInstance = GET_INSTANCE(CGameInstance);
	_float fBaseAngle;
	for (size_t i = 0; i < 12; ++i)
	{
		fBaseAngle = 30.f * i;

		//10°³¾¿ »çÃâ Èû, °¢µµ ·£´ý
		for (size_t j = 0; j < 20; j++)
		{
			int iRandAngle = rand() % 30;
			int iRandPower = rand() % 5000;
			_float fAngle = fBaseAngle + iRandAngle;
			_float fPower = (_float)iRandPower / 500.f;

			CFireCracker::DESC tDesc;
			tDesc.fPower = fPower;
			lstrcpy(tDesc.tObjInfo.szTextureName, pTagPrototypeTextureFireCracker);
			D3DXMatrixIdentity(&tDesc.tObjInfo.matWorld);
			tDesc.tObjInfo.iTextureIndex = rand() % 2;

			if (tDesc.tObjInfo.iTextureIndex == 0)
				tDesc.tObjInfo.SetUp_Scale(0.35f, 0.35f, 0.35f);
			else
				tDesc.tObjInfo.SetUp_Scale(1.f, 1.f, 1.f);

			_vec3 vPos = m_pTransform->Get_State(CTransform::STATE_POS);
			vPos.y += 1;
			vPos.x -= 3;
			tDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, vPos);
			tDesc.tObjInfo.SetUp_Rotation(_vec3(0.f, 1.f, 0.f), fAngle);

			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeFireCracker, pTagLayerNonCollision, &tDesc)))
				MSG_VOID("CPoisonCube::Tick_Particle - pTagPrototypePoisonCubeParticle");
		}
	}
	RELEASE_INSTANCE(CGameInstance);
}

void CBossClown::IsThis_An_Attack_QuestionMark(_vec3 vPos)
{
	CGameInstance * pGameInstance = GET_INSTANCE(CGameInstance);
	_float fBaseAngle;
	for (size_t i = 0; i < 12; ++i)
	{
		fBaseAngle = 30.f * i;

		//10°³¾¿ »çÃâ Èû, °¢µµ ·£´ý
		for (size_t j = 0; j < 15; j++)
		{
			int iRandAngle = rand() % 30;
			int iRandPower = rand() % 5000 ;
			_float fAngle = fBaseAngle + iRandAngle;
			_float fPower = (_float)iRandPower / 1500.f;

			CFireCracker::DESC tDesc;
			tDesc.fPower = fPower;
			lstrcpy(tDesc.tObjInfo.szTextureName, pTagPrototypeTextureConfetti);
			D3DXMatrixIdentity(&tDesc.tObjInfo.matWorld);
			tDesc.tObjInfo.iTextureIndex = rand() % 3;
			tDesc.tObjInfo.SetUp_Scale(0.5f, 0.5f, 0.5f);
			
			tDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, vPos);
			tDesc.tObjInfo.SetUp_Rotation(_vec3(0.f, 1.f, 0.f), fAngle);

			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeConfetti , pTagLayerNonCollision, &tDesc)))
				MSG_VOID("CPoisonCube::Tick_Particle - pTagPrototypePoisonCubeParticle");
		}
	}
	RELEASE_INSTANCE(CGameInstance);
}

void CBossClown::Circus_Epilouge()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pInstance->Get_GameObjPtr(LEVEL_STATIC, pTagLayerPlayer, 0));
	
	pPlayer->Set_Pos(_vec3(134.f, 2.f, 4.f), dynamic_cast<CTransform*>(pPlayer->Get_Component(pTagComTransform)));
	CPortal::DESC tOuterPortal;
	tOuterPortal.eNextLevel = LEVEL::LEVEL_BOSS_SKELETON;
	tOuterPortal.vPos = _vec3(139.5f, 2.f, 4.f);

	lstrcpy(tOuterPortal.szBufferName, L"PrototypeVIBufferCubeColorC");
	tOuterPortal.vScale = _vec3(2.f, 2.f, 2.f);
	tOuterPortal.vTrans = _vec3(0.f, 0.f, 0.f);
	if (FAILED(pInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypePortal, L"LayerPortal", &tOuterPortal)))
		MSGBOX("CBossClown Circus_Epilouge - Add_GameObjectToLayer pTagPrototypePortal");

	RELEASE_INSTANCE(CGameInstance);
}

void CBossClown::Lost_Card(_bool bVisible)
{
	m_bLostCardNumbers[(m_tCardDeck.iCardObjectsNum * 13) + m_tCardDeck.iCardTextureNum] = true;
	CCardLost::DESC tDesc;
	tDesc.iCardObjectNum = m_tCardDeck.iCardObjectsNum;
	tDesc.iCardTextureNum = m_tCardDeck.iCardTextureNum;
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, L"PrototypeCardLost", pTagLayerNonCollision, &tDesc);
	RELEASE_INSTANCE(CGameInstance);
	if(bVisible)
		Lost_CardVisible();
}

void CBossClown::Lost_CardVisible()
{
	_int iExit = 0;
	for (int i = 0; i < CARDCOUNTMAX; ++i)
		if (m_bLostCardNumbers[i])
			++iExit;
	if (iExit == CARDCOUNTMAX)
		return;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CTrumpCard::DESC tDesc;
	tDesc.bProjectile = true;
	tDesc.vPos = m_pTransform->Get_State(CTransform::STATE_POS);
	tDesc.vTargetPos = m_pTransform->Get_State(CTransform::STATE_POS);
	tDesc.vPos.y += 1.0f;
	tDesc.vTargetPos.y -= 1.0f;
	tDesc.vTargetPos.x -= 2.f;
	tDesc.iTexutreNum = m_tCardDeck.iCardTextureNum;
	
	if (m_tCardDeck.iCardObjectsNum == 0)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeCircusCardSpade, pTagLayerMonsterAttack, &tDesc)))
			MSG_VOID("CBossClown::Draw Add_GameObjectToLayer - pTagPrototypeCircusCardClover");
	}
	else if (m_tCardDeck.iCardObjectsNum == 1)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeCircusCardHeart, pTagLayerMonsterAttack, &tDesc)))
			MSG_VOID("CBossClown::Draw Add_GameObjectToLayer - pTagPrototypeCircusCardSpade");
	}
	else if (m_tCardDeck.iCardObjectsNum == 2)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeCircusCardSpade, pTagLayerMonsterAttack, &tDesc)))
			MSG_VOID("CBossClown::Draw Add_GameObjectToLayer - pTagPrototypeCircusCardHeart");
	}
	else if (m_tCardDeck.iCardObjectsNum == 3)
	{
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeCircusCardDiamond, pTagLayerMonsterAttack, &tDesc)))
			MSG_VOID("CBossClown::Draw Add_GameObjectToLayer - pTagPrototypeCircusCardDiamond");
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CBossClown::Dodge()
{
	if (m_ePreState == FSM_CLOWN_ATTACK || m_ePreState == FSM_CLOWN_DANCE || m_ePreState == FSM_CLOWN_DODGE)
		return S_OK;

	Set_State(FSM_CLOWN_DODGE);
	Lost_Card(true);

	CSoundMgr::Get_Instance()->PlaySound(L"CIRCUS_DODGE.wav", CSoundMgr::CLOWN_DODGE);

	

	return S_OK;
}

HRESULT CBossClown::Sub_Hp(_uint iDmg)
{
	m_tStat.iHp -= 0;

	Dodge();

	return S_OK;;
}

void CBossClown::Set_State(CLOWN_FSM eNextState)
{
	m_eCurState = eNextState;
}

void CBossClown::Check_Frame()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case Client::CBossClown::FSM_CLOWN_IDLE:
			m_tFrame.Set_Value(8, 0, 0.25f);
			break;
		case Client::CBossClown::FSM_CLOWN_DANCE:
			m_tFrame.Set_Value(7, 0, 0.25f);
			break;
		case Client::CBossClown::FSM_CLOWN_DODGE:
			m_tFrame.Set_Value(7, 0 , 0.25f);
			break;
		case Client::CBossClown::FSM_CLOWN_ATTACK:
			m_tFrame.Set_Value(7, 0 , 0.25f);
			break;
		case Client::CBossClown::FSM_CLOWN_DEAD:
			m_tFrame.Set_Value(5, 0, 0.25f);
			break;
		case Client::CBossClown::FSM_CLOWN_END:
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CBossClown::Tick_Frame(_float fTimeDelta, CGameInstance* pGameInstance)
{
	m_tFrame.fTime += fTimeDelta;
	if (m_tFrame.fDelay <= m_tFrame.fTime)
	{
		m_tFrame.fTime = 0.f;

		++m_tFrame.iStart;

		if (m_ePreState == FSM_CLOWN_ATTACK && m_tFrame.iStart == 7)
		{
			_uint iRandom = rand() % 2;
			if (iRandom == 0)
				Draw(pGameInstance);
			else
				Falling(pGameInstance);
		}

		if (m_tFrame.iStart >= m_tFrame.iEnd)
		{
			m_tFrame.iStart = 0;

			switch (m_ePreState)
			{
			case Client::CBossClown::FSM_CLOWN_IDLE:
				break;
			case Client::CBossClown::FSM_CLOWN_DANCE:
				Set_State(FSM_CLOWN_IDLE);
				m_tCardDeck.bShuffle = true;
				break;
			case Client::CBossClown::FSM_CLOWN_DODGE:
				Set_State(FSM_CLOWN_DANCE);
				break;
			case Client::CBossClown::FSM_CLOWN_ATTACK:
				Set_State(FSM_CLOWN_DANCE);
				break;
			case Client::CBossClown::FSM_CLOWN_DEAD:
				m_tFrame.iStart = 4;
				break;
			case Client::CBossClown::FSM_CLOWN_END:
				break;
			default:
				break;
			}
		}
	}
}

_vec3 CBossClown::Get_MinPos()
{
	return m_pBoxCollider->Get_MinPos();
}

_vec3 CBossClown::Get_MaxPos()
{
	return m_pBoxCollider->Get_MaxPos();
}

_vec3 CBossClown::Get_WorldPos()
{
	return m_pTransform->Get_State(CTransform::STATE_POS);
}

_vec3 CBossClown::Get_Length()
{
	return m_pTransform->Get_Scale();
}

CGameObject * CBossClown::Clone(void * pArg)
{
	CBossClown* pInstance = new CBossClown(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("CBossClown::Clone Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CBossClown * CBossClown::Create(_pDevice pGraphicDevice)
{
	CBossClown* pInstance = new CBossClown(pGraphicDevice);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("CBossClown::Create Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBossClown::Free()
{
	Safe_Release(m_pCardBoard);
	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pTransform);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pFrustum);
	for (auto& pTexture : m_pTexture)
		Safe_Release(pTexture);

	__super::Free();

}

void CBossClown::Collision(CGameObject * pColObj)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CAttack* pAttack = dynamic_cast<CAttack*>(pColObj);
	if (pAttack != nullptr)
	{
		_uint iDmg = pAttack->Get_Dmg();

		CTutorialMgr::Get_Instance()->Set_TutorialToolTip(CTutorialMgr::TUTORIAL_CLWON_DODGE);


		Sub_Hp(iDmg);

		IsThis_An_Attack_QuestionMark(pAttack->Get_WorldPos());

		pAttack->Set_Dead();

		CDmgFont::DESC tDesc;
		tDesc.iDmg = iDmg;
		tDesc.vPos = m_pTransform->Get_State(CTransform::STATE_POS) + m_pTransform->Get_State(CTransform::STATE_UP);
		tDesc.vScale = m_pTransform->Get_Scale();
		tDesc.bGuard = true;

		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeDmgFont, pTagLayerDmgFont, &tDesc)))
			MSG_VOID("pGameInstance->Add_GameObjectToLayer - pTagPrototypeDmgFont");

	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CBossClown::Tick_Collider(_float fTimeDelta)
{
	m_pBoxCollider->Set_WorldMatrixFromObject(*m_pTransform->Get_WorldMatrixPtr());
	m_pBoxCollider->Tick(fTimeDelta);

	return S_OK;
}

void CBossClown::Set_PoisonHitTransform(CPoisonCubeHit::DESC * pDesc)
{
	memcpy(&pDesc->tObjInfo.matWorld, m_pTransform->Get_WorldMatrixPtr(), sizeof(_mat));
}

const _vec3 & CBossClown::Get_BoxColliderCenter() const
{
	return m_pBoxCollider->Get_ColliderCenter();
}
