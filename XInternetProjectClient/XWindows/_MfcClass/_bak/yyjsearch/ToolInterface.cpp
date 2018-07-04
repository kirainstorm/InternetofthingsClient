#include "stdafx.h"
#include "ToolInterface.h"

#define MAX_UPDATE_PACK_SIZE 8000


CToolInterface::CToolInterface()
{

}



CToolInterface::~CToolInterface()
{
	m_seq = 0;
}

CToolInterface& CToolInterface::Instance()
{
	static CToolInterface inc;
	return inc;
}

BOOL CToolInterface::Login(CXNetStream* m_pTcpStream, char *user, char * pwd)
{
	return TRUE;
}
BOOL CToolInterface::SetFactoryParamUUID(char *ip, int port, char *user, char * pwd, uuidparam_t t)
{
	BOOL bReturn = FALSE;
	CXNetStream* m_pTcpStream = NULL;

	do
	{
		//链接服务器
		m_pTcpStream = XNetCreateStream4Connect(ip, port, 4);
		if (0 != XNetConnectStream(m_pTcpStream))
		{
			CROSS_TRACE("CToolInterface::SetFactoryParam -- conn svr error");
			break;
		}

		//登陆
		if (FALSE == Login(m_pTcpStream, user, pwd))
		{
			CROSS_TRACE("CToolInterface::SetFactoryParam -- login error");
			break;
		}

		//设置
		_head_t st_send;
		memset(&st_send, 0, sizeof(st_send));
		st_send.magicnum = MAGIC_NUM;
		st_send.cmd = _X_CMD_SET_FACTORY_PARAM;
		st_send.seq = m_seq++;
		st_send.session = m_session;
		st_send.datalen = sizeof(uuidparam_t);
		//
		char szTmpSend[512] = { 0 };
		char szTmpRecv[512] = { 0 };
		memset(szTmpSend, 0, 512);
		memset(szTmpRecv, 0, 512);
		//
		memcpy(szTmpSend, &st_send, sizeof(_head_t));
		memcpy(szTmpSend + sizeof(_head_t), &t, sizeof(uuidparam_t));
		//
		_head_t * pRecv = (_head_t *)szTmpRecv;
		pRecv->cmd = -1;
		//TRACE("######%d %d \n", t.cmd, t.session);
		//
		if (0 != m_pTcpStream->SyncWriteAndRead(szTmpSend, sizeof(_head_t) + sizeof(uuidparam_t), szTmpRecv, sizeof(_head_t)))
		{
			break;
		}

		if (pRecv->cmd != _X_CMD_SET_FACTORY_PARAM)
		{
			break;
		}
		if (pRecv->result != _X_CMD_RESULT_OK)
		{
			break;
		}
		if (pRecv->datalen != 0)
		{
			break;
		}

		bReturn = TRUE;
	} while (0);

	if (m_pTcpStream)
	{
		m_pTcpStream->Release();
		m_pTcpStream = NULL;
	}


	return bReturn;
}
BOOL CToolInterface::UpdateSys(char *ip, int port, char *user, char * pwd, char * pdata, int datalen)
{
	BOOL bReturn = FALSE;
	CXNetStream* m_pTcpStream = NULL;

	do
	{
		//链接服务器
		m_pTcpStream = XNetCreateStream4Connect(ip, port, 4);
		if (0 != XNetConnectStream(m_pTcpStream))
		{
			CROSS_TRACE("CToolInterface::SetFactoryParam -- conn svr error");
			break;
		}

		//登陆
		if (FALSE == Login(m_pTcpStream, user, pwd))
		{
			CROSS_TRACE("CToolInterface::SetFactoryParam -- login error");
			break;
		}

		_head_t st_send;
		_head_t st_recv;

		int k = datalen / MAX_UPDATE_PACK_SIZE; //整包数据的数量
		int z = datalen % MAX_UPDATE_PACK_SIZE; //小包数量


		//发送开始传输指令
		memset(&st_send, 0, sizeof(st_send));
		memset(&st_recv, 0, sizeof(st_recv));
		st_send.magicnum = MAGIC_NUM;
		st_send.cmd = _X_CMD_UPDATE_START;
		st_send.seq = m_seq++;
		if (0 != m_pTcpStream->SyncWriteAndRead(&st_send, sizeof(_head_t), &st_recv, sizeof(_head_t)))
		{
			break;
		}
		if (st_recv.cmd != _X_CMD_UPDATE_START)
		{
			break;
		}
		if (st_recv.result != _X_CMD_RESULT_OK)
		{
			break;
		}
		if (st_recv.datalen != 0)
		{
			break;
		}


		char szTmpSend[MAX_UPDATE_PACK_SIZE * 2] = { 0 };
		char szTmpRecv[MAX_UPDATE_PACK_SIZE * 2] = { 0 };
		//传输数据
		BOOL bSendOK = TRUE;
		//整包数据
		for (int i = 0; i < k; i++)
		{
			memset(szTmpSend, 0, MAX_UPDATE_PACK_SIZE * 2);
			memset(szTmpRecv, 0, MAX_UPDATE_PACK_SIZE * 2);

			memset(&st_send, 0, sizeof(st_send));
			st_send.magicnum = MAGIC_NUM;
			st_send.cmd = _X_CMD_UPDATE_DATA;
			st_send.seq = m_seq++;
			//st_send.session = m_session;
			st_send.datalen = MAX_UPDATE_PACK_SIZE;

			//
			memcpy(szTmpSend, &st_send, sizeof(_head_t));
			memcpy(szTmpSend + sizeof(_head_t), pdata + MAX_UPDATE_PACK_SIZE*i, MAX_UPDATE_PACK_SIZE);
			//
			_head_t * pRecv = (_head_t *)szTmpRecv;
			pRecv->cmd = -1;
			//
			if (0 != m_pTcpStream->SyncWriteAndRead(szTmpSend, sizeof(_head_t) + MAX_UPDATE_PACK_SIZE, szTmpRecv, sizeof(_head_t)))
			{
				bSendOK = FALSE;
				break;
			}
			if (pRecv->cmd != _X_CMD_UPDATE_DATA)
			{
				bSendOK = FALSE;
				break;
			}
			if (pRecv->result != _X_CMD_RESULT_OK)
			{
				bSendOK = FALSE;
				break;
			}
			if (pRecv->datalen != 0)
			{
				bSendOK = FALSE;
				break;
			}

			TRACE("------- %d \n",(i*100)/k);
		}

		if (FALSE == bSendOK)
		{
			break;
		}


		//剩余数据
		if (z > 0)
		{
			memset(szTmpSend, 0, MAX_UPDATE_PACK_SIZE*2);
			memset(szTmpRecv, 0, MAX_UPDATE_PACK_SIZE*2);

			memset(&st_send, 0, sizeof(st_send));
			st_send.magicnum = MAGIC_NUM;
			st_send.cmd = _X_CMD_UPDATE_DATA;
			st_send.seq = m_seq++;
			//st_send.session = m_session;
			st_send.datalen = z;

			//
			memcpy(szTmpSend, &st_send, sizeof(_head_t));
			memcpy(szTmpSend + sizeof(_head_t), pdata + MAX_UPDATE_PACK_SIZE*k, z);
			//
			_head_t * pRecv = (_head_t *)szTmpRecv;
			pRecv->cmd = -1;
			//
			if (0 != m_pTcpStream->SyncWriteAndRead(szTmpSend, sizeof(_head_t) + z, szTmpRecv, sizeof(_head_t)))
			{
				break;
			}
			if (pRecv->cmd != _X_CMD_UPDATE_DATA)
			{
				break;
			}
			if (pRecv->result != _X_CMD_RESULT_OK)
			{
				break;
			}
			if (pRecv->datalen != 0)
			{
				break;
			}
		}



		//发送结束指令
		memset(&st_send, 0, sizeof(st_send));
		memset(&st_recv, 0, sizeof(st_recv));
		st_send.magicnum = MAGIC_NUM;
		st_send.cmd = _X_CMD_UPDATE_END;
		st_send.seq = m_seq++;
		if (0 != m_pTcpStream->SyncWriteAndRead(&st_send, sizeof(_head_t), &st_recv, sizeof(_head_t),250))
		{
			break;
		}
		if (st_recv.cmd != _X_CMD_UPDATE_END)
		{
			break;
		}
		if (st_recv.result != _X_CMD_RESULT_OK)
		{
			break;
		}
		if (st_recv.datalen != 0)
		{
			break;
		}


		bReturn = TRUE;
	} while (0);

	if (m_pTcpStream)
	{
		m_pTcpStream->Release();
		m_pTcpStream = NULL;
	}


	return bReturn;
}



// 	BOOL bReturn = FALSE;
// 
// 	do 
// 	{
// 
// 		if (NULL == m_pTcpStream)
// 		{
// 			break;
// 		}
// 
// 		_head_t st_send;
// 		memset(&st_send, 0, sizeof(_head_t));
// 		st_send.magicnum = MAGIC_NUM;
// 		st_send.cmd = _X_CMD_LOGIN;
// 		st_send.seq = m_seq++;
// 		st_send.datalen = 0;
// 
// 		_head_t t_key;
// 		memset(&t_key, 0, sizeof(_head_t));
// 		//发送任意指令
// 		if (0 != m_pTcpStream->SyncWriteAndRead(&st_send, sizeof(st_send), &t_key, sizeof(t_key)))
// 		{
// 			break;
// 		}
// 		if (t_key.cmd != _X_CMD_LOGIN_KEY)
// 		{
// 			break;
// 		}
// 		if (t_key.datalen != 0)
// 		{
// 			break;
// 		}
// 
// 		//TRACE("111111######%d %d \n", t_key.cmd, t_key.session);
// 
// 		//使用key加密，登陆
// 		_login_t _login;
// 		memset(&_login, 0, sizeof(_login));
// 		CROSS_STRCPY(_login.user, user);
// 		char dest[256] = { 0 };
// 		//char dest1[256] = { 0 };
// 		//char data[256] = { "!@#$%^&*(IJRG$%TYI*&^%Rt866t64765" };
// 		//UINT32 ses = GetTickCount();
// 		uint32_t ses1 = 0;
// 		//TRACE("%d - %s \n", ses, data);
// 		XDESEncode(pwd, t_key.session, dest);
// 		m_session = t_key.session;
// 		//
// 		CROSS_STRCPY(_login.pwd, dest);//<<-------------------------
// 		//
// 		memset(&st_send, 0, sizeof(st_send));
// 		st_send.magicnum = MAGIC_NUM;
// 		st_send.cmd = _X_CMD_LOGIN;
// 		st_send.seq = m_seq++;
// 		st_send.session = m_session;
// 		st_send.datalen = sizeof(_login_t);
// 		//
// 		char szTmpSend[512] = { 0 };
// 		char szTmpRecv[512] = { 0 };
// 		memset(szTmpSend, 0, 512);
// 		memset(szTmpRecv, 0, 512);
// 		//
// 		memcpy(szTmpSend, &st_send, sizeof(_head_t));
// 		memcpy(szTmpSend + sizeof(_head_t), &_login, sizeof(_login_t));
// 		//
// 		_head_t * pRecv = (_head_t *)szTmpRecv;
// 		pRecv->cmd = -1;
// 		//TRACE("######%d %d \n", t.cmd, t.session);
// 		//
// 		if (0 != m_pTcpStream->SyncWriteAndRead(szTmpSend, sizeof(_head_t) + sizeof(_login_t), szTmpRecv, sizeof(_head_t)))
// 		{
// 			break;
// 		}
// 
// 		if (pRecv->cmd != _X_CMD_LOGIN)
// 		{
// 			break;
// 		}
// 		if (pRecv->result != _X_CMD_RESULT_OK)
// 		{
// 			break;
// 		}
// 		if (pRecv->datalen != 0)
// 		{
// 			break;
// 		}
// 		bReturn = TRUE;
// 
// 	} while (0);
// 
// 	return bReturn;
