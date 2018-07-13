// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// #include "targetver.h"
// 
// #include <stdio.h>
// #include <tchar.h>

#include <afx.h>
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <atlbase.h>  


// TODO: reference additional headers your program requires here

#define CHECK_MOMREY_TTTEST 0

#include "PlatformDefine.h"
#include "XCross.h"
#include "XStream.h"
#pragma comment( lib, "C:\\output32\\XStream.lib" )


#include <openssl/des.h>
#ifdef _WIN64
#pragma comment( lib, "libeay32.lib" )
#pragma comment( lib, "ssleay32.lib" )
#else
#ifdef _DEBUG
#pragma comment( lib, "libeay32MDd.lib" )
#pragma comment( lib, "ssleay32MDd.lib" )
#else
#pragma comment( lib, "libeay32MD.lib" )
#pragma comment( lib, "ssleay32MD.lib" )
#endif
#endif



struct MyStruct4Process
{
	CROSS_DWORD64 dw;//最后通讯时间戳
	BOOL bOpenProc;//是否已经打开相关的XTransmisProc进程
	char strParam[256];//进程参数
	PROCESS_INFORMATION pi;
};

//int :XTransmisProc的唯一标识,用于验证其存在，并且双方通讯时使用这个值作为sessionid
extern map<int, MyStruct4Process *> g_mapProcess;
extern CrossCriticalSection g_csProcessSession;



//256
struct ST_DEVICE_BASE
{
	//
	int dev_id;
	char dev_name[256 - 12];

	int time_start;
	int time_end;
};



struct ST_LIVE_STRUCT
{
	int nPlatformID; //>0
	ST_DEVICE_BASE dev[6];//dev_id==0表示该时间段无效
};

#define MAX_LIVE_STRUCT 8

struct ST_LIVE_STRUCT_ALL
{
	ST_LIVE_STRUCT all[MAX_LIVE_STRUCT];
};