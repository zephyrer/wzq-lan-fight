// MyEdit.cpp : ʵ���ļ�
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



// MyEdit ��Ϣ�������




//void MyEdit::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	RECT rc;
//	GetWindowRect(&rc);
//	dc.FillSolidRect(&rc,0);
//}


void MyEdit::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	ModifyStyle(0,BS_OWNERDRAW);
	CEdit::PreSubclassWindow();
}
