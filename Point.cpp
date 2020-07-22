#include "stdafx.h"
#include "Point.h"

namespace ProjectH
{

const int Point::MULTIPLICATION[9][2] =
{//	좌상			좌			좌하			하			우하			우			우상			상			동
	{ -1, -1 }, { -1, 0 },	{ -1, 1 },	{ 0, 1 },	{ 1, 1 },	{ 1, 0 },	{ 1, -1 },	{ 0, -1 },	{ 0, 0 }
};

void Point::operator+= ( const Point& point )
{
	this->x += point.x;
	this->y += point.y;
}

bool Point::operator== ( const Point& point ) const
{
	if ( this->x == point.x )
		if ( this->y == point.y )
			return true;

	return false;
}

bool Point::operator!= ( const Point& point ) const
{
	if ( x == point.x )
		if ( y == point.y )
			return false;

	return true;
}

void Point::operator= ( const Point& point )
{
	this->x = point.x;
	this->y = point.y;
}

void Point::operator= ( int i )
{
	this->x = this->y = i;
}

void Point::SetPoint( const int _x, const int _y )
{
	this->x = _x;
	this->y = _y;
}

void Point::SetXY( const int _x, const int _y )
{
	this->x = _x;
	this->y = _y;
}

void Point::Move( const Point& currentPoint, const Point& destinationPoint, const Point& unitPoint )
{
	DIRECTION drt = ( currentPoint < destinationPoint );

	int lX = MULTIPLICATION[ drt ][0] * unitPoint.x;
	int lY = MULTIPLICATION[ drt ][1] * unitPoint.y;

	this->x += lX;
	this->y += lY;
}

Point operator+ ( const Point& point1, const Point& point2 )
{
	Point temp( point1 );

	temp += point2;

	return temp;
}

// point1 을 기준으로 point2 의 방향
Point::DIRECTION operator< ( const Point& point1, const Point& point2 )
{
	if ( point1.x > point2.x )
	{
		if ( point1.y > point2.y )
			return Point::TOP_LEFT;

		else if ( point1.y < point2.y )
			return Point::BOTTOM_LEFT;

		else
			return Point::LEFT;
	}

	else if ( point1.x < point2.x )
	{
		if ( point1.y > point2.y )
			return Point::TOP_RIGHT;

		else if ( point1.y < point2.y )
			return Point::BOTTOM_RIGHT;

		else
			return Point::RIGHT;
	}

	else {
		if ( point1.y > point2.y )
			return Point::TOP;

		else if ( point1.y < point2.y )
			return Point::BOTTOM;

		else
			return Point::EQUAL;
	}
}

Point::DIRECTION operator> ( const Point& point1, const Point& point2 )
{
	return ( point1 < point2 );
}

}