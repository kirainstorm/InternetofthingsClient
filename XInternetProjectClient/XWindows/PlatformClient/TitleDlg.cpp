// TitleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "TitleDlg.h"
#include "afxdialogex.h"
#include "PlatformClientDlg.h"
#include "LoopPlaySettingDlg.h"
#include "PlatformClientDlg.h"
#include "LogDialog.h"

// CTitleDlg 对话框
IMPLEMENT_DYNAMIC(CTitleDlg, CDialogEx)
CTitleDlg::CTitleDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CTitleDlg::IDD, pParent)
{
	m_pParent = pParent;
}
CTitleDlg::~CTitleDlg()
{
}
void CTitleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_MIN, m_min);
	DDX_Control(pDX, IDC_BUTTON_MAX, m_max);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_close);
	DDX_Control(pDX, IDC_DROP_DOWN, m_dropdown);
	DDX_Control(pDX, IDC_STATIC_99, m_title);
	DDX_Control(pDX, IDC_BTN_TITLE_LIVE, m_titile_live);
	// 	DDX_Control(pDX, IDC_BTN_TITLE_REC, m_titile_lunbo);
	// 	DDX_Control(pDX, IDC_BTN_TITLE_JPG, m_titile_jpg);
	DDX_Control(pDX, IDC_BTN_TITLE_MAP, m_titile_map);
	DDX_Control(pDX, IDC_BTN_TITLE_LOOOPLAY, m_titile_loopplay);
	DDX_Control(pDX, IDC_BTN_TITLE_SC_1, m_titile_sc_1);
	DDX_Control(pDX, IDC_BTN_TITLE_SC_4, m_titile_sc_4);
	DDX_Control(pDX, IDC_BTN_TITLE_SC_6, m_titile_sc_6);
	DDX_Control(pDX, IDC_BTN_TITLE_SC_8, m_titile_sc_8);
	DDX_Control(pDX, IDC_BTN_TITLE_SC_9, m_titile_sc_9);
	DDX_Control(pDX, IDC_BTN_TITLE_SC_16, m_titile_sc_16);
	DDX_Control(pDX, IDC_BTN_TITLE_LOCK, m_titile_lock);
	DDX_Control(pDX, IDC_BTN_TITLE_FULLSC, m_titile_fullscreen);
	DDX_Control(pDX, IDC_BTN_TITLE_LB_1, m_lunboStart);
	DDX_Control(pDX, IDC_BTN_TITLE_LB_2, m_lunboPause);
	DDX_Control(pDX, IDC_BTN_TITLE_MLOG, m_logbtn);
}
BEGIN_MESSAGE_MAP(CTitleDlg, CDialogEx)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CTitleDlg::OnBnClickedButtonClose)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_MIN, &CTitleDlg::OnBnClickedButtonMin)
	ON_BN_CLICKED(IDC_BUTTON_MAX, &CTitleDlg::OnBnClickedButtonMax)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_TITLE_FULLSC, &CTitleDlg::OnBnClickedBtnTitleFullsc)
	ON_BN_CLICKED(IDC_BTN_TITLE_LOCK, &CTitleDlg::OnBnClickedBtnTitleLock)
	ON_BN_CLICKED(IDC_BTN_TITLE_SC_1, &CTitleDlg::OnBnClickedBtnTitleSc1)
	ON_BN_CLICKED(IDC_BTN_TITLE_SC_4, &CTitleDlg::OnBnClickedBtnTitleSc4)
	ON_BN_CLICKED(IDC_BTN_TITLE_SC_6, &CTitleDlg::OnBnClickedBtnTitleSc6)
	ON_BN_CLICKED(IDC_BTN_TITLE_SC_8, &CTitleDlg::OnBnClickedBtnTitleSc8)
	ON_BN_CLICKED(IDC_BTN_TITLE_SC_9, &CTitleDlg::OnBnClickedBtnTitleSc9)
	ON_BN_CLICKED(IDC_BTN_TITLE_SC_16, &CTitleDlg::OnBnClickedBtnTitleSc16)
	//
	ON_BN_CLICKED(IDC_BTN_TITLE_LIVE, &CTitleDlg::OnBnClickedBtnTitleLive)
	//ON_BN_CLICKED(IDC_BTN_TITLE_REC, &CTitleDlg::OnBnClickedBtnTitleRec)
	//ON_BN_CLICKED(IDC_BTN_TITLE_JPG, &CTitleDlg::OnBnClickedBtnTitleJpg)
	ON_BN_CLICKED(IDC_BTN_TITLE_MAP, &CTitleDlg::OnBnClickedBtnTitleMap)
	ON_BN_CLICKED(IDC_BTN_TITLE_LOOOPLAY, &CTitleDlg::OnBnClickedBtnTitleLooplay)
	//
	ON_BN_CLICKED(IDC_BTN_TITLE_LB_1, &CTitleDlg::OnBnClickedBtnTitleLb1)
	ON_BN_CLICKED(IDC_BTN_TITLE_LB_2, &CTitleDlg::OnBnClickedBtnTitleLb2)
	ON_BN_CLICKED(IDC_BTN_TITLE_MLOG, &CTitleDlg::OnBnClickedBtnTitleMlog)
	ON_BN_CLICKED(IDC_DROP_DOWN, &CTitleDlg::OnBnClickedDropDown)
END_MESSAGE_MAP()
// CTitleDlg 消息处理程序
LRESULT CTitleDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	AfxGetMainWnd()->SendMessage(WM_SYSCOMMAND, 0xF012, 0);
	return CDialogEx::OnNcHitTest(point);
}
HBRUSH CTitleDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pDC == NULL || pDC->m_hDC == NULL)
	{
		return hbr;
	}
	if ((pWnd->GetDlgCtrlID() == IDC_STATIC_99))
	{
		//pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	return theApp.hbrushDlg;
}


BOOL CTitleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//
	CString cstr;
	//
	ButtonLoadBitmap(m_dropdown, L"title_bar/drop_1.png", L"title_bar/drop_2.png", L"title_bar/drop_3.png", L"title_bar/drop_4.png");
	ButtonLoadBitmap(m_min, L"title_bar/min_1.png", L"title_bar/min_2.png", L"title_bar/min_3.png", L"title_bar/min_4.png");
	ButtonLoadBitmap(m_max, L"title_bar/max_1.png", L"title_bar/max_2.png", L"title_bar/max_3.png", L"title_bar/max_4.png");
	ButtonLoadBitmap(m_close, L"title_bar/close_1.png", L"title_bar/close_2.png", L"title_bar/close_3.png", L"title_bar/close_4.png");
	//
	Get_String_From_Language_Ini_File(_T("str_dlg_180115_msg2"), cstr);
	ButtonLoadBitmap(m_titile_live, L"title_bar/b/live_1.png", L"title_bar/b/live_2.png", L"title_bar/b/live_3.png", L"title_bar/b/live_4.png", -1, cstr);
// 	Get_String_From_Language_Ini_File(_T("str_dlg_180115_msg3"), cstr);
// 	ButtonLoadBitmap(m_titile_lunbo, L"title_bar/b/rec_1.png", L"title_bar/b/rec_2.png", L"title_bar/b/rec_3.png", L"title_bar/b/rec_4.png", -1, cstr);
// 	Get_String_From_Language_Ini_File(_T("str_dlg_180115_msg4"), cstr);
// 	ButtonLoadBitmap(m_titile_jpg, L"title_bar/b/jpg_1.png", L"title_bar/b/jpg_2.png", L"title_bar/b/jpg_3.png", L"title_bar/b/jpg_4.png", -1, cstr);
	Get_String_From_Language_Ini_File(_T("str_dlg_180115_msg5"), cstr);
	ButtonLoadBitmap(m_titile_map, L"title_bar/b/map_1.png", L"title_bar/b/map_2.png", L"title_bar/b/map_3.png", L"title_bar/b/map_4.png", -1, cstr);
	Get_String_From_Language_Ini_File(_T("str_dlg_180115_msg6"), cstr);
	ButtonLoadBitmap(m_titile_loopplay, L"title_bar/b/log_1.png", L"title_bar/b/log_2.png", L"title_bar/b/log_3.png", L"title_bar/b/log_4.png", -1, cstr);
	//
	Get_String_From_Language_Ini_File(_T("str_title_dlg_screen_tooltip_1"), cstr);
	ButtonLoadBitmap(m_titile_sc_1, L"title_bar/c/sc_1_1.png", L"title_bar/c/sc_1_2.png", L"title_bar/c/sc_1_3.png", L"title_bar/c/sc_1_4.png", -1, cstr);
	Get_String_From_Language_Ini_File(_T("str_title_dlg_screen_tooltip_4"), cstr);
	ButtonLoadBitmap(m_titile_sc_4, L"title_bar/c/sc_4_1.png", L"title_bar/c/sc_4_2.png", L"title_bar/c/sc_4_3.png", L"title_bar/c/sc_4_4.png", -1, cstr);
	Get_String_From_Language_Ini_File(_T("str_title_dlg_screen_tooltip_6"), cstr);
	ButtonLoadBitmap(m_titile_sc_6, L"title_bar/c/sc_6_1.png", L"title_bar/c/sc_6_2.png", L"title_bar/c/sc_6_3.png", L"title_bar/c/sc_6_4.png", -1, cstr);
	Get_String_From_Language_Ini_File(_T("str_title_dlg_screen_tooltip_8"), cstr);
	ButtonLoadBitmap(m_titile_sc_8, L"title_bar/c/sc_8_1.png", L"title_bar/c/sc_8_2.png", L"title_bar/c/sc_8_3.png", L"title_bar/c/sc_8_4.png", -1, cstr);
	Get_String_From_Language_Ini_File(_T("str_title_dlg_screen_tooltip_9"), cstr);
	ButtonLoadBitmap(m_titile_sc_9, L"title_bar/c/sc_9_1.png", L"title_bar/c/sc_9_2.png", L"title_bar/c/sc_9_3.png", L"title_bar/c/sc_9_4.png", -1, cstr);
	Get_String_From_Language_Ini_File(_T("str_title_dlg_screen_tooltip_16"), cstr);
	ButtonLoadBitmap(m_titile_sc_16, L"title_bar/c/sc_16_1.png", L"title_bar/c/sc_16_2.png", L"title_bar/c/sc_16_3.png", L"title_bar/c/sc_16_4.png", -1, cstr);
	//
	Get_String_From_Language_Ini_File(_T("str_title_dlg_screen_tooltip_full"), cstr);
	ButtonLoadBitmap(m_titile_fullscreen, L"title_bar/d/fs_1.png", L"title_bar/d/fs_2.png", L"title_bar/d/fs_3.png", L"title_bar/d/fs_4.png", -1, cstr);
	Get_String_From_Language_Ini_File(_T("str_dlg_180115_msg1"), cstr);
	ButtonLoadBitmap(m_titile_lock, L"title_bar/d/lock_1.png", L"title_bar/d/lock_2.png", L"title_bar/d/lock_3.png", L"title_bar/d/lock_4.png", -1, cstr);
	Get_String_From_Language_Ini_File(_T("str_dlg_180115_msg6"), cstr);
	ButtonLoadBitmap(m_logbtn, L"title_bar/b/log_1.png", L"title_bar/b/log_2.png", L"title_bar/b/log_3.png", L"title_bar/b/log_4.png", -1, cstr);
	//
	Get_String_From_Language_Ini_File(_T("str_title_dlg_screen_tooltip_1"), cstr);
	ButtonLoadBitmap(m_lunboStart, L"title_bar/c/sc_1_1.png", L"title_bar/c/sc_1_2.png", L"title_bar/c/sc_1_3.png", L"title_bar/c/sc_1_4.png", -1, cstr);
	Get_String_From_Language_Ini_File(_T("str_title_dlg_screen_tooltip_1"), cstr);
	ButtonLoadBitmap(m_lunboPause, L"title_bar/c/sc_1_1.png", L"title_bar/c/sc_1_2.png", L"title_bar/c/sc_1_3.png", L"title_bar/c/sc_1_4.png", -1, cstr);
	//
	InitFrameString();
	//
	PostMessage(WM_SIZE);
	//
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CTitleDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_min.GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	int nspace = 0;
	int mLeft = 0;
	int ntp = 42;
	//------------------------------------------------------------------------------------------------
	nspace = 0;
	m_dropdown.SetWindowPos(NULL, rectClient.left + rectClient.Width() - nspace * 4 - theApp.m_nCloseWidth * 4, 0, theApp.m_nCloseWidth, theApp.m_nCloseHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	m_min.SetWindowPos(NULL, rectClient.left + rectClient.Width() - nspace * 3 - theApp.m_nCloseWidth * 3, 0, theApp.m_nCloseWidth, theApp.m_nCloseHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	m_max.SetWindowPos(NULL, rectClient.left + rectClient.Width() - nspace * 2 - theApp.m_nCloseWidth * 2, 0, theApp.m_nCloseWidth, theApp.m_nCloseHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	m_close.SetWindowPos(NULL, rectClient.left + rectClient.Width() - nspace - theApp.m_nCloseWidth, 0, theApp.m_nCloseWidth, theApp.m_nCloseHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	//
	mLeft = 1;
	nspace = 48;


#if 0
	m_titile_live.ShowWindow(SW_HIDE);
	m_titile_rec.ShowWindow(SW_HIDE);
	m_titile_jpg.ShowWindow(SW_HIDE);
	m_titile_map.ShowWindow(SW_HIDE);
	m_titile_log.ShowWindow(SW_HIDE);

	//
	m_titile_sc_1.SetWindowPos(NULL, rectClient.left + mLeft + nspace * 0, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_sc_4.SetWindowPos(NULL, rectClient.left + mLeft + nspace * 1, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_sc_6.SetWindowPos(NULL, rectClient.left + mLeft + nspace * 2, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_sc_8.SetWindowPos(NULL, rectClient.left + mLeft + nspace * 3, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_sc_9.SetWindowPos(NULL, rectClient.left + mLeft + nspace * 4, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_sc_16.SetWindowPos(NULL, rectClient.left + mLeft + nspace * 5, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	//
	m_titile_lock.SetWindowPos(NULL, rectClient.left + rectClient.Width() - 48 * 2 - 5, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_fullscreen.SetWindowPos(NULL, rectClient.left + rectClient.Width() - 49, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
#endif


#if 1
#if 0
	m_titile_live.SetWindowPos(NULL, rectClient.left + nspace * 0, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_rec.SetWindowPos(NULL, rectClient.left + nspace * 1, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_jpg.SetWindowPos(NULL, rectClient.left + nspace * 2, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_map.SetWindowPos(NULL, rectClient.left + nspace * 3, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_log.SetWindowPos(NULL, rectClient.left + nspace * 4, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
#else
	//m_titile_jpg.ShowWindow(SW_HIDE);
	ntp = 0;
	nspace = 96;
	m_titile_map.ShowWindow(SW_HIDE);
	m_titile_live.SetWindowPos(NULL, rectClient.left + nspace * 0, ntp, nspace, 32, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_loopplay.SetWindowPos(NULL, rectClient.left + nspace * 1, ntp, nspace, 32, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_map.SetWindowPos(NULL, rectClient.left + nspace * 2, ntp, nspace, 32, SWP_NOACTIVATE | SWP_NOZORDER);
#endif
	//
	ntp = 37;
	mLeft = 0;// 340;
	nspace = 48;
	m_titile_sc_1.SetWindowPos(NULL, rectClient.left + mLeft + nspace * 0, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_sc_4.SetWindowPos(NULL, rectClient.left + mLeft + nspace * 1, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_sc_6.SetWindowPos(NULL, rectClient.left + mLeft + nspace * 2, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_sc_8.SetWindowPos(NULL, rectClient.left + mLeft + nspace * 3, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_sc_9.SetWindowPos(NULL, rectClient.left + mLeft + nspace * 4, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_sc_16.SetWindowPos(NULL, rectClient.left + mLeft + nspace * 5, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	//
	m_lunboStart.SetWindowPos(NULL, rectClient.left + mLeft + nspace * 0, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	m_lunboPause.SetWindowPos(NULL, rectClient.left + mLeft + nspace * 1, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	//
	m_logbtn.SetWindowPos(NULL, rectClient.left + rectClient.Width() - 48 * 3 - 5, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_lock.SetWindowPos(NULL, rectClient.left + rectClient.Width() - 48 * 2 - 5, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
	m_titile_fullscreen.SetWindowPos(NULL, rectClient.left + rectClient.Width() - 49, ntp, 48, 48, SWP_NOACTIVATE | SWP_NOZORDER);
#endif


}

void CTitleDlg::OnBnClickedButtonMin()
{
	AfxGetMainWnd()->SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}
void CTitleDlg::OnBnClickedButtonMax()
{
	if (AfxGetMainWnd()->IsZoomed())
		AfxGetMainWnd()->SendMessage(WM_SYSCOMMAND, SC_RESTORE);
	else
		AfxGetMainWnd()->SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CTitleDlg::InitFrameString()
{
	m_title.SetWindowText(theApp.strCompany);
}
void CTitleDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;
	CDialogEx::PostNcDestroy();
}
BOOL CTitleDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
		if (pMsg->wParam == VK_ESCAPE)
		{
			CPlatClientDlg *pWnd = (CPlatClientDlg *)m_pParent;
			if (pWnd->m_fFullScreen)
			{
				pWnd->ShowWindowMax(FALSE);
			}
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
void CTitleDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnBnClickedButtonMax();
	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CTitleDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	OnPaintBmpBackground(this, dc);
}


BOOL CTitleDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CTitleDlg::OnBnClickedButtonClose()
{
	//
#if 0
	if (IDCANCEL == AfxMessageBox(_T("您确定要退出吗"), MB_OKCANCEL))
	{
		return;
	}
#endif



	CPlatClientDlg *pWnd = (CPlatClientDlg *)m_pParent;

	for (int n = 0; n < MAX_WINDOWS_NUM; n++)
	{
		{
			pWnd->m_pDlgPanLive->XViewCloseStream(n);
		}
	}
	pWnd->m_pDlgPanLive->m_tree.Deinit();
	//pWnd->m_pDlgPanRecord->ClearList();

	//pWnd->m_pDlgPanRecord->OnlineHisClose();


	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}

// 
// void CTitleDlg::OnScreenOne()
// {
// 	ShowViewNuber(1);
// }
// 
// 
// void CTitleDlg::OnScreenFoure()
// {
// 	ShowViewNuber(4);
// }
// void CTitleDlg::On32815()
// {
// 	ShowViewNuber(6);
// }
// 
// 
// void CTitleDlg::On32816()
// {
// 	ShowViewNuber(8);
// }
// 
// 
// void CTitleDlg::OnScreenNine()
// {
// 	ShowViewNuber(9);
// }
// 
// 
// void CTitleDlg::OnScreenSixteen()
// {
// 	ShowViewNuber(16);
// }


// #include "SearchDlg.h"
// void CTitleDlg::On32817Search()
// {
// 	CSearchDlg dlg;
// 	dlg.DoModal();
// }


void CTitleDlg::OnBnClickedBtnTitleFullsc()
{
	//全屏
	CPlatClientDlg *pWnd = (CPlatClientDlg *)m_parentDlg;
	pWnd->ShowWindowMax();
}

#include "LockDlg.h"
void CTitleDlg::OnBnClickedBtnTitleLock()
{
	//锁屏
	CLockDlg dlg;
	dlg.DoModal();
}

//////////////////////////////////////////////////////////////////////////
void CTitleDlg::OnBnClickedBtnTitleSc1()
{
	ShowLivePanViewNumber(1);
}
void CTitleDlg::OnBnClickedBtnTitleSc4()
{
	ShowLivePanViewNumber(4);
}
void CTitleDlg::OnBnClickedBtnTitleSc6()
{
	ShowLivePanViewNumber(6);
}
void CTitleDlg::OnBnClickedBtnTitleSc8()
{
	ShowLivePanViewNumber(8);
}
void CTitleDlg::OnBnClickedBtnTitleSc9()
{
	ShowLivePanViewNumber(9);
}
void CTitleDlg::OnBnClickedBtnTitleSc16()
{
	ShowLivePanViewNumber(16);
}
void CTitleDlg::ShowLivePanViewNumber(int n)
{
	theApp.m_clientSetting.spiltScreen = n;
	CSysFileGetSet::Instance().Set(theApp.m_clientSetting);
	//
	CPlatClientDlg *pWnd = (CPlatClientDlg *)m_pParent;
	pWnd->m_pDlgPanLive->m_pVideoDlg->ShowVideoWindows(n);
	//
	m_titile_sc_1.EnableWindow(TRUE);
	m_titile_sc_4.EnableWindow(TRUE);
	m_titile_sc_6.EnableWindow(TRUE);
	m_titile_sc_8.EnableWindow(TRUE);
	m_titile_sc_9.EnableWindow(TRUE);
	m_titile_sc_16.EnableWindow(TRUE);



	switch (theApp.m_clientSetting.spiltScreen)
	{
	case 1:
		m_titile_sc_1.EnableWindow(FALSE);
		break;
	case 4:
		m_titile_sc_4.EnableWindow(FALSE);
		break;
	case 6:
		m_titile_sc_6.EnableWindow(FALSE);
		break;
	case 8:
		m_titile_sc_8.EnableWindow(FALSE);
		break;
	case 9:
		m_titile_sc_9.EnableWindow(FALSE);
		break;
	case 16:
		m_titile_sc_16.EnableWindow(FALSE);
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
void CTitleDlg::OnBnClickedBtnTitleLive()
{
	ShowPan(EM_PAN_TYPE_LIVE);
}
// void CTitleDlg::OnBnClickedBtnTitleRec()
// {
// 	ShowPan(EM_PAN_TYPE_LOOPPLAY);
// }
// void CTitleDlg::OnBnClickedBtnTitleJpg()
// {
// 	ShowPan(EM_PAN_TYPE_JPG);
// }
void CTitleDlg::OnBnClickedBtnTitleMap()
{
	ShowPan(EM_PAN_TYPE_MAP);
}
void CTitleDlg::OnBnClickedBtnTitleLooplay()
{
	ShowPan(EM_PAN_TYPE_LOOPPLAY);
}
void CTitleDlg::ShowPan(MyEnumPanType _type)
{

#if 1
	CPlatClientDlg *pWnd = (CPlatClientDlg *)m_pParent;
	pWnd->ShowPan(_type);


	//
	m_titile_live.EnableWindow(TRUE);
// 	m_titile_lunbo.EnableWindow(TRUE);
// 	m_titile_jpg.EnableWindow(TRUE);
	m_titile_map.EnableWindow(TRUE);
	m_titile_loopplay.EnableWindow(TRUE);
	//
	m_titile_sc_1.ShowWindow(SW_HIDE);
	m_titile_sc_4.ShowWindow(SW_HIDE);
	m_titile_sc_6.ShowWindow(SW_HIDE);
	m_titile_sc_8.ShowWindow(SW_HIDE);
	m_titile_sc_9.ShowWindow(SW_HIDE);
	m_titile_sc_16.ShowWindow(SW_HIDE);
	//
	m_lunboStart.ShowWindow(SW_HIDE);
	m_lunboPause.ShowWindow(SW_HIDE);
	switch (_type)
	{
	case EM_PAN_TYPE_LIVE:
		m_titile_sc_1.ShowWindow(SW_SHOW);
		m_titile_sc_4.ShowWindow(SW_SHOW);
		m_titile_sc_6.ShowWindow(SW_SHOW);
		m_titile_sc_8.ShowWindow(SW_SHOW);
		m_titile_sc_9.ShowWindow(SW_SHOW);
		m_titile_sc_16.ShowWindow(SW_SHOW);
		m_titile_live.EnableWindow(FALSE);
		break;
	case EM_PAN_TYPE_LOOPPLAY:
		m_titile_loopplay.EnableWindow(FALSE);
		m_lunboStart.ShowWindow(SW_SHOW);
		m_lunboPause.ShowWindow(SW_SHOW);
		break;
// 	case EM_PAN_TYPE_REC:
// 		m_titile_rec.EnableWindow(FALSE);
// 		break;
// 	case EM_PAN_TYPE_JPG:
// 		m_titile_jpg.EnableWindow(FALSE);
// 		break;
// 	case EM_PAN_TYPE_MAP:
// 		m_titile_map.EnableWindow(FALSE);
// 		break;
// 	case EM_PAN_TYPE_LOG:
// 		m_titile_loopplay.EnableWindow(FALSE);
// 		break;
	default:
		break;
	}
#endif

}

void CTitleDlg::OnBnClickedBtnTitleLb1()
{
	//开始/停止 轮播
	CLoopPlaySettingDlg dlg;
	if (dlg.DoModal() == IDOK)
	{

		CPlatClientDlg * pMainWnd = (CPlatClientDlg *)AfxGetMainWnd();
		pMainWnd->m_pDlgLoopplay->StartLoopplay();
	}
}


void CTitleDlg::OnBnClickedBtnTitleLb2()
{
	//暂停轮播
	CPlatClientDlg * pMainWnd = (CPlatClientDlg *)AfxGetMainWnd();
	pMainWnd->m_pDlgLoopplay->StopLoopplay();
}


void CTitleDlg::OnBnClickedBtnTitleMlog()
{
	CLogDialog *m_pDlgLog = new CLogDialog(this);
	m_pDlgLog->Create(CLogDialog::IDD, this);
	m_pDlgLog->ModifyStyle(0, WS_CHILDWINDOW);
	m_pDlgLog->ShowWindow(SW_SHOW);
}

#include "SysSettingDlg.h"
void CTitleDlg::OnBnClickedDropDown()
{
	// TODO: Add your control notification handler code here

	CSysSettingDlg dlg;
	dlg.DoModal();


}
