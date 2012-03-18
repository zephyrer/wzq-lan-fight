
// WZQDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <vector>
#include "MyEdit.h"
#include "MyButton.h"
#include "MyListBox.h"

// CWZQDlg 对话框
class CWZQDlg : public CDialogEx
{
// 构造
public:
	CWZQDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WZQ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedServer();
	afx_msg void OnBnClickedChallenge();
	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	void WZQInit(void);//初始化函数
private:
	//CSocket serverSock;//服务器：建立TCP游戏通信连接的socket，客户端：连接到服务器并发送数据
	//CSocket clientSock;//服务器：接受挑战者的socket，并发送数据，客户端：连接到服务器并接收数据
	CSocket RecvSock;//接收数据
	CSocket SendSock;//发送数据

	CString hostAddrSock;//搜索到的服务器地址

	static CONST UINT m_ServerPort = 61325;//游戏通信服务器TCP端口
	static CONST UINT m_ServerPortRecv = 61326;//游戏通信服务器TCP端口
	static CONST UINT m_SearchPort = 6666;//搜索游戏用的UDP端口
	
	FLOAT blockLength;
	bool m_gameStart;//标志游戏是否已开始
	bool m_appruning;//标志程序正在运行
	bool m_receving;
	HANDLE m_paintMutex;
	HANDLE m_pieceMutex;
	POINT m_mouse;//鼠标移动时保存鼠标坐标
	
	HCURSOR nullCur;
	HCURSOR arrowCur;
	FLOAT m_clientLength;//绘制棋盘的区域大小，采用正方形区域
	Gdiplus::Image *bkImage;//棋盘图像，使用位图可以减少绘制棋盘时的计算量
	MyListBox m_gameList;//搜索到的游戏列表
	MyButton m_BangButton;//建立游戏服务器按钮
	MyButton m_challenge;//挑战按钮，选择游戏列表中一项后点击

	static UINT ReceiveThread(LPVOID param);//接收游戏数据的线程函数
	static UINT SendThread(LPVOID param);//发送游戏数据的线程函数
	static UINT PaintThread(LPVOID param);//绘制棋盘的线程函数
	static UINT SearchThread(LPVOID param);//使用UDP广播来搜索局域网中游戏服务器的线程函数
	static UINT HostThread(LPVOID param);//建立UDP socket来接受搜索广播并回应请求的线程函数
	void GetSocketError(void);//如果发生socket错误，根据GetLastError()判断错误信息并给予相应提示

	std::vector<Piece> pieces;//存储棋子位置信息的vector，只存储棋子的id和坐标，用来绘制棋盘
	static CONST BYTE GRID_X = 16;
	static CONST BYTE GRID_Y = 16;
	Dot grid[GRID_X][GRID_Y];//表示棋盘数据
	UINT m_uid;//当前用户的ID号
	MyEdit m_name;//当前用户的昵称，若为服务器则定义为服务器名
public:
	afx_msg void OnBnClickedSearchgame();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
private:
	MyButton m_searchButton;
	UINT Place(BYTE x, BYTE y);//放置本地棋子，更新棋盘信息
	UINT PlaceRecv(BYTE x, BYTE y, BYTE UID);//放置接受到对方的棋子，更新棋盘信息
	// 更新棋盘节点的值
	UINT SetDotValue(BYTE x, BYTE y,BYTE UID, BYTE dir);
public:
	// 定位不同方向的棋盘格点
	Dot* DirLocDot(BYTE x, BYTE y, BYTE dir);
	// 获取节点不同方向的值
	BYTE DirGetDotValue(Dot dot, BYTE dir);
	// 设置节点不同方向的值
	void DirSetDotValue(BYTE x, BYTE y, BYTE dir, BYTE value);
	// 根据方向设置新的x,y坐标值
	void DirModify(BYTE ori_x,BYTE ori_y,BYTE dir,BYTE& to_x, BYTE& to_y);
	// 清楚棋盘信息
	void CleanGrid(void);
};
