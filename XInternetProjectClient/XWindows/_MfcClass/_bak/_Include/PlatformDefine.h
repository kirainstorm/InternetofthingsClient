#ifndef _PLATFORM_DEFINE_H_34sdfgsfadf
#define _PLATFORM_DEFINE_H_34sdfgsfadf





enum emNetRecieveStep
{
    NET_RECIEVE_STEP_HEAD = 0,
    NET_RECIEVE_STEP_BODY
};

enum REMOTESESSIONSTEP
{
	NET_SEND_STEP_MSG = 0,
	NET_SEND_STEP_MSG_OK,
};


//#define MAX_FRAME_SIZE		3112960//((1920*1080*3)/2=3110400   + 2560))  //4k
 


//-------------------------------------------------
//for vs native
#define _PLATFORM_WINDOW
//for vs-win10-uwp
//#define _PLATFORM_WINDOW_WIN10_UWP
//for android
//#define _PLATFORM_ANDROID
//for ioss
//#define _PLATFORM_IOS
//-------------------------------------------------


#ifdef _PLATFORM_WINDOW
#define STD_CALL _stdcall
#endif

#ifdef _PLATFORM_WINDOW_WIN10_UWP
#define STD_CALL _stdcall
#endif

#ifdef _PLATFORM_ANDROID
#define STD_CALL
#endif

#ifdef _PLATFORM_IOS
#define STD_CALL
#endif

#ifdef _PLATFORM_WINDOW
//#include <dbghelp.h>
//#pragma comment(lib, "Dbghelp.lib")
//
// void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
// {
// 	//  
// 	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//
// 	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
// 	dumpInfo.ExceptionPointers = pException;
// 	dumpInfo.ThreadId = GetCurrentThreadId();
// 	dumpInfo.ClientPointers = TRUE;
//
// 	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
// 
// 	CloseHandle(hDumpFile);
// }
// LONG WINAPI ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
// {
// 	CreateDumpFile(_T("C:\\Test.dmp"), pException);
// 	return EXCEPTION_EXECUTE_HANDLER;
// }
//-----
//
// SetUnhandledExceptionFilter(ApplicationCrashHandler);
#endif





#ifdef _PLATFORM_IOS
#define MAX_PATH 260

#ifndef HANDLE
//#define HANDLE (void *)
typedef void* HANDLE;
#endif


#ifndef FALSE
#define FALSE 0
#endif // !FALSE

#ifndef TRUE
#define TRUE 1
#endif // !TRUE

#ifndef NULL
#define NULL 0
#endif // !NULL

#ifndef nullptr
#define nullptr 0
#endif // !nullptr


#ifndef BYTE
typedef unsigned char       BYTE;
#endif // !BYTE

#ifndef LONG
typedef long                LONG;
#endif // !LONG

#ifndef ULONG
typedef unsigned long       ULONG;
#endif // !ULONG

#ifndef WORD
typedef unsigned short WORD;
#endif // !DWORD

#ifndef DWORD
typedef unsigned long       DWORD;
#endif // !DWORD

#ifndef LONG32
typedef signed int LONG32;
#endif // !LONG32


//boost
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>


#define READ_WRITE_BUFFER_SIZE (8*1024*1024)

//
#include <objc/objc.h>
//

#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h> //usleep

#include <pthread.h>
#include <memory.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <sys/time.h>



//#include "FrameData.h"
//#include "DevInterface.h"
#include "XStream.h"
#include "XCross.h"

#endif



#ifdef _PLATFORM_ANDROID

//android7
//
// typedefs
//
#define MAX_PATH 260

#ifndef HANDLE
//#define HANDLE (void *)
typedef void* HANDLE;
#endif


#ifndef FALSE
#define FALSE 0
#endif // !FALSE

#ifndef TRUE
#define TRUE 1
#endif // !TRUE

#ifndef NULL
#define NULL 0
#endif // !NULL

#ifndef nullptr
#define nullptr 0
#endif // !nullptr

#ifndef BOOL
#define BOOL int
#endif // !BOOL


#ifndef BYTE
typedef unsigned char       BYTE;
#endif // !BYTE

#ifndef LONG
typedef long                LONG;
#endif // !LONG

#ifndef ULONG
typedef unsigned long       ULONG;
#endif // !ULONG

#ifndef WORD
typedef unsigned short WORD;
#endif // !DWORD

#ifndef DWORD
typedef unsigned long       DWORD;
#endif // !DWORD

#ifndef LONG32
typedef signed int LONG32;
#endif // !LONG32


//boost
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>


#define READ_WRITE_BUFFER_SIZE (8*1024*1024)

#include <sys/time.h> 
#include <time.h>
#include <unistd.h> //usleep
#include "XCross.h"

#endif






#endif
