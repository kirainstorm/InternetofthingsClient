// DeviceSettingDlgWifi.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "DeviceSettingDlgWifi.h"
#include "afxdialogex.h"
#include "a_signaling_channel_json_reader_builder.h"
#include "DeviceSettingDlg.h"

// CDeviceSettingDlgWifi dialog

IMPLEMENT_DYNAMIC(CDeviceSettingDlgWifi, CDialogEx)

CDeviceSettingDlgWifi::CDeviceSettingDlgWifi(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeviceSettingDlgWifi::IDD, pParent)
	, m_wifi_ssid(_T(""))
	, m_wifi_pwd(_T(""))
{
	m_pParent = pParent;
}

CDeviceSettingDlgWifi::~CDeviceSettingDlgWifi()
{
}

void CDeviceSettingDlgWifi::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT8, m_wifi_ssid);
	DDX_Text(pDX, IDC_EDIT9, m_wifi_pwd);
}


BEGIN_MESSAGE_MAP(CDeviceSettingDlgWifi, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON11, &CDeviceSettingDlgWifi::OnBnClickedButton11)
END_MESSAGE_MAP()


// CDeviceSettingDlgWifi message handlers


BOOL CDeviceSettingDlgWifi::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nKey = (int)pMsg->wParam;
	if (VK_RETURN == nKey)
	{
		return TRUE;
	}
	if (VK_ESCAPE == nKey){
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDeviceSettingDlgWifi::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}


BOOL CDeviceSettingDlgWifi::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CDeviceSettingDlgWifi::SetParam(string v1, string v2)
{
	m_wifi_ssid = __stringToCString(v1);
	m_wifi_pwd = __stringToCString(v2);
	UpdateData(FALSE);
}

void CDeviceSettingDlgWifi::OnBnClickedButton11()
{
	UpdateData(TRUE);

	if (m_wifi_ssid.IsEmpty() || m_wifi_pwd.IsEmpty())
	{
		CMyMessageBox XXXMessageBox;
		Get_String_From_Language_Ini_File(_T("str_dlg_180517_msg1"), XXXMessageBox.m_strMsg);
		XXXMessageBox.DoModal();
		return;
	}
	



	CDeviceSettingDlg * p = (CDeviceSettingDlg *)m_pParent;
	//
	char szTmp[4096] = { 0 };
	int len = 0;
	char szDeviceId[256] = { 0 };
	//
	if (theApp.m_ClientType == X_CLIENT_XXX_TYPE_CLASSIC)
	{
		sprintf(szDeviceId, "%d", p->m_st.classicinfo.dev_id);
	}
	//
	char szuuid[64] = { 0 };
	string s = JsonBuildSetWifiNetwork(szDeviceId, szuuid, (char *)__cstringToString(m_wifi_ssid).c_str(), (char *)__cstringToString(m_wifi_pwd).c_str());
	//
	theApp.m_pClientInferface->SignalingCtrl(szDeviceId, s.c_str(), s.length(), szTmp, len);
	//
	if (len < 0)
	{
		CMyMessageBox XXXMessageBox;
		Get_String_From_Language_Ini_File(_T("str_dlg_param_set_error"), XXXMessageBox.m_strMsg);
		XXXMessageBox.DoModal();
		return;
	}
}