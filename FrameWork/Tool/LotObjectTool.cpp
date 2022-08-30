// LotObjectTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "LotObjectTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "GameObject.h"
#include "Transform.h"
#include "Form.h"


// CLotObjectTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLotObjectTool, CDialog)

CLotObjectTool::CLotObjectTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_LOTOBJECTTOOL, pParent)
	, m_fBasePosX(0.f)
	, m_fBasePosY(0.f)
	, m_fBasePosZ(0.f)
	, m_iBaseNumX(1)
	, m_iBaseNumY(1)
	, m_iBaseNumZ(1)
	, m_fScaleX(1.f)
	, m_fScaleY(1.f)
	, m_fScaleZ(1.f)
{

}

CLotObjectTool::~CLotObjectTool()
{
}

void CLotObjectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOTOBJECTTOOL_EDIT_NUM_X, m_iBaseNumX);
	DDX_Text(pDX, IDC_LOTOBJECTTOOL_EDIT_NUM_Y, m_iBaseNumY);
	DDX_Text(pDX, IDC_LOTOBJECTTOOL_EDIT_NUM_Z, m_iBaseNumZ);

	DDX_Text(pDX, IDC_LOTOBJECTTOOL_EDIT_POS_X, m_fBasePosX);
	DDX_Text(pDX, IDC_LOTOBJECTTOOL_EDIT_POS_Y, m_fBasePosY);
	DDX_Text(pDX, IDC_LOTOBJECTTOOL_EDIT_POS_Z, m_fBasePosZ);

	DDX_Text(pDX, IDC_LOTOBJECTTOOL_EDIT_SCALE_X, m_fScaleX);
	DDX_Text(pDX, IDC_LOTOBJECTTOOL_EDIT_SCALE_Y, m_fScaleY);
	DDX_Text(pDX, IDC_LOTOBJECTTOOL_EDIT_SCALE_Z, m_fScaleZ);

}


BEGIN_MESSAGE_MAP(CLotObjectTool, CDialog)
	ON_BN_CLICKED(IDC_LOTOBJECTTOOL_BUTTON_SET, &CLotObjectTool::OnBnClickedLotobjecttoolButtonSet)
END_MESSAGE_MAP()


// CLotObjectTool 메시지 처리기입니다.


void CLotObjectTool::OnBnClickedLotobjecttoolButtonSet()
{
	UpdateData(TRUE);

	CGameObject* pGameObj = m_pForm->Get_SelectedPrototypeObject();
	CTransform* pTransform = dynamic_cast<CTransform*>(pGameObj->Get_Component(pTagComTransform));
	if (pTransform == nullptr)
		MSG_VOID("CLotObjectTool::OnBnClickedLotobjecttoolButtonSet - pTransform == nullptr");

	//우선 정점의 사이즈 고려 안하고 짜봄
	_vec3 vBasePos = _vec3(m_fBasePosX, m_fBasePosY, m_fBasePosZ);
	_vec3 vScale = _vec3(m_fScaleX, m_fScaleY, m_fScaleZ);

	for (int i = 0; i < m_iBaseNumX; ++i)
	{
		for (int j = 0; j < m_iBaseNumY; ++j)
		{
			for (int k = 0; k < m_iBaseNumZ; ++k)
			{
				OBJ_INFO* pObjInfo = new OBJ_INFO;
				memcpy(pObjInfo, static_cast<CToolObj*>(pGameObj)->Get_ObjInfo(), sizeof(OBJ_INFO));

				_vec3 vObjPos = pObjInfo->Get_State(OBJ_INFO::STATE_POS);
				_vec3 vPrototypeScale = pObjInfo->Get_Scale();
				vPrototypeScale.x *= vScale.x;
				vPrototypeScale.y *= vScale.y;
				vPrototypeScale.z *= vScale.z;
				vObjPos += vBasePos;
				vObjPos += _vec3(i * vPrototypeScale.x, j * vPrototypeScale.y, k * vPrototypeScale.z);
				pObjInfo->SetUp_State(OBJ_INFO::STATE_POS, vObjPos);

				pObjInfo->SetUp_Scale(vPrototypeScale.x, vPrototypeScale.y, vPrototypeScale.z);

				m_pForm->Add_PrototypeObjToScene(pObjInfo);

			}
		}
	}

	UpdateData(FALSE);
}


BOOL CLotObjectTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	m_pForm = dynamic_cast<CForm*>(pMain->m_tMainSplitter.GetPane(0, 1));
	if (m_pForm == nullptr)
		MSGBOX("CToolView::OnDraw - pForm == nullptr");


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
