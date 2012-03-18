
// WZQ.cpp : 定义应用程序的类行为。
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


// CWZQApp 构造

CWZQApp::CWZQApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

}


// 唯一的一个 CWZQApp 对象

CWZQApp theApp;

BOOL CWZQApp::InitInstance()
{
	CWinApp::InitInstance();
	ULONG_PTR gdiplusToken;//GDI+的记号，用来启动和关闭GDI+
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

