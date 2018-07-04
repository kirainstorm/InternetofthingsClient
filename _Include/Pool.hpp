#pragma once

#include "PlatformDefine.h"
#include "XStream.hpp"

//--------------------------------------------------------------------------------------------------------
 struct pool_tag {};//仅仅用于标记的空类
//
// boost::singleton_pool在vld下泄露。。。汗
//
// --------------------------------------------------------------------------------------------------------

#define  EXTRA_SMALL_BUFFER_SIZE_B 256
 struct ST_EXTRA_SMALL_BUFFER_SIZE_B
 {
	 char buffer[EXTRA_SMALL_BUFFER_SIZE_B];
 };
 class CExtreSmallBufferPool
 {
 public:
	 CExtreSmallBufferPool() :m_pool(sizeof(ST_EXTRA_SMALL_BUFFER_SIZE_B))
	 {};
	 ~CExtreSmallBufferPool() {};
	 static CExtreSmallBufferPool& Instance()
	 {
		 static CExtreSmallBufferPool inc;
		 return inc;
	 };
	 char * malloc()
	 {
		 m_cs.Lock();
		 char * pBuffer = (char *)(ST_EXTRA_SMALL_BUFFER_SIZE_B *)m_pool.malloc();
		 assert(pBuffer);
		 memset(pBuffer, 0, EXTRA_SMALL_BUFFER_SIZE_B);
		 m_cs.Unlock();
		 return pBuffer;
	 }
	 void free(char * p)
	 {
		 m_cs.Lock();
		 m_pool.free((ST_EXTRA_SMALL_BUFFER_SIZE_B *)p);
		 m_cs.Unlock();
	 }
 private:
	 boost::pool<> m_pool;
	 CrossCriticalSection m_cs;
 };




#define LITTLE_BUFFER_SIZE_KB (4*1024)//请不要少于4KB,因为音频也用了这个 //这里可以改成改成(80*1024*1024)，测试泄露
struct ST_LITTLE_BUFFER_KB
{
	char buffer[LITTLE_BUFFER_SIZE_KB];
};

class CLittleBufferPool
{
public:
	CLittleBufferPool() :m_pool(sizeof(ST_LITTLE_BUFFER_KB)) 
	{};
	~CLittleBufferPool() {};
	static CLittleBufferPool& Instance()
	{
		static CLittleBufferPool inc;
		return inc;
	};
	char * malloc()
	{
		m_cs.Lock();
		char * pBuffer = (char *)(ST_LITTLE_BUFFER_KB *)m_pool.malloc();
		assert(pBuffer);
		//CROSS_TRACE("---### %I64d ", pBuffer);
		memset(pBuffer, 0, LITTLE_BUFFER_SIZE_KB);
		m_cs.Unlock();
		return pBuffer;
	}
	void free(char * p)
	{
		m_cs.Lock();
		//CROSS_TRACE("===### %I64d", p);
		m_pool.free((ST_LITTLE_BUFFER_KB *)p);
		m_cs.Unlock();
	}
private:
	boost::pool<> m_pool;
	CrossCriticalSection m_cs;
	//typedef boost::singleton_pool<pool_tag, sizeof(ST_LITTLE_BUFFER_KB)> spl_little_buffer_KB;
};


// typedef boost::singleton_pool<pool_tag, sizeof(ST_LITTLE_BUFFER_KB)> spl_little_buffer_KB;
// inline char * singleton_pool_malloc_little_buffer()
// {
// 	return ((ST_LITTLE_BUFFER_KB *)spl_little_buffer_KB::malloc())->buffer;
// };
// inline void singleton_pool_free_little_buffer(char *pbuffer)
// {
// 	spl_little_buffer_KB::free((ST_LITTLE_BUFFER_KB *)pbuffer);
// };
// --------------------------------------------------------------------------------------------------------
#define MIDDLE_BUFFER_SIZE_MB (1*1024*1024)//这里可以改成改成(80*1024*1024)，测试泄露
struct ST_MIDDLE_BUFFER_MB
{
	char buffer[MIDDLE_BUFFER_SIZE_MB];
};
class CMiddleBufferPool
{
public:
	CMiddleBufferPool() :m_pool(sizeof(ST_MIDDLE_BUFFER_MB)) {};
	~CMiddleBufferPool() {};
	static CMiddleBufferPool& Instance()
	{
		static CMiddleBufferPool inc;
		return inc;
	};
	char * malloc()
	{
		m_cs.Lock();
		char * pBuffer = (char *)((ST_MIDDLE_BUFFER_MB *)m_pool.malloc());
		assert(pBuffer);
		memset(pBuffer, 0, MIDDLE_BUFFER_SIZE_MB);
		m_cs.Unlock();
		return pBuffer;
	}
	void free(char * p)
	{
		m_cs.Lock();
		m_pool.free((ST_MIDDLE_BUFFER_MB *)p);
		m_cs.Unlock();
	}
private:
	boost::pool<> m_pool;
	CrossCriticalSection m_cs;
};


// struct ST_MIDDLE_BUFFER_MB
// {
// 	char buffer[MIDDLE_BUFFER_SIZE_MB];
// };
// typedef boost::singleton_pool<pool_tag, sizeof(ST_MIDDLE_BUFFER_MB)> spl_middle_buffer_MB;
// inline char * singleton_pool_malloc_middle_buffer()
// {
// 	return ((ST_MIDDLE_BUFFER_MB *)spl_middle_buffer_MB::malloc())->buffer;
// };
// inline void singleton_pool_free_middle_buffer(char *pbuffer)
// {
// 	spl_middle_buffer_MB::free((ST_MIDDLE_BUFFER_MB *)pbuffer);
// };
// --------------------------------------------------------------------------------------------------------



