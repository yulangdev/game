// NewMap.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ProjectH_MapEdit.h"
#include "NewMap.h"

#include "ImgResource.h"

#include <vector>
#include <string>

using std::vector;
using std::string;


// NewMap 대화 상자입니다.

IMPLEMENT_DYNAMIC(NewMap, CDialog)

NewMap::NewMap(CWnd* pParent /*=NULL*/)
	: CDialog(NewMap::IDD, pParent)
{

}

NewMap::~NewMap()
{

}

void NewMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, mapType);
	DDX_Control(pDX, IDC_COMBO2, width);
	DDX_Control(pDX, IDC_COMBO3, height);
}

BEGIN_MESSAGE_MAP(NewMap, CDialog)
	ON_BN_CLICKED(IDOK, &NewMap::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL NewMap::OnInitDialog()
{
	CDialog::OnInitDialog();

	width.SetCurSel(0);
	height.SetCurSel(0);

	const vector< string >& str = IMGRESOURCE()->GetThemeList();

	for ( int i = 0; i != str.size(); ++i )
		mapType.AddString( CString( str[i].c_str() ) );

	mapType.SetCurSel(0);

	return TRUE;
}

void NewMap::OnBnClickedOk()
{
	CString str;
	width.GetWindowText( str );
	widthVal = _ttoi( str );

	height.GetWindowText( str );
	heightVal = _ttoi( str );

	typeVal = mapType.GetCurSel();

	OnOK();
}
