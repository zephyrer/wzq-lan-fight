
// WZQ.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

class CWZQApp : public CWinApp
{
public:
	CWZQApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CWZQApp theApp;



typedef struct dot{
	UINT used	:1;
	UINT uid	:7;
	UINT lt		:3;
	UINT t		:3;
	UINT rt		:3;
	UINT l		:3;
	UINT r		:3;
	UINT lb		:3;
	UINT b		:3;
	UINT rb		:3;
	dot(){
		ZeroMemory(this,sizeof(Dot));
	}
}Dot;

typedef struct piece{
	BYTE uid;
	BYTE x;
	BYTE y;
}Piece;

enum {
	SEARCH_HOST,
	HOST_RESPONSE,
	ALLOT_ID,
	JOIN_GAME,
	QUIT_GAME,
	MOVE
};

typedef struct MsgPack{
	BYTE type;
	union data{
		struct ServerResponse{
			CHAR server[19];
			BYTE uesrNum;
		}SR;
		CHAR gamer[20];
		BYTE uid;
		struct PlaceDot{
			BYTE uid;
			BYTE x;
			BYTE y;
		}PD;
	}data;
	MsgPack(){
		ZeroMemory(this,sizeof(MsgPack));
	}
}MsgPack;

