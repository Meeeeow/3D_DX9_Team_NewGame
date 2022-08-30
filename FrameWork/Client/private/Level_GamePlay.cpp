#include "stdafx.h"
#include "..\public\Level_GamePlay.h"
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
#include "TagGauge.h"
#include "NonControl.h"
#include "Portal.h"
#include "SkyBox.h"
#include "DialogMgr.h"
#include "WallLook.h"
#include "WallSide.h"
#include "Dialog.h"
#include "WaterCannon.h"
#include "TriggerCollider.h"
#include "TipMgr.h"


CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CLevel(pGraphic_Device)
{
}

HRESULT CLevel_GamePlay::NativeConstruct(_uint iLevelID)
{
	if (FAILED(__super::NativeConstruct(iLevelID)))
		return E_FAIL;

	CLoadingDatFile* pLoadInstance = GET_INSTANCE(CLoadingDatFile);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	size_t iSize = pLoadInstance->Get_VectorSize(L"Intro");
	for (size_t i = 0; i < iSize; ++i)
	{
		OBJ_INFO tObjInfo = *pLoadInstance->Get_ObjInfo(L"Intro", (_uint)i);

		// 타일 블록 설치
		if (0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCaveBloc))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pTagPrototypeCaveBloc, pTagLayerTileCube, &tObjInfo)))
				MSG_FAIL("CLevelGamePlay::NativeConstruct - pTagPrototypeCaveBloc");
		}
		if (0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeCaveDoorDestroyable))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pTagPrototypeCaveDoorDestroyable, pTagLayerTileCube, &tObjInfo)))
				MSG_FAIL("CLevelGamePlay::NativeConstruct - pTagPrototypeCaveDoorDestroyable");
		}
		if (0 == lstrcmp(tObjInfo.szObjName, L"PrototypeWallRIntro")
			|| 0 == lstrcmp(tObjInfo.szObjName, L"PrototypeWallLIntro"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pTagPrototypeWallSide, pTagLayerWall, &tObjInfo)))
				MSG_FAIL("CLevelGamePlay::NativeConstruct - pTagPrototypeWallSide");
		}
		if (0 == lstrcmp(tObjInfo.szObjName, L"PrototypeWallFIntro")
			|| 0 == lstrcmp(tObjInfo.szObjName, L"PrototypeWallBIntro"))
		{
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pTagPrototypeWallLook, pTagLayerWall, &tObjInfo)))
				MSG_FAIL("CLevelGamePlay::NativeConstruct - pTagPrototypeWallLook");
		}

		if (0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeMonsterNormalMelee))
		{
			lstrcpy(tObjInfo.szTextureName, L"PrototypeTextureMonsterNormalMelee");
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pTagPrototypeMonsterNormalMelee, pTagLayerMonster, &tObjInfo)))
				MSG_FAIL("CLevelGamePlay::NativeConstruct - pTagPrototypeMonsterNormalMelee");

		}

		if (0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeMonsterNormalRange))
		{
			lstrcpy(tObjInfo.szTextureName, L"PrototypeTextureMonsterNormalRange");
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pTagPrototypeMonsterNormalRange, pTagLayerMonster, &tObjInfo)))
				MSG_FAIL("CLevelGamePlay::NativeConstruct - pTagPrototypeMonsterNormalRange");
		}

		if (0 == lstrcmp(tObjInfo.szObjName, pTagPrototypeBossSkeletonStageCube))
		{
			CBossSkeletonStageCube::DESC tDesc;
			tDesc.tObjInfo = tObjInfo;
			lstrcpy(tDesc.tObjInfo.szTextureName, pTagPrototypeTextureBossSkeletonStageCube);
			CBossSkeletonStageCube* pGameObj;
			if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pTagPrototypeBossSkeletonStageCube, pTagLayerBossSkeletonCube, &tDesc, (void**)&pGameObj)))
				MSG_FAIL("CLevel_BossSkeleton::NativeConstruct - Add_GameObjectToLayer pTagPrototypeBossSkeletonStageCube");

			_vec3 vPos = tObjInfo.Get_State(OBJ_INFO::STATE_POS);
			if (vPos == _vec3(75.f, -25.f, 125.f))
			{
				pGameObj->Set_State(CBossSkeletonStageCube::FSM_MAGMA);
				pGameObj->Set_TutorialMagma();
			}
		}
	}

	CTriggerCollider::DESC tTriggerColliderDesc;
	tTriggerColliderDesc.tObjInfo.dwColliderColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tTriggerColliderDesc.tObjInfo.vColliderCenter = _vec3(0.f, 0.f, 0.f);
	tTriggerColliderDesc.tObjInfo.vColliderSize = _vec3(1.f, 1.f, 1.f);
	tTriggerColliderDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, _vec3(75.f, -30.f, 75.f));
	tTriggerColliderDesc.tObjInfo.SetUp_Scale(150.f, 40.f, 50.f);
	tTriggerColliderDesc.eTuto = CTutorialMgr::TUTORIAL_MAGMA;

	tTriggerColliderDesc.vTargetPos = _vec3(75.f, 1.f, 45.f);
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pTagPrototypeTriggerCollider, pTagLayerBossSkeletonCube
		, &tTriggerColliderDesc)))
		MSG_FAIL("CLevel_BossSkeleton::NativeConstruct - pTagPrototypeTriggerCollider");



	CWaterCannon::DESC tWaterCannonDesc;
	tWaterCannonDesc.vPos = _vec3(150.f, 5.f, 75.f);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pTagPrototypeWaterCannon, pTagLayerBossSkeletonCube
		, &tWaterCannonDesc)))
		MSG_FAIL("CLevel_BossSkeleton::NativeConstruct - pTagPrototypeWaterCannon");

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CLoadingDatFile);

	_mat		ProjMatrix;
	D3DXMatrixPerspectiveFovLH(&ProjMatrix, D3DXToRadian(45.f), g_iWinCX / _float(g_iWinCY), 0.2f, 50.f);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &ProjMatrix);

	if (FAILED(Ready_Layer_Player(pTagLayerPlayer)))
		MSG_FAIL("CLevel_GamePlay NativeConstruct - Ready_Layer_Player");

	if (FAILED(Ready_CollisionTag()))
		MSG_FAIL("CLevel_GamePlay NativeConstruct - Ready_CollisionTag");

	if (FAILED(Ready_Layer_BackGround(TEXT("LayerBackGround"))))
		MSG_FAIL("CLevel_GamePlay NativeConstruct - Ready_Layer_BackGround");

	//카메라는 플레이어를 참조하기 때문에 뒤에 있어야함
	if (FAILED(Ready_Layer_Camera(TEXT("LayerCamera"))))
		MSG_FAIL("CLevel_GamePlay NativeConstruct - Ready_Layer_Camera");

	if (FAILED(Ready_Layer_Environment(pTagLayerEnvironment)))
		MSG_FAIL("CLevel_GamePlay NativeConstruct - Ready_Layer_Environment");

	if (FAILED(Ready_Layer_Crosshair(pTagLayerCrosshair)))
		MSG_FAIL("CLevel_GamePlay NativeConstruct - Ready_Layer_BossSkeletonStageCube");

	return S_OK;
}

_int CLevel_GamePlay::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CDialogMgr::Get_Instance()->Tick(fTimeDelta);
	CTipMgr::Get_Instance()->Tick(fTimeDelta);

	if (pGameInstance->Key_Down(DIK_P))
	{
		CLevel*		pLevel = CLevel_Loading::Create(m_pGraphic_Device, LEVEL_BOSS_SKELETON, LEVEL_LOADING);
		if (nullptr == pLevel)
			return -1;

		if (FAILED(pGameInstance->Change_Level(pLevel)))
			return -1;
	}
	if (pGameInstance->Key_Down(DIK_O))
	{
		CLevel*		pLevel = CLevel_Loading::Create(m_pGraphic_Device, LEVEL_BOSS_CIRCUS, LEVEL_LOADING);
		if (nullptr == pLevel)
			return -1;

		if (FAILED(pGameInstance->Change_Level(pLevel)))
			return -1;
	}
	if (pGameInstance->Key_Down(DIK_H))
	{
		CLevel*		pLevel = CLevel_Loading::Create(m_pGraphic_Device, LEVEL_BOSS_NECROMENCER, LEVEL_LOADING);
		if (nullptr == pLevel)
			return -1;

		if (FAILED(pGameInstance->Change_Level(pLevel)))
			return -1;
	}
	RELEASE_INSTANCE(CGameInstance)

	return 0;
}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Draw_Font(L"테스트 TEST Test", D3DCOLOR_ARGB(255, 255, 0, 0), &_vec3(1100.f, 100.f, 1.f));
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_CollisionTag()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//콜리젼 생성 코드
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayerAttack, LEVEL_GAMEPLAY, pTagLayerDestroyableCube);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_GAMEPLAY, pTagLayerTileCube);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayerAttack, LEVEL_GAMEPLAY, pTagLayerTileCube);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayerAttack, LEVEL_GAMEPLAY, pTagLayerMonster);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayerAttack, LEVEL_GAMEPLAY, pTagLayerBossSkeletonCube);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_GAMEPLAY, pTagLayerBossSkeletonCube);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerMonsterAttack, LEVEL_STATIC, pTagLayerPlayer);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_GAMEPLAY, pTagLayerWall);
	pGameInstance->Add_CollisionTag(LEVEL_STATIC, pTagLayerPlayer, LEVEL_STATIC, L"LayerPortal");
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CCamera::INIT_CAMERA tInitCam{};
	tInitCam.vEye = _vec3(-10.f, 5.f, -10.f);
	tInitCam.fAspect = g_iWinCX / (_float)g_iWinCY;
	tInitCam.fFar = 300.f;
	tInitCam.fNear = 0.2f;
	tInitCam.fFovY = 45.f;
	
	if (FAILED(pGameInstance->Add_CameraToContainer(LEVEL_STATIC, TEXT("PrototypeCameraTargetBack"), CAMERA_BACK, &tInitCam)))
		MSG_FAIL("CLevel_Logo Ready_Layer_Camera - Add_CameraToContainer - BACK");

	if(FAILED(pGameInstance->Add_CameraToContainer(LEVEL_STATIC, TEXT("PrototypeCamera2D"),CAMERA_2D)))
		MSG_FAIL("CLevel_Logo Ready_Layer_Camera - Add_CameraToContainer - 2D");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	
	CSkyBox::DESC tSkyBoxDesc;
	tSkyBoxDesc.iNumTexture = 0;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, TEXT("PrototypeSkyBox"), TEXT("LayerSkyBox"),&tSkyBoxDesc)))
		MSG_FAIL("CLevel_GamePlay Ready_Layer_Backgroud - Add_GameObjectToLayer PrototypeSkyBox");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CLoadingDatFile* pLoadingDatFile = GET_INSTANCE(CLoadingDatFile);
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	CPlayer::PLAYER_STAT tPlayerStat;
	tPlayerStat.eType = CPlayer::TYPE_WARRIOR;
	tPlayerStat.iHp = 1000;
	tPlayerStat.iMaxHp = 1000;
	tPlayerStat.iMp = 1000;
	tPlayerStat.iMaxMp = 1000;
	tPlayerStat.iDmg = 100;

	// 플레이어, 옵저버 객체 주소를 받아올 포인터 생성
	CPlayer* pObj = nullptr;
	CObserver* pObserver = nullptr;
	list<CPlayer*> plist;

	wstring wstrSceneName = TEXT("PlayerBufferScene");
	size_t iIndex = pLoadingDatFile->Get_VectorSize(wstrSceneName);
	OBJ_INFO tObjInfo = *pLoadingDatFile->Get_ObjInfo(wstrSceneName, 0);
	tPlayerStat.tObj = tObjInfo;

	// 객체 생성 후 받아오기
	if (FAILED(pInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"PrototypePlayer", pLayerTag, &tPlayerStat, (void**)&pObj)))
		MSG_FAIL("CLevel_GamePlay Ready_Layer_Player - Add_GameObjectToLayer PrototypePlayer");
	plist.emplace_back(pObj);

	tPlayerStat.eType = CPlayer::TYPE_MAGE;
	if (FAILED(pInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"PrototypePlayer", pLayerTag, &tPlayerStat, (void**)&pObj)))
		MSG_FAIL("CLevel_GamePlay Ready_Layer_Player - Add_GameObjectToLayer PrototypePlayer");
	plist.emplace_back(pObj);

	tPlayerStat.eType = CPlayer::TYPE_THIEF;
	if (FAILED(pInstance->Add_GameObjectToLayer(LEVEL_STATIC, L"PrototypePlayer", pLayerTag, &tPlayerStat, (void**)&pObj)))
		MSG_FAIL("CLevel_GamePlay Ready_Layer_Player - Add_GameObjectToLayer PrototypePlayer");
	plist.emplace_back(pObj);

	Ready_UI(plist);

	for (int i = 0; i < 3; ++i)
	{
		CGameObject* pGameObj = pInstance->Get_GameObjPtr(LEVEL_STATIC, pTagLayerPlayer, i);
		pGameObj->Set_Pos(_vec3(63.5f, 2.5f, 10.5f), dynamic_cast<CTransform*>(pGameObj->Get_Component(pTagComTransform)));
	}

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CLoadingDatFile);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	CRain::DESC tDesc;
	tDesc.tRainDropDesc.fDegree = -60.f;
	OBJ_INFO* pTemp = new OBJ_INFO;
	lstrcpy(pTemp->szTextureName,pTagPrototypeTextureRainDrop);
	tDesc.tRainDropDesc.pObjInfo = pTemp;
	if (FAILED(pInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pTagPrototypeRain, pLayerTag, &tDesc)))
		MSG_FAIL("CLevel_GamePlay Ready_Layer_Effect - Add_GameObjectToLayer PrototypeBossSkeletonMagicCircle");

	// 포탈 씬 체인지 용
	CPortal::DESC tOuterPortal;
	tOuterPortal.eNextLevel = LEVEL::LEVEL_BOSS_NECROMENCER;
	tOuterPortal.vPos = _vec3(75.f, 1.5f, 147.f);
	lstrcpy(tOuterPortal.szBufferName, L"PrototypeVIBufferCubeColorA");
	tOuterPortal.vScale = _vec3(2.f, 2.f, 2.f);
	tOuterPortal.vTrans = _vec3(0.f, 0.f, 0.f);
	if (FAILED(pInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypePortal, L"LayerPortal", &tOuterPortal)))
		MSG_FAIL("CLevel_GamePlay Ready_Layer_Backgroud - Add_GameObjectToLayer pTagPrototypePortal");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Crosshair(const _tchar * pLayerTag)
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	CCrosshair::DESC tDesc;
	lstrcpy(tDesc.tObjInfo.szTextureName, pTagPrototypeTextureCrosshair);
	tDesc.tObjInfo.SetUp_State(OBJ_INFO::STATE_POS, _vec3(g_iWinCX * 0.5f, g_iWinCY * 0.5f, 0.f));
	tDesc.tObjInfo.SetUp_Scale(1.f, 1.f, 1.f);
	if (FAILED(pInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeCrosshair, pLayerTag, &tDesc)))
		MSG_FAIL("CLevel_GamePlay Ready_Layer_Effect - Add_GameObjectToLayer PrototypeBossSkeletonMagicCircle");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_UI(list<class CPlayer*>& plist)
{
	// 플레이어 리스트 받아오기
	list<CPlayer*>& plistPlayer = plist;
	// 이터레이터 받아오기
	auto& iter = plistPlayer.begin();
	// 스킬 아이콘 배치 회수
	_uint iCount = 0;

	CLoadingDatFile* pLoadingDatFile = GET_INSTANCE(CLoadingDatFile);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	wstring wstrSceneName = TEXT("UI");
	CSubject* pSub = nullptr;
	CObserver* pObserver = nullptr;

	//태그 게이지에 쓸 정보(위치) 저장해둘 배열
	OBJ_INFO	tObjInfoArr[3];
	_uint		iObjInfoArrCount = 0;

	size_t iIndex = pLoadingDatFile->Get_VectorSize(wstrSceneName);
	for (size_t i = 0; i < iIndex; ++i)
	{
		OBJ_INFO tObjInfo = *pLoadingDatFile->Get_ObjInfo(wstrSceneName, (_uint)i);

		//이름이 같으면 생성
		if (lstrcmp(tObjInfo.szObjName, TEXT("UI0")) == 0)
		{
			// 사용할 서브젝트 받아오기
			pSub = (CSubject*)(*iter)->Get_Component(TEXT("ComStatSubject"));
			// 불러온 정보로 UI생성 후 pObserver로 받아오기
			pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("PrototypePlayerInfo"), TEXT("UI"), &tObjInfo, (void**)&pObserver);
			// 서브젝트에 구독
			pSub->Subscribe(pObserver);

			tObjInfoArr[iObjInfoArrCount++] = tObjInfo;
		}
		else if (lstrcmp(tObjInfo.szObjName, TEXT("UI1")) == 0)
		{
			// 사용할 서브젝트 받아오기
			pSub = (CSubject*)(*iter)->Get_Component(TEXT("ComStatSubject"));
			// 불러온 정보로 UI생성 후 pObserver로 받아오기
			pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("PrototypeHpBar"), TEXT("UI"), &tObjInfo, (void**)&pObserver);
			// 서브젝트에 구독
			pSub->Subscribe(pObserver);
		}
		else if (lstrcmp(tObjInfo.szObjName, TEXT("UI2")) == 0)
		{
			// 사용할 서브젝트 받아오기
			pSub = (CSubject*)(*iter)->Get_Component(TEXT("ComStatSubject"));
			// 불러온 정보로 UI생성 후 pObserver로 받아오기
			pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("PrototypeMpBar"), TEXT("UI"), &tObjInfo, (void**)&pObserver);
			// 서브젝트에 구독
			pSub->Subscribe(pObserver);
		}
		else if (lstrcmp(tObjInfo.szObjName, TEXT("UI3")) == 0)
		{
			// 사용할 서브젝트 받아오기
			pSub = (CSubject*)(*iter)->Get_Component(TEXT("ComSkillSubject"));
			// 불러온 정보로 UI생성 후 pObserver로 받아오기
			pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("PrototypeSkillIcon"), TEXT("UI"), &tObjInfo, (void**)&pObserver);
			// 서브젝트에 구독
			pSub->Subscribe(pObserver);
			// 스킬 배치 회수 증가
			++iCount;
			// 3번째 스킬 배치시 이터레이터 변경
			if (3 <= iCount)
			{
				iCount = 0;
				advance(iter, 1);
			}
		}
	}

	//위에서 담아둔 정보를 토대로 태그 게이지 생성
	CPlayer::TYPE eTypeArr[CPlayer::TYPE_END] = {CPlayer::TYPE_WARRIOR, CPlayer::TYPE_MAGE, CPlayer::TYPE_THIEF};
	_uint iTypeArrIndex = 0;
	for (auto& tObjInfoTagGauge : tObjInfoArr)
	{
		//태그 게이지는 상태창 보다 위치가 -85, 15여야함
		_vec3 vNonControlPos = tObjInfoTagGauge.Get_State(OBJ_INFO::STATE_POS);
		_vec3 vTagGaugePos = tObjInfoTagGauge.Get_State(OBJ_INFO::STATE_POS);
		vTagGaugePos += _vec3(-85.f, 15.f, 0.f);

		//태그 게이지 생성
		CTagGauge::DESC tTagGaugeDesc;
		tTagGaugeDesc.vPos = vTagGaugePos;
		tTagGaugeDesc.vScale = _vec3(70.f, 70.f, 1.f);
		tTagGaugeDesc.eType = eTypeArr[iTypeArrIndex];
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeTagGauge, pTagLayerUI, &tTagGaugeDesc)))
			MSG_FAIL("CLevel_GamePlay::Ready_UI - pTagPrototypeUITagGauge");

		//논컨트롤 생성
		CNonControl::DESC tNonControlDesc;
		tNonControlDesc.vPos = vNonControlPos;
		tNonControlDesc.vScale = tObjInfoTagGauge.Get_Scale();
		tNonControlDesc.eType = eTypeArr[iTypeArrIndex++];
		if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeNonControl, pTagLayerUI, &tNonControlDesc)))
			MSG_FAIL("CLevel_GamePlay::Ready_UI - pTagPrototypeUINonControl");
	}

	for (auto& pPlayer : plistPlayer)
		pPlayer->Ready_Observers();

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CLoadingDatFile);

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iLevelID)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(iLevelID)))
	{
		MSGBOX("Failed To Creating CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}
