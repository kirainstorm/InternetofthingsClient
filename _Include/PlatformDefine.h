#ifndef PLATFORM_DEFINE_H_34sdfgsfadf
#define PLATFORM_DEFINE_H_34sdfgsfadf



enum emXInternetType
{
	X_INTERNET_XXX_TYPE_CLASSIC = 0,
	X_INTERNET_XXX_TYPE_IOT,
};

//-------------------------------------------------
//for vs native
//#define _PLATFORM_WINDOW
//for vs-win10-uwp
//#define _PLATFORM_WINDOW_WIN10_UWP
//for android
#define _PLATFORM_ANDROID
//for ioss
//#define _PLATFORM_IOS
//-------------------------------------------------


#include <stdint.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <deque>
#include <time.h>
using namespace std;
//-----------------------------------------
//boost
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/pool/pool.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/pool/pool.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <boost/thread/thread.hpp> 
#include <boost/thread/mutex.hpp> 
#include <boost/exception/all.hpp> 
#ifdef _PLATFORM_WINDOW
#ifdef _WIN64
#include <boost/beast.hpp>
#endif
#endif
//-----------------------------------------
//openssl

#include <openssl/des.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/des.h>
#ifdef _PLATFORM_WINDOW
#ifdef _WIN64
#include <boost/asio/ssl.hpp>
#pragma comment( lib, "libeay32.lib" )
#pragma comment( lib, "ssleay32.lib" )
#else
#pragma comment( lib, "libeay32.lib" )
#pragma comment( lib, "ssleay32.lib" )
#endif
#endif
//-----------------------------------------
//soci //define after boost, because of <windows.h>

#ifdef _PLATFORM_WINDOW
#include "soci/soci.h"
#pragma comment(lib, "libsoci_core_4_0.lib" )
#include "soci/odbc/soci-odbc.h"
#pragma comment(lib, "libsoci_odbc_4_0.lib" )
#endif
//-----------------------------------------


//#define MAX_RECV_BUFFER_SIZE (1*1024*1024)

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
//-------------------------------------------------

enum emNetRecieveStep
{
    NET_RECIEVE_STEP_HEAD = 0,
    NET_RECIEVE_STEP_BODY
};
// enum emLoginStep
// {
// 	LOGIN_STEP_IDLE = 0,//
// 	LOGIN_STEP_HAD_SEND_SESSIONID,
// 	LOGIN_STEP_LOGIN_OK, //
// };
//-------------------------------------------------
inline void _SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#if (defined _PLATFORM_WINDOW) || (defined _PLATFORM_WINDOW_WIN10_UWP)



//error C4996: 'strcpy': This function or variable may be unsafe. Consider using strcpy_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#pragma warning(disable:4996 4200 4101 4099 4244 4267)
//warning C4267: '=': conversion from 'size_t' to 'int', possible loss of data
//warning C4244: '=': conversion from '__int64' to 'int', possible loss of data
//warning C4200: nonstandard extension used : zero-sized array in struct/union
//warning C4996: 'strcpy': This function or variable may be unsafe. Consider using strcpy_s instead
//warning C4101: 'e' : unreferenced local variable//catch (soci::soci_error const & e)
//warning LNK4099: PDB 'lib.pdb' was not found with 'libeay32.lib(x_spki.obj)' or at 'c:\output32\bin64\lib.pdb'; linking object as if no debug info
//#include <windows.h>
#include <process.h>
#include <windows.h>
#include <tchar.h>
#include <atlstr.h>

#include<chrono>//c++ 11 chrono
#include <thread>
#define CROSS_SPRINTF  sprintf_s
#define CROSS_STRCPY strcpy_s
#define CROSS_DWORD64 DWORD64
inline CROSS_DWORD64 CrossGetTickCount64() {
	//GetTickCount64
	//timeBeginPeriod(1)/timeGetTime
	return GetTickCount64();
}
inline void CrossSleep(int nMilliseconds){
	//c++11
	std::this_thread::sleep_for(std::chrono::milliseconds(nMilliseconds));
	//Sleep(nMilliseconds);
}
typedef unsigned int(__stdcall * THREAD_FUN_TYPE)(void *);
#define CROSS_THREAD_HANDLE		intptr_t
#define	CROSS_THREAD_NULL	NULL
inline CROSS_THREAD_HANDLE CrossCreateThread(int(*pThreadFun)(void *), void * pParam)
{
	return _beginthreadex(NULL, 0, (THREAD_FUN_TYPE)pThreadFun, pParam, NULL, NULL);
}
inline void CrossEndThread()
{
	//_endthreadex(0);//
}
inline void CrossCloseThread(CROSS_THREAD_HANDLE hThread)
{
	CloseHandle((HANDLE *)hThread);
}
inline void CrossWaitThread(CROSS_THREAD_HANDLE hThread)
{
	WaitForSingleObject((HANDLE*)hThread, INFINITE);
}

inline void CROSS_TRACE(const char * lpszLog, ...)
{
#ifdef _WINDOWS_
#ifdef _DEBUG
	va_list args;
	va_start(args, lpszLog);

	char szlog[4096] = { 0 };
	vsprintf_s(szlog, lpszLog, args);
	//string strLog(szlog);

	va_end(args);

	::OutputDebugStringA(szlog);
	::OutputDebugStringA("\n");
#endif
#endif

}




inline void Global_ServiceLog_AddLog(const char * lpszLog, ...){
#ifdef _DEBUG

	va_list args;
	va_start(args, lpszLog);

	char szlog[2048] = { 0 };
	vsprintf_s(szlog, lpszLog, args);
	string strLog(szlog);

	va_end(args);

	::OutputDebugStringA(strLog.c_str());
	::OutputDebugStringA("\n");

#endif



};

inline std::string ANSIITOUTF8(std::string str)
{
	WCHAR strSrc[2048] = { 0 };
	char szRes[2048] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, strSrc, 2048);
	WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, szRes, 2048, NULL, NULL);
	std::string result = szRes;
	return result;
}
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifdef _PLATFORM_WINDOW
#include <windows.h>
#else
#include <pthread.h>
#endif
class CrossCriticalSection
{
public:
	CrossCriticalSection()
	{
#ifdef _PLATFORM_WINDOW
		::InitializeCriticalSectionAndSpinCount(&TheCS,4000);
#else
		pthread_mutex_init(&TheMutex, NULL);
#endif
	}
	~CrossCriticalSection()
	{
#ifdef _PLATFORM_WINDOW
		::DeleteCriticalSection(&TheCS);
#else
		pthread_mutex_destroy(&TheMutex);
#endif
	}
	/// Enter the critical section -- other threads are locked out
	void Lock() const
	{
#ifdef _PLATFORM_WINDOW
		::EnterCriticalSection(&TheCS);
#else
		pthread_mutex_lock(&TheMutex);
#endif
	}
	/// Leave the critical section
	void Unlock() const
	{
#ifdef _PLATFORM_WINDOW
		::LeaveCriticalSection(&TheCS);
#else
		pthread_mutex_unlock(&TheMutex);
#endif
	}
protected:
#ifdef _PLATFORM_WINDOW
	mutable CRITICAL_SECTION TheCS;
#else
	mutable pthread_mutex_t TheMutex;// POSIX threads
#endif
};


#ifdef _PLATFORM_WINDOW
#include <direct.h>
#include <io.h>
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#endif

#if (defined _PLATFORM_ANDROID) || (defined _PLATFORM_IOS)
#include <stdarg.h>
#include <sys/stat.h>
#define ACCESS access
#define MKDIR(a) mkdir((a),0770)
#endif
inline int CreatDir(char *pszDir)
{
	int i = 0;
	int iRet;
	int iLen = (int)strlen(pszDir);

	//
	if (pszDir[iLen - 1] != '\\' && pszDir[iLen - 1] != '/')
	{
		pszDir[iLen] = '/';
		pszDir[iLen + 1] = '\0';
	}

	//
	for (i = 1; i <= iLen; i++)//for (i = 0; i <= iLen; i++)
	{
		if (pszDir[i] == '\\' || pszDir[i] == '/')
		{
			pszDir[i] = '\0';

			//
			iRet = ACCESS(pszDir, 0);
			if (iRet != 0)
			{
				iRet = MKDIR(pszDir);
				if (iRet != 0)
				{
					return -1;
				}
			}
			//
			pszDir[i] = '/';
		}
	}
	return 0;
}



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


#endif



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifdef _PLATFORM_ANDROID



#define CROSS_SPRINTF  sprintf
#define CROSS_STRCPY strcpy
#define CROSS_DWORD64 long
inline CROSS_DWORD64 CrossGetTickCount64() {
	struct timeval tvin;
	gettimeofday(&tvin, NULL);
	CROSS_DWORD64 ttick = (CROSS_DWORD64)((long)tvin.tv_sec) * 1000 + (long)tvin.tv_usec / 1000;
	return ttick;
}
inline void CrossSleep(int nMilliseconds){
	usleep(nMilliseconds * 1000);
}


// #define     THREAD_MUTEX             pthread_mutex_t
// #define     INITIALIZE_MUTEX(mutex)  pthread_mutex_init(mutex, NULL)   
// #define     DESTROY_MUTEX            pthread_mutex_destroy
// #define		LOCK_MUTEX               pthread_mutex_lock
// #define     UNLOCK_MUTEX             pthread_mutex_unlock

typedef void *(*THREAD_FUN_TYPE)(void *);
#define	CROSS_THREAD_HANDLE pthread_t
#define	CROSS_THREAD_NULL	0
inline CROSS_THREAD_HANDLE CrossCreateThread(int(*pThreadFun)(void *), void * pParam)
{
	pthread_t threadID;
	pthread_create(&threadID, NULL, (THREAD_FUN_TYPE)pThreadFun, pParam);
	return threadID;
}
inline void CrossEndThread()
{
	pthread_exit(0);
}
inline void CrossCloseThread(CROSS_THREAD_HANDLE hThread)
{

}
inline void CrossWaitThread(CROSS_THREAD_HANDLE hThread)
{
	void * retval;
	pthread_join(hThread, &retval);
}


#include <jni.h>
#include <android/log.h>
#define LOG_TAG "utilityCode"
#define Log(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

#define CROSS_TRACE LOGD

#define CROSS_SOCKET int
#define CROSS_ClOSESOCKET close
#ifndef INVALID_SOCKET
#define INVALID_SOCKET  (-1)
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR  (-1)
#endif

#endif
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifdef _PLATFORM_IOS



#define CROSS_SPRINTF  sprintf
#define CROSS_STRCPY strcpy
#define CROSS_DWORD64 long
inline CROSS_DWORD64 CrossGetTickCount64() {
	struct timeval tvin;
	gettimeofday(&tvin, NULL);
	CROSS_DWORD64 ttick = (CROSS_DWORD64)((long)tvin.tv_sec) * 1000 + (long)tvin.tv_usec / 1000;
	return ttick;
}
inline void CrossSleep(int nMilliseconds){
	usleep(nMilliseconds * 1000);
}


// #define     THREAD_MUTEX             pthread_mutex_t
// #define     INITIALIZE_MUTEX(mutex)  pthread_mutex_init(mutex, NULL)
// #define     DESTROY_MUTEX            pthread_mutex_destroy
// #define		LOCK_MUTEX               pthread_mutex_lock
// #define     UNLOCK_MUTEX             pthread_mutex_unlock

typedef void *(*THREAD_FUN_TYPE)(void *);
#define	CROSS_THREAD_HANDLE pthread_t
#define	CROSS_THREAD_NULL	0
inline CROSS_THREAD_HANDLE CrossCreateThread(int(*pThreadFun)(void *), void * pParam)
{
	pthread_t threadID;
	pthread_create(&threadID, NULL, (THREAD_FUN_TYPE)pThreadFun, pParam);
	return threadID;
}
inline void CrossEndThread()
{
	pthread_exit(0);
}
inline void CrossCloseThread(CROSS_THREAD_HANDLE hThread)
{

}
inline void CrossWaitThread(CROSS_THREAD_HANDLE hThread)
{
	void * retval;
	pthread_join(hThread, &retval);
}


//#include <jni.h>
//#include <android/log.h>
//#define LOG_TAG "utilityCode"
//#define Log(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
//#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
//#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

#define CROSS_TRACE printf

#define CROSS_SOCKET int
#define CROSS_ClOSESOCKET close
#ifndef INVALID_SOCKET
#define INVALID_SOCKET  (-1)
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR  (-1)
#endif

#endif


#endif
