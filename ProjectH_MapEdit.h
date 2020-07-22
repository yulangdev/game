
// ProjectH_MapEdit.h : ProjectH_MapEdit 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.


// CProjectH_MapEditApp:
// 이 클래스의 구현에 대해서는 ProjectH_MapEdit.cpp을 참조하십시오.
//

class CProjectH_MapEditApp : public CWinApp
{
public:
	CProjectH_MapEditApp();

	afx_msg void New();
	afx_msg void Open();
	afx_msg void Save();
	afx_msg void AsSave();
	afx_msg void Select();
	afx_msg void Brush();
	afx_msg void Eraser();
	afx_msg void ElevationUp();
	afx_msg void Elevationdown();
	afx_msg void SHBrushPalette();
	afx_msg void WideView();
	afx_msg void Help();
	afx_msg void MinimapOnOff();


// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
	virtual int Run();
};

extern CProjectH_MapEditApp theApp;
