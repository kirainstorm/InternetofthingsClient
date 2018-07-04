#pragma once

#include "syscmdinfo.h"

class CToolInterface
{
public:
	CToolInterface();
	~CToolInterface();

public:
	static CToolInterface& Instance();

public:
	BOOL SetFactoryParamUUID(char *ip, int port, char *user, char * pwd, uuidparam_t t); //配置工厂参数
	BOOL UpdateSys(char *ip, int port, char *user, char * pwd, char * pdata, int datalen); //系统升级


private:
	BOOL Login(CXNetStream* m_pTcpStream, char *user, char * pwd);

private:
	int m_seq;
	int m_session;
};

