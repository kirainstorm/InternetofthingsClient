// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "PlatformClient.h"
extern CPlatClientApp theApp;
//
#define WM_LOGIN_OK_MSG (WM_USER+5651)
#define WM_LOGIN_ERROR_MSG (WM_USER+5652)
// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)


CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CLoginDlg::IDD, pParent)
{
	m_hLoginThread = NULL;//
	bCanLogin = TRUE;
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUser);
	DDX_Control(pDX, IDC_EDIT_USERPWD, m_editPwd);
	DDX_Control(pDX, IDC_EDIT_IP, m_editIP);
	DDX_Control(pDX, IDC_BUTTON1, m_close);
	DDX_Control(pDX, IDC_STATIC_99, m_title);
	DDX_Control(pDX, IDC_COMBO1, m_cbLg);
	DDX_Control(pDX, IDC_CHECK1_PWD, m_checkpwd);
	DDX_Control(pDX, ID_BTN_HELP, m_helpName);
	DDX_Control(pDX, IDCANCEL, m_btncancel);
	DDX_Control(pDX, IDOK, m_btnlogin);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CLoginDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CLoginDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CLoginDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CLoginDlg message handlers



BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	string s = "Œ“";


	ButtonLoadBitmap(m_close, L"title_bar/close_1.png", L"title_bar/close_2.png", L"title_bar/close_3.png", L"title_bar/close_4.png");
	ButtonLoadBitmap(m_helpName, L"help.png", L"help.png", L"help.png", L"help.png");

	//-------------------------------------------------------------------
	TCHAR	wszInfo[256];
	MultiByteToWideChar(CP_UTF8, 0, theApp.m_clientSetting.loginuser, strlen(theApp.m_clientSetting.loginuser) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
	m_editUser.SetWindowText(wszInfo);// administrator;
	m_checkpwd.SetCheck(theApp.m_clientSetting.enablpwd);
	if (0 != theApp.m_clientSetting.enablpwd)
	{
		MultiByteToWideChar(CP_UTF8, 0, theApp.m_clientSetting.loginpwd, strlen(theApp.m_clientSetting.loginpwd) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
		m_editPwd.SetWindowText(wszInfo);//123456
	}
	MultiByteToWideChar(CP_UTF8, 0, theApp.m_clientSetting.serverip, strlen(theApp.m_clientSetting.serverip) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));

#if 1
	m_editUser.SetWindowText(L"13800");
	m_editPwd.SetWindowText(L"13800");
	m_editIP.SetWindowText(L"47.96.249.142");
#endif

#if 0
	m_editUser.SetWindowText(L"1133");
	m_editPwd.SetWindowText(L"1133");
	m_editIP.SetWindowText(L"127.0.0.1");
#endif
	//-------------------------------------------------------------------
	m_cbLg.ResetContent();
	for (int k = 0; k < theApp.m_strlg_string.GetCount(); k++)
	{
		m_cbLg.AddString(theApp.m_strlg_string[k]);
	}
	m_cbLg.SetCurSel(theApp.m_clientSetting.lg);
	//
	InitFrameString();
	//
	PostMessage(WM_SIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLoginDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnCancel();
}


void CLoginDlg::InitFrameString()
{
	Set_String_To_Wnd_From_Language_Ini_File(IDC_STATIC_L_USER, _T("str_user_name"), this);
	Set_String_To_Wnd_From_Language_Ini_File(IDC_STATIC_L_PWD, _T("str_user_pwd"), this);
	Set_String_To_Wnd_From_Language_Ini_File(IDC_STATIC_L_LG, _T("str_login_select_lg"), this);

	//String_TO_Language_INI(IDC_CHECK_L_ENABLE, _T("str_server_need"), this);
	Set_String_To_Wnd_From_Language_Ini_File(IDC_STATIC_L_IP, _T("str_server"), this);
	//String_TO_Language_INI(IDC_STATIC_L_PORT, _T("str_port"), this);

	Set_String_To_Wnd_From_Language_Ini_File(IDOK, _T("str_dlg_login_170108_login"), this);
	Set_String_To_Wnd_From_Language_Ini_File(IDCANCEL, _T("str_dlg_login_170108_reg"), this);

	CString str;
	Get_String_From_Language_Ini_File(L"str_login_title", str);
	str = str + theApp.strCompany;
	SetWindowText(str);
	//
	m_title.SetWindowText(theApp.strCompany);
	//
	Get_String_From_Language_Ini_File(L"str_dlg_login_170108_helpname", str);
	m_helpName.SetTooltipText(str);

}



HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (
		(pWnd->GetDlgCtrlID() == IDC_STATIC_L_USER) ||
		(pWnd->GetDlgCtrlID() == IDC_STATIC_L_PWD) ||
		(pWnd->GetDlgCtrlID() == IDC_STATIC_L_LG) ||
		(pWnd->GetDlgCtrlID() == IDC_STATIC_L_IP)
		)
	{
		//pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	pDC->SetBkMode(TRANSPARENT);

	//return (HBRUSH)theApp.hbrushView;
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

LRESULT CLoginDlg::OnNcHitTest(CPoint point)
{
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(rect);
	rect.bottom = rect.top + theApp.m_NcHitHeight;

	if (rect.PtInRect(point))
		return HTCAPTION;

	return CDialogEx::OnNcHitTest(point);
}


BOOL CLoginDlg::OnEraseBkgnd(CDC* pDC)
{
	//return TRUE;
	// TODO: Add your message handler code here and/or call default
	return CDialogEx::OnEraseBkgnd(pDC);
}

void CLoginDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	OnPaintBmpBackground(this, dc);
}


void CLoginDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_close.GetSafeHwnd() == NULL){
		return;
	}
	if (theApp.m_nCloseWidth == 0){
		return;
	}
	CRect rc;
	GetClientRect(rc);
	int space = 1;
	m_close.SetWindowPos(NULL, rc.left + rc.Width() - theApp.m_nCloseWidth, 0, theApp.m_nCloseWidth, theApp.m_nCloseHeight, SWP_NOACTIVATE | SWP_NOZORDER);
}


void CLoginDlg::OnBnClickedButton1()
{
	CDialogEx::OnCancel();
}
BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_KEYDOWN == pMsg->message)
	{
		UINT nKey = (int)pMsg->wParam;
		if (VK_RETURN == nKey)
		{
			if (bCanLogin)
			{
				OnBnClickedOk();
				return TRUE;
			}
			return TRUE;
		}


		if (VK_ESCAPE == nKey){
			//AfxGetMainWnd()->SendMessage(WM_KEYDOWN,VK_ESCAPE,0);
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CLoginDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	theApp.m_clientSetting.lg = m_cbLg.GetCurSel();
	CSysFileGetSet::Instance().Set(theApp.m_clientSetting);
	theApp.InitLanguage();
	InitFrameString();
	Invalidate();
}

void _login_thread(void * param)
{
	CLoginDlg * p = (CLoginDlg *)param;
	p->_Login();
}

void CLoginDlg::_Login()
{
	CString strName, strPwd, strIP;
	m_editUser.GetWindowText(strName);
	m_editPwd.GetWindowText(strPwd);
	m_editIP.GetWindowText(strIP);
	//
	string sName, sPwd, sIP;
	sName = __cstringToString(strName);
	sPwd = __cstringToString(strPwd);
	sIP = __cstringToString(strIP);
	//
	theApp.m_strName = sName;
	theApp.m_strPwd = sPwd;
	theApp.m_strip = sIP;
	
	char *pRecvBuffer = new char[3 * 1024 * 1024];
	int nRecv = 0;

	do
	{
		if (theApp.m_pClientInferface)
		{
			theApp.m_pClientInferface->InitInstance(sIP.c_str(), sName.c_str(), sPwd.c_str(), theApp.m_pSignalingMessageCallback);
			theApp.m_pClientInferface->GetDevices(pRecvBuffer, nRecv);
			if (nRecv <= 0)
			{
				break;
			}


		}
		else
		{
			break;
		}

		if (X_CLIENT_XXX_TYPE_CLASSIC == theApp.m_ClientType)
		{
			for (int i = 0; i < (int)(nRecv / sizeof(ST_CLASSIC_DEVICE_INFO_BASE)); i++)
			{
				stDevice st;
				memset(&st, 0, sizeof(st));
				memcpy(&st.classicinfo, pRecvBuffer + i*sizeof(ST_CLASSIC_DEVICE_INFO_BASE), sizeof(ST_CLASSIC_DEVICE_INFO_BASE));
				CDeviceManager::Instance().AddDevice(st);
			}
		}
		if (X_CLIENT_XXX_TYPE_IOT == theApp.m_ClientType)
		{
			// 		for (int i = 0; i < (int)rs.size(); i++)
			// 		{
			// 			stDevice st;
			// 			memset(&st, 0, sizeof(st));
			// 			st.SvrDevInfo = rs[i];
			// 			CDeviceManager::Instance().AddDevice(st);
			// 		}
		}

// 
// 		if (theApp.m_pNetToServerInstance)
// 		{
// 			DestroyCmdInstance(theApp.m_pNetToServerInstance);
// 			theApp.m_pNetToServerInstance = nullptr;
// 		}
// 
// 		theApp.m_pNetToServerInstance = CreateCmdInstance(theApp.m_pMessageCallback, sIP.c_str(), sName.c_str(), sPwd.c_str());
// 
// 		char * pip = CmdInstanceGetIOTServerIP(theApp.m_pNetToServerInstance);
// 		if (nullptr == pip)
// 		{
// 			break;
// 		}
// 
// 		theApp.m_strip = pip;
// 		if (0 != CmdInstanceLogin(theApp.m_pNetToServerInstance))
// 		{
// 			break;
// 		}
// 
// 		CmdInstanceGetUserDevice(theApp.m_pNetToServerInstance);

		// 		if (0 != MediaInterfaceGetUserDevices(sIP.c_str(), 7600, sName.c_str(), sPwd.c_str(), rs))
		// 		//if (0 != CNetToServer::Instance().GetUserDevices("127.0.0.1", 7768, "admin11", "4444", rs))
		// 		{
		// 			break;
		// 		}
		// 
		// 		for (int i = 0; i < (int)rs.size(); i++)
		// 		{
		// 			stDevice st;
		// 			memset(&st, 0, sizeof(st));
		// 			st.SvrDevInfo = rs[i];
		// 			CDeviceManager::Instance().AddDevice(st);
		// 		}
		// 		Sleep(1);
		// 		//----------------------------------------------------
		theApp.m_clientSetting.enablpwd = m_checkpwd.GetCheck();
		memset(theApp.m_clientSetting.loginuser, 0, sizeof(theApp.m_clientSetting.loginuser));
		memset(theApp.m_clientSetting.loginpwd, 0, sizeof(theApp.m_clientSetting.loginpwd));
		memset(theApp.m_clientSetting.serverip, 0, sizeof(theApp.m_clientSetting.serverip));
		memcpy(theApp.m_clientSetting.loginuser, sName.c_str(), sName.length());
		memcpy(theApp.m_clientSetting.loginpwd, sPwd.c_str(), sPwd.length());
		memcpy(theApp.m_clientSetting.serverip, sIP.c_str(), sIP.length());
		CSysFileGetSet::Instance().Set(theApp.m_clientSetting);
		//----------------------------------------------------
		//
		PostMessage(WM_LOGIN_OK_MSG);///°∂°∂°∂°∂°∂°∂°∂°∂
		return;
	} while (0);
	//
	PostMessage(WM_LOGIN_ERROR_MSG);///°∂°∂°∂°∂°∂°∂°∂°∂
	//
	bCanLogin = TRUE;

}


void CLoginDlg::OnBnClickedOk()
{
	int k = 0;
	// 	k = NetToSmartLifeServer::Instance().UserRegister("127.0.0.1", 7768, "admin11", "4444");
	// 	k = NetToSmartLifeServer::Instance().UserChangePwd("127.0.0.1", 7768, "admin11", "4444", "4444");
	//	k = NetToSmartLifeServer::Instance().UserAddDevice("127.0.0.1", 7768, "admin11", "4444", "WQEQE");
	// 	k = NetToSmartLifeServer::Instance().UserDeleteDevice("127.0.0.1", 7768, "admin11", "4444", "WQEQE");
	// 	vector<ST_SREVER_DEVICE_INFO_BASE> rs;
	// 	k = CNetToServer::Instance().GetUserDevices("127.0.0.1", 7768, "admin11", "4444", rs);


	//int a = 0;

#if 1
	m_btncancel.EnableWindow(FALSE);
	m_btnlogin.EnableWindow(FALSE);
	m_close.EnableWindow(FALSE);

	m_checkpwd.EnableWindow(FALSE);
	m_editUser.EnableWindow(FALSE);
	m_editPwd.EnableWindow(FALSE);
	m_editIP.EnableWindow(FALSE);
	m_cbLg.EnableWindow(FALSE);

	if (m_hLoginThread)
	{
		WaitForSingleObject(m_hLoginThread, INFINITE);
		CloseHandle(m_hLoginThread);
		m_hLoginThread = NULL;
	}
	bCanLogin = FALSE;
	m_hLoginThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)_login_thread, this, NULL, NULL);
#endif




}

LRESULT CLoginDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_LOGIN_OK_MSG == message)
	{
		CDialogEx::OnOK();
	}
	if (WM_LOGIN_ERROR_MSG == message)
	{
		CMyMessageBox XXXMessageBox;
		Get_String_From_Language_Ini_File(_T("str_login_error"), XXXMessageBox.m_strMsg);
		XXXMessageBox.DoModal();

		m_btncancel.EnableWindow(TRUE);
		m_btnlogin.EnableWindow(TRUE);
		m_close.EnableWindow(TRUE);
		m_editUser.EnableWindow(TRUE);
		m_editPwd.EnableWindow(TRUE);
		m_editIP.EnableWindow(TRUE);
		m_cbLg.EnableWindow(TRUE);
		m_checkpwd.EnableWindow(TRUE);
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}