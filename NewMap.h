#pragma once
#include "afxwin.h"


// NewMap 대화 상자입니다.

class NewMap : public CDialog
{
	DECLARE_DYNAMIC(NewMap)

public:
	NewMap(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~NewMap();


	int widthVal;
	int heightVal;
	int typeVal;

	int GetWidth() { return widthVal; }
	int GetHeight() { return heightVal; }
	int GetMapType() { return typeVal; }

// 대화 상자 데이터입니다.
	enum { IDD = NEWMAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox width;
	CComboBox height;
	CComboBox mapType;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
