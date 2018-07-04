
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars



#include "Resource.h"
//////////////////////////////////////////////////////////////////////////
#include <atlbase.h>
#include <atlstr.h>
#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;

//
#define MAX_WINDOWS_NUM 16
//
// #define WM_VIDEO_WINDOW_CLOSE_STREAM	(WM_USER+101)
// #define WM_VIDEO_WINDOW_RECORD			(WM_USER+102)
// #define WM_VIDEO_WINDOW_CAPPIC			(WM_USER+103)
// #define WM_VIDEO_WINDOW_SOUND			(WM_USER+104)
// #define WM_VIDEO_WINDOW_TALK			(WM_USER+105)
// //#define WM_VIDEO_WINDOW_VIDEOQUALITY		(WM_USER+106)
// //
// #define WM_HIS_VIDEO_WINDOW_PAUSE			(WM_USER+206)
// #define WM_HIS_VIDEO_WINDOW_STOP			(WM_USER+207)
// #define WM_HIS_VIDEO_WINDOW_POS_CALLBACK	(WM_USER+208)
// #define WM_HIS_VIDEO_WINDOW_SEEK			(WM_USER+209)


#include "PlatformDefine.h"
#include "../_MediaInterface/MediaInterface.h"
#pragma comment( lib, "C:\\output32\\MediaInterface.lib" )

#include "BmpButton.h"
#include "BCMenu.h"
#include "BtnST.h"
#include "BmpButton.h"

struct ST_IOT_DEVICE_INFO
{
	char dev_name[64];			//设备名称
	char dev_uuid[64];		//
	char dev_cfg[2048];
	int dev_online;				//是否在线
	int reserve[3];
};
struct stDevice
{
	HTREEITEM hItem;
	BOOL bOpen;//是否已经打开视频了
	ST_CLASSIC_DEVICE_INFO_BASE classicinfo;
	ST_IOT_DEVICE_INFO iotinfo;
};
//#include "dbType.h"
#include "SysFile.h"
#include <afxdhtml.h>
#include "DeviceManager.h"
#include "MyMessageBox.h"
//#include "XPlayer.hpp"


void OnPaintBmpBackground(CWnd * pWnd, CPaintDC &dc ,int height = -1);
//////////////////////////////////////////////////////////////////////////
// 从文件获取界面信息
void Set_String_To_Wnd_From_Language_Ini_File(UINT sID, CString sKey, CDialogEx * user);

// 从文件获取语言信息
void Get_String_From_Language_Ini_File(CString sKey, CString& cStr);
//////////////////////////////////////////////////////////////////////////

struct stConfig
{
	//左边栏背景色
	long bgr;
	long bgg;
	long bgb;


	//视频窗口标题背景色
	long bgvr;
	long bgvg;
	long bgvb;

	//视频窗口背景色
	long bgvtr;
	long bgvtg;
	long bgvtb;

	//视频窗口 线条 颜色
	long bglr;
	long bglg;
	long bglb;
};
//////////////////////////////////////////////////////////////////////////

typedef struct __Directory_Info_t
{
	CTime   createTime;
	CString strPath;
	bool operator < (const __Directory_Info_t & val) const
	{
		if (createTime == val.createTime)
		{
			return strPath.Compare(val.strPath) < 0;
		}
		return createTime < val.createTime;
	}

} Directory_Info_t;

//////////////////////////////////////////////////////////////////////////
//Converting a WChar string to a Ansi string
// inline std::string WChar2Ansi(LPCWSTR pwszSrc)
// {
// 	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, nullptr, 0, nullptr, nullptr);
// 	if (nLen <= 0) return std::string("");
// 	char* pszDst = new char[nLen];
// 	if (nullptr == pszDst) return std::string("");
// 	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, nullptr, nullptr);
// 	pszDst[nLen - 1] = 0;
// 	std::string strTemp(pszDst);
// 	delete[] pszDst;
// 	return strTemp;
// }
// inline string ws2s(wstring& inputws)
// {
// 	return WChar2Ansi(inputws.c_str());
// }
// 
// inline string wcs2s(CString& inputwcs){
// 	std::wstring str = inputwcs.GetString();
// 	return ws2s(str);
// }

inline string __cstringToString(CString& inputwcs)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, inputwcs, -1, nullptr, 0, nullptr, nullptr);
	if (nLen <= 0) 
		return std::string("");
	char* pszDst = new char[nLen];
	if (nullptr == pszDst) return std::string("");
	WideCharToMultiByte(CP_ACP, 0, inputwcs, -1, pszDst, nLen, nullptr, nullptr);
	pszDst[nLen - 1] = 0;
	std::string strTemp(pszDst);
	delete[] pszDst;
	return strTemp;
}


//Converting a Ansi string to WChar string
// inline std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen)
// {
// 	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
// 	if (nSize <= 0) return nullptr;
// 	WCHAR *pwszDst = new WCHAR[nSize + 1];
// 	if (nullptr == pwszDst) return nullptr;
// 	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, pwszDst, nSize);
// 	pwszDst[nSize] = 0;
// 	if (pwszDst[0] == 0xFEFF) // skip Oxfeff
// 		for (int i = 0; i < nSize; i++)
// 			pwszDst[i] = pwszDst[i + 1];
// 	wstring wcharString(pwszDst);
// 	delete pwszDst;
// 	return wcharString;
// }
// inline std::wstring s2ws(const string& s)
// {
// 	return Ansi2WChar(s.c_str(), (int)s.size());
// }
inline CString __stringToCString(const string& s)
{
	CString strReturn = L"";


	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)s.c_str(), (int)s.size(), 0, 0);
	if (nSize <= 0) 
		return strReturn;

	//
	WCHAR *pwszDst = new WCHAR[nSize + 1];
	//
	if (nullptr == pwszDst) return nullptr;
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)s.c_str(), (int)s.size(), pwszDst, nSize);
	pwszDst[nSize] = 0;
	//

// 	if (pwszDst[0] == 0xFEFF) // skip Oxfeff
// 		for (int i = 0; i < nSize; i++)
// 			pwszDst[i] = pwszDst[i + 1];
// 	wstring wcharString(pwszDst);

	strReturn = pwszDst;
	delete pwszDst;
	return strReturn;


// 	CString str(Ansi2WChar(s.c_str(), (int)s.size()).c_str());
// 	return str;
}



inline tm __systime_to_timet_to_tm(ST_XMEDIA_TIME& st1)
{

	SYSTEMTIME st;
	st.wYear = st1.wYear;
	st.wMonth = st1.wMonth;
	st.wDayOfWeek = st1.wDayOfWeek;
	st.wDay = st1.wDay;
	st.wHour = st1.wHour;
	st.wMinute = st1.wMinute;
	st.wSecond = st1.wSecond;
	st.wMilliseconds = st1.wMilliseconds;


	struct tm gm = { st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth - 1, st.wYear - 1900, st.wDayOfWeek, 0, 0 };

	return gm;
}
inline time_t __systime_to_timet_ex(ST_XMEDIA_TIME& st1)
{
	SYSTEMTIME st;
	st.wYear = st1.wYear;
	st.wMonth = st1.wMonth;
	st.wDayOfWeek = st1.wDayOfWeek;
	st.wDay = st1.wDay;
	st.wHour = st1.wHour;
	st.wMinute = st1.wMinute;
	st.wSecond = st1.wSecond;
	st.wMilliseconds = st1.wMilliseconds;

	struct tm gm = { st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth - 1, st.wYear - 1900, st.wDayOfWeek, 0, 0 };
	return mktime(&gm);
}

void  ButtonLoadBitmap(CBmpButton &btn, LPCTSTR f1, LPCTSTR f2, LPCTSTR f3, LPCTSTR f4, int ctrlid = -1, CString str = L"");


// #include "syscmdinfo.h"
// #include "BroadcastSearchStruct.h"
// struct st_device_info
// {
// 	XSEARCH_MESSAGE_T info;
// 	char ip[64];
// 	long tick;
// 
// 	_allparam_v2 param;
// 
// 
// 	imageparam_t img_param;
// 	encodeparam_t enc_param;
// };


//openssl
#include <openssl/des.h>
// #ifdef _DEBUG
// #pragma comment( lib, "libeay32MDd.lib" )
// #pragma comment( lib, "ssleay32MDd.lib" )
// #else
// #pragma comment( lib, "libeay32MD.lib" )
// #pragma comment( lib, "ssleay32MD.lib" )
// #endif
#include "XDes.h"


//void ApplicationSignalingCtrl(const char * szDeviceID, const char * pszInBuffer, int nInLen, char *pszOutBuffer, int &nOutLen);


#pragma comment(lib, "lib_json.lib")


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


