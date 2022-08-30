#pragma once
#include "afxwin.h"

#include "Base.h"
// CTerrainTool 대화 상자입니다.

BEGIN(Engine)
class CRenderer;
END

class CTerrainTool : public CDialog
{
	DECLARE_DYNAMIC(CTerrainTool)

public:
	CTerrainTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTerrainTool();

public:
	HRESULT Add_ListBoxString(CListBox& listBox, CString wstr);
	HRESULT Set_ScreenValue(OBJ_INFO* pObjInfo);
	HRESULT Create_NewTerrainBuffer();
	HRESULT Render_Tool();

private:
	void Free();
	HRESULT Load_Texture();

private:
	class CToolTerrainObj*	m_pTerrain = nullptr;
	_pDevice				m_pDevice = nullptr;
	class CToolViewCamera*	m_pCamera = nullptr;
	CRenderer *				m_pRenderer = nullptr;
	_float					m_fTime = 0.f;
	TCHAR					m_szPos[256] = TEXT("");

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAINTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdit_PosX();

	CListBox m_listBoxTexture;
	CListBox m_listBoxHeightMap;

	float m_fTerrainPosX = 0.f;
	float m_fTerrainPosY = 0.f;
	float m_fTerrainPosZ = 0.f;

	float m_fTerrainRotX = 0.f;
	float m_fTerrainRotY = 0.f;
	float m_fTerrainRotZ = 0.f;

	float m_fTerrainScaleX = 1.f;
	float m_fTerrainScaleY = 1.f;
	float m_fTerrainScaleZ = 1.f;

	float m_fTerrainSizeX = 1.f;
	float m_fTerrainSizeZ = 1.f;

	float m_fTerrainU = 1.f;
	float m_fTerrainV = 1.f;

	int m_iTerrainVerticesNumX = 2;
	int m_iTerrainVerticesNumZ = 2;

	CString m_wstrCurTerrainName;

	CStatic m_pictureCtrlTerrain;
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLbnSelchangeList_ChangeTexture();
	afx_msg void OnEnChangeEdit_ChangePosY();
	afx_msg void OnEnChangeEdit_ChangePosZ();
	afx_msg void OnEnChangeEdit_ChangeRotX();
	afx_msg void OnEnChangeEdit_ChangeRotY();
	afx_msg void OnEnChangeEdit_ChangeRotZ();
	afx_msg void OnEnChangeEdit_ChangeScaleX();
	afx_msg void OnEnChangeEdit_ChangeScaleY();
	afx_msg void OnEnChangeEdit_ChangeScaleZ();
	afx_msg void OnEnChangeEdit_ChangeUVX();
	afx_msg void OnEnChangeEdit_ChangeUVZ();
	afx_msg void OnEnChangeEdit_ChangeVecticesNumX();
	afx_msg void OnEnChangeEdit_ChangeVecticesNumZ();
	afx_msg void OnBnClickedButton_SaveTerrain();
	afx_msg void OnBnClickedButton_LoadTerrain();
	//afx_msg void OnBnClickedButtonTerrainToolWireFrame_OnOffWireFrame();
	//afx_msg void OnBnClickedTerrainToolCreatePrototype();
	afx_msg void OnBnClickedTerrainToolCreatePrototype();
	afx_msg void OnBnClickedButtonTerrainToolWireFrame();
};