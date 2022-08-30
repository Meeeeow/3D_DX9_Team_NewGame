// PrototypeTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PrototypeTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Graphic_Device.h"
#include "ToolViewCamera.h"
#include "Transform.h"
#include "Renderer.h"
#include "RectObj.h"
#include "CubeObj.h"
#include "FilePath.h"


// CPrototypeTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPrototypeTool, CDialog)

CPrototypeTool::CPrototypeTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PROTOTYPETOOL, pParent)

	, m_fSizeX(1.f)
	, m_fSizeY(1.f)
	, m_fSizeZ(1.f)
	, m_iPreRotX(0)
	, m_iPreRotY(0)	
	, m_iPreRotZ(0)

	, m_fPosX(0)
	, m_fPosY(0)
	, m_fPosZ(0)
{
	ZeroMemory(m_pBaseLineArr, sizeof(m_pBaseLineArr));
}

CPrototypeTool::~CPrototypeTool()
{
	Free();
}

void CPrototypeTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROTOTYPE_PICTURE, m_Picture);
	DDX_Control(pDX, IDC_RADIO_PROTOTYPE_RECT, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO_PROTOTYPE_CUBE, m_Radio[1]);
	DDX_Control(pDX, IDC_PROTOTYPE_IMAGE_LIST, m_ListBox);
	DDX_Control(pDX, IDC_PROTOTYPE_ROT_SLIDER_X, m_sldAxisX);
	DDX_Control(pDX, IDC_PROTOTYPE_ROT_SLIDER_Y, m_sldAxisY);
	DDX_Control(pDX, IDC_PROTOTYPE_ROT_SLIDER_Z, m_sldAxisZ);
	DDX_Control(pDX, IDC_PROTOTYPE_ROT_X, m_editAixsX);
	DDX_Control(pDX, IDC_PROTOTYPE_ROT_Y, m_editAxisY);
	DDX_Control(pDX, IDC_PROTOTYPE_ROT_Z, m_editAxisZ);
	DDX_Text(pDX, IDC_PROTOTYPE_SIZE_X, m_fSizeX);
	DDX_Text(pDX, IDC_PROTOTYPE_SIZE_Y, m_fSizeY);
	DDX_Text(pDX, IDC_PROTOTYPE_SIZE_Z, m_fSizeZ);
	DDX_Text(pDX, IDC_PROTOTYPE_POS_X, m_fPosX);
	DDX_Text(pDX, IDC_PROTOTYPE_POS_Y, m_fPosY);
	DDX_Text(pDX, IDC_PROTOTYPE_POS_X, m_fPosZ);
	DDX_Text(pDX, IDC_PROTOTYPE_EDIT_COLLIDER_POS_X, m_fColliderCenterX);
	DDX_Text(pDX, IDC_PROTOTYPE_EDIT_COLLIDER_POS_Y, m_fColliderCenterY);
	DDX_Text(pDX, IDC_PROTOTYPE_EDIT_COLLIDER_POS_Z, m_fColliderCenterZ);
	DDX_Text(pDX, IDC_PROTOTYPE_EDIT_COLLIDER_SIZE_X, m_fColliderSizerX);
	DDX_Text(pDX, IDC_PROTOTYPE_EDIT_COLLIDER_SIZE_Y, m_fColliderSizerY);
	DDX_Text(pDX, IDC_PROTOTYPE_EDIT_COLLIDER_SIZE_Z, m_fColliderSizerZ);
	DDX_Control(pDX, IDC_PROTOTYPE_EDIT_COLLIDER_ALPHA, m_editAlpha);
	DDX_Control(pDX, IDC_PROTOTYPE_EDIT_COLLIDER_RED, m_editRed);
	DDX_Control(pDX, IDC_PROTOTYPE_EDIT_COLLIDER_GREEN, m_editGreen);
	DDX_Control(pDX, IDC_PROTOTYPE_EDIT_COLLIDER_BLUE, m_editBlue);
	DDX_Control(pDX, IDC_PROTOTYPE_SLIDER_ALPHA, m_cSliderAlpha);
	DDX_Control(pDX, IDC_PROTOTYPE_SLIDER_RED, m_cSliderRed);
	DDX_Control(pDX, IDC_PROTOTYPE_SLIDER_GREEN, m_cSliderGreen);
	DDX_Control(pDX, IDC_PROTOTYPE_SLIDER_BLUE, m_cSliderBlue);
}

void CPrototypeTool::Free()
{
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pCamera);
	

	for (auto& pBaseLine : m_pBaseLineArr)
		Safe_Release(pBaseLine);
}

// 버전 업데이트용 아무나 보면 이 주석을 지우시오 2

void CPrototypeTool::Initialize_DialogSeting() // 다이얼로그 초기화
{
	m_sldAxisX.SetRange(0, 360); // 최소, 최대값
	m_sldAxisX.SetPos(0); // 현재 위치

	m_sldAxisY.SetRange(0, 360); // 최소, 최대값
	m_sldAxisY.SetPos(0); // 현재 위치

	m_sldAxisZ.SetRange(0, 360); // 최소, 최대값
	m_sldAxisZ.SetPos(0); // 현재 위치

	//m_sldAxisX.GetPos(); 스크롤의 값을 받아옴

	// 에딧박스에 초기값 채움
	int		iPos = 0;// 
	CString sPos;
	sPos.Format(_T("%d"), iPos);

	// 회전
	m_editAixsX.SetWindowText(sPos);
	m_editAxisY.SetWindowText(sPos);
	m_editAxisZ.SetWindowText(sPos);

	m_iPreRotX = 0;
	m_iPreRotY = 0;
	m_iPreRotZ = 0;

	// 사이즈
	m_fSizeX = 1.f;
	m_fSizeY = 1.f;
	m_fSizeZ = 1.f;

	m_fPosX = 0.f;
	m_fPosY = 0.f;
	m_fPosZ = 0.f;

	Set_ColliderInfo();
}

void CPrototypeTool::SetUp_RectObjectInfo() // 프로토타입 툴 초기화시 
{
	// 비어 있으면 생성
	if(nullptr == m_pObjInfo)
		m_pObjInfo = new OBJ_INFO;
	// 오브젝트 타입
	m_pObjInfo->eVIBufferType = OBJ_INFO::VI_BUFFER_TYPE_RECT;
	// 객체 이름 
	memcpy(m_pObjInfo->szObjName, TEXT("None"), sizeof(_tchar)*OBJ_NAME);
	// 텍스쳐 컴포넌트 (머지 할때 변경)
	memcpy(m_pObjInfo->szTextureName, TEXT("PrototypeTextureRect"), sizeof(_tchar)*OBJ_NAME);
	// 텍스쳐 번호
	m_pObjInfo->iTextureIndex = 0;
	// 사이즈 설정
	m_pObjInfo->fSizeX = m_fSizeX;
	m_pObjInfo->fSizeY = m_fSizeY;
	m_pObjInfo->fSizeZ = m_fSizeZ;
	// 객체 생성
	Create_Object(m_pObjInfo);
}

void CPrototypeTool::SetUp_CubeObjectInfo()
{
	// 비어 있으면 생성
	if (nullptr == m_pObjInfo)
		m_pObjInfo = new OBJ_INFO;
	// 오브젝트 타입
	m_pObjInfo->eVIBufferType = OBJ_INFO::VI_BUFFER_TYPE_CUBE;
	// 객체 이름 
	memcpy(m_pObjInfo->szObjName, TEXT("Ex1"), sizeof(_tchar)*OBJ_NAME);
	// 텍스쳐 컴포넌트 (머지 할때 변경)
	memcpy(m_pObjInfo->szTextureName, TEXT("PrototypeTextureCube"), sizeof(_tchar)*OBJ_NAME);
	// 텍스쳐 번호
	m_pObjInfo->iTextureIndex = 0;
	// 사이즈 설정
	m_pObjInfo->fSizeX = m_fSizeX;
	m_pObjInfo->fSizeY = m_fSizeY;
	m_pObjInfo->fSizeZ = m_fSizeZ;
	// 객체 생성
	Create_Object(m_pObjInfo);
}

void CPrototypeTool::Create_Object(OBJ_INFO * pInfo) // 오브젝트 생성
{
	OBJ_INFO* pObjInfo = nullptr;
	// 버퍼 변경시 삭제
	if (nullptr != m_pObject)
	{
		pObjInfo = new OBJ_INFO;
		memcpy(pObjInfo, pInfo, sizeof(OBJ_INFO));
		Safe_Release(m_pObject);
		pInfo = pObjInfo;
		m_pObjInfo = pInfo;
	}

	// enum 값에 따라 객체 생성
	switch (pInfo->eVIBufferType)
	{
	case OBJ_INFO::VI_BUFFER_TYPE_RECT:
		m_pObject = CRectObj::Create(m_pGraphic_Device, pInfo);
		break;
	case OBJ_INFO::VI_BUFFER_TYPE_CUBE:
		m_pObject = CCubeObj::Create(m_pGraphic_Device, pInfo);
		break;
	}

}

HRESULT CPrototypeTool::UpDate_ListBoxToRect() // 렉트 이미지로 리스트 업데이트
{
	// 객체 추가시 번호에 맞게 객체이름을 넣어으면 리스트에서 사용가능
	// * 순서 안맞으면 정렬이 켜져있는지 확인하고 끌것

	// 리스트 목록 초기화
	m_ListBox.ResetContent();

	_tchar szBuff[MAX_PATH]{};

	// 객체의 이름 삽입 // 리스트에 추가

	//0
	swprintf_s(szBuff, MAX_PATH, TEXT("None"));
	m_ListBox.AddString(szBuff);
	//1
	swprintf_s(szBuff, MAX_PATH, TEXT("Player"));
	m_ListBox.AddString(szBuff);
	//2
	swprintf_s(szBuff, MAX_PATH, TEXT("PlayerBuffer"));
	m_ListBox.AddString(szBuff);
	//3
	swprintf_s(szBuff, MAX_PATH, TEXT("BossSkeleton"));
	m_ListBox.AddString(szBuff);
	//4
	swprintf_s(szBuff, MAX_PATH, TEXT("Necromencer"));
	m_ListBox.AddString(szBuff);
	//5
	swprintf_s(szBuff, MAX_PATH, TEXT("NecromencerBuffer"));
	m_ListBox.AddString(szBuff);
	//6
	swprintf_s(szBuff, MAX_PATH, TEXT("MeleeSkull"));
	m_ListBox.AddString(szBuff);
	//7
	swprintf_s(szBuff, MAX_PATH, TEXT("MeleeSkullBuffer"));
	m_ListBox.AddString(szBuff);
	//8
	swprintf_s(szBuff, MAX_PATH, TEXT("RangeSkull"));
	m_ListBox.AddString(szBuff);
	//9
	swprintf_s(szBuff, MAX_PATH, TEXT("RangeSkullBuffer"));
	m_ListBox.AddString(szBuff);
	//10
	swprintf_s(szBuff, MAX_PATH, TEXT("Chimera"));
	m_ListBox.AddString(szBuff);
	//11
	swprintf_s(szBuff, MAX_PATH, TEXT("ChimeraBuffer"));
	m_ListBox.AddString(szBuff);
	//12
	swprintf_s(szBuff, MAX_PATH, pTagPrototypeMonsterNormalRange);
	m_ListBox.AddString(szBuff);
	//13
	swprintf_s(szBuff, MAX_PATH, pTagPrototypeMonsterNormalMelee);
	m_ListBox.AddString(szBuff);


	return S_OK;
}

HRESULT CPrototypeTool::UpDate_ListBoxToCube() // 큐브 이미지로 리스트 업데이트
{
	// 객체 추가시 번호에 맞게 객체이름을 넣어으면 리스트에서 사용가능
	// * 순서 안맞으면 정렬이 켜져있는지 확인하고 끌것

	// 리스트 목록 초기화
	m_ListBox.ResetContent();

	_tchar szBuff[MAX_PATH]{};

	// 객체의 이름 삽입 // 리스트에 추가
	swprintf_s(szBuff, MAX_PATH, TEXT("Ex1"));
	m_ListBox.AddString(szBuff);

	swprintf_s(szBuff, MAX_PATH, TEXT("Ex2"));
	m_ListBox.AddString(szBuff);

	swprintf_s(szBuff, MAX_PATH, TEXT("Ex3"));
	m_ListBox.AddString(szBuff);

	swprintf_s(szBuff, MAX_PATH, TEXT("Ex4"));
	m_ListBox.AddString(szBuff);

	swprintf_s(szBuff, MAX_PATH, TEXT("Ex5"));
	m_ListBox.AddString(szBuff);

	swprintf_s(szBuff, MAX_PATH, TEXT("Ex6"));
	m_ListBox.AddString(szBuff);

	swprintf_s(szBuff, MAX_PATH, TEXT("Ex7"));
	m_ListBox.AddString(szBuff);

	swprintf_s(szBuff, MAX_PATH, TEXT("Ex8"));
	m_ListBox.AddString(szBuff);

	swprintf_s(szBuff, MAX_PATH, pTagPrototypeBossSkeletonStageCube);
	m_ListBox.AddString(szBuff);

	swprintf_s(szBuff, MAX_PATH, pTagPrototypeCircusCubeLegoRed);
	m_ListBox.AddString(szBuff);

	swprintf_s(szBuff, MAX_PATH, pTagPrototypeCircusCubeLegoBlue);
	m_ListBox.AddString(szBuff);

	swprintf_s(szBuff, MAX_PATH, pTagPrototypeCircusCubeLegoGray);
	m_ListBox.AddString(szBuff);

	swprintf_s(szBuff, MAX_PATH, pTagPrototypeCircusCubeBroken);
	m_ListBox.AddString(szBuff);

	swprintf_s(szBuff, MAX_PATH, TEXT("TombStone"));
	m_ListBox.AddString(szBuff);

	swprintf_s(szBuff, MAX_PATH, TEXT("TombStoneCross"));
	m_ListBox.AddString(szBuff);

	swprintf_s(szBuff, MAX_PATH, TEXT("TombStoneBase"));
	m_ListBox.AddString(szBuff);

	swprintf_s(szBuff, MAX_PATH, pTagPrototypeCaveBloc);
	m_ListBox.AddString(szBuff);

	swprintf_s(szBuff, MAX_PATH, pTagPrototypeCaveDoorDestroyable);
	m_ListBox.AddString(szBuff);

	return S_OK;
}

void CPrototypeTool::Set_PrototypeInfo()
{
	// 사이즈
	UpdateData(TRUE);

	// 사이즈 설정
	_vec3 vSize = { m_fSizeX, m_fSizeY, m_fSizeZ };
	CVIBufferRect::SIZE_POS tSizePos;

	m_pObjInfo->fSizeX = m_fSizeX;
	m_pObjInfo->fSizeY = m_fSizeY;
	m_pObjInfo->fSizeZ = m_fSizeZ;
	tSizePos.vSize = _vec3(m_fSizeX, m_fSizeY, m_fSizeZ);

	m_pObjInfo->vVertexPos.x = m_fPosX;
	m_pObjInfo->vVertexPos.y = m_fPosY;
	m_pObjInfo->vVertexPos.z = m_fPosZ;
	tSizePos.vPos = _vec3(m_fPosX, m_fPosY, m_fPosZ);

	// 객체 타입에 맞는 세팅 호출
	switch (m_pObjInfo->eVIBufferType)
	{
	case OBJ_INFO::VIBufferType::VI_BUFFER_TYPE_RECT:
		static_cast<CRectObj*>(m_pObject)->Set_RectVIBufferSize(m_pGraphic_Device, &tSizePos);
		break;
	case OBJ_INFO::VIBufferType::VI_BUFFER_TYPE_CUBE:
		static_cast<CCubeObj*>(m_pObject)->Set_RectVIBufferSize(m_pGraphic_Device, &vSize);
		break;
	}
	UpdateData(FALSE);
}

void CPrototypeTool::UpDateFrame() // 랜더
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	// 후면 추리기 꺼져있음

	pInstance->Render_Begin();



	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	for (auto& pBaseLine : m_pBaseLineArr)
		pBaseLine->Render();

	m_pObject->Render();
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pInstance->Render_End(m_Picture.m_hWnd);

	RELEASE_INSTANCE(CGameInstance);
}

BEGIN_MESSAGE_MAP(CPrototypeTool, CDialog)
	ON_BN_CLICKED(IDC_PROTOTYPE_SIZE_SET, &CPrototypeTool::OnBnClickedPrototypeSizeSet)
	ON_LBN_SELCHANGE(IDC_PROTOTYPE_IMAGE_LIST, &CPrototypeTool::OnLbnSelchangePrototypeImageList)
	ON_BN_CLICKED(IDC_PROTOTYPE_ROT_RESET, &CPrototypeTool::OnBnClickedPrototypeRotReset)
	ON_BN_CLICKED(IDC_RADIO_PROTOTYPE_RECT, &CPrototypeTool::OnBnClickedRadioPrototypeRect)
	ON_BN_CLICKED(IDC_RADIO_PROTOTYPE_CUBE, &CPrototypeTool::OnBnClickedRadioPrototypeCube)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_WM_MBUTTONDOWN()
	ON_BN_CLICKED(IDC__PROTOTYPE_SAVE, &CPrototypeTool::OnBnClickedPrototypeSaveObject)
	ON_EN_CHANGE(IDC_PROTOTYPE_EDIT_COLLIDER_POS_X, &CPrototypeTool::OnEnChangePrototypeEditColliderPosX)
	ON_EN_CHANGE(IDC_PROTOTYPE_EDIT_COLLIDER_POS_Y, &CPrototypeTool::OnEnChangePrototypeEditColliderPosY)
	ON_EN_CHANGE(IDC_PROTOTYPE_EDIT_COLLIDER_POS_Z, &CPrototypeTool::OnEnChangePrototypeEditColliderPosZ)
	ON_EN_CHANGE(IDC_PROTOTYPE_EDIT_COLLIDER_SIZE_X, &CPrototypeTool::OnEnChangePrototypeEditColliderSizeX)
	ON_EN_CHANGE(IDC_PROTOTYPE_EDIT_COLLIDER_SIZE_Y, &CPrototypeTool::OnEnChangePrototypeEditColliderSizeY)
	ON_EN_CHANGE(IDC_PROTOTYPE_EDIT_COLLIDER_SIZE_Z, &CPrototypeTool::OnEnChangePrototypeEditColliderSizeZ)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROTOTYPE_SLIDER_ALPHA, &CPrototypeTool::OnNMCustomdrawPrototypeSliderAlpha)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROTOTYPE_SLIDER_RED, &CPrototypeTool::OnNMCustomdrawPrototypeSliderRed)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROTOTYPE_SLIDER_GREEN, &CPrototypeTool::OnNMCustomdrawPrototypeSliderGreen)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROTOTYPE_SLIDER_BLUE, &CPrototypeTool::OnNMCustomdrawPrototypeSliderBlue)
	ON_BN_CLICKED(IDC_PROTOTYPE_POS_SET, &CPrototypeTool::OnBnClickedPrototypePosSet)
END_MESSAGE_MAP()


// CPrototypeTool 메시지 처리기입니다.


BOOL CPrototypeTool::OnInitDialog() // 초기화
{
	// 초기화

	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	//(초기 설정 : 렉트)
	// 렉트 라디오 버튼 체크
	m_Radio[0].SetCheck(TRUE); 

	// 다이얼로그 초기화
	Initialize_DialogSeting();

	// 리스트 박스 설정 
	UpDate_ListBoxToRect();

	// 그래픽디바이스 클론
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	m_pGraphic_Device = pView->Clone_Device();
	pView->Clone_Renderer();


	// 객체정보 설정, 생성
	SetUp_RectObjectInfo();

	// 카메라 객체 생성
	m_pCamera = CToolViewCamera::Create(m_pGraphic_Device);
	m_pCamera->Sync_Window(m_Picture.m_hWnd);
	_vec3 vCameraLook = _vec3(0.f, 0.f, 0.f);
	m_pCamera->SetUp_InitialState(_vec3(0.f, 0.f, -5.f), vCameraLook);

	//베이스라인
	m_pBaseLineArr[CBaseLine::TYPE_X] = CBaseLine::Create(m_pGraphic_Device, CBaseLine::TYPE_X);
	m_pBaseLineArr[CBaseLine::TYPE_X]->SetUp_Scale(1.f, 0.1f, 0.1f);
	m_pBaseLineArr[CBaseLine::TYPE_Y] = CBaseLine::Create(m_pGraphic_Device, CBaseLine::TYPE_Y);
	m_pBaseLineArr[CBaseLine::TYPE_Y]->SetUp_Scale(0.1f, 1.f, 0.1f);
	m_pBaseLineArr[CBaseLine::TYPE_Z] = CBaseLine::Create(m_pGraphic_Device, CBaseLine::TYPE_Z);
	m_pBaseLineArr[CBaseLine::TYPE_Z]->SetUp_Scale(0.1f, 0.1f, 1.f);

	// 타이머 설정
	SetTimer(0, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPrototypeTool::Set_ColliderInfo()
{
	m_cSliderAlpha.SetRange(0, 255, TRUE);
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

void CPrototypeTool::Show_MetheColliderInfo()
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

HRESULT CPrototypeTool::Update_ColliderInfo()
{
	UpdateData(TRUE);


	if (m_pObjInfo == nullptr)
		return S_OK;
	Show_MetheColliderInfo();

	m_pObjInfo->vColliderSize = { m_fColliderSizerX, m_fColliderSizerY, m_fColliderSizerZ };
	m_pObjInfo->vColliderCenter = { m_fColliderCenterX, m_fColliderCenterY, m_fColliderCenterZ };
	m_pObjInfo->dwColliderColor = D3DCOLOR_ARGB(m_cSliderAlpha.GetPos(), m_cSliderRed.GetPos(), m_cSliderGreen.GetPos(), m_cSliderBlue.GetPos());

	switch (m_pObjInfo->eVIBufferType)
	{
	case OBJ_INFO::VI_BUFFER_TYPE_RECT:
		static_cast<CRectObj*>(m_pObject)->Update_BoxColliderValues(m_pObjInfo);
		break;
	case OBJ_INFO::VI_BUFFER_TYPE_CUBE:
		static_cast<CCubeObj*>(m_pObject)->Update_BoxColliderValues(m_pObjInfo);
		break;
	default:
		break;
	}

	UpdateData(FALSE);
	return S_OK;
}

void CPrototypeTool::OnBnClickedPrototypeSizeSet() // 객체 사이즈 설정
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	Set_PrototypeInfo();

}


void CPrototypeTool::OnLbnSelchangePrototypeImageList() // 리스트 클릭
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 리스트

	UpdateData(TRUE);

	_int iIndex = m_ListBox.GetCurSel();

	if (0 > iIndex)
		return;

	m_ListBox.GetText(iIndex, m_pObjInfo->szObjName);

	m_pObjInfo->iTextureIndex = iIndex;

	// 객체 타입에 맞는 세팅 호출
	switch (m_pObjInfo->eVIBufferType)
	{
	case OBJ_INFO::VIBufferType::VI_BUFFER_TYPE_RECT:
		static_cast<CRectObj*>(m_pObject)->Set_TextureNumber(iIndex);
		break;
	case OBJ_INFO::VIBufferType::VI_BUFFER_TYPE_CUBE:
		static_cast<CCubeObj*>(m_pObject)->Set_TextureNumber(iIndex);
		break;
	}
	UpdateData(TRUE);
}


void CPrototypeTool::OnBnClickedPrototypeRotReset() // 회전 초기화
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 슬라이드바 값 초기화 
	m_sldAxisX.SetPos(0);
	m_sldAxisY.SetPos(0);
	m_sldAxisZ.SetPos(0);

	int iPos;
	CString sPos;

	// 출력되는 값 초기화
	iPos = m_sldAxisX.GetPos();
	sPos.Format(_T("%d"), iPos);
	m_editAixsX.SetWindowText(sPos);

	iPos = m_sldAxisY.GetPos();
	sPos.Format(_T("%d"), iPos);
	m_editAxisY.SetWindowText(sPos);

	iPos = m_sldAxisZ.GetPos();
	sPos.Format(_T("%d"), iPos);
	m_editAxisZ.SetWindowText(sPos);

	// 객체 타입에 맞는 회전 호출
	switch (m_pObjInfo->eVIBufferType)
	{
	case OBJ_INFO::VIBufferType::VI_BUFFER_TYPE_RECT:
		static_cast<CRectObj*>(m_pObject)->ReSet_Rotation();
		break;
	case OBJ_INFO::VIBufferType::VI_BUFFER_TYPE_CUBE:
		static_cast<CCubeObj*>(m_pObject)->ReSet_Rotation();
		break;
	}

	// 카메라 원위치
	_vec3 vCameraLook = _vec3(0.f, 0.f, 0.f);
	m_pCamera->SetUp_InitialState(_vec3(0.f, 0.f, -5.f), vCameraLook);
}

void CPrototypeTool::OnBnClickedRadioPrototypeRect() // 렉트 라디오 버튼 클릭
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 렉트 버퍼

	// 다이얼로그 초기화
	Initialize_DialogSeting();
	// 리스트 박스 목록 초기화
	UpDate_ListBoxToRect();
	// 객체 생성
	SetUp_RectObjectInfo();
}

void CPrototypeTool::OnBnClickedRadioPrototypeCube() // 큐브 라디오 버튼 클릭
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 큐브 버퍼

	// 다이얼로그 초기화
	Initialize_DialogSeting();
	// 리스트 박스 목록 초기화
	UpDate_ListBoxToCube();
	// 객체 생성
	SetUp_CubeObjectInfo();
}


void CPrototypeTool::OnTimer(UINT_PTR nIDEvent) // 프레임 재생
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	UpdateData(TRUE);
	switch (nIDEvent)
	{
	case 0:
		pInstance->Tick_ToolEngine(0.1f);
		m_pCamera->Tick(0.1f);
		UpdateData(FALSE);
		UpDateFrame();
	default:
		break;
	}
	RELEASE_INSTANCE(CGameInstance);

	CDialog::OnTimer(nIDEvent);
}


void CPrototypeTool::OnMButtonDown(UINT nFlags, CPoint point) // 휠 버튼 클릭
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_pCamera->Do_CameraAction();
	m_pCamera->Get_CameraAction();

	CDialog::OnMButtonDown(nFlags, point);
}


void CPrototypeTool::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) // 스크롤 바에 커서 올리고 휠 돌릴때
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (IDC_PROTOTYPE_ROT_SLIDER_X == pScrollBar->GetDlgCtrlID() ||
		IDC_PROTOTYPE_ROT_SLIDER_Y == pScrollBar->GetDlgCtrlID() ||
		IDC_PROTOTYPE_ROT_SLIDER_Z == pScrollBar->GetDlgCtrlID())
	{
		// 슬라이드 바 값 가져오기
		_int iPosX = m_sldAxisX.GetPos();
		_int iPosY = m_sldAxisY.GetPos();
		_int iPosZ = m_sldAxisZ.GetPos();

		// 각도 값 출력
		CString sPos;
		sPos.Format(_T("%d"), iPosX);
		m_editAixsX.SetWindowText(sPos);
		sPos.Format(_T("%d"), iPosY);
		m_editAxisY.SetWindowText(sPos);
		sPos.Format(_T("%d"), iPosZ);
		m_editAxisZ.SetWindowText(sPos);

		// 객체 타입에 맞는 회전 호출
		switch (m_pObjInfo->eVIBufferType)
		{
		case OBJ_INFO::VIBufferType::VI_BUFFER_TYPE_RECT:
			static_cast<CRectObj*>(m_pObject)->Set_Rotation(iPosX, iPosY, iPosZ);
			break;
		case OBJ_INFO::VIBufferType::VI_BUFFER_TYPE_CUBE:
			static_cast<CCubeObj*>(m_pObject)->Set_Rotation(iPosX, iPosY, iPosZ);
			break;
		}
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPrototypeTool::OnBnClickedPrototypeSaveObject() // 저장
{
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

		_ulong dwByte;

		WriteFile(hFile, m_pObjInfo, sizeof(OBJ_INFO), &dwByte, nullptr);

		if (dwByte <= 0)
			MSGBOX("CTerrainTool OnBnClickedButton_SaveTerrain - Fail to SaveTerrain");

		CloseHandle(hFile);
	}
}



void CPrototypeTool::OnEnChangePrototypeEditColliderPosX()
{
	Update_ColliderInfo();
}


void CPrototypeTool::OnEnChangePrototypeEditColliderPosY()
{
	Update_ColliderInfo();
}


void CPrototypeTool::OnEnChangePrototypeEditColliderPosZ()
{
	Update_ColliderInfo();
}


void CPrototypeTool::OnEnChangePrototypeEditColliderSizeX()
{
	Update_ColliderInfo();
}


void CPrototypeTool::OnEnChangePrototypeEditColliderSizeY()
{
	Update_ColliderInfo();
}


void CPrototypeTool::OnEnChangePrototypeEditColliderSizeZ()
{
	Update_ColliderInfo();
}


void CPrototypeTool::OnNMCustomdrawPrototypeSliderAlpha(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	Update_ColliderInfo();
	*pResult = 0;
}


void CPrototypeTool::OnNMCustomdrawPrototypeSliderRed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	Update_ColliderInfo();
	*pResult = 0;
}


void CPrototypeTool::OnNMCustomdrawPrototypeSliderGreen(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	Update_ColliderInfo();
	*pResult = 0;
}


void CPrototypeTool::OnNMCustomdrawPrototypeSliderBlue(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	Update_ColliderInfo();
	*pResult = 0;
}


void CPrototypeTool::OnBnClickedPrototypePosSet()
{
	Set_PrototypeInfo();
}
