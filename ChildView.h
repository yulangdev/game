#pragma once
#include "map.h"

class CChildView : public CWnd
{
// 생성입니다.
public:
	CChildView();

// 특성입니다.
public:
	CPoint selectedTilePx;		// 선택된 타일 위에 그릴 이미지 위지
	CPoint selectedTileArr;		// 선택된 타일 배열 인덱스 값

	float ratio;
	bool miniMap;

	SCROLLINFO vScrollInfo;		// 가로 스크롤 정보
	SCROLLINFO hScrollInfo;		// 세로 스크롤 정보

	CRect clientRect;			// 클라이언트 영역 크기
	RECT rectangle;				// 미니맵 사각형 크기

	bool drag;					// 드래그중인지
	CPoint dragStart;			// 드래그 시작 위치

	ProjectH::Map* map;			// 맵 클래스 인스턴스

// 작업입니다.
public:
	bool Init( int width = 128, int height = 128, int type = 0, char* filePath = NULL );
	void Release();

	void Render();						// 매인화면 그리기 함수
	void RenderAll();					// 미니뷰 그리기 함수
	void CreateWideView();
	void SetPosition( int x, int y );
	void SetWindowSize();
	void SetScrollBar();
	void SetMiniMapRect();

	void MiniMapOnOff() { miniMap = !miniMap; }

	void OnMousePosition();
	void OnMouse( UINT nFlags, CPoint point, bool lButtonDown = false );
	void ViewDrag( CPoint& point );
	void Select() {}
	void Brush();
	void Eraser() { map->ClearDoodad(); }
	void ElevationUP();
	void ElevationDown();

	bool Save( CString& str );
	void Load( CString& str );

private:
	int LimitHScrollPos( int sx );
	int LimitVScrollPos( int sy );

// 재정의입니다.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildView();

	// 생성된 메시지 맵 함수
protected:
//	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC) { return false; }	//	return CWnd::OnEraseBkgnd(pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

