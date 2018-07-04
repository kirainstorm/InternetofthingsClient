// DeviceSettingDlgNet.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "DeviceSettingDlgNet.h"
#include "afxdialogex.h"
#include "a_signaling_channel_json_reader_builder.h"
#include "DeviceSettingDlg.h"

// CDeviceSettingDlgNet dialog

IMPLEMENT_DYNAMIC(CDeviceSettingDlgNet, CDialogEx)

CDeviceSettingDlgNet::CDeviceSettingDlgNet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeviceSettingDlgNet::IDD, pParent)
{
	m_pParent = pParent;
}

CDeviceSettingDlgNet::~CDeviceSettingDlgNet()
{
}

void CDeviceSettingDlgNet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_net_dhcp);
	DDX_Text(pDX, IDC_EDIT10, m_net_ip);
}


BEGIN_MESSAGE_MAP(CDeviceSettingDlgNet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON12, &CDeviceSettingDlgNet::OnBnClickedButton12)
END_MESSAGE_MAP()


// CDeviceSettingDlgNet message handlers


BOOL CDeviceSettingDlgNet::PreTranslateMessage(MSG* pMsg)
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


BOOL CDeviceSettingDlgNet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDeviceSettingDlgNet::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}

void CDeviceSettingDlgNet::SetParam(int v1, string v2)
{
	m_net_dhcp.SetCheck(v1);
	m_net_ip = __stringToCString(v2);
	UpdateData(FALSE);
}

void CDeviceSettingDlgNet::OnBnClickedButton12()
{
	UpdateData(TRUE);
	if (0 == m_net_dhcp.GetCheck())
	{
		if (m_net_ip.IsEmpty())
		{
			CMyMessageBox XXXMessageBox;
			Get_String_From_Language_Ini_File(_T("str_dlg_180517_msg1"), XXXMessageBox.m_strMsg);
			XXXMessageBox.DoModal();
			return;
		}
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
	string s = JsonBuildSetWiredNetwork(szDeviceId, szuuid, m_net_dhcp.GetCheck(), (char *)__cstringToString(m_net_ip).c_str());
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