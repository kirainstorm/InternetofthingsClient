#ifndef _Stream_h_kjhasdjhkl3i423kjlkj
#define _Stream_h_kjhasdjhkl3i423kjlkj

//----------------------------------------
// kirainstorm
// https://github.com/kirainstorm
//----------------------------------------

#include "PlatformDefine.h"
#include "XStreamInterface.h"
#include "XStreamManager.hpp"
#include "XStreamConnect.hpp"
inline void XNetStartStreamManager(){
	CXStreamManager::Instance().Start();
	//CXSSLStreamManager::Instance().Start();
}
inline void XNetStopStreamManager(){
	CXStreamManager::Instance().Stop();
	//CXSSLStreamManager::Instance().Stop();
}

inline CXNetStream* XNetCreateStream4Connect(const char * szServerAddress, uint32_t lPort, uint32_t dwWaitSecond)
{
	string strServerAddress(szServerAddress);
	char s[256] = { 0 };//ipv6=128?
	CROSS_SPRINTF(s, "%d", lPort);
	string str = s;
	CXNetStreamConnect * p = new CXNetStreamConnect(strServerAddress, s, dwWaitSecond);
	assert(p);
	return p;
}
inline int	XNetConnectStream(CXNetStream * handle)
{
	CXNetStreamConnect * p = (CXNetStreamConnect *)handle;
	return p->ConnectServer();
}

// 
// #ifdef _PLATFORM_WINDOW
// // inline void * XNetCreateListen(CXNetAcceptStreamData * pAccept, uint32_t lPort)
// // {
// // 	CXTcpListener *p = new CXTcpListener(pAccept, lPort);
// // 	p->Start();
// // 	return p;
// // }
// // 
// // inline void XNetDestroyListen(void * handle)
// // {
// // 	CXTcpListener *p = (CXTcpListener *)handle;
// // 	p->Stop();//вти╬ЁЩ
// // }
// 
// inline CXSyncFileStream* XCreateSyncFileStream(HANDLE hfile)
// {
// 	CFileStreamWindowsSyncFile *p = new CFileStreamWindowsSyncFile(hfile);
// 	assert(p);
// 	return p;
// }
// inline CXAsyncFileStream* XCreateAsyncFileStream(HANDLE hfile)
// {
// 	CFileStreamWindowsAsyncFile *p = new CFileStreamWindowsAsyncFile(hfile);
// 	assert(p);
// 	return p;
// }
// #else
// void *	STD_CALL	XNetCreateListen4Android(CXNetAcceptStreamData * pAccept, const char * szLocalAddress, uint32_t lPort, uint32_t timeout_read, uint32_t timeout_write)
// {
// 
// 	std::string address = szLocalAddress;
// 	CXTcpListener *p = new CXTcpListener(pAccept, address, lPort);
// 	p->Start();
// 	return p;
// }
// void	STD_CALL	XNetDestroyListen4Android(void * handle)
// {
// 	CXTcpListener *p = (CXTcpListener *)handle;
// 	p->Stop();
// 	delete p;
// }
// 
// 
// 
// 
// #endif


#endif
