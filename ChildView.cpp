#include "stdafx.h"
#include "WideView.h"
#include "ChildView.h"
#include "DirectX9.h"
#include "Point.h"
#include "ImgResource.h"

#include <list>

using ProjectH::Map;
using ProjectH::Point;
using std::list;
using ProjectH::ImgResource;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChildView::CChildView()
{

}

CChildView::~CChildView()
{

}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 멤버변수 초기화
	map = NULL;
	drag = false;
	miniMap = true;
	directXComplete = false;

	// 맵 생성
	Init();

	// 현재 주 모니터의 해상도를 얻어온다.
	int screenWidth = GetSystemMetrics( SM_CXSCREEN );
	int screenHeight = GetSystemMetrics( SM_CYSCREEN );

	// 맵의 전체 픽셀 크기를 얻어온다.
	int mapWidth = map->GetWidth();
	int mapHeight = map->GetHeight();

	// 맵의 전체 크기에 따른 미니맵 크기의 비율을 구한다.
	if ( mapWidth > mapHeight )
		ratio = 200.0f / mapWidth;
	else
		ratio = 200.0f / mapHeight;

	// 비율에 따라 미니맵의 크기를 구한다.
	int miniMapWidth = (int)( mapWidth * ratio );
	int miniMapHeight = (int)( mapHeight * ratio );

	if ( !DIRECTX9()->Init( GetSafeHwnd(), screenWidth, screenHeight, 200.0f, 200.0f, ratio ) )
	{
		TRACE( TEXT( "다이렉트X 를 초기화하지 못했습니다." ) );
		return -1;
	}
	directXComplete = true;

	return 0;
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// 스크롤바 재설정
	SetScrollBar();
}

void CChildView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// 세로 스크롤바 재설정
	if ( nSBCode == SB_THUMBPOSITION )
	{
		vScrollInfo.nPos = LimitVScrollPos( nPos );
		SetScrollInfo( SB_VERT, &vScrollInfo );

		// 미니맵 사각형 크기 재설정
		SetMiniMapRect();
	}

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CChildView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// 가로 스크롤바 재설정
	if ( nSBCode == SB_THUMBPOSITION )
	{
		hScrollInfo.nPos = LimitHScrollPos( nPos );
		SetScrollInfo( SB_HORZ, &hScrollInfo );

		// 미니맵 사각형 크기 재설정
		SetMiniMapRect();
	}

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CChildView::OnDestroy()
{
	Release();

	CWnd::OnDestroy();
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this);

	if( directXComplete )
	{
		// 매인화면 출력
		OnMousePosition();
		DIRECTX9()->BeginDrawRect( this, &CChildView::Render, &clientRect, GetSafeHwnd() );

		// 미니맵 출력
		if ( miniMap )
		{
			CMainFrame* pFrame = (CMainFrame*) AfxGetMainWnd();
			HWND hWnd2 = pFrame->m_DlgBrushPalette->navigation->GetSafeHwnd();
			DIRECTX9()->BeginDrawAll( this, &CChildView::RenderAll, &rectangle, hWnd2, ratio );
		}
	}
}

void CChildView::Render()
{
	map->SetDrawRect( hScrollInfo.nPos, vScrollInfo.nPos, hScrollInfo.nPos + clientRect.right, vScrollInfo.nPos + clientRect.bottom );

	Map::DrawData data;
	while ( map->NextTile( data ) )
	{
		// 타일의 타입과 형식에 따른 그릴 이미지의 좌표 얻어오기
		ImgResource::ImageData imgData;
		IMGRESOURCE()->GetDoodadData( data.imgIndex, imgData );

		// 타일 이미지를 그린다.
		DIRECTX9()->Draw( data.mapType,
						imgData.x, imgData.y, imgData.width, imgData.height,
						data.tileDrawX, data.tileDrawY, data.indexY );

		// 기둥 이미지를 그린다.
		if ( data.tileElevation > 0 )
		{
			IMGRESOURCE()->GetPillar( data.tileType, imgData );

			for ( int i = 2; i <= data.tileElevation; ++i )
			{
				DIRECTX9()->Draw( data.mapType,
								imgData.x, imgData.y, imgData.width, imgData.height,
								data.tileDrawX, data.tileDrawY + (imgData.centerPointY - imgData.y) * i, data.indexY );
			}
		
			// 타일과 기둥을 연결하는 기둥 이미지를 그린다.
			IMGRESOURCE()->GetPillar( data.tileType, imgData, true );
			DIRECTX9()->Draw( data.mapType,
							imgData.x, imgData.y, imgData.width, imgData.height,
							data.tileDrawX, data.tileDrawY + (imgData.centerPointY - imgData.y), data.indexY );
		}

		if ( data.startPoint )
		{
			if ( data.startPlayer != -1 )
			{
				DIRECTX9()->Draw( 101,
								0, 0 + data.startPlayer * 32, 64, 32 + data.startPlayer * 32,
								data.tileDrawX, data.tileDrawY, data.indexY );
			}
			else
			{
				if ( data.playerTyep == 3 )
				{
					DIRECTX9()->Draw( 104,
									0, 0, 100, 100,
									data.tileDrawX - 18, data.tileDrawY - 64, data.indexY );
				}
				else
				{
					DIRECTX9()->Draw( 104,
									100, 0, 200, 100,
									data.tileDrawX - 18, data.tileDrawY - 64, data.indexY );
				}
			}
		}

		// 장식물 그리기
		int doodadHeight = 0;
		for ( std::list< int >::iterator i = data.doodad->begin(); i != data.doodad->end(); ++i )
		{
			IMGRESOURCE()->GetDoodadData( *i, imgData );
			DIRECTX9()->Draw( data.mapType,
							imgData.x, imgData.y, imgData.width, imgData.height,
							data.tileDrawX - imgData.centerPointX, data.tileDrawY - imgData.centerPointY - doodadHeight,
							data.indexY );

			doodadHeight += imgData.centerPointY;
		}
	}

	Point drawPoint;
	map->GetDrawMousePoint( drawPoint );
	DIRECTX9()->Draw( 102, 0, 0, 64, 32, drawPoint.x ,drawPoint.y );
}

void CChildView::SetPosition( int x, int y )
{
	// 네비게이션 좌표를 뷰 화면 좌표로 변환
	int sx = static_cast<int>( x / ratio );
	int sy = static_cast<int>( y / ratio );

	// 좌표를 중심으로 설정
	sx -= clientRect.right /2;
	sy -= clientRect.bottom /2;

	// 변환된 좌표로 스크롤바 위치 설정
 	sy = ( sy < 0 ? 0 : sy );
	sy = static_cast<int>( ( sy > vScrollInfo.nMax - static_cast<int>(vScrollInfo.nPage) ?
							vScrollInfo.nMax - static_cast<int>(vScrollInfo.nPage) : sy ) );
	vScrollInfo.nPos = sy;
	SetScrollInfo( SB_VERT, &vScrollInfo );

	sx = ( sx < 0 ? 0 : sx );
	sx = static_cast<int>( ( sx > hScrollInfo.nMax - static_cast<int>(hScrollInfo.nPage) ?
							hScrollInfo.nMax - static_cast<int>(hScrollInfo.nPage) : sx ) );
	hScrollInfo.nPos = sx;
	SetScrollInfo( SB_HORZ, &hScrollInfo );

	// 미니맵 사각형 크기 재설정
	SetMiniMapRect();
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	OnMouse( nFlags, point, true );

	drag = true;
	SetCapture();

	dragStart = point;

	CWnd::OnLButtonDown(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	OnMouse( nFlags, point );

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CString str = pFrame->m_DlgToolBar.GetSelectTool();

	if ( drag && !str.Compare( TEXT( "TOOL_SELECT" ) ) )
	{
		if ( point.x < 0 || point.x > clientRect.right || point.y < 0 || point.y > clientRect.bottom )
		{
			drag = false;
			ReleaseCapture();
		}
		else
		{
			int sx = LimitHScrollPos( hScrollInfo.nPos + dragStart.x - point.x );
			int sy = LimitVScrollPos( vScrollInfo.nPos + dragStart.y - point.y );

			vScrollInfo.nPos = sy;
			SetScrollInfo( SB_VERT, &vScrollInfo );

			hScrollInfo.nPos = sx;
			SetScrollInfo( SB_HORZ, &hScrollInfo );

			// 미니맵 사각형 크기 재설정
			SetMiniMapRect();

			dragStart = point;
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	drag = false;
	ReleaseCapture();

	CWnd::OnLButtonUp(nFlags, point);
}

void CChildView::OnMousePosition()
{
	CPoint point;
	GetCursorPos( &point );
	ScreenToClient( &point );
	map->SetIndex( hScrollInfo.nPos + point.x, vScrollInfo.nPos + point.y );
}

void CChildView::OnMouse( UINT nFlags, CPoint point, bool lButtonDown )
{
	if ( map->SetIndex( hScrollInfo.nPos + point.x, vScrollInfo.nPos + point.y ) || lButtonDown )
	{
		if ( nFlags == MK_LBUTTON )
		{
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			CString str = pFrame->m_DlgToolBar.GetSelectTool();

			if ( !str.Compare( TEXT( "TOOL_BRUSH" ) ) )
				Brush();

			else if ( !str.Compare( TEXT( "TOOL_ERASER" ) ) )
				Eraser();

			else if ( !str.Compare( TEXT( "TOOL_ELEVATION_UP" ) ) )
				ElevationUP();

			else if ( !str.Compare( TEXT( "TOOL_ELEVATION_DOWN" ) ) )
				ElevationDown();
		}
	}
}

void CChildView::Brush()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	// 몬스터일 경우 플레이어 번호는 -1, 몬스터는 3번부터 시작함으로 +3
	// 캐릭터일 경우 플레이어 번호와, 캐릭터는 로딩시 결정되므로 0 고정
	if ( pFrame->m_DlgBrushPalette->MonsterBtnClicked() )
	{
		int monster = pFrame->m_DlgBrushPalette->GetSelectedMonster();
		map->SetStartPoint( -1, monster );
	}
	else if ( pFrame->m_DlgBrushPalette->PlayerBtnClicked() )
	{
		int player = pFrame->m_DlgBrushPalette->GetSelectedPlayer();
		map->SetStartPoint( player, 0 );
	}
	else
	{
		int index = pFrame->m_DlgBrushPalette->GetSelectedIndex();
		int key = pFrame->m_DlgBrushPalette->GetSelectedKey();

		if ( IMGRESOURCE()->IsMajorTile( index ) )
		{
			if ( pFrame->m_DlgBrushPalette->IsAutoTiling() )
				map->SetTile( index );
			else
				map->AutoTiling( key );
		}
		else if ( IMGRESOURCE()->IsAroundTile( index ) )
		{
			map->SetTile( index );
		}
		else if ( IMGRESOURCE()->IsDoodad( index ) )
		{
			map->AddDoodad( index );
		}
	}
}

void CChildView::ElevationUP()
{
	map->TileElevationUp();
}

void CChildView::ElevationDown()
{
	map->TileElevationDown();
}

bool CChildView::Init( int _width, int _height, int _type, char* filePath )
{
	directXComplete = false;
	
	/* 처음 생성되는 경우에만 Map을 할당한다. */
	if ( map == NULL )
	{
		map = new Map( _width, _height );
	}
	else if ( filePath )	/* Load */
	{
		if ( !( map->Load( filePath ) ) )
			return false;
	}
	else					/* New */
	{
		map->Reset( _width, _height, _type );
	}

	// 맵 테마 설정
	IMGRESOURCE()->SetTheme( _type );

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	//pFrame->m_DlgBrushPalette->LoadImgAndCreateBtns();
	//pFrame->m_DlgBrushPalette->OnInitDialog();
	pFrame->ResetBrushPallete();

	// 맵 크기 임시 저장
	int mapWidth = map->GetWidth();
	int mapHeight = map->GetHeight();

	// 두 뷰간의 비율 계산
	if ( mapWidth > mapHeight )
		ratio = 200.0f / mapWidth;
	else
		ratio = 200.0f / mapHeight;

	float h = mapHeight * ratio;
	float w = mapWidth * ratio;

	int x = (int) ( ( 200.0f - ( mapWidth * ratio ) ) /2 );
	int y = (int) ( ( 200.0f - ( mapHeight * ratio ) ) /2 );

	CRect navigationRect( x, y, (int)w, (int)h );
	
	// 미니 뷰 윈도우 핸들값 얻기
	HWND hWnd2 = pFrame->m_DlgBrushPalette->navigation->GetSafeHwnd();
	// 미니 뷰 크기 설정
	pFrame->m_DlgBrushPalette->navigation->Init( navigationRect );

	// 프레임 윈도우 크기 설정
	SetWindowSize();

	// 스크롤바 위치 재설정
	SetScrollBar();

	// 미니맵 사각형 크기 재설정
	SetMiniMapRect();

	directXComplete = true;

	return true;
}

void CChildView::Release()
{
	// 다이렉트X 릴리즈
	DIRECTX9()->Release();
	directXComplete = false;

	// 맵 릴리즈
	//map->Release();
	delete map;

	//_CrtDumpMemoryLeaks();
}

void CChildView::SetScrollBar()
{
	GetClientRect( &clientRect );
	clientRect.right = ( clientRect.right > map->GetWidth() ? map->GetWidth() : clientRect.right );
	clientRect.bottom = ( clientRect.bottom > map->GetHeight() ? map->GetHeight() : clientRect.bottom );

	ZeroMemory( &vScrollInfo, sizeof(vScrollInfo) );
	vScrollInfo.cbSize = sizeof( SCROLLINFO );
	vScrollInfo.fMask = SIF_ALL | SIF_DISABLENOSCROLL;

	GetScrollInfo( SB_HORZ, &vScrollInfo );	
	vScrollInfo.nMax = map->GetHeight();
	vScrollInfo.nPage = clientRect.bottom;
	vScrollInfo.nPos = LimitVScrollPos( vScrollInfo.nPos );
	SetScrollInfo( SB_VERT, &vScrollInfo );


	ZeroMemory( &hScrollInfo, sizeof(hScrollInfo) );
	hScrollInfo.cbSize = sizeof( SCROLLINFO );
	hScrollInfo.fMask = SIF_ALL | SIF_DISABLENOSCROLL;

	GetScrollInfo( SB_VERT, &hScrollInfo );
	hScrollInfo.nMax = map->GetWidth();
	hScrollInfo.nPage = clientRect.right;
	hScrollInfo.nPos = LimitHScrollPos( hScrollInfo.nPos );

	SetScrollInfo( SB_HORZ, &hScrollInfo );

	// 미니맵 사각형 크기 재설정
	SetMiniMapRect();
}

void CChildView::SetWindowSize()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	int mapWidth = map->GetWidth();
	int mapHeight = map->GetHeight();

	CRect wRt;
	CRect cRt;
	pFrame->GetWindowRect( &wRt );
	GetClientRect( &cRt );

	int width = ( mapWidth > cRt.right ? cRt.right : mapWidth + 2 );
	int height = ( mapHeight > cRt.bottom ? cRt.bottom : mapHeight + 1 );

	width = width + ( wRt.right - cRt.right - wRt.left );
	height =  height + ( wRt.bottom - cRt.bottom - wRt.top );

	pFrame->MoveWindow( wRt.left, wRt.top, width, height );
}

bool CChildView::Save( CString& str )
{
	char c[ MAX_PATH ];
	strcpy_s( c, CW2A( str.GetBuffer() ) );

	return map->Save( c );
}

void CChildView::Load( CString& str )
{
	char c[ MAX_PATH ];
	strcpy_s( c, CW2A( str.GetBuffer() ) );

	if ( !( Init( 0, 0, 0, c ) ) )
		Init();
}

void CChildView::ViewDrag( CPoint& point )
{
	int sx = point.x + hScrollInfo.nPos;
	int sy = point.y + vScrollInfo.nPos;

	sx -= LimitHScrollPos( clientRect.right /2 );
	sy -= LimitVScrollPos( clientRect.bottom /2 );

	vScrollInfo.nPos = sy;
	SetScrollInfo( SB_VERT, &vScrollInfo );

	hScrollInfo.nPos = sx;
	SetScrollInfo( SB_HORZ, &hScrollInfo );
}

int CChildView::LimitHScrollPos( int sx )
{
	sx = ( sx < 0 ? 0 : sx );
	sx = static_cast<int>( ( sx > hScrollInfo.nMax - static_cast<int>(hScrollInfo.nPage) ?
							hScrollInfo.nMax - static_cast<int>(hScrollInfo.nPage) : sx ) );

	return sx;
}

int CChildView::LimitVScrollPos( int sy )
{
	sy = ( sy < 0 ? 0 : sy );
	sy = static_cast<int>( ( sy > vScrollInfo.nMax - static_cast<int>(vScrollInfo.nPage) ?
							vScrollInfo.nMax - static_cast<int>(vScrollInfo.nPage) : sy ) );

	return sy;
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int x = 0, y = 0;

	if ( GetKeyState( VK_UP ) & 0x0800 )
		y -= 16;
	else if ( GetKeyState( VK_DOWN ) & 0x0800 )
		y += 16;

	if ( GetKeyState( VK_LEFT ) & 0x0800 )
		x -= 16;
	else if ( GetKeyState( VK_RIGHT ) & 0x0800 )
		x += 16;

	vScrollInfo.nPos = LimitVScrollPos( vScrollInfo.nPos + y );
	SetScrollInfo( SB_VERT, &vScrollInfo );

	hScrollInfo.nPos = LimitHScrollPos( hScrollInfo.nPos + x );
	SetScrollInfo( SB_HORZ, &hScrollInfo );

	//if ( nChar == '1' )
	//	map->SetStartPoint( 0 );

	//if ( nChar == '2' )
	//	map->SetStartPoint( 0 );

	//if ( nChar == '3' )
	//	map->SetStartPoint( 0 );

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::RenderAll()
{
	int mapWidthCount = map->GetTileWidthCount();
	int mapHeightCount = map->GetTileHeightCount();

	Map::DrawData data;
	for ( int y = 0; y < mapHeightCount; ++y )
	{
		for ( int x = 0; x < mapWidthCount; ++x )
		{
			map->GetDrawData( x, y, data );

			// 타일의 타입과 형식에 따른 그릴 이미지의 좌표 얻어오기
			ImgResource::ImageData imgData;
			IMGRESOURCE()->GetDoodadData( data.imgIndex, imgData );

			// 타일 이미지를 그린다.
			DIRECTX9()->Draw( data.mapType,
							imgData.x, imgData.y, imgData.width, imgData.height,
							data.tileDrawX, data.tileDrawY, data.indexY, 3 );

			// 기둥 이미지를 그린다.
			if ( data.tileElevation > 0 )
			{
				IMGRESOURCE()->GetPillar( data.tileType, imgData );

				for ( int i = 2; i <= data.tileElevation; ++i )
				{
					DIRECTX9()->Draw( data.mapType,
									imgData.x, imgData.y, imgData.width, imgData.height,
									data.tileDrawX, data.tileDrawY + (imgData.centerPointY - imgData.y) * i, data.indexY, 3 );
				}
			
				// 타일과 기둥을 연결하는 기둥 이미지를 그린다.
				IMGRESOURCE()->GetPillar( data.tileType, imgData, true );
				DIRECTX9()->Draw( data.mapType,
								imgData.x, imgData.y, imgData.width, imgData.height,
								data.tileDrawX, data.tileDrawY + (imgData.centerPointY - imgData.y), data.indexY, 3 );
			}

			if ( data.startPoint )
			{
				//RECT rt
				DIRECTX9()->Draw( 101,
								0, 0+(data.startPlayer * 32), 64, 32+(data.startPlayer * 32),
								data.tileDrawX, data.tileDrawY, data.indexY, 3 );
			}

			// 장식물 그리기
			int doodadHeight = 0;
			for ( std::list< int >::iterator i = data.doodad->begin(); i != data.doodad->end(); ++i )
			{
				IMGRESOURCE()->GetDoodadData( *i, imgData );
				DIRECTX9()->Draw( data.mapType,
								imgData.x, imgData.y, imgData.width, imgData.height,
								data.tileDrawX - imgData.centerPointX, data.tileDrawY - imgData.centerPointY - doodadHeight,
								data.indexY, 3 );

				doodadHeight += imgData.centerPointY;
			}
		}
	}
}

void CChildView::SetMiniMapRect()
{
	rectangle.left = (int)( hScrollInfo.nPos * ratio );
	rectangle.top = (int)( vScrollInfo.nPos * ratio );
	rectangle.right = (int)( ( hScrollInfo.nPos + clientRect.right ) * ratio );
	rectangle.bottom = (int)( ( vScrollInfo.nPos + clientRect.bottom ) * ratio );
}

void CChildView::CreateWideView()
{
	// 현재 주 모니터의 해상도를 얻어온다.
	int screenWidth = GetSystemMetrics( SM_CXSCREEN );
	int screenHeight = GetSystemMetrics( SM_CYSCREEN );

	// 미니맵 출력
	CMainFrame* pFrame = (CMainFrame*) AfxGetMainWnd();
	pFrame->m_DlgWideView.MoveWindow( 0, 0, screenWidth, screenHeight );
	pFrame->m_DlgWideView.ShowWindow( SW_SHOW );
	HWND hWnd2 = pFrame->m_DlgWideView.GetSafeHwnd();


	// 맵의 전체 픽셀 크기를 얻어온다.
	int mapWidth = map->GetWidth();
	int mapHeight = map->GetHeight();

	// 맵의 전체 크기에 따른 미니맵 크기의 비율을 구한다.
	float ratio;
	if ( mapWidth > mapHeight )
		ratio = (float)screenWidth / (float)mapWidth;
	else
		ratio = (float)screenHeight / (float)mapHeight;

	// 비율에 따라 미니맵의 크기를 구한다.
	RECT rt;
	rt.left = 0;
	rt.top = 0;
	rt.right = (int)( mapWidth * ratio );
	rt.bottom = (int)( mapHeight * ratio );

	RECT rt2;
	rt2.left = rt.left + ((screenWidth - rt.right) /2);
	rt2.top = rt.top + ((screenHeight - rt.bottom) /2);
	rt2.right = rt.right + rt2.left;
	rt2.bottom = rt.bottom + rt2.top;

	DIRECTX9()->BeginDrawWide( this, &CChildView::RenderAll, &rt, &rt2, hWnd2, ratio );
}