// TerrainTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "TerrainTool.h"
#include "afxdialogex.h"
#include "ToolTerrainObj.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "FilePath.h"
#include "ToolViewCamera.h"

// CTerrainTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTerrainTool, CDialog)

CTerrainTool::CTerrainTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TERRAINTOOL, pParent)
	, m_fTerrainPosX(0)
	, m_wstrCurTerrainName(_T(""))
{

}

CTerrainTool::~CTerrainTool()
{
	Free();
}

HRESULT CTerrainTool::Add_ListBoxString(CListBox & listBox, CString wstr)
{
	UpdateData(TRUE);

	if (0 < listBox.FindString(0, wstr))
		MSG_FAIL("CTerrainTool Add_ListBoxString - 중복된 스트링");

	listBox.AddString(wstr);

	UpdateData(FALSE);
	return S_OK;
}

HRESULT CTerrainTool::Set_ScreenValue(OBJ_INFO* pObjInfo)
{
	UpdateData(TRUE);

	_vec3 vPos = pObjInfo->Get_State(OBJ_INFO::STATE_POS);
	m_fTerrainPosX = vPos.x;
	m_fTerrainPosY = vPos.y;
	m_fTerrainPosZ = vPos.z;

	_vec3 vRot = pObjInfo->vRot;
	m_fTerrainRotX = vRot.x;
	m_fTerrainRotY = vRot.y;
	m_fTerrainRotZ = vRot.z;

	_vec3 vScale = pObjInfo->Get_Scale();
	m_fTerrainScaleX = vScale.x;
	m_fTerrainScaleY = vScale.y;
	m_fTerrainScaleZ = vScale.z;

	m_fTerrainSizeX = pObjInfo->fSizeX;
	m_fTerrainSizeZ = pObjInfo->fSizeZ;

	m_fTerrainU = pObjInfo->fU;
	m_fTerrainV = pObjInfo->fV;

	m_iTerrainVerticesNumX = pObjInfo->iNumVerticesX;
	m_iTerrainVerticesNumZ = pObjInfo->iNumVerticesZ;

	m_wstrCurTerrainName = pObjInfo->szObjName;

	m_listBoxTexture.SetCurSel(pObjInfo->iTextureIndex);

	UpdateData(FALSE);

	Create_NewTerrainBuffer();

	return S_OK;
}

HRESULT CTerrainTool::Create_NewTerrainBuffer()
{
	UpdateData(TRUE);
	OBJ_INFO* pObjInfo = new OBJ_INFO;

	pObjInfo->eVIBufferType = OBJ_INFO::VIBufferType::VI_BUFFER_TYPE_TERRAIN;

	pObjInfo->fSizeX = m_fTerrainSizeX;
	pObjInfo->fSizeZ = m_fTerrainSizeZ;

	pObjInfo->fU = m_fTerrainU;
	pObjInfo->fV = m_fTerrainV;

	pObjInfo->iNumVerticesX = m_iTerrainVerticesNumX;
	pObjInfo->iNumVerticesZ = m_iTerrainVerticesNumZ;
	pObjInfo->iTextureIndex = m_listBoxTexture.GetCurSel();

	pObjInfo->vRot.x = m_fTerrainRotX;
	pObjInfo->vRot.y = m_fTerrainRotY;
	pObjInfo->vRot.z = m_fTerrainRotZ;

	pObjInfo->SetUp_State(OBJ_INFO::STATE_POS, _vec3(m_fTerrainPosX, m_fTerrainPosY, m_fTerrainPosZ));
	pObjInfo->SetUp_Scale(m_fTerrainScaleX, m_fTerrainScaleY, m_fTerrainScaleZ);

	pObjInfo->SetUp_Rotation(_vec3(1.f, 0.f, 0.f), m_fTerrainRotX);
	pObjInfo->Rotation_Axis(_vec3(0.f, 1.f, 0.f), m_fTerrainRotY);
	pObjInfo->Rotation_Axis(_vec3(0.f, 0.f, 1.f), m_fTerrainRotZ);

	int iIndex = m_listBoxHeightMap.GetCurSel();
	CString wstrHeightMap;
	if (iIndex < 0)
		wstrHeightMap = L"None";
	else
		m_listBoxHeightMap.GetText(iIndex, wstrHeightMap);

	lstrcpy(pObjInfo->szHeightMap, wstrHeightMap);
	
	lstrcpy(pObjInfo->szObjName, m_wstrCurTerrainName.GetString());

	m_pTerrain->Set_TerrainBuffer(pObjInfo);

	Render_Tool();
	UpdateData(FALSE);

	return S_OK;
}

HRESULT CTerrainTool::Render_Tool()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	pInstance->Render_Begin();

	pInstance->Tick_ToolEngine(0.1f);

	_vec3 vPos = m_pCamera->Get_CameraPos();
	_float fFov = m_pCamera->Get_CameraFov();
	_stprintf_s(m_szPos, TEXT("X: %f \nY: %f \nZ: %f \nFov : %f"), vPos.x, vPos.y, vPos.z, fFov);
	m_pCamera->Tick(0.1f);

	pInstance->Draw_Font(m_szPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	m_pTerrain->Render();

	pInstance->Render_End(m_pictureCtrlTerrain.m_hWnd);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CTerrainTool::Free()
{
	if (m_pCamera != nullptr)
		if (m_pCamera->Get_CameraAction())
			m_pCamera->Do_CameraAction();

	Safe_Release(m_pCamera);
	Safe_Release(m_pTerrain);
	Safe_Release(m_pDevice);
	Safe_Release(m_pRenderer);
}

HRESULT CTerrainTool::Load_Texture()
{
	wstring wstrFilePath = L"../Client/bin/Resource/Texture/Terrain/*.*";
	CFileFind fileFind;
	BOOL bContinue = fileFind.FindFile(wstrFilePath.c_str());
	while (bContinue)
	{
		bContinue = fileFind.FindNextFile();
		if (fileFind.IsDots() || fileFind.IsDirectory())
			continue;
		else
		{
			// 내가 작업해야 하는 환경. 

			//시스템 파일일 경우. 
			if (fileFind.IsSystem())
				continue;
			
			Add_ListBoxString(m_listBoxTexture, fileFind.GetFileTitle());
		}

	}

	return S_OK;
}

void CTerrainTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST5, m_listBoxHeightMap);
	DDX_Control(pDX, IDC_LIST1, m_listBoxTexture);
	DDX_Text(pDX, IDC_EDIT27, m_wstrCurTerrainName);

	DDX_Text(pDX, IDC_EDIT1, m_fTerrainPosX);
	DDX_Text(pDX, IDC_EDIT13, m_fTerrainPosY);
	DDX_Text(pDX, IDC_EDIT14, m_fTerrainPosZ);

	DDX_Text(pDX, IDC_EDIT15, m_fTerrainRotX);
	DDX_Text(pDX, IDC_EDIT16, m_fTerrainRotY);
	DDX_Text(pDX, IDC_EDIT17, m_fTerrainRotZ);

	DDX_Text(pDX, IDC_EDIT18, m_fTerrainScaleX);
	DDX_Text(pDX, IDC_EDIT19, m_fTerrainScaleY);
	DDX_Text(pDX, IDC_EDIT20, m_fTerrainScaleZ);

	DDX_Text(pDX, IDC_EDIT21, m_fTerrainSizeX);
	DDX_Text(pDX, IDC_EDIT22, m_fTerrainSizeZ);

	DDX_Text(pDX, IDC_EDIT23, m_fTerrainU);
	DDX_Text(pDX, IDC_EDIT24, m_fTerrainV);

	DDX_Text(pDX, IDC_EDIT25, m_iTerrainVerticesNumX);
	DDX_Text(pDX, IDC_EDIT26, m_iTerrainVerticesNumZ);

	DDX_Control(pDX, ID_PICTURE1, m_pictureCtrlTerrain);
}


BEGIN_MESSAGE_MAP(CTerrainTool, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, &CTerrainTool::OnEnChangeEdit_PosX)
	ON_WM_MBUTTONDOWN()
	ON_WM_TIMER()
	ON_LBN_SELCHANGE(IDC_LIST1, &CTerrainTool::OnLbnSelchangeList_ChangeTexture)
	ON_EN_CHANGE(IDC_EDIT13, &CTerrainTool::OnEnChangeEdit_ChangePosY)
	ON_EN_CHANGE(IDC_EDIT14, &CTerrainTool::OnEnChangeEdit_ChangePosZ)
	ON_EN_CHANGE(IDC_EDIT15, &CTerrainTool::OnEnChangeEdit_ChangeRotX)
	ON_EN_CHANGE(IDC_EDIT16, &CTerrainTool::OnEnChangeEdit_ChangeRotY)
	ON_EN_CHANGE(IDC_EDIT17, &CTerrainTool::OnEnChangeEdit_ChangeRotZ)
	ON_EN_CHANGE(IDC_EDIT18, &CTerrainTool::OnEnChangeEdit_ChangeScaleX)
	ON_EN_CHANGE(IDC_EDIT19, &CTerrainTool::OnEnChangeEdit_ChangeScaleY)
	ON_EN_CHANGE(IDC_EDIT20, &CTerrainTool::OnEnChangeEdit_ChangeScaleZ)
	ON_EN_CHANGE(IDC_EDIT23, &CTerrainTool::OnEnChangeEdit_ChangeUVX)
	ON_EN_CHANGE(IDC_EDIT24, &CTerrainTool::OnEnChangeEdit_ChangeUVZ)
	ON_EN_CHANGE(IDC_EDIT25, &CTerrainTool::OnEnChangeEdit_ChangeVecticesNumX)
	ON_EN_CHANGE(IDC_EDIT26, &CTerrainTool::OnEnChangeEdit_ChangeVecticesNumZ)
	ON_BN_CLICKED(IDC_BUTTON1, &CTerrainTool::OnBnClickedButton_SaveTerrain)
	ON_BN_CLICKED(IDC_BUTTON2, &CTerrainTool::OnBnClickedButton_LoadTerrain)
	ON_BN_CLICKED(IDC_TERRAIN_TOOL_CREATE_PROTOTYPE, &CTerrainTool::OnBnClickedTerrainToolCreatePrototype)
	ON_BN_CLICKED(IDC_BUTTON_TERRAIN_TOOL_WIRE_FRAME, &CTerrainTool::OnBnClickedButtonTerrainToolWireFrame)
END_MESSAGE_MAP()


// CTerrainTool 메시지 처리기입니다.


BOOL CTerrainTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	Add_ListBoxString(m_listBoxHeightMap, L"None");

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView* pToolView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	m_pDevice = pToolView->Clone_Device();
	m_pRenderer = pToolView->Clone_Renderer();
	m_pTerrain = CToolTerrainObj::Create(m_pDevice);

	m_pCamera = CToolViewCamera::Create(m_pDevice);
	m_pCamera->Sync_Window(this->m_hWnd);
	_vec3 vCameraLook = _vec3(0.f, 0.f, 0.f);
	m_pCamera->SetUp_InitialState(_vec3(0.f, 20.f, -20.f), vCameraLook);

	_vec3 vPos = m_pCamera->Get_CameraPos();
	_float fFov = m_pCamera->Get_CameraFov();

	_stprintf_s(m_szPos, TEXT("X: %f \nY: %f \nZ: %f \nFov : %f"), vPos.x, vPos.y, vPos.z, fFov);

	Create_NewTerrainBuffer();

	Load_Texture();

	Render_Tool();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTerrainTool::OnEnChangeEdit_PosX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Create_NewTerrainBuffer();
}


void CTerrainTool::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_pCamera->Do_CameraAction();
	if (m_pCamera->Get_CameraAction())
		SetTimer(0, 100, NULL);
	else
		KillTimer(0);

	Render_Tool();

	CDialog::OnMButtonDown(nFlags, point);
}


void CTerrainTool::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);


	switch (nIDEvent)
	{
	case 0:
		Render_Tool();
	default:
		break;
	}
	RELEASE_INSTANCE(CGameInstance);


	CDialog::OnTimer(nIDEvent);
}


void CTerrainTool::OnLbnSelchangeList_ChangeTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iIndex = m_listBoxTexture.GetCurSel();
	m_pTerrain->Set_TextureIndex(iIndex);
	Render_Tool();
}


void CTerrainTool::OnEnChangeEdit_ChangePosY()
{
	Create_NewTerrainBuffer();
}


void CTerrainTool::OnEnChangeEdit_ChangePosZ()
{

	Create_NewTerrainBuffer();
}


void CTerrainTool::OnEnChangeEdit_ChangeRotX()
{
	Create_NewTerrainBuffer();
}


void CTerrainTool::OnEnChangeEdit_ChangeRotY()
{
	Create_NewTerrainBuffer();
}


void CTerrainTool::OnEnChangeEdit_ChangeRotZ()
{

	Create_NewTerrainBuffer();
}


void CTerrainTool::OnEnChangeEdit_ChangeScaleX()
{

	Create_NewTerrainBuffer();
}


void CTerrainTool::OnEnChangeEdit_ChangeScaleY()
{

	Create_NewTerrainBuffer();
}


void CTerrainTool::OnEnChangeEdit_ChangeScaleZ()
{
	Create_NewTerrainBuffer();
}


void CTerrainTool::OnEnChangeEdit_ChangeUVX()
{
	Create_NewTerrainBuffer();
}


void CTerrainTool::OnEnChangeEdit_ChangeUVZ()
{
	Create_NewTerrainBuffer();
}


void CTerrainTool::OnEnChangeEdit_ChangeVecticesNumX()
{
	Create_NewTerrainBuffer();

}


void CTerrainTool::OnEnChangeEdit_ChangeVecticesNumZ()
{
	Create_NewTerrainBuffer();

}


void CTerrainTool::OnBnClickedButton_SaveTerrain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, L"dat", L"*.dat");
	TCHAR szBuf[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Client\\bin\\Resource\\Data\\Terrain");
	Dlg.m_ofn.lpstrInitialDir = szBuf;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrPath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrPath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		_ulong dwByte;
		CString wstrTerrainName = CFilePath::Get_FileNameWithoutExtension(wstrPath);

		m_pTerrain->Set_ObjName(wstrTerrainName);
		WriteFile(hFile, m_pTerrain->Get_ObjInfo(), sizeof(OBJ_INFO), &dwByte, nullptr);

		if (dwByte <= 0)
			MSGBOX("CTerrainTool OnBnClickedButton_SaveTerrain - Fail to SaveTerrain");

		CloseHandle(hFile);
	}
	return;
}


void CTerrainTool::OnBnClickedButton_LoadTerrain()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE, L"dat", L"*.dat");
	TCHAR szBuf[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Client\\bin\\Resource\\Data\\Terrain");
	Dlg.m_ofn.lpstrInitialDir = szBuf;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrPath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrPath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		_ulong dwByte;
		OBJ_INFO* pObjInfo = new OBJ_INFO;
		ReadFile(hFile, pObjInfo, sizeof(OBJ_INFO), &dwByte, nullptr);

		if (dwByte <= 0)
			MSGBOX("CTerrainTool OnBnClickedButton_LoadTerrain - Fail to LoadTerrain");
		else
			Set_ScreenValue(pObjInfo);

		Safe_Delete(pObjInfo);
		CloseHandle(hFile);
	}

}


void CTerrainTool::OnBnClickedTerrainToolCreatePrototype()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		if (m_pTerrain == nullptr)
			MSG_VOID("CTerrainTool OnBnClickedTerrainToolCreatePrototype - m_pTerrain is null");
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	
			_ulong dwNum;
			OBJ_INFO tInfo = *m_pTerrain->Get_ObjInfo();
	
			WriteFile(hFile, &tInfo, sizeof(OBJ_INFO), &dwNum, nullptr);
	
			CloseHandle(hFile);
		}

}


void CTerrainTool::OnBnClickedButtonTerrainToolWireFrame()
{
		if (m_pTerrain == nullptr)
			MSG_VOID("CTerrainTool OnBnClickedButtonTerrainToolWireFrame_OnOffWireFrame - m_pTerrain is null");
	
		m_pTerrain->Set_WireFrame();
		Render_Tool();
}
