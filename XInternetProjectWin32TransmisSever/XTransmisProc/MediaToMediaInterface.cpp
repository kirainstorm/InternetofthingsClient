#include "stdafx.h"
#include "MediaToMediaInterface.h"
#include "TransmisStreamToPlatform.h"

CMediaToMediaInterface::CMediaToMediaInterface()
{
	m_pDevObject = NULL;
	m_bUserCloseThis = FALSE;



#if USE_THREAD_NOT_TIMER
	//m_pTimer = NULL;
	m_hThread = NULL;
	m_hEvtWorkerStop = FALSE;
#else
	m_pTimer = XNetCreateStream4Connect("127.0.0.1", 54514);
	m_pTimer->SetStreamData(this);
#endif

	m_dwLastTick = GetTickCount64();
}
CMediaToMediaInterface::~CMediaToMediaInterface()
{
//	::CloseHandle(m_hEvtWorkerStop);
}
void CMediaToMediaInterface::OpenTransmisDevice(char * p1, char * p2, char * p3, char * p4, int val1, int val2, int val3, int val4)
{
	//
	m_dwLastTick = GetTickCount64();
	memset(&t, 0, sizeof(DeviceInfo_T));
	strcpy_s(t.core_svr_ip, p1);
	strcpy_s(t.user_name, p2);
	strcpy_s(t.user_pwd, p3);
	t.core_svr_port = val1;
	t.dev_id = val2;
	t.dev_channel = 0;
	//
	m_pDevObject = new CMediaDevInterface();
	m_pDevObject->XDeviceSetInfo(t);
	m_pDevObject->XDeviceLogin();
	m_pDevObject->XPreviewOpenForWindows(NULL, this);

#if USE_THREAD_NOT_TIMER
	if (m_hThread == NULL)
	{
		m_hEvtWorkerStop = FALSE;
		m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkerThread, this, 0, NULL);
	}
#endif

}
void CMediaToMediaInterface::CloseTransmisDevice()
{
	//
	m_bUserCloseThis = TRUE;

#if USE_THREAD_NOT_TIMER
	//
	m_hEvtWorkerStop = TRUE;
	if (m_hThread)
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
#else

	if (m_pTimer)
	{
		m_pTimer->Release();
		m_pTimer = NULL;
	}

#endif



	//
	if (m_pDevObject)
	{
		m_pDevObject->XPreviewClose();
		m_pDevObject->XDeviceLogout();
		m_pDevObject->XDeviceDelete();
		m_pDevObject = NULL;
	}
	//
	CTransmisStreamToPlatform::Instance().LogoutPlatform();

	delete this;
}

BOOL CMediaToMediaInterface::GetError()
{

	if ((GetTickCount64() - m_dwLastTick) > 7*1000)
	{
		return TRUE;
	}

	return CTransmisStreamToPlatform::Instance().GetError();//ÓÀÔ¶·µ»ØFALSE
}

int CMediaToMediaInterface::OnStreamData(ST_SERVER_HEAD head, const char* pStreamData, int nLen)
{
	m_cs.Lock();
	m_dwLastTick = GetTickCount64();
	CTransmisStreamToPlatform::Instance().SendMessgeToPlatformFromPlatform(head, pStreamData, nLen);
	m_cs.Unlock();

	return 0;
}


void* WINAPI CMediaToMediaInterface::WorkerThread(void* param)
{
	CMediaToMediaInterface *pService = (CMediaToMediaInterface*)param;
	pService->Worker();
	return NULL;
}


void CMediaToMediaInterface::Worker()
{
	CTransmisStreamToPlatform::Instance().LogoutPlatform();
	CTransmisStreamToPlatform::Instance().LoginPlatform();

	while (!m_hEvtWorkerStop)
	{
		m_cs.Lock();
		if (FALSE == CTransmisStreamToPlatform::Instance().GetError())
		{
			CTransmisStreamToPlatform::Instance().SendHeartMsgToPlatformFromPlatform();
		}
		m_cs.Unlock();


		for (int i = 0; i < 100;i++)
		{
			Sleep(30);
			if (m_hEvtWorkerStop)
			{
				break;
			}
		}

		
	}
}