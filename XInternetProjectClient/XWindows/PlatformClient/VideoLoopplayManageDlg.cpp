// VideoManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "VideoLoopplayManageDlg.h"
#include "afxdialogex.h"
#include "PlatformClientDlg.h"

// CVideoPollingManageDlg dialog

IMPLEMENT_DYNAMIC(CVideoLoopplayManageDlg, CDialogEx)

CVideoLoopplayManageDlg::CVideoLoopplayManageDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CVideoLoopplayManageDlg::IDD, pParent)
{
	m_hThread = NULL;
	m_bShowMax = FALSE;
	m_pWnd = pParent;
	for (int i = 0; i < MAX_WINDOWS_NUM; i++){
		m_pVideoWindows[i] = NULL;
	}
}
CVideoLoopplayManageDlg::~CVideoLoopplayManageDlg()
{
}
void CVideoLoopplayManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CVideoLoopplayManageDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP()
BOOL CVideoLoopplayManageDlg::InitPanlLiveWnd(CWnd *pParent)
{
	m_pWnd = pParent;
	for (int i = 0; i < MAX_WINDOWS_NUM; i++){
		m_pVideoWindows[i]->m_pAfxMainWnd = m_pWnd;
	}
	return TRUE;
}
BOOL CVideoLoopplayManageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_nShow = 6;
	m_nFocusIndex = 0;
	for (int i = 0; i < MAX_WINDOWS_NUM; i++)
	{
		m_pVideoWindows[i] = new CVideoLoopplayView();
		m_pVideoWindows[i]->Create(CVideoLoopplayView::IDD, this);
		m_pVideoWindows[i]->MoveWindow(0, 0, 0, 0);
		m_pVideoWindows[i]->ShowWindow(SW_HIDE);
		m_pVideoWindows[i]->m_pAfxMainWnd = m_pWnd;
	}
	m_pVideoWindows[0]->MoveWindow(0, 0, 100, 100);
	m_pVideoWindows[0]->ShowWindow(SW_SHOW);
	//

	ShowVideoWindows(theApp.m_clientSetting.loopplay_wnd);



	TRACE(" ###>>> CVideoPollingManageDlg m_pVideoWindows[] Create ok\n");
	return TRUE;
}
BOOL CVideoLoopplayManageDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		UINT nKey = (int)pMsg->wParam;
		if (VK_RETURN == nKey){
			return TRUE;
		}
		if (VK_ESCAPE == nKey){
			CPlatClientDlg *pWnd = (CPlatClientDlg *)m_pWnd;
			if (pWnd->m_fFullScreen)
			{
				pWnd->ShowWindowMax(FALSE);
			}
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
void CVideoLoopplayManageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (m_pVideoWindows[0] == NULL)
	{
		return;
	}
	if (GetSafeHwnd() == NULL){
		return;
	}
	if (cx <= 0) return;
	ShowVideoWindows(m_nShow);
}
HBRUSH CVideoLoopplayManageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// 	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// 	if ( pDC == NULL || pDC->m_hDC == NULL )
	// 	{
	// 		return hbr;
	// 	}
	// 	if ( hbr!= NULL )
	// 	{
	// 		::DeleteObject(hbr);
	// 	}
	// 	return (HBRUSH) GetStockObject(GRAY_BRUSH);
	//return hbr;
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  在此更改 DC 的任何特性
	return (HBRUSH)theApp.hViewBrushBg;// GetStockObject(GRAY_BRUSH);
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

int CVideoLoopplayManageDlg::ShowVideoWindowMax(int nInex, BOOL bShowMax)
{
	m_nFocusIndex = nInex;
	m_bShowMax = bShowMax;

	if (bShowMax)
	{
		CRect rt;
		GetClientRect(&rt);
		//
		for (int i = 0; i < MAX_WINDOWS_NUM; i++){
			m_pVideoWindows[i]->MoveWindow(0, 0, 0, 0);
			m_pVideoWindows[i]->ShowWindow(SW_HIDE);
		}
		//
		int nWidth = rt.Width() - 1;
		int nHeigh = rt.Height() - 1;
		int nLeft = rt.left + 1;
		int nTop = rt.top + 1;
		m_pVideoWindows[nInex]->MoveWindow(nLeft, nTop, nWidth, nHeigh);
		m_pVideoWindows[nInex]->ShowWindow(SW_SHOW);
	}
	else
	{
		ShowVideoWindows(m_nShow);
	}

	return 0;
}
int CVideoLoopplayManageDlg::ShowVideoWindows(int nShowNum)
{
	if (m_bShowMax)
	{
		CRect rt;
		GetClientRect(&rt);

		for (int i = 0; i < MAX_WINDOWS_NUM; i++){
			m_pVideoWindows[i]->MoveWindow(0, 0, 0, 0);
			m_pVideoWindows[i]->ShowWindow(SW_HIDE);
		}

		int nWidth = rt.Width() - 1;
		int nHeigh = rt.Height() - 1;
		int nLeft = rt.left + 1;
		int nTop = rt.top + 1;
		m_pVideoWindows[m_nFocusIndex]->MoveWindow(nLeft, nTop, nWidth, nHeigh);
		m_pVideoWindows[m_nFocusIndex]->ShowWindow(SW_SHOW);
// 		m_pVideoWindows[m_nFocusIndex]->m_bFoucs = TRUE;
// 		m_pVideoWindows[m_nFocusIndex]->DrawFrameRect();
		return 0;
	}

	//
	//
	CRect rt;
	GetClientRect(&rt);
	for (int i = 0; i < MAX_WINDOWS_NUM; i++){
		m_pVideoWindows[i]->MoveWindow(0, 0, 0, 0);
		m_pVideoWindows[i]->ShowWindow(SW_HIDE);
	}
	//
	int nShowSel = m_nFocusIndex;
	if (m_nShow != nShowNum)
		nShowSel = 0;
	//
	m_nShow = nShowNum;
	//
	long double l = (long double)m_nShow;
	if (6 == m_nShow)
	{
		l = (long double)9;
	}
	if (8 == m_nShow)
	{
		l = (long double)16;
	}
	int j = (int)sqrt(l);
	int nWidth = rt.Width() / j;
	int nHeigh = rt.Height() / j;

	int nLeft = (rt.Width() - nWidth*j) / 2;
	int nTop = (rt.Height() - nHeigh*j) / 2;

	switch (nShowNum)
	{
	case 1:
	{
		m_pVideoWindows[0]->MoveWindow(nLeft, nTop, nWidth, nHeigh);
		m_pVideoWindows[0]->ShowWindow(SW_SHOW);
		break;
	}
	case 4:
	{
		for (int m = 0; m < 2; m++)
		{
			for (int n = 0; n < 2; n++)
			{
				m_pVideoWindows[m * 2 + n]->MoveWindow(nLeft + nWidth*n, nTop + nHeigh*m, nWidth, nHeigh);
				m_pVideoWindows[m * 2 + n]->ShowWindow(SW_SHOW);
			}
		}
		break;
	}
	case 6:
	{
		m_pVideoWindows[0]->MoveWindow(nLeft + nWidth * 0, nTop + nHeigh * 0, nWidth * 2, nHeigh * 2);
		m_pVideoWindows[0]->ShowWindow(SW_SHOW);

		m_pVideoWindows[1]->MoveWindow(nLeft + nWidth * 2, nTop + nHeigh * 0, nWidth, nHeigh);
		m_pVideoWindows[1]->ShowWindow(SW_SHOW);

		m_pVideoWindows[2]->MoveWindow(nLeft + nWidth * 2, nTop + nHeigh * 1, nWidth, nHeigh);
		m_pVideoWindows[2]->ShowWindow(SW_SHOW);

		m_pVideoWindows[3]->MoveWindow(nLeft + nWidth * 0, nTop + nHeigh * 2, nWidth, nHeigh);
		m_pVideoWindows[3]->ShowWindow(SW_SHOW);
		m_pVideoWindows[4]->MoveWindow(nLeft + nWidth * 1, nTop + nHeigh * 2, nWidth, nHeigh);
		m_pVideoWindows[4]->ShowWindow(SW_SHOW);
		m_pVideoWindows[5]->MoveWindow(nLeft + nWidth * 2, nTop + nHeigh * 2, nWidth, nHeigh);
		m_pVideoWindows[5]->ShowWindow(SW_SHOW);

		break;
	}

	case 8:
	{
		m_pVideoWindows[0]->MoveWindow(nLeft + nWidth * 0, nTop + nHeigh * 0, nWidth * 3, nHeigh * 3);
		m_pVideoWindows[0]->ShowWindow(SW_SHOW);

		m_pVideoWindows[1]->MoveWindow(nLeft + nWidth * 3, nTop + nHeigh * 0, nWidth, nHeigh);
		m_pVideoWindows[1]->ShowWindow(SW_SHOW);
		m_pVideoWindows[2]->MoveWindow(nLeft + nWidth * 3, nTop + nHeigh * 1, nWidth, nHeigh);
		m_pVideoWindows[2]->ShowWindow(SW_SHOW);
		m_pVideoWindows[3]->MoveWindow(nLeft + nWidth * 3, nTop + nHeigh * 2, nWidth, nHeigh);
		m_pVideoWindows[3]->ShowWindow(SW_SHOW);

		m_pVideoWindows[4]->MoveWindow(nLeft + nWidth * 0, nTop + nHeigh * 3, nWidth, nHeigh);
		m_pVideoWindows[4]->ShowWindow(SW_SHOW);
		m_pVideoWindows[5]->MoveWindow(nLeft + nWidth * 1, nTop + nHeigh * 3, nWidth, nHeigh);
		m_pVideoWindows[5]->ShowWindow(SW_SHOW);
		m_pVideoWindows[6]->MoveWindow(nLeft + nWidth * 2, nTop + nHeigh * 3, nWidth, nHeigh);
		m_pVideoWindows[6]->ShowWindow(SW_SHOW);
		m_pVideoWindows[7]->MoveWindow(nLeft + nWidth * 3, nTop + nHeigh * 3, nWidth, nHeigh);
		m_pVideoWindows[7]->ShowWindow(SW_SHOW);

		break;
	}
	case 9:
	{
		for (int m = 0; m < 3; m++)
		{
			for (int n = 0; n < 3; n++)
			{
				m_pVideoWindows[m * 3 + n]->MoveWindow(nLeft + nWidth*n, nTop + nHeigh*m, nWidth, nHeigh);
				m_pVideoWindows[m * 3 + n]->ShowWindow(SW_SHOW);
			}
		}
		break;
	}
	case 16:
	{
		for (int m = 0; m < 4; m++)
		{
			for (int n = 0; n < 4; n++)
			{
				m_pVideoWindows[m * 4 + n]->MoveWindow(nLeft + nWidth*n, nTop + nHeigh*m, nWidth, nHeigh);
				m_pVideoWindows[m * 4 + n]->ShowWindow(SW_SHOW);
			}
		}
		break;
	}
	case 25:
	{
		for (int m = 0; m < 5; m++)
		{
			for (int n = 0; n < 5; n++)
			{
				m_pVideoWindows[m * 5 + n]->MoveWindow(nLeft + nWidth*n, nTop + nHeigh*m, nWidth, nHeigh);
				m_pVideoWindows[m * 5 + n]->ShowWindow(SW_SHOW);
			}
		}
		break;
	}

	case 36:
	{
		for (int m = 0; m < 6; m++)
		{
			for (int n = 0; n < 6; n++)
			{
				m_pVideoWindows[m * 6 + n]->MoveWindow(nLeft + nWidth*n, nTop + nHeigh*m, nWidth, nHeigh);
				m_pVideoWindows[m * 6 + n]->ShowWindow(SW_SHOW);
			}
		}
		break;
	}
	case 49:
	{
		for (int m = 0; m < 7; m++)
		{
			for (int n = 0; n < 7; n++)
			{
				m_pVideoWindows[m * 7 + n]->MoveWindow(nLeft + nWidth*n, nTop + nHeigh*m, nWidth, nHeigh);
				m_pVideoWindows[m * 7 + n]->ShowWindow(SW_SHOW);
			}
		}
		break;
	}
	case 64:
	{
		for (int m = 0; m < 8; m++)
		{
			for (int n = 0; n < 8; n++)
			{
				m_pVideoWindows[m * 8 + n]->MoveWindow(nLeft + nWidth*n, nTop + nHeigh*m, nWidth, nHeigh);
				m_pVideoWindows[m * 8 + n]->ShowWindow(SW_SHOW);
			}
		}
		break;
	}
	default:
		break;
	}
	//
// 	m_pVideoWindows[nShowSel]->m_bFoucs = TRUE;
// 	m_pVideoWindows[nShowSel]->DrawFrameRect();
	//
	return 0;
}
void CVideoLoopplayManageDlg::PostNcDestroy()
{
	delete this;
	CDialogEx::PostNcDestroy();
}
void CVideoLoopplayManageDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonDown(nFlags, point);
}
void CVideoLoopplayManageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
}
void CVideoLoopplayManageDlg::StartLoopplay()
{
	m_stList.clear();
	//
	m_WaitSecond = theApp.m_clientSetting.loopplay_sec;
	m_wndNum = theApp.m_clientSetting.loopplay_wnd;
	m_bExit = FALSE;
	m_bPause = FALSE;
	ShowVideoWindows(m_wndNum);
	//
	
	for (int i = 0; i < theApp.m_clientSetting.loopplaycount; i++)
	{

		if (theApp.m_ClientType == X_CLIENT_XXX_TYPE_CLASSIC)
		{
			int v = atoi(theApp.m_clientSetting.loopid[i]);
			stDevice st;
			CDeviceManager::Instance().GetDeviceByIndex(v, st);
			m_stList.push_back(st);
		}

	}
	//
// 	for (vector<stDevice>::iterator it = stList.begin(); it != stList.end(); it++)
// 	{
// 		stDevice st = *it;
// 		m_stList.push_back(st);
// 	}
	//
	m_hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadWoker, this, NULL, NULL);
}
void CVideoLoopplayManageDlg::PauseLoopplay()
{
	m_bPause = !m_bPause;
}
void CVideoLoopplayManageDlg::StopLoopplay()
{
	if (m_hThread)
	{
		m_bExit = TRUE;
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;
		//
		for (int i = 0; i< 16; i++)
		{
			m_pVideoWindows[i]->StopPreview();
		}
		//
		m_stList.clear();
	}
}

void CVideoLoopplayManageDlg::ThreadWoker(void * param)
{
	CVideoLoopplayManageDlg *p = (CVideoLoopplayManageDlg *)param;
	p->Woker();
}
void CVideoLoopplayManageDlg::Woker()
{
	DWORD64 de = GetTickCount64();

	int __connt = (int)m_stList.size();
	int __pagesize = m_wndNum;
	int __page = (int)ceil((double)__connt / (double)__pagesize);
	int __curpage = 0;


	while (!m_bExit)
	{
		//==============================================================
		if (m_bPause)
		{
			Sleep(10);
			continue;
		}
		//==============================================================
		for (int m = 0; m < __pagesize; m++)
		{
			int sel = ((__curpage * __pagesize) + m);

			if (sel >= __connt)
			{
				break;
			}
			//
			//
			//
			//
			m_pVideoWindows[m]->StopPreview();
			m_pVideoWindows[m]->StartPreview(m_stList[sel]);
		}
		//==============================================================
		__curpage++;
		if (__curpage >= __page) //__curpage是从0开始的，所以 >=
		{
			__curpage = 0;
		}
		//==============================================================
		for (int i = 0; i < m_WaitSecond * 10; i++)
		{
			if (m_bExit)
			{
				break;
			}
			Sleep(100);
		}
		//==============================================================
	}


}