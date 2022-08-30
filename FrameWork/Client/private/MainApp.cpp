#include "stdafx.h"
#include "..\public\MainApp.h"
#include "GameInstance.h"
#include "Level_Logo.h"
#include "BlackScreen.h"
#include "Level_Loading.h"
#include "Graphic_Device.h"
#include "Component.h"
#include "Transform.h"
#include "PlayerTagMgr.h"
#include "LoadingDatFile.h"
#include "VIBufferRectColor.h"
#include "DmgFont.h"
#include "DestroyableCube.h"
#include "Attack.h"
#include "EnvironmentMgr.h"
#include "CrosshairMgr.h"
#include "BossSkeletonStageCube.h"
#include "BossSkeletonStageCubeObserver.h"
#include "SkyBox.h"
#include "CameraTargetBack.h"
#include "Camera2D.h"
#include "HpBar.h"
#include "MpBar.h"
#include "PlayerInfo.h"
#include "SkillIcon.h"
#include "TipMgr.h"
#include "LoadingDatFile.h"
#include "CubeObject.h"
#include "MonsterNormalMelee.h"
#include "MonsterNormalRange.h"
#include "WaterCannon.h"
#include "MagicMissile.h"
#include "WallLook.h"
#include "WallSide.h"
#include "BossSkeleton.h"
#include "BossSkeletonMagicCircle.h"
#include "BossSkeletonMagicCircleIn.h"
#include "MeleeSkull.h"
#include "RangeSkull.h"
#include "Rain.h"
#include "RainDrop.h"
#include "ClayCube.h"
#include "TutorialMgr.h"
#include "BossSkeletonStageCube.h"
#include "Crosshair.h"
#include "BossSkeletonStageCubeObserver.h"
#include "Explosion.h"
#include "Terrain.h"
#include "CameraFree.h"
#include <time.h>
#include "BossSkeletonStageCubeAttack.h"
#include "BossSkeletonStageCircle.h"
#include "BossSkeletonThorn.h"
#include "RedBlock.h"
#include "Necromencer.h"
#include "Shield.h"
#include "Resurrection.h"
#include "ShieldCube.h"
#include "NecromencerFire.h"
#include "NecromencerIce.h"
#include "IceOrb.h"
#include "BlueBlock.h"
#include "GrayBlock.h"
#include "Cloud.h"
#include "NecromencerElectric.h"
#include "BlackHole.h"
#include "ResurrectionManager.h"
#include "BossSkeletonStageCircleAttack.h"
#include "TrumpCardClover.h"
#include "TrumpCardDiamond.h"
#include "TrumpCardHeart.h"
#include "TrumpCardSpade.h"
#include "Blind.h"
#include "BossClown.h"
#include "Chimera.h"
#include "TagGauge.h"
#include "TagGaugeObserver.h"
#include "NonControl.h"
#include "NonControlObserver.h"
#include "BrokenBlock.h"
#include "TombStone.h"
#include "TombStoneManager.h"
#include "Portal.h"
#include "VIBufferCubeColor.h"
#include "Dagger.h"
#include "Meteor.h"
#include "LockOn.h"
#include "MonsterColliderManager.h"
#include "PoisonCube.h"
#include "PoisonCubeParticle.h"
#include "PoisonCubeAttack.h"
#include "PoisonCubeHit.h"
#include "Blink.h"
#include "TelekinesisManager.h"
#include "DarkOrb.h"
#include "PlayerShield.h"
#include "BloodScreen.h"
#include "MagicMissileParticle.h"
#include "RunWet.h"
#include "RunWetParticle.h"
#include "TempleCube.h"
#include "TempleCubeDestroyable.h"
#include "FireCracker.h"
#include "Confetti.h"
#include "DialogMgr.h"
#include "Dialog.h"
#include "TriggerCollider.h"
#include "CrossBlock.h"
#include "BaseBlock.h"
#include "Arrow.h"
#include "CardLost.h"
#include "TombStoneParticles.h"


CMainApp::CMainApp()
	:m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::NativeConstruct()
{
	if (m_pGameInstance == nullptr)
		MSG_FAIL("CMainApp NativeConstruct - m_pGameInstance is nullptr");

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInstance, g_hWnd, LEVEL_END)))
		MSG_FAIL("CMainApp NativeConstruct - Initialize_Engine Failed");

	if (FAILED(m_pGameInstance->Initialize_Camera(CAMERA_END)))
		MSG_FAIL("CMainApp NativeConstruct - Initialize_Camera");
	
	if (FAILED(m_pGameInstance->Ready_Graphic_Device(g_hWnd, g_iWinCX, g_iWinCY, &m_pGraphicDevice)))
		MSG_FAIL("CMainApp NativeConstruct - Ready_Graphic_Device Failed");

	CSoundMgr::Get_Instance()->Initialize();
	
	if (FAILED(m_pGameInstance->Ready_CamMatrix()))
		MSG_FAIL("CMainApp NativeConstruct - Ready_CamMatrix Failed");

	if (FAILED(CLoadingDatFile::Get_Instance()->Ready_LoadingDatFile()))
		MSG_FAIL("CMainApp NativeConstruct - Ready_LoadingDatFile Failed");

	if (FAILED(SetUp_SamplerState()))
		MSG_FAIL("CMainApp NativeConstruct - SetUp_SamplerState");

	if (FAILED(Ready_Prototype_Component_ForStatic()))
		MSG_FAIL("CMainApp NativeConstruct - Ready_Prototype_Component_ForStatic");

	if (FAILED(Ready_Prototype_GameObject_ForStatic()))
		MSG_FAIL("CMainApp NativeConstruct - Ready_Prototype_GameObject_ForStatic");

	if (FAILED(Open_Level(LEVEL_LOGO)))
		MSG_FAIL("CMainApp NativeConstruct - Open_Level Failed");

	m_pGraphicDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	CTutorialMgr::Get_Instance()->SetUp_Mgr();
	CTipMgr::Get_Instance()->SetUp_Mgr();

	srand(unsigned(time(NULL)));

	return S_OK;
}

_uint CMainApp::Tick(_float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	m_pGameInstance->Tick_Engine(fTimeDelta);

#ifdef _DEBUG
	m_fTimerAcc += fTimeDelta;
#endif

	return _uint();
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance)
		MSG_FAIL("CMainApp Render - m_pGameInstance is nullptr");

	if (nullptr == m_pRenderer)
		MSG_FAIL("CMainApp Render - m_pRenderer is nullptr");

	m_pGameInstance->Render_Begin();

	m_pRenderer->Draw_RenderGroup();
	m_pGameInstance->Render();

	m_pGameInstance->Render_End(g_hWnd);

#ifdef _DEBUG
	++m_iNumDraw;

	if (m_fTimerAcc >= 1.f)
	{
		wsprintf(m_szFPS, TEXT("FPS : %d"), m_iNumDraw);
		m_iNumDraw = 0;
		m_fTimerAcc = 0.f;
	}

	SetWindowText(g_hWnd, m_szFPS);
#endif

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevel)
{
	CLevel* pLevel = nullptr;

	switch (eLevel)
	{
	case LEVEL_LOGO:
		pLevel = CLevel_Logo::Create(m_pGraphicDevice, LEVEL_LOGO);
		break;
	default:
		pLevel = CLevel_Loading::Create(m_pGraphicDevice, eLevel, LEVEL_LOADING);
		break;
	}

	if (FAILED(m_pGameInstance->Change_Level(pLevel)))
	{
		MSGBOX("CMainApp Open_Level - Change_Level Failed");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component_ForStatic()
{
	//컴포넌트
	if (nullptr == m_pGameInstance)
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - m_pGameInstance is nullptr");

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeRenderer"), m_pRenderer = CRenderer::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeRenderer");

	if(FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeFrustum") , m_pFrustum = CFrustum::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeFrustum");

	//버퍼
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeVIBufferRect"), CVIBufferRect::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeVIBufferRect");

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeVIBufferRectXY"), CVIBufferRectXY::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeVIBufferRectXY");

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeVIBufferRectYZ"), CVIBufferRectYZ::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeVIBufferRectYZ");

	if(FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC,TEXT("PrototypeVIBufferXZ"), CVIBufferTerrain::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeVIBufferXZ");

	//VIBuffer Cube
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, pTagPrototypeVIBufferCube, CVIBufferCube::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeVIBufferCube");
	
	//VIBuffer Thorn
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, pTagPrototypeVIBufferThorn, CVIBufferThorn::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeVIBufferThorn");

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTransform"), CTransform::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeTransform");

	//텍스쳐
	if (FAILED(Ready_Texture(m_pGameInstance)))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Ready_Texture");

	// 서브젝트 컴포넌트
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeSubject"), CSubject::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeSubject");

	Ready_PlayerBuffer();

	//컬러 렉트 버퍼 만들건데 값 세팅해줘야함
	CVIBufferRectColor::VTX_INFO tVtxInfo;
	tVtxInfo.vPos = _vec3(0.f, 0.f, 0.f);
	tVtxInfo.vSize = _vec3(1.f, 1.f, 1.f);
	tVtxInfo.dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	//레드 버퍼
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeVIBufferRectColorRed"), CVIBufferRectColor::Create(m_pGraphicDevice, &tVtxInfo))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeVIBufferRectRed");

	//그린 버퍼
	tVtxInfo.dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeVIBufferRectColorGreen"), CVIBufferRectColor::Create(m_pGraphicDevice, &tVtxInfo))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeVIBufferRectGreen");

	//블루 버퍼
	tVtxInfo.dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeVIBufferRectColorBlue"), CVIBufferRectColor::Create(m_pGraphicDevice, &tVtxInfo))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeVIBufferRectBlue");

	//색 지정 없음
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeVIBufferRectColor"), CVIBufferRectColor::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeVIBufferRect");

	//큐브 컬러
	CVIBufferCubeColor::DESC tDesc;
	tDesc.dwColor = D3DCOLOR_ARGB(255, 102, 0, 153);
	tDesc.iRed = 102; tDesc.iGreen = 0; tDesc.iBlue = 153;
	if(FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeVIBufferCubeColorA"), CVIBufferCubeColor::Create(m_pGraphicDevice,tDesc))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeVIBufferCubeColor");

	tDesc.dwColor = D3DCOLOR_ARGB(255, 220, 20, 60);
	tDesc.iRed = 220; tDesc.iGreen = 20; tDesc.iBlue = 60;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeVIBufferCubeColorB"), CVIBufferCubeColor::Create(m_pGraphicDevice, tDesc))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeVIBufferCubeColor");

	tDesc.dwColor = D3DCOLOR_ARGB(255, 15, 82, 186);
	tDesc.iRed = 15; tDesc.iGreen = 82; tDesc.iBlue = 186;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeVIBufferCubeColorC"), CVIBufferCubeColor::Create(m_pGraphicDevice, tDesc))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeVIBufferCubeColor");

	Safe_AddRef(m_pRenderer);	
	Safe_AddRef(m_pFrustum);

	return S_OK;
}

HRESULT CMainApp::Ready_Texture(CGameInstance * pGameInstance)
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureBackGround"), CTexture::Create(m_pGraphicDevice
		, TEXT("../Bin/Resource/Texture/Default/Default.png")))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeTextureBackGround");

	//DmgFont 텍스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, pTagPrototypeTextureDmgFont, CTexture::Create(m_pGraphicDevice
		, L"../Bin/Resource/Texture/Effect/DmgFont/DmgFont%d.png", 11)))) 
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype pTagPrototypeTextureDmgFont");

	//Dagger 텍스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, pTagPrototypeTextureDagger, CTexture::Create(m_pGraphicDevice
		, L"../Bin/Resource/Texture/RectObject/Dagger/Dagger%d.png", 1))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype pTagPrototypeTextureDagger");

	//BrownCube 텍스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, pTagPrototypeTextureBrownCube, CTexture::Create(m_pGraphicDevice
		, L"../Bin/Resource/Texture/CubeObj/BrownCube/BrownCube%d.dds", 1, CTexture::TYPE_CUBE))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype pTagPrototypeTextureBrownCube");

	//BossSkeletonStageCube 텍스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, pTagPrototypeTextureBossSkeletonStageCube, CTexture::Create(m_pGraphicDevice
		, L"../Bin/Resource/Texture/CubeObj/BossSkeletonStageCube/BossSkeletonStageCube%d.dds", 5, CTexture::TYPE_CUBE))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype ");

	//Wall 텍스쳐
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureWall"), CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/Wall/Wall%d.png"), 8))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureWall");

	// UI 텍스쳐
	// PlayerInfo
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTexturePlayerInfo"), CTexture::Create(m_pGraphicDevice
		, TEXT("../Bin/Resource/Texture/Player/PlayerInfo/PlayerInfo%d.png"), 5))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeTexturePlayerInfo");

	// HpBar
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureHpBar"), CTexture::Create(m_pGraphicDevice
		, TEXT("../Bin/Resource/Texture/Player/HpBar/HpBar%d.png"), 1))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeTextureHpBar");

	// MpBar
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMpBar"), CTexture::Create(m_pGraphicDevice
		, TEXT("../Bin/Resource/Texture/Player/MpBar/MpBar%d.png"), 1))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeTextureMpBar");

	//WarriorSkill
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureWarriorSkill"), CTexture::Create(m_pGraphicDevice
		, TEXT("../Bin/Resource/Texture/Player/Warrior_Skill/Skill_%d.png"), 6))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeTextureWarriorSkill");

	//MageSkill
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMageSkill"), CTexture::Create(m_pGraphicDevice
		, TEXT("../Bin/Resource/Texture/Player/Mage_Skill/Skill_%d.png"), 6))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PrototypeTextureMageSkill");

	//ThiefSkill
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureThiefSkill"), CTexture::Create(m_pGraphicDevice
		, TEXT("../Bin/Resource/Texture/Player/Thief_Skill/Skill_%d.png"), 6))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureThiefSkill");

	//Black
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, pTagPrototypeTextureBlack, CTexture::Create(m_pGraphicDevice
		, TEXT("../Bin/Resource/Texture/Loading/Black/Black%d.png"), 1))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureBlack");

	//Loading
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, pTagPrototypeTextureLoading, CTexture::Create(m_pGraphicDevice
		, TEXT("../Bin/Resource/Texture/Loading/Loading%d.png"), 16))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureLoading");

	// pTagPrototypeTextureBlindEffect
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, pTagPrototypeTextureBlindEffect, CTexture::Create(m_pGraphicDevice
		, TEXT("../Bin/Resource/Texture/Effect/Blind/Blind%d.png"), 2))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeTextureBlindEffect");

	// pTagPrototypeTextureMeteor
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, pTagPrototypeTextureMeteor, CTexture::Create(m_pGraphicDevice
		, TEXT("../Bin/Resource/Texture/CubeObj/Meteor/TNTMeteor%d.dds"), 2, CTexture::TYPE_CUBE))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeTextureMeteor");

	// pTagPrototypeTextureLockOn
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, pTagPrototypeTextureLockOn, CTexture::Create(m_pGraphicDevice
		, TEXT("../Bin/Resource/Texture/RectObject/LockOn/LockOn.png"), 1))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeTextureLockOn");

	// PrototypeTextureExplosion
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, L"PrototypeTextureExplosion", CTexture::Create(m_pGraphicDevice
		, TEXT("../Bin/Resource/Texture/Effect/Explosion/Explosion%d.png"), 90))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureExplosion");

	//Player
	if (FAILED(Ready_TextureGamePlayPlayer(m_pGameInstance)))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype Ready_TextureGamePlayPlayer");

	//Shield
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, pTagPrototypeTexturePlayerShield,
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/CubeObj/Shield/Shield_%d.dds"),
			1, CTexture::TYPE_CUBE))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeTexturePlayerShield");

	//BloodScreen
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, pTagPrototypeTextureBloodScreen,
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/OrthoObj/BloodScreen/BloodScreen%d.png"),
			11))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeTexturePlayerBloodScreen");

	//BossSkeletonStageCubeAttack
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, pTagPrototypeTextureBossSkeletonStageCubeAttack,
		CTexture::Create(m_pGraphicDevice,
			L"../bin/Resource/Texture/CubeObj/BossSkeletonStageCubeAttack/BossSkeletonStageCubeAttack%d.dds", 6, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForBossSkeleton - Add_Prototype BossSkeletonStageCubeAttack");

	//BlackAlpha
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC
		, pTagPrototypeTextureBlackAlpha,
		CTexture::Create(m_pGraphicDevice,
			L"../bin/Resource/Texture/RectObject/BlackAlpha/BlackAlpha%d.png", 11))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype BlackAlpha");

	//TipObject
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC
		, pTagPrototypeTextureTipObject,
		CTexture::Create(m_pGraphicDevice,
			L"../bin/Resource/Texture/TipObject/TipObject%d.png", 4))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype TipObject");

	//TipObject
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC
		, pTagPrototypeTextureCircusTip,
		CTexture::Create(m_pGraphicDevice,
			L"../bin/Resource/Texture/TipCircus/TipObject%d.png", 4))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeTextureCircusTip");

	return S_OK;

	//텍스쳐 끝
}

HRESULT CMainApp::Ready_TextureGamePlayPlayer(CGameInstance * pGameInstance)
{
	//Warrior
	//RUN
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureWarriorRunE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Warrior/RunE/RunE%d.png"), 6))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureWarriorRunE");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureWarriorRunN"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Warrior/RunN/RunN%d.png"), 6))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureWarriorRunN");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureWarriorRunS"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Warrior/RunS/RunS%d.png"), 6))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureWarriorRunS");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureWarriorRunNE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Warrior/RunNE/RunNE%d.png"), 6))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureWarriorRunNE");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureWarriorRunSE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Warrior/RunSE/RunSE%d.png"), 6))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureWarriorRunSE");

	//SKILL
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureWarriorSkillN"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Warrior/SkillN/SkillN%d.png"), 2))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureWarriorSkillN");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureWarriorSkillNE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Warrior/SkillNE/SkillNE%d.png"), 2))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureWarriorSkillNE");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureWarriorSkillE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Warrior/SkillE/SkillE%d.png"), 2))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureWarriorSkillE");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureWarriorSkillSE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Warrior/SkillSE/SkillSE%d.png"), 2))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureWarriorSkillSE");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureWarriorSkillS"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Warrior/SkillS/SkillS%d.png"), 2))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureWarriorSkillS");

	//IDLE
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureWarriorIdleE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Warrior/IdleE/IdleE%d.png"), 1))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureWarriorIdleE");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureWarriorIdleS"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Warrior/IdleS/IdleS%d.png"), 1))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureWarriorIdleS");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureWarriorIdleN"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Warrior/IdleN/IdleN%d.png"), 1))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureWarriorIdleN");

	//Mage
	//RUN
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMageRunE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Mage/RunE/RunE%d.png"), 7))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureMageRunE");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMageRunN"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Mage/RunN/RunN%d.png"), 7))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureMageRunN");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMageRunS"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Mage/RunS/RunS%d.png"), 7))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureMageRunS");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMageRunNE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Mage/RunNE/RunNE%d.png"), 7))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureMageRunNE");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMageRunSE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Mage/RunSE/RunSE%d.png"), 7))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureMageRunSE");

	//SKILL
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMageSkillN"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Mage/SkillN/SkillN%d.png"), 6))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureMageSkillN");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMageSkillNE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Mage/SkillNE/SkillNE%d.png"), 6))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureMageSkillNE");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMageSkillE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Mage/SkillE/SkillE%d.png"), 6))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureMageSkillE");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMageSkillSE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Mage/SkillSE/SkillSE%d.png"), 6))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureMageSkillSE");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMageSkillS"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Mage/SkillS/SkillS%d.png"), 6))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureMageSkillS");

	//IDLE
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMageIdleE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Mage/IdleE/IdleE%d.png"), 4))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureMageIdleE");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMageIdleS"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Mage/IdleS/IdleS%d.png"), 4))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureMageIdleS");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMageIdleN"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Mage/IdleN/IdleN%d.png"), 4))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureMageIdleN");

	//Thief
	//RUN
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureThiefRunE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Thief/RunE/RunE%d.png"), 6))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureThiefRunE");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureThiefRunN"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Thief/RunN/RunN%d.png"), 6))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureThiefRunN");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureThiefRunS"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Thief/RunS/RunS%d.png"), 6))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureThiefRunS");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureThiefRunNE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Thief/RunNE/RunNE%d.png"), 6))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureThiefRunNE");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureThiefRunSE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Thief/RunSE/RunSE%d.png"), 6))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureThiefRunSE");

	//SKILL
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureThiefSkillN"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Thief/SkillN/SkillN%d.png"), 2))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureThiefSkillN");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureThiefSkillNE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Thief/SkillNE/SkillNE%d.png"), 2))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureThiefSkillNE");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureThiefSkillE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Thief/SkillE/SkillE%d.png"), 2))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureThiefSkillE");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureThiefSkillSE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Thief/SkillSE/SkillSE%d.png"), 2))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureThiefSkillSE");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureThiefSkillS"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Thief/SkillS/SkillS%d.png"), 2))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureThiefSkillS");

	//IDLE
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureThiefIdleE"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Thief/IdleE/IdleE%d.png"), 1))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureThiefIdleE");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureThiefIdleS"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Thief/IdleS/IdleS%d.png"), 1))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureThiefIdleS");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureThiefIdleN"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Thief/IdleN/IdleN%d.png"), 1))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureThiefIdleN");

	//THROW
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureThiefThrow"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Player/Thief/Throw/Throw%d.png"), 9))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTextureThiefThrow");

	// pTagPrototypeTexturePoisonCube
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, pTagPrototypeTexturePoisonCube, CTexture::Create(m_pGraphicDevice
		, TEXT("../Bin/Resource/Texture/CubeObj/PoisonCube/PoisonCube%d.dds"), 2, CTexture::TYPE_CUBE))))
		MSG_FAIL("MainApp Ready_Texture - Add_Prototype pTagPrototypeTexturePoisonCube");

	return S_OK; 
}

HRESULT CMainApp::Ready_Prototype_GameObject_ForStatic()
{
	

	//객체
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeDmgFont, CDmgFont::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype(pTagPrototypeDmgFont");

	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeDestroyableCube, CDestroyableCube::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype(pTagPrototypeDestroyableCube");

	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeAttack, CAttack::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype(pTagPrototypeAttack");

	/*PrototypeBossSkeletonStageCube*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeBossSkeletonStageCube, CBossSkeletonStageCube::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeBossSkeletonStageCube");

	/*PrototypeBossSkeletonStageCubeObserver*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeBossSkeletonStageCubeObserver, CBossSkeletonStageCubeObserver::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeBossSkeletonStageCubeObserver");

	/*PrototypeSkyBox*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypeSkyBox"), CSkyBox::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeSkyBox");

	/*PrototypeTerrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypeTerrain"), CTerrain::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTerrain");

	/*PrototypeCameraFree*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypeCameraFree"), CCameraFree::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeCameraFree");

	/*PrototypePlayer*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypePlayer"), CPlayer::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypePlayer");

	/*PrototypeWallLook*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeWallLook, CWallLook::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeWallLook");

	/*PrototypeWallSide*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeWallSide, CWallSide::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeWallSide");

	/*PrototypeExplosion*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypeExplosion"), CExplosion::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeSkyBox");

	/*PrototypeCameraTargetBack*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypeCameraTargetBack"), CCameraTargetBack::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeCameraTargetBack");

	/*PrototypeCamera2D*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypeCamera2D"), CCamera2D::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeCamera2D");

	/*PrototypePlayerInfo*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypePlayerInfo"), CPlayerInfo::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypePlayerInfo");

	/*PrototypeHpBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypeHpBar"), CHpBar::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeHpBar");

	/*PrototypeMpBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypeMpBar"), CMpBar::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeMpBar");

	/*PrototypeSkillIcon*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypeSkillIcon"), CSkillIcon::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeSkillIcon");

	/*PrototypeMonsterNormalMelee*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeMonsterNormalMelee", CMonsterNormalMelee::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeMonsterNormalMelee");

	/*PrototypeMonsterNormalRange*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeMonsterNormalRange", CMonsterNormalRange::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeMonsterNormalRange");

	/*PrototypeMagicMissile*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeMagicMissile", CMagicMissile::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeMagicMissile");

	/*PrototypeClay*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeClay", CClayCube::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeClay");

	/*PrototypeBossSkeletonMagicCircle*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeBossSkeletonMagicCircle", CBossSkeletonMagicCircle::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeBossSkeletonMagicCircle");

	/*PrototypeBossSkeletonMagicCircleIn*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeBossSkeletonMagicCircleIn", CBossSkeletonMagicCircleIn::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeBossSkeletonMagicCircleIn");

	/*PrototypeBossSkeleton*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeBossSkeleton", CBossSkeleton::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeBossSkeleton");

	/*PrototypeMeleeSkull*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeMeleeSkull", CMeleeSkull::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeMeleeSkull");

	/*PrototypeRedBlock*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeCircusCubeLegoRed, CRedBlock::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeRedBlock");

	/*PrototypeBlueBlock*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeCircusCubeLegoBlue, CBlueBlock::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeBlueBlock");

	/*PrototypeGrayBlock*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeCircusCubeLegoGray, CGrayBlock::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeGrayBlock");

	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeCircusCubeBroken, CBrokenBlock::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeGrayBlock");

	/*PrototypeRain*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeRain", CRain::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeRain");

	/*PrototypeRainDrop*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeRainDrop", CRainDrop::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeRainDrop");

	/*PrototypeRangeSkull*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeRangeSkull", CRangeSkull::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeRangeSkull");

	/*PrototypeCrosshair*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeCrosshair, CCrosshair::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeCrosshair");

	/*PrototypeBossSkeletonStageCube*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeBossSkeletonStageCubeAttack, CBossSkeletonStageCubeAttack::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeBossSkeletonStageCubeAttack");
	
	/*PrototypeBossSkeletonStageCircle*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeBossSkeletonStageCircle, CBossSkeletonStageCircle::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeBossSkeletonStageCubeCircle");

	/*PrototypeBossSkeletonThorn*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeBossSkeletonThorn, CBossSkeletonThorn::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeBossSkeletonThorn");
	
	/*PrototypeShield*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeShield", CShield::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeShield");

	/*PrototypeNecromencer*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeNecromencer", CNecromencer::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeNecromencer");

	/*PrototypeResurrection*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeResurrection", CResurrection::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeResurrection");

	/*PrototypeShieldCube*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeShieldCube", CShieldCube::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeShieldCube");

	/*PrototypeNecromencerFire*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeNecromencerFire", CNecromencerFire::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeNecromencerFire");

	/*PrototypeNecromencerIce*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeNecromencerIce", CNecromencerIce::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeNecromencerIce");

	/*PrototypeIceOrb*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeIceOrb", CIceOrb::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeIceOrb");

	/*PrototypeCloud*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeCloud", CCloud::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeCloud");

	/*PrototypeElectric*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeElectric", CNecromencerElectric::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeElectric");

	/*PrototypeBlackHole*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeBlackHole", CBlackHole::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeBlackHole");
	
	/*PrototypeBossSkeletonStageCircleAttack*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeBossSkeletonStageCircleAttack, CBossSkeletonStageCircleAttack::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeBossSkeletonStageCircleAttack");

	// PrototypeCircusCard
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeCircusCardClover, CTrumpCardClover::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeCircusCardClover");

	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeCircusCardDiamond, CTrumpCardDiamond::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeCircusCardDiamond");

	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeCircusCardHeart, CTrumpCardHeart::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeCircusCardHeart");

	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeCircusCardSpade, CTrumpCardSpade::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeCircusCardSpade");

	// pTagPrototypeEffectBlind
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeEffectBlind, CBlind::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeEffectBlind");

	// pTagPrototypeCircusBossClown
	if(FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeCircusBossClown, CBossClown::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeCircusBossClown");

	/*PrototypeChimera*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeChimera", CChimera::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeChimera");
	
	/*PrototypeTagGauge*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeTagGauge, CTagGauge::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTagGauge");

	/*PrototypeObserverTagGauge*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeObserverTagGauge, CTagGaugeObserver::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeObserverTagGauge");

	/*PrototypeNonControl*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeNonControl, CNonControl::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeNonControl");

	/*PrototypeObserverNonControl*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeObserverNonControl, CNonControlObserver::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeObserverNonControl");

	/*PrototypeTombStone*/
	if (FAILED(m_pGameInstance->Add_Prototype(L"PrototypeTombStone", CTombStone::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTombStone");

	//PrototypePortal
	if(FAILED(m_pGameInstance->Add_Prototype(pTagPrototypePortal,CPortal::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypePortal");

	/*PrototypeDagger*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeDagger, CDagger::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeDagger");

	/*PrototypePoisonCube*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypePoisonCube, CPoisonCube::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypePoisonCube");

	/*PrototypePoisonCubeParticle*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypePoisonCubeParticle, CPoisonCubeParticle::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypePoisonCubeParticle");

	//pTagPrototypeMeteor
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeMeteor, CMeteor::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype pTagPrototypeMeteor");

	//pTagPrototypeLockOn
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeLockOn, CLockOn::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype pTagPrototypeLockOn");

	/*PrototypePoisonCubeAttack*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypePoisonCubeAttack, CPoisonCubeAttack::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypePoisonCubeAttack");

	/*PrototypePoisonCubeHit*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypePoisonCubeHit, CPoisonCubeHit::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp::Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypePoisonCubeHit");

	/*PrototypeBlink*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypeBlink"), CBlink::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeBlink");

	/*PrototypeDarkOrb*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypeDarkOrb"), CDarkOrb::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeDarkOrb");

	/*PrototypePlayerShield*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypePlayerShield, CPlayerShield::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypePlayerShield");

	/*PrototypeBloodScreen*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeBloodScreen, CBloodScreen::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeBloodScreen");

	/*PrototypeMagicMissileParticle*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeMagicMissileParticle, CMagicMissileParticle::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeMagicMissileParticle");

	/*PrototypeRunWet*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeRunWet, CRunWet::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeRunWet");

	/*PrototypeRunWetParticle*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeRunWetParticle, CRunWetParticle::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeRunWetParticle");

	/*pTagPrototypeCaveBloc*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeCaveBloc, CTempleCube::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeCaveBloc");

	/*pTagPrototypeCaveDoorDestroyable*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeCaveDoorDestroyable, CTempleCubeDestroyable::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeCaveDoorDestroyable");

	/*pTagPrototypeFireCracker*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeFireCracker, CFireCracker::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeFireCracker");

	/*pTagPrototypeConfetti*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeConfetti, CConfetti::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype pTagPrototypeConfetti");

	/*PrototypeDialog*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeDialog, CDialog::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeDialog");

	/*PrototypeTriggerCollider*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeTriggerCollider, CTriggerCollider::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTriggerCollider");

	/*PrototypeWaterCannon*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeWaterCannon, CWaterCannon::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeWaterCannon");

	/*PrototypeBlackScreen*/
	if (FAILED(m_pGameInstance->Add_Prototype(pTagPrototypeBlackScreen, CBlackScreen::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeBlackScreen");

	/*PrototypeCrossBlock*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypeCrossBlock"), CCrossBlock::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeCrossBlock");

	/*PrototypeBaseBlock*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypeBaseBlock"), CBaseBlock::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeBaseBlock");

	/*PrototypeArrow*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypeArrow"), CArrow::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeArrow");
	// PrototypeCardLost
	if(FAILED(m_pGameInstance->Add_Prototype(L"PrototypeCardLost", CCardLost::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeCardLost");

	/*PrototypeTombStoneParticles*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("PrototypeTombStoneParticles"), CTombStoneParticles::Create(m_pGraphicDevice))))
		MSG_FAIL("CMainApp Ready_Prototype_GameObject_ForStatic - Add_Prototype PrototypeTombStoneParticles");

	return S_OK;
	//프로토타입
}

HRESULT CMainApp::SetUp_SamplerState()
{
	if (m_pGraphicDevice == nullptr)
		MSG_FAIL("CMainApp SetUp_SamplerState - m_pGraphicDevice is null");

	m_pGraphicDevice->SetSamplerState(0, D3DSAMPLERSTATETYPE::D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphicDevice->SetSamplerState(0, D3DSAMPLERSTATETYPE::D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pGraphicDevice->SetSamplerState(0, D3DSAMPLERSTATETYPE::D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphicDevice->SetSamplerState(0, D3DSAMPLERSTATETYPE::D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	return S_OK;
}

HRESULT CMainApp::Ready_PlayerBuffer()
{
	CLoadingDatFile* pLoadingDatFile = GET_INSTANCE(CLoadingDatFile);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	wstring wstrSceneName = TEXT("PlayerBufferScene");

	size_t iIndex = pLoadingDatFile->Get_VectorSize(wstrSceneName);

	OBJ_INFO tObjInfo = *pLoadingDatFile->Get_ObjInfo(wstrSceneName, 0);

	CVIBufferRect::SIZE_POS tSizePos;
	tSizePos.vSize = _vec3(tObjInfo.fSizeX, tObjInfo.fSizeY, tObjInfo.fSizeZ);
	tSizePos.vPos = tObjInfo.vVertexPos;

	//버퍼
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("PlayerBuffer"), CVIBufferRect::Create(m_pGraphicDevice, &tSizePos))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype PlayerBuffer");

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CLoadingDatFile);

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp* pMainApp = new CMainApp;

	if (FAILED(pMainApp->NativeConstruct()))
	{
		MSGBOX("Failed To Creating CMainApp");
		Safe_Release(pMainApp);
	}

	return pMainApp;
}

void Client::CMainApp::Free()
{
	// 멤버정리. 
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pFrustum);
	CSoundMgr::Get_Instance()->Destroy_Instance();
	CPlayerTagMgr::Destroy_Instance();
	CLoadingDatFile::Destroy_Instance();
	CDialogMgr::Destroy_Instance();
	CGameInstance::Release_Engine();
	CEnvironmentMgr::Destroy_Instance();
	CCrosshairMgr::Destroy_Instance();	
	CTombStoneManager::Get_Instance()->Destroy_Instance();
	CMonsterColliderManager::Get_Instance()->Destroy_Instance();
	CResurrectionManager::Get_Instance()->Destroy_Instance();
	CTelekinesisManager::Get_Instance()->Destroy_Instance();
	CTutorialMgr::Destroy_Instance();
	CTipMgr::Destroy_Instance();
}
