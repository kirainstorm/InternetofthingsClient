#ifndef x_cross_h_kj324l78q234nd
#define x_cross_h_kj324l78q234nd

#include <stdint.h>
#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;

#include "PlatformDefine.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#if (defined _PLATFORM_WINDOW) || (defined _PLATFORM_WINDOW_WIN10_UWP)
#include <windows.h>
#include <process.h>

#define CROSS_SPRINTF  sprintf_s
#define CROSS_STRCPY strcpy_s
#define CROSS_DWORD64 DWORD64
inline CROSS_DWORD64 CrossGetTickCount64() {
	//GetTickCount64有15ms的误差，高精度请用timeBeginPeriod(1)/timeGetTime
	return GetTickCount64();
}
inline void CrossSleep(int nMilliseconds){
	Sleep(nMilliseconds);
}


// #define       THREAD_MUTEX             CRITICAL_SECTION
// #define       INITIALIZE_MUTEX(mutex)  InitializeCriticalSection(mutex)
// #define       DESTROY_MUTEX            DeleteCriticalSection
// #define       LOCK_MUTEX               EnterCriticalSection
// #define       UNLOCK_MUTEX             LeaveCriticalSection


typedef unsigned int(__stdcall * THREAD_FUN_TYPE)(void *);
#define CROSS_THREAD_HANDLE		intptr_t
#define	CROSS_THREAD_NULL	NULL
inline CROSS_THREAD_HANDLE CrossCreateThread(int(*pThreadFun)(void *), void * pParam)
{
	return _beginthreadex(NULL, 0, (THREAD_FUN_TYPE)pThreadFun, pParam, NULL, NULL);
}
inline void CrossEndThread()
{
	_endthreadex(0);
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

	char szlog[512] = { 0 };
	vsprintf_s(szlog, lpszLog, args);
	//string strLog(szlog);

	va_end(args);

	::OutputDebugStringA(szlog);
	::OutputDebugStringA("\n");
#endif
#endif

}



#define CROSS_SOCKET SOCKET
#define CROSS_ClOSESOCKET closesocket
#ifndef INVALID_SOCKET
#define INVALID_SOCKET  (SOCKET)(~0)
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR  (-1)
#endif



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
	//函数pthread_join是一个线程阻塞的函数，调用它的函数将一直等待到被等待的线程结束为止，
	//当函数返回时，被等待线程的资源被收回,即线程执行完毕
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
    //函数pthread_join是一个线程阻塞的函数，调用它的函数将一直等待到被等待的线程结束为止，
    //当函数返回时，被等待线程的资源被收回,即线程执行完毕
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
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)\
{\
	if((p) != NULL)\
					{\
		delete (p) ;\
		(p) = NULL ;\
					}\
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
		::InitializeCriticalSection(&TheCS);
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
		if (!::TryEnterCriticalSection(&TheCS)) ::EnterCriticalSection(&TheCS);
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
	// POSIX threads
	mutable pthread_mutex_t TheMutex;
#endif
};



#endif /* x_cross_h_kj324l78q234nd */
