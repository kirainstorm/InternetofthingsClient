#ifndef XMsgDefForTransmisProcess_sjkehwryuiksdcxmvgsx832igv
#define XMsgDefForTransmisProcess_sjkehwryuiksdcxmvgsx832igv

#include "PlatformDefine.h"
#include "Pool.hpp"




struct TRANSMIN_PROC_DATA
{
	int cmd;
	//int result;	//0
	//int r;
	int k;//必须是0xada2
	char uuid[32];
	//char reserved[16];
};


struct ST_PROCESS_INFO
{
	int online;
	//
	unsigned int openCount;//请求次数，0关闭，1开启
	CROSS_DWORD64 openCountZeroTick;//
						   //
	char devId[64];//设备ID
				   //
	char rtmpUuid[64];//随机的推流ID，防止被其他用户使用
	//
	CROSS_DWORD64 dwTick;//最后通讯时间戳

	char strParam[256];
	PROCESS_INFORMATION pi;//进程  //使用pi的hProcess判断是否已经打开相关的XTransmisProc进程
};
inline ST_PROCESS_INFO * singleton_pool_malloc_transmis_process_info()
{
	return new ST_PROCESS_INFO;// (ST_PROCESS_INFO *)CExtreSmallBufferPool::Instance().malloc();
};
inline void singleton_pool_free_transmis_process_info(ST_PROCESS_INFO *pbuffer)
{
	delete pbuffer;
	//CExtreSmallBufferPool::Instance().free((char *)pbuffer);
};


inline int OpenTransmisProcProcessRtmp(ST_PROCESS_INFO &t, CString strexe)
{
	//获取执行路径
	CString strPath;
	::GetModuleFileName(NULL, strPath.GetBufferSetLength(MAX_PATH), MAX_PATH);
	strPath.ReleaseBuffer();
	int p = strPath.ReverseFind(_T('\\'));
	strPath = strPath.Left(p + 1);
	//启动设备访问进程
#if 0
	//ACL
	SECURITY_DESCRIPTOR 	sd;
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, (PACL)NULL, FALSE);

	SECURITY_ATTRIBUTES	sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = FALSE;
	sa.lpSecurityDescriptor = &sd;
#endif

	WCHAR wszInfo[256] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, t.strParam, (int)strlen(t.strParam) + 1, wszInfo, (int)(sizeof(wszInfo) / sizeof(wszInfo[0])));
	CString strCmd;

	strCmd.Format(_T("\"%s%s\" %s"), strPath, strexe, wszInfo);
// #ifdef _WIN64
// 	//strCmd.Format(_T("\"%s..\\XRtmpTransmisProc.exe\" %s"), strPath, wszInfo);
// 	strCmd.Format(_T("\"%s..\\%s\" %s"), strPath, strexe, wszInfo);
// #else
// 	//strCmd.Format(_T("\"%sXRtmpTransmisProc.exe\" %s"), strPath, wszInfo);
// 	strCmd.Format(_T("\"%%s\" %s"), strPath, strexe, wszInfo);
// #endif
	STARTUPINFO si;
	ZeroMemory(&t.pi, sizeof(t.pi));
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
#if 0
	if (!CreateProcess(NULL, (LPWSTR)(LPCTSTR)strCmd, &sa, &sa, FALSE, 0, NULL, NULL, &si, &pi))
	{
		return -1;
	}
#endif
	if (!CreateProcess(NULL, (LPWSTR)(LPCTSTR)strCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &t.pi))
	{

		printf("CreateProcess error");
		return -1;
	}
	//CloseHandle(pi.hThread);
	//CloseHandle(pi.hProcess);

	return 0;
};
inline int CloseTransmisProcProcessRtmp(ST_PROCESS_INFO &t)
{
	if (0 != t.pi.hProcess)
	{
		//CROSS_TRACE("CloseTransmisProcProcess %s   [%d]\n", t.strParam, t.pi.hThread);
		BOOL b;
		b = TerminateThread(t.pi.hThread, 0);
		b = TerminateProcess(t.pi.hProcess, 0);
		b = CloseHandle(t.pi.hThread);
		b = CloseHandle(t.pi.hProcess);
	}
	ZeroMemory(&t.pi, sizeof(t.pi));
	return 0;
};
inline void _newUuid(char * _uuid)
{
	memset(_uuid, 0, 32);
	GUID guid;
	CoInitialize(NULL);
	if (S_OK == CoCreateGuid(&guid))
	{
		_snprintf(_uuid, 32,
			"%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x",
			guid.Data1,
			guid.Data2,
			guid.Data3,
			guid.Data4[0], guid.Data4[1],
			guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5],
			guid.Data4[6], guid.Data4[7]);
	}
	CoUninitialize();
}



#endif
