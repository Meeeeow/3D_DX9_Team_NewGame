#include "stdafx.h"
#include "..\public\Level_Logo.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "BackGround.h"

CLevel_Logo::CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Logo::NativeConstruct(_uint iLevelID)
{
	if (FAILED(__super::NativeConstruct(iLevelID)))
		return E_FAIL;

	if (FAILED(Ready_Prototype_GameObject()))
		MSG_FAIL("CLevel_Logo NativeConstruct - Ready_Prototype_GameObject");

	if (FAILED(Ready_Layer_Backgroud(TEXT("LayerBackGround"))))
		MSG_FAIL("CLevel_Logo NativeConstruct - Ready_Layer_Backgroud");

	return S_OK;
}

_int CLevel_Logo::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

		CLevel*		pLevel = CLevel_Loading::Create(m_pGraphic_Device, LEVEL_GAMEPLAY, LEVEL_LOADING);
		if (nullptr == pLevel)
			return -1;

		if (FAILED(pGameInstance->Change_Level(pLevel)))
			return -1;

		RELEASE_INSTANCE(CGameInstance);

		return 0;
	}

	return 0;
}

HRESULT CLevel_Logo::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Prototype_GameObject()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if(FAILED(pGameInstance->Add_Prototype(TEXT("PrototypeBackGround"), CBackGround::Create(m_pGraphic_Device))))
		MSG_FAIL("CLevel_Logo Ready_Prototype_GameObject - Add_Prototype");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Backgroud(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_LOGO, TEXT("PrototypeBackGround"), pLayerTag)))
		MSG_FAIL("CLevel_Logo Ready_Layer_Backgroud - Add_GameObjectToLayer");

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iLevelID)
{
	CLevel_Logo* pInstance = new CLevel_Logo(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(iLevelID)))
	{
		MSGBOX("Failed To Creating CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}
