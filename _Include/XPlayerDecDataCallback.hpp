#pragma once

#ifdef _PLATFORM_WINDOW
#include "XPlayerD3DSurfaceRender.hpp"
#endif

#include "PlatformDefine.h"
#include "XPlayer.hpp"

class CDecDataCallback :public DecodeVideoCallBack
{
public:
	CDecDataCallback()
	{
		m_pCallback = NULL;
		m_pUser = NULL;
#ifdef _PLATFORM_WINDOW
		m_hwnd = NULL;
		m_display = new CD3DSurfaceRender();
		//m_dwTick = 0;
#endif
	};
	~CDecDataCallback()
	{
#ifdef _PLATFORM_WINDOW
		if (m_display)
		{
			delete m_display;
			m_display = NULL;
		}

#endif
	};
public:
	//
	virtual int OnDecodeVideoCallBack(const char* pDecData, int nLen, int nWidth, int nHeight, int nTime, int nTypeFrame = 0, int nOutFormat = 0, int nFrameRate = 0)
	{
#ifdef _PLATFORM_WINDOW

		// 	DWORD64 dwTick = GetTickCount64();
		// 
		// 	if ((dwTick - m_dwTick) < 15*1000)
		// 	{
		// 		Sleep(15);
		// 	}
		// 	else if ((dwTick - m_dwTick) < 25 * 1000)
		// 	{
		// 		Sleep(5);
		// 	}


		//正常的话不能这样写的，因为阻塞了回调
		if (m_display)
		{
			//判断当前是不是最小化，，，why  win10最小化之后d3d的Present非常耗时，，尤其在cpu消耗非常大的情况下
			if (IsIconic(AfxGetApp()->GetMainWnd()->GetSafeHwnd()))
			{
				return 0;
			}

			if (m_display->UpdateFrame(m_hwnd, nWidth, nHeight, pDecData))
			{
				m_display->Render();
			}
		}
#endif

		if (m_pCallback)
		{
			m_pCallback->OnDecodeVideoCallBack(pDecData, nLen, nWidth, nHeight, nTime, nTypeFrame, nOutFormat, nFrameRate);
		}

#ifdef _PLATFORM_WINDOW
		//m_dwTick = dwTick;
#endif
		return 0;
	};

	//设置解码数据回调，上层用户需要
	void SetUserDecVideoDataCallback(DecodeVideoCallBack *pCallback, void * pUser)
	{
		m_pCallback = pCallback;
		m_pUser = pUser;
	};
public:
#ifdef _PLATFORM_WINDOW
	void SetPlayHwnd(HWND hwnd)
	{
		m_hwnd = hwnd;
	};
	HWND m_hwnd;
	CD3DSurfaceRender *m_display;
	//DWORD64 m_dwTick;

#endif
private:
	DecodeVideoCallBack *m_pCallback;
	void * m_pUser;
};

