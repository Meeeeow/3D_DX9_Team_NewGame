#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "BaseLine.h"

// CPrototypeTool ��ȭ �����Դϴ�.

BEGIN(Engine)
class CGameInstance;
class CGameObject;
class CGraphice_Device;
class CTransform;
END

class CPrototypeTool : public CDialog
{
	DECLARE_DYNAMIC(CPrototypeTool)

public:
	CPrototypeTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPrototypeTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROTOTYPETOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	void Free();

private:
	// ���̾�α� �ʱ�ȭ
	void Initialize_DialogSeting();

	// ��Ʈ��ü ���� ����
	void SetUp_RectObjectInfo();

	// ť�갴ü ���� ����
	void SetUp_CubeObjectInfo();

	// ��ü ����
	void Create_Object(OBJ_INFO* pInfo);

	// ��Ʈ�ؽ��� ����Ʈ ������Ʈ
	HRESULT UpDate_ListBoxToRect();

	// ť���ؽ��� ����Ʈ ������Ʈ
	HRESULT UpDate_ListBoxToCube();

	void Set_PrototypeInfo();

private:
	void UpDateFrame();

private:
	_pDevice					m_pGraphic_Device = nullptr;
	class CToolViewCamera*		m_pCamera = nullptr;

private:
	CGameObject*			m_pObject = nullptr;
	OBJ_INFO*				m_pObjInfo = nullptr;
	CBaseLine*				m_pBaseLineArr[CBaseLine::TYPE_END];

public:
	CStatic m_Picture;
	CButton m_Radio[2];
	CListBox m_ListBox;
	virtual BOOL OnInitDialog();
	CSliderCtrl m_sldAxisX;
	CSliderCtrl m_sldAxisY;
	CSliderCtrl m_sldAxisZ;
	CEdit m_editAixsX;
	CEdit m_editAxisY;
	CEdit m_editAxisZ;
	float m_fSizeX;
	float m_fSizeY;
	float m_fSizeZ;
	_int m_iPreRotX;
	_int m_iPreRotY;
	_int m_iPreRotZ;

	// Collider Info
	float m_fColliderCenterX	= 0.f;
	float m_fColliderCenterY	= 0.f;
	float m_fColliderCenterZ	= 0.f;
	float m_fColliderSizerX		= 1.f;
	float m_fColliderSizerY		= 1.f;
	float m_fColliderSizerZ		= 1.f;
	
	CEdit m_editAlpha;
	CEdit m_editRed;
	CEdit m_editGreen;
	CEdit m_editBlue;


public: //Collider Function
	void Set_ColliderInfo();
	void Show_MetheColliderInfo();
	HRESULT Update_ColliderInfo();

	
	afx_msg void OnEnChangePrototypeEditColliderPosX();
	afx_msg void OnEnChangePrototypeEditColliderPosY();
	afx_msg void OnEnChangePrototypeEditColliderPosZ();
	afx_msg void OnEnChangePrototypeEditColliderSizeX();
	afx_msg void OnEnChangePrototypeEditColliderSizeY();
	afx_msg void OnEnChangePrototypeEditColliderSizeZ();

public:
	afx_msg void OnBnClickedPrototypeSizeSet();
	afx_msg void OnLbnSelchangePrototypeImageList();
	afx_msg void OnBnClickedPrototypeRotReset();
	afx_msg void OnBnClickedRadioPrototypeRect();
	afx_msg void OnBnClickedRadioPrototypeCube();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnBnClickedPrototypeSaveObject();
	
	
	CSliderCtrl m_cSliderAlpha;
	CSliderCtrl m_cSliderRed;
	CSliderCtrl m_cSliderGreen;
	CSliderCtrl m_cSliderBlue;
	afx_msg void OnNMCustomdrawPrototypeSliderAlpha(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawPrototypeSliderRed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawPrototypeSliderGreen(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawPrototypeSliderBlue(NMHDR *pNMHDR, LRESULT *pResult);
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
	afx_msg void OnBnClickedPrototypePosSet();
};

