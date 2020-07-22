#pragma once
#include "afxwin.h"


// About 대화 상자입니다.

class About : public CDialog
{
	DECLARE_DYNAMIC(About)

public:
	About(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~About();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit thanksTo;
	virtual BOOL OnInitDialog();
	CEdit comment;
};
