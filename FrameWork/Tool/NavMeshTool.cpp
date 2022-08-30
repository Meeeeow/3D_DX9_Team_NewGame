// NavMeshTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "NavMeshTool.h"
#include "afxdialogex.h"


// CNavMeshTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNavMeshTool, CDialog)

CNavMeshTool::CNavMeshTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_NAVMESHTOOL, pParent)
{

}

CNavMeshTool::~CNavMeshTool()
{
}

void CNavMeshTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNavMeshTool, CDialog)
END_MESSAGE_MAP()


// CNavMeshTool 메시지 처리기입니다.
