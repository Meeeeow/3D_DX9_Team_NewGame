#include "stdafx.h"
#include "..\public\Level_Loading.h"
#include "Loader.h"
#include "GameInstance.h"
#include "Level_GamePlay.h"
#include "Level_Test.h"
#include "Level_BossSkeleton.h"
#include "LevelCircus.h"
#include "LevelNecromencer.h"


CLevel_Loading::CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CLevel(pGraphic_Device)
{
}

HRESULT CLevel_Loading::NativeConstruct(LEVEL eNextLevel, _uint iLevelID)
{
	if (FAILED(__super::NativeConstruct(iLevelID)))
		return E_FAIL;

	m_eLevel = eNextLevel;
	m_pLoader = CLoader::Create(m_pGraphic_Device, eNextLevel);
	if (m_pLoader == nullptr)
		return E_FAIL;

	if (FAILED(SetUp_Component()))
		MSG_FAIL("CLevel_Loading::NativeConstruct - SetUp_Component");

	m_tFrame.Set_Value(16);

	return S_OK;
}

_int CLevel_Loading::Tick(_float fTimeDelta)
{
	if (0 > __super::Tick(fTimeDelta))
		return -1;

	if (m_pLoader == nullptr)
		return -1;

	m_tFrame.fTime += fTimeDelta;
	if (m_tFrame.fTime >= m_tFrame.fDelay)
	{
		m_tFrame.iStart++;
		m_tFrame.fTime = 0.f;

		if (m_tFrame.iStart >= m_tFrame.iEnd)
			m_tFrame.iStart = 0;
	}

	if (m_pLoader->Get_Finished())
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		/*if (pGameInstance->Key_Down(DIK_SPACE))
		{*/
			CLevel* pLevel = nullptr;

			pGameInstance->Clear_CollisionTag();
			switch (m_eLevel)
			{
			case LEVEL_GAMEPLAY:
				pLevel = CLevel_GamePlay::Create(m_pGraphic_Device, LEVEL_GAMEPLAY);
				break;
			case LEVEL_TEST:
				pLevel = CLevelTest::Create(m_pGraphic_Device, LEVEL_TEST);
				break;
			case LEVEL_BOSS_SKELETON:
				pLevel = CLevel_BossSkeleton::Create(m_pGraphic_Device, LEVEL_BOSS_SKELETON);
				break;
			case LEVEL_BOSS_CIRCUS:
				pLevel = CLevelCircus::Create(m_pGraphic_Device, LEVEL_BOSS_CIRCUS);
				break;
			case LEVEL_BOSS_NECROMENCER:
				pLevel = CLevelNecromencer::Create(m_pGraphic_Device, LEVEL_BOSS_NECROMENCER);
				break;
			}

			if (pLevel == nullptr)
				return -1;

			if (FAILED(pGameInstance->Change_Level(pLevel)))
				return -1;

		//}
		RELEASE_INSTANCE(CGameInstance);
	}

	return 0;
}

HRESULT CLevel_Loading::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (m_pTransformBlack == nullptr ||
		m_pTransformLoading == nullptr ||
		m_pTextureBlack == nullptr ||
		m_pTextureLoading == nullptr ||
		m_pVIBufferBlack == nullptr ||
		m_pVIBufferLoading == nullptr)
		MSG_FAIL("CLevel_Loading::Render - com is null");

	if (FAILED(SetUp_RenderState()))
		MSG_FAIL("CLevel_Loading::Render - SetUp_RenderState")

	//검은 화면
	if (FAILED(m_pTransformBlack->Bind_OnDevice()))
		MSG_FAIL("CLevel_Loading::Render - m_pTransformBlack->Bind_OnDevice");

	if (FAILED(m_pTextureBlack->Bind_OnDevice(0)))
		MSG_FAIL("CLevel_Loading::Render - m_pTextureBlack->Bind_OnDevice");

	if (FAILED(m_pVIBufferBlack->Render()))
		MSG_FAIL("CLevel_Loading::Render - m_pVIBufferBlack->Render");

	//로딩 중
	if (FAILED(m_pTransformLoading->Bind_OnDevice()))
		MSG_FAIL("CLevel_Loading::Render - m_pTransformLoading->Bind_OnDevice");

	if (FAILED(m_pTextureLoading->Bind_OnDevice(m_tFrame.iStart)))
		MSG_FAIL("CLevel_Loading::Render - m_pTextureLoading->Bind_OnDevice");



	if (FAILED(m_pVIBufferLoading->Render()))
		MSG_FAIL("CLevel_Loading::Render - m_pVIBufferLoading->Render");

	if (FAILED(Release_RenderState()))
		MSG_FAIL("CLevel_Loading::Render - Release_RenderState")

	return S_OK;
}

HRESULT CLevel_Loading::SetUp_RenderState()
{
	if (m_pGraphic_Device == nullptr)
		MSG_FAIL("CLevel_Loading::SetUp_RenderState - m_pGraphic_Device == nullptr");

	_mat tmp;
	D3DXMatrixIdentity(&tmp);

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, &tmp);
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_matProject);

	m_pGraphic_Device->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);

	return S_OK;
}

HRESULT CLevel_Loading::Release_RenderState()
{
	if (m_pGraphic_Device == nullptr)
		MSG_FAIL("CLevel_Loading::Release_RenderState - m_pGraphic_Device == nullptr");

	m_pGraphic_Device->SetRenderState(D3DRENDERSTATETYPE::D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_CCW);

	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);

	return S_OK;
}

HRESULT CLevel_Loading::SetUp_Component()
{
	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, (CComponent**)&m_pTransformBlack)))
		MSG_FAIL("CLevel_Loading::SetUp_Component - Add_Component pTagPrototypeTransform");
	
	D3DXMatrixOrthoLH(&m_matProject, (float)g_iWinCX, (float)g_iWinCY, 0.f, 1.f);
	_float fSizeX = (float)g_iWinCX;
	_float fSizeY = (float)g_iWinCY;
	_float fX = float(g_iWinCX >> 1);
	_float fY = float(g_iWinCY >> 1);

	m_pTransformBlack->SetUp_State(CTransform::STATE::STATE_POS, _vec3(fX - (fSizeX * 0.5f), -fY + (fSizeY * 0.5f), 0.f));
	m_pTransformBlack->SetUp_Scale(fSizeX, fSizeY, 1.f);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTransform, (CComponent**)&m_pTransformLoading)))
		MSG_FAIL("CLevel_Loading::SetUp_Component - Add_Component pTagPrototypeTransform");

	fX = _float(g_iWinCX) - 100.f;
	fY = _float(g_iWinCY) - 100.f;
	m_pTransformLoading->SetUp_State(CTransform::STATE::STATE_POS, _vec3(fX - (fSizeX * 0.5f), -fY + (fSizeY * 0.5f), 0.f));
	m_pTransformLoading->SetUp_Scale(100.f, 100.f, 1.f);

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, (CComponent**)&m_pVIBufferBlack)))
		MSG_FAIL("CLevel_Loading::SetUp_Component - Add_Component pTagPrototypeVIBufferRect");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeVIBufferRect, (CComponent**)&m_pVIBufferLoading)))
		MSG_FAIL("CLevel_Loading::SetUp_Component - Add_Component pTagPrototypeVIBufferRect");


	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTextureBlack, (CComponent**)&m_pTextureBlack)))
		MSG_FAIL("CLevel_Loading::SetUp_Component - Add_Component pTagPrototypeTextureBlack");

	if (FAILED(Add_Component(LEVEL_STATIC, pTagPrototypeTextureLoading, (CComponent**)&m_pTextureLoading)))
		MSG_FAIL("CLevel_Loading::SetUp_Component - Add_Component pTagPrototypeTextureLoading");


	return S_OK;
}

HRESULT CLevel_Loading::Add_Component(LEVEL eLevel, const _tchar * pTagPrototype, CComponent ** ppComponent, void * pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	*ppComponent = pGameInstance->Clone_Component(eLevel, pTagPrototype, pArg);
	if (*ppComponent == nullptr)
		MSG_FAIL("CLevel_Loading::Add_Component - *ppComponent == nullptr");

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel, _uint iLevelID)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pGraphic_Device);

	if (FAILED(pInstance->NativeConstruct(eNextLevel, iLevelID)))
	{
		Safe_Release(pInstance);
		MSGBOX("Fail to Create Level_Loading");
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
	Safe_Release(m_pTextureBlack);
	Safe_Release(m_pTextureLoading);
	Safe_Release(m_pTransformBlack);
	Safe_Release(m_pTransformLoading);
	Safe_Release(m_pVIBufferBlack);
	Safe_Release(m_pVIBufferLoading);
}
