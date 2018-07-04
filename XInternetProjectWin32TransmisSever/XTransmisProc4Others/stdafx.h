// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// #include "targetver.h"
// 
// #include <stdio.h>
// #include <tchar.h>
// 
// #include <atlbase.h>
// #include <atlstr.h>
// 
// // Windows 头文件:
// #include <windows.h>


//#include "vld.h"


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




#include "PlatformDefine.h"
#include "XCross.h"
#include "DataTypeTransProc.h"
#include "..\..\Direct-connected\_Include\DevInterface.h"
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

#include "XStream.h"
#pragma comment( lib, "C:\\output32\\XStream.lib" )

//openssl
#include <openssl/des.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

// #include <openssl/des.h>
// #ifdef _DEBUG
// #pragma comment( lib, "libeay32MDd.lib" )
// #pragma comment( lib, "ssleay32MDd.lib" )
// #else
// #pragma comment( lib, "libeay32MD.lib" )
// #pragma comment( lib, "ssleay32MD.lib" )
// #endif
// 
// #include <zlib.h>
// #ifdef _DEBUG
// #pragma comment( lib, "H:\\NewPlatform\\_ThirdParty\\zlib\\windows32\\lib\\ZlibDllDebug\\zlibwapi.lib" )
// #else
// #pragma comment( lib, "ZlibStatReleaseWithoutAsm/zlibstat.lib" )
// #endif

#include "rtmp.h"
#include "rtmp_sys.h"
#ifdef _DEBUG
#pragma comment( lib, "librtmpd.lib" )
#else
#pragma comment( lib, "librtmp.lib" )
#endif






class CTransmisProcFrame
{
public:
	CTransmisProcFrame(void)
	{
		m_pBuffer = new char[2 * 1024 * 1024];
		pHead = (TRANSMIN_PROC_HEAD__T *)m_pBuffer;
		memset(pHead, 0, sizeof(TRANSMIN_PROC_HEAD__T));
		m_nLen = 0;
	};
	~CTransmisProcFrame(void)
	{
		if (m_pBuffer)
		{
			delete[] m_pBuffer;
		}
	};
public:
	char *m_pBuffer;
	int m_nLen;
	TRANSMIN_PROC_HEAD__T *pHead;
};

class CTransmisProcDataReciver
{
public:
	virtual void AddFrame(boost::shared_ptr<CTransmisProcFrame> &p) = 0;
};

extern DWORD64 g_dwLastTick;//与主进程最后的通讯时间
extern void * g_hDevSession;


#define USE_SRA_JIWEI 0