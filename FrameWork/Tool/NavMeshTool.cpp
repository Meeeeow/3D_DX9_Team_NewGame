// NavMeshTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "NavMeshTool.h"
#include "afxdialogex.h"


// CNavMeshTool ��ȭ �����Դϴ�.

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


// CNavMeshTool �޽��� ó�����Դϴ�.
