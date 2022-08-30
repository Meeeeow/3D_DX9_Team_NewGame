#pragma once
#include "afxwin.h"
#include "GameInstance.h"

BEGIN(Engine)
class CRenderer;
END

// CUITool 대화 상자입니다.

class CUITool : public CDialog
{
	DECLARE_DYNAMIC(CUITool)

public:
	CUITool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUITool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UITOOL };
#endif

public:
	HRESULT Render_UITool();
	HRESULT Render_UIToolPreview();
	HRESULT Update_ObjState();
	HRESULT Update_EditState(CTransform* pTransform);
	HRESULT Add_ListBoxPlacedUIString(class CGameObject* pGameObj);
	HRESULT Add_ListBoxPrevieUIString(class CUIObj* pUIObj);
	HRESULT Load_Texture();

private:
	_pDevice				m_pDevice = nullptr;
	CRenderer*				m_pRenderer = nullptr;
	vector<class CUIObj*>	m_vectorPlacedUI;
	vector<class CUIObj*>	m_vectorPreviewUI;

	LONG					m_dwPicturePreviewX;
	LONG					m_dwPicturePreviewY;

	LONG					m_dwPicturePlacedX = 0;
	LONG					m_dwPicturePlacedY = 0;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_pictureUITool;
	CListBox m_listBoxPreviewUI;
	float m_fPosX;
	float m_fPosY;
	float m_fScaleX;
	float m_fScaleY;
	virtual BOOL OnInitDialog();
	CListBox m_listBoxPlacedUI;
	afx_msg void OnEnChangeEditUitoolPosX();
	afx_msg void OnEnChangeEditUitoolPosY();
	afx_msg void OnEnChangeEditUitoolScaleX();
	afx_msg void OnEnChangeEditUitoolScaleY();
	afx_msg void OnBnClickedButtonUitoolLoadUi();
	afx_msg void OnBnClickedButtonUitoolDeleteInScene();
	afx_msg void OnBnClickedButtonUitoolSaveUi();
	CStatic m_pictureUIToolPreview;
	afx_msg void OnLbnSelchangeListUi();
	afx_msg void OnBnClickedButtonUitoolAddToScene();
	afx_msg void OnLbnSelchangeListUitoolPlacedUi();
};
