// DeviceSettingDlgEncode.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "DeviceSettingDlgEncode.h"
#include "afxdialogex.h"
#include "DeviceSettingDlg.h"
#include "a_signaling_channel_json_reader_builder.h"

// CDeviceSettingDlgEncode dialog

IMPLEMENT_DYNAMIC(CDeviceSettingDlgEncode, CDialogEx)

CDeviceSettingDlgEncode::CDeviceSettingDlgEncode(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeviceSettingDlgEncode::IDD, pParent)
{
	m_pParent = pParent;
}

CDeviceSettingDlgEncode::~CDeviceSettingDlgEncode()
{
}

void CDeviceSettingDlgEncode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ListEncMainBitrate);
	DDX_Control(pDX, IDC_COMBO4, m_ListEncMainFps);
	DDX_Control(pDX, IDC_COMBO2, m_ListEncMainSize);
	DDX_Control(pDX, IDC_COMBO3, m_ListEncSubBitrate);
	DDX_Control(pDX, IDC_COMBO7, m_ListEncSubSize);
}


BEGIN_MESSAGE_MAP(CDeviceSettingDlgEncode, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDeviceSettingDlgEncode::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CDeviceSettingDlgEncode::OnCbnSelchangeCombo4)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDeviceSettingDlgEncode::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDeviceSettingDlgEncode::OnCbnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO7, &CDeviceSettingDlgEncode::OnCbnSelchangeCombo7)
END_MESSAGE_MAP()


// CDeviceSettingDlgEncode message handlers


void CDeviceSettingDlgEncode::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}


BOOL CDeviceSettingDlgEncode::PreTranslateMessage(MSG* pMsg)
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

int ggg_fps[15] = { 256, 384, 512, 768, 1000, 1200, 1500, 2000, 2500, 3000, 4000, 5000, 6000, 7000, 8000 };
int ggg_fps_sub[12] = { 32, 64, 96, 128, 160, 192, 224, 256, 320, 384, 448, 512 };
BOOL CDeviceSettingDlgEncode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	for (int i = 0; i < 15; i++)
	{
		CString str;
		str.Format(L"%d", ggg_fps[i]);
		m_ListEncMainBitrate.AddString(str);
	}
	//
	for (int i = 0; i < 12; i++)
	{
		CString str;
		str.Format(L"%d", ggg_fps_sub[i]);
		m_ListEncSubBitrate.AddString(str);	
	}
	//
	m_ListEncSubSize.AddString(L"D1");
	m_ListEncSubSize.AddString(L"CIF");
	//



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDeviceSettingDlgEncode::SetParam(int v1, int v2, int v3, int v4, int v5)
{
	for (int i = 0; i < 15; i++)
	{
		if (ggg_fps[i] == v1)
		{
			m_ListEncMainBitrate.SetCurSel(i);
			break;
		}
	}

	m_ListEncMainFps.SetCurSel(25 - v2);
	m_ListEncMainSize.SetCurSel(v3);
	//
	for (int i = 0; i < 12; i++)
	{
		if (ggg_fps_sub[i] == v4)
		{
			m_ListEncSubBitrate.SetCurSel(i);
			break;
		}
	}
	m_ListEncSubSize.SetCurSel(v5);
}

void CDeviceSettingDlgEncode::OnCbnSelchangeCombo1()
{
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
	string s = JsonBuildSetMainBitRate(szDeviceId, szuuid, ggg_fps[m_ListEncMainBitrate.GetCurSel()]);
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


void CDeviceSettingDlgEncode::OnCbnSelchangeCombo4()
{
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
	string s = JsonBuildSetMainFrameRate(szDeviceId, szuuid, 25-m_ListEncMainFps.GetCurSel());
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


void CDeviceSettingDlgEncode::OnCbnSelchangeCombo2()
{
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
	string s = JsonBuildSetMainImageSize(szDeviceId, szuuid, m_ListEncMainSize.GetCurSel());
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












void CDeviceSettingDlgEncode::OnCbnSelchangeCombo3()
{
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
	string s = JsonBuildSetSubBitRate(szDeviceId, szuuid, ggg_fps_sub[m_ListEncSubBitrate.GetCurSel()]);
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


void CDeviceSettingDlgEncode::OnCbnSelchangeCombo7()
{
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
	string s = JsonBuildSetSubImageSize(szDeviceId, szuuid, m_ListEncSubSize.GetCurSel());
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
