#pragma once

#include "PlatformDefine.h"
#ifdef _PLATFORM_WINDOW

#include <d3d9.h> 
#pragma comment(lib, "d3d9.lib")


//ffmpeg
#ifdef __cplusplus
extern "C" {
#endif
	/*Include ffmpeg header file*/
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif

#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "swscale.lib")


enum { BUFFER_ALIGN = 32 };


//BGRA data  
#define LOAD_BGRA    0
//YUV420P data  
#define LOAD_YUV420P 1  

class CD3DSurfaceRender
{
public:
	CD3DSurfaceRender();
	~CD3DSurfaceRender();

public:
	BOOL UpdateFrame(HWND hWnd,int width, int height, const char * pszBuffer);
	void Render();
private:
	BOOL m_bRenderSuccess;//是否render成功不成功，需要重新创建设备
	

private:
	//void Release();
	//DWORD64 m_dwDrawTick;

	//BOOL CapturePicture(const char *pFileName);
	//BOOL CapturePictureBuf(char *pBuf);

// 	void GetPicSize(int *pWidth, int *pHeight){
// 		*pWidth = (int)m_dwWidth;
// 		*pHeight = (int)m_dwHeight;
// 	};
// 
	void SetAdjustRect(LPRECT lpRect){
		CopyRect(&m_rcAdjust, lpRect);
		m_bClip = TRUE;
		//m_bAdjustChanged = TRUE;
	};


	/*
	BOOL SetOsd(int index, const osd_info_t& osd_info){
	if (index >= MAX_SET_OSD_NUM)
	return FALSE;
	memcpy_s(&m_osd_info[index], sizeof(osd_info_t), &osd_info, sizeof(osd_info_t));
	m_bOsd[index] = TRUE;

	return TRUE;
	};
	BOOL RemoveOsd(int index){
	if (index >= MAX_SET_OSD_NUM)
	return FALSE;
	m_bOsd[index] = FALSE;
	return TRUE;
	};
	*/



	void SetMatchToRes(BOOL bMatch){
		m_bMatchToRes = bMatch;
		//m_bAdjustChanged = TRUE;
	};


	//void SetRenderEffect(int type){ m_iRenderEffect = type; };

private:
	//void Tidy();
	void ReleaseD3D();
	BOOL CreateSurfaceRender(HWND hWnd,DWORD dwWidth, DWORD dwHeight);
	D3DPRESENT_PARAMETERS m_d3dpp;
#if LOAD_BGRA
	unsigned char* m_pBGR32;
#endif
public:
	void DrawOSD(HDC& dcMem, int pos, CString strTemp, RECT rc, RECT osd_rect, int osd_color_red, int osd_color_green, int osd_color_blue);


private:
	CrossCriticalSection m_critial;

	IDirect3D9 *m_pDirect3D9;
	IDirect3DDevice9 *m_pDirect3DDevice;
	IDirect3DSurface9 *m_pDirect3DSurfaceRender;
	IDirect3DSurface9 * m_pBackBuffer;

	HWND m_hWnd;
	RECT m_rcWnd;

	DWORD m_dwVideoWidth;
	DWORD m_dwVideoHeight;

 	BOOL m_bMatchToRes;//按图像原始大小比例显示图像
	BOOL m_bClip;//图像剪裁
	RECT m_rcAdjust;

	//BOOL m_bAdjustChanged;
	int m_iRenderEffect;

private:
	bool YV12ToBGR32_FFmpeg(unsigned char* pYUV, unsigned char* pBGR24, int width, int height);

};

#endif