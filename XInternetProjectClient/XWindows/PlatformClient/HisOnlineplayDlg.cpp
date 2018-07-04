// HisOnlineplayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "HisOnlineplayDlg.h"
#include "afxdialogex.h"

#include "waveOut.hpp"
#include "XPlayer.hpp"

#include "PlatformClientDlg.h"


// CHisOnlineplayDlg dialog


IMPLEMENT_DYNAMIC(CHisOnlineplayDlg, CDialogEx)

CHisOnlineplayDlg::CHisOnlineplayDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHisOnlineplayDlg::IDD, pParent)
{
	m_hThreadMp4 = NULL;
	m_hWorkerThreadRealtime = CROSS_THREAD_NULL;
	m_pDownloadInterface = NULL;
}

CHisOnlineplayDlg::~CHisOnlineplayDlg()
{
// 	if (m_bitmapBG)
// 	{
// 		CBitmap bmp;
// 		bmp.Attach(m_bitmapBG);
// 		bmp.Detach();
// 		DeleteObject(m_bitmapBG);
// 	}
}

void CHisOnlineplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// 	DDX_Control(pDX, IDC_BUTTON3, m_btnmin);
	// 	DDX_Control(pDX, IDC_BUTTON4, m_btnmax);
	DDX_Control(pDX, IDC_BUTTON5, m_btnclose);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_view);
	DDX_Control(pDX, IDC_BUTTON2, m_btnpause);
	DDX_Control(pDX, IDC_BUTTON3, m_btnSave);
	DDX_Control(pDX, IDC_SLIDER1, m_silder);
	DDX_Control(pDX, IDC_STATIC_5, m_timeshow);
	DDX_Control(pDX, IDC_BUTTON6, m_max);
	DDX_Control(pDX, IDC_BUTTON4, m_close);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_ctrlDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_ctrlTime);
	DDX_Control(pDX, IDC_LIST1, m_listbox);
	DDX_Control(pDX, IDC_BUTTON1, m_btnSearch);
}


BEGIN_MESSAGE_MAP(CHisOnlineplayDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()
	ON_WM_SIZE()
// 	ON_BN_CLICKED(IDC_BUTTON3, &CHisOnlineplayDlg::OnBnClickedButton3)
// 	ON_BN_CLICKED(IDC_BUTTON4, &CHisOnlineplayDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CHisOnlineplayDlg::OnBnClickedButton5)
	ON_WM_CTLCOLOR()
//	ON_WM_CLOSE()
//	ON_WM_DESTROY()
	ON_WM_TIMER()
//	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON2, &CHisOnlineplayDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CHisOnlineplayDlg::OnBnClickedButton3)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON6, &CHisOnlineplayDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON4, &CHisOnlineplayDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CHisOnlineplayDlg::OnBnClickedButton1)
	ON_LBN_DBLCLK(IDC_LIST1, &CHisOnlineplayDlg::OnLbnDblclkList1)
END_MESSAGE_MAP()


// CHisOnlineplayDlg message handlers

BOOL CHisOnlineplayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_bPlay = FALSE;
	m_bPause = FALSE;

	ButtonLoadBitmap(m_btnclose,
		L"hisviewpan/stop_1.png",
		L"hisviewpan/stop_2.png",
		L"hisviewpan/stop_3.png",
		L"hisviewpan/stop_4.png");
	ButtonLoadBitmap(m_btnpause,
		L"hisviewpan/pause_1.png",
		L"hisviewpan/pause_2.png",
		L"hisviewpan/pause_3.png",
		L"hisviewpan/pause_4.png");
	ButtonLoadBitmap(m_btnSave,
		L"hisviewpan/openfile.png",
		L"hisviewpan/openfile02.png",
		L"hisviewpan/openfile03.png",
		L"hisviewpan/openfile04.png");



	ButtonLoadBitmap(m_max, L"title_bar/max_1.png", L"title_bar/max_2.png", L"title_bar/max_3.png", L"title_bar/max_4.png");
	ButtonLoadBitmap(m_close, L"title_bar/close_1.png", L"title_bar/close_2.png", L"title_bar/close_3.png", L"title_bar/close_4.png");


	m_view.Invalidate();
	m_timeshow.SetWindowText(L"");
	m_silder.SetPos(0);
	m_pPause = FALSE;


	m_silder.EnableWindow(FALSE);
	m_btnclose.EnableWindow(FALSE);
	m_btnpause.EnableWindow(FALSE);
	m_btnSave.EnableWindow(FALSE);

	//

	SYSTEMTIME   sysTime;
	GetLocalTime(&sysTime);
	//CTime time = CTime((int)sysTime.wYear, (int)sysTime.wMonth, (int)sysTime.wDay, (int)sysTime.wHour, (int)sysTime.wMinute, -1);
	CTime time = CTime((int)sysTime.wYear, (int)sysTime.wMonth, (int)sysTime.wDay, (int)sysTime.wHour, (int)0, (int)0);

	CTimeSpan span(
		0, //_In_ LONG lDays,
		3, //_In_ int nHours,
		0, //_In_ int nMins,
		0//_In_ int nSecs
		);  //时间差值
	CTime newtime = time - span; //前的时间


	m_ctrlDate.SetTime(&newtime);
	m_ctrlTime.SetTime(&newtime);



	//-------------------------------------------------------------------------------------
	SendMessage(WM_SIZE, 0, 0);
	CenterWindow();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHisOnlineplayDlg::OnPaint()
{

		CPaintDC dc(this); // device context for painting
		// TODO: Add your message handler code here
		// Do not call CDialogEx::OnPaint() for painting messages
		OnPaintBmpBackground(this, dc,32);
		DrawFrameRect();
}
int CHisOnlineplayDlg::DrawFrameRect(void)
{


	CRect rt;
	GetClientRect(&rt);

	rt.left += 281;
	rt.top += 32;


	//rt.DeflateRect(1, 1, 1, 1);
	rt.bottom -= 40;

	HBRUSH hbrush, hbrushOld;
	hbrush = CreateSolidBrush(RGB(0, 85, 120));

	HDC hdc = ::GetDC(m_hWnd);
	hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);

	FrameRect(hdc, &rt, hbrush);

	SelectObject(hdc, hbrushOld);
	::DeleteObject(hbrush);
	::DeleteObject(hbrushOld);
	::ReleaseDC(m_hWnd, hdc);

	return 0;
}


LRESULT CHisOnlineplayDlg::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(rect);
	rect.bottom = rect.top + 32;

	if (rect.PtInRect(point))
		return HTCAPTION;
	return CDialogEx::OnNcHitTest(point);
}


BOOL CHisOnlineplayDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}

		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CHisOnlineplayDlg::OnBnClickedButton5()
{

	OnlineHisClose();
}

void CHisOnlineplayDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if (m_view.GetSafeHwnd() == NULL)
	{
		return;
	}
	CRect rectClient;
	GetClientRect(rectClient);

	int nspace = 0;

	//------------------------------------------------------------------------------------------------
	int mLeft = 0, nSpace = 0, mLeftWidth = 280;
	//------------------------------------------------------------------------------------------------
	nspace = 0;


	CRect rc;
	GetClientRect(rc);

	m_max.SetWindowPos(NULL, rectClient.left + rectClient.Width() - nspace * 2 - theApp.m_nCloseWidth * 2, 0, theApp.m_nCloseWidth, theApp.m_nCloseHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	m_close.SetWindowPos(NULL, rectClient.left + rectClient.Width() - nspace - theApp.m_nCloseWidth, 0, theApp.m_nCloseWidth, theApp.m_nCloseHeight, SWP_NOACTIVATE | SWP_NOZORDER);


	//
	mLeft = 2;

	CRect rcWndListbox = rc;
	rcWndListbox.left = mLeft;
	rcWndListbox.top = 180;
	rcWndListbox.right = mLeftWidth;
	rcWndListbox.bottom -= 2;
	m_listbox.MoveWindow(rcWndListbox);




	mLeft = mLeft + mLeftWidth;
	//////////////////////////////////////////////////////////////////////////
	CRect rcWnd = rc;
	rcWnd.left += mLeft;
	rcWnd.top += 33;
	rcWnd.right -= 1;
	rcWnd.bottom -= 41;
	m_view.MoveWindow(rcWnd);


	//////////////////////////////////////////////////////////////////////////
	int s = 5;
	int l = 24;
	int k = (40 - l) / 2;
	int ltitle = 180;
	//
	CRect rcSave = rc;
	rcSave.left = l * 0 + s * 1 + mLeft;
	rcSave.top = rcWnd.bottom + k;
	rcSave.bottom = rcSave.top + l;
	rcSave.right = rcSave.left + l;
	m_btnSave.MoveWindow(rcSave);
	//
	CRect rcPause = rc;
	rcPause.left = l * 1 + s * 2 + mLeft;
	rcPause.top = rcWnd.bottom + k;
	rcPause.bottom = rcPause.top + l;
	rcPause.right = rcPause.left + l;
	m_btnpause.MoveWindow(rcPause);
	//
	CRect rcClose = rc;
	rcClose.left = l * 2 + s * 3 + mLeft;
	rcClose.top = rcWnd.bottom + k;
	rcClose.bottom = rcClose.top + l;
	rcClose.right = rcClose.left + l;
	m_btnclose.MoveWindow(rcClose);

	//
	ltitle = 3;
	CRect rcSilder = rc;
	rcSilder.left = l * 3 + s * 4 + mLeft;
	rcSilder.top = rcWnd.bottom + k;
	rcSilder.bottom = rcSilder.top + l;
	rcSilder.right -= ltitle;
	m_silder.MoveWindow(rcSilder);

	//////////////////////////////////////////////////////////////////////////

	CRect rcTitile = rc;
	rcTitile.left = rcTitile.right - ltitle;
	rcTitile.top = rcWnd.bottom + k + 7;
	rcTitile.bottom = rcTitile.top + l;
	m_timeshow.MoveWindow(rcTitile);


	//
	Invalidate(TRUE);
}

HBRUSH CHisOnlineplayDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if ((pWnd->GetDlgCtrlID() == IDC_STATIC_VIEW))
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)theApp.hbrushDlg;
	}

	//return (HBRUSH)theApp.hbrushDlg;
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

//---------------------------------------------------------------------------------------

int CHisOnlineplayDlg::WorkerThreadRealtime(void* param)
{
	CHisOnlineplayDlg *pService = (CHisOnlineplayDlg*)param;
	pService->WorkerRealtime();
	return 0;
}
DWORD64 ssaa = 1;


void CHisOnlineplayDlg::WorkerRealtime()
{

	DWORD64 dw = GetTickCount64();


	while (!m_bWorkerRealtimeStop)
	{

		if (m_pPause)
		{
			Sleep(10);
			continue;
		}

		int nPlayRet = m_pDownloadInterface->XClientDownloadInterfacePlayFrameWindows(m_view.GetSafeHwnd());

		if (-1 == nPlayRet)
		{
			//-1表示失败
			Sleep(10);
			continue;
		}

		if (0 == nPlayRet)
		{
			//0表示无数据 
			Sleep(10);
			continue;
		}


		//>0表示与下一帧的时间间隔



		int needSleep = nPlayRet;
		if (needSleep <= 0)
		{
			needSleep = 1;
		}
		CROSS_DWORD64 mmm = CrossGetTickCount64();
		if (needSleep > 14)
		{
			do
			{
				if (m_bWorkerRealtimeStop)
				{
					break;
				}

				CrossSleep(1);//根据帧率休眠一段时间才播放下一帧
			} while (((CrossGetTickCount64() - mmm)) < needSleep);
		}

		//TRACE("##### %d\n", GetTickCount64() - dw);
		dw = GetTickCount64();//
	}



	//delete[] pszTmp;
}

void CHisOnlineplayDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

//	TRACE("####################### %d   %d   %d\n", m_DownloadCallback.GetPlayFileSecond(), m_DownloadCallback.GetDownloadFileSecond(), m_DownloadCallback.GetFileSecond());



	if (nIDEvent == TIMER_ONLINE_PLAY_BFJD)
	{
		m_silder.SetPos(m_pDownloadInterface->XClientDownloadInterfacePlayGetPlaySecond());
	}

	if (nIDEvent == TIMER_ONLINE_PLAY_XZJD)
	{
		CROSS_DWORD64 t1 = m_pDownloadInterface->XClientDownloadInterfacePlayGetFileSecond();
		CROSS_DWORD64 t2 = m_pDownloadInterface->XClientDownloadInterfacePlayGetDownloadSecond();
		m_silder.SetDownLoadProgress((int)(t2 * 100 / t1));
		m_silder.Invalidate();
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CHisOnlineplayDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	//save mp4;

	CString str1, str2;
	Get_String_From_Language_Ini_File(L"str_dlg_160802_msg5", str1);
	Get_String_From_Language_Ini_File(L"str_dlg_160802_msg6", str2);
	if (MessageBox(str2, str1, MB_OKCANCEL) == IDOK)
	{

		//theApp.m_clientSetting.recPath,

		if (m_hThreadMp4 != NULL)
		{
			WaitForSingleObject(m_hThreadMp4,INFINITE);
			CloseHandle(m_hThreadMp4);
			m_hThreadMp4 = NULL;
		}

		m_hThreadMp4 = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WriteMp4Thread, this, 0, 0);

		m_waitDlg.m_pMain = this;
		m_waitDlg.DoModal();
	}
	


}
int CHisOnlineplayDlg::WriteMp4Thread(void * param)
{
// 	CHisOnlineplayDlg *p = (CHisOnlineplayDlg *)param;
// 	char filepath[1024] = { 0 };
// 	sprintf_s(filepath, "%s\\Mp4Files\\%d\\Download",
// 		theApp.m_clientSetting.recPath, p->m_stDeviceInfo.SvrDevInfo.dev_uuid);
// 
// 	p->m_pDownloadInterface->XDownloadHisFileWiteMp4File(filepath);

	return 0;
}
int CHisOnlineplayDlg::GetWiteMp4FileProgress()
{
 	return m_pDownloadInterface->XClientDownloadInterfacePlayWiteMp4FileProgress();

	return 0;
}

void CHisOnlineplayDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
//	delete this;
	CDialogEx::PostNcDestroy();
}
void CHisOnlineplayDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	m_pPause = !m_pPause;
	if (m_pPause)
	{
		ButtonLoadBitmap(m_btnpause,
			L"hisviewpan/pause_3.png",
			L"hisviewpan/pause_2.png",
			L"hisviewpan/pause_1.png",
			L"hisviewpan/pause_4.png");
	}
	else
	{
		ButtonLoadBitmap(m_btnpause,
			L"hisviewpan/pause_1.png",
			L"hisviewpan/pause_2.png",
			L"hisviewpan/pause_3.png",
			L"hisviewpan/pause_4.png");
	}


}


BOOL CHisOnlineplayDlg::OnlineHisClose()
{
	KillTimer(TIMER_ONLINE_PLAY_BFJD);
	KillTimer(TIMER_ONLINE_PLAY_XZJD);
	if (m_hThreadMp4 != NULL)
	{
		WaitForSingleObject(m_hThreadMp4, INFINITE);
		CloseHandle(m_hThreadMp4);
		m_hThreadMp4 = NULL;
	}

	if (m_hWorkerThreadRealtime)
	{
		m_bWorkerRealtimeStop = TRUE;
		CrossWaitThread(m_hWorkerThreadRealtime);
		CrossCloseThread(m_hWorkerThreadRealtime);
		m_hWorkerThreadRealtime = CROSS_THREAD_NULL;
	}


	if (m_pDownloadInterface)
	{
		m_pDownloadInterface->XClientDownloadInterfaceDelete();
		m_pDownloadInterface = NULL;
	}


// 	if (m_pDevInteface)
// 	{
// 		//m_pDevInteface->XDeviceClose();
// 		m_pDevInteface->XDeviceDelete();
// 		m_pDevInteface = NULL;
// 	}

	if (m_pPause)
	{
		ButtonLoadBitmap(m_btnpause,
			L"hisviewpan/pause_1.png",
			L"hisviewpan/pause_2.png",
			L"hisviewpan/pause_3.png",
			L"hisviewpan/pause_4.png");


		m_pPause = FALSE;
	}


	m_silder.EnableWindow(FALSE);
	m_btnclose.EnableWindow(FALSE);
	m_btnpause.EnableWindow(FALSE);
	m_btnSave.EnableWindow(FALSE);
	m_silder.SetPos(0);
	m_silder.SetDownLoadProgress(0);
	m_silder.Invalidate();


	return TRUE;
}
BOOL CHisOnlineplayDlg::OnlineHisPlay()
{
	OnlineHisClose();


	char szhhhh[32] = { 0 };
	sprintf_s(szhhhh, "%d", 3);
	m_pDownloadInterface = theApp.m_pClientInferface->CreateXClientDownloadInterface(szhhhh, 0);
	//
	//pDownload->XClientDownloadInterfaceSetInfo("47.96.249.142", 555, theApp.m_strName.c_str(), theApp.m_strPwd.c_str(), 2, 0);
	//
	//m_pDownloadInterface->XClientDownloadInterfaceGetHisLogs(tm1, tm2, r);
	//
	//m_pDownloadInterface->XClientDownloadInterfaceDelete();



// 	m_pDevInteface = CreateMyMediaDeviceInterface();
// 	DeviceInfo_T t;
// 	memset(&t, 0, sizeof(DeviceInfo_T));
// 	strcpy_s(t.core_svr_ip, theApp.m_strip.c_str());
// 	t.core_svr_port = 6500 + rand() % 20;
// 	t.his_svr_port = 6800;
// 	strcpy_s(t.user_name, theApp.m_strName.c_str());
// 	strcpy_s(t.user_pwd, theApp.m_strPwd.c_str());
// 	m_pDevInteface->XDeviceSetInfo(t);
// 	m_pDownloadInterface = m_pDevInteface->CreateDownloadHisFileInstance();
// 
// 	struct tm gm = { m_stHisLogInfo.store_begin_time_tm.wSecond, m_stHisLogInfo.store_begin_time_tm.wMinute, m_stHisLogInfo.store_begin_time_tm.wHour,
// 		m_stHisLogInfo.store_begin_time_tm.wDay, m_stHisLogInfo.store_begin_time_tm.wMonth - 1, m_stHisLogInfo.store_begin_time_tm.wYear - 1900, m_stHisLogInfo.store_begin_time_tm.wDayOfWeek, 0, 0 };
// 	time_t kkkk = mktime(&gm) - 3;
// 	struct tm p;
// 	gmtime_s(&p, &kkkk);
// 
// 	m_stHisLogInfo.store_begin_time_tm.wYear = p.tm_year + 1900;
// 	m_stHisLogInfo.store_begin_time_tm.wMonth = p.tm_mon + 1;
// 	m_stHisLogInfo.store_begin_time_tm.wDay = p.tm_mday;
// 	m_stHisLogInfo.store_begin_time_tm.wHour = p.tm_hour + 8;
// 	m_stHisLogInfo.store_begin_time_tm.wMinute = p.tm_min;
// 	m_stHisLogInfo.store_begin_time_tm.wSecond = p.tm_sec;



	if (0 != m_pDownloadInterface->XClientDownloadInterfaceDownloadHislog(m_stHisLogInfo.begin_time_tm, m_stHisLogInfo.end_time_tm))//, t, m_view.GetSafeHwnd()))
	{
		m_pDownloadInterface->XClientDownloadInterfaceDelete();
		m_pDownloadInterface = NULL;
		return FALSE;
	}

// 	if (0 != m_DownloadCallback.Open(m_pDevInteface,, ))
// 	{
// 		//AfxMessageBox(L"open error");
// 		//SendMessage(WM_CLOSE);
// 
// 		m_pDevInteface->XDeviceDelete();
// 		m_pDevInteface = NULL;
// 		return FALSE;
// 	}

	//m_pDownloadInterface->XClientDownloadInterfacePlayFrameWindows(m_view.GetSafeHwnd());

	m_silder.SetRange(0, m_pDownloadInterface->XClientDownloadInterfacePlayGetFileSecond());
	m_silder.SetPageSize(10);
	m_silder.SetPos(0);
	m_silder.EnableWindow(TRUE);

	if (m_hWorkerThreadRealtime == CROSS_THREAD_NULL)
	{
		m_bWorkerRealtimeStop = FALSE;
		m_hWorkerThreadRealtime = CrossCreateThread(WorkerThreadRealtime, this);
	}



	// 		m_DownloadCallback.
	// 
	// 
	// 		m_pDevInteface->XDeviceSetInfo(
	// 			NULL,
	// 			0,
	// 			theApp.m_szUser,
	// 			theApp.m_szPwd,
	// 			theApp.m_szPlaybackIP,
	// 			theApp.m_nPlaybackPort,
	// 			0,
	// 			X_DEVICE_TYPE_NONE);
	// 
	// 
	// 		m_pDownInterface = m_pDevInteface->CreateDownloadHisFileInstance();
	// 
	// 		m_pDownInterface->XDownloadHislogFromMedia(theApp.m_nDevID, theApp.m_nDevChannel, -1,
	// 			theApp.m_tmBeginTime, theApp.m_tmEndTime, this, this);

	

	SetTimer(TIMER_ONLINE_PLAY_XZJD, 500, NULL);
	SetTimer(TIMER_ONLINE_PLAY_BFJD, 300, NULL);
	m_silder.EnableWindow(TRUE);
	m_btnclose.EnableWindow(TRUE);
	m_btnpause.EnableWindow(TRUE);
	m_btnSave.EnableWindow(TRUE);

	return TRUE;
}

void CHisOnlineplayDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == GetDlgItem(IDC_SLIDER1))//这里判断是不是IDC_SCROLLBAR1在滚动 
	{
		//		TRACE(">>>>>>>>>  nSBCode=%d\n", nSBCode);
		switch (nSBCode)
		{
		case SB_PAGELEFT://影响鼠标点击一次滑块或用户按下PageUp和PageDown键，滑块移动的距离，由SetPageSize决定  
		case SB_PAGERIGHT:
		case SB_LEFT://用户按下Home键触发  
		case SB_RIGHT://用户按下End键触发  
		case SB_LINELEFT://用户按下↑↓←→键时触发  
		case SB_LINERIGHT://用户按下↑↓←→键时触发  
		case SB_THUMBTRACK://鼠标拖拽滑块时触发，此时nPos有效  
		{
			if (m_pPause)
			{
				return;
			}
			KillTimer(TIMER_ONLINE_PLAY_BFJD);
		}
		break;
		case SB_THUMBPOSITION://鼠标拖拽滑块并释放时触发，此时nPos有效  
			break;
		case SB_ENDSCROLL:
		{
			if (m_pPause)
			{
				return;
			}


			int a = m_silder.GetPos();
			do 
			{
				if (a >= m_pDownloadInterface->XClientDownloadInterfacePlayGetDownloadSecond())
				{
					break;
				}

				m_pDownloadInterface->XClientDownloadInterfacePlaySeekSecond(m_silder.GetPos());

				

			} while (0);



			SetTimer(TIMER_ONLINE_PLAY_BFJD, 500, 0);


			// 			CString str;
			// 			str.Format(_T("CParamImageDlg::OnHScroll111------> %d\n"), m_sd1.GetPos());
			// 			TRACE(str);
			// 			m_param.nBrightness = m_sd1.GetPos();
			// 			m_pDevObject->SetChannelParam(0, (char *)&m_param, X_PARAM_TYPE_IMAGE);
		}
		//x_player_setLocalPlayPosition(m_fileid, m_sd1.GetPos());
		//m_bDrag = FALSE;
		default:
			break;
		}
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CHisOnlineplayDlg::OnBnClickedButton6()
{
	//max

	if (this->IsZoomed())
		this->SendMessage(WM_SYSCOMMAND, SC_RESTORE);
	else
		this->SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}


void CHisOnlineplayDlg::OnBnClickedButton4()
{
	//close
	ClearList();
	DestroyWindow();
}


void CHisOnlineplayDlg::OnBnClickedButton1()
{
	ClearList();
	CPlatClientDlg * pMainWnd = (CPlatClientDlg *)AfxGetMainWnd();

	//--------------------------------------------------------------------------
	ST_XMEDIA_TIME tm1, tm2;
	memset(&tm1, 0, sizeof(ST_XMEDIA_TIME));
	memset(&tm2, 0, sizeof(ST_XMEDIA_TIME));
	COleDateTime  currOleDate1;
	m_ctrlDate.GetTime(currOleDate1);
	COleDateTime  currOleDate2;
	m_ctrlTime.GetTime(currOleDate2);
	//
	SYSTEMTIME sysTime1;
	currOleDate1.GetAsSystemTime(sysTime1);
	//
	SYSTEMTIME sysTime2;
	currOleDate2.GetAsSystemTime(sysTime2);
	//
	sysTime1.wHour = sysTime2.wHour;
	sysTime1.wMinute = sysTime2.wMinute;
	sysTime1.wSecond = sysTime2.wSecond;
	//
	CTime newtime(sysTime1);// = CTime::GetCurrentTime(); //获取当前时间
	CTimeSpan span(
		0, //_In_ LONG lDays,
		4, //_In_ int nHours,
		0, //_In_ int nMins,
		0//_In_ int nSecs
		);  //时间差值
	CTime time = newtime + span; //前的时间
	//
	tm1.wYear = newtime.GetYear();
	tm1.wMonth = newtime.GetMonth();
	tm1.wDay = newtime.GetDay();
	tm1.wHour = newtime.GetHour();
	tm1.wMinute = newtime.GetMinute();
	tm1.wSecond = newtime.GetSecond();
	//
	tm2.wYear = time.GetYear();
	tm2.wMonth = time.GetMonth();
	tm2.wDay = time.GetDay();
	tm2.wHour = time.GetHour();
	tm2.wMinute = time.GetMinute();
	tm2.wSecond = time.GetSecond();
	//--------------------------------------------------------------------------
	vector<ST_SREVER_HIS_LOG> r;
	//
	char szhhhh[32] = { 0 };
	sprintf_s(szhhhh, "%d", 3);
	CXClientDownloadInterface * pDownload = theApp.m_pClientInferface->CreateXClientDownloadInterface(szhhhh,0);
	//
	//pDownload->XClientDownloadInterfaceSetInfo("47.96.249.142", 555, theApp.m_strName.c_str(), theApp.m_strPwd.c_str(), 2, 0);
	//
	pDownload->XClientDownloadInterfaceGetHisLogs(tm1, tm2, r);
	//
	pDownload->XClientDownloadInterfaceDelete();
	//
	for (int i = 0; i < (int)r.size(); i++)
	{
		//CString strname = (*_itor).strPath;
		//wchar_t ss1 = '\\';
		//strname = strname.Right(strname.GetLength() - strname.ReverseFind(ss1) - 1);
		//
		//			int nCount = m_list.GetCount();


		//序号
		// 			CString strIndex;
		// 			strIndex.Format(_T("%03d"), nCount + 1);
		// 			m_list.InsertItem(nCount, strIndex);
		//文件名
		//			WCHAR wszTempPath[256];
		// 			MultiByteToWideChar(CP_UTF8, 0, r[i].store_file, strlen(r[i].store_file) + 1, wszTempPath, sizeof(wszTempPath) / sizeof(wszTempPath[0]));
		// 			CString strname = wszTempPath;
		// 			wchar_t * S;// = { 0 };
		// 			S = L"\\";
		// 			int a = strname.ReverseFind(S[0]);
		// 
		// 
		// 			m_list.SetItemText(nCount, 1, strname.Right(strname.GetLength() - a - 1));


		//start time
		CString strStart;
		strStart.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"),
			r[i].begin_time_tm.wYear, r[i].begin_time_tm.wMonth, r[i].begin_time_tm.wDay,
			r[i].begin_time_tm.wHour, r[i].begin_time_tm.wMinute, r[i].begin_time_tm.wSecond);


		//m_list.SetItemText(nCount, 2, strStart);


		//endtime
		// 					CString strEnd;
		// 					strEnd.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"),
		// 						r[i].store_end_time_tm.tm_year + 1900, r[i].store_end_time_tm.tm_mon + 1, r[i].store_end_time_tm.tm_mday,
		// 						r[i].store_end_time_tm.tm_hour, r[i].store_end_time_tm.tm_min, r[i].store_end_time_tm.tm_sec);
		// 					m_list.SetItemText(nCount, 3, strEnd);




		//文件大小
		std::tm tm1, tm2;

		tm1.tm_year = r[i].begin_time_tm.wYear - 1900;
		tm1.tm_mon = r[i].begin_time_tm.wMonth - 1;
		tm1.tm_mday = r[i].begin_time_tm.wDay;
		tm1.tm_hour = r[i].begin_time_tm.wHour;
		tm1.tm_min = r[i].begin_time_tm.wMinute;
		tm1.tm_sec = r[i].begin_time_tm.wSecond;

		tm2.tm_year = r[i].end_time_tm.wYear - 1900;
		tm2.tm_mon = r[i].end_time_tm.wMonth - 1;
		tm2.tm_mday = r[i].end_time_tm.wDay;
		tm2.tm_hour = r[i].end_time_tm.wHour;
		tm2.tm_min = r[i].end_time_tm.wMinute;
		tm2.tm_sec = r[i].end_time_tm.wSecond;

		time_t andtime1 = mktime(&tm1);
		time_t andtime2 = mktime(&tm2);
		CString strSize;
		strSize.Format(_T("    [ %d(S)]"), andtime2 - andtime1);




		CString strInfo = strStart + strSize;


		int nCount = m_listbox.GetCount();
		m_listbox.InsertString(nCount, strInfo);

		//
		ST_SREVER_HIS_LOG* pInfo = new ST_SREVER_HIS_LOG;
		ZeroMemory(pInfo, 0);
		memcpy(pInfo, &r[i], sizeof(ST_SREVER_HIS_LOG));
		m_listbox.SetItemData(nCount, (DWORD_PTR)pInfo);



	}


}
void CHisOnlineplayDlg::ClearList()
{
	int nCount = m_listbox.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		ST_SREVER_HIS_LOG* pInfo = (ST_SREVER_HIS_LOG*)m_listbox.GetItemData(i);
		delete pInfo;
	}
	m_listbox.ResetContent();
	m_listbox.Invalidate();
}

void CHisOnlineplayDlg::OnLbnDblclkList1()
{
	// TODO: Add your control notification handler code here
	int i = m_listbox.GetCurSel();
	if (i == -1)
	{
		return;
	}

	ST_SREVER_HIS_LOG* pInfo = (ST_SREVER_HIS_LOG*)m_listbox.GetItemData(i);
	memcpy(&m_stHisLogInfo, pInfo, sizeof(ST_SREVER_HIS_LOG));
	OnlineHisPlay();
}
