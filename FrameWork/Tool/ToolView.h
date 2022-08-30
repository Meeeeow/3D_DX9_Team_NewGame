
// ToolView.h : CToolView 클래스의 인터페이스
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
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// 특성입니다.
public:
	CToolDoc* GetDocument() const;

// 작업입니다.
public:
	_pDevice Clone_Device();
	CRenderer* Clone_Renderer();
	class CToolViewCamera* Clone_Camera();
	void Free();
	void Render_ToolView();

	//오브젝트 생성 순서
	//	1. ToolView.cpp Ready_Texture() 에 텍스쳐 프로토타입 로드
	//	이 때 PrototypeTexture"텍스쳐이름" 으로 생성할 것
	//	"텍스쳐이름"은 OBJ_INFO 구조체의 szTextureName 변수 안에 저장되어야함

	//	2. ToolView.cpp의 Create_ToolObj(OBJ_INFO* pInfo) 호출
	//	pInfo의 값들은 제대로 채워졌어야 함.
	//	pInfo는 동적할당이 되어 있어야함.따로 삭제해주지 않아도 됨
	//	엔진을 안쓰는 방법이기 때문에 D2D의 오브젝트 매니저처럼 관리해줄 자료구조 하나 필요함
	//	필요에 따라 map list vector에서 선언
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
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

