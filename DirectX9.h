#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "dinput8.lib" )
#pragma comment( lib, "dxguid.lib" )

#ifndef ProjectH_DirectX_H
#define ProjectH_DirectX_H

#include	<windows.h>
#include	<d3d9.h>
#include	<d3dx9.h>

#include	<hash_map>
#include	<vector>

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

namespace ProjectH
{

class DirectX9
{
public:
	enum	{ NON_REVERSE = 0, TB_REVERSE, LR_REVERSE, SCAILE };

struct CUSTOMVERTEX
{
    FLOAT x, y, z, rhw; // The transformed position for the vertex
    DWORD color;        // The vertex color
};

private:
	LPDIRECT3D9			d3d;
	LPDIRECT3DDEVICE9	d3dDevice;
	LPD3DXSPRITE		sprite;

	// 사각형
	LPDIRECT3DVERTEXBUFFER9 vb;

	// 스왑체인
	LPDIRECT3DSWAPCHAIN9 chain1;
	LPDIRECT3DSWAPCHAIN9 chain2;
	LPDIRECT3DSWAPCHAIN9 chain3;

	int m_mapWidth;
	int m_mapHeight;
	float ratio;

	LPDIRECT3DSURFACE9 back;
	LPDIRECT3DSURFACE9 back2;

	int	backBufferWidth;
	int	backBufferHeight;

	int miniMapWidth;
	int miniMapHeight;
	RECT miniMapRect;

	stdext::hash_map< INT, LPDIRECT3DTEXTURE9 >	m_arrTex;

public:
	static
	DirectX9*	Instance		();

				~DirectX9		();

	bool		Init			( HWND _hWnd, int screenWidth, int screenHeight, int miniMapWidth, int miniMapHeight, float ratio );
	void		Release		();	

	void		Load			();

    template <class ClassName, typename FunctionName>
    void BeginDrawRect( ClassName className, FunctionName functionName, RECT* drawRt, HWND hWnd )
    {
		if ( !hWnd ) return;

		BeginDraw();
        (className->*functionName)();
		EndDraw( drawRt, hWnd );
    }

    template <class ClassName, typename FunctionName>
    void BeginDrawAll( ClassName className, FunctionName functionName, RECT* rectangle, HWND hWnd, float r )
    {
		if ( !hWnd ) return;

		float rr = ratio;
		ratio = r;

		BeginDraw2();
        (className->*functionName)();
		EndDraw2( rectangle, hWnd );

		ratio = rr;
    }

    template <class ClassName, typename FunctionName>
    void BeginDrawWide( ClassName className, FunctionName functionName, RECT* rectangle, RECT* rectangle2, HWND hWnd, float r )
    {
		if ( !hWnd ) return;

		float rr = ratio;
		ratio = r;

		BeginDraw3();
        (className->*functionName)();
		EndDraw3( rectangle, rectangle2, hWnd );

		ratio = rr;
    }

	void		Draw			( int,  int, int, int, int, int, int,
								float i = 0.0f, int _flag = 0, float _r = 1.0f, float _g = 1.0f, float _b = 1.0f, float _a = 1.0f );

	void		Reset			( int width = 0, int height = 0 );

private:
				DirectX9		();

	void		BeginDraw		();
	void		EndDraw			( RECT* drawRt, HWND hWnd );

	void		BeginDraw2		();
	void		EndDraw2		( RECT* drawRt, HWND hWnd );

	void		BeginDraw3		();
	void		EndDraw3		( RECT* drawRt, RECT* drawRt2, HWND hWnd );

	void		LoadTexture		( int , int , int , float , float , float , TCHAR* );
};

#define DIRECTX9() ProjectH::DirectX9::Instance()

} /* ProjectH NameSpace */

#endif /* ProjectH_Point_H */