#include "stdafx.h"
#include "Map.h"
#include "ImgResource.h"

#include <locale.h>
#include <math.h>
#include <fstream>
#include <iterator>

#define CALC_XY(pt, xCoord, yCoord, width, height, criterionX, criterionY, heightCnt)	\
{																						\
	pt.x = static_cast<int>((((xCoord + yCoord * 2) - criterionX)) / width);	        \
	pt.y = static_cast<int>(((criterionY - (yCoord - xCoord / 2))) / height);			\
	pt.y = static_cast<int>(heightCnt / 2 - 1 - (pt.y - pt.x));							\
	pt.x = static_cast<int>(pt.x - ceil((float)pt.y / 2));								\
}


namespace ProjectH
{

using std::vector;
using std::ifstream;
using std::ofstream;
using std::iterator;
using std::endl;
using ProjectH::Point;
using ProjectH::Tile;

void Map::TileElevationUp()
{
	map[ index.y ][ index.x ]->ElevationUp();

	int imgIndex = IMGRESOURCE()->ImageIndex( map[ index.y ][ index.x ]->GetType(), map[ index.y ][ index.x ]->GetTypeFormat() );
	map[ index.y ][ index.x ]->SetImgIndex( imgIndex );

	Tile* at;
	for ( int i = 0; i < 4; ++i )
	{
		if ( at = GetAroundTile( i, index.x, index.y ) )
		{
			imgIndex = IMGRESOURCE()->ImageIndex( at->GetType(), at->GetTypeFormat() );
			at->SetImgIndex( imgIndex );
		}
	}
}

void Map::TileElevationDown()
{
	map[ index.y ][ index.x ]->ElevationDown();

	int imgIndex = IMGRESOURCE()->ImageIndex( map[ index.y ][ index.x ]->GetType(), map[ index.y ][ index.x ]->GetTypeFormat() );
	map[ index.y ][ index.x ]->SetImgIndex( imgIndex );

	Tile* at;
	for ( int i = 0; i < 4; ++i )
	{
		if ( at = GetAroundTile( i, index.x, index.y ) )
		{
			imgIndex = IMGRESOURCE()->ImageIndex( at->GetType(), at->GetTypeFormat() );
			at->SetImgIndex( imgIndex );
		}
	}
}

bool Map::SetIndex( int mouseX, int mouseY )
{
	Point pt;
	CALC_XY( pt, mouseX, mouseY, tileWidthPixel, tileHeightPixel, criterionX, criterionY, tileHeightCount );

	// 인덱스 예외처리
	if ( pt.x >= 0 && pt.x < tileWidthCount && pt.y >= 0 && pt.y < tileHeightCount )
	{
		// 마우스 좌표가 바뀌지 않았으면 false 리턴
		if ( pt.x == index.x && pt.y == index.y )
			return false;

		index.x = pt.x;
		index.y = pt.y;

		return true;
	}
	// 마우스 좌표가 잘못계산되었으면 false 리턴
	return false;
}

void Map::SetTile( int imgIndex )
{
	//map[ index.y ][ index.x ]->SetUnLinkType( imgIndex );
	for ( int i = 0; i < 4; ++i )
	{
		Tile *at = NULL;
		if ( !( at = GetAroundTile(i, index.x, index.y) ) )
		{
			map[index.y][index.x]->Link( i, ( at ? true : false ) );
			at->Link( ReverseDirection(i), ( map[index.y][index.x] ? true : false ) );
		}
	}
	map[ index.y ][ index.x ]->SetImgIndex( imgIndex );
}

void Map::AutoTiling( int type )
{
	// 타일들의 타입을 설정한다. (현재 타일과 주변타일 8개)
	map[ index.y ][ index.x ]->SetLinkType( type );

	for ( int i = 0; i < 8; ++i )
	{
		Tile* at = NULL;
		if ( !( at = GetAroundTile( i, index.x, index.y ) ) )
			continue;

		if ( i < 4 )
		{
			at->Link( ReverseDirection(i), ( map[ index.y ][ index.x ] ? true : false ) );
			map[ index.y ][ index.x ]->Link( i, ( at ? true : false ) );
		}

		at->SetLinkType( type );
	}

	// 타일들의 링크를 설정한다. ( 현재 타일과 주변타일 8개와 그의 주변타일 4개 )
	for ( int i = 0; i < 8; ++i )
	{
		Tile* at = NULL;
		Point ai;
		if ( !( at = GetAroundTileIndex( i, index.x, index.y, ai ) ) )
			continue;

		for ( int j = 0; j < 4; ++j )
		{
			Tile* at2 = NULL;
			if ( !( at2 = GetAroundTile( j, ai.x, ai.y ) ) )
				continue;

			if ( at->GetType() == at2->GetType() )
			{
				at->Link( j, ( at2 ? true : false ) );
				at2->Link( ReverseDirection(j), ( at ? true : false ) );
			}
			else
			{
				at->Link( j, 0 );
				at2->Link( ReverseDirection(j), 0 );
			}
		}
	}

	// 타일들의 타입별로 그릴수 있는지 체크한다. (현재 타일과 주변타일 8개와 그의 주변타일 4개)
	for ( int i = 0; i < 8; ++i )
	{
		Point ai;
		if ( !GetAroundTileIndex( i, index.x, index.y, ai ) )
			continue;

		for ( int j = 0; j < 4; ++j )
		{
			Tile* at;
			if ( !( at = GetAroundTile( j, ai.x, ai.y ) ) )
				continue;

			at->CheckSelf();
		}
	}

	// 설정된 타일들의 타일타입에 따른 타일포맷에 따라 이미지 인덱스를 설정한다. (현재 타일과 주변타일 8개와 그의 주변타일 4개)
	for ( int i = 0; i < 8; ++i )
	{
		Point ai;
		if ( !GetAroundTileIndex( i, index.x, index.y, ai ) )
			continue;

		for ( int j = 0; j < 4; ++j )
		{
			Tile* at;
			if ( !( at = GetAroundTile( j, ai.x, ai.y ) ) )
				continue;

			int imgIndex = IMGRESOURCE()->ImageIndex( at->GetType(), at->GetTypeFormat() );
			at->SetImgIndex( imgIndex );
		}

		int imgIndex = IMGRESOURCE()->ImageIndex( map[ ai.y ][ ai.x ]->GetType(), map[ ai.y ][ ai.x ]->GetTypeFormat() );
		map[ ai.y ][ ai.x ]->SetImgIndex( imgIndex );
	}

	int imgIndex = IMGRESOURCE()->ImageIndex( map[ index.y ][ index.x ]->GetType(), map[ index.y ][ index.x ]->GetTypeFormat() );
	map[ index.y ][ index.x ]->SetImgIndex( imgIndex );

} /* void Map::SetTileType( int type ) */

void Map::SetDrawRect( int left, int top, int right, int bottom )
{
	l = left;
	t = top;

	CALC_XY( drawLT, left, top, tileWidthPixel, tileHeightPixel, criterionX, criterionY, tileHeightCount );
	CALC_XY( drawRB, right, bottom, tileWidthPixel, tileHeightPixel, criterionX, criterionY, tileHeightCount );

	if ( drawLT.x >= 0 && drawLT.x < tileWidthCount && drawLT.y >= 0 && drawLT.y < tileHeightCount &&
		drawRB.x >= 0 && drawRB.x <= tileWidthCount && drawRB.y >= 0 && drawRB.y <= tileHeightCount )
	{
		drawLT.x = ( drawLT.x -1 < 0 ? drawLT.x : drawLT.x -1 );
		drawLT.y = ( drawLT.y -1 < 0 ? drawLT.y : drawLT.y -1 );

		drawRB.x = ( drawRB.x +1 > tileWidthCount ? drawRB.x : drawRB.x +1 );
		drawRB.y = ( drawRB.y +1 > tileHeightCount ? drawRB.y : drawRB.y +1 );
		drawRB.x = ( drawRB.x +1 > tileWidthCount ? drawRB.x : drawRB.x +1 );
		drawRB.y = ( drawRB.y +1 > tileHeightCount ? drawRB.y : drawRB.y +1 );

		drawIndex = drawLT;
	}
}

bool Map::NextTile( DrawData& drawData )
{
	if ( drawIndex.y == drawRB.y )
		return false;

	Tile::TileDrawData tileData;
	map[ drawIndex.y ][ drawIndex.x ]->GetDrawData( tileData );

	drawData.mapType = mapType;
	drawData.imgIndex = tileData.imgIndex;
	drawData.tileType = tileData.tileType;
	drawData.tileDrawX = tileData.drawX - l;
	drawData.tileDrawY = tileData.drawY - t;
	drawData.tileElevation = tileData.elevationGrade;
	drawData.startPlayer = tileData.startPlayer;
	drawData.startPoint = tileData.startPoint;
	drawData.playerTyep = tileData.playerType;
	drawData.doodad = tileData.doodad;
	drawData.indexY = ( (float) drawIndex.y ) * 0.000f;
	
	if ( drawIndex.x == drawRB.x -1 )
	{
		drawIndex.y++;
		drawIndex.x = drawLT.x;
	}
	else
	{
		drawIndex.x++;
	}

	return true;
}

void Map::GetDrawMousePoint( Point& pt )
{
	pt = map[ index.y ][ index.x ]->GetDrawCoordinate();
	pt.x -= l;
	pt.y -= t;
}

Tile* Map::GetAroundTile( int direction, int x, int y )
{
	Point pt;

	switch( direction )
	{
	case 0:		// ↗
	{
		pt.x = ( y %2 == 1 )	?	x +1	:	x;
		pt.y = y -1;
		break;
	}
	case 1:		// ↘
	{
		pt.x = ( y %2 == 1 )	?	x +1	:	x;
		pt.y = y +1;
		break;
	}
	case 2:		// ↙
	{
		pt.x = ( y %2 == 0 )	?	x -1	:	x;
		pt.y = y +1;
		break;
	}
	case 3:		// ↖
	{
		pt.x = ( y %2 == 0 )	?	x -1	:	x;
		pt.y = y -1;
		break;
	}
	case 4:		// →
	{
		pt.x = x +1;
		pt.y = y;
		break;
	}
	case 5:		// ↓
	{
		pt.x = x;
		pt.y = y +2;
		break;
	}
	case 6:		// ←
	{
		pt.x = x -1;
		pt.y = y;
		break;
	}
	case 7:		// ↑
	{
		pt.x = x;
		pt.y = y -2;
		break;
	}
	} /* switch( direction ) */

	if ( CheckIndex( pt ) )
		return map[ pt.y ][ pt.x ];
	else
		return 0;
}

Tile* Map::GetAroundTileIndex( int direction, int x, int y, Point& pt )
{
	switch( direction )
	{
	case 0:		// ↗
	{
		pt.x = ( y %2 == 1 )	?	x +1	:	x;
		pt.y = y -1;
		break;
	}
	case 1:		// ↘
	{
		pt.x = ( y %2 == 1 )	?	x +1	:	x;
		pt.y = y +1;
		break;
	}
	case 2:		// ↙
	{
		pt.x = ( y %2 == 0 )	?	x -1	:	x;
		pt.y = y +1;
		break;
	}
	case 3:		// ↖
	{
		pt.x = ( y %2 == 0 )	?	x -1	:	x;
		pt.y = y -1;
		break;
	}
	case 4:		// →
	{
		pt.x = x +1;
		pt.y = y;
		break;
	}
	case 5:		// ↓
	{
		pt.x = x;
		pt.y = y +2;
		break;
	}
	case 6:		// ←
	{
		pt.x = x -1;
		pt.y = y;
		break;
	}
	case 7:		// ↑
	{
		pt.x = x;
		pt.y = y -2;
		break;
	}
	} /* switch( direction ) */

	if ( CheckIndex( pt ) )
		return map[ pt.y ][ pt.x ];
	else
		return 0;
}

void Map::GetDrawData( int x, int y, DrawData& drawData )
{
	Tile::TileDrawData tileData;
	map[ y ][ x ]->GetDrawData( tileData );

	drawData.mapType = mapType;
	drawData.imgIndex = tileData.imgIndex;
	drawData.tileType = tileData.tileType;
	drawData.tileDrawX = tileData.drawX;
	drawData.tileDrawY = tileData.drawY;
	drawData.tileElevation = tileData.elevationGrade;
	drawData.startPlayer = tileData.startPlayer;
	drawData.startPoint = tileData.startPoint;
	drawData.playerTyep = tileData.playerType;
	drawData.doodad = tileData.doodad;
	drawData.indexY = ( (float) drawIndex.y ) * 0.000f;
}

void Map::SetStartPoint( int player, int type )
{
	// 몬스터 시작지점 일때
	if ( player == -1 )
	{
		// 현재 타일에서 시작하는 플레이어나 몬스터가 있는지 확인
		if ( map[ index.y ][ index.x ]->GetStartPoint() )
		{
			int p = map[ index.y ][ index.x ]->GetStartPlayer();
			// 현재 타일에서 몬스터가 시작하면 몬스터 시작위치 리스트에서 해당 위치를 삭제한다.
			if ( p == -1 )
			{
				std::vector< Point >::iterator iter;
				for ( iter = startPointMoster.begin(); iter != startPointMoster.end(); ++iter )
				{
					if ( (*iter) == index )
					{
						startPointMoster.erase( iter );
						break;
					}
				}
			}
			// 현재 타일에서 캐릭터가 시작하면 캐릭터 시작위치 리스트에서 해당 플레이어의 캐릭터의 위치를 초기화한다.
			else
			{
				startPointList[ p ] = 0;
			}
		}
		// 현재 좌표를 몬스터 시작위치 리스트에 삽입한다.
		startPointMoster.push_back( index );
	}
	// 플레이어 캐릭터 시작지점 일때
	else
	{
		// 현재 선택된 캐릭터의 기존 시작위치 타일의 시작 설정을 초기화한다.
		map[ startPointList[ player ].y ][ startPointList[ player ].x ]->SetStartPlayer( 0 );
		map[ startPointList[ player ].y ][ startPointList[ player ].x ]->SetStartPoint( false );
		
		// 현재 좌표를 캐릭터 시작위치 리스트에 삽입한다.
		startPointList[ player ] = index;		
	}

	// 현재 위치의 장식물을 초기화한다.
	map[ index.y ][ index.x ]->ClearDoodad();

	// 현재 좌표에 캐릭터 시작위치를 설정한다.
	map[ index.y ][ index.x ]->SetStartPlayer( player );
	map[ index.y ][ index.x ]->SetStartPoint( true );
	map[ index.y ][ index.x ]->SetPlayerType( type );
}

void Map::AddDoodad( int doodad )
{
	Tile::TileData tileData;
	map[ index.y ][ index.x ]->GetData( tileData );
	int lastDoodad = ( tileData.doodad->empty() == true ? 0 : tileData.doodad->back() );

	if ( lastDoodad && !( IMGRESOURCE()->DoodadAvailable( lastDoodad, doodad ) ) )
		map[ index.y ][ index.x ]->ClearDoodad();

	map[ index.y ][ index.x ]->AddDoodad( doodad );

	if ( IMGRESOURCE()->IsMajorTile( map[ index.y ][ index.x ]->GetImgIndex() ) )
	{
		int type = map[ index.y ][ index.x ]->GetType();
		int firstDoodad = ( tileData.doodad->empty() == true ? 0 : tileData.doodad->back() );

		if ( firstDoodad )
		{
			int imgIndex = IMGRESOURCE()->GetShadowIndex( type, firstDoodad );

			if ( imgIndex )
				map[ index.y ][ index.x ]->SetImgIndex( imgIndex );
		}
	}

	// 이동 가능 값 저장
	map[ index.y ][ index.x ]->AvailableMove( IMGRESOURCE()->IsMoveable( doodad ) );
}

Map::Map( int _tileWidthCount, int _tileHeightCount, int _mapType /*= 0*/, int _tileWidthPixel /*= 64*/, int _tileHeightPixel /*= 32*/, int _mapBgTopPixel /*= 0*/, int _mapBgBottomPixel /*= 0*/, int _mapBgLeftPixel /*= 0*/, int _mapBgRightPixel /*= 0 */ )
{
	// 맵 데이터 설정
	InitMap( _tileWidthCount, _tileHeightCount, _mapType, _tileWidthPixel, _tileHeightPixel, _mapBgTopPixel, _mapBgBottomPixel, _mapBgLeftPixel, _mapBgRightPixel );

	// 타일 가로, 세로 갯수와 상관없이 맵 벡터를 최대크기인 128 로 리사이징하고 타일 데이타 삽입
	map.resize( 128 );
	for ( int y = 0; y < static_cast< int >( map.size() ); ++y )
	{
		map[y].resize( 128 );

		for ( int x = 0; x < static_cast< int >( map[y].size() ); ++x )
		{
			// 타일의 그려질 픽셀 위치 계산
			int drawX =	x * tileWidthPixel	+	( ( y %2 ) ? ( tileWidthPixel /2 ) : 0 ) - ( tileWidthPixel /2 ) + mapBgLeftPixel;
			int drawY = y * ( tileHeightPixel /2 ) - ( tileHeightPixel /2 ) + mapBgTopPixel;

			// 타일 데이터 삽입
			map[y][x] = new Tile( tileHeightPixel/2, drawX, drawY );
		}
	}

	// 캐릭터 시작 위치 벡터 리사이징
	startPointList.resize( 9 );

	// 타일 관계 설정
	InitTileRelation();
}

void Map::InitMap( int _tileWidthCount, int _tileHeightCount, int _mapType /*= 0*/, int _tileWidthPixel /*= 64*/, int _tileHeightPixel /*= 32*/, int _mapBgTopPixel /*= 0*/, int _mapBgBottomPixel /*= 0*/, int _mapBgLeftPixel /*= 0*/, int _mapBgRightPixel /*= 0 */ )
{
	// 맵 타입 설정
	mapType			= _mapType;

	// 맵의 배경 픽셀 크기 설정
	mapBgTopPixel	= _mapBgTopPixel;
	mapBgLeftPixel	= _mapBgLeftPixel;
	mapBgBottomPixel= _mapBgBottomPixel;
	mapBgRightPixel	= _mapBgRightPixel;

	// 타일의 가로 세로 픽셀크기 설정
	tileWidthPixel	= _tileWidthPixel;
	tileHeightPixel	= _tileHeightPixel;

	// 타일의 가로 세로 갯수가 0보다 작으면 128로 설정
	tileWidthCount	= ( _tileWidthCount < 0 ? 128 : _tileWidthCount );
	tileHeightCount	= ( _tileHeightCount < 0 ? 128 : _tileHeightCount );

	// 타일의 가로 세로 갯수가 128보다 크면 128로 설정
	tileWidthCount	= ( _tileWidthCount > 128 ? 128 : _tileWidthCount );
	tileHeightCount	= ( _tileHeightCount > 128 ? 128 : _tileHeightCount );

	// 타일의 가로 세로 갯수가 짝수가 아니면 짝수로 설정
	tileWidthCount	= ( _tileWidthCount %2 == 0 ? _tileWidthCount : _tileWidthCount -1 );
	tileHeightCount	= ( _tileHeightCount %2 == 0 ? _tileHeightCount : _tileHeightCount -1 );

	// 맵 픽셀 크기 설정
	mapWidthPixel	= ( tileWidthPixel * tileWidthCount ) - tileWidthPixel / 2 + mapBgLeftPixel + mapBgRightPixel;
	mapHeightPixel	= ( tileHeightPixel * tileHeightCount / 2 ) - tileHeightPixel / 2 + mapBgTopPixel + mapBgBottomPixel;

	// 뷰의 마우스좌표와의 변환을 위해 중심값 설정
	criterionX		= ( mapBgLeftPixel - tileWidthPixel / 2 ) + mapBgTopPixel * 2;
	criterionY		= ( mapHeightPixel - mapBgBottomPixel ) - mapBgLeftPixel / 2;
}

void Map::Reset( int _tileWidthCount, int _tileHeightCount, int _mapType /*= 0*/, int _tileWidthPixel /*= 64*/, int _tileHeightPixel /*= 32*/, int _mapBgTopPixel /*= 0*/, int _mapBgBottomPixel /*= 0*/, int _mapBgLeftPixel /*= 0*/, int _mapBgRightPixel /*= 0 */ )
{
	// 맵 데이터 설정
	InitMap( _tileWidthCount, _tileHeightCount, _mapType, _tileWidthPixel, _tileHeightPixel, _mapBgTopPixel, _mapBgBottomPixel, _mapBgLeftPixel, _mapBgRightPixel );

	// 타일 데이터 초기화
	for ( int y = 0; y < static_cast< int >( map.size() ); ++y )
		for ( int x = 0; x < static_cast< int >( map[y].size() ); ++ x )
			map[y][x]->Reset( _tileHeightPixel/2 );

	// 캐릭터 시작 위치 초기화
	for ( int y = 0; y != static_cast< int >( startPointList.size() ); ++y )
		startPointList[y] = 0;

	startPointMoster.clear();
}

void Map::Release()
{
	// 타일 데이터 메모리 해제
	for ( int y = 0; y != static_cast<int>( map.size() ); ++y )
	{
		for ( int x = 0; x != static_cast<int>( map[y].size() ); ++x )
			delete map[y][x];

		map[y].clear();
		map[y].resize( 0 );
	}

	map.clear();
	map.resize( 0 );
}

void Map::InitTileRelation()
{
	for ( int y = 0; y != tileHeightCount; ++y )
	{
		for ( int x = 0; x != tileWidthCount; ++x )
		{
			// 주변타일 갯수 8개만큼 돌면서
			for ( int i = 0; i != 8; ++i )
			{
				Tile* tile = GetAroundTile( i, x, y );		// 주변타일의 포인터를 얻어온다.

				map[y][x]->AddAroundTile( tile );			// 주변타일 포인터 저장
			}
		}
	}
}

bool Map::Load( char* c )
{
	// 언어를 한국어로 설정하고 파일을 연다.
	setlocale( LC_ALL, "Korean" );
	ifstream f( c, std::ios::in );
	if ( !f.is_open() ) return false;

	std::istream_iterator<int> iit( f );
	std::istream_iterator<int> eos;

	// 맵 데이터 로드
	int mapType = *iit++;
	int mapBgLeftPixel = *iit++;
	int mapBgRightPixel = *iit++;
	int mapBgTopPixel = *iit++;
	int mapBgBottomPixel = *iit++;
	int tileWidthCount = *iit++;
	int tileHeightCount = *iit++;
	int tileWidthPixel = *iit++;
	int tileHeightPixel = *iit++;

	// 맵 데이터 초기화
	Reset( tileWidthCount, tileHeightCount, mapType, tileWidthPixel, tileHeightPixel, mapBgTopPixel, mapBgBottomPixel, mapBgLeftPixel, mapBgRightPixel );

	// 캐릭터 시작지점 데이터 로드
	int iMax = *iit++;
	for ( int i = 0; i != iMax; ++i )
	{
		startPointList[i].x = *iit++;
		startPointList[i].y = *iit++;
	}

	// 몬스터 시작지점 데이터 로드
	iMax = *iit++;
	startPointMoster.resize( iMax );
	for ( int i = 0; i != iMax; ++i )
	{
		startPointMoster[i].x = *iit++;
		startPointMoster[i].y = *iit++;
	}

	Tile::TileData tileData;
	for ( int y = 0; y != tileHeightCount; ++y )
	{
		// 타일 데이터 삽입
		for ( int x = 0; x != tileWidthCount; ++x )
		{
			tileData.imgIndex = *iit++;
			tileData.tileType = *iit++;
			tileData.coordinateX = *iit++;
			tileData.coordinateY = *iit++;
			tileData.drawCoordinateX = *iit++;
			tileData.drawCoordinateY = *iit++;
			tileData.availableMove = (*iit++) ? true : false;
			tileData.elevationPixel = *iit++;
			tileData.elevationGrade = *iit++;
			tileData.startPoint = (*iit++) ? true : false;
			tileData.startPlayer = (*iit++);
			tileData.playerType = (*iit++);
			tileData.linkUp = (*iit++) ? true : false;
			tileData.linkRight = (*iit++) ? true : false;
			tileData.linkDown = (*iit++) ? true : false;
			tileData.linkLeft = (*iit++) ? true : false;

			if ( tileData.startPoint )
			{
				StartPlayer s;
				s.point.x = tileData.drawCoordinateX;
				s.point.y = tileData.drawCoordinateY;
				s.type = tileData.playerType;

				startPlayer.push_back( s );
			}

			map[y][x]->SetData( tileData );

			int doodadSize = *iit++;
			for ( int i = 0; i < doodadSize; ++i )
				map[y][x]->AddDoodad( *iit++ );
		}
	}

	// 시작 지점 데이터 타일에 설정
	for ( int i = 0; i != startPointList.size(); ++i )
	{
		if ( startPointList[i].x > 1 && startPointList[i].y > 1 )
		{
			map[ startPointList[i].y ][ startPointList[i].x ]->SetStartPoint( true );
			map[ startPointList[i].y ][ startPointList[i].x ]->SetStartPlayer( i );
		}
	}
	f.close();

	return true;
}

bool Map::Save( char* c )
{
	// 맵 데이터 출력
	setlocale( LC_ALL, "Korean" );
	ofstream f( c, std::ios::out );
	if ( !f.is_open() )	return false;

	f << mapType << endl;
	f << mapBgLeftPixel << endl;
	f << mapBgRightPixel << endl;
	f << mapBgTopPixel << endl;
	f << mapBgBottomPixel << endl;
	f << tileWidthCount << endl;
	f << tileHeightCount << endl;
	f << tileWidthPixel << endl;
	f << tileHeightPixel << endl;
	
	// 캐릭터 시작 지점 데이터 저장
	f << startPointList.size() << endl;
	for ( int i = 0; i != startPointList.size(); ++i )
		f << startPointList[i].x << "\t" << startPointList[i].y << endl;

	// 몬스터 시작 지점 데이터 저장
	f << startPointMoster.size() << endl;
	for ( int i = 0; i != startPointMoster.size(); ++i )
		f << startPointMoster[i].x << "\t" << startPointMoster[i].y << endl;

	Tile::TileData tileData;
	for( int y = 0; y < tileHeightCount; ++y )
	{
		for ( int x = 0; x < tileWidthCount; ++x )
		{
			// 저장할 타일 데이터 얻어오기
			map[y][x]->GetData( tileData );

			// 타일 데이터 출력
			f	<< tileData.imgIndex << "\t"
				<< tileData.tileType << "\t"
				<< tileData.coordinateX << "\t"
				<< tileData.coordinateY << "\t"
				<< tileData.drawCoordinateX << "\t"
				<< tileData.drawCoordinateY << "\t"
				<< tileData.availableMove << "\t"
				<< tileData.elevationPixel << "\t"
				<< tileData.elevationGrade << "\t" 
				<< tileData.startPoint << "\t"
				<< tileData.startPlayer << "\t"
				<< tileData.playerType << "\t"
				<< tileData.linkUp << "\t"
				<< tileData.linkRight << "\t"
				<< tileData.linkDown << "\t"
				<< tileData.linkLeft << "\t"
				<< tileData.doodad->size() << "\t";

			for ( std::list< int >::iterator iter = tileData.doodad->begin(); iter != tileData.doodad->end(); ++iter )
				f << (*iter) << "\t";
			
			f << endl;
		}
	}

	f.close();

	return true;
}

void Map::ClearDoodad()
{
	map[ index.y ][ index.x ]->ClearDoodad();

	int imgIndex = IMGRESOURCE()->ImageIndex( map[ index.y ][ index.x ]->GetType(), map[ index.y ][ index.x ]->GetTypeFormat() );
	map[ index.y ][ index.x ]->SetImgIndex( imgIndex );

}
} /* ProjectH namespace */