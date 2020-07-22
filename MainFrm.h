#pragma once
#include "ChildView.h"
#include "BrushPalette.h"
#include "ToolBar.h"
#include "WideView.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:
	CChildView* GetView() { return &m_wndView; }

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // 컨트롤 모음이 포함된 멤버입니다. 원래 protected:
	CStatusBar		m_wndStatusBar;
	CChildView		m_wndView;
	ToolBar			m_DlgToolBar;
	BrushPalette*	m_DlgBrushPalette;
	WideView		m_DlgWideView;

	CPoint			pt;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMove(int x, int y);
	afx_msg void OnDestroy();

	void ResetBrushPallete();
};


