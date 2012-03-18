// MyEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "WZQ.h"
#include "MyEdit.h"


// MyEdit

IMPLEMENT_DYNAMIC(MyEdit, CEdit)

MyEdit::MyEdit()
{

}

MyEdit::~MyEdit()
{
}


BEGIN_MESSAGE_MAP(MyEdit, CEdit)
//	ON_WM_PAINT()
END_MESSAGE_MAP()



// MyEdit 消息处理程序




//void MyEdit::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	RECT rc;
//	GetWindowRect(&rc);
//	dc.FillSolidRect(&rc,0);
//}


void MyEdit::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle(0,BS_OWNERDRAW);
	CEdit::PreSubclassWindow();
}
