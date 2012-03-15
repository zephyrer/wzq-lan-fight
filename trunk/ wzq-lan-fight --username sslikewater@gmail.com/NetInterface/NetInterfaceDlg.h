
// NetInterfaceDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <pdh.h>
#include <pdhmsg.h>
#define UPDATESPEED 345
#pragma comment(lib, "pdh.lib")
#define HALFHOUR 3600

// CNetInterfaceDlg 对话框
class CNetInterfaceDlg : public CDialogEx
{
// 构造
public:
	CNetInterfaceDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_NETINTERFACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	CListBox m_adpterlist;
	RECT grapharea;
	PDH_STATUS Status;
    HQUERY Query;
    HCOUNTER Counter;
	INT halftag;
	INT sixtag;
	LONG HalfHour[HALFHOUR];//每秒记录一个数据
	LONG SixHours[HALFHOUR];//每六秒记录一个数据
	DOUBLE MaxSpeed;
    PDH_FMT_COUNTERVALUE DisplayValue;
    DWORD CounterType;
	WCHAR CounterPathBuffer[PDH_MAX_COUNTER_PATH];
	WCHAR SelectedAdapter[200];
	WORD m_nAdpterList;
	LPCTSTR m_strAdpter[10];
	afx_msg void OnBnClickedOk();
	bool InitPDH(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void UpdateSpeed(void);
	void CleanExit(LPCTSTR msgstr,HANDLE fQuery,LONG fstatus);
	bool InitSpeedQuery(void);
	afx_msg void OnBnClickedCancel();
	void DrawSpeedGraph(CPaintDC* dc);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void CheckAvgSpeed(void);
	UINT m_minspeed;
	CComboBox m_timesel;
	CComboBox m_ops;
	void SetFuncCont(void);
//	afx_msg void OnCbnSelchangeTimelimit();
	void ExcuteOption(void);
	CComboBox m_adpterlist;
};
