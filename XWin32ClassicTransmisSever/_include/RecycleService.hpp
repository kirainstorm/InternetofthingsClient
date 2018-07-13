#ifndef recycle_hpp_0DFBD776_6947_497A_8B41_20EB3EF9DFC3
#define recycle_hpp_0DFBD776_6947_497A_8B41_20EB3EF9DFC3
//
//
//----------------------------------------
// kirainstorm
// https://github.com/kirainstorm
//----------------------------------------
//
// 用于其他类的回收，要回收的类需要从CXRecycleSession派生
//
//
#include "PlatformDefine.h"

#define MAX_CONNECT_COUNT_34CA4F6309A64A90961D85F4DA27B458 100

class CXRecycleSession
{
public:
	virtual void SessionDelete() = 0; //请不要在该函数中做耗费时间的操作
};
class CRecycleService
{
public:
	CRecycleService(void) {
		m_ref = 0;
		m_hThread = NULL;
		m_hEvtWorkerStop = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	};
	~CRecycleService(void) {
		Stop();
		::CloseHandle(m_hEvtWorkerStop);
	};
public:
	static CRecycleService& Instance() {
		static CRecycleService ins;
		return ins;
	};
	//启动回收服务
	int Start(const char * szServiceName)
	{
		if (m_hThread == NULL)
		{
			memset(m_szServiceName, 0, sizeof(m_szServiceName));
			memcpy(m_szServiceName, szServiceName, strlen(szServiceName));
			::ResetEvent(m_hEvtWorkerStop);
			m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkerThread, this, 0, NULL);
		}
		return 0;
	};
	//停止回收服务，是程序退出前最后一个调用的函数
	int Stop()
	{
		::SetEvent(m_hEvtWorkerStop);
		if (m_hThread)
		{
			WaitForSingleObject(m_hThread, INFINITE);
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}

		if (m_ref > 0)
		{
			while (m_ref > 0)
			{
				Recycle();
				Sleep(10);
			}
			Global_ServiceLog_AddLog("#CRecycleService#  Stop\n");
		}
		return 0;
	};
public:
	unsigned int GetRef()
	{
		return m_ref;
	};
	//所有从CXRecycleSession派生的类的构造函数都要调用此函数
	void AddRef()
	{
		m_cs.Lock();
		m_ref++;
		m_cs.Unlock();
	};
	//CXRecycleSession的派生类在需要释放的地方调用此函数
	int AddToReleaseList(CXRecycleSession* p)
	{
		m_cs.Lock();
		m_pList.push_back(p);
		m_cs.Unlock();
		return 0;
	};
private:
	static void* WINAPI WorkerThread(void*param)
	{
		CRecycleService *pService = (CRecycleService*)param;
		pService->Worker();
		return NULL;
	};
	void Worker()//管理与用户的链接，如果出错则清理
	{
		while (::WaitForSingleObject(m_hEvtWorkerStop, 2000) != WAIT_OBJECT_0)
		{
			Recycle();
		}
	}
	void Recycle()
	{
		m_cs.Lock();
		if ((m_pList.size() > 0) && (false == m_pList.empty()))
		{

			//Global_ServiceLog_AddLog("11>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>#CRecycleService#  m_ref = %d  m_pList.size()=%d\n", m_ref, m_pList.size());
			for (list<CXRecycleSession*>::iterator it = m_pList.begin(); it != m_pList.end();/**blank*/)
			{
				if (m_pList.size() > 0)
				{
					//Global_ServiceLog_AddLog("22>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>#CRecycleService#  m_ref = %d  m_pList.size()=%d\n", m_ref, m_pList.size());
					(*it)->SessionDelete();
					it = m_pList.erase(it);
					m_ref--;
				}

			}
			//Global_ServiceLog_AddLog("33>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>#CRecycleService#  m_ref = %d  m_pList.size()=%d\n", m_ref, m_pList.size());
		}
		m_cs.Unlock();
		//
		if (m_ref > 0)
			Global_ServiceLog_AddLog("#CRecycleService# Service:%s    m_ref:%d\n", m_szServiceName, m_ref);
	};
private:
	char m_szServiceName[64];
	volatile unsigned int m_ref;
	CrossCriticalSection m_cs;
	volatile HANDLE		m_hThread;
	volatile HANDLE m_hEvtWorkerStop;
	list<CXRecycleSession*> m_pList;
};


#endif