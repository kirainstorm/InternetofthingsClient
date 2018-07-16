// XWin32RtspTransmisProcess.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <atlconv.h>
#include <dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")
//
#include "XDevice.h"
#include "RTSPClient.h"
#include "TransmisClient.hpp"
#include "XStream.hpp"


#define TEST_HAS_WINDOWS 0

//����Dump�ļ�  
void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException);
LONG WINAPI ApplicationCrashHandler(EXCEPTION_POINTERS *pException);
//----------------------------------
#if TEST_HAS_WINDOWS
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"" )//���ؿ���̨����
#endif
//----------------------------------
XDeviceMessageCallback *pBack = nullptr;
XDeviceInterface * pDevice = nullptr;


class MessageCallback :public XDeviceMessageCallback
{
public:
	MessageCallback(){};
	~MessageCallback(){};

	virtual void OnDeviceMessageCallback(const char *jsonbuffer, int len)
	{
	};
private:

};
void RtspAVCallBackFunction(unsigned char *data, int len, int type, int iframe, timeval tv)
{
	if (1 == type)
	{
		//Log("RtspAVCallBackFunction  ....... len=%d\n",len);

		//
		int64_t s = (int64_t)tv.tv_sec * 1000;
		int64_t c = tv.tv_usec / 1000;
		int64_t time = s + c;
		//
		if (pDevice)
		{
			pDevice->XSendStream(iframe ? XMEDIA_FREAM_TYPE_H264_IFRAME : XMEDIA_FREAM_TYPE_H264_PFRAME, (const char *)data, len, time,
				8, 640, 480);
		}
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	char _rtspurl[1024] = { 0 };
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
	SetUnhandledExceptionFilter(ApplicationCrashHandler);//���ñ���������
	//
	XNetStartStreamManager();
	//

#if TEST_HAS_WINDOWS
	CString strArgv = "rtsp://192.168.1.228:8554/stream1";	
#else
	if (argc < 2)
	{
		OutputDebugStringA("����Ĳ���");
		return 0;
	}
	CString strArgv = argv[1];
	MessageBox(NULL,strArgv,L"",0);
#endif

	WideCharToMultiByte(CP_UTF8, NULL, strArgv, -1, _rtspurl, sizeof(_rtspurl), NULL, FALSE);
	//
#if TEST_HAS_WINDOWS

#else
	string s(_rtspurl);
	CTransmisClient::Instance().Start(s);
#endif
	//
	pBack = new MessageCallback();
	pDevice = CreateDeviceInstance();
	pDevice->XConnect(pBack, "47.96.249.142", "3001");
	//
	RTSPClientInit(RtspAVCallBackFunction);
	int nRet = RTSPClientStart(_rtspurl);
	if (0 != nRet)
	{
		OutputDebugStringA("OPEN ERROR:");
		OutputDebugStringA(_rtspurl);
		OutputDebugStringA("\n");
		return 0;
	}
	//



	while (1)
	{
#if TEST_HAS_WINDOWS

#else
		if (CTransmisClient::Instance().GetError())
		{
			break;
		}
#endif
		Sleep(1000);
	}

	return 0;
}




//����Dump�ļ�  
void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
{
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
	CloseHandle(hDumpFile);
}
LONG WINAPI ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
	/*
	EXCEPTION_EXECUTE_HANDLER=1��ʾ���Ѿ��������쳣,�������ŵؽ�����
	EXCEPTION_CONTINUE_SEARCH=0��ʾ�Ҳ�����,����������,����windows����Ĭ�ϵĴ��������ʾһ�������,������
	EXCEPTION_CONTINUE_EXECUTION=-1��ʾ�����Ѿ����޸�,����쳣����������ִ��
	*/
	SYSTEMTIME stLocalTime;
	GetLocalTime(&stLocalTime);
	WCHAR wszInfo[256] = { L"C:\\XWin32RtspTransmisProcess.dmp" };
	CreateDumpFile(wszInfo, pException);
	return EXCEPTION_EXECUTE_HANDLER;
}