#pragma once

#ifdef _PLATFORM_WINDOW
#include "D3DSurfaceRender.h"
#endif

#include "PlatformDefine.h"
#include "XPlayer.h"

class CDecDataCallback :public DecodeVideoCallBack
{
public:
	CDecDataCallback();
	~CDecDataCallback();


public:
	//
	virtual int OnDecodeVideoCallBack(const char* pDecData, int nLen, int nWidth, int nHeight, int nTime, int nTypeFrame = 0, int nOutFormat = 0, int nFrameRate = 0);


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

