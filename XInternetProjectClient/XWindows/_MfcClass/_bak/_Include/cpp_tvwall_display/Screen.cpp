
#include "stdafx.h"
#include "Screen.h"


CVideoSet::CVideoSet()
{
	m_dwVideoHeight = 0;
	m_dwVideoWidth = 0;
	m_lpDD = 0;
	ZeroMemory(&m_GUID, sizeof(GUID));
	ZeroMemory(m_szDevName, 512);
	m_mapDisplayWnd.clear();
	ZeroMemory(&m_dm, sizeof(m_dm));
	m_nWndNum = 0;
	m_bDisplayUnite = false;
	m_VideoSrcRect.SetRect(0, 0, 0, 0);
};

CVideoSet::~CVideoSet()
{
	if (m_nWndNum > 0)
	{
		for (int i = 0; i < m_nWndNum; i++)
		{
			delete m_mapDisplayWnd[i];
			m_mapDisplayWnd[i] = NULL;
		}
	}
	if (m_lpDD)
	{
		m_lpDD->Release();
		m_lpDD = NULL;
	}
}

int CVideoSet::SetDisplayNum(int nDisplayNum)
{
	if (m_nWndNum > 0)
	{
		for (int i = 0; i < m_nWndNum; i++)
		{
			delete m_mapDisplayWnd[i];
			m_mapDisplayWnd[i] = NULL;
		}
		m_nWndNum = 0;
	}

	int nLineNum = 0;
	switch(nDisplayNum)
	{
	case 1:
		{
			nLineNum = 1;
			break;
		}
	case 4:
		{
			nLineNum = 2;
			break;
		}
	case 9:
		{
			nLineNum = 3;
			break;
		}
	case 16:
		{
			nLineNum = 4;
			break;
		}
	case 25:
		{
			nLineNum = 5;
			break;
		}
	case 36:
		{
			nLineNum = 6;
			break;
		}
	default:
		return -1;
	}

	DWORD dwDisplayHeight = m_dwVideoHeight / nLineNum;
	DWORD dwDisplayWidth = m_dwVideoWidth / nLineNum;
	CRect cDisplayRect;
	int  x = 0, y = 0;
	for (int i = 0; i < nLineNum; i++)	// èÈ×ø±ê
	{
		for (int j = 0; j < nLineNum; j++) // ºá×ø±ê
		{
			x = dwDisplayWidth * j;
			y = dwDisplayHeight * i;
			cDisplayRect.SetRect(x, y, x + dwDisplayWidth, y + dwDisplayHeight);
			m_mapDisplayWnd[m_nWndNum] = new CDisplay;
			if (m_mapDisplayWnd[m_nWndNum] == NULL)
			{
				return -2;
			}
			m_mapDisplayWnd[m_nWndNum]->SetOutDstRect(cDisplayRect);
			m_mapDisplayWnd[m_nWndNum]->SetDevMode(m_dm);
			m_mapDisplayWnd[m_nWndNum]->Init(&m_GUID, dwDisplayWidth, dwDisplayHeight);
			m_nWndNum++;
		}
	}
	m_bDisplayUnite = false;
	return 0;
}

int CVideoSet::Init(GUID *pGuid, char *szDevName, DEVMODEA dm)
{
	m_GUID = *pGuid;
	strcpy_s(m_szDevName, 512, szDevName);
	m_dm = dm;

	HRESULT hr = DD_OK;
	hr = DirectDrawCreate(&m_GUID, &m_lpDD, NULL);
	if (hr != DD_OK)
	{
		return -2;
	}
	hr = m_lpDD->SetCooperativeLevel(NULL, DDSCL_NORMAL);
	if (hr != DD_OK)
	{
		return -3;
	}
	m_dwVideoWidth = m_dm.dmPelsWidth;
	m_dwVideoHeight = m_dm.dmPelsHeight;
	return 0;
}

IRender* CVideoSet::InputData(int nDisplayCount, const void* pData, long lDataLen, long lDataFmt, DWORD dwWidth, DWORD dwHeight)
{
	CDisplay* pResult = m_mapDisplayWnd[nDisplayCount];

	if (pResult == NULL)
	{
		return NULL;
	}

	if (pResult->InputData(pData, lDataLen, lDataFmt, dwWidth, dwHeight) == 0)
	{
		return pResult;
	}

	return NULL;
}

void CVideoSet::Release()
{
	delete this;
}

void CVideoSet::FreshData(int nDisplayCount)
{
	if (m_mapDisplayWnd[nDisplayCount] == NULL)
	{
		return;
	}
	m_mapDisplayWnd[nDisplayCount]->Refresh(TRUE);
	m_mapDisplayWnd[nDisplayCount]->ClearOutText();
}

void CVideoSet::AddOutInfoText(long lMatrix, const wchar_t* szOutText, int nTextLen)
{
	if (m_mapDisplayWnd[lMatrix] == NULL)
	{
		return;
	}
	m_mapDisplayWnd[lMatrix]->AddOutText(szOutText, nTextLen);
}

int CVideoSet::SetDisplayUnite(int nAllDisplay, int nWhichDisplay, DWORD dwWidth, DWORD dwHeight)
{
	int nLinkNum = 0;
	switch(nAllDisplay)
	{
	case 4:
		{
			nLinkNum = 2;
			break;
		}
	case 9:
		{
			nLinkNum = 3;
			break;
		}
	case 16:
		{
			nLinkNum = 4;
			break;
		}
	case 25:
		{
			nLinkNum = 5;
			break;
		}
	case 36:
		{
			nLinkNum = 6;
			break;
		}
	default:
		return -1;
	}
	if (SetDisplayNum(1))
	{
		return -2;
	}
	DWORD dwSrcWidth = dwWidth / nLinkNum;
	DWORD dwSrcHeight = dwHeight / nLinkNum;
	CRect SrcRect;
	int  x = 0, y = 0;
	int nWndCount = 0;
	for (int i = 0; i < nLinkNum; i++)
	{
		for (int j = 0; j < nLinkNum; j++)
		{
			x = dwSrcWidth * j;
			y = dwSrcHeight * i;
			SrcRect.SetRect(x, y, x + dwSrcWidth, y + dwSrcHeight);
			if (nWndCount == nWhichDisplay)
			{
				m_VideoSrcRect = SrcRect;
				m_bDisplayUnite = TRUE;
				m_mapDisplayWnd[0]->SetVideoSrcRect(m_VideoSrcRect, m_bDisplayUnite);
			}
			nWndCount++;
		}
	}
	return 0;
}

void CVideoSet::DeleteDisplayUnite()
{
	m_bDisplayUnite = FALSE;
	m_mapDisplayWnd[0]->SetVideoSrcRect(m_VideoSrcRect, m_bDisplayUnite);
}