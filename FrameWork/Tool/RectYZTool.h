#pragma once

#include "Base.h"
#include "afxwin.h"
#include "afxcmn.h"
// CRectYZTool ��ȭ �����Դϴ�.

BEGIN(Engine)
class CRenderer;
END

class CRectYZTool : public CDialog
{
	DECLARE_DYNAMIC(CRectYZTool)

public:
	CRectYZTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CRectYZTool();

public:
	virtual BOOL OnInitDialog();
	void	Set_SliderInfo();
	void	Show_MetheSliderValues();
	HRESULT Set_ChangeValues();
	HRESULT Set_LoadValue(OBJ_INFO* pObjInfo);
	HRESULT Load_Texture();
	HRESULT Add_TextureString(CListBox& listBox, CString wstr);
	HRESULT Render_Picture();
	void	Set_ChangeSliderValues();
	void	Free();

private:
	_pDevice				m_pDevice		= nullptr;
	class CRectYZObj*		m_pRectYZ		= nullptr;
	class CRectXYObj*		m_pRectXY		= nullptr;
	class CToolViewCamera*	m_pCamera		= nullptr;
	CRenderer*				m_pRenderer		= nullptr;
	_float					m_fTime			= 0.f;
	TCHAR					m_szPos[256]	= TEXT("");
// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RECTYZTOOL };
#endif



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:	

	CListBox	m_listTexture;			// �ؽ��� ����Ʈ �ڽ��Դϴ�.
	float		m_fU			= 1.f;	// UV ��ǥ�� U(Z) ���� ����� �������ݴϴ�. (1 ~ 100)
	float		m_fV			= 1.f;	// UV ��ǥ�� V(Y) ���� ����� �������ݴϴ�. (1 ~ 100)
	float		m_fXInterval	= 1.f;	// ����� X �������� ���� ���̸� �������ݴϴ�.
	float		m_fZInterval	= 1.f;	// ����� Z �������� ���� ���̸� �������ݴϴ�. 
	float		m_fYInterval	= 1.f;	// ����� Y �������� ���� ���̸� �������ݴϴ�.
	int			m_iVerticesX	= 2;	// X ������ ���� �Դϴ�.
	int			m_iVerticesY	= 2;	// Y ������ �����Դϴ�.
	int			m_iVerticesZ	= 2;	// Z ������ �����Դϴ�.
	float		m_fPosX			= 0.f;	// Transform�� ���� Position X�� �� �Դϴ�.
	float		m_fPosY			= 0.f;	// Transform�� ���� Position Y�� �� �Դϴ�.
	float		m_fPosZ			= 0.f;	// Transform�� ���� Position Z�� �� �Դϴ�.
	float		m_fRotX			= 0.f;	// Transform�� ���� Rotation X�� �� �Դϴ�.
	float		m_fRotY			= 0.f;	// Transform�� ���� Rotation Y�� �� �Դϴ�.
	float		m_fRotZ			= 0.f;	// Transform�� ���� Rotation Z�� �� �Դϴ�.
	float		m_fScaleX		= 1.f;	// Transform�� ���� Scale X�� �� �Դϴ�.
	float		m_fScaleY		= 1.f;	// Transform�� ���� Scale Y�� �� �Դϴ�.
	float		m_fScaleZ		= 1.f;	// Transform�� ���� Scale Z�� �� �Դϴ�.
	float m_fColliderCenterX	= 0.f;	// Collder ���� �� X�� ���� ��ǥ�Դϴ�.
	float m_fColliderCenterY	= 0.f;	// Collder ���� �� Y�� ���� ��ǥ�Դϴ�.
	float m_fColliderCenterZ	= 0.f;	// Collder ���� �� Z�� ���� ��ǥ�Դϴ�.
	float m_fColliderSizeX		= 1.f;	// Collder ���� �� X�� ũ���Դϴ�.
	float m_fColliderSizeY		= 1.f;	// Collder ���� �� Y�� ũ���Դϴ�.
	float m_fColliderSizeZ		= 1.f;	// Collder ���� �� Z�� ũ���Դϴ�.
	CButton		m_RadioRectDir[2];		// ���� ���¸� �����ݴϴ�. ( 0- YZ / 1- XY )
	CButton		m_RadioSide[2];			// ���� �ٶ󺸴� ������ �����մϴ�. (�ݴ� ���� �ĸ� �߷�����)
	CButton		m_RadioLook[2];			// ���� �ٶ󺸴� ������ �����մϴ�. ( 0- �� / 1- �� ) (�ݴ� ���� �ĸ� �߷�����)
	CStatic		m_PicWall;				// ���� ǥ���� �����Դϴ�.
	CString		m_strObjName = L"";		// ������ ���� �̸��Դϴ�.
	CString		m_szTextureName = L"";	// ������ ���� ������Ÿ�� �̸��Դϴ�.
	CSliderCtrl m_cSliderAlpha;			// Collider ���� �� ���� Alpha �� �Դϴ�. 
	CSliderCtrl m_cSliderRed;			// Collider ���� �� ���� Red �� �Դϴ�. 
	CSliderCtrl m_cSliderGreen;			// Collider ���� �� ���� Green �� �Դϴ�. 
	CSliderCtrl m_cSliderBlue;			// Collider ���� �� ���� Blue �� �Դϴ�. 
	CEdit		m_editAlpha;			// Collider Slider�� Alpha ���� ��ġȭ ���ݴϴ�.
	CEdit		m_editRed;				// Collider Slider�� Red ���� ��ġȭ ���ݴϴ�.
	CEdit		m_editGreen;			// Collider Slider�� Blue ���� ��ġȭ ���ݴϴ�.
	CEdit		m_editBlue;				// Collider Slider�� Green ���� ��ġȭ ���ݴϴ�.

	afx_msg void OnEnChange_UV_U();
	afx_msg void OnEnChange_UV_V();
	afx_msg void OnEnChange_INTERVAL_Z();
	afx_msg void OnEnChange_INTERVAL_Y();
	afx_msg void OnEnChange_Vertices_Y();
	afx_msg void OnEnChange_Vertices_Z();
	afx_msg void OnEnChange_Position_X();
	afx_msg void OnEnChange_Position_Y();
	afx_msg void OnEnChange_Position_Z();
	afx_msg void OnEnChange_Scale_X();
	afx_msg void OnEnChange_Scale_Y();
	afx_msg void OnEnChange_Scale_Z();
	
	
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLbnSelchange_TextureList();
	afx_msg void OnBnClicked_WireFrame_Render();
	afx_msg void OnBnClicked_Save_Data();
	afx_msg void OnBnClicked_Load_Data();
	afx_msg void OnBnClickedWallRadioLeft();
	afx_msg void OnBnClickedWallRadioRight();
	
	afx_msg void OnEnChangeWallEditColliderPosX();
	afx_msg void OnEnChangeWallEditColliderPosY();
	afx_msg void OnEnChangeWallEditColliderPosZ();
	afx_msg void OnEnChangeWallEditColliderSizeX();
	afx_msg void OnEnChangeWallEditColliderSizeY();
	afx_msg void OnEnChangeWallEditColliderSizeZ();
	
	
	
	afx_msg void OnNMCustomdrawWallSliderAlpha(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawWallSliderRed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawWallSliderGreen(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawWallSliderBlue(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeWallEditStringTexture();
	afx_msg void OnEnChangeWallEditStringName();

	afx_msg void OnBnClickedWallRadioBufferTypeYz();
	afx_msg void OnBnClickedWallRadioBufferTypeXy();
	afx_msg void OnBnClickedWallRadioFront();
	afx_msg void OnBnClickedWallRadioBack();
	afx_msg void OnEnChangeWallEditXInt();
	afx_msg void OnEnChangeWallEditXVert();
};
