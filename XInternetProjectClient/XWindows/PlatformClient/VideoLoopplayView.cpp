// VideoViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "VideoLoopplayView.h"
#include "PlatformClientDlg.h"


//#include "PlatformClientDlg.h"

extern CPlatClientApp theApp;

IMPLEMENT_DYNAMIC(CVideoLoopplayView, CDialogEx)




CVideoLoopplayView::CVideoLoopplayView(CWnd* pParent /*=NULL*/)
: CDialogEx(CVideoLoopplayView::IDD, pParent)
{
	m_hThread = nullptr;
	m_pStreamObject = nullptr;
}

CVideoLoopplayView::~CVideoLoopplayView()
{

}

void CVideoLoopplayView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_view);
	DDX_Control(pDX, IDC_STATIC_LOADING, m_loadingview);
}

BEGIN_MESSAGE_MAP(CVideoLoopplayView, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

BOOL CVideoLoopplayView::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		UINT nKey = (int)pMsg->wParam;
		if (VK_RETURN == nKey){
			return TRUE;
		}

		if (VK_ESCAPE == nKey){
			CPlatClientDlg *pWnd = (CPlatClientDlg *)m_pAfxMainWnd;
			if (pWnd->m_fFullScreen)
			{
				pWnd->ShowWindowMax(FALSE);
			}
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CVideoLoopplayView::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_loadingview.ShowWindow(SW_HIDE);
	ShowLoadingGif(FALSE);
	if (m_loadingview.Load(MAKEINTRESOURCE(IDR_GIF1), _T("gif")))//IDR_GIF1是添加的GIF动画的ID号。gif即是在添加时所键入的文件类型
		m_loadingview.Draw();
	//
	SendMessage(WM_SIZE, 0, 0);
	//
	m_view.SetWindowText(L"无信号");
	return TRUE;
}
void CVideoLoopplayView::OnDestroy()
{
	CDialog::OnDestroy();
}
void CVideoLoopplayView::PostNcDestroy()
{
	delete this;
	CDialog::PostNcDestroy();
}

BOOL CVideoLoopplayView::OnEraseBkgnd(CDC* pDC)
{
	return CDialog::OnEraseBkgnd(pDC);
}

void CVideoLoopplayView::OnPaint()
{
	CPaintDC dc(this);
}

HBRUSH CVideoLoopplayView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pDC == NULL || pDC->m_hDC == NULL)
	{
		return hbr;
	}
	// 	if (hbr != NULL)
	// 	{
	// 		::DeleteObject(hbr);
	// 	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TITLE)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));
		//return (HBRUSH)theApp.hViewTitleBrushBg;// GetStockObject(NULL_BRUSH);
		return (HBRUSH)theApp.hViewTitleBrushBg;
	}

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_VIEW)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));
		return (HBRUSH)theApp.hViewBrushBg;
	}
	// 
	// 
	return (HBRUSH)theApp.hViewTitleBrushBg;// GetStockObject(NULL_BRUSH);


	return hbr;
}

void CVideoLoopplayView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	//
	if (GetSafeHwnd() == NULL){
		return;
	}

	if (m_view.GetSafeHwnd() == NULL){
		return;
	}
	CRect rt;
	GetClientRect(rt);
	//---
	m_view.MoveWindow(1, 1, rt.Width() - 1, rt.Height() - 1);
	m_loadingview.MoveWindow(1, 1, 16, 16);
}

void CVideoLoopplayView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDblClk(nFlags, point);
}




void CVideoLoopplayView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDown(nFlags, point);
}

void CVideoLoopplayView::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnMouseMove(nFlags, point);
}
void CVideoLoopplayView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonUp(nFlags, point);
}
void CVideoLoopplayView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnRButtonDown(nFlags, point);
}
void CVideoLoopplayView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}
LRESULT CVideoLoopplayView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CDialogEx::WindowProc(message, wParam, lParam);
}
void CVideoLoopplayView::ShowLoadingGif(BOOL bTrue)
{

	if (bTrue)
	{
		m_view.Invalidate();
	}
	m_loadingview.ShowWindow(bTrue ? SW_SHOW : SW_HIDE);
}


void CVideoLoopplayView::StartPreview(stDevice dev)
{
	StopPreview();
	//
	memcpy(&m_stDevice, &dev, sizeof(m_stDevice));
	//
	m_bExit = FALSE;
	m_hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadWoker, this, NULL, NULL);
}
void CVideoLoopplayView::StopPreview()
{
	if (m_hThread)
	{
		m_bExit = TRUE;
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;

	}
}

void CVideoLoopplayView::ThreadWoker(void * param)
{
	CVideoLoopplayView *p = (CVideoLoopplayView *)param;
	p->Woker();
}
void CVideoLoopplayView::Woker()
{
	XViewOpenStream();
	while (!m_bExit)
	{
		if (m_pStreamObject)//----------------------------------看视频的时候
		{
			emClientNetStatus em = m_pStreamObject->XClientStreamGetStatus();
			if (0 == CLIENT_NET_STATUS_CONNECTING)
			{
				ShowLoadingGif(TRUE);
			}
			else
			{
				ShowLoadingGif(FALSE);
			}
		}
		Sleep(10);
	}
	XViewCloseStream();
}


int CVideoLoopplayView::XViewOpenStream()
{
	//
	XViewCloseStream();
	//
	do
	{
		//
		//m_view.Invalidate();
		//m_view.SetWindowText(L"正在连接！");
		ShowLoadingGif(TRUE);
		//
		char szhhhh[32] = { 0 };
		sprintf_s(szhhhh, "%d", m_stDevice.classicinfo.dev_id);
		//
		m_pStreamObject = theApp.m_pClientInferface->CreateXClientStreamInterface(szhhhh,0);// pDevFun();
		//m_pStreamObject->XClientStreamSetInfo(theApp.m_strip.c_str(), 6503, theApp.m_strName.c_str(),theApp.m_strPwd.c_str(), m_stDevice.classicinfo.dev_id, 0, nullptr);
		m_pStreamObject->XClientStreamPlay(m_view.GetSafeHwnd());
	} while (0);
	//
	return 0;
}

int CVideoLoopplayView::XViewCloseStream()
{
	if (m_pStreamObject)
	{
		m_pStreamObject->XClientStreamClose();
		m_pStreamObject->XDelete();
		m_pStreamObject = NULL;
	}
	ShowLoadingGif(FALSE);
	//m_view.Invalidate();
	//m_view.SetWindowText(L"");
	return 0;

}
