// WideView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ProjectH_MapEdit.h"
#include "WideView.h"


// WideView 대화 상자입니다.

IMPLEMENT_DYNAMIC(WideView, CDialog)

WideView::WideView(CWnd* pParent /*=NULL*/)
	: CDialog(WideView::IDD, pParent)
{

}

WideView::~WideView()
{
}

void WideView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(WideView, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


HBRUSH WideView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    switch(nCtlColor)
    {
    case CTLCOLOR_DLG:
        return (HBRUSH)GetStockObject(BLACK_BRUSH);
        break;
    }    

	return hbr;
}
