// MyListBox.cpp : 实现文件
//

#include "stdafx.h"
#include "WZQ.h"
#include "MyListBox.h"


// MyListBox

IMPLEMENT_DYNAMIC(MyListBox, CListBox)

MyListBox::MyListBox()
{

}

MyListBox::~MyListBox()
{
}


BEGIN_MESSAGE_MAP(MyListBox, CListBox)
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// MyListBox 消息处理程序



using namespace Gdiplus;
void MyListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);
	LPCTSTR lpszText = (LPCTSTR) lpDrawItemStruct->itemData;
	WCHAR wcsText[10]={0};
	::MultiByteToWideChar(CP_ACP,MB_COMPOSITE,lpszText,-1,wcsText,10);
	ASSERT(lpszText != NULL);
	CDC dc;

	dc.Attach(lpDrawItemStruct->hDC);
	dc.FillSolidRect(&lpDrawItemStruct->rcItem, 
		0);
	dc.Detach();
	Graphics gs(lpDrawItemStruct->hDC);
	gs.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		LinearGradientBrush linGrBrush(
			Point(0, 0),
			Point(lpDrawItemStruct->rcItem.right, 0),
			Color(100, 255, 0, 0),   // opaque red
			Color(0, 0, 0, 255));  // opaque blue

		gs.FillRectangle(&linGrBrush,
			lpDrawItemStruct->rcItem.left,
			lpDrawItemStruct->rcItem.top,
			lpDrawItemStruct->rcItem.right-lpDrawItemStruct->rcItem.left,
			lpDrawItemStruct->rcItem.bottom-lpDrawItemStruct->rcItem.top-2
			);
	}
	else
	{

	}

	if ((lpDrawItemStruct->itemAction | ODA_FOCUS) &&
		(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		//CBrush br(RGB(255, 0, 0));
		//dc.FrameRect(&lpDrawItemStruct->rcItem, &br);

	}

	Gdiplus::Font myFont(L"黑体", 10);
	RectF layoutRect(
		(REAL)lpDrawItemStruct->rcItem.left,
		(REAL)lpDrawItemStruct->rcItem.top,
		(REAL)lpDrawItemStruct->rcItem.right-lpDrawItemStruct->rcItem.left,
		(REAL)lpDrawItemStruct->rcItem.bottom-lpDrawItemStruct->rcItem.top
		);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	SolidBrush blackBrush(Color(255, 0, 0, 0));

	// Draw string.
	gs.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	gs.DrawString(
		wcsText,
		wcslen(wcsText),
		&myFont,
		layoutRect,
		&format,
		&blackBrush);
	gs.ReleaseHDC(lpDrawItemStruct->hDC);
}



BOOL MyListBox::OnEraseBkgnd(CDC* pDC)
{
	return true;
}


void MyListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{

	// TODO:  Add your code to determine the size of specified item
}
