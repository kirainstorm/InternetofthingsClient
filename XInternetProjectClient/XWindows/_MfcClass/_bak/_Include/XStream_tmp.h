#ifndef _Stream_h_kjhasdjhkl3i423kjlkj
#define _Stream_h_kjhasdjhkl3i423kjlkj

#include <stdint.h>
#include "PlatformDefine.h"

#define EVENT_ON_PACKET_COMPLETE_READING	-17000	/* error encountered while reading */
#define EVENT_ON_PACKET_COMPLETE_WRITING	-17001	/* error encountered while writing */
enum emXStreamTansferKey
{
	ENUM_XTSTREAM_TRANSKEY_READ = 0,
	ENUM_XTSTREAM_TRANSKEY_WRITE,
	ENUM_XTSTREAM_TRANSKEY_TIMER
};

class CXNetStreamData
{
public:
	virtual void OnPacketCompleteNetStreamData(int32_t bytesTransferred, emXStreamTansferKey transferKey) = 0;
};
class CXNetStream;
class CXNetAcceptStreamData
{
public:
	virtual void OnAcceptComplete(CXNetStream * pAcceptStream) = 0;
};
class CXNetStream
{
public:
	virtual void Release() = 0;
public:
	virtual void SetStreamData(CXNetStreamData*) = 0;
	virtual void AsyncRead(void *data, int32_t dataLen) = 0;
	virtual int32_t SyncWriteAndRead(const void *writeData, int32_t writeLen, void * readData, int32_t wantReadLen, int32_t waitsec = 3) = 0;
	virtual void AsyncWrite(const void *data, int32_t dataLen) = 0;
	virtual void PostDelayWriteStatus(int32_t milliseconds = 5) = 0;
	virtual void PostDelayTimer(int32_t milliseconds = 200) = 0;
};


//每个进程/dll只能在开始/结束时调用一次，不能多次重复调用
void STD_CALL XNetStartStreamManager(uint32_t n = 8);
void STD_CALL	XNetStopStreamManager();
//创建网络连接对象
CXNetStream*	STD_CALL	XNetCreateStream4Connect(const char * szServerAddress, uint32_t lPort, uint32_t dwWaitSecond = 5);
int	STD_CALL	XNetConnectStream(CXNetStream * handle);


#ifdef _PLATFORM_WINDOW
//创建网络监听对象
//windows 服务器使用
void *	STD_CALL	XNetCreateListen(CXNetAcceptStreamData * pAccept, uint32_t lPort, uint32_t timeout_read = 0, uint32_t timeout_write = 0);//0表示不设置超时
void	STD_CALL	XNetDestroyListen(void * handle);
#endif



#endif
