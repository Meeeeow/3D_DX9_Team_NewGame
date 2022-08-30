#include "stdafx.h"
#include "..\public\DialogMgr.h"
#include "BlackScreen.h"

IMPLEMENT_SINGLETON(CDialogMgr)

CDialogMgr::CDialogMgr()
{
}

CDialogMgr::~CDialogMgr()
{
}

HRESULT CDialogMgr::Add_Dialog(CDialog * pDialog)
{
	if (pDialog == nullptr)
		MSG_FAIL("CDialogMgr::Add_Dialog - pDialog == nullptr");

	m_listDialog.emplace_back(pDialog);
	Safe_AddRef(pDialog);

	return S_OK;
}

HRESULT CDialogMgr::SetUp_Dialog(_bool bNotTick, CDialog::DESC tDesc)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Set_NotTick(bNotTick);

	//주어진 폰트로 글씨 만들고 리스트에 삽입(엔터 키 누를때 한번에 삭제하기 위함)
	CDialog* pDialog;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pTagPrototypeDialog, pTagLayerNonCollision, &tDesc, (void **)&pDialog)))
		MSG_FAIL("CDialogMgr::SetUp_Dialog - pTagPrototypeDialog");

	if (FAILED(Add_Dialog(pDialog)))
		MSG_FAIL("CDialogMgr::SetUp_Dialog - Add_Dialog");

	CBlackScreen::DESC tBlackScreenDesc;
	tBlackScreenDesc.iTextureNum = 7;
	pGameInstance->Add_GameObjectToLayer(LEVEL_GAMEPLAY, pTagPrototypeBlackScreen, pTagLayerNonCollision, &tBlackScreenDesc);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CDialogMgr::Draw_Font(const _tchar * pText, D3DXCOLOR FontColor, _vec3* vPos /*= nullptr*/, _vec3 vScale /*= _vec3(0.5f, 1.f, 1.f)*/)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Draw_Font(pText, FontColor, vPos, vScale);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

_int CDialogMgr::Tick(_float fTimeDelta)
{
	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	//if (pGameInstance->Key_Down(DIK_RETURN))
	//{
	//	pGameInstance->Set_NotTick(false);
	//	Free();
	//}
	//RELEASE_INSTANCE(CGameInstance);

	return _int();
}

void CDialogMgr::Free()
{
	for (auto& pDialog : m_listDialog)
	{
		pDialog->Set_Dead();
		Safe_Release(pDialog);
		pDialog = nullptr;
	}
	m_listDialog.clear();
}
