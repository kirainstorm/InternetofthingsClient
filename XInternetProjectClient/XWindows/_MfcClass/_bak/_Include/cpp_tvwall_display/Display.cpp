#include "stdafx.h"
#include "Display.h"

CDisplay::CDisplay(GUID guid, DEVMODEA dm, CRect DstRect, int h, int l, int x, int y)
{
	m_pDisplayGUID = guid;
	m_dm = dm;
	m_OutDstRect = DstRect;//设置要输出的图像在显示器上的位置
	m_h = h;
	m_l = l;
	m_x = x;
	m_y = y; //要输出的图像区域，因为拼屏的话只要输出一部分


	m_lpDDraw = NULL;
	m_lpPrimaryBuffer = NULL;
	m_lpBackBuffer = NULL;

	m_nHeight = 0;
	m_nWidth = 0;

	
}

CDisplay::~CDisplay()
{
	Refresh();
	//	delete[] m_pBGR;
	/*if (m_lpDDopl)
	{
	m_lpDDopl->Release();
	m_lpDDopl = NULL;
	}*/
	if (m_lpBackBuffer)
	{
		m_lpBackBuffer->Release();
		m_lpBackBuffer = NULL;
	}
	if (m_lpPrimaryBuffer)
	{
		m_lpPrimaryBuffer->Release();
		m_lpPrimaryBuffer = NULL;
	}
	if (m_lpDDraw)
	{
		m_lpDDraw->Release();
		m_lpDDraw = NULL;
	}
}

void CDisplay::Release()
{
	delete this;
}

int CDisplay::Init(int nWidth, int nHeight)
{
	//m_pDisplayGUID = pGUID;
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	m_VideoSrcRect.left = m_x *(m_nWidth / m_l);
	m_VideoSrcRect.top = m_y*(m_nHeight / m_h);
	m_VideoSrcRect.right = m_VideoSrcRect.left + (m_nWidth / m_l);
	m_VideoSrcRect.bottom = m_VideoSrcRect.top + (m_nHeight / m_h);


	HRESULT hr = 0;
	hr = DirectDrawCreateEx(&m_pDisplayGUID, (LPVOID *)&m_lpDDraw, IID_IDirectDraw7, NULL); //DirectDrawCreate(&m_pDisplayGUID, &m_lpDDraw, NULL);
	if (hr != S_OK)
	{
		return -1;
	}

	hr = m_lpDDraw->SetCooperativeLevel(NULL, DDSCL_NORMAL);
	if (hr != S_OK)
	{
		return -1;
	}

	/*DDCAPS ddcps;
	ZeroMemory(&ddcps, sizeof(ddcps));
	hr = m_lpDDraw->GetCaps(&ddcps, NULL);
	if (hr != DD_OK)
	{
	KsWriteLog("GetCaps Error!");
	return -1;
	}*/

	DDSURFACEDESC2 ddsc;
	ZeroMemory(&ddsc, sizeof(ddsc));
	ddsc.dwSize = sizeof(ddsc);
	ddsc.dwFlags = DDSD_CAPS;
	ddsc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_VIDEOMEMORY;
	hr = m_lpDDraw->CreateSurface(&ddsc, &m_lpPrimaryBuffer, NULL);
	if (hr != S_OK)
	{
		ddsc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		if (m_lpDDraw->CreateSurface(&ddsc, &m_lpPrimaryBuffer, NULL) != S_OK)
		{
			return -1;
		}
	}
	/*ddsc.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
	hr = m_lpPrimaryBuffer->GetAttachedSurface(&ddsc.ddsCaps, &m_lpBackBuffer);
	if (hr != DD_OK)
	{
	return -1;
	}*/
	if (!CreateOffScreenSurface())
	{
		return -1;
	}

	return 0;
}

int CDisplay::CreateOffScreenSurface()
{
	if (m_lpDDraw == NULL)
	{
		return FALSE;
	}
	if (m_lpBackBuffer != NULL)
	{
		m_lpBackBuffer->Release();
		m_lpBackBuffer = NULL;
	}
#if DRAW_RGB
	HRESULT hr;
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	ddsd.dwWidth = m_nWidth;
	ddsd.dwHeight = m_nHeight;
	hr = m_lpDDraw->CreateSurface(&ddsd, &m_lpBackBuffer, NULL);
	if (hr != S_OK)
	{
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		if (m_lpDDraw->CreateSurface(&ddsd, &m_lpBackBuffer, NULL) != S_OK)
		{
			return FALSE;
		}
	}

#endif


#if DRAW_YUV
	HRESULT hr;
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	ddsd.dwWidth = m_nWidth;
	ddsd.dwHeight = m_nHeight;
	ddsd.ddpfPixelFormat.dwSize = sizeof(DDSD_PIXELFORMAT);
	ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC | DDPF_YUV;
	ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('Y', 'V', '1', '2');
	ddsd.ddpfPixelFormat.dwYUVBitCount = 8;
	hr = m_lpDDraw->CreateSurface(&ddsd, &m_lpBackBuffer, NULL);
	if (hr != S_OK)
	{
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		if (m_lpDDraw->CreateSurface(&ddsd, &m_lpBackBuffer, NULL) != S_OK)
		{
			return FALSE;
		}
	}
#endif


	return TRUE;
}


//void CDisplay::SetVideoSrcRect(CRect SrcRect)
//{
//	m_VideoSrcRect = SrcRect;
//}
void CDisplay::Flash()
{
	if (m_lpBackBuffer)
	{
		for (size_t i = 0; i < 4; i++)
		{
			long lRefreshLen = m_nWidth * m_nHeight * 4;
			LPBYTE pRefresh = new BYTE[lRefreshLen];

#if DRAW_YUV
			int m = m_nWidth * m_nHeight;
			int n = m_nWidth * m_nHeight / 2;
			memset(pRefresh, 0, m);
			memset(pRefresh + m, i % 2 ? 128:0, n);
			memset(pRefresh + m + n, 0, n);
#endif
#if DRAW_RGB
			memset(pRefresh, i%2?0:128, lRefreshLen);
#endif

			InputData(pRefresh, lRefreshLen, 0, m_nWidth, m_nHeight);
			render();
			delete[] pRefresh;

			for (size_t i = 0; i < 1000; i++)
			{
				;
			}
		}

	}
}
void CDisplay::Refresh()
{
	if (m_lpBackBuffer)
	{
		{
			long lRefreshLen = m_nWidth * m_nHeight * 4;
			LPBYTE pRefresh = new BYTE[lRefreshLen];


#if DRAW_YUV
			int m = m_nWidth * m_nHeight;
			int n = m_nWidth * m_nHeight / 2;
			memset(pRefresh, 0, m);
			memset(pRefresh + m, 128, n);
			memset(pRefresh + m + n, 0, n);
#endif
#if DRAW_RGB
			memset(pRefresh, 0, lRefreshLen);
#endif

			DWORD dw = GetTickCount();
			InputData(pRefresh, lRefreshLen, 0, m_nWidth, m_nHeight);
			int ff = GetTickCount() - dw;
			render();
			ff = GetTickCount() - dw;




			delete[] pRefresh;
		}

	}
}

long CDisplay::InputData(const void* pData, long lDataLen, long lDataFmt, DWORD dwWidth, DWORD dwHeight)
{


	if (lDataFmt != 0)
	{
		return -1;
	}

	if (m_nWidth != dwWidth || m_nHeight != dwHeight)
	{
		if (SetVideoSize(dwWidth, dwHeight))
		{
			return -1;
		}
	}

	HRESULT hr = DD_OK;
	DDSURFACEDESC2 Desc;
	ZeroMemory(&Desc, sizeof(Desc));
	Desc.dwSize = sizeof(Desc);

	while ((hr = IDirectDrawSurface_Lock(m_lpBackBuffer, NULL, &Desc, DDLOCK_WAIT, NULL)) != DD_OK)
	{
		if (hr != DDERR_SURFACELOST || IDirectDrawSurface_Restore(m_lpBackBuffer) != DD_OK)
		{
			return -1;
		}
	}

	if (Desc.lpSurface)
	{
#if DRAW_YUV
		LPBYTE lpSurf = (LPBYTE)Desc.lpSurface;
		LPBYTE lpY = (LPBYTE)pData;
		LPBYTE lpU = lpY + dwWidth * dwHeight;
		LPBYTE lpV = lpY + dwWidth * dwHeight * 5 / 4;
		DWORD i = 0;


		__try
		{
			for (i = 0; i < Desc.dwHeight; i++)
			{
				memcpy(lpSurf, lpY, Desc.dwWidth);
				lpY += dwWidth;
				lpSurf += Desc.lPitch;
			}
			for (i = 0; i < Desc.dwHeight / 2; i++)
			{
				memcpy(lpSurf, lpV, Desc.dwWidth / 2);
				lpV += dwWidth / 2;
				lpSurf += Desc.lPitch / 2;
			}
			for (i = 0; i < Desc.dwHeight / 2; i++)
			{
				memcpy(lpSurf, lpU, Desc.dwWidth / 2);
				lpU += dwWidth / 2;
				lpSurf += Desc.lPitch / 2;
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
#endif
#if DRAW_RGB
		memcpy((byte *)Desc.lpSurface, (unsigned char *)pData, lDataLen);
#endif
	}


	IDirectDrawSurface_Unlock(m_lpBackBuffer, NULL);

	m_lpPrimaryBuffer->Blt(&m_OutDstRect, m_lpBackBuffer, /*m_SrcRect*/&m_VideoSrcRect, DDBLT_WAIT, NULL);


	return 0;
}
long CDisplay::render()
{
	
	return 0;
}
int CDisplay::SetVideoSize(DWORD dwWidth, DWORD dwHeight)
{
	m_nWidth = dwWidth;
	m_nHeight = dwHeight;
	if (!CreateOffScreenSurface())
	{
		return -1;
	}
	m_VideoSrcRect.left = m_x *(m_nWidth / m_l);
	m_VideoSrcRect.top = m_y*(m_nHeight / m_h);
	m_VideoSrcRect.right = m_VideoSrcRect.left + (m_nWidth / m_l);
	m_VideoSrcRect.bottom = m_VideoSrcRect.top + (m_nHeight / m_h);
	return 0;
}
