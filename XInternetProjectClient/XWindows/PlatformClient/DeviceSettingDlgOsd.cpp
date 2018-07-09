// DeviceSettingDlgOsd.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "DeviceSettingDlgOsd.h"
#include "afxdialogex.h"
#include "a_signaling_channel_json_reader_builder.h"
#include "DeviceSettingDlg.h"

// CDeviceSettingDlgOsd dialog

IMPLEMENT_DYNAMIC(CDeviceSettingDlgOsd, CDialogEx)

CDeviceSettingDlgOsd::CDeviceSettingDlgOsd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeviceSettingDlgOsd::IDD, pParent)
	, m_osd_name(_T(""))
{
	m_pParent = pParent;
}

CDeviceSettingDlgOsd::~CDeviceSettingDlgOsd()
{
}

void CDeviceSettingDlgOsd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT6, m_osd_name);
}


BEGIN_MESSAGE_MAP(CDeviceSettingDlgOsd, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDeviceSettingDlgOsd::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDeviceSettingDlgOsd message handlers


BOOL CDeviceSettingDlgOsd::PreTranslateMessage(MSG* pMsg)
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


void CDeviceSettingDlgOsd::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}


BOOL CDeviceSettingDlgOsd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CDeviceSettingDlgOsd::SetParam(string osd)
{
	m_osd_name = __stringToCString(osd);
	UpdateData(FALSE);
}

void CDeviceSettingDlgOsd::OnBnClickedButton1()
{
	UpdateData(TRUE);
	if (m_osd_name.IsEmpty())
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
	string s = JsonBuildSetOsdTitle(szDeviceId, szuuid, (char *)__cstringToString(m_osd_name).c_str());
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