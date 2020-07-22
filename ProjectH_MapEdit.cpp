// ProjectH_MapEdit.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//


#include "stdafx.h"

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>


#include "afxwinappex.h"
#include "ProjectH_MapEdit.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProjectH_MapEditApp

BEGIN_MESSAGE_MAP(CProjectH_MapEditApp, CWinApp)
	ON_COMMAND( ID_ProjectH_Acc_New, &CProjectH_MapEditApp::New )
	ON_COMMAND( ID_ProjectH_Acc_Open, &CProjectH_MapEditApp::Open )
	ON_COMMAND( ID_ProjectH_Acc_Save, &CProjectH_MapEditApp::Save )
	ON_COMMAND( ID_ProjectH_Acc_AsSave, &CProjectH_MapEditApp::AsSave )
	ON_COMMAND( ID_ProjectH_Acc_Select, &CProjectH_MapEditApp::Select )
	ON_COMMAND( ID_ProjectH_Acc_Brush, &CProjectH_MapEditApp::Brush )
	ON_COMMAND( ID_ProjectH_Acc_Eraser, &CProjectH_MapEditApp::Eraser )
	ON_COMMAND( ID_ProjectH_Acc_ElevationUp, &CProjectH_MapEditApp::ElevationUp )
	ON_COMMAND( ID_ProjectH_Acc_ElevationDown, &CProjectH_MapEditApp::Elevationdown )
	ON_COMMAND( ID_ProjectH_Acc_SHBrushPalette, &CProjectH_MapEditApp::SHBrushPalette )
	ON_COMMAND( ID_ProjectH_Acc_Help, &CProjectH_MapEditApp::Help )
	ON_COMMAND( ID_ProjectH_Acc_WideView, &CProjectH_MapEditApp::WideView )
	ON_COMMAND( ID_ProjectH_Acc_MiniMapOnOff, &CProjectH_MapEditApp::MinimapOnOff )
END_MESSAGE_MAP()


// CProjectH_MapEditApp 생성

CProjectH_MapEditApp::CProjectH_MapEditApp()
{

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.

	

}

// 유일한 CProjectH_MapEditApp 개체입니다.

CProjectH_MapEditApp theApp;


// CProjectH_MapEditApp 초기화

BOOL CProjectH_MapEditApp::InitInstance()
{
	/*_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );*/
	//_CrtSetBreakAlloc(494);
	//_CrtSetBreakAlloc(4454);

	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));

	// 주 창을 만들기 위해 이 코드에서는 새 프레임 창 개체를
	// 만든 다음 이를 응용 프로그램의 주 창 개체로 설정합니다.
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 프레임을 만들어 리소스와 함께 로드합니다.
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);






	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	//  SDI 응용 프로그램에서는 ProcessShellCommand 후에 이러한 호출이 발생해야 합니다.
	return TRUE;
}


// CProjectH_MapEditApp 메시지 처리기




// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CProjectH_MapEditApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CProjectH_MapEditApp 메시지 처리기




BOOL CProjectH_MapEditApp::OnIdle(LONG lCount)
{

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	if ( directXComplete )
	{
		pFrame->GetView()->RedrawWindow();
		return true;
	}

	return CWinApp::OnIdle(lCount);
}

void CProjectH_MapEditApp::New()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.NewFile();
}

void CProjectH_MapEditApp::Open()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.OpenFile();
}

void CProjectH_MapEditApp::Save()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.SaveFile();
}

void CProjectH_MapEditApp::AsSave()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.AsSaveFile();
}

void CProjectH_MapEditApp::Select()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.CheckButton( TEXT( "TOOL_SELECT" ) );
}

void CProjectH_MapEditApp::Brush()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.CheckButton( TEXT( "TOOL_BRUSH" ) );
}

void CProjectH_MapEditApp::Eraser()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.CheckButton( TEXT( "TOOL_ERASER" ) );
}

void CProjectH_MapEditApp::ElevationUp()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.CheckButton( TEXT( "TOOL_ELEVATION_UP" ) );
}

void CProjectH_MapEditApp::Elevationdown()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.CheckButton( TEXT( "TOOL_ELEVATION_DOWN" ) );
}

void CProjectH_MapEditApp::SHBrushPalette()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	BOOL b = pFrame->m_DlgBrushPalette->IsWindowVisible();
	pFrame->m_DlgBrushPalette->ShowWindow( !b );
	
	CChildView* view = (CChildView*) pFrame->GetView();
	view->SetFocus();
}

void CProjectH_MapEditApp::Help()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.AboutProgram();
}

void CProjectH_MapEditApp::WideView()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChildView* view = (CChildView*) pFrame->GetView();
	view->CreateWideView();
}

int CProjectH_MapEditApp::Run()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CWinApp::Run();
}

void CProjectH_MapEditApp::MinimapOnOff()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChildView* view = (CChildView*) pFrame->GetView();
	view->MiniMapOnOff();
}