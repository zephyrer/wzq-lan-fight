
// NetInterfaceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NetInterface.h"
#include "NetInterfaceDlg.h"
#include "afxdialogex.h"
#pragma comment(lib, "IPHLPAPI.lib")
#include <iphlpapi.h>
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CNetInterfaceDlg::CNetInterfaceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNetInterfaceDlg::IDD, pParent)
	, m_minspeed(0)
{
	Query = NULL;
	sixtag=0;
	halftag=0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	MaxSpeed = 100000;
}

void CNetInterfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, ID_ADAPTERLIST, m_adpterlist);
	DDX_Text(pDX, ID_MINSPEED, m_minspeed);
	DDV_MinMaxUInt(pDX, m_minspeed, 0, UINT_MAX);
	DDX_Control(pDX, ID_TIMELIMIT, m_timesel);
	DDX_Control(pDX, ID_COMBO_OPS, m_ops);
	DDX_Control(pDX, IDC_COMBO1, m_adpterlist);
}

BEGIN_MESSAGE_MAP(CNetInterfaceDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CNetInterfaceDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CNetInterfaceDlg::OnBnClickedCancel)
//	ON_WM_ERASEBKGND()
//ON_CBN_SELCHANGE(ID_TIMELIMIT, &CNetInterfaceDlg::OnCbnSelchangeTimelimit)
END_MESSAGE_MAP()


// CNetInterfaceDlg ��Ϣ�������

BOOL CNetInterfaceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if(!InitPDH())
	{
		MessageBox(L"Failed to initial PDH");
		exit(0);
	}
	SetFuncCont();
	GetDlgItem(ID_GRAPHAREA)->GetWindowRect(&grapharea);
	ScreenToClient(&grapharea);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CNetInterfaceDlg::OnPaint()
{
	if (IsIconic())
	{
		//CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		//SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		//// ʹͼ���ڹ����������о���
		//int cxIcon = GetSystemMetrics(SM_CXICON);
		//int cyIcon = GetSystemMetrics(SM_CYICON);
		//CRect rect;
		//GetClientRect(&rect);
		//int x = (rect.Width() - cxIcon + 1) / 2;
		//int y = (rect.Height() - cyIcon + 1) / 2;

		//// ����ͼ��
		//dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		DrawSpeedGraph(&dc);
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CNetInterfaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNetInterfaceDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDlgItemTextW(IDOK,L"���ڼ��ӡ���");
	GetDlgItem(IDC_COMBO1)->EnableWindow(false);
	GetDlgItem(IDOK)->EnableWindow(false);
	InitSpeedQuery();
	UpdateSpeed();
	SetTimer(UPDATESPEED,1000,NULL);
}

//��ʼ�������б�2.0
bool CNetInterfaceDlg::InitPDH(void){

	DWORD dwSize = 0;
	DWORD dwRetVal = 0;
	ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
	ULONG family = AF_INET;
	LPVOID lpMsgBuf = NULL;
	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	ULONG outBufLen = 0;
	PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
	outBufLen = sizeof (IP_ADAPTER_ADDRESSES);
	pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
	if (GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen)
		== ERROR_BUFFER_OVERFLOW) {
			FREE(pAddresses);
			pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
	}

	if (pAddresses == NULL) {
		exit(1);
	}
	dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

	if (dwRetVal == NO_ERROR) {
		pCurrAddresses = pAddresses;
		m_nAdpterList = 0;
		while (pCurrAddresses) {
			if(pCurrAddresses->OperStatus==IfOperStatusUp
				&&pCurrAddresses->IfType!=IF_TYPE_TUNNEL
				&&pCurrAddresses->IfType!=IF_TYPE_SOFTWARE_LOOPBACK)
			{
				int i = 0;
				while(pCurrAddresses->Description[i]){
					if(pCurrAddresses->Description[i]=='(')
						pCurrAddresses->Description[i]='[';
					else if(pCurrAddresses->Description[i]==')')
						pCurrAddresses->Description[i]=']';
					i++;
				}
				m_strAdpter[m_nAdpterList] = pCurrAddresses->Description;
				m_adpterlist.AddString(pCurrAddresses->Description);
				m_nAdpterList++;
			}
			pCurrAddresses = pCurrAddresses->Next;
		}
		m_adpterlist.SetCurSel(0);
		return true;
	}
	return false;
}
//bool CNetInterfaceDlg::InitPDH(void)//��ʼ�������б�1.0 ------������ȷָ���������ӵ��������������Ϊ2.0
//{
//	CONST LPWSTR COUNTER_OBJECT = L"Network Interface";
//	PDH_STATUS status = ERROR_SUCCESS;
//	LPWSTR pwsCounterListBuffer = NULL;
//	DWORD dwCounterListSize = 0;
//	LPWSTR pwsInstanceListBuffer = NULL;
//	DWORD dwInstanceListSize = 0;
//	LPWSTR pTemp = NULL;
//
//	// ��ȡ���軺�����Ĵ�С
//	status = PdhEnumObjectItems(
//		NULL,                   // real-time source
//		NULL,                   // local machine
//		COUNTER_OBJECT,         // object to enumerate
//		pwsCounterListBuffer,   // pass NULL and 0
//		&dwCounterListSize,     // to get required buffer size
//		pwsInstanceListBuffer, 
//		&dwInstanceListSize, 
//		PERF_DETAIL_WIZARD,     // counter detail level
//		0); 
//	if (status == PDH_MORE_DATA) 
//      {
//          //�õ������С��λ�����������ڴ�
//          pwsCounterListBuffer = (LPWSTR)malloc(dwCounterListSize * sizeof(WCHAR));
//          pwsInstanceListBuffer = (LPWSTR)malloc(dwInstanceListSize * sizeof(WCHAR));
//  
//          if (NULL != pwsCounterListBuffer && NULL != pwsInstanceListBuffer) 
//          {
//              status = PdhEnumObjectItems(
//                  NULL,                   // real-time source
//                  NULL,                   // local machine
//                  COUNTER_OBJECT,         // object to enumerate
//                  pwsCounterListBuffer, 
//                  &dwCounterListSize,
//                  pwsInstanceListBuffer, 
//                  &dwInstanceListSize, 
//                  PERF_DETAIL_WIZARD,     // counter detail level
//                  0); 
//       
//              if (status == ERROR_SUCCESS) 
//              {
//				  m_nAdpterList=0;
//				  for (pTemp = pwsInstanceListBuffer; *pTemp != 0; pTemp += wcslen(pTemp) + 1) 
//                  {
//					  m_strAdpter[m_nAdpterList]=pTemp;
//					  m_nAdpterList++;
//					  m_adpterlist.AddString(pTemp);
//                  }
//				  m_adpterlist.SetCurSel(0);
//			  }
//			  else
//				  return false;
//		  }
//		  else
//			  return false;
//	}
//	else
//		return false;
//	return true;
//}


void CNetInterfaceDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDEvent == UPDATESPEED)
		UpdateSpeed();
	CDialogEx::OnTimer(nIDEvent);
}


void CNetInterfaceDlg::UpdateSpeed(void)
{

	Status = PdhCollectQueryData(Query);
	if (Status != ERROR_SUCCESS) 
		CleanExit(L"failed to collect query data.",Query,Status);
	Status = PdhGetFormattedCounterValue(Counter,PDH_FMT_DOUBLE,&CounterType,&DisplayValue);
	if (Status != ERROR_SUCCESS) 
		CleanExit(L"failed to get formatted counter value.",Query,Status);
	CString spd;
	if(DisplayValue.doubleValue<1024)
	{
		spd.Format(L"%.0f B/s",DisplayValue.doubleValue);
	}
	else if(DisplayValue.doubleValue<1024*1024)
	{
		spd.Format(L"%.1f KB/s",DisplayValue.doubleValue/1024);
	}
	else{
		spd.Format(L"%.2f MB/s",DisplayValue.doubleValue/(1024*1024));
	}
	Invalidate();
	SetDlgItemTextW(ID_REALSPEED,spd);
	HalfHour[halftag%HALFHOUR]=DisplayValue.doubleValue;
	if(DisplayValue.doubleValue>MaxSpeed)
	{
		SetDlgItemTextW(ID_MAXSPEED,spd);
		MaxSpeed = DisplayValue.doubleValue;
	}
	
	if(halftag%6==0)
	{
		SixHours[sixtag%HALFHOUR]=HalfHour[halftag%HALFHOUR];
		for(int i=1;i<6;i++)
			SixHours[sixtag%HALFHOUR]+=HalfHour[(halftag-i)%HALFHOUR];
		SixHours[sixtag%HALFHOUR]/=6;
		sixtag++;
	}
	if(m_timesel.GetCurSel()==0||sixtag%(m_timesel.GetCurSel()*30*10)==0)
		CheckAvgSpeed();
	halftag++;
}


void CNetInterfaceDlg::CleanExit(LPCTSTR msgstr,HQUERY fQuery,PDH_STATUS fstatus)
{
	MessageBox(msgstr);
	if(MessageBoxW(L"quit?",L"confirm",MB_YESNO)==6)
	{
		PdhCloseQuery(fQuery);
		exit(fstatus);
	}
}


bool CNetInterfaceDlg::InitSpeedQuery(void)
{
	WORD n_selectAdapter = m_adpterlist.GetCurSel();
	wsprintf(CounterPathBuffer,L"\\Network Interface(%s)\\Bytes Received/sec",m_strAdpter[n_selectAdapter]);
	Status = PdhOpenQuery(NULL, NULL, &Query);
	if (Status != ERROR_SUCCESS) 
		CleanExit(L"failed to open query.",Query,Status);
	Status = PdhAddCounter(Query, CounterPathBuffer, 0, &Counter);
	if (Status != ERROR_SUCCESS) 
		CleanExit(L"failed to add counter.",Query,Status);
	SYSTEMTIME nowtime;
	::GetLocalTime(&nowtime);
	WCHAR timestr[50];
	wsprintf(timestr,L"��ʼʱ��: %4d��%2d��%2d�� - %2d:%2d:%2d",
		nowtime.wYear,nowtime.wMonth,nowtime.wDay,nowtime.wHour,nowtime.wMinute,nowtime.wSecond);
	SetDlgItemTextW(ID_BEGINTIME,timestr);
	Status = PdhCollectQueryData(Query);
	if (Status != ERROR_SUCCESS) 
		CleanExit(L"failed to collect query data.",Query,Status);
	SetDlgItemTextW(ID_MAXSPEED,L"100.0 KB/s");
	return true;
}


void CNetInterfaceDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(Query)
		PdhCloseQuery(Query);
	CDialogEx::OnCancel();
}


void CNetInterfaceDlg::DrawSpeedGraph(CPaintDC* dc)
{
	CDC memDC;
	memDC.CreateCompatibleDC(dc);
	CBitmap bt;
	bt.LoadBitmapW(IDB_BITMAP1);
	BITMAP hbt;
	bt.GetBitmap(&hbt);
	memDC.SelectObject(&bt);

	memDC.MoveTo(0,0);
	memDC.LineTo(0,grapharea.bottom-grapharea.top);
	memDC.LineTo(grapharea.right-grapharea.left,grapharea.bottom-grapharea.top);
	memDC.LineTo(grapharea.right-grapharea.left,0);
	memDC.LineTo(0,0);
	memDC.MoveTo(0,grapharea.bottom-grapharea.top-1);//��ͼ�������½ǵ�ԭ��
	memDC.SelectObject(&CPen(PS_SOLID,2,RGB(23,233,120)));
	INT width = grapharea.right-grapharea.left-1;
	INT height = grapharea.bottom-grapharea.top-1;
	if(halftag<width/2)
	{
		INT tmp = 0,Y;
		while(tmp<halftag)
		{
			Y = height*(1-(HalfHour[tmp]/MaxSpeed));
			memDC.LineTo(2*(tmp++),Y);
		}
	}
	else
	{
		INT Y,tmp = 0;
		while(tmp<width/2)
		{
			Y = height*(1-(HalfHour[tmp+(halftag-width/2)]/MaxSpeed));
			memDC.LineTo(2*(tmp++),Y);
		}
	}
	dc->BitBlt(grapharea.left,
			grapharea.top,
			grapharea.right-grapharea.left+5,
			grapharea.bottom-grapharea.top+3,
			&memDC,0,0,SRCCOPY);
}

void CNetInterfaceDlg::CheckAvgSpeed(void)
{
	if(m_timesel.GetCurSel()==0)
	{
		if(HalfHour[halftag]<m_minspeed*1024)
			ExcuteOption();
	}
	else
	{
		UINT minutespan = m_timesel.GetCurSel()*30;
		DOUBLE avgspeed = 0;
		for(UINT i = sixtag-minutespan*10;i<sixtag;i++)
			avgspeed += SixHours[i];
		avgspeed /= sixtag-minutespan*10;
		if(avgspeed < m_minspeed)
			ExcuteOption();
	}
}


void CNetInterfaceDlg::SetFuncCont(void)
{
	m_ops.AddString(L"����/����");
	m_ops.AddString(L"�ػ�");
	m_ops.SetCurSel(1);
	m_timesel.AddString(L"       0��");
	m_timesel.AddString(L"     30��");
	m_timesel.AddString(L"     60��");
	m_timesel.AddString(L"1Сʱ30��");
	m_timesel.AddString(L"2Сʱ  0��");
	m_timesel.AddString(L"2Сʱ30��");
	m_timesel.AddString(L"3Сʱ  0��");
	m_timesel.AddString(L"4Сʱ  0��");
	m_timesel.SetCurSel(1);
	m_minspeed = 100;
	UpdateData(0);
}



void CNetInterfaceDlg::ExcuteOption(void)
{
	switch(m_ops.GetCurSel())
	{
	case 0:
		//����������һ������
		system("rundll32.exe powrprof.dll,SetSuspendState");
		exit(0);
		break;
	case 1:
		//�ػ�����
		system("shutdown -s -t 30");
		if(MessageBox(L"ϵͳ����30����Զ��ر�",L"��ȷ��",MB_YESNO)==1)
			system("shutdown -a");
		break;
	default:
		break;
	}
}
