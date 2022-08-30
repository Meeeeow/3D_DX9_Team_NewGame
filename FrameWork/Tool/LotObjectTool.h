#pragma once

#ifndef __TOOL_LOT_OBJECT_TOOL_H__
#define __TOOL_LOT_OBJECT_TOOL_H__

#include "afxwin.h"
#include "afxcmn.h"
#include "BaseLine.h"

// CLotObjectTool ��ȭ �����Դϴ�.

class CLotObjectTool : public CDialog
{
	DECLARE_DYNAMIC(CLotObjectTool)

public:
	CLotObjectTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLotObjectTool();

public:
	class CForm* m_pForm = nullptr;

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOTOBJECTTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	float m_fBasePosX;
	float m_fBasePosY;
	float m_fBasePosZ;
	int m_iBaseNumX;
	int m_iBaseNumY;
	int m_iBaseNumZ;
	afx_msg void OnBnClickedLotobjecttoolButtonSet();
	virtual BOOL OnInitDialog();
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;
};
#endif // !__TOOL_LOT_OBJECT_TOOL_H__
