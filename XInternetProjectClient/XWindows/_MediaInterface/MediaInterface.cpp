// MediaInterface.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "MediaInterface.h"
#include "XClientStructDefine.h"
#include "ClassicClientInstance.h"
#include "IOTClientInstance.h"
#include "XStream.hpp"

void XMediaInferfaceInit()
{
#ifdef _PLATFORM_WINDOW
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	WSAStartup(wVersionRequested, &wsaData);
#endif
	XNetStartStreamManager();
}
void XMediaInferfaceUninit()
{
#ifdef _PLATFORM_WINDOW
	WSACleanup();
#endif
	XNetStopStreamManager();
}
CXClientInferface * XMediaCreateClientInferface(emXClientType type)
{
	CXClientInferface * pInstance = nullptr;
	if (type == X_CLIENT_XXX_TYPE_CLASSIC)
	{
		pInstance = new CXClassicClientInstance();
		assert(pInstance);
	}
	else
	{
		pInstance = new CXIotClientInstance();
		assert(pInstance);
	}
	return pInstance;
}

/*

class CXSignalingChannelInferfaceMessageCallback//Message
{
public:
virtual int OnSignalingChannelMessageCallback(const char * jsondata, int datalen) = 0;
};
class CXSignalingChannelInferface
{
public:
//pszIp classic:���ķ�������ַ iot:·�ɷ�������ַ
virtual void CreateChannel(const char *pszIp, const char* pszUser, const char* pszPwd, CXSignalingChannelInferfaceMessageCallback * pCallback) = 0;
virtual void DestroyChannel() = 0;
//
virtual void XMediaInferfaceGetClassicDevices(vector<ST_SREVER_DEVICE_INFO_BASE> &v) = 0;
//
virtual void XMediaInferfaceIotDeviceGet() = 0;//��ȡiot�豸
virtual void XMediaInferfaceIotDeviceSet(const char * jsondata);//���/�޸�/ɾ�� iot�豸
//
//͸������,ascii�����1024���ֽڣ�����������Ϣ�����������������ʾ�������ͳɹ���������ӶϿ�������δ���͵���Ϣ��ȫ����ʧ
virtual void XMediaInferfaceTransData(const char * jsondata) = 0;
};




class CXIOTMessageCallback//Message
{
public:
virtual int OnIOTMessageCallback(const char * data, int datalen) = 0;
};
//---------------------------------------------------------------------------------------------------------------------------------------------
//cmd ͨ��
//int���ͷ���ֵ 0:�ɹ� ����:ʧ��
//�벻Ҫ��CXIOTMessageCallback�ص��е�������XXCmdInstanceXXX����
//
void * CreateCmdInstance(CXIOTMessageCallback * pIOTMsgCallback, const char *pRedirectUrl, const char* szUser, const char* szPwd);
int DestroyCmdInstance(void * hCmdIns);
int CmdInstanceSetIOTServerIP(void * hCmdIns, const char * IOTIP);
char * CmdInstanceGetIOTServerIP(void * hCmdIns);
int CmdInstanceLogin(void * hCmdIns);
int CmdInstanceLogout(void * hCmdIns);
//
int CmdInstanceGetUserDevice(void * hCmdIns);
int CmdInstanceAddDevice(void * hCmdIns, const char * devuuid);
int CmdInstanceDeleteDevice(void * hCmdIns, const char * devuuid);
int CmdInstanceOpenHLS(void * hCmdIns, const char * devuuid);
//͸�����䣬���1024���ֽڣ�����������Ϣ�����������������ʾ�������ͳɹ���������ӶϿ�������δ���͵���Ϣ��ȫ����ʧ
int CmdInstanceSendTransData(void * hCmdIns, const char* szData, int nLen);//
//---------------------------------------------------------------------------------------------------------------------------------------------


//����ʵʱ�������
CXMediaDeviceInterface * WINAPI CreateMyMediaDeviceInterface();


//��ʷ�ط�
void * CreateDownloadBufferInstance();
int DestoryDownloadBufferInstance(void * pDownloadBuffer);

*/
/*

//---------------------------------------------------------------------------------------------------------------------------------------------
//cmd ͨ������ע�⣬��ͨ�����Զ���������Ҫ�ϲ�Ӧ����ʾ����
void * CreateCmdInstance(CXIOTMessageCallback * pIOTMsgCallback, const char *pRedirectUrl, const char* szUser, const char* szPwd)
{
	return new CNetToServer(pIOTMsgCallback, pRedirectUrl, szUser, szPwd);
}
int DestroyCmdInstance(void * hCmdIns)
{
	delete ((CNetToServer *)hCmdIns);
	return 0;
}
int CmdInstanceSetIOTServerIP(void * hCmdIns, const char * IOTIP)
{
	((CNetToServer *)hCmdIns)->SetIOTServerIP(IOTIP);
	return 0;
}
char *  CmdInstanceGetIOTServerIP(void * hCmdIns)
{
	return ((CNetToServer *)hCmdIns)->GetIOTServerIP();
}
int CmdInstanceLogin(void * hCmdIns)
{
	return ((CNetToServer *)hCmdIns)->Login();
}
int CmdInstanceLogout(void * hCmdIns)
{
	return ((CNetToServer *)hCmdIns)->Logout();
}
int CmdInstanceGetUserDevice(void * hCmdIns)
{
	return ((CNetToServer *)hCmdIns)->GetUserDevice();
}
int CmdInstanceOpenHLS(void * hCmdIns, const char * devuuid)
{
	return ((CNetToServer *)hCmdIns)->OpenHLS(devuuid);
}
int CmdInstanceAddDevice(void * hCmdIns, const char * devuuid)
{
	return ((CNetToServer *)hCmdIns)->AddDevice(devuuid);
}
int CmdInstanceDeleteDevice(void * hCmdIns, const char * devuuid)
{
	return ((CNetToServer *)hCmdIns)->DeleteDevice(devuuid);
}

//͸�����䣬���1024���ֽڣ�����������Ϣ�����������������ʾ�������ͳɹ���������ӶϿ�������δ���͵���Ϣ��ȫ����ʧ
int CmdInstanceSendTransData(void * hCmdIns, const char* szData, int nLen)
{
	return ((CNetToServer *)hCmdIns)->TansData(szData,nLen);
}
//---------------------------------------------------------------------------------------------------------------------------------------------







CXMediaDeviceInterface * WINAPI CreateMyMediaDeviceInterface()
{
	CXMediaDeviceInterface * p = new CMediaDevInterface;
	return p;
}


// int MediaInterfaceGetUserDevices(const char *ip, int port, const char* user, const char * pwd, vector<ST_SREVER_DEVICE_INFO_BASE> &rs)
// {
// 	return CNetToServer::Instance().GetUserDevices(ip, port, user, pwd, rs);
// }
*/