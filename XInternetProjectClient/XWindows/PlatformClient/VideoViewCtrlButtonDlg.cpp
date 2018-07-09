// VideoViewPanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "VideoViewCtrlButtonDlg.h"
#include "afxdialogex.h"
#include "PlatformClientDlg.h"
// CVideoViewPanDlg dialog
IMPLEMENT_DYNAMIC(CVideoViewCtrlButton, CDialogEx)

CVideoViewCtrlButton::CVideoViewCtrlButton(CWnd* pParent /*=NULL*/)
: CDialogEx(CVideoViewCtrlButton::IDD, pParent)
{
	m_bAudio = FALSE;
	m_bTalk = FALSE;
	m_bRecord = FALSE;
	m_pVideoView = pParent;
}
CVideoViewCtrlButton::~CVideoViewCtrlButton()
{
}
void CVideoViewCtrlButton::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_title);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btn_close);
	DDX_Control(pDX, IDC_BTN_CAP, m_btn_cap);
	DDX_Control(pDX, IDC_BTN_SOUND, m_btn_sound);
	DDX_Control(pDX, IDC_BTN_TALK, m_btn_talk);
	DDX_Control(pDX, IDC_BTN_RECORD, m_btn_record);
	DDX_Control(pDX, IDC_BTN_MEMU, m_btnSet);
}
BEGIN_MESSAGE_MAP(CVideoViewCtrlButton, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CVideoViewCtrlButton::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_CAP, &CVideoViewCtrlButton::OnBnClickedBtnCap)
	ON_BN_CLICKED(IDC_BTN_RECORD, &CVideoViewCtrlButton::OnBnClickedBtnRecord)
	ON_BN_CLICKED(IDC_BTN_TALK, &CVideoViewCtrlButton::OnBnClickedBtnTalk)
	ON_BN_CLICKED(IDC_BTN_SOUND, &CVideoViewCtrlButton::OnBnClickedBtnSound)
	ON_BN_CLICKED(IDC_BTN_MEMU, &CVideoViewCtrlButton::OnBnClickedBtnParam)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CVideoViewPanDlg message handlers
BOOL CVideoViewCtrlButton::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	ButtonLoadBitmap(m_btn_sound, L"viewpan/sound_1.png", L"viewpan/sound_2.png", L"viewpan/sound_3.png", L"viewpan/sound_4.png");
	ButtonLoadBitmap(m_btn_talk, L"viewpan/talk_1.png", L"viewpan/talk_2.png", L"viewpan/talk_3.png", L"viewpan/talk_4.png");
	ButtonLoadBitmap(m_btn_record, L"viewpan/record_1.png", L"viewpan/record_2.png", L"viewpan/record_3.png", L"viewpan/record_4.png");
	ButtonLoadBitmap(m_btn_cap, L"viewpan/cap_1.png", L"viewpan/cap_2.png", L"viewpan/cap_3.png", L"viewpan/cap_4.png");
	ButtonLoadBitmap(m_btn_close, L"viewpan/close_1.png", L"viewpan/close_2.png", L"viewpan/close_3.png", L"viewpan/close_4.png");
	ButtonLoadBitmap(m_btnSet, L"viewpan/menu_1.png", L"viewpan/menu_2.png", L"viewpan/menu_3.png", L"viewpan/menu_4.png");
	//
	m_title.ModifyStyle(0, SS_CENTERIMAGE);
	m_title.ShowWindow(SW_HIDE);
	//
	inilg();
	//
#if 1
	//在OnInitDialog()中加入以下代码：
	//加入WS_EX_LAYERED扩展属性
	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE,
		GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) ^ 0x80000);
	HINSTANCE hInst = LoadLibrary(L"User32.DLL");
	if (hInst)
	{
		typedef BOOL(WINAPI *MYFUNC)(HWND, COLORREF, BYTE, DWORD);
		MYFUNC fun = NULL;
		//取得SetLayeredWindowAttributes函数指针
		fun = (MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		//if (fun)fun(this->GetSafeHwnd(), RGB(240,240,240), 64, 2);//注意：fun的参数128不能太小，否则就完全透明了！
		if (fun)fun(this->GetSafeHwnd(), RGB(240, 240, 240), 128, 1);//注意：fun的参数128不能太小，否则就完全透明了！
		FreeLibrary(hInst);
	}
	//HWND hWnd : 窗口句柄
	//COLORREF col : 透明化颜色
	//BYTE bAlpha : ＝0：整个窗口透明, = 255 完全不透明
	//DWORD dwFlags : ＝1：仅颜色 col 透明， = 2 : 窗口按照bAlpha变量进行透明处理。
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CVideoViewCtrlButton::inilg()
{
	CString strName;

	Get_String_From_Language_Ini_File(_T("IDS_STRING3"), strName);
	m_btn_sound.SetTooltipText(strName);
	m_btn_sound.SetWindowText(_T(""));

	Get_String_From_Language_Ini_File(_T("IDS_STRING4"), strName);
	m_btn_talk.SetTooltipText(strName);
	m_btn_talk.SetWindowText(_T(""));

	Get_String_From_Language_Ini_File(_T("IDS_STRING5"), strName);
	m_btn_record.SetTooltipText(strName);
	m_btn_record.SetWindowText(_T(""));

	Get_String_From_Language_Ini_File(_T("IDS_STRING2"), strName);
	m_btn_cap.SetTooltipText(strName);
	m_btn_cap.SetWindowText(_T(""));

	Get_String_From_Language_Ini_File(_T("IDS_STRING1"), strName);
	m_btn_close.SetTooltipText(strName);
	m_btn_close.SetWindowText(_T(""));

	Get_String_From_Language_Ini_File(_T("IDS_STRING1"), strName);
	m_btnSet.SetTooltipText(strName);
	m_btnSet.SetWindowText(_T(""));
}

void CVideoViewCtrlButton::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}
BOOL CVideoViewCtrlButton::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_KEYDOWN == pMsg->message)
	{
		UINT nKey = (int)pMsg->wParam;
		if (VK_RETURN == nKey){
			return TRUE;
		}

		if (VK_ESCAPE == nKey){
			CVideoViewDlg *pParent = (CVideoViewDlg *)m_pVideoView;
			CPlatClientDlg *pWnd = (CPlatClientDlg *)(pParent->m_pAfxMainWnd);
			if (pWnd->m_fFullScreen)
			{
				pWnd->ShowWindowMax(FALSE);
			}
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CVideoViewCtrlButton::OnBnClickedBtnClose()
{
	CVideoViewDlg *pParent = (CVideoViewDlg *)m_pVideoView;
	pParent->XViewCloseStream();
	pParent->drawSelected();
	//
	pParent->m_bCanShowCtrlPan = FALSE;
//	pParent->m_dwMouseMoveTick = 0;
	m_title.SetWindowText(_T(""));
	pParent->m_view.SetWindowText(_T(""));
	pParent->m_view.Invalidate();
	//
	m_bAudio = FALSE;
	m_bTalk = FALSE;
	m_bRecord = FALSE;
	//
	ButtonLoadBitmap(m_btn_sound, L"viewpan/sound_1.png", L"viewpan/sound_2.png", L"viewpan/sound_3.png", L"viewpan/sound_4.png");
	ButtonLoadBitmap(m_btn_talk, L"viewpan/talk_1.png", L"viewpan/talk_2.png", L"viewpan/talk_3.png", L"viewpan/talk_4.png");
	ButtonLoadBitmap(m_btn_record, L"viewpan/record_1.png", L"viewpan/record_2.png", L"viewpan/record_3.png", L"viewpan/record_4.png");
}
void CVideoViewCtrlButton::OnBnClickedBtnCap()
{
	CVideoViewDlg *pParent = (CVideoViewDlg *)m_pVideoView;
	pParent->drawSelected();
	pParent->XViewCapturePictures();
}
void CVideoViewCtrlButton::OnBnClickedBtnRecord()
{
	CVideoViewDlg *pParent = (CVideoViewDlg *)m_pVideoView;
	pParent->drawSelected();
	m_bRecord = !m_bRecord;
	int m = m_bRecord ? 1 : 0;
	pParent->XViewRecord(m);
	if (m_bRecord)
	{
		ButtonLoadBitmap(m_btn_record, L"viewpan/record_2.png", L"viewpan/record_1.png", L"viewpan/record_3.png", L"viewpan/record_4.png");
	}
	else
	{
		ButtonLoadBitmap(m_btn_record, L"viewpan/record_1.png", L"viewpan/record_2.png", L"viewpan/record_3.png", L"viewpan/record_4.png");
	}
}

void CVideoViewCtrlButton::OnBnClickedBtnTalk()
{
	// 	CVideoViewDlg *pParent = (CVideoViewDlg *)m_pVideoView;
	// 
	// 	pParent->drawSelected();
	// 	m_bTalk = !m_bTalk;
	// 	int m = m_bTalk ? 1 : 0;
	// 	pParent->m_pWnd->SendMessage(WM_VIDEO_WINDOW_TALK, pParent->m_nIndex, m);
	// 	if (m_bTalk)
	// 	{
	// 		ButtonLoadBitmap(m_btn_talk,
	// 			L"viewpan/talk_2.png",
	// 			L"viewpan/talk_1.png",
	// 			L"viewpan/talk_3.png",
	// 			L"viewpan/talk_4.png");
	// 	}
	// 	else
	// 	{
	// 		ButtonLoadBitmap(m_btn_talk,
	// 			L"viewpan/talk_1.png",
	// 			L"viewpan/talk_2.png",
	// 			L"viewpan/talk_3.png",
	// 			L"viewpan/talk_4.png");
	// 	}
}

void CVideoViewCtrlButton::OnBnClickedBtnSound()
{
	CVideoViewDlg *pParent = (CVideoViewDlg *)m_pVideoView;
	pParent->drawSelected();
	m_bAudio = !m_bAudio;
	int m = m_bAudio ? 1 : 0;
	pParent->XViewSound(m);
	if (m_bAudio)
	{
		ButtonLoadBitmap(m_btn_sound, L"viewpan/sound_2.png", L"viewpan/sound_1.png", L"viewpan/sound_3.png", L"viewpan/sound_4.png");
	}
	else
	{
		ButtonLoadBitmap(m_btn_sound, L"viewpan/sound_1.png", L"viewpan/sound_2.png", L"viewpan/sound_3.png", L"viewpan/sound_4.png");
	}
}
void CVideoViewCtrlButton::OnBnClickedBtnParam()
{
	//CVideoViewDlg *pParent = (CVideoViewDlg *)m_pVideoView;
	//pParent->ParamSet();
}
void CVideoViewCtrlButton::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	if (m_btn_close.GetSafeHwnd() == NULL)
	{
		return;
	}
	//m_btn_close.ShowWindow(SW_HIDE);
	m_title.ShowWindow(SW_HIDE);
	//
	CRect rt;
	GetClientRect(&rt);
	int Btn_Space = 0;
	int www = 28;
	int hhh = 24;
	//

	//
#if 0
	m_title.MoveWindow(1, rt.Height() - hhh, rt.Width() - www * 4 - Btn_Space * 3 - 3, hhh);
	m_btn_sound.MoveWindow		(rt.Width() - www * 4 - Btn_Space * 3, rt.Height() - hhh, www, hhh, SWP_NOSIZE);
	m_btn_record.MoveWindow(rt.Width() - www * 3 - Btn_Space * 2, rt.Height() - hhh, www, hhh, SWP_NOSIZE);
	m_btn_cap.MoveWindow(rt.Width() - www * 2 - Btn_Space * 1, rt.Height() - hhh, www, hhh, SWP_NOSIZE);
	m_btn_close.MoveWindow(rt.Width() - www * 1 - Btn_Space * 0, rt.Height() - hhh, www, hhh, SWP_NOSIZE);
#endif
	m_btn_sound.MoveWindow(rt.Width() - www * 4 - Btn_Space * 3, rt.Height() - hhh, www, hhh, SWP_NOSIZE);
	m_btn_record.MoveWindow(rt.Width() - www * 3 - Btn_Space * 2, rt.Height() - hhh, www, hhh, SWP_NOSIZE);
	m_btn_cap.MoveWindow(rt.Width() - www * 2 - Btn_Space * 1, rt.Height() - hhh, www, hhh, SWP_NOSIZE);
	m_btn_close.MoveWindow(rt.Width() - www * 1 - Btn_Space * 0, rt.Height() - hhh, www, hhh, SWP_NOSIZE);
}
HBRUSH CVideoViewCtrlButton::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TITLE)
	{
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetTextColor(RGB(255, 255, 255));
		//return (HBRUSH)theApp.hViewTitleBrushBg;
	}
	//return (HBRUSH)theApp.hViewTitleBrushBg;
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
