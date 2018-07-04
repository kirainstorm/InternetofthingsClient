// XTransmisProc.cpp : Defines the entry point for the console application.
//

//----------------------------------------------------------------------
//
//
//	请注意：该进程只链接某台设备的某<1>通道的某<1>种（主码流/子码流）码流
//
//
//----------------------------------------------------------------------

#include "stdafx.h"
#include <atlconv.h>
#include <dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")
#include "TransmisService.h"
#include "MediaToMediaInterface.h"
#include "MediaToRtmpHlsInterface.h"
#include "TransmisStreamToPlatform.h"




//创建Dump文件  
void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException);
LONG WINAPI ApplicationCrashHandler(EXCEPTION_POINTERS *pException);
//----------------------------------
//----------------------------------
#define TEST_ONLY_EE 0


#if (1==TEST_ONLY_EE)?0:1
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"" )//隐藏控制台窗口
#endif

//----------------------------------
//----------------------------------
int _tmain(int argc, _TCHAR* argv[])
{

	//Sleep(10000);

	SetUnhandledExceptionFilter(ApplicationCrashHandler);//设置崩溃处理函数

	int nSessionID = 0;
	XTransmisDeviceInterface * pInterface = NULL;
	XNetStartStreamManager(4);

	//argv传递的字符串数据，结构定义如下：
	//与主程序心跳的Session#
	//设备类型 emTRANSMIN_PROC_DEVICE_TYPE#
	//设备IP#
	//设备端口#
	//设备ID#
	//用户名#
	//密码#
	//要推送到的服务器类型emTRANSMIN_PROC_SERVER_TYPE#
	//要推送到的服务器ip#
	//要推送到的服务器端口#
	//要推送到的服务器上的ID（自己的服务器的id//或者hls名称）
#if TEST_ONLY_EE
	//CString strArgv = L"33#0#192.168.7.111#8000#2#13800138000#13800138000#0#192.168.7.111#8002#62019";
	//CString strArgv = L"33#5####113#888888#0#192.168.7.111#8002#62019";
	//CString strArgv = L"#5####113#888888#0#120.76.217.18#8002#888888";
	//CString strArgv = L"33#1#192.168.7.111#8000#2#13800138000#13800138000#0#192.168.7.111#8002#62019";
	//CString strArgv = L"33#1#127.0.0.1#8000#12#13800138000#13800138000#0#10.30.0.180#1935#6002";



	//CString strArgv = L"33#0#127.0.0.1#8000#1#13800138000#13800138000#0#120.76.217.18#8002#6015";
	CString strArgv = L"33#1#127.0.0.1#8000#1#13800138000#13800138000#0#119.23.152.32#1935#16026";
#else
	if (argc < 2)
	{
		printf("错误的参数");
		return 0;
	}
	CString strArgv = argv[1];
	//AfxMessageBox(strArgv);
	//return 0;
#endif

	CString strTmp = strArgv;
	CStringArray arr;
	int nIndex = 0; //
	while (1)
	{
		nIndex = strTmp.Find(_T("#"));
		if (nIndex >= 0)
		{
			arr.Add(strTmp.Left(nIndex));
			strTmp = strTmp.Right(strTmp.GetLength() - nIndex - 1);
		}
		else break;
	}
	arr.Add(strTmp);


 
 	//----------------------------------------------------------------------------------------------------------------------------------------------
  	int emDeviceType = _wtoi(arr[1]);
	if (TRANSMIN_PROC_DEVICE_ME_MEDIA_TO_RTMP_HLS != emDeviceType)
	{
		char _to_svr_ip[256] = { 0 };
		WideCharToMultiByte(CP_UTF8, NULL, arr[8], -1, _to_svr_ip, sizeof(_to_svr_ip), NULL, FALSE);
		CTransmisStreamToPlatform::Instance().SetPlatformInfo(_to_svr_ip, _wtoi(arr[9]), _wtoi(arr[10]));//这是要传送给自己的平台的
	}
	//----------------------------------------------------------------------------------------------------------------------------------------------
	switch (emDeviceType)
	{
	case TRANSMIN_PROC_DEVICE_ME_MEDIA_TO_MEDIA:
	{
		pInterface = new CMediaToMediaInterface();

		char _svr_ip[256] = { 0 };
		WideCharToMultiByte(CP_UTF8, NULL, arr[2], -1, _svr_ip, sizeof(_svr_ip), NULL, FALSE);
		char _svr_name[256] = { 0 };
		WideCharToMultiByte(CP_UTF8, NULL, arr[5], -1, _svr_name, sizeof(_svr_name), NULL, FALSE);
		char _svr_pwd[256] = { 0 };
		WideCharToMultiByte(CP_UTF8, NULL, arr[6], -1, _svr_pwd, sizeof(_svr_pwd), NULL, FALSE);

		pInterface->OpenTransmisDevice(_svr_ip, _svr_name, _svr_pwd, NULL,_wtoi(arr[3]), _wtoi(arr[4]));
	}
		break;
	case TRANSMIN_PROC_DEVICE_ME_MEDIA_TO_RTMP_HLS:
	{

		pInterface = new CMediaToRtmpHlsInterface();

		char _svr_ip[256] = { 0 };
		WideCharToMultiByte(CP_UTF8, NULL, arr[2], -1, _svr_ip, sizeof(_svr_ip), NULL, FALSE);
		char _svr_name[256] = { 0 };
		WideCharToMultiByte(CP_UTF8, NULL, arr[5], -1, _svr_name, sizeof(_svr_name), NULL, FALSE);
		char _svr_pwd[256] = { 0 };
		WideCharToMultiByte(CP_UTF8, NULL, arr[6], -1, _svr_pwd, sizeof(_svr_pwd), NULL, FALSE);


		char _to_svr_ip[256] = { 0 };
		WideCharToMultiByte(CP_UTF8, NULL, arr[8], -1, _to_svr_ip, sizeof(_to_svr_ip), NULL, FALSE);

		pInterface->OpenTransmisDevice(_svr_ip, _svr_name, _svr_pwd, _to_svr_ip, _wtoi(arr[3]), _wtoi(arr[4]), _wtoi(arr[9]), _wtoi(arr[10]));

	}
	break;
	default:
		break;
	}
	//----------------------------------------------------------------------------------------------------------------------------------------------
	if (NULL == pInterface)
	{
		//不支持的类型
		XNetStopStreamManager();
		return 0;
	}
	//----------------------------------------------------------------------------------------------------------------------------------------------


#if TEST_ONLY_EE
	while ((FALSE == pInterface->GetError()))
	{
		Sleep(200);
	}
// 
// 	BOOL a = pInterface->GetError();
//	Sleep(30*1000);

#else
	//--------------------------------------------------------------
	//启动与主服务器的连接
	if (0 == CTransmisService::Instance().Start((_wtoi(arr[0]))))
	{
		while ( (FALSE == CTransmisService::Instance().GetError()) && (FALSE == pInterface->GetError()) )
		{
			Sleep(200);
		}
	}

	return 0;

	//--------------------------------------------------------------
	OutputDebugStringA("XTransmisProc Exit");
	CTransmisService::Instance().Stop();
	//--------------------------------------------------------------
#endif

	//getchar();

	//AfxMessageBox(L"111");

	pInterface->CloseTransmisDevice();

	//
	XNetStopStreamManager();


	//OutputDebugStringA("XTransmisProc Exit");
	return 0;
}



//创建Dump文件  
void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
{
	// 创建Dump文件  
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	// Dump信息  
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;
	// 写入Dump文件内容    
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
	CloseHandle(hDumpFile);
}
LONG WINAPI ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
	/*
	EXCEPTION_EXECUTE_HANDLER=1表示我已经处理了异常,可以优雅地结束了
	EXCEPTION_CONTINUE_SEARCH=0表示我不处理,其他人来吧,于是windows调用默认的处理程序显示一个错误框,并结束
	EXCEPTION_CONTINUE_EXECUTION=-1表示错误已经被修复,请从异常发生处继续执行
	*/
	SYSTEMTIME stLocalTime;
	GetLocalTime(&stLocalTime);

// 	char szTmp[64] = { 0 };
// 	sprintf_s(szTmp, "C:\\XTransmisProc%d.dmp", GetCurrentProcessId());
// 	WCHAR wszInfo[256] = { 0 };
// 	MultiByteToWideChar(CP_UTF8, 0, szTmp, (int)strlen(szTmp) + 1, wszInfo, (int)(sizeof(wszInfo) / sizeof(wszInfo[0])));


	WCHAR wszInfo[256] = { L"C:\\XTransmisProc%d.dmp"};

	CreateDumpFile(wszInfo, pException);
	return EXCEPTION_EXECUTE_HANDLER;
}