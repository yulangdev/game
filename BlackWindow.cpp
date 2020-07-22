#include "stdafx.h"
#include "BlackWindow.h"

IMPLEMENT_DYNAMIC(BlackWindow, CWnd)

BEGIN_MESSAGE_MAP(BlackWindow, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
END_MESSAGE_MAP()

BOOL BlackWindow::OnEraseBkgnd(CDC* pDC)
{
	CBrush backBrush(RGB(0, 0, 0));

	CBrush* pOldBrush = pDC->SelectObject(&backBrush);

	CRect rect;
	pDC->GetClipBox(&rect);

	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);

	return CWnd::OnEraseBkgnd(pDC);
}

int BlackWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	this->EnableWindow( false );

	return 0;
}
