// Win32Device.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "XDevice.h"
#include "XStream.hpp"


//-----------------------------------------
// ��������
//-----------------------------------------
//	Э����ο� 
//	������ͨ�����Э��Լ���Ͷ���.docx��
//	������ͨ�����Э��Լ���Ͷ���.docx��
//
//	��ʾ��������ʾ�����ӷ����������˷����ڲ���������ָ����⣬û�з���������Ϣ
//	
//	�����Э���������䣬���� ��
//	�Խ��������������ҵ�ipc
//	�ɼ���������ͷ����/����
//
//-----------------------------------------

class MessageCallback :public XDeviceMessageCallback
{
public:
	MessageCallback(){};
	~MessageCallback(){};

	virtual void OnDeviceMessageCallback(const char *jsonbuffer, int len)
	{
		//�û����豸���͵���Ϣ�ص����봦����������Ϣ����� //pDevice->XSendMessage()
		//�벻Ҫ�����ú���
		//����Э����ο� ������ͨ�����Э��Լ���Ͷ���.docx��


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

