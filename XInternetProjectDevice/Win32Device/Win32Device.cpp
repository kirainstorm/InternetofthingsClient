// Win32Device.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "XDevice.h"
#include "XStream.hpp"


//-----------------------------------------
// 正在完善
//-----------------------------------------
//	协议请参考 
//	《信令通道相关协议约定和定义.docx》
//	《信令通道相关协议约定和定义.docx》
//
//	该示例仅仅演示了连接服务器，除了发送内部发送心跳指令保活外，没有发送其他信息
//	
//	请根据协议自行扩充，例如 ：
//	对接其他将其他厂家的ipc
//	采集电脑摄像头数据/截屏
//
//-----------------------------------------

class MessageCallback :public XDeviceMessageCallback
{
public:
	MessageCallback(){};
	~MessageCallback(){};

	virtual void OnDeviceMessageCallback(const char *jsonbuffer, int len)
	{
		//用户向设备发送的消息回调，请处理完成相关信息后调用 //pDevice->XSendMessage()
		//请不要阻塞该函数
		//具体协议请参考 《信令通道相关协议约定和定义.docx》


	};
private:

};
#include "XDes.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//init net core
	XNetStartStreamManager();

	//Signaling callback
	XDeviceMessageCallback *pBack = new MessageCallback();

	//create instance
	XDeviceInterface * pDevice = CreateDeviceInstance();

	//connect
	//pDevice->XConnect(pBack, "47.96.249.142", "3001");
	pDevice->XConnect(pBack, "172.25.189.244", "122");



	while (1)
	{
		Sleep(1000);
	}

	//delete
 	pDevice->XDelete();
	pDevice = nullptr;

	//
	XNetStopStreamManager();


	return 0;
}

