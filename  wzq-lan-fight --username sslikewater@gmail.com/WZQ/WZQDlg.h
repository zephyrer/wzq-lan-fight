
// WZQDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <vector>
#include "MyEdit.h"
#include "MyButton.h"
#include "MyListBox.h"

// CWZQDlg �Ի���
class CWZQDlg : public CDialogEx
{
// ����
public:
	CWZQDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WZQ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

	void WZQInit(void);//��ʼ������
private:
	//CSocket serverSock;//������������TCP��Ϸͨ�����ӵ�socket���ͻ��ˣ����ӵ�����������������
	//CSocket clientSock;//��������������ս�ߵ�socket�����������ݣ��ͻ��ˣ����ӵ�����������������
	CSocket RecvSock;//��������
	CSocket SendSock;//��������

	CString hostAddrSock;//�������ķ�������ַ

	static CONST UINT m_ServerPort = 61325;//��Ϸͨ�ŷ�����TCP�˿�
	static CONST UINT m_ServerPortRecv = 61326;//��Ϸͨ�ŷ�����TCP�˿�
	static CONST UINT m_SearchPort = 6666;//������Ϸ�õ�UDP�˿�
	
	FLOAT blockLength;
	bool m_gameStart;//��־��Ϸ�Ƿ��ѿ�ʼ
	bool m_appruning;//��־������������
	bool m_receving;
	HANDLE m_paintMutex;
	HANDLE m_pieceMutex;
	POINT m_mouse;//����ƶ�ʱ�����������
	
	HCURSOR nullCur;
	HCURSOR arrowCur;
	FLOAT m_clientLength;//�������̵������С����������������
	Gdiplus::Image *bkImage;//����ͼ��ʹ��λͼ���Լ��ٻ�������ʱ�ļ�����
	MyListBox m_gameList;//����������Ϸ�б�
	MyButton m_BangButton;//������Ϸ��������ť
	MyButton m_challenge;//��ս��ť��ѡ����Ϸ�б���һ�����

	static UINT ReceiveThread(LPVOID param);//������Ϸ���ݵ��̺߳���
	static UINT SendThread(LPVOID param);//������Ϸ���ݵ��̺߳���
	static UINT PaintThread(LPVOID param);//�������̵��̺߳���
	static UINT SearchThread(LPVOID param);//ʹ��UDP�㲥����������������Ϸ���������̺߳���
	static UINT HostThread(LPVOID param);//����UDP socket�����������㲥����Ӧ������̺߳���
	void GetSocketError(void);//�������socket���󣬸���GetLastError()�жϴ�����Ϣ��������Ӧ��ʾ

	std::vector<Piece> pieces;//�洢����λ����Ϣ��vector��ֻ�洢���ӵ�id�����꣬������������
	static CONST BYTE GRID_X = 16;
	static CONST BYTE GRID_Y = 16;
	Dot grid[GRID_X][GRID_Y];//��ʾ��������
	UINT m_uid;//��ǰ�û���ID��
	MyEdit m_name;//��ǰ�û����ǳƣ���Ϊ����������Ϊ��������
public:
	afx_msg void OnBnClickedSearchgame();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
private:
	MyButton m_searchButton;
	UINT Place(BYTE x, BYTE y);//���ñ������ӣ�����������Ϣ
	UINT PlaceRecv(BYTE x, BYTE y, BYTE UID);//���ý��ܵ��Է������ӣ�����������Ϣ
	// �������̽ڵ��ֵ
	UINT SetDotValue(BYTE x, BYTE y,BYTE UID, BYTE dir);
public:
	// ��λ��ͬ��������̸��
	Dot* DirLocDot(BYTE x, BYTE y, BYTE dir);
	// ��ȡ�ڵ㲻ͬ�����ֵ
	BYTE DirGetDotValue(Dot dot, BYTE dir);
	// ���ýڵ㲻ͬ�����ֵ
	void DirSetDotValue(BYTE x, BYTE y, BYTE dir, BYTE value);
	// ���ݷ��������µ�x,y����ֵ
	void DirModify(BYTE ori_x,BYTE ori_y,BYTE dir,BYTE& to_x, BYTE& to_y);
	// ���������Ϣ
	void CleanGrid(void);
};
