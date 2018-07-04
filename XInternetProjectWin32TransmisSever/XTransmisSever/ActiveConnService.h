#pragma once



#include "ActiveConnDeviceSession.h"


class CActiveConnService : public CXNetAcceptStreamData
{
public:
	CActiveConnService();
	~CActiveConnService();

public:
	static CActiveConnService& Instance()
	{
		static CActiveConnService ins;
		return ins;
	};

public:
	virtual void OnAcceptComplete(CXNetStream * pAcceptStream);//�����豸����

public:
	int Start();//����
	int Stop();//ֹͣ

private:
	int OpenTransmisProcProcess(MyStruct4Process &t);
	int CloseTransmisProcProcess(MyStruct4Process &t);

private:
	static void* WINAPI WorkerThread(void*);
	void Worker();
	HANDLE m_hThread;
	volatile BOOL m_hEvtWorkerStop;

private:
	void * m_hServer;
};