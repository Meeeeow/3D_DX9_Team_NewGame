#pragma once

#ifndef __TOOL_LOT_OBJECT_TOOL_H__
#define __TOOL_LOT_OBJECT_TOOL_H__

#include "afxwin.h"
#include "afxcmn.h"
#include "BaseLine.h"

// CLotObjectTool 대화 상자입니다.

class CLotObjectTool : public CDialog
{
	DECLARE_DYNAMIC(CLotObjectTool)

public:
	CLotObjectTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLotObjectTool();

public:
	class CForm* m_pForm = nullptr;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOTOBJECTTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
