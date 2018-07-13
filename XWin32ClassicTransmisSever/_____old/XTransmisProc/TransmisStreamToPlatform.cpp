#include "stdafx.h"
#include "TransmisStreamToPlatform.h"


CTransmisStreamToPlatform::CTransmisStreamToPlatform() :m_plMsg(sizeof(MyStruct4TransmisData))
{
	m_bNetError = FALSE;// 请注意这里写的 ！！！！！！！！！！ 
	m_pTcpStream = NULL;
	m_dTickHearbeat = CrossGetTickCount64();
}
CTransmisStreamToPlatform::~CTransmisStreamToPlatform()
{
}
CTransmisStreamToPlatform& CTransmisStreamToPlatform::Instance()
{
	static CTransmisStreamToPlatform ins;
	return ins;
}
void CTransmisStreamToPlatform::SetPlatformInfo(char * ip, int port, int id)
{
	memset(m_szIp, 0, sizeof(m_szIp));
	memcpy(m_szIp, ip, strlen(ip));
	m_nPort = port;
	m_nID = id;
}

BOOL CTransmisStreamToPlatform::GetError()
{

	if (m_bNetError)
	{
		return TRUE;
	}

	if ((CrossGetTickCount64() - m_dTickHearbeat) > (10 * 1000))
	{
		m_bNetError = TRUE;
	}

	return m_bNetError;
}


BOOL CTransmisStreamToPlatform::LoginPlatform()
{
	BOOL bLoginOK = FALSE;
	MyStruct4TransmisData * pSendmsg = new MyStruct4TransmisData;
	MyStruct4TransmisData *pRecvmsg = new MyStruct4TransmisData;
	rqst_head * pSendHead = (rqst_head *)pSendmsg->buffer;
	rqst_head * pRecvHead = (rqst_head *)pRecvmsg->buffer;

	do
	{
		m_pTcpStream = XNetCreateStream4Connect(m_szIp, m_nPort, 4);
		if (0 != XNetConnectStream(m_pTcpStream))
		{
			//CROSS_TRACE("CPhoneDevice -- connect error\n");
			m_bNetError = TRUE;
			break;
		}

		//请求session id
		memset(pSendmsg, 0, sizeof(MyStruct4TransmisData));
		//
		pSendHead->magicnum = MAGIC_NUM;
		pSendHead->cmd = _X_CMD_LOGIN_NEED_SESSION;
		pSendHead->datalen = 0;
		//
		pSendmsg->nSendLen = sizeof(rqst_head);
		//
		if (0 != m_pTcpStream->SyncWriteAndRead(pSendmsg->buffer, pSendmsg->nSendLen, pRecvmsg->buffer, sizeof(rqst_head)))
		{
			//CROSS_TRACE("CPhoneDevice -- connect error 1\n");
			m_bNetError = TRUE;
			break;
		}
		if (_X_CMD_LOGIN_NEED_SESSION != pRecvHead->cmd)
		{
			//CROSS_TRACE("CPhoneDevice -- connect error 2\n");
			m_bNetError = TRUE;
			break;
		}

		m_nSessionID = pRecvHead->session;


		//登陆
		memset(pSendmsg, 0, sizeof(MyStruct4TransmisData));
		//
		pSendHead->magicnum = MAGIC_NUM;
		pSendHead->cmd = _X_CMD_LOGIN;
		pSendHead->datalen = sizeof(_login_t);
		//
		pSendmsg->nSendLen = sizeof(rqst_head) + sizeof(_login_t);

		//
		_login_t * pLogin = (_login_t *)(pSendmsg->buffer + sizeof(rqst_head));
		char pwd[32] = { 0 };
		sprintf_s(pwd, "%d", m_nID);
		//char dest[128] = { 0 };
		XDESEncode(pwd, m_nSessionID, pLogin->pwd);
		//strcpy_s(pLogin->pwd, dest);

		//
		if (0 != m_pTcpStream->SyncWriteAndRead(pSendmsg->buffer, pSendmsg->nSendLen, pRecvmsg->buffer, sizeof(_head_t)))
		{
			//CROSS_TRACE("CPhoneDevice -- connect error 1\n");
			m_bNetError = TRUE;
			break;
		}
		if (_X_CMD_LOGIN != pRecvHead->cmd)
		{
			//CROSS_TRACE("CPhoneDevice -- connect error 2\n");
			m_bNetError = TRUE;
			break;
		}

		//
		m_dTickHearbeat = CrossGetTickCount64();
		m_pTcpStream->SetStreamData(this);
		m_bNetError = FALSE;
		bLoginOK = TRUE;
		//////////////////////////////////////////////////////////////////////////


	} while (0);


	if (!bLoginOK)
	{
		if (m_pTcpStream)
		{
			m_pTcpStream->Release();
			m_pTcpStream = NULL;//这里没有SetStreamData，所以不能do...while
		}
	}

	delete pSendmsg;
	delete pRecvmsg;

	return bLoginOK;
}
BOOL CTransmisStreamToPlatform::LogoutPlatform()
{
	if (m_pTcpStream)
	{
		m_pTcpStream->Release();
		m_pTcpStream = NULL;
	}

	for (list<MyStruct4TransmisData*>::iterator it = m_msgList.begin(); it != m_msgList.end();/**blank*/)
	{
		MyStruct4TransmisData * p = *it;
		it = m_msgList.erase(it);
		m_plMsg.free(p);
	}

	m_msgList.clear();

	return TRUE;
}



BOOL CTransmisStreamToPlatform::SendMessgeToPlatformFromPlatform(ST_SERVER_HEAD head, const char* pStreamData, int nLen)
{
	m_csMsg.Lock();
	if (m_pTcpStream)
	{
		if (m_msgList.size() > 30)
		{
			for (list<MyStruct4TransmisData*>::iterator it = m_msgList.begin(); it != m_msgList.end();/**blank*/)
			{
				MyStruct4TransmisData * p = *it;
				it = m_msgList.erase(it);
				m_plMsg.free(p);
			}
		}

		MyStruct4TransmisData *p = (MyStruct4TransmisData *)m_plMsg.malloc();
		rqst_head * pSendHead = (rqst_head *)p->buffer;
		_stream_t * pStream = (_stream_t *)(p->buffer + sizeof(rqst_head));
		char *pdata = p->buffer + sizeof(rqst_head) + sizeof(_stream_t);
		//
		p->nSendLen = sizeof(rqst_head) + nLen + sizeof(_stream_t);
		//
		pSendHead->magicnum = MAGIC_NUM;
		pSendHead->cmd = _X_CMD_STREAM;
		pSendHead->result = _X_CMD_RESULT_OK;
		pSendHead->session = m_nSessionID;
		//
		//
		struct tm gm = { head.wSecond, head.wMinute, head.wHour, head.wDay, head.wMonth - 1, head.wYear - 1900, head.wDayOfWeek, 0, 0 };
		pSendHead->tick = mktime(&gm) * 1000 + head.wMilliseconds;
		//
		//
		pSendHead->datalen = nLen + sizeof(_stream_t);
		//
		pStream->avType = head.nAVFrameType;
		pStream->datalen = nLen;

		pStream->nVideoFrameRate = head.nVideoFrameRate;	//帧率
		pStream->nVideoWidth = head.nVideoWidth;
		pStream->nVideoHeight = head.nVideoHeight;
		pStream->nVideoReserve = 0;
		pStream->nAudioChannels = head.nAudioChannels;
		pStream->nAudioSamplesRate = head.nAudioSamplesRate;
		pStream->nAudioBitRate = head.nAudioBitRate;
		pStream->nAudioBitsPerSample = head.nAudioBitsPerSample;
		//
		memcpy(pdata, pStreamData, nLen);
		//
		m_msgList.push_back(p);
	}


	//TRACE("-------m_msgList.size() %d  head.nAVFrameType=%d\n", m_msgList.size(), head.nAVFrameType);
	m_csMsg.Unlock();
	return TRUE;
}

BOOL CTransmisStreamToPlatform::SendHeartMsgToPlatformFromPlatform()
{
	m_csMsg.Lock();
	if (m_pTcpStream)
	{
		//-------------------------------------
		//心跳
		if ((CrossGetTickCount64() - m_dTickHearbeat) > 3000)
		{
			


			MyStruct4TransmisData *p = (MyStruct4TransmisData *)m_plMsg.malloc();
			rqst_head * pSendHead = (rqst_head *)p->buffer;
			memset(pSendHead, 0, sizeof(rqst_head));
			//
			pSendHead->magicnum = MAGIC_NUM;
			pSendHead->cmd = _X_CMD_HEARBEAT;
			pSendHead->result = _X_CMD_RESULT_OK;
			pSendHead->session = m_nSessionID;
			pSendHead->tick = CrossGetTickCount64();
			pSendHead->datalen = 0;
			//
			p->nSendLen = sizeof(rqst_head);
			//
			m_msgList.push_back(p);
		}
		//-------------------------------------
	}
	//TRACE("-------m_msgList.size() %d  head.nAVFrameType=%d\n", m_msgList.size(), head.nAVFrameType);
	m_csMsg.Unlock();
	return TRUE;
}
void CTransmisStreamToPlatform::OnPacketCompleteNetStreamData(int32_t bytesTransferred, emXStreamTansferKey transferKey)
{
	if (NULL == m_pTcpStream)
	{
		return;
	}
	if (m_bStreamIsError)
	{
		return;
	}
	////网络出错，用户断开连接
	if (bytesTransferred < 0)
	{
		m_bStreamIsError = TRUE;
		return;
	}

	if (ENUM_XTSTREAM_TRANSKEY_READ == transferKey)
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
				if (m_dataLen > (MAX_STRUCT_LEN_4_TRANSMIS_DATA - sizeof(int) - sizeof(rqst_head)))
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

	if (ENUM_XTSTREAM_TRANSKEY_WRITE == transferKey)
	{
		m_csMsg.Lock();
		if (TRANSMIS_NET_SEND_STEP_SEND == m_nSendStep)
		{
			m_nSendStep = TRANSMIS_NET_SEND_STEP_SLEEP;
			MyStruct4TransmisData * p = m_msgList.front();
			if (p)
			{
				m_msgList.pop_front();
				m_plMsg.free(p);
			}

		}
		if (TRANSMIS_NET_SEND_STEP_SLEEP == m_nSendStep)
		{
			if (m_msgList.size() > 0)
			{
//				TRACE("-------OnPacketCompleteNetStreamData send： %d  \n", ((MyStruct4TransmisData *)m_msgList.front())->nSendLen);
				m_nSendStep = TRANSMIS_NET_SEND_STEP_SEND;
				m_pTcpStream->AsyncWrite(((MyStruct4TransmisData *)m_msgList.front())->buffer, ((MyStruct4TransmisData *)m_msgList.front())->nSendLen);
			}
			else
			{
				m_pTcpStream->PostDelayWriteStatus();//强制调用一次发送回调
			}
		}
		m_csMsg.Unlock();
	}
}

void CTransmisStreamToPlatform::DoMsg()
{
	//暂时不支持透传等操作
	m_dTickHearbeat = CrossGetTickCount64();

	//OutputDebugStringA("--------------------heart\n");
}

