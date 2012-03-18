
// WZQ.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "WZQ.h"
#include "WZQDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWZQApp

BEGIN_MESSAGE_MAP(CWZQApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CWZQApp ����

CWZQApp::CWZQApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

}


// Ψһ��һ�� CWZQApp ����

CWZQApp theApp;

BOOL CWZQApp::InitInstance()
{
	CWinApp::InitInstance();
	ULONG_PTR gdiplusToken;//GDI+�ļǺţ����������͹ر�GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	AfxSocketInit();
	CWZQDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	Gdiplus::GdiplusShutdown(gdiplusToken);
	exit(0);
	return FALSE;
}

