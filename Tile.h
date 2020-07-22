#ifndef ProjectH_Tile_H
#define ProjectH_Tile_H

#include <vector>
#include <list>

#include "Point.h"

namespace ProjectH
{

class Tile
{
friend class Tile;
friend class Map;

public:
	/**************************************************************************
		그리기를 위한 구조체
	**************************************************************************/
	struct TileDrawData
	{
		int imgIndex;
		int tileType;
		int drawX;
		int drawY;
		int elevationGrade;
		int startPlayer;
		int playerType;
		bool startPoint;
		std::list< int >* doodad;
	};

	/**************************************************************************
		저장을 위한 구조체
	**************************************************************************/
	struct TileData
	{
		int tileType;
		int imgIndex;
		int elevationPixel;
		int elevationGrade;

		int coordinateX;
		int coordinateY;
		int drawCoordinateX;
		int drawCoordinateY;
		
		int playerType;
		int startPlayer;
		bool startPoint;

		bool availableMove;

		bool linkUp;
		bool linkRight;
		bool linkDown;
		bool linkLeft;

		std::list< int >* doodad;
	};

public:
	/**************************************************************************
		타일 데이터 기본 생성자
	**************************************************************************/
	Tile( int elevation, int x, int y ) :
		elevationPixel( elevation ), startPoint( false ), startPlayer( 0 ), playerType( 0 ), imgIndex( 0 ),
		tileType( 0 ), elevationGrade( 0 ), availableMove( true ),
		linkUp( false ), linkRight( false ), linkDown( false ), linkLeft( false ),
		coordinate( x, y ), drawCoordinate( x, y ), isTransform( false ) {}
	
	~Tile() {}

	// 데이터 초기화
	void Reset( int elevationPixel );

	/**************************************************************************
		타일 데이터를 얻기 위한 메서드
	**************************************************************************/
	// 타일타입 얻기
	int GetType() { return tileType; }
	// 타일형태 얻기
	int GetTypeFormat();
	// 이미지 인덱스 얻기
	int GetImgIndex() { return imgIndex; }
	// 높이단계 얻어오기
	int GetElevationGrade() { return elevationGrade; }
	// 기본 픽셀좌표 얻기
	const Point& GetCoordinate() { return coordinate; }
	// 그려질 픽셀좌표 얻기
	const Point& GetDrawCoordinate();
	// 저장할 데이터 얻기
	void GetData( TileData& data );
	// 그리기 위한 데이터 얻기
	void GetDrawData( TileDrawData& data );

	/**************************************************************************
		타일 데이터 설정을 위한 메서드
	**************************************************************************/
	// 이미지 인덱스 변경
	void SetImgIndex( int index ) { imgIndex = index; }
	// 높이 올리기
	void ElevationUp();	
	// 높이 낮추기
	void ElevationDown();
	// 타일 종류 변경 ( 주변타일 링크를 끊지 않는다. )
	void SetLinkType( int type ) { tileType = type; }
	// 타일 종류 변경 ( 주변타일 링크를 끊는다. )
	void SetUnLinkType( int type );
	// 장식물 추가
	void AddDoodad( int thing );
	// 장식물 초기화
	void ClearDoodad();
	// 이동 가능 설정
	void AvailableMove( bool b ) { availableMove == true ? availableMove = b : availableMove = availableMove; }
	// 시작지점 설정
	void SetStartPoint( bool b ) { startPoint = b; }
	// 시작지점 설정 얻기
	bool GetStartPoint() { return startPoint; }
	// 시작 캐릭터 설정
	void SetStartPlayer( int player ) { startPlayer = player; }
	// 시작 플레이어 번호 얻기
	int GetStartPlayer() { return startPlayer; }
	// 캐릭터 종류 설정
	void SetPlayerType( int type ) { playerType = type; }	
	// 타일 데이터 설정
	void SetData( TileData &data );

private:
	/**************************************************************************
		Friend Map; 을 위한 메서드
	**************************************************************************/
	// 주변타일 추가
	void AddAroundTile( Tile* tile ) { aroundTile.push_back( tile ); }
	// 오토타일링을 위한 메서드
	void Link( int direction, bool link );				// 타일 연결
	void CheckSelf();									// 지워져야 하는지 체크
	bool CheckEraseableSelf();							// 지워져야 하는지 체크

	/**************************************************************************
		Tile Class 내부 로직을 위한 메서드
	**************************************************************************/
	// 그려질 픽셀좌표 설정
	void SetDrawCoordinate();

private:
	bool isTransform;
	int imgIndex;					// 타일 이미지 인덱스
	int tileType;					// 타일 종류
	int elevationGrade;				// 높이 단계

	int elevationPixel;				// 타일 높이 값 (픽셀 단위)

	bool availableMove;				// 이동 가능 여부

	bool linkUp;					// ↗
	bool linkRight;					// ↘
	bool linkDown;					// ↙
	bool linkLeft;					// ↖

	int playerType;					// 캐릭터 종류
	int startPlayer;				// 시작 캐릭터 번호
	bool startPoint;				// 캐릭터 시작 위치인지

	Point coordinate;				// 기준 좌표 (픽셀 단위)
	Point drawCoordinate;			// 그려질 좌표 (픽셀 단위)

	std::list< int > doodad;		// 장식물 리스트

	std::vector< Tile* > aroundTile;// 주변타일 ↗↘↙↖→↓←↑ 방향순서

	static const int indexTable[4][6];	// 오토 타일링을 위한 데이터
};

} /* ProjectH NameSpace */

#endif /* ProjectH_Tile_H */