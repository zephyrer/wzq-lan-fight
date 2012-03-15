
// NetInterfaceDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <pdh.h>
#include <pdhmsg.h>
#define UPDATESPEED 345
#pragma comment(lib, "pdh.lib")
#define HALFHOUR 3600

// CNetInterfaceDlg �Ի���
class CNetInterfaceDlg : public CDialogEx
{
// ����
public:
	CNetInterfaceDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_NETINTERFACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	LONG HalfHour[HALFHOUR];//ÿ���¼һ������
	LONG SixHours[HALFHOUR];//ÿ�����¼һ������
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
