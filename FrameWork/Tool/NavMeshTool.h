#pragma once


// CNavMeshTool ��ȭ �����Դϴ�.

class CNavMeshTool : public CDialog
{
	DECLARE_DYNAMIC(CNavMeshTool)

public:
	CNavMeshTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CNavMeshTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAVMESHTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
