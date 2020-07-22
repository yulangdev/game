#include "stdafx.h"
#include "Tile.h"

#include <algorithm>

namespace ProjectH
{

const Point& Tile::GetDrawCoordinate()
{
	SetDrawCoordinate();	// 그려질 좌표 설정

	return drawCoordinate;
}

void Tile::SetDrawCoordinate()
{
	// 그려질 좌표 설정 ( 기준 좌표 X 값, 기준 좌표 Y 값 + ( 높이단계 * 높이단위당 픽셀값 )
	drawCoordinate.SetPoint( coordinate.x, coordinate.y - ( elevationPixel * elevationGrade ) );
}

void Tile::ElevationUp()
{
	elevationGrade++;

	SetDrawCoordinate();	// 그려질 좌표 설정
}

void Tile::ElevationDown()
{
	elevationGrade--;

	// 제한 낮이 = 0
	if ( elevationGrade < 0 )
		elevationGrade = 0;

	SetDrawCoordinate();	// 그려질 좌표 설정
}

int Tile::GetTypeFormat()
{
	int index = 0;
	for ( int i = 0; i < 4; ++i )
	{
		if ( !aroundTile[i] )
		{
			index += indexTable[i][1];
			continue;
		}

		int aroundTileElevation = aroundTile[i]->GetElevationGrade();

		if ( aroundTile[i]->GetType() == tileType )
		{
			if ( elevationGrade < aroundTileElevation )
				index += indexTable[i][0];

			else if ( elevationGrade == aroundTileElevation )
				index += indexTable[i][1];

			else
				index += indexTable[i][2];
		}
		else
		{
			if ( elevationGrade < aroundTileElevation )
				index += indexTable[i][3];

			else if ( elevationGrade == aroundTileElevation )
				index += indexTable[i][4];

			else
				index += indexTable[i][5];
		}
	}

	return index;
}

void Tile::Link( int direction, bool link )
{
	switch ( direction )
	{
	case 0:
		linkUp = link;
		break;

	case 1:
		linkRight = link;
		break;

	case 2:
		linkDown = link;
		break;

	case 3:
		linkLeft = link;
		break;
	}
}

void Tile::CheckSelf()
{
	if ( CheckEraseableSelf() )
	{
		tileType = 0;
		imgIndex = 0;

		// 주변 타일이 나에게 연결한 링크를 다 끊는다.		
		if ( linkUp && aroundTile[0]->GetType() != tileType )
		{
			aroundTile[0]->Link( 2, false );
			aroundTile[0]->CheckSelf();
			linkUp = false;
		}

		if ( linkRight && aroundTile[1]->GetType() != tileType )
		{
			aroundTile[1]->Link( 3, false );
			aroundTile[1]->CheckSelf();
			linkRight = false;
		}

		if ( linkDown && aroundTile[2]->GetType() != tileType )
		{
			aroundTile[2]->Link( 0, false );
			aroundTile[2]->CheckSelf();
			linkDown = false;
		}

		if ( linkLeft && aroundTile[3]->GetType() != tileType )
		{
			aroundTile[3]->Link( 1, false );
			aroundTile[3]->CheckSelf();
			linkLeft = false;
		}
	}
}

bool Tile::CheckEraseableSelf()
{
	if ( !linkUp && !linkDown )
		return true;

	if ( !linkLeft && !linkRight )
		return true;

	return false;
}

void Tile::AddDoodad( int thing )
{
	doodad.push_back( thing );
}

const int Tile::indexTable[4][6] =
{
	{ 0,	1,		2,		3,		4,		5 },
	{ 0,	6,		12,		18,		24,		30 },
	{ 0,	36,		72,		108,	144,	180 },
	{ 0,	216,	432,	648,	864,	1080 }
};

void Tile::GetData( TileData& data )
{
	data.tileType = tileType;
	data.imgIndex = imgIndex;
	data.availableMove = availableMove;

	data.elevationGrade = elevationGrade;
	data.elevationPixel = elevationPixel;

	data.coordinateX = coordinate.x;
	data.coordinateY = coordinate.y;

	data.drawCoordinateX = drawCoordinate.x;
	data.drawCoordinateY = drawCoordinate.y;

	data.doodad = &doodad;

	data.startPlayer = startPlayer;
	data.startPoint = startPoint;
	data.playerType = playerType;

	data.linkUp = linkUp;
	data.linkRight = linkRight;
	data.linkDown = linkDown;
	data.linkLeft = linkLeft;
}

void Tile::GetDrawData( TileDrawData& data )
{
	data.imgIndex = imgIndex;
	data.tileType = tileType;
	data.drawX = drawCoordinate.x;
	data.drawY = drawCoordinate.y;
	data.elevationGrade = elevationGrade;
	data.startPlayer = startPlayer;
	data.startPoint = startPoint;
	data.playerType = playerType;
	data.doodad = &doodad;
}

void Tile::SetUnLinkType( int type )
{
	tileType = 0;
	CheckSelf();
}

void Tile::ClearDoodad()
{
	doodad.clear();
	availableMove = true;
}

void Tile::Reset( int _elevationPixel )
{
	// 단계당 높이픽셀, 기본 좌표, 주변타일 벡터를 제외한 데이터를 초기화한다.
	availableMove	= true;
	startPoint		= false;
	startPlayer		= 0;
	playerType		= 0;
	imgIndex		= 0;
	tileType		= 0;
	elevationPixel	= elevationPixel;
	elevationGrade	= 0;
	linkUp			= false;
	linkRight		= false;
	linkDown		= false;
	linkLeft		= false;

	drawCoordinate = coordinate;
	doodad.clear();
}

void Tile::SetData( TileData &data )
{
	tileType = data.tileType;
	imgIndex = data.imgIndex;
	elevationPixel = data.elevationPixel;
	elevationGrade = data.elevationGrade;

	coordinate.x = data.coordinateX;
	coordinate.y = data.coordinateY;

	drawCoordinate.x = data.drawCoordinateX;
	drawCoordinate.y = data.drawCoordinateY;

	startPlayer = data.startPlayer;
	startPoint = data.startPoint;
	playerType = data.playerType;

	availableMove = data.availableMove;

	linkUp = data.linkUp;
	linkRight = data.linkRight;
	linkDown = data.linkDown;
	linkLeft = data.linkLeft;
}

} /* ProjectH NameSpace */