// SysSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "SysSettingDlg.h"
#include "afxdialogex.h"


// CSysSettingDlg dialog

IMPLEMENT_DYNAMIC(CSysSettingDlg, CDialogEx)




CSysSettingDlg::CSysSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSysSettingDlg::IDD, pParent)
{
	m_pParent = pParent;
//	m_bitmapBG = NULL;
}

CSysSettingDlg::~CSysSettingDlg()
{
// 	if (m_bitmapBG)
// 	{
// 		CBitmap bmp;
// 		bmp.Attach(m_bitmapBG);
// 		bmp.Detach();
// 		DeleteObject(m_bitmapBG);
// 	}
}

void CSysSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_close);
	DDX_Control(pDX, IDC_EDIT_PIC, m_editPic);
	DDX_Control(pDX, IDC_EDIT_REC, m_editRecLocal);
	DDX_Control(pDX, IDC_COMBO3, m_ccScreen);
	//DDX_Control(pDX, IDC_COMBO4, m_reclLen);
	//DDX_Control(pDX, IDC_COMBO5, m_recTime);
	//DDX_Control(pDX, IDC_COMBO6, m_recSpace);
	//DDX_Control(pDX, IDC_REC_X_4, m_cover);
}


BEGIN_MESSAGE_MAP(CSysSettingDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSysSettingDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDOK, &CSysSettingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSysSettingDlg::OnBnClickedCancel)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BTN_PIC_SEL, &CSysSettingDlg::OnBnClickedBtnPicSel)
	ON_BN_CLICKED(IDC_BTN_REC_SEL, &CSysSettingDlg::OnBnClickedBtnRecSel)
	ON_BN_CLICKED(IDC_BTN_PIC_LOOK, &CSysSettingDlg::OnBnClickedBtnPicLook)
	ON_BN_CLICKED(IDC_BTN_REC_LOOK, &CSysSettingDlg::OnBnClickedBtnRecLook)
END_MESSAGE_MAP()


// CSysSettingDlg message handlers


BOOL CSysSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//---- 背景图 --------------------------------------------------
	//SetWindowPos(NULL, 0, 0, 640, 480, SWP_NOSIZE);
	CRect   temprect(0, 0, 640, 200);
	SetWindowPos(NULL, 0, 0, temprect.Width(), temprect.Height(), SWP_NOZORDER | SWP_NOMOVE);

	//img.Load(theApp.m_strSkinPath + L"bg/paper.png");
	//m_bitmapBG = img.Detach();






	//---- 最小化/最大化/关闭 --------------------------------------------------
	img.Load(theApp.m_strSkinPath + L"title_bar/close_1.png");
	m_nCloseWidth = img.GetWidth();//<-------------------------------------------------
	m_nCloseHeight = img.GetHeight();//<-------------------------------------------------

	img.Destroy();

	ButtonLoadBitmap(m_close,
		L"title_bar/close_1.png",
		L"title_bar/close_2.png",
		L"title_bar/close_3.png",
		L"title_bar/close_4.png");

	//---------------------------------------------------------------------------


	TCHAR	wszInfo[256];
	char szInfo[256];
	CString str;


	MultiByteToWideChar(CP_UTF8, 0, theApp.m_clientSetting.picPath, strlen(theApp.m_clientSetting.picPath) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
	m_editPic.SetWindowText(wszInfo);

	MultiByteToWideChar(CP_UTF8, 0, theApp.m_clientSetting.recPath, strlen(theApp.m_clientSetting.recPath) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
	m_editRecLocal.SetWindowText(wszInfo);

	sprintf_s(szInfo, "%d", theApp.m_clientSetting.spiltScreen);
	MultiByteToWideChar(CP_UTF8, 0, szInfo, strlen(szInfo) + 1,wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
	m_ccScreen.SetCurSel(m_ccScreen.FindString(0, wszInfo));

	//sprintf_s(szInfo, "%d", theApp.m_clientSetting.recSize);
	//MultiByteToWideChar(CP_UTF8, 0, szInfo, strlen(szInfo) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
	//m_reclLen.SetCurSel(m_reclLen.FindString(0, wszInfo));

	//sprintf_s(szInfo, "%d", theApp.m_clientSetting.recTime);
	//MultiByteToWideChar(CP_UTF8, 0, szInfo, strlen(szInfo) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
	//m_recTime.SetCurSel(m_recTime.FindString(0, wszInfo));

	//sprintf_s(szInfo, "%d", theApp.m_clientSetting.diskSpace);
	//MultiByteToWideChar(CP_UTF8, 0, szInfo, strlen(szInfo) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
	//m_recSpace.SetCurSel(m_recSpace.FindString(0, wszInfo));

	//m_cover.SetCheck(theApp.m_clientSetting.autoRemain);



	Set_String_To_Wnd_From_Language_Ini_File(IDC_SYS_SET_DLG_STATIC1, L"str_sys_store_path_pic", this);
	Set_String_To_Wnd_From_Language_Ini_File(IDC_SYS_SET_DLG_STATIC2, L"str_sys_store_path_record", this);


	Set_String_To_Wnd_From_Language_Ini_File(IDC_BTN_PIC_SEL, L"str_sys_select", this);
	Set_String_To_Wnd_From_Language_Ini_File(IDC_BTN_PIC_LOOK, L"str_sys_look", this);
	Set_String_To_Wnd_From_Language_Ini_File(IDC_BTN_REC_SEL, L"str_sys_select", this);
	Set_String_To_Wnd_From_Language_Ini_File(IDC_BTN_REC_LOOK, L"str_sys_look", this);


	Set_String_To_Wnd_From_Language_Ini_File(IDOK, L"str_confirm", this);
	Set_String_To_Wnd_From_Language_Ini_File(IDCANCEL, L"str_cancel", this);


	SendMessage(WM_SIZE, 0, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


HBRUSH CSysSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if ((pWnd->GetDlgCtrlID() == IDC_SYS_SET_DLG_STATIC1) ||
		(pWnd->GetDlgCtrlID() == IDC_SYS_SET_DLG_STATIC2) ||
		(pWnd->GetDlgCtrlID() == IDC_STATIC_LG2) 
		)
	{
		//pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}



	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CSysSettingDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	// TODO: 在此处添加消息处理程序代码
	if (m_close.GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int nspace = 0;


	//------------------------------------------------------------------------------------------------
	nspace = 0;
	m_close.SetWindowPos(NULL, rectClient.left + rectClient.Width() - nspace - m_nCloseWidth, 0, m_nCloseWidth, m_nCloseHeight, SWP_NOACTIVATE | SWP_NOZORDER);

}


BOOL CSysSettingDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CSysSettingDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	OnPaintBmpBackground(this, dc);
}


BOOL CSysSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSysSettingDlg::OnBnClickedButtonClose()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CSysSettingDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	CString str;
	m_editPic.GetWindowText(str);
	WideCharToMultiByte(CP_UTF8, NULL, str, -1, theApp.m_clientSetting.picPath, sizeof(theApp.m_clientSetting.picPath), NULL, FALSE);

	m_editRecLocal.GetWindowText(str);
	WideCharToMultiByte(CP_UTF8, NULL, str, -1, theApp.m_clientSetting.recPath, sizeof(theApp.m_clientSetting.recPath), NULL, FALSE);

	m_ccScreen.GetWindowText(str);
	theApp.m_clientSetting.spiltScreen = _wtoi(str);


	//m_reclLen.GetWindowText(str);
	//theApp.m_clientSetting.recSize = _wtoi(str);
	//m_recTime.GetWindowText(str);
	//theApp.m_clientSetting.recTime = _wtoi(str);
	//m_recSpace.GetWindowText(str);
	//theApp.m_clientSetting.diskSpace = _wtoi(str);
	//theApp.m_clientSetting.autoRemain = m_cover.GetCheck();



	CSysFileGetSet::Instance().Set(theApp.m_clientSetting);

	CDialogEx::OnOK();
}


void CSysSettingDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


LRESULT CSysSettingDlg::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//this->SendMessage(WM_SYSCOMMAND, 0xF012, 0);


	CRect rect;
	GetClientRect(rect);
	ClientToScreen(rect);
	rect.bottom = rect.top + theApp.m_NcHitHeight;

	if (rect.PtInRect(point))
		return HTCAPTION;



	return CDialogEx::OnNcHitTest(point);
}


void CSysSettingDlg::OnBnClickedBtnPicSel()
{
	BROWSEINFO   bi;
	TCHAR   Buffer[MAX_PATH];
	TCHAR   FullPath[MAX_PATH];
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;
	bi.lpszTitle = _T("Selection");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	ITEMIDLIST*   pidl = ::SHBrowseForFolder(&bi);
	if (::SHGetPathFromIDList(pidl, FullPath))
	{
		m_editPic.SetWindowText(FullPath);
	}
	else
	{

	}
}


void CSysSettingDlg::OnBnClickedBtnRecSel()
{
	BROWSEINFO   bi;
	TCHAR   Buffer[MAX_PATH];
	TCHAR   FullPath[MAX_PATH];
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;
	bi.lpszTitle = _T("");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	ITEMIDLIST*   pidl = ::SHBrowseForFolder(&bi);
	if (::SHGetPathFromIDList(pidl, FullPath))
	{
		m_editRecLocal.SetWindowText(FullPath);
	}
	else
	{

	}
}


void CSysSettingDlg::OnBnClickedBtnPicLook()
{
	CString str;
	m_editPic.GetWindowText(str);
	//str += L"Pictures";
	ShellExecute(NULL, NULL, str, NULL, NULL, SW_SHOW);
}


void CSysSettingDlg::OnBnClickedBtnRecLook()
{

	CString str;
	m_editRecLocal.GetWindowText(str);
	//str += L"RecordFiles";
	ShellExecute(NULL, NULL, str, NULL, NULL, SW_SHOW);
}
