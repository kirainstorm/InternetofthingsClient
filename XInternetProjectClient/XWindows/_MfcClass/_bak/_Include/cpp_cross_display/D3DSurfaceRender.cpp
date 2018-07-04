#include "stdafx.h"
#include "D3DSurfaceRender.h"


#ifdef _PLATFORM_WINDOW



CD3DSurfaceRender::CD3DSurfaceRender()
{
	//
	m_pDirect3D9 = NULL;
	m_pDirect3DDevice = NULL;
	m_pDirect3DSurfaceRender = NULL;
	//
	m_bMatchToRes = FALSE;
	m_bClip = FALSE;
	//
	m_hWnd = NULL;
	m_dwVideoWidth = 0;
	m_dwVideoHeight = 0;
	//
#if LOAD_BGRA
	int numBytes = av_image_get_buffer_size(AV_PIX_FMT_BGR32, 1920, 1080, BUFFER_ALIGN)*sizeof(uint8_t);
	m_pBGR32 = new unsigned char[numBytes];
#endif

	m_bRenderSuccess = TRUE;
	
}


CD3DSurfaceRender::~CD3DSurfaceRender()
{
	ReleaseD3D();
#if LOAD_BGRA
	delete[] m_pBGR32;
#endif
}

void CD3DSurfaceRender::ReleaseD3D()
{
	//请确保所有的返回值为 S_OK
	HRESULT lRet;
	m_critial.Lock();
	if (m_pDirect3DSurfaceRender)
	{
		lRet = m_pDirect3DSurfaceRender->Release();
		m_pDirect3DSurfaceRender = NULL;
	}
	
	if (m_pBackBuffer)
	{
		lRet = m_pBackBuffer->Release();
		m_pBackBuffer = NULL;
	}

	if (m_pDirect3DDevice)
	{
		lRet = m_pDirect3DDevice->Reset(&m_d3dpp);
		lRet = m_pDirect3DDevice->Release();
		m_pDirect3DDevice = NULL;
	}
	if (m_pDirect3D9)
	{
		lRet = m_pDirect3D9->Release();
		m_pDirect3D9 = NULL;
	}
	m_critial.Unlock();
}



BOOL CD3DSurfaceRender::UpdateFrame(HWND hWnd,int width, int height, const char * pszBuffer)
{
#if 0
	unsigned char  *pY = (unsigned char  *)pszBuffer;
	unsigned char  *pU = (unsigned char  *)pszBuffer + (width*height);
	unsigned char  *pV = (unsigned char  *)pszBuffer + (width*height * 5 / 4);
#endif

	if (width <= 0 || height <= 0)
	{
		return FALSE;
	}

	m_hWnd = hWnd;

	if (!(::IsWindowVisible(m_hWnd)))
	{
		return FALSE;
	}

	if (!CreateSurfaceRender(hWnd,width, height))
	{
		//TRACE(" ###>>> Create DDraw failed!\n");
		return FALSE;
	}

	if (m_pDirect3D9 == NULL)
		return FALSE;
	if (m_pDirect3DSurfaceRender == NULL)
		return FALSE;
	if (m_pDirect3DDevice == NULL)
		return FALSE;
	if (m_pBackBuffer == NULL)
		return FALSE;






	HRESULT lRet;
	int pixel_w = width, pixel_h = height;

	D3DLOCKED_RECT d3d_rect;
	RECT rsc;
	rsc.left = 0;
	rsc.top = 0;
	rsc.right = width;
	rsc.bottom = height;

	lRet = m_pDirect3DSurfaceRender->LockRect(&d3d_rect, &rsc, 0/*D3DLOCK_DONOTWAIT*/);
	if (FAILED(lRet))
		return FALSE;

	
#if LOAD_BGRA

	byte *pSrc = (byte *)m_pBGR32;
	byte * pDest = (BYTE *)d3d_rect.pBits;
	int stride = d3d_rect.Pitch;
	int i = 0;

	//Copy Data  
	if (YV12ToBGR32_FFmpeg((unsigned char *)pszBuffer, m_pBGR32, width, height))
	{
		int pixel_w_size = pixel_w * 4;
		for (i = 0; i < pixel_h; i++){
			memcpy(pDest, pSrc, pixel_w_size);
			pDest += stride;
			pSrc += pixel_w_size;
		}
	}
	else
	{
		;
	}
#endif

#if LOAD_YUV420P

	byte *pSrc = (byte *)pszBuffer;
	byte * pDest = (BYTE *)d3d_rect.pBits;
	int stride = d3d_rect.Pitch;
	int i = 0;

	for (i = 0; i < pixel_h; i++){
		memcpy(pDest + i * stride, pSrc + i * pixel_w, pixel_w);
	}
	for (i = 0; i < pixel_h / 2; i++){
		memcpy(pDest + stride * pixel_h + i * stride / 2, pSrc + pixel_w * pixel_h + pixel_w * pixel_h / 4 + i * pixel_w / 2, pixel_w / 2);
	}
	for (i = 0; i < pixel_h / 2; i++){
		memcpy(pDest + stride * pixel_h + stride * pixel_h / 4 + i * stride / 2, pSrc + pixel_w * pixel_h + i * pixel_w / 2, pixel_w / 2);
	}
#endif  

	lRet = m_pDirect3DSurfaceRender->UnlockRect();

	//TRACE("UpdateFrame use time :   %d \n", CrossGetTickCount64() - dw);

	if (FAILED(lRet))
		return FALSE;

	return TRUE;
}
void CD3DSurfaceRender::Render()
{

	DWORD64 dwdw = CrossGetTickCount64();
	



// 		if (m_bCapture)
// 		{
// 			DDSURFACEDESC2  ddsd;
// 			ZeroMemory(&ddsd, sizeof(ddsd));
// 			ddsd.dwSize = sizeof(ddsd);
// 
// 			if (FAILED(pddsBackBuffer->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL)))
// 			{
// 				ATLTRACE(" ###>>> Render Lock Failed\n");
// 				Release();
// 				return;
// 
// 			}
// 
// 			InternalCapture(&ddsd);
// 
// 			pddsBackBuffer->Unlock(NULL);
// 		}

// 		m_rcAdjust.left = 100;
// 		m_rcAdjust.right = 0;
// 		m_rcAdjust.top = 0;
// 		m_rcAdjust.bottom = 0;

		RECT rcSourceRect;
		SetRect(&rcSourceRect, 0, 0, m_dwVideoWidth, m_dwVideoHeight);
//  		LONG lWidth = m_dwVideoWidth;
//  		LONG lHeight = m_dwVideoHeight;
//  		if (m_bClip)
//  		{
// 			if (m_rcAdjust.left > 0 && m_rcAdjust.left < lWidth)
// 			{
// 				lWidth -= m_rcAdjust.left;
// 				rcSourceRect.left += m_rcAdjust.left;
// 			}
// 			if (m_rcAdjust.right > 0 && m_rcAdjust.right < lWidth)
// 			{
// 				lWidth -= m_rcAdjust.right;
// 				rcSourceRect.right -= m_rcAdjust.right;
// 			}
// 			if (m_rcAdjust.top > 0 && m_rcAdjust.top < lHeight)
// 			{
// 				lHeight -= m_rcAdjust.top;
// 				rcSourceRect.top += m_rcAdjust.top;
// 			}
// 			if (m_rcAdjust.bottom > 0 && m_rcAdjust.bottom < lHeight)
// 			{
// 				lHeight -= m_rcAdjust.bottom;
// 				rcSourceRect.bottom -= m_rcAdjust.bottom;
// 			}
//  		}




		RECT rcDestRect;
		GetClientRect(m_hWnd, &rcDestRect);
 		
// 		if (m_bMatchToRes)
// 		{
// 		LONG lWndWidth = rcDestRect.right - rcDestRect.left;
// 		LONG lWndHeight = rcDestRect.bottom - rcDestRect.top;
// 			LONG lWidthWndHeight = lWidth * lWndHeight;
// 			LONG lHeightWndWidth = lHeight * lWndWidth;
// 			if (lWidthWndHeight > lHeightWndWidth)
// 			{
// 				LONG lMatchWndHeight = lHeightWndWidth / lWidth;
// 				LONG lAdjust = (lWndHeight - lMatchWndHeight) / 2;
// 				rcDestRect.top += lAdjust;
// 				rcDestRect.bottom -= lAdjust;
// 			}
// 			else
// 			{
// 				LONG lMatchWndWidth = lWidthWndHeight / lHeight;
// 				LONG lAdjust = (lWndWidth - lMatchWndWidth) / 2;
// 				rcDestRect.left += lAdjust;
// 				rcDestRect.right -= lAdjust;
// 			}
// 		}



		/*

		switch (m_iRenderEffect)
		{
		case RE_MIRROR_LEFTRIGHT :
		RenderEffect(pddsBackBuffer, DDBLTFX_MIRRORLEFTRIGHT);
		break;
		case RE_MIRROR_UPDOWN :
		RenderEffect(pddsBackBuffer, DDBLTFX_MIRRORUPDOWN);
		break;
		case RE_MIRROR_LEFTRIGHTUPDOWN :
		RenderEffect(pddsBackBuffer, DDBLTFX_MIRRORUPDOWN|DDBLTFX_MIRRORLEFTRIGHT);
		break;
		}
		*/


		/*
		HDC hDC;
		pddsBackBuffer->GetDC(&hDC);
		for (int k = 0; k < MAX_SET_OSD_NUM; k++)
		{
		if (m_bOsd[k])
		{
		WCHAR	wszInfo[256];

		::SetBkMode(hDC, TRANSPARENT);

		MultiByteToWideChar( CP_UTF8, 0, m_osd_info[k].font,strlen(m_osd_info[k].font)+1,
		wszInfo,sizeof(wszInfo)/sizeof(wszInfo[0]) );


		HFONT font;
		if ((m_dwWidth > 1000) && ((rcBlt.right-rcBlt.left) < 700))
		{
		font = ::CreateFont(36, 17,
		0, 0, FW_NORMAL, FALSE, FALSE,0,0,0,0,0,0, wszInfo);
		}
		else
		{
		font = ::CreateFont(m_osd_info[k].font_height, m_osd_info[k].font_width,
		0, 0, FW_NORMAL, FALSE, FALSE,0,0,0,0,0,0, wszInfo);
		}

		HFONT oldfont = (HFONT)::SelectObject(hDC, font);


		if (1 == m_osd_info[k].isTime)
		{

		SYSTEMTIME   sysTime = m_st;
		//GetLocalTime(&sysTime);


		MultiByteToWideChar( CP_UTF8, 0, m_osd_info[k].osd_string,strlen(m_osd_info[k].osd_string)+1,
		wszInfo,sizeof(wszInfo)/sizeof(wszInfo[0]) );

		CString strTemp;
		strTemp.Format(_T("%s%04d-%02d-%02d %02d:%02d:%02d"),
		wszInfo,
		sysTime.wYear,sysTime.wMonth, sysTime.wDay,
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond);


		DrawOSD(hDC,m_osd_info[k].pos,strTemp,rcPic,m_osd_info[k].osd_rect,m_osd_info[k].osd_color_red,m_osd_info[k].osd_color_green,m_osd_info[k].osd_color_blue);

		}
		else
		{
		MultiByteToWideChar( CP_UTF8, 0, m_osd_info[k].osd_string,strlen(m_osd_info[k].osd_string)+1,
		wszInfo,sizeof(wszInfo)/sizeof(wszInfo[0]) );

		CString strTemp = wszInfo;

		DrawOSD(hDC,m_osd_info[k].pos,strTemp,rcPic,m_osd_info[k].osd_rect,m_osd_info[k].osd_color_red,m_osd_info[k].osd_color_green,m_osd_info[k].osd_color_blue);

		}

		::SelectObject(hDC, oldfont);
		DeleteObject(font);

		}
		}

		pddsBackBuffer->ReleaseDC(hDC);
		*/

		///

#if 0

		DWORD64 dw = CrossGetTickCount64();

		HFONT m_hAlarmFont;
		m_hAlarmFont = CreateFont(
			60,								// nHeight
			0,								// nWidth
			0,								// nEscapement
			0,								// nOrientation
			FW_BOLD,//FW_NORMAL,						// nWeight
			FALSE,							// bItalic
			FALSE,							// bUnderline
			0,								// cStrikeOut
			ANSI_CHARSET,					// nCharSet
			OUT_DEFAULT_PRECIS,				// nOutPrecision
			CLIP_DEFAULT_PRECIS,			// nClipPrecision
			PROOF_QUALITY,					// nQuality
			DEFAULT_PITCH | FF_SWISS,		// nPitchAndFamily
			NULL);					// lpszFacename
		SYSTEMTIME   sysTime;
		GetLocalTime(&sysTime);
		
		CString strTemp;
		strTemp.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
			sysTime.wYear, sysTime.wMonth, sysTime.wDay,
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond);


		HDC hdc = NULL;
		HGDIOBJ hOldFont = NULL;
		SIZE stTextSize;
		CString szDugInfo;

		if (m_pDirect3DSurfaceRender->GetDC(&hdc) != S_OK)
		{
			return;
		}


		// 设置字体
		hOldFont = SelectObject(hdc, m_hAlarmFont);
		//SetBkColor(hdc, RGB(0, 255, 0));

		SetBkMode(hdc, TRANSPARENT);

		SetTextColor(hdc, RGB(255, 255, 0));
		TextOut(hdc, rcDestRect.left + 1300, rcDestRect.top + 850, strTemp, strTemp.GetLength());
		SetTextColor(hdc, RGB(255, 0, 0));
		TextOut(hdc, rcDestRect.left + 1301, rcDestRect.top + 851, strTemp, strTemp.GetLength());

		SelectObject(hdc, hOldFont);
		m_pDirect3DSurfaceRender->ReleaseDC(hdc);


		CROSS_TRACE("DRAW OSD:%d ms\n", CrossGetTickCount64() - dw);//0,0,0,0,0,0,0,15,0,0,0,0,0,0,,0,16,0,0,0,0....
#endif



		//--------------------------------------------------------------------------------------------------------------------
		if (m_pDirect3DDevice == NULL)
			return ;

		//CROSS_DWORD64 dw = CrossGetTickCount64();


		HRESULT lRet;
		m_pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 0), 1.0f, 0);
		lRet = m_pDirect3DDevice->BeginScene();
		lRet = m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender, &rcSourceRect, m_pBackBuffer, &rcDestRect, D3DTEXF_LINEAR);
		lRet = m_pDirect3DDevice->EndScene();
		lRet = m_pDirect3DDevice->Present(NULL, NULL, NULL, NULL);
		if (S_OK != lRet)
		{
			m_bRenderSuccess = FALSE;
		}
		else
		{
			m_bRenderSuccess = TRUE;
		}
		//为什么最小化都任务栏的时候Present耗时很严重 30多个ms
		//TRACE("Render use time :   %d 555\n", CrossGetTickCount64() - dw);//

		//--------------------------------------------------------------------------------------------------------------------
}

BOOL CD3DSurfaceRender::CreateSurfaceRender(HWND hWnd, DWORD dwWidth, DWORD dwHeight)
{
	BOOL bWndChanged = FALSE, bWndRectChanged = FALSE, bSizeChanged = FALSE;

	RECT rc;
	HRESULT lRet;

	if (!IsWindowVisible(m_hWnd) || !GetClientRect(m_hWnd, &rc)){
		return FALSE;
	}

	//bWndChanged
	if (m_hWnd != hWnd)
	{
		m_hWnd = hWnd;
		bWndChanged = TRUE;
	}

	//bWndRectChanged
	if (!EqualRect(&m_rcWnd, &rc))
	{
		CopyRect(&m_rcWnd, &rc);
		bWndRectChanged = TRUE;
	}

	//bSizeChanged
	if (dwWidth != 0 && (m_dwVideoWidth != dwWidth || m_dwVideoHeight != dwHeight))
	{
		m_dwVideoWidth = dwWidth;
		m_dwVideoHeight = dwHeight;
		bSizeChanged = TRUE;
	}

	if (FALSE == m_bRenderSuccess)
	{
		bSizeChanged = TRUE;
	}

	if (bWndChanged || bSizeChanged || bWndRectChanged)
	{
		CROSS_TRACE(">>>>>>> CD3DSurfaceRender::Create \n");
 		ReleaseD3D();

		//--------------------------------------------------------------------------------------------------------------------
		m_pDirect3D9 = Direct3DCreate9(D3D_SDK_VERSION);//创建d3d对象
		if (m_pDirect3D9 == NULL)
		{
			ReleaseD3D();
			return FALSE;
		}



		//--------------------------------------------------------------------------------------------------------------------
		ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));//清空显示参数，重新设置参数
		m_d3dpp.Windowed = TRUE;//FALSE，表示要渲染全屏，如果为TRUE，表示要渲染窗口
		m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//交换缓冲支持的效果类型，指定表面在交换链中D是如何被交换的  
		//D3DSWAPEFFECT_DISCARD，则后备缓冲区的东西被复制到屏幕上后,后备缓冲区的东西就没有什么用可以丢弃
		m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;//D3DFMT_UNKNOWN;//后备缓冲的格式D3DFMT_UNKNOWN，这时候它将使用桌面的格式  
		m_d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;



		//UINT Adapter显卡，D3DADAPTER_DEFAULT默认显卡
		//IDirect3D9:: GetAdapterCount()获取显卡数量，D3D9中的显卡表示为0-GetAdapterCount()-1的整数，主显卡也可用D3DADAPTER_DEFAULT表示。
		//GetAdapterCount()返回的是连接到显示器并且在扩展屏配置了的数量
		//如何对应以后找
#if 1
		D3DADAPTER_IDENTIFIER9 adapterID; // Used to store device info
		//		char strBuffer[2048];
		DWORD dwDisplayCount = m_pDirect3D9->GetAdapterCount();
		for (DWORD i = 0; i < dwDisplayCount; i++)
		{
			if (m_pDirect3D9->GetAdapterIdentifier(i/*D3DADAPTER_DEFAULT*/, 0, &adapterID) != D3D_OK)
			{
				break;
			}
			int k = 0;
			k++;
		}
#endif
		lRet = m_pDirect3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp, &m_pDirect3DDevice);
		if (FAILED(lRet))
		{
			ReleaseD3D();
			return FALSE;
		}

		//--------------------------------------------------------------------------------------------------------------------
#if LOAD_BGRA
		lRet = m_pDirect3DDevice->CreateOffscreenPlainSurface(
			m_dwVideoWidth, m_dwVideoHeight,
			D3DFMT_X8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_pDirect3DSurfaceRender,
			NULL);
#endif
#if LOAD_YUV420P
		lRet = m_pDirect3DDevice->CreateOffscreenPlainSurface(
			m_dwVideoWidth, m_dwVideoHeight,
			(D3DFORMAT)MAKEFOURCC('Y', 'V', '1', '2'),
			D3DPOOL_DEFAULT,
			&m_pDirect3DSurfaceRender,
			NULL);
#endif

		if (FAILED(lRet))
		{
			ReleaseD3D();
			return FALSE;
		}


		lRet = m_pDirect3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer);
		if (FAILED(lRet))
		{
			ReleaseD3D();
			return FALSE;
		}



		//
		m_pDirect3DDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	}

	return TRUE;

}




bool CD3DSurfaceRender::YV12ToBGR32_FFmpeg(unsigned char* pYUV, unsigned char* pBGR24, int width, int height)
{

	DWORD64 dw = GetTickCount64();


	if (width < 1 || height < 1 || pYUV == NULL || pBGR24 == NULL)
		return false;
	//int srcNumBytes,dstNumBytes;
	//uint8_t *pSrc,*pDst;
	AVFrame pFrameYUV, pFrameBGR;

	//pFrameYUV = avpicture_alloc();
	//srcNumBytes = avpicture_get_size(PIX_FMT_YUV420P,width,height);
	//pSrc = (uint8_t *)malloc(sizeof(uint8_t) * srcNumBytes);
	//av_image_alloc(pFrameYUV.data, pFrameYUV.linesize, width, height, AV_PIX_FMT_YUV420P, BUFFER_ALIGN);
	av_image_fill_arrays(pFrameYUV.data, pFrameYUV.linesize, pYUV, AV_PIX_FMT_YUV420P, width, height, BUFFER_ALIGN);

	//U,V互换
	uint8_t * ptmp = pFrameYUV.data[1];
	pFrameYUV.data[1] = pFrameYUV.data[2];
	pFrameYUV.data[2] = ptmp;

	//pFrameBGR = avcodec_alloc_frame();
	//dstNumBytes = avpicture_get_size(PIX_FMT_BGR24,width,height);
	//pDst = (uint8_t *)malloc(sizeof(uint8_t) * dstNumBytes);
	//av_image_alloc(pFrameBGR.data, pFrameBGR.linesize, width, height, AV_PIX_FMT_BGR32, BUFFER_ALIGN);
	av_image_fill_arrays(pFrameBGR.data, pFrameBGR.linesize, pBGR24, AV_PIX_FMT_BGR32, width, height, BUFFER_ALIGN);

	struct SwsContext* imgCtx = NULL;
	imgCtx = sws_getContext(width, height, AV_PIX_FMT_YUV420P, width, height, AV_PIX_FMT_BGR32, SWS_BILINEAR, 0, 0, 0);

	if (imgCtx != NULL){
		sws_scale(imgCtx, pFrameYUV.data, pFrameYUV.linesize, 0, height, pFrameBGR.data, pFrameBGR.linesize);
		if (imgCtx){
			sws_freeContext(imgCtx);
			imgCtx = NULL;
		}
		//		CROSS_TRACE("YV12ToBGR32_FFmpeg:%d ms\n", CrossGetTickCount64() - dw);
		return true;
	}
	else{
		//av_free(pFrameYUV);
		sws_freeContext(imgCtx);
		imgCtx = NULL;
		return false;
	}
}







void CD3DSurfaceRender::DrawOSD(HDC& dcMem, int pos, CString strTemp, RECT rc, RECT osd_rect, int osd_color_red, int osd_color_green, int osd_color_blue)
{
	/*


	RECT rcTemp;
	switch(pos)
	{
	case OSD_POSITION_LEFT_TOP:
	{
	rcTemp.top = 5;
	rcTemp.left = 5;
	rcTemp.bottom = rcTemp.top+34;
	rcTemp.right = rcTemp.left + (rc.right-rc.left -10);

	::SetTextColor(dcMem, RGB(255,255,255));
	::DrawText(dcMem, strTemp, strTemp.GetLength(), &rcTemp, DT_LEFT);

	rcTemp.left+=1;
	rcTemp.right+=1;
	rcTemp.top+=1;
	rcTemp.bottom+=1;
	::SetTextColor(dcMem, RGB(osd_color_red,osd_color_green,osd_color_blue));
	::DrawText(dcMem, strTemp, strTemp.GetLength(), &rcTemp, DT_LEFT);
	}
	break;
	case OSD_POSITION_RIGHT_TOP:
	{
	rcTemp.top = 5;
	rcTemp.left = 5;
	rcTemp.bottom = rcTemp.top+34;
	rcTemp.right = rcTemp.left + (rc.right-rc.left-10);

	::SetTextColor(dcMem,  RGB(255,255,255));
	::DrawText(dcMem, strTemp, strTemp.GetLength(), &rcTemp, DT_RIGHT);

	rcTemp.left+=1;
	rcTemp.right+=1;
	rcTemp.top+=1;
	rcTemp.bottom+=1;
	::SetTextColor(dcMem, RGB(osd_color_red,osd_color_green,osd_color_blue));
	::DrawText(dcMem, strTemp, strTemp.GetLength(), &rcTemp, DT_RIGHT);
	}
	break;
	case OSD_POSITION_LEFT_BUTTOM:
	{
	rcTemp.top = rc.bottom-rc.top - 29;
	rcTemp.left = 5;
	rcTemp.bottom = rcTemp.top+24;
	rcTemp.right = rcTemp.left + (rc.right-rc.left-10);

	::SetTextColor(dcMem,  RGB(255,255,255));
	::DrawText(dcMem, strTemp, strTemp.GetLength(), &rcTemp, DT_LEFT);

	rcTemp.left+=1;
	rcTemp.right+=1;
	rcTemp.top+=1;
	rcTemp.bottom+=1;
	::SetTextColor(dcMem, RGB(osd_color_red,osd_color_green,osd_color_blue));
	::DrawText(dcMem, strTemp, strTemp.GetLength(), &rcTemp, DT_LEFT);
	}
	break;
	case OSD_POSITION_RIGHT_BUTTOM:
	{
	rcTemp.top = rc.bottom-rc.top - 29;
	rcTemp.left = 5;
	rcTemp.bottom = rcTemp.top+24;
	rcTemp.right = rcTemp.left + (rc.right-rc.left-10);

	::SetTextColor(dcMem,  RGB(255,255,255));
	::DrawText(dcMem, strTemp, strTemp.GetLength(), &rcTemp, DT_RIGHT);

	rcTemp.left+=1;
	rcTemp.right+=1;
	rcTemp.top+=1;
	rcTemp.bottom+=1;
	::SetTextColor(dcMem, RGB(osd_color_red,osd_color_green,osd_color_blue));
	::DrawText(dcMem, strTemp, strTemp.GetLength(), &rcTemp, DT_RIGHT);
	}
	break;
	case OSD_POSITION_CENTER:
	{

	rcTemp.top = (rc.bottom-rc.top)/2 - 17;
	rcTemp.left = 5;
	rcTemp.bottom = rcTemp.top+34;
	rcTemp.right = rcTemp.left + (rc.right-rc.left-10);

	::SetTextColor(dcMem,  RGB(255,255,255));
	::DrawText(dcMem, strTemp, strTemp.GetLength(), &rcTemp, DT_CENTER);

	rcTemp.left+=1;
	rcTemp.right+=1;
	rcTemp.top+=1;
	rcTemp.bottom+=1;
	::SetTextColor(dcMem, RGB(osd_color_red,osd_color_green,osd_color_blue));
	::DrawText(dcMem, strTemp, strTemp.GetLength(),&rcTemp, DT_CENTER);

	}
	break;
	default:
	{
	::SetTextColor(dcMem,  RGB(255,255,255));
	::DrawText(dcMem, strTemp, strTemp.GetLength(), &osd_rect, DT_LEFT);

	rcTemp = osd_rect;
	rcTemp.left+=1;
	rcTemp.right+=1;
	rcTemp.top+=1;
	rcTemp.bottom+=1;
	::SetTextColor(dcMem, RGB(osd_color_red,osd_color_green,osd_color_blue));
	::DrawText(dcMem, strTemp, strTemp.GetLength(), &rcTemp, DT_LEFT);
	}
	break;
	}

	*/
}

// 
// BOOL CD3DSurfaceRender::CapturePicture(const char *pFileName)
// {
// 	if (m_bCapture)
// 		return FALSE;
// 
// 	ResetEvent(m_hCaptueredEvent);
// 
// 	m_pFileName = pFileName;
// 	m_pBuf = NULL;
// 	m_bCapture = TRUE;
// 
// 	DWORD ret = WaitForSingleObject(m_hCaptueredEvent, 200);
// 	m_bCapture = FALSE;
// 	if (ret == WAIT_OBJECT_0)
// 		return TRUE;
// 
// 	DDSURFACEDESC2  ddsd;
// 	ZeroMemory(&ddsd, sizeof(ddsd));
// 	ddsd.dwSize = sizeof(ddsd);
// 
// 	if (pddsBackBuffer == NULL)
// 	{
// 		return FALSE;
// 	}
// 
// 	if (FAILED(pddsBackBuffer->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL)))
// 	{
// 		ATLTRACE(" ###>>> Render Lock Failed\n");
// 		Release();
// 		return FALSE;
// 
// 	}
// 
// 	InternalCapture(&ddsd);
// 
// 	pddsBackBuffer->Unlock(NULL);
// 
// 	return TRUE;
// }
// 
// BOOL CD3DSurfaceRender::CapturePictureBuf(char *pBuf)
// {
// 	if (m_bCapture)
// 		return FALSE;
// 
// 	ResetEvent(m_hCaptueredEvent);
// 
// 	m_pFileName = NULL;
// 	m_pBuf = pBuf;
// 	m_bCapture = TRUE;
// 
// 	DWORD ret = WaitForSingleObject(m_hCaptueredEvent, 200);
// 	m_bCapture = FALSE;
// 	if (ret == WAIT_OBJECT_0)
// 		return TRUE;
// 
// 	DDSURFACEDESC2  ddsd;
// 	ZeroMemory(&ddsd, sizeof(ddsd));
// 	ddsd.dwSize = sizeof(ddsd);
// 
// 	if (pddsBackBuffer == NULL)
// 	{
// 		return FALSE;
// 	}
// 
// 	if (FAILED(pddsBackBuffer->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL)))
// 	{
// 		ATLTRACE(" ###>>> Render Lock Failed\n");
// 		Release();
// 		return FALSE;
// 
// 	}
// 
// 	InternalCapture(&ddsd);
// 
// 	pddsBackBuffer->Unlock(NULL);
// 
// 	return TRUE;
// }







#endif








// void CD3DSurfaceRender::RenderEffect(LPDIRECTDRAWSURFACE7 pdds, DWORD dwDDFX)
// {
// 
// 
// 	//RenderEffect
// 	//不起作用，请留意显卡型号
// 	//在zyx，集显ok，独显(凄惨红gt430)XX
// 
// 
// 	DDBLTFX ddbltfx;
// 	ZeroMemory(&ddbltfx, sizeof(ddbltfx));
// 	ddbltfx.dwSize = sizeof(ddbltfx);
// 	ddbltfx.dwDDFX = dwDDFX;
// 
// 
// 	pdds->Blt(NULL, pdds, NULL, DDBLT_WAIT | DDBLT_DDFX, &ddbltfx);
// }

// 
// void CD3DSurfaceRender::InternalCapture(LPDDSURFACEDESC2 lpddsd)
// {
// 	if (m_pFileName)
// 	{
// 		FILE* fp;
// 		if (fopen_s(&fp, m_pFileName, "wb") != 0)
// 		{
// 			return;
// 		}
// 
// 		BITMAPFILEHEADER FileHeader;
// 		FileHeader.bfType = 'MB';
// 		FileHeader.bfSize = m_dwWidth * m_dwHeight * m_iBpp + 0x36;
// 		FileHeader.bfReserved1 = 0;
// 		FileHeader.bfReserved2 = 0;
// 		FileHeader.bfOffBits = 0x36;
// 		fwrite(&FileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
// 
// 
// 		BITMAPINFOHEADER Header;
// 		Header.biSize = sizeof(BITMAPINFOHEADER);
// 		Header.biWidth = m_dwWidth;
// 		Header.biHeight = m_dwHeight;
// 		Header.biPlanes = 1;
// 		Header.biBitCount = m_iBpp * 8;
// 		Header.biCompression = BI_RGB;
// 		Header.biSizeImage = m_dwWidth * m_dwHeight * m_iBpp;
// 		Header.biXPelsPerMeter = 0;
// 		Header.biYPelsPerMeter = 0;
// 		Header.biClrUsed = 0;
// 		Header.biClrImportant = 0;
// 		fwrite(&Header, Header.biSize, 1, fp);
// 
// 
// 		fseek(fp, 0x36, SEEK_SET);
// 
// 		for (INT i = m_dwHeight - 1; i >= 0; i--)
// 		{
// 			for (DWORD j = 0; j < m_dwWidth*m_iBpp; j += m_iBpp)
// 			{
// 				fwrite((PCHAR)lpddsd->lpSurface + i*lpddsd->lPitch + j, m_iBpp, 1, fp);
// 			}
// 		}
// 
// 		fclose(fp);
// 	}
// 	else
// 	{
// 
// 		BITMAPFILEHEADER FileHeader;
// 		FileHeader.bfType = 'MB';
// 		FileHeader.bfSize = m_dwWidth * m_dwHeight * m_iBpp + 0x36;
// 		FileHeader.bfReserved1 = 0;
// 		FileHeader.bfReserved2 = 0;
// 		FileHeader.bfOffBits = 0x36;
// 		memcpy(m_pBuf, &FileHeader, sizeof(BITMAPFILEHEADER));
// 
// 
// 		BITMAPINFOHEADER Header;
// 		Header.biSize = sizeof(BITMAPINFOHEADER);
// 		Header.biWidth = m_dwWidth;
// 		Header.biHeight = m_dwHeight;
// 		Header.biPlanes = 1;
// 		Header.biBitCount = m_iBpp * 8;
// 		Header.biCompression = BI_RGB;
// 		Header.biSizeImage = m_dwWidth * m_dwHeight * m_iBpp;
// 		Header.biXPelsPerMeter = 0;
// 		Header.biYPelsPerMeter = 0;
// 		Header.biClrUsed = 0;
// 		Header.biClrImportant = 0;
// 		memcpy(m_pBuf + sizeof(BITMAPFILEHEADER), &Header, Header.biSize);
// 
// 		m_pBuf += sizeof(BITMAPFILEHEADER) + Header.biSize;
// 		for (INT i = m_dwHeight - 1; i >= 0; i--)
// 		{
// 			for (DWORD j = 0; j < m_dwWidth*m_iBpp; j += m_iBpp)
// 			{
// 				memcpy(m_pBuf, (PCHAR)lpddsd->lpSurface + i*lpddsd->lPitch + j, m_iBpp);
// 			}
// 		}
// 	}
// 
// 	m_bCapture = FALSE;
// 	SetEvent(m_hCaptueredEvent);
// }
