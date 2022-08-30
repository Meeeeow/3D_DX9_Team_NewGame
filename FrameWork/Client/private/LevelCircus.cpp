#include "stdafx.h"
#include "..\public\LevelCircus.h"
#include "LoadingDatFile.h"
#include "Player.h"
#include "Camera.h"
#include "TrumpCardClover.h"
#include "SkyBox.h"
#include "TriggerCollider.h"
#include "TipMgr.h"

CLevelCircus::CLevelCircus(_pDevice pGraphicDevice)
	: CLevel(pGraphicDevice)
{
}

HRESULT CLevelCircus::NativeConstruct(_uint iLevelID)
{
	if (FAILED(__super::NativeConstruct(iLevelID)))
		return E_FAIL;
	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Splendid_Circus.wav");
	
	CLoadingDatFile* pLoadInstance = GET_INSTANCE(CLoadingDatFile);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	size_t iSize = pLoadInstance->Get_VectorSize(pTagBossClownScnenName);
	for (size_t i = 0; i < iSize; ++i)
	{
		OBJ_INFO tObjInfo = *pLoadInstance->Get_ObjInfo(pTagBossClownScnenName, (_uint)i);

		// 타일 블록 설치
		if (0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCircusCubeLegoBlue))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeCircusCubeLegoBlue, pTagLayerCircusCube, &tObjInfo)))
				MSG_FAIL("CLevelCircus::NativeConstruct - CubeLegoBlue");
		}
		if (0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCircusCubeLegoGray))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeCircusCubeLegoGray, pTagLayerCircusCube, &tObjInfo)))
				MSG_FAIL("CLevelCircus::NativeConstruct - CubeLegoGray");
		}
		if (0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCircusCubeLegoRed))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeCircusCubeLegoRed, pTagLayerCircusCube, &tObjInfo)))
				MSG_FAIL("CLevelCircus::NativeConstruct - CubeLegoRed");
		}
		// 벽 방향 별 설치 - 파랑
		if (0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCircusWallLegoBlueB)
		||  0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCircusWallLegoBlueF))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeWallLook, pTagLayerCircusWall, &tObjInfo)))
				MSG_FAIL("CLevelCircus::NativeConstruct - WallBlueBF");
		}
		if (0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCircusWallLegoBlueR)
		||  0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCircusWallLegoBlueL))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeWallSide, pTagLayerCircusWall, &tObjInfo)))
				MSG_FAIL("CLevelCircus::NativeConstruct - WallBlueLR");
		}
		// 벽 방향 별 설치 - 빨강
		if (0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCircusWallLegoRedB)
		||  0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCircusWallLegoRedF))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeWallLook, pTagLayerCircusWall, &tObjInfo)))
				MSG_FAIL("CLevelCircus::NativeConstruct - WallRedBF");
		}
		if (0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCircusWallLegoRedR)
		||  0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCircusWallLegoRedL))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeWallSide, pTagLayerCircusWall, &tObjInfo)))
				MSG_FAIL("CLevelCircus::NativeConstruct - WallRedLR");
		}
		// 벽 방향 별 설치 - 회색
		if (0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCircusWallLegoGrayB)
		||  0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCircusWallLegoGrayF))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeWallLook, pTagLayerCircusWall, &tObjInfo)))
				MSG_FAIL("CLevelCircus::NativeConstruct - WallGrayBF");
		}
		if (0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCircusWallLegoGrayR)
		||  0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCircusWallLegoGrayL))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeWallSide, pTagLayerCircusWall, &tObjInfo)))
				MSG_FAIL("CLevelCircus::NativeConstruct - WallGrayLR");
		}

		// 보스
		if (0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCircusBossClown))
		{
			
			lstrcpy(tObjInfo.szTextureName, L"PrototypeTextureBossClown");
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeCircusBossClown, pTagLayerMonster, &tObjInfo)))
				MSG_FAIL("CLevelCircus::NativeConstruct - PrototypeTextureBossClown");
		}

		if (0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCircusCubeBroken))
		{
			if(FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeCircusCubeBroken, pTagLayerCircusBrokenCube, &tObjInfo)))
				MSG_FAIL("CLevelCircus::NativeConstruct - pTagPrototypeCircusCubeBroken");
		}

		//// 플레이어 위치
		//if (0 == lstrcmp(tObjInfo.szObjName, L"Player"))
		//{
		//	for (int i = 0; i < 3; ++i)
		//	{
		//		CGameObject* pGameObj = pGameInstance->Get_GameObjPtr(LEVEL_STATIC, pTagLayerPlayer, i);
		//		pGameObj->Set_Pos(_vec3(8.5f,10.5f,4.5f), dynamic_cast<CTransform*>(pGameObj->Get_Component(pTagComTransform)));
		//		//pGameObj->Set_Pos(tObjInfo.Get_State(OBJ_INFO::STATE_POS), dynamic_cast<CTransform*>(pGameObj->Get_Component(pTagComTransform)));
		//	}
		//}
	}
	CTriggerCollider::DESC tTriggerColliderDesc;
	tTriggerColliderDesc.tObjInfo.dwColliderColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tTriggerColliderDesc.tObjInfo.vColliderCenter = _vec3(0.f, 0.f, 0.f);
	tTriggerColliderDesc.tObjInfo.vColliderSize = _vec3(1.f, 1.f, 1.f);
	tTriggerColliderDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, _vec3(0.f, 2.f, 4.f));
	tTriggerColliderDesc.tObjInfo.SetUp_Scale(1.f, 3.f, 6.f);
	tTriggerColliderDesc.bTrans = false;
	tTriggerColliderDesc.eTuto = CTutorialMgr::TUTORIAL_CIRCUS_CAMERA;

	tTriggerColliderDesc.vTargetPos = _vec3(1.f, 1.f, 3.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeTriggerCollider, pTagLayerCircusCube
		, &tTriggerColliderDesc)))
		MSG_FAIL("CLevelCircus::NativeConstruct - pTagPrototypeTriggerCollider");

	tTriggerColliderDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, _vec3(139.f, 2.f, 4.f));
	tTriggerColliderDesc.tObjInfo.SetUp_Scale(1.f, 3.f, 6.f);
	tTriggerColliderDesc.bTrans = false;
	tTriggerColliderDesc.eTuto = CTutorialMgr::TUTORIAL_CLOWN_TELEKINESIS;
	
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeTriggerCollider, pTagLayerCircusCube
		, &tTriggerColliderDesc)))
		MSG_FAIL("CLevelCircus::NativeConstruct - pTagPrototypeTriggerCollider");

	tTriggerColliderDesc.tObjInfo.dwColliderColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tTriggerColliderDesc.tObjInfo.vColliderCenter = _vec3(0.f, 0.f, 0.f);
	tTriggerColliderDesc.tObjInfo.vColliderSize = _vec3(1.f, 1.f, 1.f);
	tTriggerColliderDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, _vec3(0.f, -80.f, 0.f));
	tTriggerColliderDesc.tObjInfo.SetUp_Scale(700.f, 15.f, 600.f);
	tTriggerColliderDesc.bTrans = true;
	tTriggerColliderDesc.eTuto = CTutorialMgr::TUTORIAL_FALL;

	tTriggerColliderDesc.vTargetPos = _vec3(8.f, 3.f, 3.f);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_CIRCUS, pTagPrototypeTriggerCollider, pTagLayerCircusCube
		, &tTriggerColliderDesc)))
		MSG_FAIL("CLevelCircus::NativeConstruct - pTagPrototypeTriggerCollider");

	if (FAILED(Ready_CollisionTag()))
		MSG_FAIL("CLevelCircus NativeConstruct - Ready_CollisionTag");

	if (FAILED(Ready_Layer_BackGround(TEXT("LayerBackGround"))))
		MSG_FAIL("CLevelCircus NativeConstruct - Ready_Layer_BackGround");

	if (FAILED(Ready_Layer_Monster(pTagLayerMonster)))
		MSG_FAIL("CLevelCircus NativeConstruct - Ready_Layer_Monster");

	if (FAILED(Ready_Layer_Player(pTagLayerPlayer)))
		MSG_FAIL("CLevelCircus NativeConstruct - Ready_Layer_Player");

	//카메라는 플레이어를 참조하기 때문에 뒤에 있어야함
	if (FAILED(Ready_Layer_Camera(TEXT("LayerCamera"))))
		MSG_FAIL("CLevelCircus NativeConstruct - Ready_Layer_Camera");

	if (FAILED(Ready_Layer_Environment(pTagLayerEnvironment)))
		MSG_FAIL("CLevelCircus NativeConstruct - Ready_Layer_Environment");

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CLoadingDatFile);
	return S_OK;
}

_int CLevelCircus::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	CTipMgr::Get_Instance()->Tick(fTimeDelta);

	return _int();
}

HRESULT CLevelCircus::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevelCircus::Ready_CollisionTag()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_BOSS_CIRCUS, pTagLayerCircusCube);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_BOSS_CIRCUS, pTagLayerCircusWall);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_BOSS_CIRCUS, pTagLayerCircusBrokenCube);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayerAttack, LEVEL_BOSS_CIRCUS, pTagLayerCircusBrokenCube);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_BOSS_CIRCUS, pTagLayerMonsterAttack);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_BOSS_CIRCUS, pTagLayerMonster);
	pGameInstance->Add_CollisionTag(LEVEL_BOSS_CIRCUS, pTagLayerMonsterAttack, LEVEL_BOSS_CIRCUS, pTagLayerCircusCube);
	pGameInstance->Add_CollisionTag(LEVEL_BOSS_CIRCUS, pTagLayerMonsterAttack, LEVEL_BOSS_CIRCUS, pTagLayerCircusWall);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayerAttack, LEVEL_BOSS_CIRCUS, pTagLayerMonster);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_STATIC, pTagLayerLockOn);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_STATIC, L"LayerPortal");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevelCircus::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CCamera::INIT_CAMERA tInitCam{};
	tInitCam.vEye = _vec3(-10.f, 5.f, -10.f);
	tInitCam.fAspect = g_iWinCX / (_float)g_iWinCY;
	tInitCam.fFar = 450.f;
	tInitCam.fNear = 0.2f;
	tInitCam.fFovY = 45.f;

	if (FAILED(pGameInstance->Add_CameraToContainer(LEVEL_BOSS_CIRCUS, TEXT("PrototypeCameraTargetBack"), CAMERA_BACK, &tInitCam)))
		MSG_FAIL("CLevel_Logo Ready_Layer_Camera - Add_CameraToContainer - BACK");

	if (FAILED(pGameInstance->Add_CameraToContainer(LEVEL_BOSS_CIRCUS, TEXT("PrototypeCamera2D"), CAMERA_2D)))
		MSG_FAIL("CLevel_Logo Ready_Layer_Camera - Add_CameraToContainer - 2D");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevelCircus::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CSkyBox::DESC tDesc;
	tDesc.iNumTexture = 4;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_SKELETON, TEXT("PrototypeSkyBox"), TEXT("LayerSkyBox"),&tDesc)))
		MSG_FAIL("CLevel_Logo Ready_Layer_Backgroud - Add_GameObjectToLayer PrototypeSkyBox");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevelCircus::Ready_Layer_Monster(const _tchar * pLayerTag)
{

	return S_OK;
}

HRESULT CLevelCircus::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	for (int i = 0; i < 3; ++i)
	{
		CPlayer* pGameObj = dynamic_cast<CPlayer*>(pGameInstance->Get_GameObjPtr(LEVEL_STATIC, pTagLayerPlayer, i));
		pGameObj->Set_Pos(_vec3(8.5f, 4.5f, 3.5f), dynamic_cast<CTransform*>(pGameObj->Get_Component(pTagComTransform)));
	}


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevelCircus::Ready_Layer_Environment(const _tchar * pLayerTag)
{



	return S_OK;
}

CLevelCircus * CLevelCircus::Create(_pDevice pGraphicDevice, _uint iLevelID)
{
	CLevelCircus* pInstance = new CLevelCircus(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct(iLevelID)))
	{
		MSGBOX("Failed To Creating CLevelCircus");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevelCircus::Free()
{
	__super::Free();
}
