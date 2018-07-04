#pragma once
#include "Display.h"


struct stChannelParam
{
	emSCREENTYPE type;
	int ch[128];//��type==SCREEN_TYPE_UNITE_SINGLEʱ�˲�����Ч��������������Ļ������
};


class CVideoSet
{
public:
	CVideoSet();
	~CVideoSet();

public:
	GUID GetGuid(){ return m_GUID; };
	int Init(GUID *pGuid, char *szDevName, DEVMODEA dm);



private:



public:
	
	int					SetDisplayNum(int nDisplayNum);
	int 				SetDisplayUnite(int nAllDisplay, int nWhichDisplay, DWORD dwWidth, DWORD dwHeight);
	void				DeleteDisplayUnite();
	IRender*			InputData(int nDisplayCount,const void* pData, long lDataLen, long lDataFmt, DWORD dwWidth, DWORD dwHeight);
	void				FreshData(int nDisplayCount);
	void				Release();
	void				AddOutInfoText(long lMatrix, const wchar_t* szOutText, int nTextLen);
		
private:
	

private:
	GUID		m_GUID;
	char		m_szDevName[512];
	DWORD		m_dwVideoWidth;
	DWORD		m_dwVideoHeight;
	IDirectDraw	*m_lpDD;
	map<int, CDisplay*> m_mapDisplayWnd;
	int			m_nWndNum;
	DEVMODEA	m_dm;
	CRect		m_VideoSrcRect;
	BOOL		m_bDisplayUnite;
};
