// MyButton.cpp : 实现文件
//

#include "stdafx.h"
#include "MyButton.h"


// MyButton

IMPLEMENT_DYNAMIC(MyButton, CButton)
MyButton::MyButton():
	activeImage(NULL),
	normalImage(NULL),
	hoverImage(NULL)
{
	m_tracking = FALSE;
	m_hover = false;

}

MyButton::~MyButton()
{
}


BEGIN_MESSAGE_MAP(MyButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


using namespace Gdiplus;
void MyButton::DrawItem(LPDRAWITEMSTRUCT lpDS)
{
	HBRUSH hbr = ::CreateSolidBrush(0);
	::FillRect(lpDS->hDC,&lpDS->rcItem,hbr);
	CDC memdc;
	memdc.Attach(lpDS->hDC);
	CDC bufferdc;
	bufferdc.CreateCompatibleDC(&memdc);
	CBitmap buffermap;
	buffermap.CreateCompatibleBitmap(&memdc,lpDS->rcItem.right-lpDS->rcItem.left,lpDS->rcItem.bottom-lpDS->rcItem.top);
	bufferdc.SelectObject(&buffermap);
	//Graphics gs(lpDS->hDC);
	Graphics gs(bufferdc.GetSafeHdc());
	UINT W = activeImage->GetWidth(),H = activeImage->GetHeight();
	if(lpDS->itemState&ODS_SELECTED)//按钮按下
	{
		gs.DrawImage(activeImage,0,0,W,H);
	}
	else if(m_hover)//鼠标移上
	{
		gs.DrawImage(hoverImage,0,0,W,H);
	}
	else//普通形态
	{
		gs.DrawImage(normalImage,0,0,W,H);
	}
	//gs.ReleaseHDC(lpDS->hDC);
	gs.ReleaseHDC(bufferdc.GetSafeHdc());
	memdc.BitBlt(0,0,lpDS->rcItem.right-lpDS->rcItem.left,lpDS->rcItem.bottom-lpDS->rcItem.top,&bufferdc,0,0,SRCCOPY);
	memdc.Detach();

}


void MyButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_tracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_HOVER|TME_LEAVE;
		tme.dwHoverTime =1;
		tme.hwndTrack = m_hWnd;
		m_tracking = _TrackMouseEvent(&tme);
	}
	
	CButton::OnMouseMove(nFlags, point);
}


LRESULT MyButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if(m_hover == true)
	{
		m_hover = false;
		m_tracking = FALSE;
		Invalidate();
	}
	return 1;
}


LRESULT MyButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if(m_hover == false)
	{
		m_hover = true;
		m_tracking = FALSE;
		Invalidate();
	}
	return 1;
}


void MyButton::PreSubclassWindow()
{
	CButton::PreSubclassWindow();
	
	//CRgn rg;
	ModifyStyle(0, BS_OWNERDRAW);
	//GetClientRect(&rect);
	//rg.CreateEllipticRgn(rect.left,rect.top,rect.right,rect.bottom);
	//SetWindowRgn(rg,TRUE);
	//rg.DeleteObject();
}


BOOL MyButton::OnEraseBkgnd(CDC* pDC)
{
	return true;
}


BOOL MyButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	HCURSOR hCur  =  LoadCursor( NULL  , IDC_HAND ) ;
	::SetCursor(hCur);
	return TRUE;
	//return CButton::OnSetCursor(pWnd, nHitTest, message);
}
