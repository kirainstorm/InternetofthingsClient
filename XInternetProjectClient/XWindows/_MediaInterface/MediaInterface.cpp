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
//pszIp classic:中心服务器地址 iot:路由服务器地址
virtual void CreateChannel(const char *pszIp, const char* pszUser, const char* pszPwd, CXSignalingChannelInferfaceMessageCallback * pCallback) = 0;
virtual void DestroyChannel() = 0;
//
virtual void XMediaInferfaceGetClassicDevices(vector<ST_SREVER_DEVICE_INFO_BASE> &v) = 0;
//
virtual void XMediaInferfaceIotDeviceGet() = 0;//获取iot设备
virtual void XMediaInferfaceIotDeviceSet(const char * jsondata);//添加/修改/删除 iot设备
//
//透明传输,ascii，最大1024个字节，仅仅将该信息插入待发送链表，不表示真正发送成功，如果链接断开，所有未发送的信息将全部丢失
virtual void XMediaInferfaceTransData(const char * jsondata) = 0;
};




class CXIOTMessageCallback//Message
{
public:
virtual int OnIOTMessageCallback(const char * data, int datalen) = 0;
};
//---------------------------------------------------------------------------------------------------------------------------------------------
//cmd 通道
//int类型返回值 0:成功 其他:失败
//请不要在CXIOTMessageCallback回调中调用其它XXCmdInstanceXXX函数
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
//透明传输，最大1024个字节，仅仅将该信息插入待发送链表，不表示真正发送成功，如果链接断开，所有未发送的信息将全部丢失
int CmdInstanceSendTransData(void * hCmdIns, const char* szData, int nLen);//
//---------------------------------------------------------------------------------------------------------------------------------------------


//创建实时传输对象
CXMediaDeviceInterface * WINAPI CreateMyMediaDeviceInterface();


//历史回放
void * CreateDownloadBufferInstance();
int DestoryDownloadBufferInstance(void * pDownloadBuffer);

*/
/*

//---------------------------------------------------------------------------------------------------------------------------------------------
//cmd 通道，请注意，该通道不自动重连，需要上层应用显示调用
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

//透明传输，最大1024个字节，仅仅将该信息插入待发送链表，不表示真正发送成功，如果链接断开，所有未发送的信息将全部丢失
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