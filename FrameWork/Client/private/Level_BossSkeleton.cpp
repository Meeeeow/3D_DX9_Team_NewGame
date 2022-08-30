#include "stdafx.h"
#include "..\public\Level_BossSkeleton.h"
#include "GameInstance.h"
#include "LoadingDatFile.h"
#include "VIBufferTerrain.h"
#include "CameraFree.h"
#include "Player.h"
#include "Subject.h"
#include "Observer.h"
#include "Level_Loading.h"
#include "BossSkeletonMagicCircle.h"
#include "DmgFont.h"
#include "PlayerTagMgr.h"
#include "Rain.h"
#include "BossSkeletonStageCube.h"
#include "Crosshair.h"
#include "WallLook.h"
#include "WallSide.h"
#include "BossSkeleton.h"
#include "TipMgr.h"


CLevel_BossSkeleton::CLevel_BossSkeleton(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CLevel(pGraphic_Device)
{
}

HRESULT CLevel_BossSkeleton::NativeConstruct(_uint iLevelID)
{
	if (FAILED(__super::NativeConstruct(iLevelID)))
		return E_FAIL;

	CLoadingDatFile* pLoadInstance = GET_INSTANCE(CLoadingDatFile);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	size_t iSize = pLoadInstance->Get_VectorSize(pTagBossSkeletonSceneName);
	for (size_t i = 0; i < iSize; ++i)
	{
		OBJ_INFO tObjInfo = *pLoadInstance->Get_ObjInfo(pTagBossSkeletonSceneName, (_uint)i);

		//보스 큐브
		if (0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeBossSkeletonStageCube))
		{
			CBossSkeletonStageCube::DESC tDesc;
			tDesc.tObjInfo = tObjInfo;
			lstrcpy(tDesc.tObjInfo.szTextureName, pTagPrototypeTextureBossSkeletonStageCube);
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_SKELETON, pTagPrototypeBossSkeletonStageCube, pTagLayerBossSkeletonCube, &tDesc)))
				MSG_FAIL("CLevel_BossSkeleton::NativeConstruct - Add_GameObjectToLayer pTagPrototypeBossSkeletonStageCube");
		}

		//보스 생성
		if (0 == lstrcmp(tObjInfo.szObjName, L"BossSkeleton"))
		{
			CBossSkeleton::DESC tDesc;
			tDesc.tObjInfo = tObjInfo;
			lstrcpy(tDesc.tObjInfo.szTextureName, pTagPrototypeTextureBossSkeleton);
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_SKELETON, pTagPrototypeBossSkeleton, pTagLayerMonster, &tDesc)))
				MSG_FAIL("CLevel_BossSkeleton::NativeConstruct - Add_GameObjectToLayer pTagPrototypeBossSkeleton");
		}

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
		if (0 == lstrcmp(tObjInfo.szObjName, L"BossSkeletonWallBack") || 0 == lstrcmp(tObjInfo.szObjName, L"BossSkeletonWallFront"))
		{
			CWallLook::DESC tDesc;
			tDesc.tObjInfo = tObjInfo;
			lstrcpy(tDesc.tObjInfo.szTextureName, pTagPrototypeTextureWall);
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_SKELETON, pTagPrototypeWallLook, pTagLayerWall, &tDesc)))
				MSG_FAIL("CLevel_BossSkeleton::NativeConstruct - Add_GameObjectToLayer pTagPrototypeWallLook");
		}

		//오른쪽 벽 생성
		if (0 == lstrcmp(tObjInfo.szObjName, L"BossSkeletonWallLeft") || 0 == lstrcmp(tObjInfo.szObjName, L"BossSkeletonWallRight"))
		{
			CWallSide::DESC tDesc;
			tDesc.tObjInfo = tObjInfo;
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_SKELETON, pTagPrototypeWallSide, pTagLayerWall, &tDesc)))
				MSG_FAIL("CLevel_BossSkeleton::NativeConstruct - Add_GameObjectToLayer pTagPrototypeWallSide");
		}
	}

	if (FAILED(Ready_CollisionTag()))
		MSG_FAIL("CLevel_BossSkeleton NativeConstruct - Ready_CollisionTag");

	if (FAILED(Ready_Layer_BackGround(TEXT("LayerBackGround"))))
		MSG_FAIL("CLevel_BossSkeleton NativeConstruct - Ready_Layer_BackGround");

	if (FAILED(Ready_Layer_Player(pTagLayerPlayer)))
		MSG_FAIL("CLevel_BossSkeleton NativeConstruct - Ready_Layer_Player");

	//카메라는 플레이어를 참조하기 때문에 뒤에 있어야함
	if (FAILED(Ready_Layer_Camera(TEXT("LayerCamera"))))
		MSG_FAIL("CLevel_BossSkeleton NativeConstruct - Ready_Layer_Camera");

	if (FAILED(Ready_Layer_Environment(pTagLayerEnvironment)))
		MSG_FAIL("CLevel_BossSkeleton NativeConstruct - Ready_Layer_Environment");

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CLoadingDatFile);
	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"BossSkeletonBgm.ogg");
	return S_OK;
}

_int CLevel_BossSkeleton::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CTipMgr::Get_Instance()->Tick(fTimeDelta);

	if (pGameInstance->Key_Down(DIK_P))
	{
		CLevel*		pLevel = CLevel_Loading::Create(m_pGraphic_Device, LEVEL_BOSS_CIRCUS, LEVEL_LOADING);
		if (pLevel == nullptr)
			MSG_MINUSONE("CLevel_BossSkeleton::Tick - pLevel == nullptr");
		
		if (FAILED(pGameInstance->Change_Level(pLevel)))
			MSG_MINUSONE("CLevel_BossSkeleton::Tick - pGameInstance->Change_Level");
	}
	
	RELEASE_INSTANCE(CGameInstance)

	return 0;
}

HRESULT CLevel_BossSkeleton::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_BossSkeleton::Ready_CollisionTag()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//콜리젼 생성 코드
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_BOSS_SKELETON, pTagLayerBossSkeletonCube);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_BOSS_SKELETON, pTagLayerWall);

	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayerAttack, LEVEL_BOSS_SKELETON, pTagLayerMonster);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayerAttack, LEVEL_BOSS_SKELETON, pTagLayerBossSkeletonCube);

	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerMonsterAttack, LEVEL_STATIC, pTagLayerPlayer);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_BOSS_SKELETON, pTagLayerWall);

	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_STATIC, L"LayerPortal");

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_BossSkeleton::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CCamera::INIT_CAMERA tInitCam{};
	tInitCam.vEye = _vec3(-10.f, 5.f, -10.f);
	tInitCam.fAspect = g_iWinCX / (_float)g_iWinCY;
	tInitCam.fFar = 300.f;
	tInitCam.fNear = 0.2f;
	tInitCam.fFovY = 45.f;
	
	if (FAILED(pGameInstance->Add_CameraToContainer(LEVEL_BOSS_SKELETON, TEXT("PrototypeCameraTargetBack"), CAMERA_BACK, &tInitCam)))
		MSG_FAIL("CLevel_Logo Ready_Layer_Camera - Add_CameraToContainer - BACK");

	if(FAILED(pGameInstance->Add_CameraToContainer(LEVEL_BOSS_SKELETON, TEXT("PrototypeCamera2D"),CAMERA_2D)))
		MSG_FAIL("CLevel_Logo Ready_Layer_Camera - Add_CameraToContainer - 2D");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_BossSkeleton::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_SKELETON, TEXT("PrototypeSkyBox"), TEXT("LayerSkyBox"))))
		MSG_FAIL("CLevel_Logo Ready_Layer_Backgroud - Add_GameObjectToLayer PrototypeSkyBox");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_BossSkeleton::Ready_Layer_Player(const _tchar * pLayerTag)
{

	//CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	//CTransform* pTransform = CPlayerTagMgr::Get_Instance()->Get_CurPlayerTransform();
	//pTransform->SetUp_State(CTransform::STATE_POS, _vec3(1.f, 100.f, 1.f));

	//RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_BossSkeleton::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	CRain::DESC tDesc;
	tDesc.tRainDropDesc.fDegree = -60.f;
	OBJ_INFO* pTemp = new OBJ_INFO;
	lstrcpy(pTemp->szTextureName,pTagPrototypeTextureRainDrop);
	tDesc.tRainDropDesc.pObjInfo = pTemp;
	if (FAILED(pInstance->Add_GameObjectToLayer(LEVEL_BOSS_SKELETON, pTagPrototypeRain, pLayerTag, &tDesc)))
		MSG_FAIL("CLevel_BossSkeleton Ready_Layer_Effect - Add_GameObjectToLayer PrototypeBossSkeletonMagicCircle");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_BossSkeleton * CLevel_BossSkeleton::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iLevelID)
{
	CLevel_BossSkeleton* pInstance = new CLevel_BossSkeleton(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(iLevelID)))
	{
		MSGBOX("Failed To Creating CLevel_BossSkeleton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_BossSkeleton::Free()
{
	__super::Free();
}
