#include "StdAfx.h"
#include "ActiveConnDeviceSession.h"
#include "ActiveConnService.h"
#include "RecycleService.h"

extern volatile BOOL g_bServiceExit;
CActiveConnDeviceSession::CActiveConnDeviceSession(CXNetStream * pAcceptStream)
{
	CRecycleService::Instance().AddRef();
	m_pTcpStream = pAcceptStream;
	m_bNetIsError = FALSE;
	m_bCheckError = FALSE;
	m_nRcvStep = NET_RECIEVE_STEP_HEAD;
	m_nSession = 0;

	m_headSend.cmd = _TRANSMIS_PROC_HEARBEAT;
	m_headSend.session = m_nSession;
	m_headSend.result = 0;
	m_headSend.k = 0xada2;

	m_dwTick = CrossGetTickCount64();
}

CActiveConnDeviceSession::~CActiveConnDeviceSession()
{

}
void CActiveConnDeviceSession::SessionDelete()
{

	if (m_pTcpStream)
	{
		m_pTcpStream->Release();
		m_pTcpStream = NULL;
	}

	delete this;
}

void CActiveConnDeviceSession::OnPacketCompleteNetStreamData(int32_t bytesTransferred, emXStreamTansferKey transferKey)
{
	if (m_bCheckError)
	{
		return;
	}

	if (ENUM_XTSTREAM_TRANSKEY_TIMER == transferKey)
	{
		if (g_bServiceExit || (CrossGetTickCount64() - m_dwTick) > 7 * 1000)//与用户的链接/超时
		{
			CRecycleService::Instance().AddToList(this);//移动自己到待删除链表
			m_bCheckError = TRUE;
		}
		else
		{
			m_pTcpStream->PostDelayTimer();
		}

		return;
	}
	//---------------------------------------------------------------------------

	if (m_bNetIsError)
	{
		return;
	}
	if (bytesTransferred < 0)
	{
		//Global_ServiceLog_AddLog("(bytesTransferred < 0)");
		m_bNetIsError = TRUE;
		return;
	}

	//---------------------------------------------------------------------------
	if (ENUM_XTSTREAM_TRANSKEY_READ == transferKey)
	{
		if (0 == bytesTransferred)
		{
			m_nRcvStep = NET_RECIEVE_STEP_HEAD;
			m_pTcpStream->AsyncRead(&m_headRecv, sizeof(m_headRecv));
			return;
		}
		else
		{
			if (m_nRcvStep == NET_RECIEVE_STEP_HEAD)
			{
				DoMsg();
				m_nRcvStep = NET_RECIEVE_STEP_HEAD;
				m_pTcpStream->AsyncRead(&m_headRecv, sizeof(m_headRecv));
				return;
			}
		}
	}
}
void CActiveConnDeviceSession::DoMsg()
{
	if (m_headRecv.cmd == _TRANSMIS_PROC_HEARBEAT)
	{
		m_dwTick = CrossGetTickCount64();

		if (m_headRecv.k != 0xada2)
		{
			m_bNetIsError = TRUE;
		}
		else
		{

			//OutputDebugStringA("^^^^^c\n");
			m_nSession = m_headRecv.session;
			m_headSend.session = m_nSession;
			m_pTcpStream->AsyncWrite(&m_headSend, sizeof(m_headSend));

			//-----------------------------------------------------------------------------------------------
			g_csProcessSession.Lock();
			map<int, MyStruct4Process*>::iterator it_conn = g_mapProcess.find(m_headSend.session);
			if (it_conn != g_mapProcess.end())
			{
				it_conn->second->dw = CrossGetTickCount64();//告诉CActiveConnService当前session正在推流
			}
			else
			{
				m_bNetIsError = TRUE;//session不存在了，所以不与XTransmisProc通讯了，让他退出，实际上不会走到这里的，如果超时了CActiveConnService会干掉XTransmisProc的
			}
			g_csProcessSession.Unlock();
			//-----------------------------------------------------------------------------------------------
		}
	}
}