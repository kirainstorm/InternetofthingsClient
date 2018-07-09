#include "stdafx.h"
#include "DeviceManager.h"
#include "PlatformClient.h"

#ifdef _PLATFORM_WINDOW

CDeviceManager::CDeviceManager()
{

}


CDeviceManager::~CDeviceManager()
{



	ClearDevice();


	// 	if (pData->pDevObject)
	// 	{
	// 		pData->pDevObject->XDeviceClose();
	// 		pData->pDevObject->XDeviceDelete();
	// 		pData->pDevObject = NULL;
	// 	}
}
void CDeviceManager::ClearDevice()
{
	m_cs.Lock();
// 	vector <stDevice>::iterator Iter;
// 	for (Iter = m_DevList.begin(); Iter != m_DevList.end(); Iter++)
// 	{
// 		if ((*Iter).pDevObject)
// 		{
// 			(*Iter).pDevObject->XDeviceClose();
// 			(*Iter).pDevObject->XDeviceDelete();
// 			(*Iter).pDevObject = NULL;
// 		}
// 	}
	m_DevList.clear();
	m_cs.Unlock();
}

// BOOL CDeviceManager::GetDeviceByDeviceId(int dev_id, stDevice &st)
// {
// 	BOOL bFind = FALSE;
// 
// 	m_cs.Lock();
// 	vector <stDevice>::iterator Iter;
// 	for (Iter = m_DevList.begin(); Iter != m_DevList.end(); Iter++)
// 	{
// 		if ((*Iter).SvrDevInfo.dev_id == dev_id)
// 		{
// 			// memcpy(&st, &(*Iter), sizeof(stDevice));
// 			st = (*Iter);
// 			bFind = TRUE;
// 			break;
// 		}
// 	}
// 	m_cs.Unlock();
// 	return bFind;
// }

// BOOL CDeviceManager::GetDeviceByUuid(char * _devuuid, stDevice &st)
// {
// 	BOOL bFind = FALSE;
// 
// // 	m_cs.Lock();
// // 	vector <stDevice>::iterator Iter;
// // 	for (Iter = m_DevList.begin(); Iter != m_DevList.end(); Iter++)
// // 	{
// // 		if (0 == strcmp((*Iter).SvrDevInfo.dev_uuid, _devuuid))
// // 		{
// // 			st = *Iter;
// // 			bFind = TRUE;
// // 			break;
// // 		}
// // 	}
// // 	m_cs.Unlock();
// 	return bFind;
// 
// }
BOOL CDeviceManager::GetDeviceByIndex(int i, stDevice &st)
{
	BOOL bFind = FALSE;

	m_cs.Lock();
	if ((int)m_DevList.size()>i)
	{
		st = m_DevList[i];
		bFind = TRUE;
	}
	m_cs.Unlock();
	return bFind;
}


void CDeviceManager::SetDeviceOpenStatusByDeviceID(HTREEITEM item, BOOL bOpen)
{
	m_cs.Lock();
	vector <stDevice>::iterator Iter;
	for (Iter = m_DevList.begin(); Iter != m_DevList.end(); Iter++)
	{
		if ((*Iter).hItem == item)
		{
			(*Iter).bOpen = bOpen;
			break;
		}
	}
	m_cs.Unlock();
}

BOOL CDeviceManager::AddDevice(stDevice st1)
{
	m_cs.Lock();
	stDevice stTemp = st1;
	stTemp.bOpen = FALSE;
	m_DevList.push_back(stTemp);
	m_cs.Unlock();
	return TRUE;
}
BOOL CDeviceManager::ModifyDevice(stDevice st)
{
	BOOL bResult = TRUE;
// 
// 	m_cs.Lock();
// 
// 	vector <stDevice>::iterator Iter;
// 	for (Iter = m_DevList.begin(); Iter != m_DevList.end(); Iter++)
// 	{
// 		if (0 == strcmp((*Iter).SvrDevInfo.dev_uuid,st.SvrDevInfo.dev_uuid))
// 		{
// 			break;
// 		}
// 	}
// 
// // 	if ((*Iter).pDevObject)
// // 	{
// // 		(*Iter).pDevObject->XDeviceClose();
// // 		(*Iter).pDevObject->XDeviceDelete();
// // 
// // 		(*Iter).pDevObject = NULL;
// // 	}
// 
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	(*Iter) = st;
// 	(*Iter).bOpen = FALSE;
// 	//(*Iter).pDevObject = NULL;
// 
// 	//////////////////////////////////////////////////////////////////////////
// // 	HMODULE hDevModule = CDeviceLibMgr::Instance().GetDevLibModule();
// // 	//
// // 	XCreateDeviceInterfaceFunction pDevFun = (XCreateDeviceInterfaceFunction)GetProcAddress(hDevModule, "CreateDevice");
// // 	(*Iter).pDevObject = pDevFun();
// // 	//
// // 	(*Iter).pDevObject->XDeviceSetInfo(
// // 		(*Iter).SvrDevInfo.dev_media_ip,
// // 		(*Iter).SvrDevInfo.dev_media_port,
// // 		theApp.m_strName.c_str(),
// // 		theApp.m_strPwd.c_str(),
// // 		(*Iter).SvrDevInfo.dev_playback_ip,
// // 		(*Iter).SvrDevInfo.dev_playback_port,
// // 		0,
// // 		X_DEVICE_TYPE_NONE);
// // 
// // 	(*Iter).pDevObject->XDeviceOpen();
// 
// 
// 
// 
// 	m_cs.Unlock();

	return bResult;
}
BOOL CDeviceManager::DeleteDevice(char * _devuuid)
{
 	BOOL bResult = TRUE;

// 	m_cs.Lock();
// 	vector <stDevice>::iterator Iter;
// 	for (Iter = m_DevList.begin(); Iter != m_DevList.end(); Iter++)
// 	{
// 		if (0 == strcmp((*Iter).SvrDevInfo.dev_uuid, _devuuid))
// 		{
// 			break;
// 		}
// 	}
// 
// // 	if ((*Iter).pDevObject)
// // 	{
// // 		(*Iter).pDevObject->XDeviceClose();
// // 		(*Iter).pDevObject->XDeviceDelete();
// // 		(*Iter).pDevObject = NULL;
// // 	}
// 	m_DevList.erase(Iter);
// 
// 	m_cs.Unlock();


	return bResult;
}

// CXDeviceInterface* CDeviceManager::GetDeviceInterfaceByDbid(int dbid)
// {
// 	CXDeviceInterface* p = NULL;
// 
// 	m_cs.Lock();
// 	vector <stDevice>::iterator Iter;
// 	for (Iter = m_DevList.begin(); Iter != m_DevList.end(); Iter++)
// 	{
// 		if ((*Iter).SvrDevInfo.dev_id == dbid)
// 		{
// 			p = (*Iter).pDevObject;
// 			break;
// 		}
// 	}
// 	m_cs.Unlock();
// 
// 	return p;
// }

#endif