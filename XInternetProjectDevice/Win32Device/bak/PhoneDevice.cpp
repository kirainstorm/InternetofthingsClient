#include "stdafx.h"
#include "PhoneDevice.h"
#include "XDes.h"

CPhoneDevice::CPhoneDevice() :m_plMsg(sizeof(MyStruct4PhoneDeviceSession))
{
	m_szBuffer = new unsigned char[MAX_MSG_4_PHONE_DEVICE_SESSION];
	m_hWorkerThread = CROSS_THREAD_NULL;
	m_bLogin = FALSE;
	m_pTcpStream = NULL;
}
CPhoneDevice::~CPhoneDevice(void)
{
	Stop();
	delete[] m_szBuffer;
}
void CPhoneDevice::Start()
{
	if (m_hWorkerThread == CROSS_THREAD_NULL)
	{
		m_bStreamIsError = FALSE;
		m_nSession = CrossGetTickCount64();
		m_bLogin = FALSE;
		m_hEvtWorkerStop = FALSE;
		m_hWorkerThread = CrossCreateThread(WorkerThread, this);
	}
}
void CPhoneDevice::Stop()
{
	if (m_hWorkerThread)
	{
		m_hEvtWorkerStop = TRUE;
		CROSS_TRACE("CPhoneDevice -- Stop -- 2\n");
		//
		CrossWaitThread(m_hWorkerThread);
		CrossCloseThread(m_hWorkerThread);
		m_hWorkerThread = CROSS_THREAD_NULL;
		m_bLogin = FALSE;
	}
	LogoutPlat();
}

void CPhoneDevice::LogoutPlat()
{
	if (m_pTcpStream)
	{
		m_pTcpStream->Release();
		m_pTcpStream = NULL;
	}
	m_csMsg.Lock();
	while (m_msgList.size() > 0)
	{
		MyStruct4PhoneDeviceSession * p = m_msgList.front();
		m_msgList.pop_front();
		m_plMsg.free(p);
	}
	m_csMsg.Unlock();
}

void CPhoneDevice::InputData(int avtype, int isKey, char * buffer, int len,int width,int height,int rate)
{
	if (!m_bLogin)
	{
		return;
	}

	if (m_hEvtWorkerStop)
	{
		return;
	}
	m_csMsg.Lock();
	MyStruct4PhoneDeviceSession *p = (MyStruct4PhoneDeviceSession *)m_plMsg.malloc();
	p->head.magicnum = MAGIC_NUM;
	p->head.cmd = _X_CMD_STREAM;
	p->head.result = _X_CMD_RESULT_OK;
	p->head.session = m_nSession;
	p->head.datalen = len + sizeof(_stream_t);
	p->head.tick = CrossGetTickCount64();
	//
	_stream_t m_stream;
	m_stream.avType = (isKey) ? 0 : 1;
	//
	m_stream.nVideoFrameRate = rate;	//帧率
	m_stream.nVideoWidth = width;
	m_stream.nVideoHeight = height;
	m_stream.nAudioChannels = 0;
	m_stream.nAudioSamplesRate = 0;
	m_stream.nAudioBitRate = 0;
	m_stream.nAudioBitsPerSample = 0;

	//
	memcpy(p->buffer, &m_stream, sizeof(m_stream));
	memcpy(p->buffer + sizeof(m_stream), buffer, len);
	//
	m_msgList.push_back(p);
	m_csMsg.Unlock();
}








void CPhoneDevice::OnPacketCompleteNetStreamData(int32_t bytesTransferred, emXStreamTansferKey transferKey)
{
	if (NULL == m_pTcpStream)
	{
		return;
	}
	if (m_bStreamIsError)
	{
		return;
	}
	//
	if (bytesTransferred < 0)
	{
		m_bStreamIsError = TRUE;
		return;
	}

	if (transferKey == ENUM_XTSTREAM_TRANSKEY_READ)
	{
		if (0 == bytesTransferred)
		{
			m_nRcvStep = NET_RECIEVE_STEP_HEAD;
			m_pTcpStream->AsyncRead(&m_head, sizeof(m_head));
			return;
		}
		else
		{
			if (m_nRcvStep == NET_RECIEVE_STEP_HEAD)
			{
				m_dataLen = m_head.datalen;
				if (m_dataLen > MAX_MSG_4_PHONE_DEVICE_SESSION)
				{
					m_bStreamIsError = TRUE;
					return;
				}
				if (m_dataLen == 0)
				{
					DoMsg();
					m_nRcvStep = NET_RECIEVE_STEP_HEAD;
					m_pTcpStream->AsyncRead(&m_head, sizeof(m_head));
				}
				else
				{
					m_nRcvStep = NET_RECIEVE_STEP_BODY;
					m_pTcpStream->AsyncRead(m_szBuffer, m_dataLen);
				}
				return;
			}
			if (m_nRcvStep == NET_RECIEVE_STEP_BODY)
			{
				DoMsg();
				m_nRcvStep = NET_RECIEVE_STEP_HEAD;
				m_pTcpStream->AsyncRead(&m_head, sizeof(m_head));
				return;
			}
		}
	}
	if (transferKey == ENUM_XTSTREAM_TRANSKEY_WRITE)
	{

		m_csMsg.Lock();
			if (m_msgList.size() > 0)
			{
				m_dTickHearbeat = CrossGetTickCount64();
				m_pTcpStream->AsyncWrite(m_msgList.front(), 
					((MyStruct4PhoneDeviceSession *)m_msgList.front())->head.datalen + sizeof(_head_t));
m_msgList.pop_front();

				
			}
			else
			{
				m_pTcpStream->PostDelayWriteStatus();//
			}
			m_csMsg.Unlock();
	}
}

void CPhoneDevice::DoMsg()
{
	m_dTickHearbeat = CrossGetTickCount64();
}



int CPhoneDevice::LoginPlat()
{
	CROSS_TRACE("CPhoneDevice -- Login \n");
	MyStruct4PhoneDeviceSession * pSendmsg = new MyStruct4PhoneDeviceSession;
	MyStruct4PhoneDeviceSession *pRecvmsg = new MyStruct4PhoneDeviceSession;
	do
	{
		CROSS_TRACE("CPhoneDevice -- Login 1\n");
		m_pTcpStream = XNetCreateStream4Connect("120.76.217.18", 8002, 3);//23.99.10
		//m_pTcpStream = XNetCreateStream4Connect("127.0.0.1", 8002, 3);//23.99.10
		if (0 != XNetConnectStream(m_pTcpStream))
		{
			CROSS_TRACE("CPhoneDevice -- connect error\n");
			break;
		}
		CROSS_TRACE("CPhoneDevice -- Login 2\n");
		memset(pSendmsg, 0, sizeof(MyStruct4PhoneDeviceSession));
		pSendmsg->head.result = _X_CMD_RESULT_OK;
		pSendmsg->head.cmd = _X_CMD_LOGIN_NEED_SESSION;
		pSendmsg->head.datalen = 0;
		if (0 != m_pTcpStream->SyncWriteAndRead(pSendmsg, sizeof(pSendmsg->head), pRecvmsg, sizeof(pRecvmsg->head),5))
		{
			CROSS_TRACE("CPhoneDevice -- connect error 1\n");
			break;
		}
		if (_X_CMD_LOGIN_NEED_SESSION != pRecvmsg->head.cmd)
		{
			CROSS_TRACE("CPhoneDevice -- connect error 2\n");
			break;
		}
		m_nSession = pRecvmsg->head.session;
		//
		//
		memset(pSendmsg, 0, sizeof(MyStruct4PhoneDeviceSession));
		pSendmsg->head.result = _X_CMD_RESULT_OK;
		pSendmsg->head.cmd = _X_CMD_LOGIN;
		pSendmsg->head.session = m_nSession;
		pSendmsg->head.datalen = sizeof(_login_t);
		//
		_login_t *plogin = (_login_t *)pSendmsg->buffer;
		//
		//XDESEncode("19544", 19544, login.user);
		XDESEncode("8012", m_nSession, plogin->pwd);


		//
		//
		if (0 != m_pTcpStream->SyncWriteAndRead(pSendmsg, pSendmsg->head.datalen + sizeof(pSendmsg->head), pRecvmsg, sizeof(pRecvmsg->head)))
		{
			CROSS_TRACE("CPhoneDevice -- connect error 1\n");
			break;
		}
		if (_X_CMD_LOGIN != pRecvmsg->head.cmd)
		{
			CROSS_TRACE("CPhoneDevice -- connect error 2\n");
			break;
		}
		CROSS_TRACE("CPhoneDevice -- connect okokokokokok\n");
		//////////////////////////////////////////////////////////////////////////
		
		m_dTickHearbeat = CrossGetTickCount64();
		m_pTcpStream->SetStreamData(this);
		delete pSendmsg;
		delete pRecvmsg;
		return 0;
		//////////////////////////////////////////////////////////////////////////


	} while (0);

	if (m_pTcpStream)
	{
		m_pTcpStream->Release();
		m_pTcpStream = NULL;
	}

	delete pSendmsg;
	delete pRecvmsg;
	return -1;
}


int CPhoneDevice::WorkerThread(void* param)
{
	CPhoneDevice *pService = (CPhoneDevice*)param;
	pService->Worker();
	return 0;
}
//---------------------------------------------------------------------------------------
void CPhoneDevice::Worker()
{
	//CROSS_TRACE("CPhoneDevice -- Worker 1\n");
	m_bLogin = FALSE;
	while (!m_hEvtWorkerStop)// (::WaitForSingleObject(m_hEvtWorkerStop, 3 * 1000) != WAIT_OBJECT_0)
	{

		//CROSS_TRACE("CPhoneDevice -- Worker 2\n");
		//////////////////////////////////////////////////////////////////////////
		if (!m_bLogin)
		{
			//CROSS_TRACE("CPhoneDevice -- Worker 3\n");
			
			if (0 != LoginPlat())
			{
				//CROSS_TRACE("CPhoneDevice -- Worker 4\n");
				m_bLogin = FALSE;
				if (!m_hEvtWorkerStop)
				{
					
					for (int i = 0; i < 30; i++)
					{
						CrossSleep(100);
						if (m_hEvtWorkerStop)
							break;
					}
					CROSS_TRACE("Login error!\n");
					continue;
				}
			}
			else
			{
				m_bLogin = TRUE;
			}
			//CROSS_TRACE("CPhoneDevice -- Worker 5\n");
		}
		//CROSS_TRACE("CPhoneDevice -- Worker 6\n");

		//////////////////////////////////////////////////////////////////////////
		if ((CrossGetTickCount64() - m_dTickHearbeat) > 20 * 1000)
		{
			m_bLogin = FALSE;
			LogoutPlat();
		}

		//////////////////////////////////////////////////////////////////////////
		CrossSleep(1);

	}


	// #ifdef _PLATFORM_WINDOW
	// 	TRACE("##@@@@@@  CYYJPlayInterface::Worker() Exit!\n");
	// #endif

}
