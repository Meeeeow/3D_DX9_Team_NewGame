// PrototypeTool.cpp : ���� �����Դϴ�.
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


// CPrototypeTool ��ȭ �����Դϴ�.

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

// ���� ������Ʈ�� �ƹ��� ���� �� �ּ��� ����ÿ� 2

void CPrototypeTool::Initialize_DialogSeting() // ���̾�α� �ʱ�ȭ
{
	m_sldAxisX.SetRange(0, 360); // �ּ�, �ִ밪
	m_sldAxisX.SetPos(0); // ���� ��ġ

	m_sldAxisY.SetRange(0, 360); // �ּ�, �ִ밪
	m_sldAxisY.SetPos(0); // ���� ��ġ

	m_sldAxisZ.SetRange(0, 360); // �ּ�, �ִ밪
	m_sldAxisZ.SetPos(0); // ���� ��ġ

	//m_sldAxisX.GetPos(); ��ũ���� ���� �޾ƿ�

	// �����ڽ��� �ʱⰪ ä��
	int		iPos = 0;// 
	CString sPos;
	sPos.Format(_T("%d"), iPos);

	// ȸ��
	m_editAixsX.SetWindowText(sPos);
	m_editAxisY.SetWindowText(sPos);
	m_editAxisZ.SetWindowText(sPos);

	m_iPreRotX = 0;
	m_iPreRotY = 0;
	m_iPreRotZ = 0;

	// ������
	m_fSizeX = 1.f;
	m_fSizeY = 1.f;
	m_fSizeZ = 1.f;

	m_fPosX = 0.f;
	m_fPosY = 0.f;
	m_fPosZ = 0.f;

	Set_ColliderInfo();
}

void CPrototypeTool::SetUp_RectObjectInfo() // ������Ÿ�� �� �ʱ�ȭ�� 
{
	// ��� ������ ����
	if(nullptr == m_pObjInfo)
		m_pObjInfo = new OBJ_INFO;
	// ������Ʈ Ÿ��
	m_pObjInfo->eVIBufferType = OBJ_INFO::VI_BUFFER_TYPE_RECT;
	// ��ü �̸� 
	memcpy(m_pObjInfo->szObjName, TEXT("None"), sizeof(_tchar)*OBJ_NAME);
	// �ؽ��� ������Ʈ (���� �Ҷ� ����)
	memcpy(m_pObjInfo->szTextureName, TEXT("PrototypeTextureRect"), sizeof(_tchar)*OBJ_NAME);
	// �ؽ��� ��ȣ
	m_pObjInfo->iTextureIndex = 0;
	// ������ ����
	m_pObjInfo->fSizeX = m_fSizeX;
	m_pObjInfo->fSizeY = m_fSizeY;
	m_pObjInfo->fSizeZ = m_fSizeZ;
	// ��ü ����
	Create_Object(m_pObjInfo);
}

void CPrototypeTool::SetUp_CubeObjectInfo()
{
	// ��� ������ ����
	if (nullptr == m_pObjInfo)
		m_pObjInfo = new OBJ_INFO;
	// ������Ʈ Ÿ��
	m_pObjInfo->eVIBufferType = OBJ_INFO::VI_BUFFER_TYPE_CUBE;
	// ��ü �̸� 
	memcpy(m_pObjInfo->szObjName, TEXT("Ex1"), sizeof(_tchar)*OBJ_NAME);
	// �ؽ��� ������Ʈ (���� �Ҷ� ����)
	memcpy(m_pObjInfo->szTextureName, TEXT("PrototypeTextureCube"), sizeof(_tchar)*OBJ_NAME);
	// �ؽ��� ��ȣ
	m_pObjInfo->iTextureIndex = 0;
	// ������ ����
	m_pObjInfo->fSizeX = m_fSizeX;
	m_pObjInfo->fSizeY = m_fSizeY;
	m_pObjInfo->fSizeZ = m_fSizeZ;
	// ��ü ����
	Create_Object(m_pObjInfo);
}

void CPrototypeTool::Create_Object(OBJ_INFO * pInfo) // ������Ʈ ����
{
	OBJ_INFO* pObjInfo = nullptr;
	// ���� ����� ����
	if (nullptr != m_pObject)
	{
		pObjInfo = new OBJ_INFO;
		memcpy(pObjInfo, pInfo, sizeof(OBJ_INFO));
		Safe_Release(m_pObject);
		pInfo = pObjInfo;
		m_pObjInfo = pInfo;
	}

	// enum ���� ���� ��ü ����
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

HRESULT CPrototypeTool::UpDate_ListBoxToRect() // ��Ʈ �̹����� ����Ʈ ������Ʈ
{
	// ��ü �߰��� ��ȣ�� �°� ��ü�̸��� �־����� ����Ʈ���� ��밡��
	// * ���� �ȸ����� ������ �����ִ��� Ȯ���ϰ� ����

	// ����Ʈ ��� �ʱ�ȭ
	m_ListBox.ResetContent();

	_tchar szBuff[MAX_PATH]{};

	// ��ü�� �̸� ���� // ����Ʈ�� �߰�

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

HRESULT CPrototypeTool::UpDate_ListBoxToCube() // ť�� �̹����� ����Ʈ ������Ʈ
{
	// ��ü �߰��� ��ȣ�� �°� ��ü�̸��� �־����� ����Ʈ���� ��밡��
	// * ���� �ȸ����� ������ �����ִ��� Ȯ���ϰ� ����

	// ����Ʈ ��� �ʱ�ȭ
	m_ListBox.ResetContent();

	_tchar szBuff[MAX_PATH]{};

	// ��ü�� �̸� ���� // ����Ʈ�� �߰�
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
	// ������
	UpdateData(TRUE);

	// ������ ����
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

	// ��ü Ÿ�Կ� �´� ���� ȣ��
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

void CPrototypeTool::UpDateFrame() // ����
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	// �ĸ� �߸��� ��������

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


// CPrototypeTool �޽��� ó�����Դϴ�.


BOOL CPrototypeTool::OnInitDialog() // �ʱ�ȭ
{
	// �ʱ�ȭ

	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	//(�ʱ� ���� : ��Ʈ)
	// ��Ʈ ���� ��ư üũ
	m_Radio[0].SetCheck(TRUE); 

	// ���̾�α� �ʱ�ȭ
	Initialize_DialogSeting();

	// ����Ʈ �ڽ� ���� 
	UpDate_ListBoxToRect();

	// �׷��ȵ���̽� Ŭ��
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CToolView* pView = dynamic_cast<CToolView*>(pMain->m_tMainSplitter.GetPane(0, 0));
	m_pGraphic_Device = pView->Clone_Device();
	pView->Clone_Renderer();


	// ��ü���� ����, ����
	SetUp_RectObjectInfo();

	// ī�޶� ��ü ����
	m_pCamera = CToolViewCamera::Create(m_pGraphic_Device);
	m_pCamera->Sync_Window(m_Picture.m_hWnd);
	_vec3 vCameraLook = _vec3(0.f, 0.f, 0.f);
	m_pCamera->SetUp_InitialState(_vec3(0.f, 0.f, -5.f), vCameraLook);

	//���̽�����
	m_pBaseLineArr[CBaseLine::TYPE_X] = CBaseLine::Create(m_pGraphic_Device, CBaseLine::TYPE_X);
	m_pBaseLineArr[CBaseLine::TYPE_X]->SetUp_Scale(1.f, 0.1f, 0.1f);
	m_pBaseLineArr[CBaseLine::TYPE_Y] = CBaseLine::Create(m_pGraphic_Device, CBaseLine::TYPE_Y);
	m_pBaseLineArr[CBaseLine::TYPE_Y]->SetUp_Scale(0.1f, 1.f, 0.1f);
	m_pBaseLineArr[CBaseLine::TYPE_Z] = CBaseLine::Create(m_pGraphic_Device, CBaseLine::TYPE_Z);
	m_pBaseLineArr[CBaseLine::TYPE_Z]->SetUp_Scale(0.1f, 0.1f, 1.f);

	// Ÿ�̸� ����
	SetTimer(0, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
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

void CPrototypeTool::OnBnClickedPrototypeSizeSet() // ��ü ������ ����
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	Set_PrototypeInfo();

}


void CPrototypeTool::OnLbnSelchangePrototypeImageList() // ����Ʈ Ŭ��
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// ����Ʈ

	UpdateData(TRUE);

	_int iIndex = m_ListBox.GetCurSel();

	if (0 > iIndex)
		return;

	m_ListBox.GetText(iIndex, m_pObjInfo->szObjName);

	m_pObjInfo->iTextureIndex = iIndex;

	// ��ü Ÿ�Կ� �´� ���� ȣ��
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


void CPrototypeTool::OnBnClickedPrototypeRotReset() // ȸ�� �ʱ�ȭ
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// �����̵�� �� �ʱ�ȭ 
	m_sldAxisX.SetPos(0);
	m_sldAxisY.SetPos(0);
	m_sldAxisZ.SetPos(0);

	int iPos;
	CString sPos;

	// ��µǴ� �� �ʱ�ȭ
	iPos = m_sldAxisX.GetPos();
	sPos.Format(_T("%d"), iPos);
	m_editAixsX.SetWindowText(sPos);

	iPos = m_sldAxisY.GetPos();
	sPos.Format(_T("%d"), iPos);
	m_editAxisY.SetWindowText(sPos);

	iPos = m_sldAxisZ.GetPos();
	sPos.Format(_T("%d"), iPos);
	m_editAxisZ.SetWindowText(sPos);

	// ��ü Ÿ�Կ� �´� ȸ�� ȣ��
	switch (m_pObjInfo->eVIBufferType)
	{
	case OBJ_INFO::VIBufferType::VI_BUFFER_TYPE_RECT:
		static_cast<CRectObj*>(m_pObject)->ReSet_Rotation();
		break;
	case OBJ_INFO::VIBufferType::VI_BUFFER_TYPE_CUBE:
		static_cast<CCubeObj*>(m_pObject)->ReSet_Rotation();
		break;
	}

	// ī�޶� ����ġ
	_vec3 vCameraLook = _vec3(0.f, 0.f, 0.f);
	m_pCamera->SetUp_InitialState(_vec3(0.f, 0.f, -5.f), vCameraLook);
}

void CPrototypeTool::OnBnClickedRadioPrototypeRect() // ��Ʈ ���� ��ư Ŭ��
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// ��Ʈ ����

	// ���̾�α� �ʱ�ȭ
	Initialize_DialogSeting();
	// ����Ʈ �ڽ� ��� �ʱ�ȭ
	UpDate_ListBoxToRect();
	// ��ü ����
	SetUp_RectObjectInfo();
}

void CPrototypeTool::OnBnClickedRadioPrototypeCube() // ť�� ���� ��ư Ŭ��
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// ť�� ����

	// ���̾�α� �ʱ�ȭ
	Initialize_DialogSeting();
	// ����Ʈ �ڽ� ��� �ʱ�ȭ
	UpDate_ListBoxToCube();
	// ��ü ����
	SetUp_CubeObjectInfo();
}


void CPrototypeTool::OnTimer(UINT_PTR nIDEvent) // ������ ���
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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


void CPrototypeTool::OnMButtonDown(UINT nFlags, CPoint point) // �� ��ư Ŭ��
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	m_pCamera->Do_CameraAction();
	m_pCamera->Get_CameraAction();

	CDialog::OnMButtonDown(nFlags, point);
}


void CPrototypeTool::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) // ��ũ�� �ٿ� Ŀ�� �ø��� �� ������
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (IDC_PROTOTYPE_ROT_SLIDER_X == pScrollBar->GetDlgCtrlID() ||
		IDC_PROTOTYPE_ROT_SLIDER_Y == pScrollBar->GetDlgCtrlID() ||
		IDC_PROTOTYPE_ROT_SLIDER_Z == pScrollBar->GetDlgCtrlID())
	{
		// �����̵� �� �� ��������
		_int iPosX = m_sldAxisX.GetPos();
		_int iPosY = m_sldAxisY.GetPos();
		_int iPosZ = m_sldAxisZ.GetPos();

		// ���� �� ���
		CString sPos;
		sPos.Format(_T("%d"), iPosX);
		m_editAixsX.SetWindowText(sPos);
		sPos.Format(_T("%d"), iPosY);
		m_editAxisY.SetWindowText(sPos);
		sPos.Format(_T("%d"), iPosZ);
		m_editAxisZ.SetWindowText(sPos);

		// ��ü Ÿ�Կ� �´� ȸ�� ȣ��
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

void CPrototypeTool::OnBnClickedPrototypeSaveObject() // ����
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
