// VideoViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "VideoViewDlg.h"
#include "afxdialogex.h"

#include "VideoManageDlg.h"
#include "ZPanLive.h"

//#include "PlatformClientDlg.h"

extern CPlatClientApp theApp;

IMPLEMENT_DYNAMIC(CVideoViewDlg, CDialogEx)

void check(void * param)
{
	CVideoViewDlg *p = (CVideoViewDlg *)param;
	if (NULL != p)
	{
		p->CheckStatus();
	}
}


CVideoViewDlg::CVideoViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoViewDlg::IDD, pParent)
{
	m_bFoucs = FALSE;
	//m_bLbtnDown = FALSE;
	//m_bDragging = FALSE;
//	UserParam = NULL;
//	m_dwMouseMoveTick = 0;
//	m_bMouseMove = FALSE;
	m_bCanShowCtrlPan = FALSE;
//	m_nMouseMoveCount = 0;
	m_pCtrlPanDlg = NULL;
	m_pStreamObject = NULL;
}

CVideoViewDlg::~CVideoViewDlg()
{

}

void CVideoViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_view);

	DDX_Control(pDX, IDC_STATIC_LOADING, m_loadingview);
	//DDX_Control(pDX, IDC_BTN_CLOSE, m_closebtn);
}

BEGIN_MESSAGE_MAP(CVideoViewDlg, CDialogEx)
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

	//ON_BN_CLICKED(IDC_BTN_CLOSE, &CVideoViewDlg::OnBnClickedBtnClose)
END_MESSAGE_MAP()

BOOL CVideoViewDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		UINT nKey = (int)pMsg->wParam;
		if (VK_RETURN == nKey){
			return TRUE;
		}

		if (VK_ESCAPE == nKey){

			// 			CPlatClientDlg *pWnd = (CPlatClientDlg *)m_pWnd;
			// 			if (pWnd->m_fFullScreen)
			// 			{
			// 				pWnd->ShowWindowMax(FALSE);
			// 			}
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CVideoViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//SetBackgroundColor(RGB(55, 84,102), TRUE);

	//----------------------------------------------------------
	m_bVideoError = FALSE;
	m_bThreadExit = FALSE;
	//---------------------------------------------
	// 	m_pDragImage = new CImageList();
	// 	HRESULT ret = img.Load(theApp.m_strSkinPath + L"viewpan/PanMove.bmp"); // filename 是要加载的文件名（包含路径） 
	// 	HBITMAP bitmap = img.Detach();
	// 	pcBmp = CBitmap::FromHandle(bitmap);//CBitmap 
	// 	m_pDragImage->Create(35, 35, ILC_COLORDDB | ILC_MASK, 1, 1);
	// 	m_pDragImage->Add(pcBmp, RGB(255, 255, 255));
	// 	pcBmp->Detach();

	//  [2/29/2016 xjm] ----------------------------------------
//	m_bShowPtzDlg = FALSE;
//	m_pPtzDlg = NULL;
	//-------------

	//  [5/5/2016 xjm]-------------------------------------------
	m_pCtrlPanDlg = new CVideoViewCtrlButton(this);
	m_pCtrlPanDlg->Create(CVideoViewCtrlButton::IDD,this);
	m_pCtrlPanDlg->ShowWindow(m_bCanShowCtrlPan ? SW_SHOW : SW_HIDE);
	//------------------
	m_bShowLoading = FALSE;
	m_loadingview.ShowWindow(SW_HIDE);
	ShowLoadingGif(FALSE);
	if (m_loadingview.Load(MAKEINTRESOURCE(IDR_GIF1), _T("gif")))//IDR_GIF1是添加的GIF动画的ID号。gif即是在添加时所键入的文件类型
		m_loadingview.Draw();
	//
	//ButtonLoadBitmap(m_closebtn, L"viewpan/close_1.png", L"viewpan/close_2.png", L"viewpan/close_3.png", L"viewpan/close_4.png");
	//m_closebtn.ShowWindow(SW_HIDE);
	//


	SendMessage(WM_SIZE, 0, 0);

	return TRUE;
}
void CVideoViewDlg::OnDestroy()
{
	CDialog::OnDestroy();
}
void CVideoViewDlg::PostNcDestroy()
{
	// 	m_bDragging = FALSE;
	// 	m_pDragImage->DragLeave(this);
	// 	m_pDragImage->EndDrag();
	// 	m_pDragImage->Detach();
	// 	ReleaseCapture();
	// 	delete m_pDragImage;

	//	DeleteObject(hbrushView);
	//	DeleteObject(hbrushTitle);
	//	DeleteObject(hbrushBg);

	delete this;
	CDialog::PostNcDestroy();
}

BOOL CVideoViewDlg::OnEraseBkgnd(CDC* pDC)
{
	return CDialog::OnEraseBkgnd(pDC);
}

void CVideoViewDlg::OnPaint()
{
	CPaintDC dc(this);

	// 	CRect rt;
	// 	GetClientRect(&rt);
	// 
	// 	//rt.bottom = rt.top + m_nCloseHeight;
	// 
	// 	HBRUSH hbrushOld;
	// 
	// 	HDC hdc = ::GetDC(m_hWnd);
	// 	hbrushOld = (HBRUSH)SelectObject(hdc, theApp.hViewTitleBrushBg);
	// 
	// 	FillRect(hdc, &rt, theApp.hViewTitleBrushBg);
	// 
	// 	SelectObject(hdc, hbrushOld);
	// 	//DeleteObject(hbrush);
	// 	DeleteObject(hbrushOld);
	// 	::ReleaseDC(m_hWnd, hdc);

	// 	m_title.Invalidate();
	// 	m_btn_sound.Invalidate();
	// 	m_btn_talk.Invalidate();
	// 	m_btn_record.Invalidate();
	// 	m_btn_cap.Invalidate();
	// 	m_btn_close.Invalidate();
	// 	m_btnSet.Invalidate();

	DrawFrameRect();


	if (m_bVideoError)
	{

	}
}

HBRUSH CVideoViewDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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



void CVideoViewDlg::XOnMove()
{
	SetCtrlPanPos();


// 	if (m_pCtrlPanDlg == NULL){
// 		return;
// 	}
// 
// 	CRect rt;
// 	GetClientRect(&rt);
// 	//--
// 	GetWindowRect(&rt);
// 	int panWidth = 150;//(0 != theApp.m_clientSetting.server_enable) ? 150 : 220;
// 
// 	int panHeight = 32;
// 	int panX = rt.left + (rt.Width() / 2 - panWidth / 2);
// 	int panY = rt.top + (rt.Height() - panHeight - 4);
// 	m_pCtrlPanDlg->MoveWindow(panX, panY, panWidth, panHeight);
// 	m_pCtrlPanDlg->SendMessage(WM_SIZE, 0, 0);
// 	//-
// 	if (m_bShowPtzDlg && m_pPtzDlg)
// 	{
// 		m_bShowPtzDlg = !m_bShowPtzDlg;
// 		m_pPtzDlg->ShowWindow(m_bShowPtzDlg ? SW_SHOW : SW_HIDE);
// 	}

}

void CVideoViewDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	//
	if (GetSafeHwnd() == NULL){
		return;
	}
	if (m_pCtrlPanDlg == NULL){
		return;
	}
	CRect rt;
	GetClientRect(rt);
	//---
	m_view.MoveWindow(1, 1, rt.Width() - 2, rt.Height() - 2);
	m_loadingview.MoveWindow(1, 1, 16, 16);
	//m_closebtn.MoveWindow(rt.Width() - 28 - 1, 1, 28, 24);
	//--

	if (rt.Width() > 0)
	{
		SetCtrlPanPos();
	}
	else
	{
		//m_pPanDlg->ShowWindow(SW_HIDE);
		//m_dwMouseMoveTick = 0;
	}

	m_bCanShowCtrlPan = FALSE;
	m_pCtrlPanDlg->ShowWindow(m_bCanShowCtrlPan ? SW_SHOW : SW_HIDE);


	//--
	//m_pPanDlg->ShowWindow(m_bPanShow ? SW_SHOW : SW_HIDE);


	// 
	// 	if (m_EnableButtnState)
	// 	{
	// #ifdef title_aotu_hide
	// 
	// 		if (m_bTitleIsShow)
	// 		{
	// 			m_view.MoveWindow(1, 1, rt.Width() - 2, rt.Height() - m_nCloseHeight - 2);
	// 			m_title.ShowWindow(SW_NORMAL);
	// 			m_btn_sound.ShowWindow(SW_NORMAL);
	// 			m_btn_talk.ShowWindow(SW_NORMAL);
	// 			m_btn_record.ShowWindow(SW_NORMAL);
	// 			m_btn_cap.ShowWindow(SW_NORMAL);
	// 			m_btn_close.ShowWindow(SW_NORMAL);
	// 			m_btnSet.ShowWindow(SW_NORMAL);
	// 			if (theApp.m_clientSetting.server_enable)
	// 			{
	// 				m_btnSet.ShowWindow(SW_HIDE);
	// 				m_btn_talk.ShowWindow(SW_HIDE);
	// 			}
	// 		}
	// 		else
	// 		{
	// 			m_view.MoveWindow(1, 1, rt.Width() - 2, rt.Height() - 2);
	// 			m_title.ShowWindow(SW_HIDE);
	// 			m_btn_sound.ShowWindow(SW_HIDE);
	// 			m_btn_talk.ShowWindow(SW_HIDE);
	// 			m_btn_record.ShowWindow(SW_HIDE);
	// 			m_btn_cap.ShowWindow(SW_HIDE);
	// 			m_btn_close.ShowWindow(SW_HIDE);
	// 			m_btnSet.ShowWindow(SW_HIDE);
	// 		}
	// #else
	// 		m_view.MoveWindow(1, 1, rt.Width() - 2, rt.Height() - m_nCloseHeight - 2);
	// #endif
	// 
	// 		if (0 == theApp.m_clientSetting.server_enable)
	// 		{
	// 			m_title.MoveWindow(1, rt.Height() - m_nCloseHeight, rt.Width() - m_nCloseWidth * 6 - Btn_Space * 5 - 3, m_nCloseHeight - 1);
	// 			//
	// 			m_btnSet.SetWindowPos(&wndTop, rt.Width() - m_nCloseWidth * 6 - Btn_Space * 5 - 2, rt.Height() - m_nCloseHeight - 1, m_nCloseWidth, m_nCloseHeight, SWP_NOSIZE);
	// 			m_btn_talk.SetWindowPos(&wndTop, rt.Width() - m_nCloseWidth * 5 - Btn_Space * 4 - 2, rt.Height() - m_nCloseHeight - 1, 0, 0, SWP_NOSIZE);
	// 			m_btn_sound.SetWindowPos(&wndTop, rt.Width() - m_nCloseWidth * 4 - Btn_Space * 3 - 2, rt.Height() - m_nCloseHeight - 1, 0, 0, SWP_NOSIZE);
	// 			m_btn_record.SetWindowPos(&wndTop, rt.Width() - m_nCloseWidth * 3 - Btn_Space * 2 - 2, rt.Height() - m_nCloseHeight - 1, 0, 0, SWP_NOSIZE);
	// 			m_btn_cap.SetWindowPos(&wndTop, rt.Width() - m_nCloseWidth * 2 - Btn_Space * 1 - 2, rt.Height() - m_nCloseHeight - 1, 0, 0, SWP_NOSIZE);
	// 			m_btn_close.SetWindowPos(&wndTop, rt.Width() - m_nCloseWidth * 1 - Btn_Space * 0 - 2, rt.Height() - m_nCloseHeight - 1, 0, 0, SWP_NOSIZE);
	// 		}
	// 		else
	// 		{
	// 			m_title.MoveWindow(1, rt.Height() - m_nCloseHeight, rt.Width() - m_nCloseWidth * 4 - Btn_Space * 3 - 3, m_nCloseHeight - 1);
	// 			//
	// 			m_btn_sound.SetWindowPos(&wndTop, rt.Width() - m_nCloseWidth * 4 - Btn_Space * 3 - 2, rt.Height() - m_nCloseHeight - 1, 0, 0, SWP_NOSIZE);
	// 			m_btn_record.SetWindowPos(&wndTop, rt.Width() - m_nCloseWidth * 3 - Btn_Space * 2 - 2, rt.Height() - m_nCloseHeight - 1, 0, 0, SWP_NOSIZE);
	// 			m_btn_cap.SetWindowPos(&wndTop, rt.Width() - m_nCloseWidth * 2 - Btn_Space * 1 - 2, rt.Height() - m_nCloseHeight - 1, 0, 0, SWP_NOSIZE);
	// 			m_btn_close.SetWindowPos(&wndTop, rt.Width() - m_nCloseWidth * 1 - Btn_Space * 0 - 2, rt.Height() - m_nCloseHeight - 1, 0, 0, SWP_NOSIZE);
	// 		}
	// 
	// 	}
	// 	else
	// 	{
	// 		m_view.MoveWindow(1, 1, rt.Width() - 2, rt.Height() - 2);
	// 	}
	// 
	// 	Invalidate();
}




int CVideoViewDlg::DrawFrameRect(void)
{

	if (!m_bFoucs)
	{
// 		if (m_pStreamObject && m_pPtzDlg)
// 		{
// 			//  [2/29/2016 xjm]
// 			m_bShowPtzDlg = FALSE;
// 			m_pPtzDlg->ShowWindow(m_bShowPtzDlg ? SW_SHOW : SW_HIDE);
// 			//
// 		}
	}
	else
	{

		//postmessage
		CZPanLive *pWnd = (CZPanLive *)m_pAfxMainWnd;
		pWnd->PostMessage(WM_VIDEO_SELECT_ITEM, (WPARAM)m_stDevice.hItem, (LPARAM)1);

		//pWnd->m_tree.SelectItem(pData->hitem);
	}




	CRect rt;
	GetClientRect(&rt);

	HBRUSH hbrush, hbrushOld;
	if (m_bFoucs)
		hbrush = CreateSolidBrush(RGB(theApp.m_cfg.bglr, theApp.m_cfg.bglg, theApp.m_cfg.bglb));
	else
		hbrush = CreateSolidBrush(RGB(theApp.m_cfg.bgvr, theApp.m_cfg.bgvg, theApp.m_cfg.bgvb));


	HDC hdc = ::GetDC(m_hWnd);
	hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);

	//FillRect(hdc,&rt,hbrush);

	FrameRect(hdc, &rt, hbrush);

	SelectObject(hdc, hbrushOld);
	::DeleteObject(hbrush);
	::DeleteObject(hbrushOld);
	::ReleaseDC(m_hWnd, hdc);

	return 0;
}

void CVideoViewDlg::drawSelected()
{
	CVideoManageDlg *pParent = (CVideoManageDlg *)m_pParent;
	pParent->ClearAllSubWindowFoucs();
	m_bFoucs = TRUE;
	pParent->SetFocusViewIndex(m_nIndex);
	DrawFrameRect();
}


void CVideoViewDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	drawSelected();

	m_bShowMax = !m_bShowMax;
	CVideoManageDlg *p = (CVideoManageDlg *)m_pParent;
	p->ShowVideoWindowMax(m_nIndex, m_bShowMax);

	CDialog::OnLButtonDblClk(nFlags, point);
}




void CVideoViewDlg::OnLButtonDown(UINT nFlags, CPoint point)
{


	drawSelected();


#if 0
	//  [1/29/2015 win8]
	//暂时禁止拖动，会造成索引错乱，以后解决
	CRect rc;
	CPoint pt = point;
	ScreenToClient(&pt);
	m_view.GetClientRect(&rc);
	if (rc.PtInRect(point))
	{
		m_bLbtnDown = TRUE;
	}
	CVideoManageDlg *p = (CVideoManageDlg *)m_pParent;
#endif


	CDialog::OnLButtonDown(nFlags, point);
}

void CVideoViewDlg::OnMouseMove(UINT nFlags, CPoint point)
{

	// 	if (m_bLbtnDown)
	// 	{
	// 		if (!m_bDragging)
	// 		{
	// 			m_bDragging = TRUE;
	// 			m_pDragImage->BeginDrag(0, CPoint(8, 8));
	// 			CPoint pt = point;
	// 			ClientToScreen(&pt);
	// 
	// 			m_pDragImage->DragEnter(NULL, pt);
	// 			SetCapture();
	// 		}
	// 		else
	// 		{
	// 			CPoint  pt;
	// 			GetCursorPos(&pt);
	// 			m_pDragImage->DragMove(pt);
	// 		}
	// 	}
	// 	else
// 	{
// 		m_nMouseMoveCount++;
// 		if (m_nMouseMoveCount > 10)
// 		{
// 
// 			m_dwMouseMoveTick = GetTickCount();
// 			m_bMouseMove = TRUE;
// 		}
// 
// 	}
	CDialog::OnMouseMove(nFlags, point);
}
void CVideoViewDlg::OnLButtonUp(UINT nFlags, CPoint point)
{

	// 	m_bLbtnDown = FALSE;
	// 
	// 	if (m_bDragging)
	// 	{
	// 		m_bDragging = FALSE;
	// 		m_pDragImage->DragLeave(this);
	// 		m_pDragImage->EndDrag();
	// 		ReleaseCapture();
	// 
	// 		CVideoManageDlg *p = (CVideoManageDlg *)m_pParent;
	// 		p->ExchangeWindowsPosition(m_nIndex);
	// 	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CVideoViewDlg::SetCtrlPanPos()
{
	CRect rt;
	GetClientRect(rt);
	ClientToScreen(rt);
	int panHeight = 24;
	int panX = rt.left + rt.Width() - 28 * 4 - 1;//
	int panY = rt.top;//
	//
	if (m_pCtrlPanDlg)
	{
		m_pCtrlPanDlg->MoveWindow(panX, panY, 28 * 4, panHeight);
		m_pCtrlPanDlg->SendMessage(WM_SIZE, 0, 0);
	}
}


void CVideoViewDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetCtrlPanPos();


	if (m_pStreamObject)
	{
		m_bCanShowCtrlPan = !m_bCanShowCtrlPan;
		m_pCtrlPanDlg->ShowWindow(m_bCanShowCtrlPan ? SW_SHOW : SW_HIDE);
	}

	// 	if ()
	// 	{
	// 
	// 	}
	// 	else
	// 	{
	// 
	// 	}


	drawSelected();
//	return;


// 
// 
// #if 0
// 	PLAY_WINDOW_DATA *pData = (PLAY_WINDOW_DATA *)UserParam;
// 	if (pData->pDevObject)
// 	{
// 		if (0 != theApp.m_clientSetting.server_enable)
// 		{
// 			CXTranTransmissionInterface * pTrans = pData->pDevObject->CreateTranTransmissionObject();
// 			pTrans->OpenTranTransmission(pData->_id);
// 			Sleep(5000);
// 			pTrans->TransparentTransmission(NULL, 0);
// 
// 			Sleep(100);
// 			pTrans->CloseTranTransmission();
// 			pTrans->DeleteTranTransmission();
// 		}
// 
// 
// 
// 
// 
// 
// 	}
// #else
// 	//if (0 == theApp.m_clientSetting.server_enable)
// 	{
// 		//  [2/29/2016 xjm]
// 		if (m_pStreamObject)
// 		{
// 
// 			if (!m_pPtzDlg)
// 			{
// #if THIS_MY_TEST_CLIENT
// 				//m_pPtzDlg = new CVideoViewPtzDlg(this, theApp.m_clientSetting.server_enable ? pData->_id : 0);
// 
// 				m_pPtzDlg = new CVideoViewPtzDlg(this, pData->_id);
// #endif
// 
// 
// 
// 
// 				m_pPtzDlg->m_pDevObject = m_pStreamObject;
// 
// 
// 				m_pPtzDlg->Create(CVideoViewPtzDlg::IDD);
// 				m_pPtzDlg->ShowWindow(m_bShowPtzDlg ? SW_SHOW : SW_HIDE);
// 			}
// 
// 			m_pPtzDlg->m_pDevObject = m_pStreamObject;
// 			m_bShowPtzDlg = !m_bShowPtzDlg;
// 			m_pPtzDlg->ShowWindow(m_bShowPtzDlg ? SW_SHOW : SW_HIDE);
// 
// 
// 			CRect rc;
// 			//GetWindowRect(rc);
// 
// 			CPoint pt = point;
// 			ClientToScreen(&pt);
// 			rc.left = pt.x;// -20;
// 			rc.top = pt.y;// -20;
// 			rc.right = rc.left + 120;
// 			rc.bottom = rc.top + 200;
// 			m_pPtzDlg->MoveWindow(rc);
// 		}
// 	}
// #endif
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 	//

	CDialog::OnRButtonDown(nFlags, point);
}





// void CVideoViewDlg::EnableButtnState(BOOL bEnable)
// {
// 	m_bPreviewState = bEnable;
// 	m_closebtn.ShowWindow(m_bPreviewState?SW_SHOW:SW_HIDE);
// 
// 	if (!m_bPreviewState)
// 	{
// 		m_pPanDlg->ShowWindow(SW_HIDE);
// 		ShowLoadingGif(FALSE);
// 	}
// 	else
// 	{
// 		ShowLoadingGif(TRUE);
// 	}
// }

void CVideoViewDlg::SetTitle(const char *szTitle)
{
	WCHAR wszInfo[256];
	MultiByteToWideChar(CP_UTF8, 0, szTitle, strlen(szTitle) + 1,
		wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
	m_pCtrlPanDlg->m_title.SetWindowText(wszInfo);
}

void CVideoViewDlg::SetVideoError(const char *szTitle)
{
	WCHAR wszInfo[256];
	MultiByteToWideChar(CP_UTF8, 0, szTitle, strlen(szTitle) + 1,
		wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
	m_view.SetWindowText(wszInfo);
	m_bVideoError = TRUE;
	m_view.Invalidate();
}
BOOL CVideoViewDlg::GetRecordStatus()
{
	return m_pCtrlPanDlg->m_bRecord;
}


void CVideoViewDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

LRESULT CVideoViewDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_MENU_BTN_CLICKED == message)
	{
		drawSelected();
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}


//#include "ParamDlg.h"
void CVideoViewDlg::ParamSet()
{
	// 	PLAY_WINDOW_DATA *pData = (PLAY_WINDOW_DATA *)UserParam;
	// 
	// 	MyEnumDeviceLibary em;
	// 	switch (pData->library)
	// 	{
	// 	case 0:
	// 		em = LIBRARY_MODULE_ONVIF;
	// 		break;
	// 	case 1:
	// 		em = LIBRARY_MODULE_YYJ;
	// 		break;
	// 	case 2:
	// 		em = LIBRARY_MODULE_ZYX;
	// 		break;
	// 	case 3:
	// 		em = LIBRARY_MODULE_SRA_JIWEI;
	// 		break;
	// 	default:
	// 		break;
	// 	}
	// 
	// 
	// 	CParamDlg dlg(pData->pDevObject, em, this);
	// 	dlg.DoModal();
}




void CVideoViewDlg::CheckStatus()
{
	while (!m_bThreadExit)
	{
		if (m_pStreamObject)//----------------------------------看视频的时候
		{
			emClientNetStatus em = m_pStreamObject->XClientStreamGetStatus();
			if (em == CLIENT_NET_STATUS_CONNECTED)
			{
				ShowLoadingGif(FALSE);
				
			}
			else
			{
				ShowLoadingGif(TRUE);
			}


// 			if (m_bMouseMove)
// 			{
// 				if (!m_bPanShow)
// 				{
// 					m_bPanShow = TRUE;
// 					m_pPanDlg->ShowWindow(m_bPanShow ? SW_SHOW : SW_HIDE);
// 					m_dwMouseMoveTick = GetTickCount();
// 				}
// 				m_bMouseMove = FALSE;
// 			}
// 
// 			if ((GetTickCount() - m_dwMouseMoveTick) > 3800)
// 			{
// 				if (m_bPanShow)
// 				{
// 					m_bPanShow = FALSE;
// 					m_pPanDlg->ShowWindow(m_bPanShow ? SW_SHOW : SW_HIDE);
// 					//SendMessage(WM_SIZE, 0, 0);
// 				}
// 			}


			//////////////////////////////////////////////////////////////////////////
			//检测是否有流过来，显示隐藏等待状态
			// 			PLAY_WINDOW_DATA *pData = (PLAY_WINDOW_DATA *)UserParam;
			// 			if (pData->pDevObject)
			// 			{
			// 				int s = 0;
			// 				pData->pDevObject->XDeviceGetChannelParam(0, NULL, s, X_PARAM_TYPE_GET_DEVICE_PREVIEW_STATUS);
			// 				if (0 == s)
			// 				{
			// 					ShowLoadingGif(TRUE);
			// 				}
			// 				else
			// 				{
			// 					ShowLoadingGif(FALSE);
			// 				}
			// 			}
		}
		else
		{
			break;
		}

		// 		for (int i = 0; i < 30; i++)
		// 		{
		// 			if (m_bExit)
		// 			{
		// 				break;
		// 			}
		// 			CrossSleep(40);
		// 		}

		CrossSleep(100);
	}
}


void CVideoViewDlg::ShowLoadingGif(BOOL bTrue)
{
	if (bTrue == m_bShowLoading)
	{
		return;
	}
	m_bShowLoading = bTrue;
	if (m_bShowLoading)
	{
		m_view.Invalidate();
	}
	else
	{
		m_view.SetWindowText(L"");
	}
	m_loadingview.ShowWindow(m_bShowLoading ? SW_SHOW : SW_HIDE);
}




int CVideoViewDlg::XViewOpenStream(stDevice _stDevice)
{
	//
	XViewCloseStream();

	memcpy(&m_stDevice, &_stDevice, sizeof(_stDevice));
	//
	//stDevice st;
	//CDeviceManager::Instance().GetDeviceByUuid((char *)m_tree.GetItemData(m_tree.m_hSelectedItem), st);
	//
	//m_csOpenRealtime.Lock();
	//
	// 	if (nIndex <= -1 || nIndex >= MAX_WINDOWS_NUM)
	// 	{
	// 		TRACE("PersonalViewOpenStream error - 000 - %d\n", nIndex);
	// 		//m_csOpenRealtime.Unlock();
	// 		return -1;
	// 	}
	// 
	// 
	// 	void *p = NULL;
	// 	m_pVideoDlg->XGetSubWindowData(nIndex, &p);
	// 	PLAY_WINDOW_DATA *pData = (PLAY_WINDOW_DATA *)p;
	// 	CROSS_STRCPY(pData->dev_uuid, st.SvrDevInfo.dev_uuid);
	// 	pData->hitem = NULL;
	// 	pData->winIndex = nIndex;

	do
	{
		char szhhhh[32] = { 0 };
		sprintf_s(szhhhh, "%d", m_stDevice.classicinfo.dev_id);
		m_pStreamObject = theApp.m_pClientInferface->CreateXClientStreamInterface(szhhhh,0);// pDevFun();

		//m_pStreamObject->XClientStreamSetInfo(theApp.m_strip.c_str(), 6503, theApp.m_strName.c_str(),
		//	theApp.m_strPwd.c_str(), m_stDevice.classicinfo.dev_id, 0, nullptr);


		m_pStreamObject->XClientStreamPlay(m_view.GetSafeHwnd());


		CZPanLive *pWnd = (CZPanLive *)m_pAfxMainWnd;
		pWnd->PostMessage(WM_VIDEO_OPEN_CLOSE, (WPARAM)m_stDevice.hItem, (LPARAM)1);


		ShowLoadingGif(TRUE);
		m_view.SetWindowText(L"正在连接！");
		//m_closebtn.ShowWindow(SW_SHOW);

#if 1
		m_bThreadExit = FALSE;
		m_hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)check, this, NULL, NULL);
#else
		m_bThreadExit = FALSE;
		m_hThread = NULL;
#endif
		//
		//DeviceInfo_T t;
		//memset(&t, 0, sizeof(t));
		//


		// 		strcpy_s(t.core_svr_ip, theApp.m_strip.c_str());
		// 		t.core_svr_port = 6500 + rand() % 20;
		// 		t.his_svr_port = 6800;
		// 		strcpy_s(t.user_name, theApp.m_strName.c_str());
		// 		strcpy_s(t.user_pwd, theApp.m_strPwd.c_str());
		//

		// 		if (X_CLIENT_XXX_TYPE_CLASSIC == theApp.m_ClientType)
		// 		{
		// 			pData->pDevObject->XClientStreamSetInfo(theApp.m_strip.c_str(), 6499, theApp.m_strName.c_str(),
		// 				theApp.m_strPwd.c_str(),);
		// 		}
		// 		if (X_CLIENT_XXX_TYPE_IOT == theApp.m_ClientType)
		// 		{
		// 		}




		//pData->pDevObject->XDeviceLogin();
		//pData->pDevObject->XPreviewOpenForWindows(m_pVideoDlg->XGetSubWindowHwnd(nIndex));
		///
		//memset(pData->dev_name, 0, sizeof(pData->dev_name));
		// 		//memcpy(pData->dev_name, st.SvrDevInfo.dev_name, strlen(st.SvrDevInfo.dev_name));
		// 		m_pVideoDlg->XSetSubWindowTitleOfIndex(nIndex, pData->dev_name);
		// 		m_pVideoDlg->XEnableSubWindowButtnOfIndex(nIndex, TRUE);
		// 		pData->bPlay = TRUE;
		// 		pData->hitem = hitem;//-<<<<<<<<<<<<<<<
		// 		//m_csOpenRealtime.Unlock();
		// 		return 0;


	} while (0);

	//
	//m_csOpenRealtime.Unlock();
	return 0;
}

int CVideoViewDlg::XViewCloseStream()
{
	if (m_hThread)
	{
		m_bThreadExit = TRUE;
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	if (m_pStreamObject)
	{
		m_pStreamObject->XClientStreamClose();
		m_pStreamObject->XDelete();
		m_pStreamObject = NULL;
		//
		CZPanLive *pWnd = (CZPanLive *)m_pAfxMainWnd;
		pWnd->PostMessage(WM_VIDEO_OPEN_CLOSE, (WPARAM)m_stDevice.hItem, (LPARAM)0);
	}


		ShowLoadingGif(FALSE);
		m_view.SetWindowText(L"");
		//m_closebtn.ShowWindow(SW_HIDE);
		//
		m_bCanShowCtrlPan = FALSE;
		m_pCtrlPanDlg->ShowWindow(m_bCanShowCtrlPan ? SW_SHOW : SW_HIDE);



	// 	if (NULL != pData->hitem)
	// 	{
	// 		CDeviceManager::Instance().SetDeviceOpenStatusByDeviceID(pData->hitem, FALSE);
	// 	}




	//EnableButtnState(FALSE);

	// 	m_pVideoDlg->XSetSubWindowTitleOfIndex(nIndex, "");
	// 	m_pVideoDlg->XEnableSubWindowButtnOfIndex(nIndex, FALSE);
	// 	m_tree.SetItemImage(pData->hitem, 2, 2);
	// 	pData->hitem = NULL;
	// 
	// 	//m_csOpenRealtime.Unlock();




	return 0;

}

//int a = 0;
int CVideoViewDlg::XViewRecord(int nStart)
{
	if (m_pStreamObject)
	{
		char szName[MAX_PATH] = { 0 };

		if (theApp.m_ClientType == X_CLIENT_XXX_TYPE_CLASSIC)
		{
			sprintf_s(szName, "%d", m_stDevice.classicinfo.dev_id);
		}
		else
		{
			sprintf_s(szName, "%s", m_stDevice.iotinfo.dev_uuid);
		}



		m_pStreamObject->XClientStreamRecord(
			theApp.m_clientSetting.recPath,
			szName,
			nStart
			);
	}

	return 0;
}

int CVideoViewDlg::XViewSound(int nStart)
{
	// 	void *p = NULL;
	// 	m_pVideoDlg->XGetSubWindowData(nIndex, &p);
	// 	PLAY_WINDOW_DATA *pData = (PLAY_WINDOW_DATA *)p;
	// 
	// 
	if (m_pStreamObject)
	{
		m_pStreamObject->XClientStreamSound(nStart);
	}


	return 0;
}

int CVideoViewDlg::XViewTalk(int nStart)
{
	// 	void *p = NULL;
	// 	m_pVideoDlg->XGetSubWindowData(nIndex, &p);
	// 	PLAY_WINDOW_DATA *pData = (PLAY_WINDOW_DATA *)p;

	return 0;
}



int CVideoViewDlg::XViewCapturePictures()
{
	// 	void *p = NULL;
	// 	m_pVideoDlg->XGetSubWindowData(nIndex, &p);
	// 	PLAY_WINDOW_DATA *pData = (PLAY_WINDOW_DATA *)p;

	if (m_pStreamObject)
	{
		char szPath[MAX_PATH] = { 0 };
		char szName[MAX_PATH] = { 0 };

		SYSTEMTIME   sysTime;
		GetLocalTime(&sysTime);


		if (theApp.m_ClientType == X_CLIENT_XXX_TYPE_CLASSIC)
		{
			//根目录+Pictures目录+日期目录+设备名称目录+文件名.jpg
			sprintf_s(szPath, "%s\\Pictures\\%04d%02d%02d\\%d",
				theApp.m_clientSetting.picPath,
				sysTime.wYear, sysTime.wMonth, sysTime.wDay,
				m_stDevice.classicinfo.dev_id
				);
		}
		else
		{
			//根目录+Pictures目录+日期目录+设备名称目录+文件名.jpg
			sprintf_s(szPath, "%s\\Pictures\\%04d%02d%02d\\%s",
				theApp.m_clientSetting.picPath,
				sysTime.wYear, sysTime.wMonth, sysTime.wDay,
				m_stDevice.iotinfo.dev_uuid
				);
		}



		sprintf_s(szName, "%02d%02d%02d_%d.jpg", sysTime.wHour, sysTime.wMinute, sysTime.wSecond, GetTickCount());

		m_pStreamObject->XClientStreamCapturePictures(szPath, szName);
	}

	return 0;
}

// void CVideoViewDlg::OnBnClickedBtnClose()
// {
// 	// TODO: Add your control notification handler code here
// 	ShowLoadingGif(FALSE);
// 	m_view.SetWindowText(L"");
// 	m_closebtn.ShowWindow(SW_HIDE);
// 	//
// 	m_bCanShowCtrlPan = FALSE;
// 	m_pCtrlPanDlg->ShowWindow(m_bCanShowCtrlPan ? SW_SHOW : SW_HIDE);
// 	//
// 	XViewCloseStream();
// }
