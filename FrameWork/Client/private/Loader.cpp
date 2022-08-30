#include "stdafx.h"
#include "..\public\Loader.h"
#include "GameInstance.h"
#include "Terrain.h"
#include "Player.h"
#include "CameraFree.h"
#include "SkyBox.h"
#include "Explosion.h"
#include "CameraTargetBack.h"
#include "Camera2D.h"
#include "HpBar.h"
#include "MpBar.h"
#include "PlayerInfo.h"
#include "SkillIcon.h"
#include "LoadingDatFile.h"
#include "CubeObject.h"
#include "MonsterNormalMelee.h"
#include "MonsterNormalRange.h"
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
#include "BossSkeletonStageCube.h"
#include "Crosshair.h"
#include "BossSkeletonStageCubeObserver.h"
#include "Shield.h"
#include "Necromencer.h"
#include "RedBlock.h"
#include "GrayBlock.h"
#include "Confetti.h"


CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphicDevice)
{
	m_pGraphicDevice = pGraphicDevice;
	Safe_AddRef(m_pGraphicDevice);
}

_uint APIENTRY LoadingMain(void* pArg)
{
	CLoader* pLoader = (CLoader*)(pArg);

	EnterCriticalSection(&pLoader->Get_CS());

	HRESULT hr = 0;

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_GAMEPLAY:
		hr = pLoader->LoadingForGamePlay();
		break;
	case LEVEL_TEST:
		hr = pLoader->LoadingForTest();
		break;
	case LEVEL_BOSS_SKELETON:
		hr = pLoader->LoadingForBossSkeleton();
		break;
	case LEVEL_BOSS_CIRCUS:
		hr = pLoader->LoadingForCircus();
		break;
	case LEVEL_BOSS_NECROMENCER:
		hr = pLoader->LoadingForNecromencer();
		break;
	default:
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&pLoader->Get_CS());

	return S_OK;
}

HRESULT CLoader::NativeConstruct(LEVEL eLevel)
{
	InitializeCriticalSection(&m_CS);

	m_isFinished = false;

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);

	if (m_hThread == 0)
		return E_FAIL;

	m_eNextLevel = eLevel;

	return S_OK;
}

HRESULT CLoader::LoadingForGamePlay()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	CLoadingDatFile* pLoadingDatFile = GET_INSTANCE(CLoadingDatFile);

	wsprintf(m_szLoading, TEXT("정점버퍼를 로딩중입니다. "));

	//Terrain
	if (FAILED(pInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("PrototypeVIBufferTerrain"), CVIBufferTerrain::Create(m_pGraphicDevice, 129, 129, 1, 1, 60, 60))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeVIBufferTerrain");

	wsprintf(m_szLoading, TEXT("텍스쳐를 로딩중입니다. "));

	//Terrain
	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureTerrain"), CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/Terrain/Terrain%d.tga"), 4))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureTerrain");

	//MonsterNormalMelee
	if (FAILED(Ready_TextureGamePlayMonster(pInstance)))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype Ready_TextureGamePlayMonster");

	//RainDrop 텍스쳐
	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, pTagPrototypeTextureRainDrop, CTexture::Create(m_pGraphicDevice
		, L"../Bin/Resource/Texture/CubeObj/RainDrop/RainDrop%d.dds", 1, CTexture::TYPE_CUBE))))
		MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype ");

	//BossSkeletonMagicCircle, In 텍스쳐
	if (FAILED(pInstance->Add_Prototype(LEVEL_GAMEPLAY, L"PrototypeTextureBossSkeletonMagicCircle",
		CTexture::Create(m_pGraphicDevice,
			L"../bin/Resource/Texture/Effect/BossSkeleton/BossSkeletonMagicCircle/BossSkeletonMagicCircle/BossSkeletonMagicCircle%d.png", 13))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureBossSkeletonMagicCircle");


	if (FAILED(pInstance->Add_Prototype(LEVEL_GAMEPLAY, L"PrototypeTextureBossSkeletonMagicCircleIn",
		CTexture::Create(m_pGraphicDevice,
			L"../bin/Resource/Texture/Effect/BossSkeleton/BossSkeletonMagicCircle/BossSkeletonMagicCircleIn/BossSkeletonMagicCircleIn%d.png", 1))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureBossSkeletonMagicCircleIn");

	//SkyBox 텍스쳐 //추후 수정
	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureSkyBox"), CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/SkyBox/SkyBox%d.dds"), 6, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureSkyBox");

	//RedBlock
	if (FAILED(pInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("PrototypeTextureRedBlock"), CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/CubeObj/TileCube/Cube_Red.dds"), 1, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureRedBlock");

	//BlueBlock
	if (FAILED(pInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("PrototypeTextureBlueBlock"), CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/CubeObj/TileCube/Cube_Blue.dds"), 1, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureBlueBlock");

	//GrayBlock
	if (FAILED(pInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("PrototypeTextureGrayBlock"), CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/CubeObj/TileCube/Cube_Gray.dds"), 1, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureGrayBlock");

	//Crosshair 텍스쳐
	if (FAILED(pInstance->Add_Prototype(LEVEL_GAMEPLAY, pTagPrototypeTextureCrosshair, CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/Crosshair/Crosshair%d.png"), 1))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureCrosshair");

	// GamePlay 큐브 텍스쳐
	if (FAILED(pInstance->Add_Prototype(LEVEL_GAMEPLAY, pTagPrototypeTextureTempleCube, CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/CubeObj/TempleCube/TempleCube.dds"), 1, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype pTagPrototypeTextureTempleCube");

	if (FAILED(pInstance->Add_Prototype(LEVEL_GAMEPLAY, pTagPrototypeTextureTempleBrokenCube, CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/CubeObj/TempleDestroyableCube/TempleCube_Destroyable%d.dds"), 10, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype pTagPrototypeTextureTempleCube");

	wsprintf(m_szLoading, TEXT("객체를 생성중입니다. "));
	wsprintf(m_szLoading, TEXT("로딩이 완료되었습니다. "));

	m_isFinished = true;

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CLoadingDatFile);

	return S_OK;
}

HRESULT CLoader::LoadingForTest()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	

	wsprintf(m_szLoading, TEXT("정점버퍼를 로딩중입니다."));
	wstring wstrSceneName = L"Test";
	size_t iIndex = CLoadingDatFile::Get_Instance()->Get_VectorSize(wstrSceneName);
	for (size_t i = 0; i < iIndex; ++i)
	{
		OBJ_INFO tObjInfo = *CLoadingDatFile::Get_Instance()->Get_ObjInfo(wstrSceneName, (_uint)i);

		//이름이 같으면 생성
		if (lstrcmp(tObjInfo.szObjName, L"Terrain") == 0)
		{
			if (lstrcmp(tObjInfo.szHeightMap, L"None") == 0)
			{
				if (FAILED(pInstance->Add_Prototype(LEVEL_TEST, L"PrototypeVIBufferTerrainTest", CVIBufferTerrain::Create(m_pGraphicDevice,
					tObjInfo.iNumVerticesX, tObjInfo.iNumVerticesZ, tObjInfo.fSizeX, tObjInfo.fSizeZ, tObjInfo.fU, tObjInfo.fV))))
					MSG_FAIL("CLoader::LoadingForTest - Add_Prototype PrototypeVIBufferTerrainTest");
			}
			else
			{
				wstring wstrHeightMapPath = L"../bin/Resource/Texture/HeightMap/";
				wstrHeightMapPath += tObjInfo.szHeightMap;
				wstrHeightMapPath += L".bmp";
				if (FAILED(pInstance->Add_Prototype(LEVEL_TEST, L"PrototypeVIBufferTerrainTest", CVIBufferTerrain::Create(m_pGraphicDevice, wstrHeightMapPath.c_str()))))
					MSG_FAIL("CLoader::LoadingForTest - Add_Prototype PrototypeVIBufferTerrainTest");
			}
		}
	}
	

	wsprintf(m_szLoading, TEXT("텍스쳐를 로딩중입니다. "));

	if (FAILED(pInstance->Add_Prototype(LEVEL_TEST, L"PrototypeTextureEx3",
		CTexture::Create(m_pGraphicDevice, L"../bin/Resource/Texture/ListCube/Cube3.dds", 1, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader::LoadingForTest - Add_Prototype PrototypeTextureEx3");

	

	wsprintf(m_szLoading, TEXT("객체를 생성중입니다. "));
	if (FAILED(pInstance->Add_Prototype(L"PrototypeTestTerrain", CTerrain::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader::LoadingForTest - Add_Prototype PrototypeTestTerrain");

	if (FAILED(pInstance->Add_Prototype(L"PrototypeTestEx3", CCubeObject::Create(m_pGraphicDevice))))
		MSG_FAIL("CLoader::LoadingForTest - Add_Prototype PrototypeTestEx3");


	wsprintf(m_szLoading, TEXT("로딩이 완료되었습니다. "));

	m_isFinished = true;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::LoadingForBossSkeleton()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	wsprintf(m_szLoading, TEXT("정점버퍼를 로딩중입니다."));

	wsprintf(m_szLoading, TEXT("텍스쳐를 로딩중입니다. "));

	// 해골 보스
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_SKELETON, TEXT("PrototypeTextureBossSkeletonAttack"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/BossSkeleton/Attack/Attack%d.png"),
			10))))
		MSG_FAIL("CLoader LoadingForBossSkeleton - Add_Prototype PrototypeTextureBossSkeletonAttack");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_SKELETON, TEXT("PrototypeTextureBossSkeletonDie"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/BossSkeleton/Die/Die%d.png"),
			13))))
		MSG_FAIL("CLoader LoadingForBossSkeleton - Add_Prototype PrototypeTextureBossSkeletonDie");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_SKELETON, TEXT("PrototypeTextureBossSkeletonIdle"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/BossSkeleton/Idle/Idle%d.png"),
			6))))
		MSG_FAIL("CLoader LoadingForBossSkeleton - Add_Prototype PrototypeTextureBossSkeletonIdle");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_SKELETON, TEXT("PrototypeTextureBossSkeletonSpawn"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/BossSkeleton/Spawn/Spawn%d.png"),
			15))))
		MSG_FAIL("CLoader LoadingForBossSkeleton - Add_Prototype PrototypeTextureBossSkeletonSpawn");

	//BossSkeletonMagicCircle, In 텍스쳐
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_SKELETON, L"PrototypeTextureBossSkeletonMagicCircle",
		CTexture::Create(m_pGraphicDevice,
			L"../bin/Resource/Texture/Effect/BossSkeleton/BossSkeletonMagicCircle/BossSkeletonMagicCircle/BossSkeletonMagicCircle%d.png", 13))))
		MSG_FAIL("CLoader LoadingForBossSkeleton - Add_Prototype PrototypeTextureBossSkeletonMagicCircle");


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_SKELETON, L"PrototypeTextureBossSkeletonMagicCircleIn",
		CTexture::Create(m_pGraphicDevice,
			L"../bin/Resource/Texture/Effect/BossSkeleton/BossSkeletonMagicCircle/BossSkeletonMagicCircleIn/BossSkeletonMagicCircleIn%d.png", 1))))
		MSG_FAIL("CLoader LoadingForBossSkeleton - Add_Prototype PrototypeTextureBossSkeletonMagicCircleIn");

	//BossSkeletonStageCircle
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_SKELETON, pTagPrototypeTextureBossSkeletonStageCircle,
		CTexture::Create(m_pGraphicDevice,
			L"../bin/Resource/Texture/RectObject/BossSkeletonStageCircle/BossSkeletonStageCircle%d.png", 1))))
		MSG_FAIL("CLoader LoadingForBossSkeleton - Add_Prototype BossSkeletonStageCircle");

	//BossSkeletonThorn
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_BOSS_SKELETON, pTagPrototypeTextureBossSkeletonThorn,
		CTexture::Create(m_pGraphicDevice,
			L"../bin/Resource/Texture/ThornObject/BossSkeletonThorn/BossSkeletonThorn%d.png", 1))))
		MSG_FAIL("CLoader LoadingForBossSkeleton - Add_Prototype BossSkeletonThorn");
	
	wsprintf(m_szLoading, TEXT("객체를 생성중입니다. "));
	wsprintf(m_szLoading, TEXT("로딩이 완료되었습니다. "));
	m_isFinished = true;
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::LoadingForCircus()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	wsprintf(m_szLoading, TEXT("텍스쳐를 로딩중입니다. "));
	//Clay
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_CIRCUS, TEXT("PrototypeTextureClay"), CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/CubeObj/Clay/Cube_Clay.dds"), 1, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureClay");

	//RedBlock
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_CIRCUS, TEXT("PrototypeTextureRedBlock"), CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/CubeObj/TileCube/Cube_Red.dds"), 1, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureRedBlock");

	//RedBlock
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_CIRCUS, TEXT("PrototypeTextureBlueBlock"), CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/CubeObj/TileCube/Cube_Blue.dds"), 1, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureBlueBlock");

	//GrayBlock
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_CIRCUS, TEXT("PrototypeTextureGrayBlock"), CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/CubeObj/TileCube/Cube_Gray.dds"), 1, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureGrayBlock");

	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_CIRCUS, TEXT("PrototypeTextureBrokenBlock"), CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/CubeObj/TileCube/Cube_Broken.dds"), 1, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureBrokenBlock");

	// pTagPrototypeTextureFireCracker
	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, pTagPrototypeTextureFireCracker, CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/CubeObj/FireCracker/FireCracker%d.dds"), 2, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureBrokenBlock");

	// pTagPrototypeTextureConfetti
	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, pTagPrototypeTextureConfetti, CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/CubeObj/Confetti/Confetti%d.dds"), 3, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype pTagPrototypeTextureConfetti");

	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_CIRCUS, pTagPrototypeTextureTrumpClover, CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/Effect/BossClown/Clover/Clover%d.png"), 13))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype pTagPrototypeTextureTrumpClover");

	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_CIRCUS, pTagPrototypeTextureTrumpHeart, CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/Effect/BossClown/Heart/Heart%d.png"), 13))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype pTagPrototypeTextureTrumpHeart");

	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_CIRCUS, pTagPrototypeTextureTrumpDiamond, CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/Effect/BossClown/Diamond/Diamond%d.png"), 13))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype pTagPrototypeTextureTrumpDiamond");

	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_CIRCUS, pTagPrototypeTextureTrumpSpade, CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/Effect/BossClown/Spade/Spade%d.png"), 13))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype pTagPrototypeTextureTrumpSpade");

	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_CIRCUS, L"PrototypeTextureBossClownIdle", CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/Monster/BossClown/Idle/Idle%d.png"), 9))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureBossClownIdle");

	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_CIRCUS, L"PrototypeTextureBossClownAttack", CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/Monster/BossClown/Attack/Attack%d.png"), 8))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureBossClownAttack");

	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_CIRCUS, L"PrototypeTextureBossClownDodge", CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/Monster/BossClown/Dodge/Dodge%d.png"), 8))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureBossClownDodge");

	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_CIRCUS, L"PrototypeTextureBossClownDance", CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/Monster/BossClown/Dance/Dance%d.png"), 8))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureBossClownDance");

	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_CIRCUS, L"PrototypeTextureBossClownDead", CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/Monster/BossClown/Dead/Dead%d.png"), 6))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureBossClownDead");

	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_CIRCUS, L"PrototypeTextureCardBoard", CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/Effect/BossClown/Board/CardBoard.png"), 1))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureCardBoard");

	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_CIRCUS, L"PrototypeTextureCardSelect", CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/Effect/BossClown/Board/BoardSelect.png"), 1))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureCardSelect");

	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_CIRCUS, L"PrototypeTextureCardLost", CTexture::Create(m_pGraphicDevice, TEXT("../Bin/Resource/Texture/Effect/BossClown/Board/CardLost.png"), 1))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureCardLost");

	m_isFinished = true;
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::LoadingForNecromencer()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	wsprintf(m_szLoading, TEXT("정점버퍼를 로딩중입니다."));

	wsprintf(m_szLoading, TEXT("텍스쳐를 로딩중입니다. "));

	// 원진 몹 이미지
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Melee_Skull
	// Attack 1
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureMeleeSkullAttack1"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Melee_Skull/Attack_1/Attack_%d.png"),
			12))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMeleeSkullAttack1");
	// Attack 2
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureMeleeSkullAttack2"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Melee_Skull/Attack_2/Attack_%d.png"),
			12))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMeleeSkullAttack2");
	// Death
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureMeleeSkullDeath"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Melee_Skull/Death/Death_%d.png"),
			14))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMeleeSkullDeath");
	// Die
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureMeleeSkullDie"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Melee_Skull/Die/Die_%d.png"),
			1))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMeleeSkullDie");
	// Hit
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureMeleeSkullHit"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Melee_Skull/Hit/Hit_%d.png"),
			6))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMeleeSkullHit");
	// Idle
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureMeleeSkullIdle"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Melee_Skull/Idle/Idle_%d.png"),
			11))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMeleeSkullIdle");
	// Move
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureMeleeSkullMove"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Melee_Skull/Move/Move_%d.png"),
			13))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMeleeSkullMove");
	// Spawn
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureMeleeSkullSpawn"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Melee_Skull/Spawn/Spawn_%d.png"),
			15))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMeleeSkullSpawn");

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Range_Skull
	// Attack
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureRangeSkullAttack"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Range_Skull/Attack/Attack_%d.png"),
			22))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureRangeSkullAttack");
	// Death
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureRangeSkullDeath"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Range_Skull/Death/Death_%d.png"),
			11))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureRangeSkullDeath");
	// Die
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureRangeSkullDie"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Range_Skull/Die/Die_%d.png"),
			1))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureRangeSkullDie");
	// Hit
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureRangeSkullHit"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Range_Skull/Hit/Hit_%d.png"),
			9))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureRangeSkullHit");
	// Idle
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureRangeSkullIdle"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Range_Skull/Idle/Idle_%d.png"),
			16))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureRangeSkullIdle");
	// Move
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureRangeSkullMove"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Range_Skull/Move/Move_%d.png"),
			11))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureRangeSkullMove");
	// Spawn
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureRangeSkullSpawn"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Range_Skull/Spawn/Spawn_%d.png"),
			11))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureRangeSkullSpawn");
	// Arrow
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureArrow"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Range_Skull/Arrow/Arrow_%d.png"),
			1))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureArrow");
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Necromencer
	//Cast
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureNecromencerCast"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Necromencer/Cast/Cast_%d.png"),
			12))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureNecromencerCast");
	//Death
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureNecromencerDeath"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Necromencer/Death/Death_%d.png"),
			16))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureNecromencerDeath");
	//Die
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureNecromencerDie"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Necromencer/Die/Die_%d.png"),
			1))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureNecromencerCast");
	//Hit
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureNecromencerHit"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Necromencer/Hit/Hit_%d.png"),
			3))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureNecromencerHit");
	//Idle
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureNecromencerIdle"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Necromencer/Idle/Idle_%d.png"),
			8))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureNecromencerIdle");
	//Move
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureNecromencerMove"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Necromencer/Move/Move_%d.png"),
			8))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureNecromencerMove");
	//Shield
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureShield"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/CubeObj/Shield/Shield_%d.dds"),
			1, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureShield");

	//Resurrection
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureResurrection"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Effect/Aura_1/Aura_%d.png"),
			5))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureResurrection");
	//Fire
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureFire"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Effect/Fire/Fire_%d.png"),
			4))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureFire");

	//Ice
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureIce"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Effect/Ice/Ice_%d.png"),
			1))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureIce");
	//IceOrb
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureIceOrb"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Effect/IceOrb/IceOrb_%d.png"),
			1))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureIceOrb");

	//Cloud_Charge
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureCloud_Charge"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Cloud/Cloud_Charge/Cloud_Charge_%d.png"),
			8))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureCloud_Charge");
	//Cloud_Shot
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureCloud_Shot"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Cloud/Cloud_Shot/Cloud_Shot_%d.png"),
			4))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureCloud_Shot");
	//Electric
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureElectric"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Effect/Electric/Electric_%d.png"),
			9))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureElectric");

	//BlackHole
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureBlackHole"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Effect/BlackHole/BlackHole_%d.png"),
			4))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureBlackHole");

	//Blink
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureBlink"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Effect/Blink/Blink_%d.png"),
			7))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureBlink");

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Chimera
	//Attack
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureChimeraAttack"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Chimera/Attack_1/Attack_%d.png"),
			18))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureChimeraAttack");
	//Cast
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureChimeraCast"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Chimera/Attack_2/Attack_%d.png"),
			17))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureChimeraCast");
	//Death
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureChimeraDeath"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Chimera/Death/Death_%d.png"),
			24))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureChimeraDeath");
	//Idle
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureChimeraIdle"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Chimera/Idle/Idle_%d.png"),
			13))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureChimeraIdle");
	//Move
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureChimeraMove"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/Chimera/Move/Move_%d.png"),
			13))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureChimeraMove");

	//Move
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureDarkOrb"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Effect/DarkOrb/DarkOrb_%d.png"),
			1))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureDarkOrb");

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TombStone
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureTombStone"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/CubeObj/TombStone/TombStone_%d.dds"),
			1, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureTombStone");

	// TombStoneCross
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureTombStoneCross"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/CubeObj/TombStoneCross/TombStoneCross_%d.dds"),
			1, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureTombStoneCross");

	// TombStoneBase
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureTombStoneBase"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/CubeObj/TombStoneBase/TombStoneBase_%d.dds"),
			1, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureTombStoneBase");

	// TombStoneParticle
	if (FAILED(pInstance->Add_Prototype(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTextureTombStoneParticle"),
		CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Effect/TombStoneParticle/TombStoneParticle_%d.png"),
			1))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureTombStoneParticle");

	wsprintf(m_szLoading, TEXT("객체를 생성중입니다. "));
	wsprintf(m_szLoading, TEXT("로딩이 완료되었습니다. "));
	m_isFinished = true;
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::Ready_TextureGamePlayMonster(CGameInstance * pInstance)
{
	// 근거리 일반 몹
	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMonsterNormalMeleeAttack"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/MonsterNormalMelee/Attack/Attack%d.png"), 7))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMonsterNormalMeleeAttack");

	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMonsterNormalMeleeDie"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/MonsterNormalMelee/Die/Die%d.png"), 3))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMonsterNormalMeleeDie");

	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMonsterNormalMeleeHit"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/MonsterNormalMelee/Hit/Hit%d.png"), 2))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMonsterNormalMeleeHit");

	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMonsterNormalMeleeIdle"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/MonsterNormalMelee/Idle/Idle%d.png"), 1))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMonsterNormalMeleeIdle");

	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMonsterNormalMeleeMove"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/MonsterNormalMelee/Move/Move%d.png"), 4))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMonsterNormalMeleeMove");

	// 원거리 일반 몹
	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMonsterNormalRangeMove"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/MonsterNormalRange/Move/Move%d.png"), 4))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMonsterNormalRangeMove");

	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMonsterNormalRangeAttack"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/MonsterNormalRange/Attack/Attack%d.png"), 8))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMonsterNormalRangeAttack");

	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMonsterNormalRangeDie"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/MonsterNormalRange/Die/Die%d.png"), 3))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMonsterNormalRangeDie");

	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMonsterNormalRangeHit"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/MonsterNormalRange/Hit/Hit%d.png"), 2))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMonsterNormalRangeHit");

	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMonsterNormalRangeIdle"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/Monster/MonsterNormalRange/Idle/Idle%d.png"), 1))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMonsterNormalRangeIdle");

	// 원거리 일반 몹 미사일
	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureMagicMissile"), CTexture::Create(m_pGraphicDevice, TEXT("../bin/Resource/Texture/CubeObj/MagicMissile/MagicMissile%d.dds"), 2, CTexture::TYPE_CUBE))))
		MSG_FAIL("CLoader LoadingForGamePlay - Add_Prototype PrototypeTextureMagicMissile");

	return S_OK;
}

CLoader * CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel)
{
	CLoader* pLoader = new CLoader(pGraphic_Device);

	if (FAILED(pLoader->NativeConstruct(eLevel)))
	{
		Safe_Release(pLoader);
		MSGBOX("Loader Create Failed");
	}

	return pLoader;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);
	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CS);
	Safe_Release(m_pGraphicDevice);
}
