#include "stdafx.h"
#include "Navigation.h"

IMPLEMENT_DYNAMIC(Navigation, CWnd)

BEGIN_MESSAGE_MAP(Navigation, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void Navigation::OnLButtonDown(UINT nFlags, CPoint point)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChildView* pView = (CChildView*)pFrame->GetView();
	pView->SetPosition( point.x, point.y );

	CWnd::OnLButtonDown(nFlags, point);
}

void Navigation::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( nFlags & MK_LBUTTON )
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CChildView* pView = (CChildView*)pFrame->GetView();
		pView->SetPosition( point.x, point.y );
	}

	CWnd::OnMouseMove(nFlags, point);
}
