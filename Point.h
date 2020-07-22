#ifndef ProjectH_Point_H
#define ProjectH_Point_H

namespace ProjectH
{

class Point
{
public:
	enum DIRECTION { TOP_LEFT = 0, LEFT, BOTTOM_LEFT, BOTTOM, BOTTOM_RIGHT, RIGHT, TOP_RIGHT, TOP, EQUAL };
	static const int MULTIPLICATION[9][2];

	friend Point operator+ ( const Point& point1, const Point& point2 );
	friend Point::DIRECTION operator< ( const Point& point1, const Point& point2 );
	friend Point::DIRECTION operator> ( const Point& point1, const Point& point2 );

	Point() : x(0), y(0) {}
	Point( int _x, int _y = 0 ) : x( _x ), y( _y ) {}
	Point( const Point& point ) : x( point.x ), y( point.y ) {}

	void SetPoint( const int _x, const int _y );
	void SetXY( const int _x, const int _y );

	// currentPoint를 destinationPoint까지 unitPoint만큼 가까이 이동
	void Move( const Point& currentPoint, const Point& destinationPoint, const Point& unitPoint );

	void operator+= ( const Point& point );

	bool operator== ( const Point& point ) const;

	bool operator!= ( const Point& point ) const;

	void operator= ( const Point& point );

	void operator= ( int i );

	int x;
	int y;	
};



} /* ProjectH NameSpace */

#endif /* ProjectH_Point_H */