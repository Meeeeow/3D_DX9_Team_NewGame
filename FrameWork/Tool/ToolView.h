
// ToolView.h : CToolView Ŭ������ �������̽�
//

#pragma once
#include "Base.h"
#include "ToolDoc.h"
#include "BaseLine.h"

BEGIN(Engine)
class CRenderer;
END

class CToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	_pDevice Clone_Device();
	CRenderer* Clone_Renderer();
	class CToolViewCamera* Clone_Camera();
	void Free();
	void Render_ToolView();

	//������Ʈ ���� ����
	//	1. ToolView.cpp Ready_Texture() �� �ؽ��� ������Ÿ�� �ε�
	//	�� �� PrototypeTexture"�ؽ����̸�" ���� ������ ��
	//	"�ؽ����̸�"�� OBJ_INFO ����ü�� szTextureName ���� �ȿ� ����Ǿ����

	//	2. ToolView.cpp�� Create_ToolObj(OBJ_INFO* pInfo) ȣ��
	//	pInfo�� ������ ����� ä������� ��.
	//	pInfo�� �����Ҵ��� �Ǿ� �־����.���� ���������� �ʾƵ� ��
	//	������ �Ⱦ��� ����̱� ������ D2D�� ������Ʈ �Ŵ���ó�� �������� �ڷᱸ�� �ϳ� �ʿ���
	//	�ʿ信 ���� map list vector���� ����
	CGameObject* Create_ToolObj(OBJ_INFO* pInfo);

private:
	HRESULT Ready_ComponentPrototype();
	HRESULT Ready_Texture();

private:
	_pDevice m_pDevice = nullptr;
	class CRenderer* m_pRenderer = nullptr;
	class CToolViewCamera* m_pCamera = nullptr;
	_float m_fTime = 0.f;
	TCHAR m_szPos[256] = TEXT("");

	class CForm*		m_pForm = nullptr;
	_vec3				m_vMousePos;

public:
	CBaseLine* m_pBaseLine[CBaseLine::TYPE_END];
// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

