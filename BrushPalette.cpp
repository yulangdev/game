// BrushPalette.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ProjectH_MapEdit.h"
#include "BrushPalette.h"

#include <fstream>
#include <iostream>
#include <cmath>


#define ProjectH_ID_Palette_ComboBox		1229
#define ProjectH_ID_Palette_RadioButton		1231
#define ProjectH_ID_Palette_RadioButton2	1232

using std::list;
using std::vector;
using std::string;
using std::ifstream;
using std::getline;

// BrushPalette 대화 상자입니다.

IMPLEMENT_DYNAMIC(BrushPalette, CDialog)

BEGIN_MESSAGE_MAP(BrushPalette, CDialog)
	ON_CBN_SELCHANGE( ProjectH_ID_Palette_ComboBox, OnComboBoxChange )
	ON_BN_CLICKED( ProjectH_ID_Palette_RadioButton, OnRadioButtonClicked )
	ON_WM_CREATE()
	ON_WM_MOVE()
END_MESSAGE_MAP()

BrushPalette::BrushPalette(CWnd* pParent /*=NULL*/)
	: CDialog(BrushPalette::IDD, pParent)
{

}

BrushPalette::~BrushPalette()
{
	navigation->DestroyWindow();
	delete navigation;
	navigation = NULL;

	blackBg->DestroyWindow();
	delete blackBg;
	blackBg = NULL;

	ReleaseButtonList(true);
}

void BrushPalette::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

// BrushPalette 메시지 처리기입니다.

int BrushPalette::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	selectedBtnIdx = 0;
	selectedBtnKey = 0;
	playerBtnClicked = false;

	blackBg = new BlackWindow();
	blackBg->Create( NULL, NULL, WS_CHILD | WS_VISIBLE, CRect( 5, 5, 205, 205 ), this, 1233, NULL );

	navigation = new Navigation();
	navigation->Create( NULL, NULL, WS_CHILD | WS_VISIBLE, CRect( 5, 5, 205, 205 ), this, 1234, NULL );

	return 0;
}

/***********************************
 * 버튼 이미지 관련 메소드 시작 *
 ***********************************/

/* 사용자가 넘겨준 img, type 정보를 가지고 전체 이미지를 로드하고
 * 버튼정보를 읽어들이며, 마지막으로 버튼을 생성한다.
 * 이 메소드는 ReadOverallImage(), ReadImgInfo(), CreateButtons() 메소드
 * 를 순서대로 호출한다.
 */
void BrushPalette::LoadImgAndCreateBtns()
{
	m_TileImage.Destroy();
	ReadOverallImage();

	if (Buttons.empty() == false)
		ReleaseButtonList();
	ReadImgInfo(Buttons);

	CreateButtons(Buttons);
}

/* 사용자가 넘겨준 img에 type으로 넘어온 타입의 이미지를
 * 불러들임
 */
void BrushPalette::ReadOverallImage()
{
	OSVERSIONINFOEX osvi;
	ZeroMemory( &osvi, sizeof( OSVERSIONINFOEX ) );
	osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );

	if( GetVersionEx( (OSVERSIONINFO *)&osvi ) )
	{
		osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
		GetVersionEx( (OSVERSIONINFO *)&osvi );
	}



	CString str( IMGRESOURCE()->GetBtnImgPath().c_str() );
	if ( osvi.dwMajorVersion == 6 )
		str.Replace( TEXT( "bmp" ), TEXT( "png" ) );

	m_TileImage.Load( str );
}

/* type에 따른 이미지 정보 텍스트 파일을 읽어들인 다음, 사용자가
 * 넘겨준 리스트 coll에 버튼 이미지정보를 생성하여 저장한다.
 */
bool BrushPalette::ReadImgInfo(list<BtnImgInfo*> &coll)
{
	/* 버튼 이미지에 쓰이는 이미지들의 정보를 모두 얻어옴 */
	vector<ProjectH::ImgResource::ImageData> BtnInfo;
	vector<int> BtnIdx;
	IMGRESOURCE()->GetBtnImgData(BtnInfo, BtnIdx);

	Buttons.resize(BtnInfo.size());

	int i = 0;
	for (list<BtnImgInfo*>::iterator iter = Buttons.begin(); iter != Buttons.end(); ++iter, ++i)
	{
		(*iter)				= new BtnImgInfo();
		(*iter)->button		= 0;
		(*iter)->ComboIdx	= BtnInfo[i].isBtn;
		(*iter)->ImgIdx		= BtnIdx[i];
		(*iter)->x			= BtnInfo[i].x;
		(*iter)->y			= BtnInfo[i].y;
		(*iter)->w			= BtnInfo[i].width;
		(*iter)->h			= BtnInfo[i].height;
	}
	return true;
}

/* 유저가 넘겨준 버튼 정보를 기초로 하여 버튼을
 * 생성한다.
 */
bool BrushPalette::CreateButtons(list<BtnImgInfo*> &btnLists)
{
	list<BtnImgInfo*>::iterator iter;
	CButton *btn = 0;
	CImage* image;
	
	int i = 0;
	for (iter = btnLists.begin(); iter != btnLists.end(); ++iter)
	{
		image = new CImage;
		btn = new CButton();
		btn->Create(TEXT("Button"), WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE | BS_BITMAP,
					CRect(0, 0, 0, 0), this, ProjectH_ID_Palette_RadioButton);
		
		float viewRatio;
		if( (*iter)->h > (*iter)->w )
			viewRatio = (*iter)->h / 48.0f;
		else
			viewRatio = (*iter)->w / 64.0f;

		int h = static_cast<int>( (*iter)->h / viewRatio );
		int w = static_cast<int>( (*iter)->w / viewRatio );

		int x = static_cast<int>( ( 64.0f - w ) /2 );
		int y = static_cast<int>( ( 48.0f - h ) /2 );

		image->Create(w, h, m_TileImage.GetBPP());
		HDC dc = image->GetDC();

		if ( (*iter)->h <= 48 )
		{
			m_TileImage.StretchBlt(	dc,
									0, 0, w, h,
									(*iter)->x, (*iter)->y, (*iter)->w, (*iter)->h);
		}
		else
		{
			m_TileImage.StretchBlt(	dc,
									0, 0, w, h,
									(*iter)->x, (*iter)->y, (*iter)->w, (*iter)->h);
		}

		image->ReleaseDC();
		btn->SetBitmap(*image);
		(*iter)->button = btn;

		buttonsImg.push_back( image );
		++i;
	}
	return true;
}

/* 탭을 구분자로 하여서 문자열을 쪼갬 */
inline void BrushPalette::SplitString(string str, vector<string> &coll)
{
	string::iterator startPos, endPos;

	startPos = str.begin();
	endPos = startPos;
	while (endPos != str.end())
	{
		if (*endPos == '\t' && endPos != str.begin())
		{
			coll.push_back(string(startPos, endPos));
			startPos = endPos + 1;
		}
		++endPos;
		if (endPos == str.end())
			coll.push_back(string(startPos, endPos));
	}
}

inline bool BrushPalette::CheckLine(vector<string> &str)
{
	return (str.size() == 6);
}

inline void BrushPalette::AdjustBtnPosition()
{
	RECT clientRect;
	GetClientRect( &clientRect );
	int x = clientRect.right;

	int countX = 0, ColumnLength = clientRect.right / 64;
	int startX, startY;
	startX = 5;
	startY = 270;

	CString str;
	m_ComboBox.GetWindowText( str );
	if ( !( str.Compare( L"시작지점" ) ) )
	{
		const int w = 67, h = 50;
		for (int i = 0; i < static_cast<int>(playerBtns.size()); ++i)
			playerBtns[ i ]->MoveWindow(startX + (w *(i %3)), startY +( h *( i /3)), w, h);

		GetWindowRect( &clientRect );
		MoveWindow( clientRect.left, clientRect.top, clientRect.right - clientRect.left, 4* 48 + startY );
	}
	else
	{
		for (int i = 0; i < static_cast<int>(playerBtns.size()); ++i)
			playerBtns[ i ]->MoveWindow( 0, 0, 0, 0 );
	}

	if ( !( str.Compare( L"몬스터" ) ) )
	{
		const int w = 96, h = 90;
		for (int i = 0; i < static_cast<int>(monsterBtns.size()); ++i)
			monsterBtns[ i ]->MoveWindow(startX + (w *(i %2)) + (i %2 *5), startY +( h *( i /2)), w, h);

		GetWindowRect( &clientRect );
		MoveWindow( clientRect.left, clientRect.top, clientRect.right - clientRect.left, ( monsterBtns.size()/2 + monsterBtns.size()%2 )*h + startY + 35 );
	}
	else
	{
		for (int i = 0; i < static_cast<int>(monsterBtns.size()); ++i)
			monsterBtns[ i ]->MoveWindow( 0, 0, 0, 0 );
	}
	
	CRect btnRt;
	int currentX = startX;
	int currentY = startY;
	for ( list<BtnImgInfo*>::iterator iter = Buttons.begin(); iter != Buttons.end(); ++iter)
	{
		if (m_ComboBox.GetCurSel() + 1 == (*iter)->ComboIdx)
		{
			if (countX == 0)
				(*iter)->button->SetCheck(BST_CHECKED);

			countX++;

			(*iter)->button->MoveWindow(currentX, currentY, 66, 50);
			currentX += 67;
			if ((currentX + 67) >= x)
			{
				currentX = startX;
				currentY += 50;
			}
		}
		else
		{
			if ((*iter)->button->GetCheck())
				(*iter)->button->SetCheck(BST_UNCHECKED);
			(*iter)->button->MoveWindow( 0, 0, 0, 0 );
		}
	}
	OnRadioButtonClicked();

	if ( str.Compare( L"시작지점" ) && str.Compare( L"몬스터" ) )
	{
		int nRow = ( countX % ColumnLength == 0 ? countX / ColumnLength + 1 : countX / ColumnLength + 2 );
		GetWindowRect( &clientRect );
		MoveWindow( clientRect.left, clientRect.top, clientRect.right - clientRect.left, nRow * 48 + startY );
	}
}

/*********************************
 * 버튼 이미지 관련 메소드 끝 *
 *********************************/

void BrushPalette::ReleaseButtonList(bool releaseStartPoint)
{
	for ( std::list< CImage* >::iterator iter = buttonsImg.begin(); iter != buttonsImg.end(); ++iter )
	{
		(*iter)->Destroy();
		delete (*iter);
	}

	for ( std::list< CImage* >::iterator iter = playerBtnsImg.begin(); iter != playerBtnsImg.end(); ++iter )
	{
		(*iter)->Destroy();
		delete (*iter);
	}

	list<BtnImgInfo*>::iterator iter;
	for (iter = Buttons.begin(); iter != Buttons.end(); ++iter)
	{
		if ((*iter)->button != 0)
			delete (*iter)->button;

		delete (*iter);
	}
	Buttons.clear();

	if (releaseStartPoint)
	{
		vector<CButton*>::iterator iter2;
		for (iter2 = playerBtns.begin(); iter2 != playerBtns.end(); ++iter2)
		{
			if ((*iter2) != 0)
				delete (*iter2);
		}
	}
}

BOOL BrushPalette::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 윈도우 반투명하게
	OSVERSIONINFOEX osvi;
	ZeroMemory( &osvi, sizeof( OSVERSIONINFOEX ) );
	osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );

	if( GetVersionEx( (OSVERSIONINFO *)&osvi ) )
	{
		osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
		GetVersionEx( (OSVERSIONINFO *)&osvi );
	}

	if ( osvi.dwMajorVersion == 6 )
	{
		int alphaValue = 222;
		typedef BOOL(WINAPI *SLWA)(HWND,COLORREF,BYTE,DWORD);
		SLWA pSetLayeredWindowAttributes = NULL;

		HINSTANCE hmodUSER32 = LoadLibrary( TEXT( "USER32.DLL" ) );
		pSetLayeredWindowAttributes = (SLWA)GetProcAddress( hmodUSER32, "SetLayeredWindowAttributes" );

		SetWindowLong( GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong( GetSafeHwnd(), GWL_EXSTYLE ) | 0x00080000 );
		SetLayeredWindowAttributes( RGB( 240, 240, 240 ), alphaValue, LWA_ALPHA );
	}

	// 윈도우 크기 설정
	MoveWindow( 100, 100, 215, 0 );

	/* StartPoint 이미지 로드 */
	LoadStartPointImage();

	/* Monster 이미지 로드 */
	LoadMonsterImage();

	// 타일 및 악세서리 구분 콤보박스 초기화
	int nCombo = IMGRESOURCE()->NumberOfComboBox();
	m_ComboBox.Create( WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_PUSHED,
						CRect( 5, 240, 205, 0 ), this, ProjectH_ID_Palette_ComboBox );

	for (int i = 0; i < nCombo; ++i)
	{
		const char* c = IMGRESOURCE()->ComboBoxName(i).c_str();
		wchar_t w[256];

		memset( w, 0, sizeof(w) );
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, c, strlen(c), w, strlen(c));

		CString str( w );
		m_ComboBox.AddString(str);
	}
	m_ComboBox.AddString( L"시작지점" );
	m_ComboBox.AddString( L"몬스터" );
	m_ComboBox.SendMessage( CB_SETCURSEL, 0, 0 );	// 첫번쨰 아이템 선택

	// 타일버튼 초기화
	LoadImgAndCreateBtns();
	AdjustBtnPosition();
	AdjustStartPointPos();

	// 오토타일링 체크버튼 초기화
	m_BtnCheckAutoTiling.Create(TEXT("AutoTiling"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
					CRect(5, 210, 205, 232), this, 1111 );

	m_BtnCheckAutoTiling.SetCheck( 1 );

	return TRUE;
}

void BrushPalette::OnRadioButtonClicked()
{
	playerBtnClicked = true;
	monsterBtnClicked = true;

	enum { MAJOR_TILE = 0, AROUND_TILE, GENERAL_TILE, ACCESSORY, PILLAR };
	list<BtnImgInfo*>::iterator iter;
	for (iter = Buttons.begin(); iter != Buttons.end(); ++iter)
	{
		if ((*iter)->button->GetCheck())
		{
			selectedBtnIdx = (*iter)->ImgIdx;
			selectedBtnKey = IMGRESOURCE()->GetTileType((*iter)->ImgIdx);
			playerBtnClicked = false;
			monsterBtnClicked = false;
			break;
		}
	}

	if ( playerBtnClicked )
	{
		for (int i = 0; i < 9; ++i)
		{
			if (playerBtns[i]->GetCheck())
			{
				selectedPlayer = i;
				playerBtnClicked = true;
				monsterBtnClicked = false;
			}
		}
	}

	if ( monsterBtnClicked )
	{
		for ( int i = 0; i < 2; ++i )
		{
			if ( monsterBtns[i]->GetCheck() )
			{
				// 몬스터는 3번부터라 +3
				selectedMonster = i+3;
				playerBtnClicked = false;
				monsterBtnClicked = true;
			}
		}
	}

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_DlgToolBar.CheckButton( TEXT( "TOOL_BRUSH" ) );
}

BOOL BrushPalette::PreTranslateMessage(MSG* pMsg)
{
	// ESC, ENTER, ALT + F4 로 종료 방지
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) 
		return TRUE;

	else if(pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
		return TRUE;

	// 툴바 단축키
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	switch ( pMsg->wParam )
	{
		case 's':
		case 'v':
		{
			pFrame->m_DlgToolBar.CheckButton( TEXT( "TOOL_SELECT" ) );
			return TRUE;
		}
		case 'b':
		{
			pFrame->m_DlgToolBar.CheckButton( TEXT( "TOOL_BRUSH" ) );
			return TRUE;
		}
		case 'e':
		{
			pFrame->m_DlgToolBar.CheckButton( TEXT( "TOOL_ERASER" ) );
			return TRUE;
		}
		case 'u':
		{
			pFrame->m_DlgToolBar.CheckButton( TEXT( "TOOL_ELEVATION_UP" ) );
			return TRUE;
		}
		case 'd':
		{
			pFrame->m_DlgToolBar.CheckButton( TEXT( "TOOL_ELEVATION_DOWN" ) );
			return TRUE;
		}
		case 'h':
		{
			pFrame->m_DlgToolBar.AboutProgram();
			return TRUE;
		}
		case 'm':
		{
			CChildView* view = (CChildView*) pFrame->GetView();
			view->CreateWideView();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void BrushPalette::LoadStartPointImage()
{
	OSVERSIONINFOEX osvi;
	ZeroMemory( &osvi, sizeof( OSVERSIONINFOEX ) );
	osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );

	if( GetVersionEx( (OSVERSIONINFO *)&osvi ) )
	{
		osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
		GetVersionEx( (OSVERSIONINFO *)&osvi );
	}

	CString str( "BtnImage/BtnStartPoint.bmp" );
	if ( osvi.dwMajorVersion == 6 )
		str.Replace( TEXT( "bmp" ), TEXT( "png" ) );
	
	startPoint.Load( str );
	CreateStartPointBtns();
}

void BrushPalette::CreateStartPointBtns()
{
	const int w = 64, h = 32;
	CImage* image;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			image = new CImage;
			CButton *temp = new CButton;
			temp->Create(TEXT("Button"), WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE | BS_BITMAP,
			CRect(0, 0, 0, 0), this, ProjectH_ID_Palette_RadioButton);

			image->Create(w, h, startPoint.GetBPP());
			HDC dc = image->GetDC();

			startPoint.StretchBlt(dc, 0, 0, 64, 32, j * w, i * h, w, h);
			image->ReleaseDC();
			temp->SetBitmap(*image);
			playerBtns.push_back(temp);

			playerBtnsImg.push_back( image );
		}
	}
}

void BrushPalette::AdjustStartPointPos()
{

}
void BrushPalette::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
}

void BrushPalette::Move( int x, int y )
{
	CRect rt;
	GetWindowRect( &rt );

	MoveWindow( rt.left - x, rt.top - y, rt.right - rt.left, rt.bottom - rt.top );
}

void BrushPalette::LoadMonsterImage()
{
	OSVERSIONINFOEX osvi;
	ZeroMemory( &osvi, sizeof( OSVERSIONINFOEX ) );
	osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );

	if( GetVersionEx( (OSVERSIONINFO *)&osvi ) )
	{
		osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
		GetVersionEx( (OSVERSIONINFO *)&osvi );
	}

	CString str( "BtnImage/BtnMonster.bmp" );
	if ( osvi.dwMajorVersion == 6 )
		str.Replace( TEXT( "bmp" ), TEXT( "png" ) );
	
	monster.Load( str );
	CreateMonsterBtns();
}

void BrushPalette::CreateMonsterBtns()
{
	const int w = 96, h = 90;
	CImage* image;

	for (int i = 0; i < 2; ++i)
	{
		image = new CImage;
		CButton *temp = new CButton;
		temp->Create(TEXT("Button"), WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_PUSHLIKE | BS_BITMAP,
		CRect(0, 0, 0, 0), this, ProjectH_ID_Palette_RadioButton);

		image->Create(w, h, startPoint.GetBPP());
		HDC dc = image->GetDC();

		monster.StretchBlt(dc, 0, 0, 96, 90, i *w, i /2 *h, w, h);
		image->ReleaseDC();
		temp->SetBitmap(*image);
		monsterBtns.push_back(temp);

		monsterBtnsImg.push_back( image );
	}
}