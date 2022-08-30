#include "stdafx.h"
#include "..\public\LevelNecromencer.h"
#include "LoadingDatFile.h"
#include "Player.h"
#include "Camera.h"
#include "SkyBox.h"
#include "WallLook.h"
#include "WallSide.h"
#include "MonsterColliderManager.h"
#include "TipMgr.h"


CLevelNecromencer::CLevelNecromencer(_pDevice pGraphicDevice)
	: CLevel(pGraphicDevice)
{
}

HRESULT CLevelNecromencer::NativeConstruct(_uint iLevelID)
{
	if (FAILED(__super::NativeConstruct(iLevelID)))
		return E_FAIL;

	CLoadingDatFile* pLoadInstance = GET_INSTANCE(CLoadingDatFile);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	int j = 0;

	size_t iSize = pLoadInstance->Get_VectorSize(pTagBossNecromencerSceneName);
	for (size_t i = 0; i < iSize; ++i)
	{
		OBJ_INFO tObjInfo = *pLoadInstance->Get_ObjInfo(pTagBossNecromencerSceneName, (_uint)i);

		//플레이어 위치 조절
		if (0 == lstrcmp(tObjInfo.szObjName, L"Player"))
		{
			for (int i = 0; i < 3; ++i)
			{
				CGameObject* pGameObj = pGameInstance->Get_GameObjPtr(LEVEL_STATIC, pTagLayerPlayer, i);
				pGameObj->Set_Pos(tObjInfo.Get_State(OBJ_INFO::STATE_POS), dynamic_cast<CTransform*>(pGameObj->Get_Component(pTagComTransform)));
			}
		}

		//앞, 뒤쪽 벽 생성
		if (0 == lstrcmp(tObjInfo.szObjName, L"NecroBackWall") || 0 == lstrcmp(tObjInfo.szObjName, L"NecroFrontWall") || 0 == lstrcmp(tObjInfo.szObjName, L"NecroGate"))
		{
			if (0 == lstrcmp(tObjInfo.szObjName, L"NecroGate"))
				int asdf = 0;

			CWallLook::DESC tDesc;
			tDesc.tObjInfo = tObjInfo;
			lstrcpy(tDesc.tObjInfo.szTextureName, pTagPrototypeTextureWall);
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_NECROMENCER, pTagPrototypeWallLook, pTagLayerWall, &tDesc)))
				MSG_FAIL("CLevel_BossSkeleton::NativeConstruct - Add_GameObjectToLayer pTagPrototypeWallLook");
		}

		//오른쪽 벽 생성
		if (0 == lstrcmp(tObjInfo.szObjName, L"NecroLeftWall") || 0 == lstrcmp(tObjInfo.szObjName, L"NecroRightWall"))
		{
			CWallSide::DESC tDesc;
			tDesc.tObjInfo = tObjInfo;
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_NECROMENCER, pTagPrototypeWallSide, pTagLayerWall, &tDesc)))
				MSG_FAIL("CLevel_BossSkeleton::NativeConstruct - Add_GameObjectToLayer pTagPrototypeWallSide");
		}

		//터레인
		if (0 == lstrcmp(tObjInfo.szObjName, L"NecromencerTerrain"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTerrain"), TEXT("LayerTerrain"), &tObjInfo)))
				MSG_FAIL("CLevel_BossSkeleton::NativeConstruct - Add_GameObjectToLayer PrototypeTerrainNecromencer");
		}

		//Necromencer
		if (0 == lstrcmp(tObjInfo.szObjName, L"Necromencer"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeNecromencer"), TEXT("LayerMonster"), &tObjInfo)))
				MSG_FAIL("CLevel_BossSkeleton::NativeConstruct - Add_GameObjectToLayer PrototypeNecromencer");
		}

		//MeleeSkull
		if (0 == lstrcmp(tObjInfo.szObjName, L"MeleeSkull"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeMeleeSkull"), TEXT("LayerMonster"), &tObjInfo)))
				MSG_FAIL("CLevel_BossSkeleton::NativeConstruct - Add_GameObjectToLayer PrototypeMeleeSkull");
		}

		//RangeSkull
		if (0 == lstrcmp(tObjInfo.szObjName, L"RangeSkull"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeRangeSkull"), TEXT("LayerMonster"), &tObjInfo)))
				MSG_FAIL("CLevel_BossSkeleton::NativeConstruct - Add_GameObjectToLayer PrototypeRangeSkull");
		}

		//Chimera
		if (0 == lstrcmp(tObjInfo.szObjName, L"Chimera"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeChimera"), TEXT("LayerMonster"), &tObjInfo)))
				MSG_FAIL("CLevel_BossSkeleton::NativeConstruct - Add_GameObjectToLayer PrototypeChimera");
		}

		//TombStone
		if (0 == lstrcmp(tObjInfo.szObjName, L"TombStone"))
		{
			//if (j > 0)
			//	continue;

			//++j;
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeTombStone"), TEXT("LayerMonster"), &tObjInfo)))
				MSG_FAIL("CLevel_BossSkeleton::NativeConstruct - Add_GameObjectToLayer PrototypeTombStone");
		}

		//NecromencerBuffer
		if (0 == lstrcmp(tObjInfo.szObjName, L"NecromencerBuffer"))
		{
			CVIBufferRect::SIZE_POS tSizePos;
			tSizePos.vSize = _vec3(tObjInfo.fSizeX, tObjInfo.fSizeY, tObjInfo.fSizeZ);
			tSizePos.vPos = tObjInfo.vVertexPos;

			CMonsterColliderManager::Get_Instance()->Add_ColliderDat(CMonsterColliderManager::COLLIDER_NECROMENCER, &tObjInfo);

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("NecromencerBuffer"), CVIBufferRect::Create(m_pGraphic_Device, &tSizePos))))
				MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype NecromencerBuffer");
		}

		//MeleeSkullBuffer
		if (0 == lstrcmp(tObjInfo.szObjName, L"MeleeSkullBuffer"))
		{
			CVIBufferRect::SIZE_POS tSizePos;
			tSizePos.vSize = _vec3(tObjInfo.fSizeX, tObjInfo.fSizeY, tObjInfo.fSizeZ);
			tSizePos.vPos = tObjInfo.vVertexPos;

			CMonsterColliderManager::Get_Instance()->Add_ColliderDat(CMonsterColliderManager::COLLIDER_MELEE_SKULL, &tObjInfo);

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("MeleeSkullBuffer"), CVIBufferRect::Create(m_pGraphic_Device, &tSizePos))))
				MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype MeleeSkullBuffer");
		}

		//RangeSkullBuffer
		if (0 == lstrcmp(tObjInfo.szObjName, L"RangeSkullBuffer"))
		{
			CVIBufferRect::SIZE_POS tSizePos;
			tSizePos.vSize = _vec3(tObjInfo.fSizeX, tObjInfo.fSizeY, tObjInfo.fSizeZ);
			tSizePos.vPos = tObjInfo.vVertexPos;

			CMonsterColliderManager::Get_Instance()->Add_ColliderDat(CMonsterColliderManager::COLLIDER_RANGE_SKULL, &tObjInfo);

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("RangeSkullBuffer"), CVIBufferRect::Create(m_pGraphic_Device, &tSizePos))))
				MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype RangeSkullBuffer");
		}

		//ChimeraBuffer
		if (0 == lstrcmp(tObjInfo.szObjName, L"ChimeraBuffer"))
		{
			CVIBufferRect::SIZE_POS tSizePos;
			tSizePos.vSize = _vec3(tObjInfo.fSizeX, tObjInfo.fSizeY, tObjInfo.fSizeZ);
			tSizePos.vPos = tObjInfo.vVertexPos;

			CMonsterColliderManager::Get_Instance()->Add_ColliderDat(CMonsterColliderManager::COLLIDER_CHIMERA, &tObjInfo);

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ChimeraBuffer"), CVIBufferRect::Create(m_pGraphic_Device, &tSizePos))))
				MSG_FAIL("CMainApp Ready_Prototype_Component_ForStatic - Add_Prototype ChimeraBuffer");
		}

		//TombStoneCross
		if (0 == lstrcmp(tObjInfo.szObjName, L"TombStoneCross"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeCrossBlock"), TEXT("LayerBlock"), &tObjInfo)))
				MSG_FAIL("CLevel_BossSkeleton::NativeConstruct - Add_GameObjectToLayer PrototypeCrossBlock");
		}

		//TombStoneBase
		if (0 == lstrcmp(tObjInfo.szObjName, L"TombStoneBase"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeBaseBlock"), TEXT("LayerBlock"), &tObjInfo)))
				MSG_FAIL("CLevel_BossSkeleton::NativeConstruct - Add_GameObjectToLayer PrototypeBaseBlock");
		}

	}

	if (FAILED(Ready_CollisionTag()))
		MSG_FAIL("CLevel_BossSkeleton NativeConstruct - Ready_CollisionTag");

	if (FAILED(Ready_Layer_BackGround(TEXT("LayerBackGround"))))
		MSG_FAIL("CLevel_BossSkeleton NativeConstruct - Ready_Layer_BackGround");

	//카메라는 플레이어를 참조하기 때문에 뒤에 있어야함
	if (FAILED(Ready_Layer_Camera(TEXT("LayerCamera"))))
		MSG_FAIL("CLevel_BossSkeleton NativeConstruct - Ready_Layer_Camera");

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CLoadingDatFile);
	return S_OK;
}

_int CLevelNecromencer::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	CTipMgr::Get_Instance()->Tick(fTimeDelta);


	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//if (pGameInstance->Key_Down(DIK_P))
	//{
	//	CLevel*		pLevel = CLevel_Loading::Create(m_pGraphic_Device, LEVEL_BOSS_CIRCUS, LEVEL_LOADING);
	//	if (pLevel == nullptr)
	//		MSG_MINUSONE("CLevel_BossSkeleton::Tick - pLevel == nullptr");

	//	if (FAILED(pGameInstance->Change_Level(pLevel)))
	//		MSG_MINUSONE("CLevel_BossSkeleton::Tick - pGameInstance->Change_Level");
	//}

	//RELEASE_INSTANCE(CGameInstance)

		return 0;
}

HRESULT CLevelNecromencer::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevelNecromencer::Ready_CollisionTag()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//콜리젼 생성 코드
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_BOSS_NECROMENCER, pTagLayerTileCube);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_BOSS_NECROMENCER, pTagLayerWall);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayerAttack, LEVEL_BOSS_NECROMENCER, pTagLayerMonster);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerMonsterAttack, LEVEL_STATIC, pTagLayerPlayer);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_STATIC, L"LayerPortal");

	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_STATIC, pTagLayerLockOn);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_STATIC, pTagLayerPortal);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevelNecromencer::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CCamera::INIT_CAMERA tInitCam{};
	tInitCam.vEye = _vec3(-10.f, 5.f, -10.f);
	tInitCam.fAspect = g_iWinCX / (_float)g_iWinCY;
	tInitCam.fFar = 300.f;
	tInitCam.fNear = 0.2f;
	tInitCam.fFovY = 45.f;

	if (FAILED(pGameInstance->Add_CameraToContainer(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeCameraTargetBack"), CAMERA_BACK, &tInitCam)))
		MSG_FAIL("CLevelNecromencer Ready_Layer_Camera - Add_CameraToContainer - BACK");

	if (FAILED(pGameInstance->Add_CameraToContainer(LEVEL_BOSS_NECROMENCER, TEXT("PrototypeCamera2D"), CAMERA_2D)))
		MSG_FAIL("CLevelNecromencer Ready_Layer_Camera - Add_CameraToContainer - 2D");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevelNecromencer::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CSkyBox::DESC tDesc;
	tDesc.iNumTexture = 5;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_SKELETON, TEXT("PrototypeSkyBox"), TEXT("LayerSkyBox"), &tDesc)))
		MSG_FAIL("CLevelNecromencer Ready_Layer_Backgroud - Add_GameObjectToLayer PrototypeSkyBox");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevelNecromencer * CLevelNecromencer::Create(_pDevice pGraphicDevice, _uint iLevelID)
{
	CLevelNecromencer* pInstance = new CLevelNecromencer(pGraphicDevice);

	if (FAILED(pInstance->NativeConstruct(iLevelID)))
	{
		MSGBOX("Failed To Creating CLevelNecromencer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevelNecromencer::Free()
{
	__super::Free();
}
