// Form.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Form.h"
#include "FilePath.h"
#include "Transform.h"
#include "GameObject.h"
#include "RectObj.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "ToolObj.h"
#include "ToolTerrainObj.h"
#include "RectYZObj.h"
#include "ToolViewCamera.h"

// CForm

IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_FORM)
	, m_wstrObjName(_T(""))
	, m_fObjPosX(0)
	, m_fObjPosY(0)
	, m_fObjPosZ(0)
	, m_fObjRotX(0)
	, m_fObjRotY(0)
	, m_fObjRotZ(0)
	, m_fObjScaleX(0)
	, m_fObjScaleY(0)
	, m_fObjScaleZ(0)
	, m_wstrSceneName(_T(""))
	, m_fEditCamPosX(0)
	, m_fEditCamPosY(0)
	, m_fEditCamPosZ(0)
	, m_wstrBackCurlingOnOff(_T("On"))
{
}

CForm::~CForm()
{
	Free();
}

void CForm::Set_ObjName(CString wstrObjName)
{
	UpdateData(TRUE);

	m_wstrObjName = wstrObjName;

	UpdateData(FALSE);
}

void CForm::Set_SceneName(CString wstrSceneName)
{
	UpdateData(TRUE);

	m_wstrSceneName = wstrSceneName;

	UpdateData(FALSE);
}

void CForm::Set_ObjInfoToScreen(const OBJ_INFO* pObjInfo)
{
	UpdateData(TRUE);

	if (pObjInfo == nullptr)
		MSG_VOID("CForm::Set_ObjInfoToScreen - pObjInfo == nullptr");

	_vec3 vPos = pObjInfo->Get_State(OBJ_INFO::STATE_POS);
	_vec3 vScale = pObjInfo->Get_Scale();

	m_fObjPosX = vPos.x;
	m_fObjPosY = vPos.y;
	m_fObjPosZ = vPos.z;

	m_fObjScaleX = vScale.x;
	m_fObjScaleY = vScale.y;
	m_fObjScaleZ = vScale.z;

	m_fObjRotX = pObjInfo->vRot.x;
	m_fObjRotY = pObjInfo->vRot.y;
	m_fObjRotZ = pObjInfo->vRot.z;

	UpdateData(FALSE);
}

HRESULT CForm::Set_MapScene(CString wstrSceneName)
{
	auto& iter = find_if(m_mapScene.begin(), m_mapScene.end(), CTagFinder(wstrSceneName.GetString()));
	if (iter != m_mapScene.end())
	{
		MSGBOX("CForm Set_MapScene - iter is not unique");
		return E_FAIL;
	}

	vector<CGameObject*> vectorObj;
	m_mapScene.emplace(wstrSceneName, vectorObj);

	return S_OK;
}

HRESULT CForm::Change_SelectedObjectTransform()
{
	UpdateData(TRUE);

	int iIndex = m_listBoxSceneObj.GetCurSel();
	if (iIndex < 0)
		MSG_FAIL("CForm::Change_SelectedObjectTransform - iIndex < 0");
	
	if (iIndex >= m_vectorSceneObj.size())
		MSG_FAIL("CForm::Change_SelectedObjectTransform - iIndex >= m_vectorSceneObj.size()");

	CToolObj* pToolObj = dynamic_cast<CToolObj*>(m_vectorSceneObj[iIndex]);
	if (pToolObj == nullptr)
		MSG_FAIL("CForm::Change_SelectedObjectTransform - pToolObj == nullptr");

	OBJ_INFO* pObjInfo = new OBJ_INFO;
	memcpy(pObjInfo, pToolObj->Get_ObjInfo(), sizeof(OBJ_INFO));

	_vec3 vPos;
	vPos.x = m_fObjPosX;
	vPos.y = m_fObjPosY;
	vPos.z = m_fObjPosZ;
	pObjInfo->SetUp_State(OBJ_INFO::STATE_POS, vPos);

	pObjInfo->SetUp_Scale(m_fObjScaleX, m_fObjScaleY, m_fObjScaleZ);

	pObjInfo->vRot = _vec3(m_fObjRotX, m_fObjRotY, m_fObjRotZ);
	pObjInfo->SetUp_Rotation(_vec3(1.f, 0.f, 0.f), m_fObjRotX);
	pObjInfo->Rotation_Axis(_vec3(0.f, 1.f, 0.f), m_fObjRotY);
	pObjInfo->Rotation_Axis(_vec3(0.f, 0.f, 1.f), m_fObjRotZ);


	pToolObj->Set_ObjInfo(pObjInfo);
	pToolObj->Set_Matrix(pObjInfo->matWorld);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	if (pMain == nullptr)
		MSG_FAIL("CForm::Change_SelectedObjectTransform - pMain == nullptr");
	CToolView* pToolView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	if (pToolView == nullptr)
		MSG_FAIL("CForm::Change_SelectedObjectTransform - pToolView == nullptr");

	UpdateData(FALSE);

	pToolView->Render_ToolView();

	return S_OK;
}

HRESULT CForm::Set_CamPosToCam()
{
	UpdateData(TRUE);

	_vec3 vPos = _vec3(m_fEditCamPosX, m_fEditCamPosY, m_fEditCamPosZ);
	m_pCamera->Set_CamPos(&vPos);

	UpdateData(FALSE);
	return S_OK;
}

HRESULT CForm::Set_CamPosToScreen()
{
	UpdateData(TRUE);

	_vec3 vPos = m_pCamera->Get_CameraPos();
	m_fEditCamPosX = vPos.x;
	m_fEditCamPosY = vPos.y;
	m_fEditCamPosZ = vPos.z;

	UpdateData(FALSE);
	return S_OK;
}

void CForm::AddString_ListBoxScene(CString wstrName)
{
	UpdateData(TRUE);

	//중복 체크
	if (0 <= m_listBoxScene.FindString(-1, wstrName))
		MSG_VOID("CForm AddString_ListBoxScene - 0 <= m_listBoxScene.FindString(-1, wstrName)");

	m_listBoxScene.AddString(wstrName);

	UpdateData(FALSE);
}

void CForm::AddString_ListBoxSceneObj(const CString wstrObjName)
{
	UpdateData(TRUE);

	m_listBoxSceneObj.AddString(wstrObjName);

	UpdateData(FALSE);

}

HRESULT CForm::Add_ObjectToScene(CGameObject * pObj)
{
	auto& iter = find_if(m_mapScene.begin(), m_mapScene.end(), CTagFinder(m_wstrSceneName.GetString()));
	if (iter == m_mapScene.end())
		MSG_FAIL("선택하신 씬이 없습니다.");

	if (pObj == nullptr)
		MSG_FAIL("CForm Add_ObjectToScene - pObj is null");

	iter->second.emplace_back(pObj);
	Safe_AddRef(pObj);
	m_vectorSceneObj.emplace_back(pObj);

	return S_OK;
}

void CForm::AddString_ListBoxPrototype(CString wstrName, CGameObject* pObj)
{
	UpdateData(TRUE);
	//중복 체크
	if (0 <= m_listBoxPrototype.FindString(-1, wstrName))
		return;

	if (pObj == nullptr)
		MSG_VOID("CForm AddString_ListBoxPrototype - pObj is null");

	auto& iter = find_if(m_mapPrototype.begin(), m_mapPrototype.end(), CTagFinder(wstrName.GetString()));
	if (iter != m_mapPrototype.end())
		return;

	m_listBoxPrototype.AddString(wstrName);
	m_mapPrototype.emplace(wstrName, pObj);
	m_vectorPrototype.emplace_back(pObj);

	UpdateData(FALSE);
}

void CForm::Clear_ListBox(CListBox& listBox)
{
	_int iCount = listBox.GetCount();
	for (int i = 0; i < iCount; ++i)
		listBox.DeleteString(0);
}

size_t CForm::Get_VectorSizeMapScene(CString wstrSceneName)
{
	auto& iter = find_if(m_mapScene.begin(), m_mapScene.end(), CTagFinder(wstrSceneName.GetString()));
	if (iter == m_mapScene.end())
		return 0;

	return iter->second.size();
}

CGameObject * CForm::Get_SelectedPrototypeObject()
{
	_uint iIndex = m_listBoxPrototype.GetCurSel();
	if (iIndex < 0)
		MSG_NULL("CForm::Get_SelectedPrototypeObject - iIndex < 0");

	return m_vectorPrototype[iIndex];
}

_int CForm::Tick(_float fTimeDelta)
{
	auto& iter = find_if(m_mapScene.begin(), m_mapScene.end(), CTagFinder(m_wstrSceneName.GetString()));
	if (iter == m_mapScene.end())
		MSG_FAIL("CForm Tick - iter == m_mapScene.end()");

	for (auto& pObj : iter->second)
		pObj->Tick(fTimeDelta);

	return _int();
}

_int CForm::LateTick(_float fTimeDelta)
{
	auto& iter = find_if(m_mapScene.begin(), m_mapScene.end(), CTagFinder(m_wstrSceneName.GetString()));
	if (iter == m_mapScene.end())
		MSG_FAIL("CForm LateTick - iter == m_mapScene.end()");

	for (auto& pObj : iter->second)
		pObj->LateTick(fTimeDelta);

	return _int();
}

HRESULT CForm::Render_SceneObject(CString wstrSceneName, _uint iIndex)
{
	auto& iter = find_if(m_mapScene.begin(), m_mapScene.end(), CTagFinder(wstrSceneName.GetString()));
	if (iter == m_mapScene.end())
		MSG_FAIL("CForm Render_SceneObject - iter == m_mapScene.end()");

	if (FAILED(iter->second[iIndex]->Render()))
		MSG_FAIL("CForm::Render_SceneObject - Render ");

	return S_OK;
}

HRESULT CForm::Change_MatOrth()
{
	if (m_pDevice == nullptr)
		MSG_FAIL("CForm::Change_MatOrth - m_pDevice == nullptr");

	//뷰 행렬은 항등으로
	_mat matProj;
	D3DXMatrixIdentity(&matProj);
	m_pDevice->SetTransform(D3DTS_VIEW, &matProj);

	//투영 행렬 세팅
	D3DXMatrixOrthoLH(&matProj, (FLOAT)g_iWinCX, (FLOAT)g_iWinCY, 0.f, 1.f);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	return S_OK;
}

HRESULT CForm::Render_PrototypeObjectOrth(CGameObject * pGameObj)
{
	//프로토타입 직교투영으로 렌더할거임
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->Render_Begin();

	pGameInstance->Tick_ToolEngine(0.1f);

	_mat matProj;
	D3DXMatrixIdentity(&matProj);
	m_pDevice->SetTransform(D3DTS_VIEW, &matProj);

	//픽쳐 크기 받아올거임
	RECT rc;
	m_picturePortotypeOrth.GetClientRect(&rc);
	D3DXMatrixOrthoLH(&matProj, (FLOAT)rc.right, (FLOAT)rc.bottom, 0.f, 1.f);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	int iIndex = m_listBoxPrototype.GetCurSel();
	CTransform* pTransform = dynamic_cast<CTransform*>(pGameObj->Get_Component(pTagComTransform));

	//스케일을 픽쳐크기만큼 늘렸다가 다시 줄일거임
	_vec3 vScale = pTransform->Get_Scale();
	pTransform->SetUp_Scale((_float)rc.right, (_float)rc.bottom, 1.f);

	//포스도 가운데로 옮겼다가 다시 원점 보낼거임
	_vec3 vPos = pTransform->Get_State(CTransform::STATE_POS);
	pTransform->SetUp_State(CTransform::STATE_POS, _vec3(vPos.x + _float(g_iWinCX >> 1), vPos.y + _float(g_iWinCY >> 1), 0.f));

	pGameObj->Render();

	pGameInstance->Render_End(m_picturePortotypeOrth.m_hWnd);

	//포스랑 스케일 원위치
	pTransform->SetUp_Scale(vScale.x, vScale.y, vScale.z);
	pTransform->SetUp_State(CTransform::STATE_POS, vPos);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CForm::Add_PrototypeObjToScene(OBJ_INFO * pObjInfo)
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	if (pMain == nullptr)
		MSG_FAIL("CForm::OnBnClicked_LoadScene - pMain == nullptr");

	CToolView* pToolView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	if (pToolView == nullptr)
		MSG_FAIL("CForm::OnBnClicked_LoadScene - pToolView == nullptr");

	CGameObject* pGameObj = pToolView->Create_ToolObj(pObjInfo);
	if (FAILED(Add_ObjectToScene(pGameObj)))
		MSG_FAIL("CForm OnBnClicked_AddPrototypeObjectToScene - Add_ObjectToScene")
	else
		AddString_ListBoxSceneObj(pObjInfo->szObjName);

	pToolView->Render_ToolView();

	return S_OK;
}

void CForm::LoadSetAll_ListBox(CString wstrPath, CListBox & listBox)
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile(wstrPath);
	CString fileName;
	while (bWorking)
	{
		//다음 파일 or 폴더 가 존재하면다면 TRUE 반환 
		bWorking = finder.FindNextFile();
		// folder 일 경우는 continue 
		if (finder.IsDirectory())
			continue;
		fileName = finder.GetFileTitle();

		if (0 > listBox.FindString(-1, fileName))
			listBox.AddString(fileName);
		//읽어온 파일 이름을 리스트박스에 넣음 
	}
}

void CForm::Free()
{
	//얕은 복사 받은 친구 맵에서 다 지울거기때문에 클리어만

	for (auto& pair : m_mapPrototype)
		Safe_Release(pair.second);
	m_mapPrototype.clear();

	for (auto& pair : m_mapScene)
	{
		for (auto& Obj : pair.second)
			Safe_Release(Obj);
		pair.second.clear();
		pair.second.shrink_to_fit();
	}

	m_mapScene.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pDevice);
	Safe_Release(m_pCamera);

	if (m_tUITool.GetSafeHwnd() != nullptr)
		m_tUITool.DestroyWindow();
	if (m_tNavMesh.GetSafeHwnd() != nullptr)
		m_tNavMesh.DestroyWindow();
	if (m_tTerrainTool.GetSafeHwnd() != nullptr)
		m_tTerrainTool.DestroyWindow();
	if (m_tWallTool.GetSafeHwnd() != nullptr)
		m_tWallTool.DestroyWindow();
	if (m_tPrototype.GetSafeHwnd() != nullptr)
		m_tPrototype.DestroyWindow();

	for (auto& pObj : m_vectorPrototype)
		Safe_Release(pObj);
	m_vectorPrototype.clear();
	m_vectorPrototype.shrink_to_fit();

	for (auto& pObj : m_vectorSceneObj)
		Safe_Release(pObj);
	m_vectorSceneObj.clear();
	m_vectorSceneObj.shrink_to_fit();
}

void CForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_wstrObjName);
	DDX_Control(pDX, IDC_LIST3, m_listBoxPrototype);
	DDX_Control(pDX, IDC_LIST2, m_listBoxScene);
	DDX_Text(pDX, IDC_EDIT2, m_wstrSceneName);
	DDX_Control(pDX, IDC_LIST4, m_listBoxSceneObj);
	DDX_Text(pDX, IDC_EDIT4, m_fObjPosX);
	DDX_Text(pDX, IDC_EDIT12, m_fObjPosY);
	DDX_Text(pDX, IDC_EDIT5, m_fObjPosZ);

	DDX_Text(pDX, IDC_EDIT6, m_fObjRotX);
	DDX_Text(pDX, IDC_EDIT9, m_fObjRotY);
	DDX_Text(pDX, IDC_EDIT8, m_fObjRotZ);

	DDX_Text(pDX, IDC_EDIT7, m_fObjScaleX);
	DDX_Text(pDX, IDC_EDIT10, m_fObjScaleY);
	DDX_Text(pDX, IDC_EDIT11, m_fObjScaleZ);
	DDX_Control(pDX, IDC_FORM_PICTURE_PROTOTYPE_ORTH, m_picturePortotypeOrth);

	DDX_Text(pDX, IDC_FORM_EDIT_CAM_POS_X, m_fEditCamPosX);
	DDX_Text(pDX, IDC_FORM_EDIT_CAM_POS_Y, m_fEditCamPosY);
	DDX_Text(pDX, IDC_FORM_EDIT_CAM_POS_Z, m_fEditCamPosZ);
	DDX_Text(pDX, IDC_FORM_EDIT_BACK_CURLING_ON_OFF, m_wstrBackCurlingOnOff);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON6, &CForm::OnBnClicked_CreatePrototype)
	ON_BN_CLICKED(IDC_BUTTON7, &CForm::OnBnClicked_CreateNavMesh)
	ON_BN_CLICKED(IDC_BUTTON5, &CForm::OnBnClicked_CreateScene)
	ON_BN_CLICKED(IDC_BUTTON4, &CForm::OnBnClicked_AddPrototypeObjectToScene)
	ON_BN_CLICKED(IDC_BUTTON8, &CForm::OnBnClicked_SetTransform)
	ON_BN_CLICKED(IDC_BUTTON1, &CForm::OnBnClicked_SaveScene)
	ON_BN_CLICKED(IDC_BUTTON2, &CForm::OnBnClicked_LoadScene)
	ON_BN_CLICKED(IDC_BUTTON3, &CForm::OnBnClicked_DeleteObjectInScene)
	ON_BN_CLICKED(IDC_BUTTON9, &CForm::OnBnClicked_LoadPrototypeObject)
	ON_LBN_SELCHANGE(IDC_LIST2, &CForm::OnLbnSelchange_ChangeScene)
	ON_LBN_SELCHANGE(IDC_LIST3, &CForm::OnLbnSelchange_ClickPrototype)
	ON_BN_CLICKED(IDC_BUTTON10, &CForm::OnBnClickedButton_SavePrototype)
	ON_LBN_SELCHANGE(IDC_LIST4, &CForm::OnLbnSelchangeList_ChangeSceneObject)
	ON_BN_CLICKED(IDC_BUTTON11, &CForm::OnBnClickedButton_CreateEditTerrain)
	ON_EN_CHANGE(IDC_EDIT4, &CForm::OnEnChangeEdit4_ChangePosX)
	ON_EN_CHANGE(IDC_EDIT12, &CForm::OnEnChangeEdit12_ChangePosY)
	ON_EN_CHANGE(IDC_EDIT5, &CForm::OnEnChangeEdit5_ChangePosZ)
	ON_EN_CHANGE(IDC_EDIT6, &CForm::OnEnChangeEdit6_ChangeRotX)
	ON_EN_CHANGE(IDC_EDIT9, &CForm::OnEnChangeEdit9_ChangeRotY)
	ON_EN_CHANGE(IDC_EDIT8, &CForm::OnEnChangeEdit8_ChangeRotZ)
	ON_EN_CHANGE(IDC_EDIT7, &CForm::OnEnChangeEdit7_ChangeScaleX)
	ON_EN_CHANGE(IDC_EDIT10, &CForm::OnEnChangeEdit10_ChangeScaleY)
	ON_EN_CHANGE(IDC_EDIT11, &CForm::OnEnChangeEdit11__ChangeScaleZ)
	ON_BN_CLICKED(IDD_RECTYZTOOL, &CForm::OnBnClicked_Wall_Create_Edit_Button)
	ON_BN_CLICKED(IDC_BUTTON_FORM_CREATE_UI, &CForm::OnBnClickedButtonFormCreateUi)
	ON_BN_CLICKED(IDC_FORM_BUTTON_CREATE_UI_PROTOTYPE, &CForm::OnBnClickedFormButtonCreateUiPrototype)
	ON_EN_CHANGE(IDC_FORM_EDIT_CAM_POS_X, &CForm::OnEnChangeFormEditCamPosX)
	ON_EN_CHANGE(IDC_FORM_EDIT_CAM_POS_Y, &CForm::OnEnChangeFormEditCamPosY)
	ON_EN_CHANGE(IDC_FORM_EDIT_CAM_POS_Z, &CForm::OnEnChangeFormEditCamPosZ)
	ON_BN_CLICKED(IDC_FORM_BUTTON_BACK_CURLING, &CForm::OnBnClickedFormButtonBackCurling)
	ON_BN_CLICKED(IDC_FORM_BUTTON_CREATE_LOT_OBJECT, &CForm::OnBnClickedFormButtonCreateLotObject)
END_MESSAGE_MAP()


// CForm 진단입니다.

#ifdef _DEBUG
void CForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForm 메시지 처리기입니다.


void CForm::OnBnClicked_CreatePrototype()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_tPrototype.GetSafeHwnd())
		m_tPrototype.Create(IDD_PROTOTYPETOOL);

	m_tPrototype.ShowWindow(SW_SHOW);
}


void CForm::OnBnClicked_CreateNavMesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_tNavMesh.GetSafeHwnd())
		m_tNavMesh.Create(IDD_NAVMESHTOOL);

	m_tNavMesh.ShowWindow(SW_SHOW);
}


void CForm::OnBnClicked_CreateScene()
{
	CFileDialog Dlg(FALSE, L"dat", L"*.dat");
	TCHAR szBuf[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Client\\bin\\Resource\\Data\\Scene");
	Dlg.m_ofn.lpstrInitialDir = szBuf;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrPath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrPath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		CloseHandle(hFile);
	}
}

void CForm::OnBnClicked_AddPrototypeObjectToScene()
{
	int iIndex = m_listBoxPrototype.GetCurSel();

	if (iIndex < 0)
		MSG_VOID("CForm OnBnClicked_AddPrototypeObjectToScene - iIndex is -1");

	CString wstrPrototypeName{};
	m_listBoxPrototype.GetText(iIndex, wstrPrototypeName);

	auto& iter = find_if(m_mapPrototype.begin(), m_mapPrototype.end(), CTagFinder(wstrPrototypeName.GetString()));

	if (iter == m_mapPrototype.end())
		MSG_VOID("CForm OnBnClicked_AddPrototypeObjectToScene - m_mapPrototype.end()");

	//값 복사해서 새로 생성 해줘야함!
	OBJ_INFO* pObjInfo = new OBJ_INFO;
	memcpy(pObjInfo, static_cast<CToolObj*>(iter->second)->Get_ObjInfo(), sizeof(OBJ_INFO));

	Add_PrototypeObjToScene(pObjInfo);

}

void CForm::OnBnClicked_SetTransform()
{
	UpdateData(TRUE);

	_int iIndex = m_listBoxSceneObj.GetCurSel();
	if (iIndex < 0)
		MSG_VOID("CForm OnBnClicked_SetTransform - iIndex < 0");
	if (iIndex >= m_vectorSceneObj.size())
		MSG_VOID("CForm OnBnClicked_SetTransform - iIndex >= m_vectorSceneObj.size() ");

	//dynamic_cast<CToolObj*>(m_vectorSceneObj[iIndex])->Set_Matrix();

	UpdateData(FALSE);
}

void CForm::OnBnClicked_SaveScene()
{
	TCHAR szBuf[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Client\\bin\\Resource\\Data\\Scene\\");

	for (auto& pair : m_mapScene)
	{
		CString wstrPath = szBuf;
		wstrPath += pair.first + L".dat";
		HANDLE hFile = CreateFile(wstrPath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			MSG_VOID("CForm OnBnClicked_SaveScene - hFile is INVALID_HANDLE_VALUE");

		_ulong dwByte = 0;
		for (auto& pGameObj : pair.second)
		{
			CToolObj* pToolObj = dynamic_cast<CToolObj*>(pGameObj);
			if (pToolObj == nullptr)
				MSG_VOID("CForm OnBnClicked_SaveScene - pToolObj is null");
			OBJ_INFO tObjInfo = *pToolObj->Get_ObjInfo();

			WriteFile(hFile, &tObjInfo, sizeof(OBJ_INFO), &dwByte, nullptr);
		}

		CloseHandle(hFile);
	}
	return;
}


void CForm::OnBnClicked_LoadScene()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	TCHAR szBuf[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szBuf);
	PathRemoveFileSpec(szBuf); 
	lstrcat(szBuf, L"\\Client\\bin\\Resource\\Data\\Scene\\");
	CString wstrFilePathOrigin = szBuf;
	CString wstrFilePath = wstrFilePathOrigin;
	lstrcat(szBuf, L"*.*");
	CString wstrPath = szBuf;
	CString fileNameWithoutExtension, fileName;
	CFileFind finder;
	BOOL bWorking = finder.FindFile(wstrPath);

	while (bWorking)
	{
		//다음 파일 or 폴더 가 존재하면다면 TRUE 반환 
		bWorking = finder.FindNextFile();
		// folder 일 경우는 continue 
		if (finder.IsDirectory())
			continue;

		fileNameWithoutExtension = finder.GetFileTitle();
		fileName = finder.GetFileName();
		wstrFilePath += fileName;
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			MSG_VOID("CForm OnBnClicked_LoadScene - hFile is INVALID_HANDLE_VALUE");


		AddString_ListBoxScene(fileNameWithoutExtension);
		if (FAILED(Set_MapScene(fileNameWithoutExtension)))
		{
			MSGBOX("CForm::OnBnClicked_LoadScene - Set_MapScene");
			wstrFilePath = wstrFilePathOrigin;
			CloseHandle(hFile);
			continue;
		}
		Set_SceneName(fileNameWithoutExtension);

		_ulong dwByte = 0;
		auto& iter = find_if(m_mapScene.begin(), m_mapScene.end(), CTagFinder(fileNameWithoutExtension));
		if (iter == m_mapScene.end())
		{
			MSGBOX("CForm::OnBnClicked_LoadScene - iter is end");
			wstrFilePath = wstrFilePathOrigin;
			CloseHandle(hFile);
			continue;
		}

		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		if (pMain == nullptr)
			MSG_VOID("CForm::OnBnClicked_LoadScene - pMain == nullptr");
		CToolView* pToolView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
		if (pToolView == nullptr)
			MSG_VOID("CForm::OnBnClicked_LoadScene - pToolView == nullptr");

		//오브젝트 씬에 추가 후 씬 오브젝트 리스트에도 추가
		while (true)
		{
			OBJ_INFO* pObjInfo = new OBJ_INFO;
			ReadFile(hFile, pObjInfo, sizeof(OBJ_INFO), &dwByte, nullptr);
			if (dwByte <= 0)
			{
				Safe_Delete(pObjInfo);
				break;
			}

			iter->second.emplace_back(pToolView->Create_ToolObj(pObjInfo));
		}

		CloseHandle(hFile);
		wstrFilePath = wstrFilePathOrigin;
	}
}


void CForm::OnBnClicked_DeleteObjectInScene()
{
	UpdateData(TRUE);
	int iIndex = m_listBoxSceneObj.GetCurSel();

	if (iIndex < 0)
		MSG_VOID("CForm OnBnClicked_DeleteObjectInScene - iIndex < 0");

	m_listBoxSceneObj.DeleteString(iIndex);
	auto& iter = find_if(m_mapScene.begin(), m_mapScene.end(), CTagFinder(m_wstrSceneName));
	if (iter == m_mapScene.end())
		MSG_VOID("CForm OnBnClicked_DeleteObjectInScene - SceneName is wrong");

	iter->second.erase(iter->second.begin() + iIndex);
	m_vectorSceneObj.erase(m_vectorSceneObj.begin() + iIndex);

	m_pTool->Render_ToolView();
	UpdateData(FALSE);
}


void CForm::OnBnClicked_LoadPrototypeObject()
{
	//경로 받아오기
	TCHAR szBuf[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Client\\bin\\Resource\\Data\\PrototypeObject\\");
	CString wstrFilePathOrigin = szBuf;
	CString wstrFilePath = wstrFilePathOrigin;
	lstrcat(szBuf, L"*.*");
	CString wstrPath = szBuf;
	CString fileNameWithoutExtension, fileName;
	CFileFind finder;
	//경로에 파일이 있는지 체크
	BOOL bWorking = finder.FindFile(wstrPath);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	if (pMain == nullptr)
		MSG_VOID("CForm::OnBnClicked_LoadScene - pMain == nullptr");
	CToolView* pToolView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	if (pToolView == nullptr)
		MSG_VOID("CForm::OnBnClicked_LoadScene - pToolView == nullptr");

	while (bWorking)
	{
		//다음 파일 or 폴더 가 존재하면다면 TRUE 반환 
		bWorking = finder.FindNextFile();
		// folder 일 경우는 continue 
		if (finder.IsDirectory())
			continue;

		//확장자 제거하고 파일의 이름만 가져옴 예시) text.png 가져온다 하면 text만 가져옴
		fileNameWithoutExtension = finder.GetFileTitle();
		//확장자 포함 파일이름 
		fileName = finder.GetFileName();
		wstrFilePath += fileName;
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;
		
		OBJ_INFO* pInfo = nullptr;

		while (true)
		{
			pInfo = new OBJ_INFO;

			_ulong dwNum;
			ReadFile(hFile, pInfo, sizeof(OBJ_INFO), &dwNum, nullptr);

			if (dwNum <= 0)
			{
				Safe_Delete(pInfo);
				break;
			}
			else
			{
				CGameObject* pGameObj = pToolView->Create_ToolObj(pInfo);

				AddString_ListBoxPrototype(pInfo->szObjName, pGameObj);
			}
		}

		CloseHandle(hFile);
		wstrFilePath = wstrFilePathOrigin;
	}
}


void CForm::OnLbnSelchange_ChangeScene()
{
	int iIndex = m_listBoxScene.GetCurSel();

	if (iIndex < 0)
		MSG_VOID("CForm OnLbnSelchange_ChangeScene - iIndex < 0");
	CString wstrName;
	m_listBoxScene.GetText(iIndex, wstrName);

	Set_SceneName(wstrName);

	auto& iter = find_if(m_mapScene.begin(), m_mapScene.end(), CTagFinder(wstrName));
	if (iter == m_mapScene.end())
		MSG_VOID("CForm OnLbnSelchange_ChangeScene - iter == end()");

	Clear_ListBox(m_listBoxSceneObj);

	CString wstrObjName;
	m_vectorSceneObj.clear();
	for (auto& pObj : iter->second)
	{
		m_vectorSceneObj.emplace_back(pObj);
		AddString_ListBoxSceneObj(static_cast<CToolObj*>(pObj)->Get_ObjInfo()->szObjName);
	}

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	if (pMain == nullptr)
		MSG_VOID("CForm::Change_SelectedObjectTransform - pMain == nullptr");
	CToolView* pToolView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	if (pToolView == nullptr)
		MSG_VOID("CForm::Change_SelectedObjectTransform - pToolView == nullptr");

	pToolView->Render_ToolView();
}


//리스트박스프로토타입 셀 체인지
void CForm::OnLbnSelchange_ClickPrototype()
{
	//잘못된 값인지 검사
	int iIndex = m_listBoxPrototype.GetCurSel();
	CString wstrName;
	m_listBoxPrototype.GetText(iIndex, wstrName);
	auto& iter = find_if(m_mapPrototype.begin(), m_mapPrototype.end(), CTagFinder(wstrName.GetString()));
	if (iter == m_mapPrototype.end())
		MSG_VOID("CForm OnLbnSelchange_ClickPrototype - iter is end");
	
	//오브젝트 이름 출력
	Set_ObjName(wstrName);

	//오브젝트 정보 화면에 출력
	CToolObj* pToolObj = dynamic_cast<CToolObj*>(iter->second);
	if (pToolObj == nullptr)
		MSG_VOID("CForm::OnLbnSelchange_ClickPrototype - pToolObj == nullptr");
	Set_ObjInfoToScreen(pToolObj->Get_ObjInfo());

	//선택된 프로토타입 직교투영으로 렌더
	Render_PrototypeObjectOrth(pToolObj);
}


void CForm::OnBnClickedButton_SavePrototype()
{
	CFileDialog Dlg(FALSE, L"dat", L"*.dat");
	TCHAR szBuf[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Client\\bin\\Resource\\Data\\PrototypeObject");
	Dlg.m_ofn.lpstrInitialDir = szBuf;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrPath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrPath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		OBJ_INFO tInfo;
		lstrcpy(tInfo.szObjName, L"Test");
		D3DXMatrixIdentity(&tInfo.matWorld);
		_ulong dwNum;

		WriteFile(hFile, &tInfo, sizeof(OBJ_INFO), &dwNum, nullptr);

		CloseHandle(hFile);
	}
}


void CForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	m_pTool = pView;
	m_pDevice = pView->Clone_Device();
	m_pCamera = pView->Clone_Camera();
	m_pCamera->Setup_Form(this);
	
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);
	 
	RELEASE_INSTANCE(CGameInstance);
}


void CForm::OnLbnSelchangeList_ChangeSceneObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iIndex = m_listBoxSceneObj.GetCurSel();

	if (iIndex < 0)
		MSG_VOID("CForm OnLbnSelchangeList_ChangeSceneObject - iIndex < 0");

	auto& iter = find_if(m_mapScene.begin(), m_mapScene.end(), CTagFinder(m_wstrSceneName));
	if (iter == m_mapScene.end())
		MSG_VOID("CForm OnLbnSelchangeList_ChangeSceneObject - 씬 이름이 잘못됬습니다");

	CToolObj* pToolObj = dynamic_cast<CToolObj*>(iter->second[iIndex]);
	if (pToolObj == nullptr)
		MSG_VOID("CForm::OnLbnSelchangeList_ChangeSceneObject - pToolObj == nullptr");
	Set_ObjInfoToScreen(pToolObj->Get_ObjInfo());
	Set_ObjName(pToolObj->Get_ObjInfo()->szObjName);
}


void CForm::OnBnClickedButton_CreateEditTerrain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_tTerrainTool.GetSafeHwnd())
		m_tTerrainTool.Create(IDD_TERRAINTOOL);

	m_tTerrainTool.ShowWindow(SW_SHOW);
}

void CForm::OnEnChangeEdit4_ChangePosX()
{
	Change_SelectedObjectTransform();
}


void CForm::OnEnChangeEdit12_ChangePosY()
{
	Change_SelectedObjectTransform();
}


void CForm::OnEnChangeEdit5_ChangePosZ()
{
	Change_SelectedObjectTransform();
}


void CForm::OnEnChangeEdit6_ChangeRotX()
{
	Change_SelectedObjectTransform();

}


void CForm::OnEnChangeEdit9_ChangeRotY()
{
	Change_SelectedObjectTransform();

}


void CForm::OnEnChangeEdit8_ChangeRotZ()
{
	Change_SelectedObjectTransform();

}


void CForm::OnEnChangeEdit7_ChangeScaleX()
{
	Change_SelectedObjectTransform();

}


void CForm::OnEnChangeEdit10_ChangeScaleY()
{
	Change_SelectedObjectTransform();

}


void CForm::OnEnChangeEdit11__ChangeScaleZ()
{
	Change_SelectedObjectTransform();
}


void CForm::OnBnClicked_Wall_Create_Edit_Button()
{
	if (nullptr == m_tWallTool.GetSafeHwnd())
		m_tWallTool.Create(IDD_RECTYZTOOL);

	m_tWallTool.ShowWindow(SW_SHOW);
}

void CForm::OnBnClickedButtonFormCreateUi()
{
	if (nullptr == m_tUITool.GetSafeHwnd())
		m_tUITool.Create(IDD_UITOOL);

	m_tUITool.ShowWindow(SW_SHOW);
}

//Texture UI 폴더 참조해서 dat 파일 만드는 버튼
void CForm::OnBnClickedFormButtonCreateUiPrototype()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	if (pMain == nullptr)
		MSG_VOID("CForm::Change_SelectedObjectTransform - pMain == nullptr");
	CToolView* pToolView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	if (pToolView == nullptr)
		MSG_VOID("CForm::Change_SelectedObjectTransform - pToolView == nullptr");

	CFileFind finder;
	BOOL bWorking = finder.FindFile(L"../Client/bin/Resource/Texture/UI/*.*");
	CString fileName;
	while (bWorking)
	{
		//다음 파일 or 폴더 가 존재하면다면 TRUE 반환 
		bWorking = finder.FindNextFile();
		// folder 일 경우는 continue 
		if (finder.IsDirectory())
			continue;

		//프로토타입 스트링에 들어갈 이름 UI0
		fileName = finder.GetFileTitle();
		//UI0 라는 파일명에서 UI를 지우고 0 만 남김
		//저장할 객체정보 생성
		CString wstrNumber = fileName;
		CString wstrClientTextureName = L"PrototypeTextureUI";
		wstrNumber.TrimLeft(L"UI");
		OBJ_INFO tObjInfo;
		tObjInfo.eVIBufferType = OBJ_INFO::VI_BUFFER_TYPE_UI;
		tObjInfo.iTextureIndex = _wtoi(wstrNumber);
		tObjInfo.SetUp_Scale(1.f, 1.f, 1.f);
		lstrcpy(tObjInfo.szObjName, fileName.GetString());
		lstrcpy(tObjInfo.szTextureName, wstrClientTextureName.GetString());

		//객체 정보 생성 했으니 이제 저장할 파일 열어야함
		CString wstrTextureDatPath = L"../Client/bin/Resource/Data/PrototypeObject/";
		wstrTextureDatPath += tObjInfo.szObjName;
		wstrTextureDatPath += L".dat";

		HANDLE hFile = CreateFile(wstrTextureDatPath, GENERIC_WRITE, NULL, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (hFile == INVALID_HANDLE_VALUE)
			MSG_VOID("OnBnClickedFormButtonCreateUiPrototype - hFile == INVALID_HANDLE_VALUE");

		//파일 열었으니 이제 저장해야함
		_ulong dwByte;
		WriteFile(hFile, &tObjInfo, sizeof(OBJ_INFO), &dwByte, nullptr);

		CloseHandle(hFile);
	}
}


void CForm::OnEnChangeFormEditCamPosX()
{
	Set_CamPosToCam();
}


void CForm::OnEnChangeFormEditCamPosY()
{
	Set_CamPosToCam();
}


void CForm::OnEnChangeFormEditCamPosZ()
{
	Set_CamPosToCam();
}


void CForm::OnBnClickedFormButtonBackCurling()
{
	UpdateData(TRUE);

	if (m_wstrBackCurlingOnOff == L"On")
	{
		m_pDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_NONE);
		m_wstrBackCurlingOnOff = L"Off";
		m_pTool->Render_ToolView();
	}
	else if (m_wstrBackCurlingOnOff == L"Off")
	{
		m_pDevice->SetRenderState(D3DRENDERSTATETYPE::D3DRS_CULLMODE, D3DCULL_CCW);
		m_wstrBackCurlingOnOff = L"On";
		m_pTool->Render_ToolView();
	}

	UpdateData(FALSE);
}

void CForm::OnBnClickedFormButtonCreateLotObject()
{
	if (nullptr == m_tLotObjTool.GetSafeHwnd())
		m_tLotObjTool.Create(IDD_LOTOBJECTTOOL);
	
	m_tLotObjTool.ShowWindow(SW_SHOW);
}
