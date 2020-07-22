#pragma once

class BlackWindow : public CWnd
{
	DECLARE_DYNAMIC(BlackWindow)

public:
	BlackWindow() {}
	virtual ~BlackWindow() {}

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


