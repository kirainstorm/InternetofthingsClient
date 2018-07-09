// XTransmisSever.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <map>
using namespace std;
#include "ActiveConnService.h"
#include "XRegisterService.h"
#include "cpp_authorized/LicenseLocal.h"
#include "RecycleService.h"


//////////////////////////////////////////////////////////////////////////
#include <Psapi.h>
#include <Winsvc.h>
#include <dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")
//HANDLE hEvent = NULL;
BOOL UpdateConfig();


//����Dump�ļ�  
void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
{
	// ����Dump�ļ�  
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Dump��Ϣ  
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	// д��Dump�ļ�����  
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

	CloseHandle(hDumpFile);
}
LONG WINAPI ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
	CreateDumpFile(_T("C:\\XTransmisSever.dmp"), pException);
// 	if (hEvent)
// 	{
// 		CloseHandle(hEvent);
// 		hEvent = NULL;
// 	}
	return EXCEPTION_EXECUTE_HANDLER;
}






//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"" )

// void CreateMyEvent()
// {
// 	// set SECURITY_DESCRIPTOR
// 	SECURITY_DESCRIPTOR secutityDese;
// 	::InitializeSecurityDescriptor(&secutityDese, SECURITY_DESCRIPTOR_REVISION);
// 	::SetSecurityDescriptorDacl(&secutityDese, TRUE, NULL, FALSE);
// 	SECURITY_ATTRIBUTES securityAttr;
// 	// set SECURITY_ATTRIBUTES
// 	securityAttr.nLength = sizeof SECURITY_ATTRIBUTES;
// 	securityAttr.bInheritHandle = FALSE;
// 	securityAttr.lpSecurityDescriptor = &secutityDese;
// 	hEvent = CreateEvent(&securityAttr, FALSE, FALSE, _T("Global\\WriteTransSeverEvent"));//��������̿���event//_T("Global\\WriteTransSeverEvent")
// 	SetEvent(hEvent);
// }

CStringArray strArray;


CString UTF8ToUni(char* szchar)
{
	//�ȼ���ת�����unicode�ַ����ĳ��� 
	int len;
	len = MultiByteToWideChar(CP_UTF8, 0, szchar, -1, NULL, 0);
	// ����Unicode�ַ���������������0
	wchar_t* wszUniCode = new wchar_t[len + 1];
	memset(wszUniCode, 0, len * 2 + 2);
	// ת������
	MultiByteToWideChar(CP_UTF8, 0, szchar, -1, wszUniCode, len);
	//���ػ�
	CString  str(wszUniCode);
	delete[] wszUniCode;
	return  str;
}
void LoadInfoFromFile()
{

	UpdateConfig();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	CString strFile = _T("");
	TCHAR szFilePath[MAX_PATH];
	ZeroMemory(szFilePath, MAX_PATH);
	if (GetModuleFileName(NULL, szFilePath, MAX_PATH))
	{
		PathRemoveFileSpec(szFilePath);
	}
	strFile = szFilePath;
	strFile += _T("\\info.txt");
	//
	CString str;
	CFile f;
	if (f.Open(strFile, CFile::modeRead, NULL))
	{
		char *s = new char[100 * 1024];
		ZeroMemory(s, 100 * 1024);
		f.Read(s, 100 * 1024);
		str = UTF8ToUni(s);
		delete[] s;
		f.Close();
	}
	else
	{
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//
	strArray.RemoveAll();
	//
	CString strTemp = str;	//�˸�ֵ������
	int nIndex = 0; //
	while (1)
	{
		nIndex = strTemp.Find(_T("\r\n"));
		if (nIndex >= 0)
		{
			strArray.Add(strTemp.Left(nIndex));
			strTemp = strTemp.Right(strTemp.GetLength() - nIndex - 2);
		}
		else break;
	}
	strArray.Add(strTemp);

#if CHECK_MOMREY_TTTEST
	// 	TRACE(L"------------------------------------------------\n");
	// 	for (int i = 0; i < strArray.GetCount(); i++)
	// 	{
	// 		CString sTmp = strArray[i];
	// 		TRACE(sTmp + L"\n");
	// 	}
	// 	TRACE(L"------------------------------------------------\n");
#endif

	// ȥ������/ע���У����������б�����#��ͷ
	for (int i = strArray.GetCount() - 1; i >= 0; i--)
	{
		//ȥ������
		if (strArray[i].IsEmpty())
		{
			strArray.RemoveAt(i);
		}
		else
		{
			//ȥ��ע����
			CString sTmp = strArray[i][0];
			//TRACE(sTmp + L"\n");
			if (sTmp != L"#")
			{
				strArray.RemoveAt(i);
			}
		}
	}
}


//--------------------------------------------------
#define SERVER_NAME _T("XTransmisSever")
//--------------------------------------------------


//--------------------------------------------------
#define use_service ((CHECK_MOMREY_TTTEST == 1)? 0 : 1)
//--------------------------------------------------
void WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
void Worker_Service();
void stop();

#if use_service
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"wmainCRTStartup\"" )
#endif




int _tmain(int argc, _TCHAR* argv[])
{


#if use_service

	if (argc == 2)
	{
		if (_tcsicmp(argv[1], _T("/register")) == 0
			|| _tcsicmp(argv[1], _T("-register")) == 0)
		{
			XXX_WINSERVICE_RegisterService(SERVER_NAME);
		}
		else if (_tcsicmp(argv[1], _T("/unregister")) == 0
			|| _tcsicmp(argv[1], _T("-unregister")) == 0)
		{
			XXX_WINSERVICE_UnRegisterService(SERVER_NAME);
		}


		OutputDebugStringA("register/unregister return!");
		return 0;
	}


	SERVICE_TABLE_ENTRY ServiceTable[2];
	ServiceTable[0].lpServiceName = SERVER_NAME;
	ServiceTable[0].lpServiceProc = ServiceMain;
	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;

	if (StartServiceCtrlDispatcher(ServiceTable))
	{
		OutputDebugStringA("StartServiceCtrlDispatcher ok!");
	}
	else
	{
		OutputDebugStringA("StartServiceCtrlDispatcher error!");
	}

#else
	Worker_Service();
#endif
	//
	return 0;
}



#if use_service
void WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	OutputDebugStringA("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   ServiceMain !");

	//ע�������ƴ�����
	ssStatusHandle = RegisterServiceCtrlHandler(SERVER_NAME, ServiceCtrl);
	if (ssStatusHandle == NULL)
	{
		OutputDebugStringA("RegisterServiceCtrlHandler error!");
		return;
	}
	//
	g_bServiceExit = FALSE;
	//����Ϊ����״̬
	XXX_WINSERVICE_SetTheServiceStatus(SERVICE_RUNNING);
	//
	Worker_Service();
	//����Ϊֹͣ״̬
	XXX_WINSERVICE_SetTheServiceStatus(SERVICE_STOPPED);
}
#endif





void Worker_Service()
{

	char strModulePath[MAX_PATH] = { 0 };
	::GetModuleFileNameA(::GetModuleHandle(nullptr), strModulePath, MAX_PATH);
	std::string str(strModulePath);
	size_t nCount = str.find_last_of("\\");
	if (nCount < 0)
		nCount = str.find_last_of("/");

	if (nCount >= 0)
		str = str.substr(0, nCount + 1);

	str.append("\\XTransmisSever.ini");

	char szlicense[512] = { 0 };
	::GetPrivateProfileStringA("TRANS", "TRANS_LICENSE", "", szlicense, sizeof(szlicense), str.c_str());

	if (!CLicense::Instance().DecryptLicense(szlicense))
	{
		OutputDebugStringA("License Error!! ");
		//getchar();
		return;
	}



	//CreateMyEvent();
	SetUnhandledExceptionFilter(ApplicationCrashHandler);//���ñ���������


	XNetStartStreamManager(2);


	//���ļ������ݵ�g_mapActiveConn
	LoadInfoFromFile();

	g_csProcessSession.Lock();
	TRACE(L"------------------------------------------------\n");
	for (int i = 0; i < strArray.GetCount(); i++)
	{
		CString s = strArray[i];
		//TRACE(sTmp + L"\n");
		//
		unsigned int session = GetTickCount();
		//
		CString strTmp;
		strTmp.Format(L"%u", session);//��ע�⣬���������õ� %u !!!!!!!!!!!!!!!!!
		strTmp += s;
		//
		MyStruct4Process *p = new MyStruct4Process;
		memset(p, 0, sizeof(MyStruct4Process));
		p->bOpenProc = FALSE;
		p->dw = CrossGetTickCount64();
		WideCharToMultiByte(CP_UTF8, NULL, strTmp, -1, p->strParam, sizeof(p->strParam), NULL, FALSE);
		//
		g_mapProcess[session] = p;
		//
		TRACE(strTmp + L"\n");
		Sleep(3);
	}
	TRACE(L"------------------------------------------------\n");
	g_csProcessSession.Unlock();



	//����
	CRecycleService::Instance().Start();
	CActiveConnService::Instance().Start();

#if use_service
	while (!g_bServiceExit)
#else
	int ktest = 0;
	while (1)
#endif
	{

#if use_service
		//if (!g_bServiceExit)

		for (size_t i = 0; i < 100; i++)
		{
			Sleep(30);
			if (g_bServiceExit)
			{
				break;
			}
		}

		if (g_bServiceExit)
		{
			continue;
		}


#else
		Sleep(1 * 1000);
#endif



		LoadInfoFromFile();//<<<===============================================

		g_csProcessSession.Lock();

		

		//ȥ���Ѿ�ɾ��/�޸�֮ǰ��
		for (map<int, MyStruct4Process*>::iterator  it = g_mapProcess.begin(); it != g_mapProcess.end();)
		{
			BOOL bFind = FALSE;
			for (int i = 0; i < strArray.GetCount(); i++)
			{
				char szAAA[256] = { 0 };
				WideCharToMultiByte(CP_UTF8, NULL, strArray[i], -1, szAAA, sizeof(szAAA), NULL, FALSE);

				char *tmp = it->second->strParam;
				char *p = strchr(tmp, '#');

				if (0 == strcmp(szAAA, p))
				{
					bFind = TRUE;
					break;
				}
			}

			if (FALSE == bFind)
			{
				//û���ҵ�˵���Ǳ�ɾ����
				if (0 != it->second->pi.hProcess)
				{
					BOOL b;
					b = TerminateThread(it->second->pi.hThread, 0);
					b = TerminateProcess(it->second->pi.hProcess, 0);
					b = CloseHandle(it->second->pi.hThread);
					b = CloseHandle(it->second->pi.hProcess);
				}
				it->second->bOpenProc = FALSE;
				printf("REMOVE TransmisProcProcess %s   [%d]\n", it->second->strParam, it->second->pi.hThread);
				it = g_mapProcess.erase(it);
			}
			else
			{
				it++;
			}
		}



		//�����ӵ�
		for (int i = 0; i < strArray.GetCount(); i++)
		{

			char szAAA[256] = { 0 };
			WideCharToMultiByte(CP_UTF8, NULL, strArray[i], -1, szAAA, sizeof(szAAA), NULL, FALSE);

			BOOL bFind = FALSE;

			for (map<int, MyStruct4Process*>::iterator it_conn = g_mapProcess.begin(); it_conn != g_mapProcess.end(); it_conn++)
			{
				char *tmp = it_conn->second->strParam;
				char *p = strchr(tmp, '#');

				if (0 == strcmp(szAAA, p))
				{
					bFind = TRUE;
					break;
				}
			}

			if (FALSE == bFind)
			{
				CString s = strArray[i];
				unsigned int session = GetTickCount();
				CString strTmp;
				strTmp.Format(L"%u", session);//��ע�⣬���������õ� %u !!!!!!!!!!!!!!!!!
				strTmp += s;
				MyStruct4Process *p = new MyStruct4Process;
				memset(p, 0, sizeof(MyStruct4Process));
				p->bOpenProc = FALSE;
				p->dw = CrossGetTickCount64();
				WideCharToMultiByte(CP_UTF8, NULL, strTmp, -1, p->strParam, sizeof(p->strParam), NULL, FALSE);
				//
				g_mapProcess[session] = p;
				//
				printf("ADD--- TransmisProcProcess %s  \n", p->strParam);

				Sleep(3);
			}
		}
		//
		g_csProcessSession.Unlock();

	}


	//�����map���������е������û���������
	g_csProcessSession.Lock();
	for (map<int, MyStruct4Process*>::iterator it = g_mapProcess.begin(); it != g_mapProcess.end(); it++)
	{
		if (0 != it->second->pi.hProcess)
		{
			BOOL b;
			b = TerminateThread(it->second->pi.hThread, 0);
			b = TerminateProcess(it->second->pi.hProcess, 0);
			b = CloseHandle(it->second->pi.hThread);
			b = CloseHandle(it->second->pi.hProcess);
		}
	}
	g_mapProcess.clear();
	g_csProcessSession.Unlock();

	//
	CActiveConnService::Instance().Stop();
	CRecycleService::Instance().Stop();
	XNetStopStreamManager();
	//
	return;
}



#define SUWEMJ_LEN (8 * 1024)
#define SSSYYY_HEAD "XTransmisConfig\r\n"
BOOL UpdateConfig()
{

	TCHAR szFilePath[MAX_PATH];
	ZeroMemory(szFilePath, MAX_PATH);
	if (GetModuleFileName(NULL, szFilePath, MAX_PATH))
	{
		PathRemoveFileSpec(szFilePath);
	}

	//////////////////////////////////////////////////////////////////////////
	ST_LIVE_STRUCT_ALL m_stAllConfig;
	CString strFileConfig = _T("");
	strFileConfig = szFilePath;
	strFileConfig += _T("\\info.xtu");

	CFile file;
	if (file.Open(strFileConfig, CFile::modeRead))//����������
	{
		file.Read(&m_stAllConfig, sizeof(m_stAllConfig));
		file.Close();
	}
	else
	{
		return FALSE;
	}


	//////////////////////////////////////////////////////////////////////////
	char  pszTmp[SUWEMJ_LEN] = { 0 };
	memset(pszTmp, 0, SUWEMJ_LEN);
	memcpy(pszTmp, SSSYYY_HEAD, strlen(SSSYYY_HEAD));
	int nSeekStart = strlen(SSSYYY_HEAD);


	for (int i = 0; i < MAX_LIVE_STRUCT; i++) //ѭ��MAX_LIVE_STRUCT����¼
	{
		if (0 != m_stAllConfig.all[i].nPlatformID)//�����жϸ�����¼�ǲ����Ѿ����ã�Ҫ����
		{
			for (int k = 0; k < 6; k++)//����6����¼���ҵ�һ������ʵ�
			{

				if (0 != m_stAllConfig.all[i].dev[k].dev_id) //��ǰ��¼������
				{

					SYSTEMTIME   sysTime;
					GetLocalTime(&sysTime);
					int nLocalMinute = sysTime.wHour * 60 + sysTime.wMinute; //��ǰʱ����һ���еķ�����


					int nDevStartMinute = m_stAllConfig.all[i].dev[k].time_start * 60;
					int nEndMinute = m_stAllConfig.all[i].dev[k].time_end * 60;


					if ((nLocalMinute >= nDevStartMinute) && (nLocalMinute <= nEndMinute))
					{
						//�ҵ���һ�����ʵļ�¼

						char tmp[1024] = { 0 };

						sprintf_s(tmp, "#0#127.0.0.1#8000#%d#13800138000#13800138000#0#120.76.217.18#8002#%d\r\n",
							m_stAllConfig.all[i].dev[k].dev_id,
							m_stAllConfig.all[i].nPlatformID);


						// 							sprintf_s(tmp, "#0#127.0.0.1#8000#%d#13800138000#13800138000#0#192.168.1.186#8002#%d\r\n",
						// 								CConfigFile::Instance().m_stAllConfig.all[i].dev[k].dev_id,
						// 								CConfigFile::Instance().m_stAllConfig.all[i].nPlatformID);

						memcpy(pszTmp + nSeekStart, tmp, strlen(tmp));
						nSeekStart += strlen(tmp);



						break;//�˳�ȥ��һ���˳�ȥ�������Ĳ�����
					}

				}
			}
		}
	}



	CString strFile = _T("");
	strFile = szFilePath;
	strFile += _T("\\info.txt");

	if (file.Open(strFile, CFile::modeCreate | CFile::modeReadWrite))
	{
		file.Write(pszTmp, strlen(pszTmp));// nMaxSize);
		file.Close();
	}
	else
	{
		return FALSE;

	}

	return TRUE;
}