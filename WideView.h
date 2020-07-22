#pragma once


// WideView 대화 상자입니다.

class WideView : public CDialog
{
	DECLARE_DYNAMIC(WideView)

public:
	WideView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~WideView();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
