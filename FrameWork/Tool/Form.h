#pragma once

#ifndef __TOOL_FORM_H__
#define __TOOL_FORM_H__


#include "NavMeshTool.h"
#include "PrototypeTool.h"
#include "afxwin.h"
#include "Base.h"
#include "TerrainTool.h"
#include "RectYZTool.h"
#include "UITool.h"
#include "LotObjectTool.h"

// CForm 폼 뷰입니다.

BEGIN(Engine)
class CGameObject;
END

class CForm : public CFormView
{
	DECLARE_DYNCREATE(CForm)

public:
	CForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CForm();

				

public:
	//현재 선택된 오브젝트 이름을 화면에 띄워준다
	void					Set_ObjName(CString wstrObjName);
	//현재 선택된 씬 이름을 화면에 띄워준다
	void					Set_SceneName(CString wstrSceneName);

	//화면에 띄워줄 값들을 OBJ_INFO의 값을 통해 갱신한다.
	void					Set_ObjInfoToScreen(const OBJ_INFO* pObjInfo);
	//선택된 오브젝트의 트랜스폼이 변경될 경우 해당 오브젝트의 값을 변경하고 화면을 다시 그린다
	HRESULT					Change_SelectedObjectTransform();

	//자료구조 map에 씬 넣어주기
	HRESULT					Set_MapScene(CString wstrSceneName);


	//화면에 카메라 좌표값을 입력하면 카메라의 좌표를 이동시킨다.
	HRESULT					Set_CamPosToCam();
	//카메라가 이동할 때는 카메라의 좌표값을 화면에 출력해준다 
	HRESULT					Set_CamPosToScreen();

	void AddString_ListBoxScene(CString wstrName);
	void AddString_ListBoxSceneObj(CString wstrObjectName);
	HRESULT Add_ObjectToScene(CGameObject* pObj);
	void AddString_ListBoxPrototype(CString wstrName, CGameObject* pObj);

	void Clear_ListBox(CListBox& listBox);

public:
	const CListBox& Get_ListBoxPrototype() const { return m_listBoxPrototype; }
	size_t Get_VectorSizeMapScene(CString wstrSceneName);
	const CString& Get_CurSceneName() const { return m_wstrSceneName; }

	CGameObject* Get_PrototypeObject(_int iIndex) { return m_vectorPrototype[iIndex]; }
	CGameObject* Get_SelectedPrototypeObject();

public:
	//오브젝트 매니저 역할 해줄 함수
	_int	Tick(_float fTimeDelta);
	_int	LateTick(_float fTimeDelta);
	HRESULT Render_SceneObject(CString wstrSceneName, _uint iIndex);
	//UI 전용
	HRESULT Change_MatOrth();

	HRESULT Render_PrototypeObjectOrth(CGameObject* pGameObj);

	HRESULT Add_PrototypeObjToScene(OBJ_INFO* pObjInfo);

public:
	//파일명 뒤에 *.*까지 붙여서
	void LoadSetAll_ListBox(CString wstrPath, CListBox& listBox);

private:
	void Free();

private:
	CPrototypeTool						m_tPrototype;
	CNavMeshTool						m_tNavMesh;
	CTerrainTool						m_tTerrainTool;
	CRectYZTool							m_tWallTool;
	CUITool								m_tUITool;
	CLotObjectTool						m_tLotObjTool;
	class CToolView*					m_pTool = nullptr;

	//씬 저장
	map<CString, vector<CGameObject*>>			m_mapScene;
	typedef map<CString, vector<CGameObject*>>	SCENES;

	//씬 오브젝트 벡터 //얕은 복사만 할 친구
	vector<CGameObject*>						m_vectorSceneObj;
	//픽쳐에 렌더하기 위해 값 보관할 친구
	vector<CGameObject*>						m_vectorPrototype;

	//프로토타입 저장
	map<CString, CGameObject*>					m_mapPrototype;
	typedef map<CString, CGameObject*>			PROTOTYPES;

	//씬-오브젝트 오브젝트 저장

	_pDevice							m_pDevice = nullptr;

	class CToolViewCamera*				m_pCamera = nullptr;

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked_CreatePrototype();
	afx_msg void OnBnClicked_CreateNavMesh();
	CString m_wstrObjName;
	CListBox m_listBoxPrototype;
	CListBox m_listBoxScene;
	CListBox m_listBoxSceneObj;
	float m_fObjPosX;
	float m_fObjPosY;
	float m_fObjPosZ;
	float m_fObjRotX;
	float m_fObjRotY;
	float m_fObjRotZ;
	float m_fObjScaleX;
	float m_fObjScaleY;
	float m_fObjScaleZ;
	afx_msg void OnBnClicked_CreateScene();
	afx_msg void OnBnClicked_AddPrototypeObjectToScene();
	afx_msg void OnBnClicked_SetTransform();
	CString m_wstrSceneName;
	afx_msg void OnBnClicked_SaveScene();
	afx_msg void OnBnClicked_LoadScene();
	afx_msg void OnBnClicked_DeleteObjectInScene();
	afx_msg void OnBnClicked_LoadPrototypeObject();
	afx_msg void OnLbnSelchange_ChangeScene();
	afx_msg void OnLbnSelchange_ClickPrototype();
	afx_msg void OnBnClickedButton_SavePrototype();
	virtual void OnInitialUpdate();
	afx_msg void OnLbnSelchangeList_ChangeSceneObject();
	afx_msg void OnBnClickedButton_CreateEditTerrain();
	afx_msg void OnEnChangeEdit4_ChangePosX();
	afx_msg void OnEnChangeEdit12_ChangePosY();
	afx_msg void OnEnChangeEdit5_ChangePosZ();
	afx_msg void OnEnChangeEdit6_ChangeRotX();
	afx_msg void OnEnChangeEdit9_ChangeRotY();
	afx_msg void OnEnChangeEdit8_ChangeRotZ();
	afx_msg void OnEnChangeEdit7_ChangeScaleX();
	afx_msg void OnEnChangeEdit10_ChangeScaleY();
	afx_msg void OnEnChangeEdit11__ChangeScaleZ();
	afx_msg void OnBnClicked_Wall_Create_Edit_Button();
	afx_msg void OnBnClickedButtonFormCreateUi();
	CStatic m_picturePortotypeOrth;
	afx_msg void OnBnClickedFormButtonCreateUiPrototype();
	float m_fEditCamPosX;
	float m_fEditCamPosY;
	float m_fEditCamPosZ;
	afx_msg void OnEnChangeFormEditCamPosX();
	afx_msg void OnEnChangeFormEditCamPosY();
	afx_msg void OnEnChangeFormEditCamPosZ();
	afx_msg void OnBnClickedFormButtonBackCurling();
	CString m_wstrBackCurlingOnOff;
	afx_msg void OnBnClickedFormButtonCreateLotObject();
};


#endif // !__TOOL_FORM_H__
