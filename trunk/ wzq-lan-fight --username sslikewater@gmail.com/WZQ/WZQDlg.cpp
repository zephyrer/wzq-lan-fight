
// WZQDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WZQ.h"
#include "WZQDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
UINT mutex;

using namespace Gdiplus;

CWZQDlg::CWZQDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CWZQDlg::IDD, pParent),
m_gameStart(false),
m_clientLength(554.0f),
m_uid(0),
m_receving(false),
m_appruning(true)

{
	ZeroMemory(grid,sizeof(grid));
	blockLength=m_clientLength/15.0F;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_challenge.hoverImage = Image::FromFile(L"img\\hoverB.png");
	m_challenge.normalImage = Image::FromFile(L"img\\normalB.png");
	m_challenge.activeImage = Image::FromFile(L"img\\activeB.png");

	m_BangButton.normalImage = Image::FromFile(L"img\\normalC.png");
	m_BangButton.hoverImage = Image::FromFile(L"img\\hoverC.png");
	m_BangButton.activeImage = Image::FromFile(L"img\\activeC.png");

	m_searchButton.normalImage = Image::FromFile(L"img\\normalS.png");
	m_searchButton.hoverImage = Image::FromFile(L"img\\hoverS.png");
	m_searchButton.activeImage = Image::FromFile(L"img\\activeS.png");

	bkImage = Image::FromFile(L"img\\prod.jpg");

	nullCur  =  LoadCursor( NULL  , MAKEINTRESOURCE (ID_NULLCUR) ) ;
	arrowCur = LoadCursor(NULL, IDC_UPARROW);
}

void CWZQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_gameList);
	DDX_Control(pDX, ID_SERVER, m_BangButton);
	DDX_Control(pDX, ID_CHALLENGE, m_challenge);
	DDX_Control(pDX, ID_NAME, m_name);
	DDX_Control(pDX, ID_SEARCHGAME, m_searchButton);
}

BEGIN_MESSAGE_MAP(CWZQDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(ID_SERVER, &CWZQDlg::OnBnClickedServer)
	ON_BN_CLICKED(ID_CHALLENGE, &CWZQDlg::OnBnClickedChallenge)
	ON_BN_CLICKED(ID_SEARCHGAME, &CWZQDlg::OnBnClickedSearchgame)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CWZQDlg 消息处理程序

BOOL CWZQDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	WZQInit();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWZQDlg::OnPaint()
{
	CRect clientrc;
	GetWindowRect(&clientrc);
	CPaintDC dc(this);
	dc.FillSolidRect((INT)m_clientLength-2,0,(INT)m_clientLength,(INT)m_clientLength,RGB(0,0,0));
	CDialogEx::OnPaint();
}


HCURSOR CWZQDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWZQDlg::WZQInit(void)
{
	MARGINS margins;
	margins.cxLeftWidth = 0;
	margins.cxRightWidth = 190;
	margins.cyTopHeight = 0;
	margins.cyBottomHeight = 0;
	DwmExtendFrameIntoClientArea(GetSafeHwnd(), &margins);

	::SetCursor(nullCur);
	m_gameList.AddString("游戏列表");
	m_paintMutex = ::CreateMutex(NULL,FALSE,NULL);
	m_pieceMutex = ::CreateMutex(NULL,FALSE,NULL);
	AfxBeginThread(PaintThread,this);
	

}


void CWZQDlg::GetSocketError(void)
{
	DWORD errorCode = WSAGetLastError();
	CString error="NaN socket error";
	switch(errorCode)
	{
	case WSANOTINITIALISED:
		error = "A successful AfxSocketInit must occur before using this API. ";
		break;

	case WSAENETDOWN:
		error = "The Windows Sockets implementation detected that the network subsystem failed. ";
		break;

	case WSAENOTCONN:
		error = "The socket is not connected. ";
		break;

	case WSAEINPROGRESS:
		error = "A blocking Windows Sockets operation is in progress. ";
		break;

	case WSAENOTSOCK:
		error = "The descriptor is not a socket. ";
		break;

	case WSAEOPNOTSUPP:
		error = "MSG_OOB was specified, but the socket is not of type SOCK_STREAM. ";
		break;

	case WSAESHUTDOWN:
		error = "The socket has been shut down; it is not possible to call Receive on a socket after ShutDown has been invoked with nHow set to 0 or 2. ";
		break;

	case WSAEWOULDBLOCK:
		error = "The socket is marked as nonblocking and the Receive operation would block. ";
		break;

	case WSAEMSGSIZE:
		error = "The datagram was too large to fit into the specified buffer and was truncated. ";
		break;

	case WSAEINVAL:
		error = "The socket has not been bound with Bind. ";
		break;

	case WSAECONNABORTED:
		error = "The virtual circuit was aborted due to timeout or other failure. ";
		break;

	case WSAECONNRESET:
		error = "The virtual circuit was reset by the remote side. ";
		break;
	}
	MessageBox(error);
}


void CWZQDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	m_mouse=point;
	CDialogEx::OnMouseMove(nFlags, point);
}


void CWZQDlg::OnClose()
{
	m_appruning = false;
	MsgPack msp;
	msp.type = QUIT_GAME;
	SendSock.Send(&msp,sizeof(MsgPack));
	::WaitForSingleObject(m_paintMutex,INFINITE);
	CDialogEx::OnClose();
}


BOOL CWZQDlg::OnEraseBkgnd(CDC* pDC)
{
	return true;
}


LRESULT CWZQDlg::OnNcHitTest(CPoint point)
{
	RECT rc;
	GetWindowRect(&rc);
	if(point.x-rc.left>556)
		return HTCAPTION;
	else
		return CDialogEx::OnNcHitTest(point);
}


BOOL CWZQDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	POINT cp;
	RECT rc;
	GetWindowRect(&rc);
	::GetCursorPos(&cp);
	if(cp.x-rc.left<554&&cp.y-rc.top>26)
		::SetCursor(nullCur);
	else
		::SetCursor(arrowCur);
	return TRUE;
}


void CWZQDlg::OnBnClickedServer()
{
	if(m_receving)
		return;
	m_uid = 1;
	AfxBeginThread(HostThread,this);
	AfxBeginThread(ReceiveThread,this);
}


void CWZQDlg::OnBnClickedChallenge()
{
	if(m_gameStart)
		return;
	SetWindowTextA("尝试与服务器建立连接");
	m_uid = 2;
	AfxBeginThread(ReceiveThread,this);
}


UINT CWZQDlg::PaintThread(LPVOID param)
{
	CWZQDlg* pwd = (CWZQDlg*)param;
	UINT sideLength = 554;
	REAL blockLength = sideLength/15.0F;
	POINT or = pwd->m_mouse;
	HPEN hp = ::CreatePen(PS_SOLID,8,RGB(255,0,0));
	::WaitForSingleObject(pwd->m_paintMutex,INFINITE);
	while(pwd->m_appruning)
	{
		CDC *pdc =pwd->GetDC();
		CBitmap memb;
		memb.CreateCompatibleBitmap(pdc,sideLength,sideLength);
		CDC memdc;
		memdc.CreateCompatibleDC(pdc);
		memdc.SelectObject(memb);
		Graphics gs(memdc.GetSafeHdc());
		gs.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		gs.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
		gs.DrawImage(pwd->bkImage,0,0,sideLength,sideLength);
		::WaitForSingleObject(pwd->m_pieceMutex,INFINITE);
		for(UINT i=0;i<pwd->pieces.size();i++)
		{
			gs.FillEllipse(
				&SolidBrush(Color(245,(pwd->pieces.at(i).uid-1)*255,(pwd->pieces.at(i).uid-1)*230,200)),
				pwd->pieces.at(i).x*blockLength-20,
				pwd->pieces.at(i).y*blockLength-20,
				38.0f,
				38.0f
				);
		}
		::ReleaseMutex(pwd->m_pieceMutex);
		gs.FillEllipse(&SolidBrush(Color(120,2,22,233)),(UINT)(1+or.x/blockLength)*blockLength-20,(UINT)(1+or.y/blockLength)*blockLength-20,38.0f,38.0f);
		or = pwd->m_mouse;
		pdc->BitBlt(0,0,sideLength,sideLength,&memdc,0,0,SRCCOPY);
		gs.ReleaseHDC(memdc.GetSafeHdc());
		memdc.DeleteDC();
		pwd->ReleaseDC(pdc);
		Sleep(50);
	}
	::ReleaseMutex(pwd->m_paintMutex);
	return 0;
}

UINT CWZQDlg::ReceiveThread(LPVOID param)
{
	CWZQDlg* pwd = (CWZQDlg*)param;
	if(pwd->m_gameStart)
		return 0;
	
	if(pwd->m_uid == 1)//服务器模式
	{
		pwd->SetWindowTextA("已建立游戏、等待其他人加入");
		CSocket listSock,lisRecvSock;

		if(!listSock.Create(pwd->m_ServerPort))
		{
			pwd->GetSocketError();
			return -1;
		}
		if(!lisRecvSock.Create(pwd->m_ServerPortRecv))
		{
			pwd->GetSocketError();
			return -1;
		}
		
		lisRecvSock.Listen(3);
		listSock.Listen(3);

		if(!listSock.Accept(pwd->SendSock))//给客户端发送数据socket连接
		{
			pwd->GetSocketError();
			return -1;
		}
		MsgPack msp;
		msp.type = ALLOT_ID;
		msp.data.uid = pwd->m_uid+1;
		//分配客户ID
		if(SOCKET_ERROR==pwd->SendSock.Send(&msp,sizeof(MsgPack)))
		{
			pwd->GetSocketError();
			return -1;
		}
		if(!lisRecvSock.Accept(pwd->RecvSock))//从客户端接收数据socket连接
		{
			pwd->GetSocketError();
			return -1;
		}

		//接收加入游戏确认信息
		//if(SOCKET_ERROR==pwd->RecvSock.Receive(&msp,sizeof(MsgPack)))
		//{
		//	pwd->GetSocketError();
		//	return -1;
		//}
		//if(msp.type == JOIN_GAME)
		//{
		//	pwd->SetWindowTextA("成功建立游戏连接");
		//}
		
	}
	else//客户端模式
	{
		MsgPack msp;
		pwd->SendSock.Create();
		pwd->RecvSock.Create();
		//从服务器接收数据的socket
		if(!pwd->RecvSock.Connect(pwd->hostAddrSock,m_ServerPort))
		{
			pwd->GetSocketError();
			return -1;
		}
		//从服务器接收ID分配的数据
		if(SOCKET_ERROR==pwd->RecvSock.Receive(&msp,sizeof(MsgPack)))
		{
			pwd->GetSocketError();
			return -1;
		}
		if(msp.type == ALLOT_ID)
		{
			pwd->m_uid = msp.data.uid;
		}
		//发送数据到服务器的socket
		if(!pwd->SendSock.Connect(pwd->hostAddrSock,m_ServerPortRecv))
		{
			pwd->GetSocketError();
			return -1;
		}
		msp.type = JOIN_GAME;
		//给服务器发送加入游戏的确认信息
		if(SOCKET_ERROR==pwd->RecvSock.Send(&msp,sizeof(MsgPack)))
		{
			pwd->GetSocketError();
			return -1;
		}
		pwd->SetWindowTextA("已连接上服务器");
	}
	pwd->SetWindowTextA("开始游戏！");
	
	pwd->m_gameStart = true;
	pwd->m_receving = true;
	while (pwd->m_appruning)//开始接收游戏数据
	{
		MsgPack msp;
		
		if(pwd->RecvSock.Receive(&msp,sizeof(MsgPack)) == SOCKET_ERROR)
		{
			pwd->GetSocketError();
			return -1;
		}

		if (msp.type == MOVE)
		{
			UINT winner = pwd->PlaceRecv(msp.data.PD.x,msp.data.PD.y,msp.data.PD.uid);
			if(0==winner)
			{
				continue;
			}
			else if (winner==pwd->m_uid)
			{
				pwd->MessageBox("你赢了！！");
				pwd->CleanGrid();
				return 0;
			}
			else
			{
				pwd->MessageBox("你输了");
				pwd->CleanGrid();
				return 0;
			}
		}
		else if (msp.type == QUIT_GAME)
		{
			return 0;
		}
	}
	return 0;
}

//将pieces中最后一个元素发送给对方
UINT CWZQDlg::SendThread(LPVOID param)
{
	CWZQDlg* pwd = (CWZQDlg*)param;
	MsgPack msp;

	return 0;
}

UINT CWZQDlg::SearchThread(LPVOID param)
{
	CWZQDlg* pwd = (CWZQDlg*)param;
	if(pwd->m_gameStart)
		return 0;
	MsgPack msp;
	UINT hostPort;
	BOOL broadcast=TRUE;
	msp.type = SEARCH_HOST;
	CSocket searchSock;//创建广播UDP socket用来搜索局域网上的服务器

	searchSock.Create(0,SOCK_DGRAM);
	searchSock.SetSockOpt(SO_BROADCAST,&broadcast,sizeof(BOOL));
	searchSock.SendTo(&msp,sizeof(MsgPack),pwd->m_SearchPort,"255.255.255.255");
	searchSock.ReceiveFrom(&msp,sizeof(MsgPack),pwd->hostAddrSock,hostPort);
	if(msp.type == HOST_RESPONSE)
	{
		LPSTR serverName = (CHAR*)::malloc(strlen(msp.data.SR.server)+1);

		strcpy_s(serverName,7,msp.data.SR.server);
		pwd->m_gameList.AddString(serverName);
	}
	return 0;
}


UINT CWZQDlg::HostThread(LPVOID param)
{
	CWZQDlg* pwd = (CWZQDlg*)param;
	MsgPack msp;
	CString clientAddr;
	UINT clientPort;
	CSocket hostSock;//建立服务器，接收并回应UDP广播搜索的socket

	hostSock.Create(pwd->m_SearchPort,SOCK_DGRAM);

	pwd->SetWindowTextA("已建立服务器、等待用户");
	hostSock.ReceiveFrom(&msp,sizeof(MsgPack),clientAddr,clientPort);
	if(msp.type == SEARCH_HOST)
	{
		MsgPack hostMsg;
		hostMsg.type = HOST_RESPONSE;
		pwd->m_name.GetWindowTextA(hostMsg.data.SR.server,sizeof(hostMsg.data.SR.server)-1);
		hostMsg.data.SR.uesrNum = 1;
		Sleep(300);
		hostSock.SendTo(&hostMsg,sizeof(MsgPack),clientPort,clientAddr);
	}

	return 0;
}


void CWZQDlg::OnBnClickedSearchgame()
{
	AfxBeginThread(SearchThread,this);
}


void CWZQDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(!m_gameStart)
		return;
	m_gameStart = false;
	Piece pe;
	pe.uid = m_uid;
	pe.x =(BYTE)(point.x/blockLength+1);
	pe.y =(BYTE)(point.y/blockLength+1);
	if(pe.x>=1&&pe.x<=14
		&&
		pe.y>=1&&pe.y<=14
		&&
		!grid[pe.x][pe.y].used
		)
	{
		::WaitForSingleObject(m_pieceMutex,INFINITE);
		pieces.push_back(pe);
		::ReleaseMutex(m_pieceMutex);
		MsgPack msp;
		msp.type = MOVE;
		msp.data.PD.uid = m_uid;
		msp.data.PD.x = pe.x;
		msp.data.PD.y = pe.y;
		int	result = SendSock.Send(&msp,sizeof(MsgPack));
		if(result == SOCKET_ERROR||result == 0)
		{
			GetSocketError();
			return;
		}
		UINT winner = Place(pe.x,pe.y);
		if (winner==m_uid)
		{
			MessageBox("你赢了！！");
			CleanGrid();
		}

	}
	m_gameStart = true;
}


UINT CWZQDlg::Place(BYTE x, BYTE y)
{
	if (grid[x][y].used)
	{
		return 0;
	}
	grid[x][y].used = 1;
	grid[x][y].uid = m_uid;

	return SetDotValue(x,y,m_uid,0);
}


UINT CWZQDlg::PlaceRecv(BYTE x, BYTE y, BYTE UID)
{
	grid[x][y].used = 1;
	grid[x][y].uid = UID;
	Piece pe;
	pe.uid = UID;
	pe.x=x;
	pe.y=y;
	::WaitForSingleObject(m_pieceMutex,INFINITE);
	pieces.push_back(pe);
	::ReleaseMutex(m_pieceMutex);

	return SetDotValue(x,y,UID,0);
}


// 更新棋盘所有节点的值
UINT CWZQDlg::SetDotValue(BYTE x, BYTE y, BYTE UID, BYTE dir)
{
	if (dir == 0)
	{
		for (int i = 1; i <= 8; i++)
		{
			if ((*DirLocDot(x,y,i)).uid == UID)
			{
				DirSetDotValue(x,y,i,DirGetDotValue((*DirLocDot(x,y,i)),i)+1);
				BYTE a,b;
				DirModify(x,y,i,a,b);
				SetDotValue(a,b,UID,i);
			}
		}
		if(
			grid[x][y].t	+	grid[x][y].b	>=4||
			grid[x][y].l	+	grid[x][y].r	>=4||
			grid[x][y].lt	+	grid[x][y].rb	>=4||
			grid[x][y].rt	+	grid[x][y].lb	>=4
			)
			return grid[x][y].uid;
	}
	else
	{
		BYTE from_dir;
		if(dir%2 == 0)
		{
			from_dir = dir - 1;
		}
		else
		{
			from_dir = dir + 1;
		}
		DirSetDotValue(x,y,from_dir,DirGetDotValue((*DirLocDot(x,y,from_dir)),from_dir)+1);
		if ((*DirLocDot(x,y,dir)).uid == UID)
		{
			BYTE a,b;
			DirModify(x,y,dir,a,b);
			SetDotValue(a,b,UID,dir);
		}
	}	
	return 0;
}


/*
 *	规定方向代码
 *	1  3  5
 *	8  ×  7
 *	6  4  2
 */

// 定位不同方向的棋盘格点
Dot* CWZQDlg::DirLocDot(BYTE x, BYTE y, BYTE dir)
{
	switch (dir)
	{
	case 1:
		return &grid[x-1][y-1];
		break;
	case 2:
		return &grid[x+1][y+1];
		break;
	case 3:
		return &grid[x][y-1];
		break;
	case 4:
		return &grid[x][y+1];
		break;
	case 5:
		return &grid[x+1][y-1];
		break;
	case 6:
		return &grid[x-1][y+1];
		break;
	case 7:
		return &grid[x+1][y];
		break;
	case 8:
		return &grid[x-1][y];
		break;
	default:
		return NULL;
		break;
	}
}


// 获取节点不同方向的值
BYTE CWZQDlg::DirGetDotValue(Dot dot, BYTE dir)
{
	switch (dir)
	{
	case 1:
		return dot.lt;
		break;
	case 2:
		return dot.rb;
		break;
	case 3:
		return dot.t;
		break;
	case 4:
		return dot.b;
		break;
	case 5:
		return dot.rt;
		break;
	case 6:
		return dot.lb;
		break;
	case 7:
		return dot.r;
		break;
	case 8:
		return dot.l;
		break;
	default:
		return 0;
		break;
	}
}


// 设置节点不同方向的值
void CWZQDlg::DirSetDotValue(BYTE x, BYTE y, BYTE dir, BYTE value)
{
	switch (dir)
	{
	case 1:
		grid[x][y].lt = value;
		break;
	case 2:
		grid[x][y].rb = value;
		break;
	case 3:
		grid[x][y].t = value;
		break;
	case 4:
		grid[x][y].b = value;
		break;
	case 5:
		grid[x][y].rt = value;
		break;
	case 6:
		grid[x][y].lb = value;
		break;
	case 7:
		grid[x][y].r = value;
		break;
	case 8:
		grid[x][y].l = value;
		break;
	default:
		break;
	}
}


// 根据方向设置新的x,y坐标值
void CWZQDlg::DirModify(BYTE ori_x,BYTE ori_y,BYTE dir,BYTE& to_x, BYTE& to_y)
{
	if (dir == 1||dir == 6||dir == 8)
			{
				to_x = ori_x-1;
			}
			else if (dir == 3||dir == 4)
			{
				to_x = ori_x;
			}
			else
			{
				to_x = ori_x+1;
			}
			if (dir == 1||dir == 3||dir == 5)
			{
				to_y = ori_y-1;
			}
			else if (dir == 7||dir == 8)
			{
				to_y = ori_y;
			}
			else
			{
				to_y = ori_y+1;
			}
}


// 清楚棋盘信息
void CWZQDlg::CleanGrid(void)
{
	::WaitForSingleObject(m_pieceMutex,INFINITE);
	pieces.erase(pieces.begin(),pieces.end());
	::ReleaseMutex(m_pieceMutex);
	ZeroMemory(grid,sizeof(grid));
}
