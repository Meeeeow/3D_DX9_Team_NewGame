#include "stdafx.h"
#include "..\public\Level_Test.h"
#include "GameInstance.h"
#include "VIBufferTerrain.h"
#include "CameraFree.h"
#include "Player.h"
#include "LoadingDatFile.h"

CLevelTest::CLevelTest(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CLevel(pGraphic_Device)
{
}

HRESULT CLevelTest::NativeConstruct(_uint iLevelID)
{
	if (FAILED(__super::NativeConstruct(iLevelID)))
		return E_FAIL;

	CLoadingDatFile* pLoadingDatFile = GET_INSTANCE(CLoadingDatFile);
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	wstring wstrSceneName = L"Test";

	const _tchar* pLayerTerrain = L"LayerTerrain";
	const _tchar* pLayerCube = L"LayerCube";

	size_t iIndex = pLoadingDatFile->Get_VectorSize(wstrSceneName);
	for (size_t i = 0; i < iIndex; ++i)
	{
		OBJ_INFO tObjInfo = *pLoadingDatFile->Get_ObjInfo(wstrSceneName, (_uint)i);

		//이름이 같으면 생성
		if (lstrcmp(tObjInfo.szObjName, L"Terrain") == 0)
		{
			pGameInstance->Add_GameObjectToLayer(LEVEL_TEST, L"PrototypeTestTerrain", pLayerTerrain, &tObjInfo);
		}
		else if (lstrcmp(tObjInfo.szObjName, L"Ex3") == 0)
		{
			pGameInstance->Add_GameObjectToLayer(LEVEL_TEST, L"PrototypeTestEx3", pLayerCube, &tObjInfo);
		}
	}

	RELEASE_INSTANCE(CLoadingDatFile);
	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

_int CLevelTest::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	return 0;
}

HRESULT CLevelTest::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevelTest::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevelTest::Ready_Layer_Cube(const _tchar * pLayerTag)
{
	return E_NOTIMPL;
}

CLevelTest * CLevelTest::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iLevelID)
{
	CLevelTest* pInstance = new CLevelTest(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(iLevelID)))
	{
		MSGBOX("Failed To Creating CLevelTest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevelTest::Free()
{
	__super::Free();
}
