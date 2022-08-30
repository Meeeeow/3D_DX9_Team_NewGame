
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "GameInstance.h"
#include "ToolViewCamera.h"
#include "TextureSaver.h"
#include "Form.h"
#include "ToolTerrainObj.h"
#include "RectObj.h"
#include "CubeObj.h"
#include "RectYZObj.h"
#include "UIObj.h"
#include "BaseLine.h"
#include "RectXYObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	ZeroMemory(&m_vMousePos, sizeof(_vec3));
}

CToolView::~CToolView()
{
	Free();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 그리기

_pDevice CToolView::Clone_Device()
{
	Safe_AddRef(m_pDevice);
	return m_pDevice;
}

CRenderer * CToolView::Clone_Renderer()
{
	if (m_pRenderer == nullptr)
		MSG_NULL("CToolView Clone_Renderer - m_pRenderer is null");

	Safe_AddRef(m_pRenderer);
	return m_pRenderer;
}

CToolViewCamera * CToolView::Clone_Camera()
{
	Safe_AddRef(m_pCamera);
	return m_pCamera;
}

void CToolView::Free()
{
	Safe_Release(m_pBaseLine[0]);
	Safe_Release(m_pBaseLine[1]);
	Safe_Release(m_pBaseLine[2]);

	Safe_Release(m_pCamera);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDevice);
}

void CToolView::Render_ToolView()
{
	Invalidate(TRUE);
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	_vec3 vPos = m_pCamera->Get_CameraPos();
	_float fFov = m_pCamera->Get_CameraFov();
	_stprintf_s(m_szPos, TEXT("X: %f \nY: %f \nZ: %f \nFov : %f"), vPos.x, vPos.y, vPos.z, fFov);

	RELEASE_INSTANCE(CGameInstance);
	Invalidate(FALSE);
}

HRESULT CToolView::Ready_ComponentPrototype()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	//터레인버퍼는 프로토타입 추가 안함! 계속 새로 만들 것

	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTransform"), CTransform::Create(m_pDevice))))
		MSG_FAIL("CToolView Ready_ComponentPrototype - Add_Prototype PrototypeTransform");

	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeRenderer"), m_pRenderer = CRenderer::Create(m_pDevice))))
		MSG_FAIL("CToolView Ready_ComponentPrototype - Add_Prototype PrototypeRenderer");

	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeVIBufferRect"), CVIBufferRect::Create(m_pDevice))))
		MSG_FAIL("CToolView Ready_ComponentPrototype - Add_Prototype PrototypeVIbufferRect");

	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeVIBufferRectYZ"), CVIBufferRectYZ::Create(m_pDevice))))
		MSG_FAIL("CToolView Ready_ComponentPrototype - Add_Prototype PrototypeVIbufferRectYZ");

	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeVIBufferCube"), CVIBufferCube::Create(m_pDevice))))
		MSG_FAIL("CToolView Ready_ComponentPrototype - Add_Prototype PrototypeVIBufferCube");

	RELEASE_INSTANCE(CGameInstance);
	Safe_AddRef(m_pRenderer);

	return S_OK;
}

HRESULT CToolView::Ready_Texture()
{
	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureTerrain"), 
		CTexture::Create(m_pDevice, L"../Client/bin/Resource/Texture/Terrain/Terrain%d.tga", 4))))
		MSG_FAIL("CToolView Ready_ComponentPrototype - Add_Prototype PrototypeTextureTerrain");

	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTexturePlayer"),
		CTexture::Create(m_pDevice, L"../Client/bin/Resource/Texture/Player/Player%d.png", 1))))
		MSG_FAIL("CToolView Ready_ComponentPrototype - Add_Prototype PrototypeTexturePlayer");

	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureWall"),
		CTexture::Create(m_pDevice, L"../Client/bin/Resource/Texture/Wall/Wall%d.png", 8))))
		MSG_FAIL("CToolView Ready_ComponentPrototype - Add_Prototype PrototypeTextureWall");

	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureUI"),
		CTexture::Create(m_pDevice, L"../Client/bin/Resource/Texture/UI/UI%d.png", 4))))
		MSG_FAIL("CToolView Ready_ComponentPrototype - Add_Prototype PrototypeTextureUI");

	// 이미지 추가시 숫자 변경하시오
	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureRect"),
		CTexture::Create(m_pDevice, L"../Client/bin/Resource/Texture/ListRect/Rect%d.png", 14))))
		MSG_FAIL("CToolView Ready_ComponentPrototype - Add_Prototype PrototypeTextureRect");

	// 큐브 텍스쳐는 dds 확장자 일것
	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureCube"),
		CTexture::Create(m_pDevice, L"../Client/bin/Resource/Texture/ListCube/Cube%d.dds", 18, CTexture::TYPE_CUBE))))
		MSG_FAIL("CToolView Ready_ComponentPrototype - Add_Prototype PrototypeTextureCube");

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CGameObject * CToolView::Create_ToolObj(OBJ_INFO * pInfo)
{
	CGameObject* pGameObj = nullptr;

	switch (pInfo->eVIBufferType)
	{
	case OBJ_INFO::VI_BUFFER_TYPE_RECT:
		pGameObj = CRectObj::Create(m_pDevice, pInfo);
		break;
	case OBJ_INFO::VI_BUFFER_TYPE_RECT_YZ:
		pGameObj = CRectYZObj::Create(m_pDevice);
		static_cast<CRectYZObj*>(pGameObj)->Set_RectYZBuffer(pInfo);
		break;
	case OBJ_INFO::VI_BUFFER_TYPE_RECT_XY:
		pGameObj = CRectXYObj::Create(m_pDevice);
		static_cast<CRectXYObj*>(pGameObj)->Set_RectXYBuffer(pInfo);
		break;
	case OBJ_INFO::VI_BUFFER_TYPE_CUBE:
		//수정 필요
		pGameObj = CCubeObj::Create(m_pDevice, pInfo);
		break;
	case OBJ_INFO::VI_BUFFER_TYPE_TERRAIN:
	{
		pGameObj = CToolTerrainObj::Create(m_pDevice);
		CToolTerrainObj* pTerrainObj = dynamic_cast<CToolTerrainObj*>(pGameObj);
		if (pTerrainObj == nullptr)
			MSG_NULL("CTool Create_ToolObj - pTerrainObj is null");
		pTerrainObj->Set_TerrainBuffer(pInfo);
		pTerrainObj->Set_TextureIndex(pInfo->iTextureIndex);
	}
		break;
	case OBJ_INFO::VI_BUFFER_TYPE_UI:
		pGameObj = CUIObj::Create(m_pDevice, pInfo);
		break;
	}

	return pGameObj;
}

void CToolView::OnDraw(CDC* pDC)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	//현재 씬의 이름가져오기
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_tMainSplitter.GetPane(0, 1));
	if (pForm == nullptr)
		MSG_VOID("CToolView::OnDraw - pForm == nullptr");
	CString wstrCurSceneName = pForm->Get_CurSceneName();

	
	//장치 입력을 위한 엔진 틱
	pInstance->Tick_ToolEngine(0.1f);

	if (wstrCurSceneName != L"UI")
		m_pCamera->Tick(0.1f);

	pInstance->Render_Begin();

	//현재 씬이 선택되지 않았거나 카메라가 작동 중이 아니면 렌더하지말자!
	if (m_pCamera != nullptr && wstrCurSceneName != L"")
	{
		if (m_pRenderer == nullptr)
			MSG_VOID("CToolView OnDraw - m_pRenderer is null");

		//UI 씬의 경우 직교 투영해야함
		if (wstrCurSceneName == L"UI")
		{
			if (FAILED(pForm->Change_MatOrth()))
				MSG_VOID("CToolView::OnDraw - pForm->Change_MatOrth");

			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);
			RECT rc;
			GetClientRect(&rc);
			rc;
			m_vMousePos = _vec3((_float)pt.x, (_float)(g_iWinCY) - (_float)pt.y, 0.f);
			_stprintf_s(m_szPos, TEXT("X: %f \nY: %f \n"), m_vMousePos.x, m_vMousePos.y);
			pInstance->Draw_Font(m_szPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}
		else
			pInstance->Draw_Font(m_szPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

		//틱 레이트틱 돌려서 렌더러에 오브젝트 등록해야함
		pForm->Tick(0.1f);
		pForm->LateTick(0.1f);
		

		for (size_t i = 0; i < CBaseLine::TYPE_END; ++i)
			m_pBaseLine[i]->Render();

		//렌더
		m_pRenderer->Draw_RenderGroup();

		pInstance->Draw_Font(m_szPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	pInstance->Render_End(m_hWnd);
	RELEASE_INSTANCE(CGameInstance);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	RECT rcMain = {};
	pMain->GetWindowRect(&rcMain);
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);
	RECT rcView{};
	GetClientRect(&rcView);

	int iGapX = rcMain.right - rcView.right;
	int iGapY = rcMain.bottom - rcView.bottom;

	pMain->SetWindowPos(nullptr, 0, 0, g_iWinCX + iGapX, g_iWinCY + iGapY, SWP_NOMOVE);
	pMain->SetWindowPos(nullptr, 0, 0, 3000, g_iWinCY + iGapY, SWP_NOMOVE);
	//pMain->SetWindowPos(nullptr, 0, 0, g_iWinCX , g_iWinCY, SWP_NOMOVE);
	RECT rcWnd, rcasdf;
	GetWindowRect(&rcWnd);
	GetClientRect(&rcasdf);
	int a = 0;
	

	//폼에서 받아올게 많으니 맴버변수로 저장해둠
	m_pForm = dynamic_cast<CForm*>(pMain->m_tMainSplitter.GetPane(0, 1));
	if (m_pForm == nullptr)
		MSG_VOID("CToolView::OnDraw - pForm == nullptr");

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pInstance->Initialize_Engine(AfxGetInstanceHandle() , pMain->m_hWnd, LEVEL_END)))
		MSG_VOID("CToolView OnInitialUpdate - Initialize_Engine");

	if (FAILED(pInstance->Ready_Graphic_Device(g_hWnd, g_iWinCX, g_iWinCY, &m_pDevice)))
		MSG_VOID("CToolView OnInitialUpdate - Ready_Graphic_Device");

	//컴포넌트 프로토타입 전부 여기로!
	Ready_ComponentPrototype();

	//텍스쳐 전부 여기로!
	Ready_Texture();

	if (FAILED(pInstance->Add_Timer(TEXT("Timer_Default"))))
		MSG_VOID("CToolView OnInitialUpdate - Add_Timer Timer_Default");

	if (FAILED(pInstance->Add_Timer(TEXT("Timer_60"))))
		MSG_VOID("CToolView OnInitialUpdate - Add_Timer Timer_60");

	m_pCamera = CToolViewCamera::Create(m_pDevice);
	m_pCamera->Sync_Window(this->m_hWnd);
	_vec3 vPos = m_pCamera->Get_CameraPos();
	_float fFov = m_pCamera->Get_CameraFov();
	m_pCamera->SetUp_InitialState(_vec3(0.f, 30.f, -30.f), _vec3(0.f, 0.f, 0.f));

	_stprintf_s(m_szPos, TEXT("X: %f \nY: %f \nZ: %f \nFov : %f"), vPos.x, vPos.y, vPos.z,fFov);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (size_t i = 0; i < CBaseLine::TYPE_END; ++i)
		m_pBaseLine[i] = nullptr;
	m_pBaseLine[CBaseLine::TYPE_X] = CBaseLine::Create(m_pDevice, CBaseLine::TYPE_X);
	m_pBaseLine[CBaseLine::TYPE_Y] = CBaseLine::Create(m_pDevice, CBaseLine::TYPE_Y);
	m_pBaseLine[CBaseLine::TYPE_Z] = CBaseLine::Create(m_pDevice, CBaseLine::TYPE_Z);

	RELEASE_INSTANCE(CGameInstance);
}

void CToolView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_pCamera->Do_CameraAction();
	if (m_pCamera->Get_CameraAction())
		SetTimer(0, 100, NULL);
	else
		KillTimer(0);
	CView::OnMButtonDown(nFlags, point);
}

void CToolView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	Render_ToolView();

	CView::OnTimer(nIDEvent);
}


//클릭 시 UI씬의 경우 UI 배치
void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pForm == nullptr)
		MSG_VOID("CToolView::OnLButtonDown - m_pForm == nullptr");

	if (L"UI" == m_pForm->Get_CurSceneName())
	{
		int iIndex = m_pForm->Get_ListBoxPrototype().GetCurSel();
		//자꾸 알림뜨면 귀찮으니까 그냥 리턴
		if (iIndex < 0)
			return;

		OBJ_INFO* pObjInfo = new OBJ_INFO;
		memcpy(pObjInfo, static_cast<CToolObj*>(m_pForm->Get_PrototypeObject(iIndex))->Get_ObjInfo(), sizeof(OBJ_INFO));

		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		m_vMousePos = _vec3((_float)pt.x, (_float)(g_iWinCY)-(_float)pt.y, 0.f);
		pObjInfo->SetUp_State(OBJ_INFO::STATE_POS, m_vMousePos);
		CGameObject* pGameObj = Create_ToolObj(pObjInfo);

		if (FAILED(m_pForm->Add_ObjectToScene(pGameObj)))
			MSG_VOID("CToolView::OnLButtonDown - m_pForm->Add_ObjectToScene");
		m_pForm->AddString_ListBoxSceneObj(pObjInfo->szObjName);
	}
	else
	{

	}

	CView::OnLButtonDown(nFlags, point);
}
