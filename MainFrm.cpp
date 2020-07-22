// MainFrm.cpp : CMainFrame 클래스의 구현
//
#include "stdafx.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_MOVE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
	m_DlgBrushPalette->DestroyWindow();
	m_DlgToolBar.DestroyWindow();
	m_wndView.DestroyWindow();
	m_wndStatusBar.DestroyWindow();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if ( !m_DlgToolBar.Create( this, IDD_DIALOGBAR, CBRS_TOP, IDD_DIALOGBAR ) )
	{
		TRACE0("다이얼로그 툴바를 만들지 못했습니다.\n");
		return -1;
	}
	m_DlgToolBar.EnableDocking( CBRS_ALIGN_TOP );
	ShowControlBar( &m_DlgToolBar, TRUE, FALSE );

	m_DlgBrushPalette = 0;

	if ( !m_DlgWideView.Create( IDD_DIALOG2, this ) )
	{
		TRACE0("와이드 뷰를 만들지 못했습니다.\n");
		return -1;
	}

	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("뷰 창을 만들지 못했습니다.\n");
		return -1;
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.hMenu = NULL;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 뷰 창으로 포커스를 이동합니다.
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 뷰에서 첫째 크랙이 해당 명령에 나타나도록 합니다.
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 그렇지 않으면 기본 처리합니다.
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
void CMainFrame::OnMove(int x, int y)
{
	CFrameWnd::OnMove(x, y);

	m_DlgBrushPalette->Move( pt.x - x, pt.y - y );

	pt.x = x;
	pt.y = y;
}

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();
}

void CMainFrame::ResetBrushPallete()
{
	delete m_DlgBrushPalette;

	m_DlgBrushPalette = new BrushPalette();

	if ( !m_DlgBrushPalette->Create( IDD_DIALOG1, this ) )
		TRACE0("브러쉬 팔레트를 만들지 못했습니다.\n");

	// 브러쉬 팔레트 크기 조정
	CRect windowRt;
	m_DlgBrushPalette->GetWindowRect( &windowRt );

	CRect clientRt;
	m_DlgBrushPalette->GetClientRect( &clientRt );

	CRect frameRt;
	GetWindowRect( &frameRt );

	CRect rt;
	rt.left = frameRt.left + 20;
	rt.top = frameRt.top + 80;
	rt.right = rt.left + windowRt.right - windowRt.left - clientRt.right + 210;
	rt.bottom = rt.top + windowRt.bottom - windowRt.top;

	m_DlgBrushPalette->MoveWindow( rt );

	pt.x = frameRt.left;
	pt.y = frameRt.top;
}