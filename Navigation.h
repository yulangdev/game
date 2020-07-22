#pragma once
#include "MainFrm.h"

class Navigation : public CWnd
{
	DECLARE_DYNAMIC(Navigation)

public:
	Navigation() {}
	virtual ~Navigation() {}

	void Init( CRect& rt ){ MoveWindow( rt.left +5, rt.top + 5, rt.right, rt.bottom ); }

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC) { return false; }
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};