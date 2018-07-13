#include "stdafx.h"
#include "MediaToMediaInterface.h"

#include "TransmisStreamToPlatform.h"
#include "../../_MediaInterface/MediaInterface.h"
#pragma comment( lib, "C:\\output32\\MediaInterface.lib" )



CMediaToMediaInterface::CMediaToMediaInterface()
{
	m_pDevObject = NULL;
	m_dTickHearbeat = CrossGetTickCount64();
}

CMediaToMediaInterface::~CMediaToMediaInterface()
{
}

void CMediaToMediaInterface::OpenTransmisDevice(char * p1, char * p2, char * p3, char * p4, int val1, int val2, int val3, int val4)
{
	m_pDevObject = CreateMyMediaDeviceInterface();// pDevFun();
	//
	DeviceInfo_T t;
	memset(&t, 0, sizeof(DeviceInfo_T));
	strcpy_s(t.core_svr_ip, p1);
	strcpy_s(t.user_name, p2);
	strcpy_s(t.user_pwd, p3);
	t.core_svr_port = val1;
	t.dev_id = val2;
	t.dev_channel = 0;
	//
	m_pDevObject->XDeviceSetInfo(t);
	m_pDevObject->XDeviceLogin();
	m_pDevObject->XPreviewOpenForWindows(NULL, this);
	m_dTickHearbeat = CrossGetTickCount64();
}

void CMediaToMediaInterface::CloseTransmisDevice()
{
	if (m_pDevObject)
	{
		m_pDevObject->XPreviewClose();
		m_pDevObject->XDeviceLogout();
		m_pDevObject->XDeviceDelete();
		m_pDevObject = NULL;
	}

}

BOOL CMediaToMediaInterface::GetError()
{
	if ((CrossGetTickCount64() - m_dTickHearbeat) > (10 * 1000))
	{
		return TRUE;//超时出错
	}

	return CTransmisStreamToPlatform::Instance().GetError();//发送到服务器线程出错
}

int CMediaToMediaInterface::OnStreamData(ST_SERVER_HEAD head, const char* pStreamData, int nLen)
{

	//TRACE("-------OnStreamData %d  \n", nLen);
	m_dTickHearbeat = CrossGetTickCount64();
	CTransmisStreamToPlatform::Instance().SendMessgeToPlatformFromPlatform(head, pStreamData, nLen);
	return 0;
}