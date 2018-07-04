#ifndef __msg_define_for_device_channel_llkssdlj3498234bh23n
#define __msg_define_for_device_channel_llkssdlj3498234bh23n
#include "XDefineSignalingChannelStruct.h"
#include "XDefineMediaChannelStruct.h"
class XDeviceMessageCallback
{
public:
	virtual void OnDeviceMessageCallback(int type, char *buffer, int len) = 0;
};
class XDeviceInterface
{
public:
	virtual void XDelete() = 0;
	//
	virtual int XConnect(XDeviceMessageCallback *callback, char * s_ip, char * uuid) = 0;
	virtual int XDisconnect() = 0;
	//
	virtual int XSendMeaasge() = 0;
	virtual int XSendStream() = 0;
	//
};
inline BOOL ____XDeviceInterfaceIsClassicID(string str) //classic id Îª´¿Êý×Ö
{
	for (int i = 0; i < (int)(str.size()); i++)
	{
		if ((str.at(i) > '9') || (str.at(i) < '0'))
		{
			return FALSE;
		}
	}
	return TRUE;
}
#endif