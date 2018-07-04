// VideoManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "VideoManageDlg.h"
#include "afxdialogex.h"
#include "PlatformClientDlg.h"

// CVideoManageDlg dialog

IMPLEMENT_DYNAMIC(CVideoManageDlg, CDialogEx)

CVideoManageDlg::CVideoManageDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CVideoManageDlg::IDD, pParent)
{
	m_bShowMax = FALSE;
	m_pWnd = pParent;
	m_nFocusIndex = 0;
	for (int i = 0; i < MAX_WINDOWS_NUM; i++){
		m_pVideoWindows[i] = NULL;
	}
}
CVideoManageDlg::~CVideoManageDlg()
{
}
void CVideoManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideoManageDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CVideoManageDlg::InitPanlLiveWnd(CWnd *pParent)
{
	m_pWnd = pParent;
	for (int i = 0; i < MAX_WINDOWS_NUM; i++){
		m_pVideoWindows[i]->m_pAfxMainWnd = m_pWnd;
	}
	return TRUE;
}

// int CVideoManageDlg::UninitPanlLiveWnd(void)
// {
// 	for (int i = 0; i < MAX_WINDOWS_NUM; i++){
// 		m_pVideoWindows[i]->m_bThreadExit = TRUE;
// 	}
// 	for (int i = 0; i < MAX_WINDOWS_NUM; i++)
// 	{
// 		if (m_pVideoWindows[i]->m_hThread)
// 		{
// 			WaitForSingleObject(m_pVideoWindows[i]->m_hThread, INFINITE);
// 			CloseHandle(m_pVideoWindows[i]->m_hThread);
// 			m_pVideoWindows[i]->m_hThread = NULL;
// 		}
// 	}
// 	return TRUE;
// }
BOOL CVideoManageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_nShow = theApp.m_clientSetting.spiltScreen;//////////////////////////////////////////////////////////////////////////
	m_nFocusIndex = 0;
	for (int i = 0; i < MAX_WINDOWS_NUM; i++)
	{
		m_pVideoWindows[i] = new CVideoViewDlg();
		m_pVideoWindows[i]->Create(CVideoViewDlg::IDD, this);
		m_pVideoWindows[i]->MoveWindow(0, 0, 0, 0);
		m_pVideoWindows[i]->ShowWindow(SW_HIDE);
		m_pVideoWindows[i]->m_pParent = this;
		m_pVideoWindows[i]->m_nIndex = i;
		m_pVideoWindows[i]->m_bShowMax = FALSE;
		m_pVideoWindows[i]->m_pAfxMainWnd = m_pWnd;
	}
	m_pVideoWindows[0]->m_bFoucs = TRUE;
	m_pVideoWindows[0]->MoveWindow(0, 0, 100, 100);
	m_pVideoWindows[0]->ShowWindow(SW_SHOW);
	TRACE(" ###>>> m_pVideoWindows[] Create ok\n");
	return TRUE;
}
BOOL CVideoManageDlg::PreTranslateMessage(MSG* pMsg)
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
void CVideoManageDlg::OnSize(UINT nType, int cx, int cy)
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
HBRUSH CVideoManageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CVideoManageDlg::ClearAllSubWindowFoucs()
{
	for (int i = 0; i < MAX_WINDOWS_NUM; i++){
		m_pVideoWindows[i]->m_bFoucs = FALSE;
		m_pVideoWindows[i]->DrawFrameRect();
	}
}
void CVideoManageDlg::SetFocusViewIndex(int i)
{
	m_nFocusIndex = i;
}
int CVideoManageDlg::ShowVideoWindowMax(int nInex, BOOL bShowMax)
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
		m_pVideoWindows[nInex]->m_bFoucs = TRUE;
		m_pVideoWindows[nInex]->DrawFrameRect();
	}
	else
	{
		ShowVideoWindows(m_nShow);
	}

	return 0;
}
int CVideoManageDlg::ShowVideoWindows(int nShowNum)
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
		m_pVideoWindows[m_nFocusIndex]->m_bFoucs = TRUE;
		m_pVideoWindows[m_nFocusIndex]->DrawFrameRect();
		return 0;
	}

	//
	ClearAllSubWindowFoucs();
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
	m_pVideoWindows[nShowSel]->m_bFoucs = TRUE;
	m_pVideoWindows[nShowSel]->DrawFrameRect();
	//
	return 0;
}
void CVideoManageDlg::PostNcDestroy()
{
	delete this;
	CDialogEx::PostNcDestroy();
}
void CVideoManageDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonDown(nFlags, point);
}
void CVideoManageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
}
int CVideoManageDlg::GetNextOpenWindows()
{
	int lll = m_nFocusIndex;
	//
	m_nFocusIndex++;
	//
	switch (m_nShow)
	{
	case 1:
		if (m_nFocusIndex > 0)
		{
			m_nFocusIndex = 0;
		}
		break;
	case 4:
		if (m_nFocusIndex > 3)
		{
			m_nFocusIndex = 0;
		}
		break;
	case 6:
		if (m_nFocusIndex > 5)
		{
			m_nFocusIndex = 0;
		}
		break;
	case 8:
		if (m_nFocusIndex > 7)
		{
			m_nFocusIndex = 0;
		}
		break;
	case 9:
		if (m_nFocusIndex > 8)
		{
			m_nFocusIndex = 0;
		}
		break;
	case 16:
		if (m_nFocusIndex > 15)
		{
			m_nFocusIndex = 0;
		}
		break;
	default:
		break;
	}
	//
	return lll;
}


//窗口交换，暂时去掉
// int CVideoManageDlg::ExchangeWindowsPosition(int nOldIndex)
// {
// 	CPoint pt;
// 	GetCursorPos(&pt);
// 	for (int nNewIndex = 0; nNewIndex < MAX_WINDOWS_NUM; nNewIndex++)
// 	{
// 		CRect rt;
// 		m_pVideoWindows[nNewIndex]->GetWindowRect(&rt);
// 		if (rt.PtInRect(pt))
// 		{
// 			if (nNewIndex == nOldIndex)
// 				return 0;
// 
// 			CRect rcFrom;
// 			CRect rcTo;
// 			m_pVideoWindows[nOldIndex]->GetWindowRect(rcFrom);
// 			m_pVideoWindows[nNewIndex]->GetWindowRect(rcTo);
// 			ScreenToClient(rcFrom);
// 			ScreenToClient(rcTo);
// 			m_pVideoWindows[nNewIndex]->MoveWindow(rcFrom);
// 			m_pVideoWindows[nOldIndex]->MoveWindow(rcTo);
// 
// 
// 			CVideoViewDlg * ptemp = m_pVideoWindows[nOldIndex];
// 			m_pVideoWindows[nOldIndex] = m_pVideoWindows[nNewIndex];
// 			m_pVideoWindows[nOldIndex]->m_nIndex = nOldIndex;
// 
// 			m_pVideoWindows[nNewIndex] = ptemp;
// 			m_pVideoWindows[nNewIndex]->m_nIndex = nNewIndex;
// 
// 			break;
// 		}
// 	}
// 
// 	return 0;
// }
