#include "StdAfx.h"
#include "TransmisService.h"

CTransmisService::CTransmisService(void)
{
	m_bError = FALSE;
	m_pTimer = NULL;
	m_TcpStream = NULL;
	m_nSessionID = 0;
}
CTransmisService::~CTransmisService(void)
{
}
CTransmisService& CTransmisService::Instance()
{
	static CTransmisService ins;
	return ins;
}
int CTransmisService::Start(int session_id)
{
	m_nSessionID = session_id;

	m_TcpStream = XNetCreateStream4Connect("127.0.0.1", 7891, 8);
	if (0 != XNetConnectStream(m_TcpStream))
	{
		m_bError = TRUE;
		return -1;//���Ӳ������������Ҳ�֪���Լ���˭������ʧ�ܷ��أ�������Ҫ�˳�
	}

	m_pTimer = XNetCreateStream4Connect("127.0.0.1", 37891, 3);
	m_pTimer->SetStreamData(this);

	return 0;
}
int CTransmisService::Stop()
{
	if (m_pTimer)
	{
		m_pTimer->Release();
		m_pTimer = NULL;
	}

	if (m_TcpStream)
	{
		m_TcpStream->Release();
		m_TcpStream = NULL;
	}

	return 0;
}
void CTransmisService::OnPacketCompleteNetStreamData(int32_t bytesTransferred, emXStreamTansferKey transferKey)
{
	if (m_bError)
	{
		return;
	}


	if (ENUM_XTSTREAM_TRANSKEY_TIMER == transferKey)
	{
		TRANSMIN_PROC_HEAD__T stSend, stRecv;
		memset(&stSend, 0, sizeof(TRANSMIN_PROC_HEAD__T));
		memset(&stRecv, 0, sizeof(TRANSMIN_PROC_HEAD__T));
		stSend.cmd = _TRANSMIS_PROC_HEARBEAT;
		stSend.session = m_nSessionID;
		stSend.k = 0xada2;
		//�������ʹ��������Ϣ���߸������Լ������ţ����߸����������Լ����š�ʹ�ô��ݹ�����session id��ʶ
		if (0 != m_TcpStream->SyncWriteAndRead(&stSend, sizeof(TRANSMIN_PROC_HEAD__T), &stRecv, sizeof(TRANSMIN_PROC_HEAD__T)))
		{
			OutputDebugStringA("CTransmisService::OnPacketCompleteNetStreamData error\n");
			m_bError = TRUE;
		}


		m_pTimer->PostDelayTimer(1500);
	}
}