#pragma once

#include "stdafx.h"
// MyButton

class MyButton : public CButton
{
	DECLARE_DYNAMIC(MyButton)

public:
	MyButton();
	virtual ~MyButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private :
	BOOL m_tracking;
	RECT rect;
	bool m_hover;
	virtual void PreSubclassWindow();
public:
	Gdiplus::Image*	normalImage;
	Gdiplus::Image*	hoverImage;
	Gdiplus::Image*	activeImage;
	//WCHAR *image[3];
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};


