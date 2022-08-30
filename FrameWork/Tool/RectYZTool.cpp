// RectYZTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "RectYZTool.h"
#include "afxdialogex.h"
#include "ToolViewCamera.h"
#include "RectYZObj.h"
#include "RectXYObj.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "FilePath.h"

// CRectYZTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRectYZTool, CDialog)

CRectYZTool::CRectYZTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_RECTYZTOOL, pParent)
	, m_strObjName(_T(""))
	, m_szTextureName(_T(""))
{

}

CRectYZTool::~CRectYZTool()
{
	Free();
}

BOOL CRectYZTool::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_RadioRectDir[0].SetCheck(true);
	m_RadioSide[1].SetCheck(true);
	

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CToolView* pToolView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	m_pDevice = pToolView->Clone_Device();
	m_pRenderer = pToolView->Clone_Renderer();
	m_pRectYZ = CRectYZObj::Create(m_pDevice);

	m_pCamera = CToolViewCamera::Create(m_pDevice);
	m_pCamera->Sync_Window(this->m_hWnd);
	_vec3 vCameraLook = _vec3(0.f, 0.f, 0.f);
	m_pCamera->SetUp_InitialState(_vec3(10.f, 10.f, -20.f), vCameraLook);

	_vec3 vPos = m_pCamera->Get_CameraPos();
	_float fFov = m_pCamera->Get_CameraFov();

	_stprintf_s(m_szPos, TEXT("X: %f \nY: %f \nZ: %f \nFov : %f"), vPos.x, vPos.y, vPos.z, fFov);

	Load_Texture();
	Set_SliderInfo();
	Set_ChangeValues();

	return true;
}

void CRectYZTool::Set_SliderInfo()
{
	m_cSliderAlpha.SetRange(0, 255,TRUE);
	m_cSliderAlpha.SetPos(255);
	m_cSliderAlpha.SetLineSize(1);
	
	m_cSliderRed.SetRange(0, 255, TRUE);
	m_cSliderRed.SetPos(255);
	m_cSliderRed.SetLineSize(1);

	m_cSliderGreen.SetRange(0, 255, TRUE);
	m_cSliderGreen.SetPos(255);
	m_cSliderGreen.SetLineSize(1);

	m_cSliderBlue.SetRange(0, 255, TRUE);
	m_cSliderBlue.SetPos(255);
	m_cSliderBlue.SetLineSize(1);
}

void CRectYZTool::Show_MetheSliderValues()
{
	CString szValue;
	int iVal = 0;
	iVal = m_cSliderAlpha.GetPos();
	szValue.Format(_T("%d"), iVal);
	m_editAlpha.SetWindowText(szValue);

	iVal = m_cSliderRed.GetPos();
	szValue.Format(_T("%d"), iVal);
	m_editRed.SetWindowText(szValue);

	iVal = m_cSliderGreen.GetPos();
	szValue.Format(_T("%d"), iVal);
	m_editGreen.SetWindowText(szValue);

	iVal = m_cSliderBlue.GetPos();
	szValue.Format(_T("%d"), iVal);
	m_editBlue.SetWindowText(szValue);

}

HRESULT CRectYZTool::Set_ChangeValues()
{
	UpdateData(TRUE);
	OBJ_INFO* pObjInfo = new OBJ_INFO;

	if (m_RadioRectDir[0].GetCheck())
	{
		pObjInfo->eVIBufferType = OBJ_INFO::VI_BUFFER_TYPE_RECT_YZ;

		if (m_pRectXY != nullptr)
			Safe_Release(m_pRectXY);

		if (m_pRectYZ == nullptr)
			m_pRectYZ = CRectYZObj::Create(m_pDevice);

		if (m_RadioLook[0].GetCheck())
			m_RadioLook[0].SetCheck(FALSE);
		if (m_RadioLook[1].GetCheck())
			m_RadioLook[1].SetCheck(FALSE);
		if (!m_RadioSide[0].GetCheck() && !m_RadioSide[1].GetCheck())
			m_RadioSide[1].SetCheck(TRUE);

		if (m_RadioSide[0].GetCheck())
			pObjInfo->eWallSide = CVIBufferRectYZ::WS_LEFT;
		else if (m_RadioSide[1].GetCheck())
			pObjInfo->eWallSide = CVIBufferRectYZ::WS_RIGHT;
		else
			pObjInfo->eWallSide = CVIBufferRectYZ::WS_END;



		pObjInfo->fSizeX = 1;
		pObjInfo->fSizeY = m_fYInterval;
		pObjInfo->fSizeZ = m_fZInterval;

		pObjInfo->iNumVerticesX = 0;
		pObjInfo->iNumVerticesY = m_iVerticesY;
		pObjInfo->iNumVerticesZ = m_iVerticesZ;

		pObjInfo->fU = m_fU;
		pObjInfo->fV = m_fV;

		pObjInfo->vColliderCenter.x = m_fColliderCenterX;
		pObjInfo->vColliderCenter.y = m_fColliderCenterY;
		pObjInfo->vColliderCenter.z = m_fColliderCenterZ;
		pObjInfo->vColliderSize.x = m_fColliderSizeX;
		pObjInfo->vColliderSize.y = m_fColliderSizeY;
		pObjInfo->vColliderSize.z = m_fColliderSizeZ;

		Show_MetheSliderValues();
		pObjInfo->dwColliderColor = D3DCOLOR_ARGB(m_cSliderAlpha.GetPos(), m_cSliderRed.GetPos(), m_cSliderGreen.GetPos(), m_cSliderBlue.GetPos());

		pObjInfo->vRot = _vec3(m_fRotX, m_fRotY, m_fRotZ);
		pObjInfo->SetUp_State(OBJ_INFO::STATE_POS, _vec3(m_fPosX, m_fPosY, m_fPosZ));
		pObjInfo->SetUp_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);


		lstrcpy(pObjInfo->szObjName, m_strObjName.GetString());
		lstrcpy(pObjInfo->szTextureName, m_szTextureName.GetString());

		

		if (m_iVerticesY > 1 && m_iVerticesZ > 1)
			m_pRectYZ->Set_RectYZBuffer(pObjInfo);
	}
	else if (m_RadioRectDir[1].GetCheck())
	{
		pObjInfo->eVIBufferType = OBJ_INFO::VI_BUFFER_TYPE_RECT_XY;

		if (m_pRectYZ != nullptr)
			Safe_Release(m_pRectYZ);

		if (m_pRectXY == nullptr)
			m_pRectXY = CRectXYObj::Create(m_pDevice);

		if (m_RadioSide[0].GetCheck())
			m_RadioSide[0].SetCheck(FALSE);
		if (m_RadioSide[1].GetCheck())
			m_RadioSide[1].SetCheck(FALSE);

		if (!m_RadioLook[0].GetCheck() && !m_RadioLook[1].GetCheck())
			m_RadioLook[0].SetCheck(TRUE);

		if (m_RadioLook[0].GetCheck())
			pObjInfo->eWallLook = CVIBufferRectXY::WL_FRONT;
		else if (m_RadioLook[1].GetCheck())
			pObjInfo->eWallLook = CVIBufferRectXY::WL_BACK;
		else
			pObjInfo->eWallLook = CVIBufferRectXY::WL_END;


		pObjInfo->fSizeX = m_fXInterval;
		pObjInfo->fSizeY = m_fYInterval;
		pObjInfo->fSizeZ = 1;

		pObjInfo->iNumVerticesX = m_iVerticesX;
		pObjInfo->iNumVerticesY = m_iVerticesY;
		pObjInfo->iNumVerticesZ = 0;

		pObjInfo->fU = m_fU;
		pObjInfo->fV = m_fV;

		pObjInfo->vColliderCenter.x = m_fColliderCenterX;
		pObjInfo->vColliderCenter.y = m_fColliderCenterY;
		pObjInfo->vColliderCenter.z = m_fColliderCenterZ;
		pObjInfo->vColliderSize.x = m_fColliderSizeX;
		pObjInfo->vColliderSize.y = m_fColliderSizeY;
		pObjInfo->vColliderSize.z = m_fColliderSizeZ;

		Show_MetheSliderValues();
		pObjInfo->dwColliderColor = D3DCOLOR_ARGB(m_cSliderAlpha.GetPos(), m_cSliderRed.GetPos(), m_cSliderGreen.GetPos(), m_cSliderBlue.GetPos());

		pObjInfo->vRot = _vec3(m_fRotX, m_fRotY, m_fRotZ);
		pObjInfo->SetUp_State(OBJ_INFO::STATE_POS, _vec3(m_fPosX, m_fPosY, m_fPosZ));
		pObjInfo->SetUp_Scale(m_fScaleX, m_fScaleY, m_fScaleZ);


		lstrcpy(pObjInfo->szObjName, m_strObjName.GetString());
		lstrcpy(pObjInfo->szTextureName, m_szTextureName.GetString());

		if (m_iVerticesX > 1 && m_iVerticesY > 1)
			m_pRectXY->Set_RectXYBuffer(pObjInfo);
	}
	Render_Picture();
	UpdateData(FALSE);
	return S_OK;
}

HRESULT CRectYZTool::Set_LoadValue(OBJ_INFO * pObjInfo)
{
	UpdateData(TRUE);

	_vec3 vPos = pObjInfo->Get_State(OBJ_INFO::STATE_POS);
	m_fPosX = vPos.x;
	m_fPosY = vPos.y;
	m_fPosZ = vPos.z;

	_vec3 vRot = pObjInfo->vRot;
	m_fRotX = vRot.x;
	m_fRotY = vRot.y;
	m_fRotZ = vRot.z;

	_vec3 vScale = pObjInfo->Get_Scale();
	m_fScaleX = vScale.x;
	m_fScaleY = vScale.y;
	m_fScaleZ = vScale.z;

	m_fXInterval = pObjInfo->fSizeX;
	m_fYInterval = pObjInfo->fSizeY;
	m_fZInterval = pObjInfo->fSizeZ;

	m_fU = pObjInfo->fU;
	m_fV = pObjInfo->fV;

	m_iVerticesX = pObjInfo->iNumVerticesX;
	m_iVerticesY = pObjInfo->iNumVerticesY;
	m_iVerticesZ = pObjInfo->iNumVerticesZ;

	m_strObjName = pObjInfo->szObjName;
	m_szTextureName = pObjInfo->szTextureName;

	m_fColliderCenterX = pObjInfo->vColliderCenter.x;
	m_fColliderCenterY = pObjInfo->vColliderCenter.y;
	m_fColliderCenterZ = pObjInfo->vColliderCenter.z;

	m_fColliderSizeX = pObjInfo->vColliderSize.x;
	m_fColliderSizeY = pObjInfo->vColliderSize.y;
	m_fColliderSizeZ = pObjInfo->vColliderSize.z;


	m_listTexture.SetCurSel(pObjInfo->iTextureIndex);

	

	UpdateData(FALSE);

	Set_ChangeValues();

	return S_OK;
}

HRESULT CRectYZTool::Load_Texture()
{
	wstring wstrFilePath = L"../Client/bin/Resource/Texture/Wall/*.*";
	CFileFind fileFind;
	BOOL bContinue = fileFind.FindFile(wstrFilePath.c_str());
	while (bContinue)
	{
		bContinue = fileFind.FindNextFile();
		if (fileFind.IsDots() || fileFind.IsDirectory())
			continue;
		else
		{
			if (fileFind.IsSystem())
				continue;
			Add_TextureString(m_listTexture, fileFind.GetFileTitle());
		}
	}

	return S_OK;
}

HRESULT CRectYZTool::Add_TextureString(CListBox & listBox, CString wstr)
{
	UpdateData(TRUE);

	if (0 < listBox.FindString(0, wstr))
		MSG_FAIL("CRectYZTool Add_TextureString - 중복된 스트링");

	listBox.AddString(wstr);

	UpdateData(FALSE);
	return S_OK;
}

HRESULT CRectYZTool::Render_Picture()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	pInstance->Render_Begin();

	pInstance->Tick_ToolEngine(0.1f);

	_vec3 vPos = m_pCamera->Get_CameraPos();
	_float fFov = m_pCamera->Get_CameraFov();
	_stprintf_s(m_szPos, TEXT("X: %f \nY: %f \nZ: %f \nFov : %f"), vPos.x, vPos.y, vPos.z, fFov);
	m_pCamera->Tick(0.1f);

	pInstance->Draw_Font(m_szPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	if (m_pRectXY != nullptr)
		m_pRectXY->Render();
	if (m_pRectYZ != nullptr)
		m_pRectYZ->Render();

	pInstance->Render_End(m_PicWall.m_hWnd);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

void CRectYZTool::Set_ChangeSliderValues()
{
	m_cSliderAlpha.GetPos();
}

void CRectYZTool::Free()
{
	if (m_pCamera != nullptr)
		if (m_pCamera->Get_CameraAction())
			m_pCamera->Do_CameraAction();

	Safe_Release(m_pRectYZ);
	Safe_Release(m_pRectXY);
	Safe_Release(m_pCamera);
	Safe_Release(m_pDevice);
	Safe_Release(m_pRenderer);
}


void CRectYZTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WALL_EDIT_U, m_fU);
	DDV_MinMaxFloat(pDX, m_fU, 0, 100);
	DDX_Text(pDX, IDC_WALL_EDIT_V, m_fV);
	DDV_MinMaxFloat(pDX, m_fV, 0, 100);
	DDX_Text(pDX, IDC_WALL_EDIT_Z_INT, m_fZInterval);
	DDX_Text(pDX, IDC_WALL_EDIT_V_INT, m_fYInterval);
	DDX_Text(pDX, IDC_WALL_EDIT_Z_VERT, m_iVerticesZ);
	DDX_Text(pDX, IDC_WALL_EDIT_Y_VERT, m_iVerticesY);
	DDX_Control(pDX, IDC_WALL_LIST_TEXTURE, m_listTexture);
	DDX_Text(pDX, IDC_WALL_EDIT_POS_X, m_fPosX);
	DDX_Text(pDX, IDC_WALL_EDIT_POS_Y, m_fPosY);
	DDX_Text(pDX, IDC_WALL_EDIT_POS_Z, m_fPosZ);
	DDX_Text(pDX, IDC_WALL_EDIT_ROT_X, m_fRotX);
	DDX_Text(pDX, IDC_WALL_EDIT_ROT_Y, m_fRotY);
	DDX_Text(pDX, IDC_WALL_EDIT_ROT_Z, m_fRotZ);
	DDX_Text(pDX, IDC_WALL_EDIT_SCALE_X, m_fScaleX);
	DDX_Text(pDX, IDC_WALL_EDIT_SCALE_Y, m_fScaleY);
	DDX_Text(pDX, IDC_WALL_EDIT_SCALE_Z, m_fScaleZ);
	DDX_Control(pDX, IDC_WALL_RADIO_LEFT, m_RadioSide[0]);
	DDX_Control(pDX, IDC_WALL_RADIO_RIGHT, m_RadioSide[1]);
	DDX_Control(pDX, IDC_WALL_PICTURE, m_PicWall);
	DDX_Text(pDX, IDC_WALL_EDIT_String_Name, m_strObjName);

	DDX_Text(pDX, IDC_WALL_EDIT_COLLIDER_POS_X, m_fColliderCenterX);
	DDX_Text(pDX, IDC_WALL_EDIT_COLLIDER_POS_Y, m_fColliderCenterY);
	DDX_Text(pDX, IDC_WALL_EDIT_COLLIDER_POS_Z, m_fColliderCenterZ);
	DDX_Text(pDX, IDC_WALL_EDIT_COLLIDER_SIZE_X, m_fColliderSizeX);
	DDX_Text(pDX, IDC_WALL_EDIT_COLLIDER_SIZE_Y, m_fColliderSizeY);
	DDX_Text(pDX, IDC_WALL_EDIT_COLLIDER_SIZE_Z, m_fColliderSizeZ);
	DDX_Control(pDX, IDC_WALL_SLIDER_ALPHA, m_cSliderAlpha);
	DDX_Control(pDX, IDC_WALL_SLIDER_RED, m_cSliderRed);
	DDX_Control(pDX, IDC_WALL_SLIDER_GREEN, m_cSliderGreen);
	DDX_Control(pDX, IDC_WALL_SLIDER_BLUE, m_cSliderBlue);
	DDX_Control(pDX, IDC_WALL_EDIT_COLLIDER_ALPHA, m_editAlpha);
	DDX_Control(pDX, IDC_WALL_EDIT_COLLIDER_RED, m_editRed);
	DDX_Control(pDX, IDC_WALL_EDIT_COLLIDER_GREEN, m_editGreen);
	DDX_Control(pDX, IDC_WALL_EDIT_COLLIDER_BLUE, m_editBlue);
	DDX_Text(pDX, IDC_WALL_EDIT_STRING_TEXTURE, m_szTextureName);
	DDX_Control(pDX, IDC_WALL_RADIO_BUFFER_TYPE_YZ, m_RadioRectDir[0]);
	DDX_Control(pDX, IDC_WALL_RADIO_BUFFER_TYPE_XY, m_RadioRectDir[1]);
	DDX_Control(pDX, IDC_WALL_RADIO_FRONT, m_RadioLook[0]);
	DDX_Control(pDX, IDC_WALL_RADIO_BACK, m_RadioLook[1]);

	DDX_Text(pDX, IDC_WALL_EDIT_X_INT, m_fXInterval);
	DDX_Text(pDX, IDC_WALL_EDIT_X_VERT, m_iVerticesX);
}


BEGIN_MESSAGE_MAP(CRectYZTool, CDialog)

	ON_EN_CHANGE(IDC_WALL_EDIT_U, &CRectYZTool::OnEnChange_UV_U)
	ON_EN_CHANGE(IDC_WALL_EDIT_V, &CRectYZTool::OnEnChange_UV_V)
	ON_EN_CHANGE(IDC_WALL_EDIT_Z_INT, &CRectYZTool::OnEnChange_INTERVAL_Z)
	ON_EN_CHANGE(IDC_WALL_EDIT_V_INT, &CRectYZTool::OnEnChange_INTERVAL_Y)
	ON_EN_CHANGE(IDC_WALL_EDIT_Z_VERT, &CRectYZTool::OnEnChange_Vertices_Y)
	ON_EN_CHANGE(IDC_WALL_EDIT_Y_VERT, &CRectYZTool::OnEnChange_Vertices_Z)
	ON_EN_CHANGE(IDC_WALL_EDIT_POS_X, &CRectYZTool::OnEnChange_Position_X)
	ON_EN_CHANGE(IDC_WALL_EDIT_POS_Y, &CRectYZTool::OnEnChange_Position_Y)
	ON_EN_CHANGE(IDC_WALL_EDIT_POS_Z, &CRectYZTool::OnEnChange_Position_Z)
	ON_EN_CHANGE(IDC_WALL_EDIT_SCALE_X, &CRectYZTool::OnEnChange_Scale_X)
	ON_EN_CHANGE(IDC_WALL_EDIT_SCALE_Y, &CRectYZTool::OnEnChange_Scale_Y)
	ON_EN_CHANGE(IDC_WALL_EDIT_SCALE_Z, &CRectYZTool::OnEnChange_Scale_Z)
	ON_WM_MBUTTONDOWN()
	ON_WM_TIMER()
	ON_LBN_SELCHANGE(IDC_WALL_LIST_TEXTURE, &CRectYZTool::OnLbnSelchange_TextureList)
	ON_BN_CLICKED(IDC_WALL_BUTTON_WIREFRAME, &CRectYZTool::OnBnClicked_WireFrame_Render)
	ON_BN_CLICKED(IDC_WALL_BUTTON_SAVE, &CRectYZTool::OnBnClicked_Save_Data)
	ON_BN_CLICKED(IDC_WALL_BUTTON_LOAD, &CRectYZTool::OnBnClicked_Load_Data)
	ON_BN_CLICKED(IDC_WALL_RADIO_LEFT, &CRectYZTool::OnBnClickedWallRadioLeft)
	ON_BN_CLICKED(IDC_WALL_RADIO_RIGHT, &CRectYZTool::OnBnClickedWallRadioRight)
	ON_EN_CHANGE(IDC_WALL_EDIT_COLLIDER_POS_X, &CRectYZTool::OnEnChangeWallEditColliderPosX)
	ON_EN_CHANGE(IDC_WALL_EDIT_COLLIDER_POS_Y, &CRectYZTool::OnEnChangeWallEditColliderPosY)
	ON_EN_CHANGE(IDC_WALL_EDIT_COLLIDER_POS_Z, &CRectYZTool::OnEnChangeWallEditColliderPosZ)
	ON_EN_CHANGE(IDC_WALL_EDIT_COLLIDER_SIZE_X, &CRectYZTool::OnEnChangeWallEditColliderSizeX)
	ON_EN_CHANGE(IDC_WALL_EDIT_COLLIDER_SIZE_Y, &CRectYZTool::OnEnChangeWallEditColliderSizeY)
	ON_EN_CHANGE(IDC_WALL_EDIT_COLLIDER_SIZE_Z, &CRectYZTool::OnEnChangeWallEditColliderSizeZ)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_WALL_SLIDER_ALPHA, &CRectYZTool::OnNMCustomdrawWallSliderAlpha)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_WALL_SLIDER_RED, &CRectYZTool::OnNMCustomdrawWallSliderRed)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_WALL_SLIDER_GREEN, &CRectYZTool::OnNMCustomdrawWallSliderGreen)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_WALL_SLIDER_BLUE, &CRectYZTool::OnNMCustomdrawWallSliderBlue)
	ON_EN_CHANGE(IDC_WALL_EDIT_STRING_TEXTURE, &CRectYZTool::OnEnChangeWallEditStringTexture)
	ON_EN_CHANGE(IDC_WALL_EDIT_String_Name, &CRectYZTool::OnEnChangeWallEditStringName)
	ON_BN_CLICKED(IDC_WALL_RADIO_BUFFER_TYPE_YZ, &CRectYZTool::OnBnClickedWallRadioBufferTypeYz)
	ON_BN_CLICKED(IDC_WALL_RADIO_BUFFER_TYPE_XY, &CRectYZTool::OnBnClickedWallRadioBufferTypeXy)
	ON_BN_CLICKED(IDC_WALL_RADIO_FRONT, &CRectYZTool::OnBnClickedWallRadioFront)
	ON_BN_CLICKED(IDC_WALL_RADIO_BACK, &CRectYZTool::OnBnClickedWallRadioBack)
	ON_EN_CHANGE(IDC_WALL_EDIT_X_INT, &CRectYZTool::OnEnChangeWallEditXInt)
	ON_EN_CHANGE(IDC_WALL_EDIT_X_VERT, &CRectYZTool::OnEnChangeWallEditXVert)
END_MESSAGE_MAP()


// CRectYZTool 메시지 처리기입니다.




void CRectYZTool::OnEnChange_UV_U()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChange_UV_V()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChange_INTERVAL_Z()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChange_INTERVAL_Y()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChange_Vertices_Y()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChange_Vertices_Z()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChange_Position_X()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChange_Position_Y()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChange_Position_Z()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChange_Scale_X()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChange_Scale_Y()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChange_Scale_Z()
{
	Set_ChangeValues();
}


void CRectYZTool::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_pCamera->Do_CameraAction();
	if (m_pCamera->Get_CameraAction())
		SetTimer(0, 100, NULL);
	else
		KillTimer(0);

	Render_Picture();

	CDialog::OnMButtonDown(nFlags, point);
}


void CRectYZTool::OnTimer(UINT_PTR nIDEvent)
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);


	switch (nIDEvent)
	{
	case 0:
		Render_Picture();
	default:
		break;
	}
	RELEASE_INSTANCE(CGameInstance);


	CDialog::OnTimer(nIDEvent);
}


void CRectYZTool::OnLbnSelchange_TextureList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iIndex = m_listTexture.GetCurSel();
	if (iIndex < 0)
		return;

	if (m_pRectYZ != nullptr)
	{
		m_pRectYZ->Set_TextureIndex(iIndex);
		m_pRectYZ->Set_ObjInfoTextureIndex(iIndex);
	}

	if (m_pRectXY != nullptr)
	{
		m_pRectXY->Set_TextureIndex(iIndex);
		m_pRectXY->Set_ObjInfoTextureIndex(iIndex);
	}
	Render_Picture();
}


void CRectYZTool::OnBnClicked_WireFrame_Render()
{
	if (m_pRectYZ != nullptr)
		m_pRectYZ->Do_WireFrameRendering();
	if (m_pRectXY != nullptr)
		m_pRectXY->Do_WireFrameRendering();
	Render_Picture();
}


void CRectYZTool::OnBnClicked_Save_Data()
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

		_ulong dwByte;
		CString wstrTerrainName = CFilePath::Get_FileNameWithoutExtension(wstrPath);

		//렉트 yz가 널이면 xy로 저장
		if (m_pRectYZ == nullptr)
		{
			m_pRectXY->Set_ObjName(wstrTerrainName);
			WriteFile(hFile, m_pRectXY->Get_ObjInfo(), sizeof(OBJ_INFO), &dwByte, nullptr);
		}
		//아니면 yz로 저장
		else
		{
			m_pRectYZ->Set_ObjName(wstrTerrainName);
			WriteFile(hFile, m_pRectYZ->Get_ObjInfo(), sizeof(OBJ_INFO), &dwByte, nullptr);
		}
		
		if (dwByte <= 0)
			MSGBOX("CRectYZTool OnBnClicked_Save_Data - Failed to Save Data");

		CloseHandle(hFile);
	}
	return;
}


void CRectYZTool::OnBnClicked_Load_Data()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE, L"dat", L"*.dat");
	TCHAR szBuf[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szBuf);
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\Client\\bin\\Resource\\Data\\PrototypeObject");
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
			MSGBOX("CRectYZTool OnBnClicked_Load_Data - Failed to Load Data");
		else
			Set_LoadValue(pObjInfo);

		Safe_Delete(pObjInfo);
		CloseHandle(hFile);
	}
}


void CRectYZTool::OnBnClickedWallRadioLeft()
{
	if (FAILED(Set_ChangeValues()))
		MSG_VOID("CRectYZTool::OnBnClickedWallRadioLeft - Set_ChangeValues");
}


void CRectYZTool::OnBnClickedWallRadioRight()
{
	if (FAILED(Set_ChangeValues()))
		MSG_VOID("CRectYZTool::OnBnClickedWallRadioRight - Set_ChangeValues");
}


void CRectYZTool::OnEnChangeWallEditColliderPosX()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChangeWallEditColliderPosY()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChangeWallEditColliderPosZ()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChangeWallEditColliderSizeX()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChangeWallEditColliderSizeY()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChangeWallEditColliderSizeZ()
{
	Set_ChangeValues();
}


void CRectYZTool::OnNMCustomdrawWallSliderAlpha(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	Set_ChangeValues();
	*pResult = 0;
}


void CRectYZTool::OnNMCustomdrawWallSliderRed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	Set_ChangeValues();
	*pResult = 0;
}


void CRectYZTool::OnNMCustomdrawWallSliderGreen(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	Set_ChangeValues();
	*pResult = 0;
}


void CRectYZTool::OnNMCustomdrawWallSliderBlue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	Set_ChangeValues();
	*pResult = 0;
}


void CRectYZTool::OnEnChangeWallEditStringTexture()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChangeWallEditStringName()
{
	Set_ChangeValues();
}


void CRectYZTool::OnBnClickedWallRadioBufferTypeYz()
{
	Set_ChangeValues();
}


void CRectYZTool::OnBnClickedWallRadioBufferTypeXy()
{
	Set_ChangeValues();
}


void CRectYZTool::OnBnClickedWallRadioFront()
{
	Set_ChangeValues();
}


void CRectYZTool::OnBnClickedWallRadioBack()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChangeWallEditXInt()
{
	Set_ChangeValues();
}


void CRectYZTool::OnEnChangeWallEditXVert()
{
	Set_ChangeValues();
}
