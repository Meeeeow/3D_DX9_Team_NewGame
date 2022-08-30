// UITool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "UITool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "GameInstance.h"
#include "ToolObj.h"
#include "UIObj.h"


// CUITool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUITool, CDialog)

CUITool::CUITool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UITOOL, pParent)
	, m_fPosX(0)
	, m_fPosY(0)
	, m_fScaleX(0)
	, m_fScaleY(0)
{

}

CUITool::~CUITool()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDevice);

	for (auto& pObj : m_vectorPlacedUI)
		Safe_Release(pObj);
	m_vectorPlacedUI.clear();
	m_vectorPlacedUI.shrink_to_fit();

	for (auto& pObj : m_vectorPreviewUI)
		Safe_Release(pObj);
	m_vectorPreviewUI.clear();
	m_vectorPreviewUI.shrink_to_fit();
}

HRESULT CUITool::Render_UITool()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->Render_Begin();

	pGameInstance->Tick_ToolEngine(0.1f);
	_mat matProj;
	D3DXMatrixIdentity(&matProj);
	m_pDevice->SetTransform(D3DTS_VIEW, &matProj);

	D3DXMatrixOrthoLH(&matProj, (FLOAT)m_dwPicturePlacedX, (FLOAT)m_dwPicturePlacedY, 0.f, 1.f);

	m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	for (auto& pUIObj : m_vectorPlacedUI)
		pUIObj->Tick(0.1f);

	for (auto& pUIObj : m_vectorPlacedUI)
		pUIObj->LateTick(0.1f);

	m_pRenderer->Draw_RenderGroup();

	pGameInstance->Render_End(m_pictureUITool.m_hWnd);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CUITool::Render_UIToolPreview()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Render_Begin();

	pGameInstance->Tick_ToolEngine(0.1f);
	_mat matProj;
	D3DXMatrixIdentity(&matProj);
	m_pDevice->SetTransform(D3DTS_VIEW, &matProj);

	D3DXMatrixOrthoLH(&matProj, (FLOAT)m_dwPicturePreviewX, (FLOAT)m_dwPicturePreviewY, 0.f, 1.f);

	m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	int iIndex = m_listBoxPreviewUI.GetCurSel();
	CTransform* pTransform = dynamic_cast<CTransform*>(m_vectorPreviewUI[iIndex]->Get_Component(pTagComTransform));
	pTransform->SetUp_Scale(_float(m_dwPicturePreviewX), _float(m_dwPicturePreviewY), 1.f);
	pTransform->SetUp_State(CTransform::STATE_POS, _vec3(_float(m_dwPicturePreviewX), _float(m_dwPicturePreviewY), 0.f));
	m_vectorPreviewUI[iIndex]->Tick(0.1f);
	m_vectorPreviewUI[iIndex]->LateTick(0.1f);

	m_pRenderer->Draw_RenderGroup();

	pGameInstance->Render_End(m_pictureUIToolPreview.m_hWnd);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CUITool::Update_ObjState()
{
	UpdateData(TRUE);

	int iIndex = m_listBoxPlacedUI.GetCurSel();
	if (iIndex < 0)
		MSG_FAIL("CUITool::Update_ObjState - iIndex < 0");

	CTransform* pTransform = dynamic_cast<CTransform*>(m_vectorPlacedUI[iIndex]->Get_Component(pTagComTransform));
	if (pTransform == nullptr)
		MSG_FAIL("CUITool::Update_ObjState - pTransform == nullptr");

	_vec3 vPos = _vec3(m_fPosX, m_fPosY, 0.f);

	pTransform->SetUp_Scale(m_fScaleX, m_fScaleY, 0.f);
	pTransform->SetUp_State(CTransform::STATE_POS, vPos);

	if(FAILED(Render_UITool()))
		MSG_FAIL("CUITool::Update_ObjState - Render_UITool");

	UpdateData(FALSE);
	return S_OK;
}

HRESULT CUITool::Update_EditState(CTransform* pTransform)
{
	if (pTransform == nullptr)
		MSG_FAIL("CUITool::Update_EditState - pTransform == nullptr");

	UpdateData(TRUE);

	_vec3 vPos = pTransform->Get_State(CTransform::STATE_POS);
	_vec3 vScale = pTransform->Get_Scale();

	m_fPosX = vPos.x;
	m_fPosY = vPos.y;
	m_fScaleX = vScale.x;
	m_fScaleY = vScale.y;
	
	UpdateData(FALSE);
	return S_OK;
}

HRESULT CUITool::Add_ListBoxPlacedUIString(CGameObject * pGameObj)
{
	if (pGameObj == nullptr)
		MSG_FAIL("CUITool::Add_ListBoxPlacedUIString - pGameObj == nullptr");
	CUIObj* pUIObj = dynamic_cast<CUIObj*>(pGameObj);
	if (pUIObj == nullptr)
		MSG_FAIL("CUITool::Add_ListBoxPlacedUIString - pUIObj == nullptr");

	m_listBoxPlacedUI.AddString(pUIObj->Get_ObjInfo()->szObjName);
	m_vectorPlacedUI.emplace_back(pUIObj);
	
	return S_OK;
}

HRESULT CUITool::Add_ListBoxPrevieUIString(CUIObj * pUIObj)
{
	if (pUIObj == nullptr)
		MSG_FAIL("CUITool::Add_ListBoxPrevieUIString - pUIObj == nullptr");

	m_listBoxPreviewUI.AddString(pUIObj->Get_ObjInfo()->szObjName);
	m_vectorPreviewUI.emplace_back(pUIObj);

	return S_OK;
}

HRESULT CUITool::Load_Texture()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, L"PrototypeTextureUI", CTexture::Create(m_pDevice,
		L"../Client/bin/Resource/Texture/UI/UI%d.png", 5))))
		MSG_FAIL("CUITool::Load_Texture - pGameInstance->Add_Prototype");
	RELEASE_INSTANCE(CGameInstance);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	if (pMain == nullptr)
		MSG_FAIL("CUITool::Load_Texture - pMain == nullptr");
	CToolView* pToolView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	if (pToolView == nullptr)
		MSG_FAIL("CUITool::Load_Texture - pToolView == nullptr");

	for (int i = 0; i < 3; ++i)
	{
		OBJ_INFO* pObjInfo = new OBJ_INFO;
		pObjInfo->eVIBufferType = OBJ_INFO::VI_BUFFER_TYPE_UI;
		_tchar szBuf[16];
		_itow_s(i, szBuf, 10);
		lstrcpy(pObjInfo->szObjName, szBuf);
		pObjInfo->iTextureIndex = i;
		D3DXMatrixIdentity(&pObjInfo->matWorld);
		pObjInfo->SetUp_State(OBJ_INFO::STATE_POS, _vec3(0.f, 0.f, 0.f));
		pObjInfo->SetUp_Scale(_float(m_dwPicturePreviewX), _float(m_dwPicturePreviewY), 0.f);
		CUIObj* pUIObj = dynamic_cast<CUIObj*>(pToolView->Create_ToolObj(pObjInfo));
		if (pUIObj == nullptr)
			MSG_FAIL("CUITool::Load_Texture - pUIObj == nullptr");

		Add_ListBoxPrevieUIString(pUIObj);
	}



	return S_OK;
}

void CUITool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UITOOL_SCREEN, m_pictureUITool);
	DDX_Control(pDX, IDC_LIST_UI, m_listBoxPreviewUI);
	DDX_Control(pDX, IDC_LIST_UITOOL_PLACED_UI, m_listBoxPlacedUI);
	DDX_Control(pDX, IDC_PICTURE_UI_TOOL_PREVIEW, m_pictureUIToolPreview);

	DDX_Text(pDX, IDC_EDIT_UITOOL_POS_X, m_fPosX);
	DDX_Text(pDX, IDC_EDIT_UITOOL_POS_Y, m_fPosY);
	DDX_Text(pDX, IDC_EDIT_UITOOL_SCALE_X, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT_UITOOL_SCALE_Y, m_fScaleY);
}


BEGIN_MESSAGE_MAP(CUITool, CDialog)
	ON_EN_CHANGE(IDC_EDIT_UITOOL_POS_X, &CUITool::OnEnChangeEditUitoolPosX)
	ON_EN_CHANGE(IDC_EDIT_UITOOL_POS_Y, &CUITool::OnEnChangeEditUitoolPosY)
	ON_EN_CHANGE(IDC_EDIT_UITOOL_SCALE_X, &CUITool::OnEnChangeEditUitoolScaleX)
	ON_EN_CHANGE(IDC_EDIT_UITOOL_SCALE_Y, &CUITool::OnEnChangeEditUitoolScaleY)
	ON_BN_CLICKED(IDC_BUTTON_UITOOL_LOAD_UI, &CUITool::OnBnClickedButtonUitoolLoadUi)
	ON_BN_CLICKED(IDC_BUTTON_UITOOL_DELETE_IN_SCENE, &CUITool::OnBnClickedButtonUitoolDeleteInScene)
	ON_BN_CLICKED(IDC_BUTTON_UITOOL_SAVE_UI2, &CUITool::OnBnClickedButtonUitoolSaveUi)
	ON_LBN_SELCHANGE(IDC_LIST_UI, &CUITool::OnLbnSelchangeListUi)
	ON_BN_CLICKED(IDC_BUTTON_UITOOL_ADD_TO_SCENE, &CUITool::OnBnClickedButtonUitoolAddToScene)
	ON_LBN_SELCHANGE(IDC_LIST_UITOOL_PLACED_UI, &CUITool::OnLbnSelchangeListUitoolPlacedUi)
END_MESSAGE_MAP()


// CUITool 메시지 처리기입니다.


BOOL CUITool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	RECT rc;
	m_pictureUIToolPreview.GetClientRect(&rc);
	m_dwPicturePreviewX = rc.right;
	m_dwPicturePreviewY = rc.bottom;

	m_pictureUITool.GetClientRect(&rc);
	m_dwPicturePlacedX = rc.right;
	m_dwPicturePlacedY = rc.bottom;

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	if (pMain == nullptr)
		MSGBOX("CUITool::OnInitDialog - pMain == nullptr");
	CToolView* pToolView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	if (pToolView == nullptr)
		MSGBOX("CUITool::OnInitDialog - pToolView == nullptr");

	m_pDevice = pToolView->Clone_Device();
	if (m_pDevice == nullptr)
		MSGBOX("CUITool::OnInitDialog - m_pDevice == nullptr");

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	m_pRenderer = dynamic_cast<CRenderer*>(pGameInstance->Clone_Component(LEVEL_STATIC, pTagPrototypeRenderer));
	if (m_pRenderer == nullptr)
		MSGBOX("CUITool::OnInitDialog - m_pRenderer == nullptr");

	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(Load_Texture()))
		MSGBOX("CUITool::OnInitDialog - Load_Texture");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CUITool::OnEnChangeEditUitoolPosX()
{
	Update_ObjState();

}


void CUITool::OnEnChangeEditUitoolPosY()
{
	Update_ObjState();
}


void CUITool::OnEnChangeEditUitoolScaleX()
{

	Update_ObjState();

}


void CUITool::OnEnChangeEditUitoolScaleY()
{
	Update_ObjState();

}


void CUITool::OnBnClickedButtonUitoolLoadUi()
{
	CFileDialog Dlg(TRUE, L"dat", L"*.dat");
	TCHAR szBuf[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Client\\bin\\Resource\\Data\\UI\\");
	Dlg.m_ofn.lpstrInitialDir = szBuf;

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	if (pMain == nullptr)
		MSG_VOID("CForm::OnBnClicked_LoadScene - pMain == nullptr");
	CToolView* pToolView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	if (pToolView == nullptr)
		MSG_VOID("CForm::OnBnClicked_LoadScene - pToolView == nullptr");

	if (IDOK == Dlg.DoModal())
	{
		CString wstrPath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrPath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		OBJ_INFO* pInfo = nullptr;
		pInfo = new OBJ_INFO;
		_ulong dwNum;
		while (true)
		{
			ReadFile(hFile, pInfo, sizeof(OBJ_INFO), &dwNum, nullptr);

			if (dwNum <= 0)
				Safe_Delete(pInfo);
			else
			{
				CGameObject* pGameObj = pToolView->Create_ToolObj(pInfo);
				Add_ListBoxPlacedUIString(pGameObj);
			}
		}

		CloseHandle(hFile);
	}
}


void CUITool::OnBnClickedButtonUitoolDeleteInScene()
{
	int iIndex = m_listBoxPlacedUI.GetCurSel();
	if (iIndex < 0)
		MSG_VOID("OnBnClickedButtonUitoolDeleteInScene - iIndex < 0");

	m_listBoxPlacedUI.DeleteString(iIndex);
	m_vectorPlacedUI.erase(m_vectorPlacedUI.begin() + iIndex);
}


void CUITool::OnBnClickedButtonUitoolSaveUi()
{
	CFileDialog Dlg(FALSE, L"dat", L"*.dat");
	TCHAR szBuf[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Client\\bin\\Resource\\Data\\UI\\");
	Dlg.m_ofn.lpstrInitialDir = szBuf;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrPath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrPath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		_ulong dwNum;
		OBJ_INFO tInfo;

		for (auto& pObj : m_vectorPlacedUI)
		{
			tInfo = *pObj->Get_ObjInfo();
			WriteFile(hFile, &tInfo, sizeof(OBJ_INFO), &dwNum, nullptr);
		}

		CloseHandle(hFile);
	}
}


void CUITool::OnLbnSelchangeListUi()
{
	Render_UIToolPreview();
}


void CUITool::OnBnClickedButtonUitoolAddToScene()
{
	//프리뷰에 있는 오브젝트 크리에이트해서 넘겨줌
	int iIndex = m_listBoxPreviewUI.GetCurSel();
	if (iIndex < 0)
		MSG_VOID("CUITool::OnBnClickedButtonUitoolAddToScene - iIndex < 0");

	//툴뷰 함수 쓰기위해 받아옴
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	if (pMain == nullptr)
		MSG_VOID("CUITool::OnBnClickedButtonUitoolAddToScene - pMain == nullptr");
	CToolView* pToolView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	if (pToolView == nullptr)
		MSG_VOID("CUITool::OnBnClickedButtonUitoolAddToScene - pToolView == nullptr");

	//넘겨줄 오브젝트 생성
	OBJ_INFO* pObjInfo = new OBJ_INFO;
	memcpy(pObjInfo, m_vectorPreviewUI[iIndex]->Get_ObjInfo(), sizeof(OBJ_INFO));
	CUIObj* pUIObj = dynamic_cast<CUIObj*>(pToolView->Create_ToolObj(pObjInfo));
	if (pUIObj == nullptr)
		MSG_VOID("CUITool::OnBnClickedButtonUitoolAddToScene - pUIObj == nullptr");

	//넘겨줌
	Add_ListBoxPlacedUIString(pUIObj);
}


void CUITool::OnLbnSelchangeListUitoolPlacedUi()
{
	_int iIndex = m_listBoxPlacedUI.GetCurSel();
	if (iIndex < 0)
		MSG_VOID("CUITool::OnLbnSelchangeListUitoolPlacedUi - iIndex < 0");

	if (FAILED(Update_EditState(dynamic_cast<CTransform*>(m_vectorPlacedUI[iIndex]->Get_Component(pTagComTransform)))))
		MSG_VOID("CUITool::OnLbnSelchangeListUitoolPlacedUi - Update_EditState");
}
