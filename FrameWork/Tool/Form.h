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

// CForm �� ���Դϴ�.

BEGIN(Engine)
class CGameObject;
END

class CForm : public CFormView
{
	DECLARE_DYNCREATE(CForm)

public:
	CForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CForm();

				

public:
	//���� ���õ� ������Ʈ �̸��� ȭ�鿡 ����ش�
	void					Set_ObjName(CString wstrObjName);
	//���� ���õ� �� �̸��� ȭ�鿡 ����ش�
	void					Set_SceneName(CString wstrSceneName);

	//ȭ�鿡 ����� ������ OBJ_INFO�� ���� ���� �����Ѵ�.
	void					Set_ObjInfoToScreen(const OBJ_INFO* pObjInfo);
	//���õ� ������Ʈ�� Ʈ�������� ����� ��� �ش� ������Ʈ�� ���� �����ϰ� ȭ���� �ٽ� �׸���
	HRESULT					Change_SelectedObjectTransform();

	//�ڷᱸ�� map�� �� �־��ֱ�
	HRESULT					Set_MapScene(CString wstrSceneName);


	//ȭ�鿡 ī�޶� ��ǥ���� �Է��ϸ� ī�޶��� ��ǥ�� �̵���Ų��.
	HRESULT					Set_CamPosToCam();
	//ī�޶� �̵��� ���� ī�޶��� ��ǥ���� ȭ�鿡 ������ش� 
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
	//������Ʈ �Ŵ��� ���� ���� �Լ�
	_int	Tick(_float fTimeDelta);
	_int	LateTick(_float fTimeDelta);
	HRESULT Render_SceneObject(CString wstrSceneName, _uint iIndex);
	//UI ����
	HRESULT Change_MatOrth();

	HRESULT Render_PrototypeObjectOrth(CGameObject* pGameObj);

	HRESULT Add_PrototypeObjToScene(OBJ_INFO* pObjInfo);

public:
	//���ϸ� �ڿ� *.*���� �ٿ���
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

	//�� ����
	map<CString, vector<CGameObject*>>			m_mapScene;
	typedef map<CString, vector<CGameObject*>>	SCENES;

	//�� ������Ʈ ���� //���� ���縸 �� ģ��
	vector<CGameObject*>						m_vectorSceneObj;
	//���Ŀ� �����ϱ� ���� �� ������ ģ��
	vector<CGameObject*>						m_vectorPrototype;

	//������Ÿ�� ����
	map<CString, CGameObject*>					m_mapPrototype;
	typedef map<CString, CGameObject*>			PROTOTYPES;

	//��-������Ʈ ������Ʈ ����

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
