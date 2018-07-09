// DeviceSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "DeviceSettingDlg.h"
#include "afxdialogex.h"


// CDeviceSettingDlg dialog

IMPLEMENT_DYNAMIC(CDeviceSettingDlg, CDialogEx)

CDeviceSettingDlg::CDeviceSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeviceSettingDlg::IDD, pParent)
{

}

CDeviceSettingDlg::~CDeviceSettingDlg()
{
}

void CDeviceSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabParam);
	DDX_Control(pDX, IDC_BUTTON3, m_close);
}


BEGIN_MESSAGE_MAP(CDeviceSettingDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDeviceSettingDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON3, &CDeviceSettingDlg::OnBnClickedButton3)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON1, &CDeviceSettingDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDeviceSettingDlg message handlers


BOOL CDeviceSettingDlg::PreTranslateMessage(MSG* pMsg)
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


BOOL CDeviceSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ButtonLoadBitmap(m_close, L"title_bar/close_1.png", L"title_bar/close_2.png", L"title_bar/close_3.png", L"title_bar/close_4.png");

	// TODO:  Add extra initialization here
	TCITEM item;
	item.mask = TCIF_TEXT;

	item.pszText = _T("编码参数");
	m_tabParam.InsertItem(0, &item);

	item.pszText = _T("图像参数");
	m_tabParam.InsertItem(1, &item);

	item.pszText = _T("有线网络");
	m_tabParam.InsertItem(2, &item);

	item.pszText = _T("无线网络");
	m_tabParam.InsertItem(3, &item);

	item.pszText = _T("OSD");
	m_tabParam.InsertItem(4, &item);

	item.pszText = _T("时间");
	m_tabParam.InsertItem(5, &item);

	item.pszText = _T("平台");
	m_tabParam.InsertItem(6, &item);


	CRect rcSub;
	GetDlgItem(IDC_STATIC_1)->GetWindowRect(rcSub);
	ScreenToClient(rcSub);
	//
	m_pDlgEnc = new CDeviceSettingDlgEncode(this);
	m_pDlgEnc->Create(CDeviceSettingDlgEncode::IDD, this);
	m_pDlgEnc->ModifyStyle(0, WS_CHILDWINDOW);
	m_pDlgEnc->MoveWindow(rcSub);
	m_pDlgEnc->ShowWindow(SW_HIDE);
	//
	m_pDlgImage = new CDeviceSettingDlgImage(this);
	m_pDlgImage->Create(CDeviceSettingDlgImage::IDD, this);
	m_pDlgImage->ModifyStyle(0, WS_CHILDWINDOW);
	m_pDlgImage->MoveWindow(rcSub);
	m_pDlgImage->ShowWindow(SW_HIDE);
	//
	m_pDlgNet = new CDeviceSettingDlgNet(this);
	m_pDlgNet->Create(CDeviceSettingDlgNet::IDD, this);
	m_pDlgNet->ModifyStyle(0, WS_CHILDWINDOW);
	m_pDlgNet->MoveWindow(rcSub);
	m_pDlgNet->ShowWindow(SW_HIDE);
	//
	m_pDlgOsd = new CDeviceSettingDlgOsd(this);
	m_pDlgOsd->Create(CDeviceSettingDlgOsd::IDD, this);
	m_pDlgOsd->ModifyStyle(0, WS_CHILDWINDOW);
	m_pDlgOsd->MoveWindow(rcSub);
	m_pDlgOsd->ShowWindow(SW_HIDE);
	//
	m_pDlgPlatform = new CDeviceSettingDlgPlatform(this);
	m_pDlgPlatform->Create(CDeviceSettingDlgPlatform::IDD, this);
	m_pDlgPlatform->ModifyStyle(0, WS_CHILDWINDOW);
	m_pDlgPlatform->MoveWindow(rcSub);
	m_pDlgPlatform->ShowWindow(SW_HIDE);
	//
	m_pDlgTime = new CDeviceSettingDlgTime(this);
	m_pDlgTime->Create(CDeviceSettingDlgTime::IDD, this);
	m_pDlgTime->ModifyStyle(0, WS_CHILDWINDOW);
	m_pDlgTime->MoveWindow(rcSub);
	m_pDlgTime->ShowWindow(SW_HIDE);
	//
	m_pDlgWifi = new CDeviceSettingDlgWifi(this);
	m_pDlgWifi->Create(CDeviceSettingDlgWifi::IDD, this);
	m_pDlgWifi->ModifyStyle(0, WS_CHILDWINDOW);
	m_pDlgWifi->MoveWindow(rcSub);
	m_pDlgWifi->ShowWindow(SW_HIDE);
	//
	ShowPage(0);
	//
	CenterWindow();
	//
	CRect rectClient;
	GetClientRect(rectClient);
	int nspace = 0;
	int mLeft = 0;
	int ntp = 42;
	//------------------------------------------------------------------------------------------------
	nspace = 0;
	m_close.SetWindowPos(NULL, rectClient.left + rectClient.Width() - nspace - theApp.m_nCloseWidth, 0, theApp.m_nCloseWidth, theApp.m_nCloseHeight, SWP_NOACTIVATE | SWP_NOZORDER);



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDeviceSettingDlg::ShowPage(int index)
{
	m_nSelItem = index;
	//
	m_pDlgEnc->ShowWindow(SW_HIDE);
	m_pDlgImage->ShowWindow(SW_HIDE);
	m_pDlgNet->ShowWindow(SW_HIDE);
	m_pDlgOsd->ShowWindow(SW_HIDE);
	m_pDlgPlatform->ShowWindow(SW_HIDE);
	m_pDlgTime->ShowWindow(SW_HIDE);
	m_pDlgWifi->ShowWindow(SW_HIDE);
	switch (index)
	{
	case 0:
	{
		m_pDlgEnc->ShowWindow(SW_SHOW);
	}
	break;
	case 1:
	{
		m_pDlgImage->ShowWindow(SW_SHOW);
	}
	break;
	case 2:
	{
		m_pDlgNet->ShowWindow(SW_SHOW);
	}
	break;
	case 3:
	{
		m_pDlgWifi->ShowWindow(SW_SHOW);
	}
	break;
	case 4:
	{
		m_pDlgOsd->ShowWindow(SW_SHOW);
	}
	break;
	case 5:
	{
		m_pDlgTime->ShowWindow(SW_SHOW);
	}
	break;
	case 6:
	{
		m_pDlgPlatform->ShowWindow(SW_SHOW);
	}
	break;
	default:
		break;
	}
}

void CDeviceSettingDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	int i  = m_tabParam.GetCurSel();
	ShowPage(i);
	*pResult = 0;
}


void CDeviceSettingDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	//ShowWindow(SW_HIDE);
	DestroyWindow();
}


void CDeviceSettingDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}


void CDeviceSettingDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	OnPaintBmpBackground(this, dc);
}


LRESULT CDeviceSettingDlg::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SendMessage(WM_SYSCOMMAND, 0xF012, 0);
	return CDialogEx::OnNcHitTest(point);
}

#include "a_signaling_channel_json_reader_builder.h"
void CDeviceSettingDlg::OnBnClickedButton1()
{
	//获取
	char szTmp[4096] = { 0 };
	int len = 0;
	char szDeviceId[256] = {0};

	if (theApp.m_ClientType == X_CLIENT_XXX_TYPE_CLASSIC)
	{
		sprintf(szDeviceId, "%d", m_st.classicinfo.dev_id);
	}

	char szuuid[64] = { 0 };
	string s = JsonBuildParamInfoGet(szDeviceId,szuuid);
	//
	theApp.m_pClientInferface->SignalingCtrl(szDeviceId, s.c_str(),s.length(), szTmp, len);
	//
	if (len < 0)
	{
		CMyMessageBox XXXMessageBox;
		Get_String_From_Language_Ini_File(_T("str_dlg_param_get_error"), XXXMessageBox.m_strMsg);
		XXXMessageBox.DoModal();
		return;
	}
	//
	//更新各个子窗口数据
	string command;
	int result;
	string deviceuuid;
	string msguuid;
	//
	int mainbitrate;
	int mainframetate;
	int mainimagesize;
	int subbitrate;
	int subimagesize;
	//
	int staturtion;
	int brightness;
	int contrast;
	int sharpness;
	int hue;
	int flip;
	//
	int dhcp;
	string wiredip;
	//
	string ssid;
	string wifipwd;
	//
	int timezone;
	string ntpserver;
	//
	string osdtitle;
	//
	string platip;
	int platport;
	string platid;



	BOOL bOk = JsonGetAllParams(szTmp, command, result,deviceuuid, msguuid,
		mainbitrate, mainframetate, mainimagesize,
		subbitrate, subimagesize,
		staturtion, brightness, contrast, sharpness, hue, flip,
		dhcp, wiredip,
		ssid, wifipwd,
		timezone, ntpserver,
		osdtitle,
		platip, platport, platid);


	if (!bOk)
	{
		CMyMessageBox XXXMessageBox;
		Get_String_From_Language_Ini_File(_T("str_dlg_param_get_error"), XXXMessageBox.m_strMsg);
		XXXMessageBox.DoModal();
		return;
	}

	//
	m_pDlgEnc->SetParam(mainbitrate, mainframetate, mainimagesize, subbitrate, subimagesize);
	m_pDlgImage->SetParam( brightness, contrast, staturtion,sharpness, hue, flip);
	//
	m_pDlgOsd->SetParam(osdtitle);
	m_pDlgNet->SetParam(dhcp, wiredip);
	m_pDlgWifi->SetParam(ssid, wifipwd);
	m_pDlgPlatform->SetParam(platip,platid);
	m_pDlgTime->SetParam(timezone, ntpserver);
}
