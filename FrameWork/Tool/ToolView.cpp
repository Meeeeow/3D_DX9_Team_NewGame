
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	ZeroMemory(&m_vMousePos, sizeof(_vec3));
}

CToolView::~CToolView()
{
	Free();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CToolView �׸���

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

	//�ͷ��ι��۴� ������Ÿ�� �߰� ����! ��� ���� ���� ��

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

	// �̹��� �߰��� ���� �����Ͻÿ�
	if (FAILED(pInstance->Add_Prototype(LEVEL_STATIC, TEXT("PrototypeTextureRect"),
		CTexture::Create(m_pDevice, L"../Client/bin/Resource/Texture/ListRect/Rect%d.png", 14))))
		MSG_FAIL("CToolView Ready_ComponentPrototype - Add_Prototype PrototypeTextureRect");

	// ť�� �ؽ��Ĵ� dds Ȯ���� �ϰ�
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
		//���� �ʿ�
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

	//���� ���� �̸���������
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_tMainSplitter.GetPane(0, 1));
	if (pForm == nullptr)
		MSG_VOID("CToolView::OnDraw - pForm == nullptr");
	CString wstrCurSceneName = pForm->Get_CurSceneName();

	
	//��ġ �Է��� ���� ���� ƽ
	pInstance->Tick_ToolEngine(0.1f);

	if (wstrCurSceneName != L"UI")
		m_pCamera->Tick(0.1f);

	pInstance->Render_Begin();

	//���� ���� ���õ��� �ʾҰų� ī�޶� �۵� ���� �ƴϸ� ������������!
	if (m_pCamera != nullptr && wstrCurSceneName != L"")
	{
		if (m_pRenderer == nullptr)
			MSG_VOID("CToolView OnDraw - m_pRenderer is null");

		//UI ���� ��� ���� �����ؾ���
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

		//ƽ ����Ʈƽ ������ �������� ������Ʈ ����ؾ���
		pForm->Tick(0.1f);
		pForm->LateTick(0.1f);
		

		for (size_t i = 0; i < CBaseLine::TYPE_END; ++i)
			m_pBaseLine[i]->Render();

		//����
		m_pRenderer->Draw_RenderGroup();

		pInstance->Draw_Font(m_szPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
	pInstance->Render_End(m_hWnd);
	RELEASE_INSTANCE(CGameInstance);

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


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
	

	//������ �޾ƿð� ������ �ɹ������� �����ص�
	m_pForm = dynamic_cast<CForm*>(pMain->m_tMainSplitter.GetPane(0, 1));
	if (m_pForm == nullptr)
		MSG_VOID("CToolView::OnDraw - pForm == nullptr");

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pInstance->Initialize_Engine(AfxGetInstanceHandle() , pMain->m_hWnd, LEVEL_END)))
		MSG_VOID("CToolView OnInitialUpdate - Initialize_Engine");

	if (FAILED(pInstance->Ready_Graphic_Device(g_hWnd, g_iWinCX, g_iWinCY, &m_pDevice)))
		MSG_VOID("CToolView OnInitialUpdate - Ready_Graphic_Device");

	//������Ʈ ������Ÿ�� ���� �����!
	Ready_ComponentPrototype();

	//�ؽ��� ���� �����!
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	m_pCamera->Do_CameraAction();
	if (m_pCamera->Get_CameraAction())
		SetTimer(0, 100, NULL);
	else
		KillTimer(0);
	CView::OnMButtonDown(nFlags, point);
}

void CToolView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	Render_ToolView();

	CView::OnTimer(nIDEvent);
}


//Ŭ�� �� UI���� ��� UI ��ġ
void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pForm == nullptr)
		MSG_VOID("CToolView::OnLButtonDown - m_pForm == nullptr");

	if (L"UI" == m_pForm->Get_CurSceneName())
	{
		int iIndex = m_pForm->Get_ListBoxPrototype().GetCurSel();
		//�ڲ� �˸��߸� �������ϱ� �׳� ����
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
