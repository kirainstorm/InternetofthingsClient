#ifndef _Stream_interfaceh_sdfwewcsa1233erfewe2fvxzw
#define _Stream_interfaceh_sdfwewcsa1233erfewe2fvxzw

//----------------------------------------
// kirainstorm
// https://github.com/kirainstorm
//----------------------------------------

#include <stdint.h>
#include "PlatformDefine.h"
//#include<chrono>//c++ 11 chrono
//std::this_thread::sleep_for(std::chrono::seconds(3)); //��������  
//std::this_thread::sleep_for(std::chrono::milliseconds(100)); //����100����  

//#include <stdint.h>
//c99 :char, int8_t,int16_t,int32_t,int64_t,uint8_t,uint16_t,uint32_t,uint64_t
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void trace_catch_error_code(const char * __FILE, int __LINE, const char * __FUNCTION, const char * _error)
{
	//CROSS_TRACE("%s(%d)-%s: %s", __FILE__, __LINE__, __FUNCTION__, _error);
	CROSS_TRACE("%s(%d)-%s: %s", __FILE, __LINE, __FUNCTION, _error);
}



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
	//Operation has completed --> Send completed / Read completed
	//must call "AsyncRead" "AsyncWrite" "PostDelayWriteStatus" here -!!!!!-----------------Very important------------
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
	//Call this function to delete the network examples
	//Can only be called once
	//Will stop calling "CXNetStreamData->OnPacketCompleteNetStreamData()"
	//when call "Release()" , you can't use this "CXNetStream" anymore
	virtual void Release() = 0; //���ô˺�����OnPacketCompleteNetStreamData�������ٻص�
public:
	//syncRead/AsyncWrite/PostDelayWriteStatusֻ�����û���OnPacketCompleteNetStreamData�е���


	//sync-write , only for "CConnectStream"
	//befor "SetStreamData" and "AsyncWrite"
	//Not recommended for users
	//error:-1 ; suss:0
	//only send:dataLen>0 & wantReadLen=0
	//only read:dataLen=0 & wantReadLen>0
	//send&read:dataLen>0 & wantReadLen>0
	//error: -1 ,when return -1,donot use again!!!not thread safe
	virtual int32_t SyncWriteAndRead(const void *writeData, int32_t writeLen, void * readData, int32_t wantReadLen, int32_t waitsec = 3) = 0;


	//virtual CXNetStreamData* GetStreamData() = 0;
	//Will call "CXNetStreamData->OnPacketCompleteNetStreamData(0,0)/(0,1)"
	//only call once
	virtual void SetStreamData(CXNetStreamData*) = 0;
	//Async-Read
	//NEVER start your second Read before the first has completed.!!!!-----------Very important------------
	virtual void AsyncRead(void *data, int32_t dataLen) = 0;
	//Async-Write
	//NEVER start your second Write before the first has completed.!!!!-----------Very important------------
	//
	//This function is used to asynchronously write a certain number of bytes of data to a stream.
	//The function call always returns immediately.The asynchronous operation will continue until one of the following conditions is true:
	//1:All of the data in the supplied basic_streambuf has been written.
	//2:An error occurred.
	//This operation is implemented in terms of zero or more calls to the stream's async_write_some function, and is known as a composed operation. 
	//The program must ensure that the stream performs no other write operations (such as async_write, the stream's async_write_some function, 
	//or any other composed operations that perform writes) until this operation completes.
	virtual void AsyncWrite(const void *data, int32_t dataLen) = 0;
	//Send "write" delay callback 
	//Will call "CXNetStreamData->OnPacketCompleteNetStreamData(0,0)
	virtual void PostDelayWriteStatus(int32_t milliseconds = 15) = 0;
	//for check
	virtual void PostDelayTimer(int32_t milliseconds = 200) = 0;
	//
};

class CXSyncFileStream //ͬ��
{
public:
	virtual void Release() = 0;
public:
	//success: ret == 0
	virtual void SyncRead(int32_t offset, char * buffer, int32_t dataLen, int32_t & ret) = 0;
	virtual void SyncWrite(int32_t offset, const char * buffer, int32_t dataLen, int32_t & ret) = 0;
};

class CXAsyncFileStreamData//�첽
{
public:
	//Called when delete class:"CXFileStream"
	//���������˵��û����ڴ˺�����ִ�к�ʱ�Ĳ���
	//virtual void DeleteFileData() = 0;
	//Operation has completed
	//Write completed == 0 ------ Read completed > 0 
	virtual void OnPacketCompleteFileStreamData(int32_t bytesTransferred, emXStreamTansferKey transferKey) = 0;
};
class CXAsyncFileStream//�첽
{
public:
	//Call this function to delete the file examples
	virtual void Release() = 0;
public:
	//virtual CXFileStreamData* GetStreamData() = 0;
	virtual void SetStreamData(CXAsyncFileStreamData*) = 0;
	//Send an asynchronous read request
	//NEVER start your second read before the first has completed.!!!!-----------Very important------------
	virtual void AsyncRead(int32_t offset, char * buffer, int32_t dataLen) = 0;
	//Will call "CXFileStreamData->OnPacketCompleteFileStreamData(0,1)
	virtual void PostDelayReadStatus(int32_t milliseconds = 5) = 0;
	//Send an asynchronous write request
	//NEVER start your second Write before the first has completed.!!!!-----------Very important------------
	//NEVER free "data" until "OnPacketCompleteNetStreamData"!!!!-----------Very important------------
	virtual void AsyncWrite(int32_t offset, const void *data, int32_t dataLen) = 0;
	//Will call "CXFileStreamData->OnPacketCompleteFileStreamData(0,0)
	virtual void PostDelayWriteStatus(int32_t milliseconds = 15) = 0;
};



#endif
