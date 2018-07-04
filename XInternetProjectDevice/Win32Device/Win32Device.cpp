// Win32Device.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "XDevice.h"
#include "XStream.hpp"


class MessageCallback :public XDeviceMessageCallback
{
public:
	MessageCallback(){};
	~MessageCallback(){};

	virtual void OnDeviceMessageCallback(int type, char *buffer, int len)
	{

	};
private:

};
#include "XDes.h"
//#include "PhoneDevice.h"
int _tmain(int argc, _TCHAR* argv[])
{

	char _uuuuu[256] = { 0 };
	XDESEncode("stLogin.userdes", 3345, _uuuuu);


	XNetStartStreamManager();
	XDeviceMessageCallback *pBack = new MessageCallback();

	XDeviceInterface * pDevice = CreateDeviceInstance();
	pDevice->XConnect(pBack, "47.96.249.142", "3001");
	//pDevice->XConnect(pBack, "127.0.0.1", "6000");

// 	CPhoneDevice *p = new CPhoneDevice();;
// 	p->Start();
	while (1)
	{
		Sleep(1000);
	}
 	delete pDevice;

	return 0;
}

