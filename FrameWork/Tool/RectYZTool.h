#pragma once

#include "Base.h"
#include "afxwin.h"
#include "afxcmn.h"
// CRectYZTool 대화 상자입니다.

BEGIN(Engine)
class CRenderer;
END

class CRectYZTool : public CDialog
{
	DECLARE_DYNAMIC(CRectYZTool)

public:
	CRectYZTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
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
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RECTYZTOOL };
#endif



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:	

	CListBox	m_listTexture;			// 텍스쳐 리스트 박스입니다.
	float		m_fU			= 1.f;	// UV 좌표의 U(Z) 값의 배수를 설정해줍니다. (1 ~ 100)
	float		m_fV			= 1.f;	// UV 좌표의 V(Y) 값의 배수를 설정해줍니다. (1 ~ 100)
	float		m_fXInterval	= 1.f;	// 평면의 X 정점간의 간격 차이를 설정해줍니다.
	float		m_fZInterval	= 1.f;	// 평면의 Z 정점간의 간격 차이를 설정해줍니다. 
	float		m_fYInterval	= 1.f;	// 평면의 Y 정점간의 간격 차이를 설정해줍니다.
	int			m_iVerticesX	= 2;	// X 정점의 개수 입니다.
	int			m_iVerticesY	= 2;	// Y 정점의 개수입니다.
	int			m_iVerticesZ	= 2;	// Z 정점의 개수입니다.
	float		m_fPosX			= 0.f;	// Transform에 사용될 Position X의 값 입니다.
	float		m_fPosY			= 0.f;	// Transform에 사용될 Position Y의 값 입니다.
	float		m_fPosZ			= 0.f;	// Transform에 사용될 Position Z의 값 입니다.
	float		m_fRotX			= 0.f;	// Transform에 사용될 Rotation X의 값 입니다.
	float		m_fRotY			= 0.f;	// Transform에 사용될 Rotation Y의 값 입니다.
	float		m_fRotZ			= 0.f;	// Transform에 사용될 Rotation Z의 값 입니다.
	float		m_fScaleX		= 1.f;	// Transform에 사용될 Scale X의 값 입니다.
	float		m_fScaleY		= 1.f;	// Transform에 사용될 Scale Y의 값 입니다.
	float		m_fScaleZ		= 1.f;	// Transform에 사용될 Scale Z의 값 입니다.
	float m_fColliderCenterX	= 0.f;	// Collder 생성 시 X의 중점 좌표입니다.
	float m_fColliderCenterY	= 0.f;	// Collder 생성 시 Y의 중점 좌표입니다.
	float m_fColliderCenterZ	= 0.f;	// Collder 생성 시 Z의 중점 좌표입니다.
	float m_fColliderSizeX		= 1.f;	// Collder 생성 시 X의 크기입니다.
	float m_fColliderSizeY		= 1.f;	// Collder 생성 시 Y의 크기입니다.
	float m_fColliderSizeZ		= 1.f;	// Collder 생성 시 Z의 크기입니다.
	CButton		m_RadioRectDir[2];		// 벽의 형태를 정해줍니다. ( 0- YZ / 1- XY )
	CButton		m_RadioSide[2];			// 벽이 바라보는 방향을 설정합니다. (반대 방향 후면 추려내기)
	CButton		m_RadioLook[2];			// 벽이 바라보는 방향을 설정합니다. ( 0- 앞 / 1- 뒤 ) (반대 방향 후면 추려내기)
	CStatic		m_PicWall;				// 벽이 표현될 공간입니다.
	CString		m_strObjName = L"";		// 생성할 벽의 이름입니다.
	CString		m_szTextureName = L"";	// 생성할 벽의 프로토타입 이름입니다.
	CSliderCtrl m_cSliderAlpha;			// Collider 생성 시 선의 Alpha 값 입니다. 
	CSliderCtrl m_cSliderRed;			// Collider 생성 시 선의 Red 값 입니다. 
	CSliderCtrl m_cSliderGreen;			// Collider 생성 시 선의 Green 값 입니다. 
	CSliderCtrl m_cSliderBlue;			// Collider 생성 시 선의 Blue 값 입니다. 
	CEdit		m_editAlpha;			// Collider Slider의 Alpha 값을 수치화 해줍니다.
	CEdit		m_editRed;				// Collider Slider의 Red 값을 수치화 해줍니다.
	CEdit		m_editGreen;			// Collider Slider의 Blue 값을 수치화 해줍니다.
	CEdit		m_editBlue;				// Collider Slider의 Green 값을 수치화 해줍니다.

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
