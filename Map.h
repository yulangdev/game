#ifndef ProjectH_Map_H
#define ProjectH_Map_H

#include <vector>
#include <list>

#include "Point.h"
#include "tile.h"

namespace ProjectH
{

class Map
{
public:
	// 타일을 그리기 위해 얻는 데이터를 담는 구조체
	struct DrawData
	{
		int mapType;
		int imgIndex;
		int tileType;
		int tileFormat;
		int tileDrawX;
		int tileDrawY;
		int tileElevation;
		int startPlayer;
		int playerTyep;
		int startObjectType;
		bool startPoint;
		float indexY;
		std::list< int >* doodad;
	};

	struct StartPlayer
	{
		Point point;
		int type;
	};

public:
	/**************************************************************************
		맵 생성과 해제 하는 기본 생성자, 소멸자 ( 메모리 할당은 최초 한번만 한다. )
	**************************************************************************/
	Map( int tileWidthCount, int tileHeightCount, int mapType = 0, int tileWidthPixel = 64, int tileHeightPixel = 32,
				int mapBgTopPixel = 0, int mapBgBottomPixel = 0, int mapBgLeftPixel = 0, int mapBgRightPixel = 0 );
	~Map() { Release(); }

	/**************************************************************************
		맵 생성과 초기화 릴리즈 하는 메서드
	**************************************************************************/
	// 새로 만들기 ( 데이터 초기화 )
	void Reset( int tileWidthCount, int tileHeightCount, int mapType = 0, int tileWidthPixel = 64, int tileHeightPixel = 32,
				int mapBgTopPixel = 0, int mapBgBottomPixel = 0, int mapBgLeftPixel = 0, int mapBgRightPixel = 0 );
	// 데이터 읽기
	bool Load( char* c );
	// 데이터 저장
	bool Save( char* c );
	// 데이터 해제
	void Release();

	/**************************************************************************
		맵이 최초 생성된 후 릴리즈까지의 맵에 관한 동작
	**************************************************************************/
	// 맵 데이터 설정
	void InitMap( int tileWidthCount, int tileHeightCount, int mapType = 0, int tileWidthPixel = 64, int tileHeightPixel = 32,
				int mapBgTopPixel = 0, int mapBgBottomPixel = 0, int mapBgLeftPixel = 0, int mapBgRightPixel = 0 );
	// 타일 관계 설정
	void InitTileRelation();
	
	/**************************************************************************
		맵 데이터를 얻기 위한 메서드
	**************************************************************************/
	// 맵 가로크기 얻기
	int GetWidth() { return mapWidthPixel; }
	// 맵 세로 크기 얻기
	int GetHeight() { return mapHeightPixel; }
	// 타일 가로갯수 얻기
	int GetTileWidthCount() { return tileWidthCount; }
	// 타일 세로갯수 얻기
	int GetTileHeightCount() { return tileHeightCount; }
	// 맵 픽셀 크기 얻기
	void GetMapSize( Point& point ) { point.SetPoint( mapWidthPixel, mapHeightPixel ); }
	// 몬스터 리스트 얻기
	std::vector< StartPlayer >& GetStartList() { return startPlayer; }

	/**************************************************************************
		타일 데이터 접근에 관한 메서드
	**************************************************************************/
	// Tile 의 데이터를 담고있는 Vector 의 인덱스로 타일의 데이터에 접근한다.
	const std::vector< ProjectH::Tile* >& operator[] ( int y ) { return map[y]; }
	// 클라이언트의 마우스 좌표에 해당하는 타일을 선택한다.
	bool SetIndex( int mouseX, int mouseY );
	// SetIndex 로 선택된 타일의 타입을 바꾼다.
	void SetTile( int imgIndex );
	// SetIndex 로 선택된 타일의 타입과 주변타일의 타입을 바꾼다.
	void AutoTiling( int type );
	// SetIndex 로 선택된 타일의 높이를 올린다.
	void TileElevationUp();
	// SetIndex 로 선택된 타일의 높이를 내린다.
	void TileElevationDown();
	// SetIndex 로 선택된 타일에 장식물을 추가한다.
	void AddDoodad( int doodad );
	// SetIndex 로 선택된 타일의 장식물을 초기화한다.
	void ClearDoodad();
	// SetIndex 로 선택된 타일을 캐릭터 시작지점으로 설정한다.
	void SetStartPoint( int player, int type );

	/**************************************************************************
	*	클라이언트에 그리기 위해 맵 데이터를 얻기 위한 메서드
	**************************************************************************/
	// 그릴 영역을 설정한다. ( 현재 클라이언트에 보여지는 좌표로 NextTile 의 영역을 설정한다. )
	void SetDrawRect( int left, int top, int right, int bottom );
	// 그릴 데이터를 얻어온다. ( SetDrawRect 로 설정된 영역의 타일을 선택하고 그 데이터를 언어온다. )
	bool NextTile( DrawData& drawData );

	/**************************************************************************
		맵 전체를 그리기 위해 맵 데이터를 얻기 위한 메서드
	**************************************************************************/
	// 그릴 타일의 데이터를 얻어온다.
	void GetDrawData( int x, int y, DrawData& drawData );
	// 마우스 이미지를 그릴 좌표를 얻어온다.
	void GetDrawMousePoint( Point& pt );

private:
	/**************************************************************************
		AutoTiling() 을 위한 메서드 ( direction = 0:상, 1:우, 2:하, 3:좌 )
	**************************************************************************/
	// 타일 데이터 인덱스 x, y 의 direction 방향에 있는 타일의 포인터를 리턴한다.
	Tile* GetAroundTile( int direction, int x, int y );
	// 타일 데이터 인덱스 x, y 의 direction 방향에 있는 타일의 인덱스를 얻고 포인터를 리턴한다.
	Tile* GetAroundTileIndex( int direction, int x, int y, Point& pt );
	// direction 의 방향을 뒤집어 해당하는 번호를 리턴한다.
	int ReverseDirection( int direction ) { return ( ( direction + 2 ) % 4 ); }

	/**************************************************************************
		Map Class 내부 로직을 위한 메서드
	**************************************************************************/
	// 맵 인덱스 유효성 검사
	bool CheckIndex( Point& pt )
	{ return ( ( pt.x >= 0 && pt.y >= 0 && pt.x < tileWidthCount && pt.y < tileHeightCount ) ? true : false ); }

private:
	int mapType;				// 맵 종류

	int tileWidthCount;			// 타일 가로 갯수
	int tileHeightCount;		// 타일 세로 갯수

	int mapWidthPixel;			// 맵 가로 크기
	int mapHeightPixel;			// 맵 세로 크기

	int mapBgTopPixel;			// 상단 배경 픽셀 크기
	int mapBgLeftPixel;			// 좌측 배경 픽셀 크기
	int mapBgBottomPixel;		// 하단 배경 픽셀 크기
	int mapBgRightPixel;		// 우측 배경 픽셀 크기

	int tileWidthPixel;			// 타일 가로 크기(픽셀)
	int tileHeightPixel;		// 타일 세로 크기(픽셀)

	int criterionX;				// 맵 기준점X (클라이언트 좌표를 맵 인덱스로 변환하기 위한)
	int criterionY;				// 맵 기준점Y (클라이언트 좌표를 맵 인덱스로 변환하기 위한)

	Point index;				// 마우스( SetIndex() )로 선택한 타일 인덱스 번호

	Point drawLT;				// SetDrawRect() 로 설정된 좌측 상단 인덱스
	Point drawRB;				// SetDrawRect() 로 설정된 우측 하단 인덱스
	Point drawIndex;			// NextTile() 에 사용되는 drawLT, drawRB 사이의 인덱스

	std::vector< StartPlayer > startPlayer;	// 모든 오브젝트 시작 데이터( 위치, 타입 )
	std::vector< Point > startPointList;	// 캐릭터 시작지점 리스트
	std::vector< Point > startPointMoster;	// 몬스터 시작지점 리스트
	std::vector< std::vector< ProjectH::Tile* > > map;	// 맵 데이타

	int l;
	int t;
};

} /* ProjectH namespace */

#endif /* ProjectH_Map_H */