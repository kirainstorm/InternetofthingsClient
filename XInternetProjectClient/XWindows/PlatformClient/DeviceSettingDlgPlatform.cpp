// DeviceSettingDlgPlatform.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "DeviceSettingDlgPlatform.h"
#include "afxdialogex.h"
#include "a_signaling_channel_json_reader_builder.h"
#include "DeviceSettingDlg.h"


// CDeviceSettingDlgPlatform dialog

IMPLEMENT_DYNAMIC(CDeviceSettingDlgPlatform, CDialogEx)

CDeviceSettingDlgPlatform::CDeviceSettingDlgPlatform(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeviceSettingDlgPlatform::IDD, pParent)
	, m_strPlat_ip(_T(""))
	, m_strPlat_id(_T(""))
{
	m_pParent = pParent;
}

CDeviceSettingDlgPlatform::~CDeviceSettingDlgPlatform()
{
}

void CDeviceSettingDlgPlatform::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strPlat_ip);
	DDX_Text(pDX, IDC_EDIT5, m_strPlat_id);
}


BEGIN_MESSAGE_MAP(CDeviceSettingDlgPlatform, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDeviceSettingDlgPlatform::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDeviceSettingDlgPlatform message handlers


BOOL CDeviceSettingDlgPlatform::PreTranslateMessage(MSG* pMsg)
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


BOOL CDeviceSettingDlgPlatform::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDeviceSettingDlgPlatform::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}



void CDeviceSettingDlgPlatform::SetParam(string v1, string v2)
{
	m_strPlat_ip = __stringToCString(v1);
	m_strPlat_id = __stringToCString(v2);
	UpdateData(FALSE);
}

void CDeviceSettingDlgPlatform::OnBnClickedButton1()
{
	UpdateData(TRUE);

	if (m_strPlat_ip.IsEmpty() || m_strPlat_id.IsEmpty())
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


		//
		char szuuid[64] = { 0 };
		string s = JsonBuildSetPlatInfo(szDeviceId, szuuid, (char *)__cstringToString(m_strPlat_ip).c_str(), 6501, (char *)__cstringToString(m_strPlat_id).c_str());
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

}