// DeviceSettingDlgImage.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "DeviceSettingDlgImage.h"
#include "afxdialogex.h"
#include "a_signaling_channel_json_reader_builder.h"
#include "DeviceSettingDlg.h"


// CDeviceSettingDlgImage dialog

IMPLEMENT_DYNAMIC(CDeviceSettingDlgImage, CDialogEx)

CDeviceSettingDlgImage::CDeviceSettingDlgImage(CWnd* pParent /*=NULL*/)
: CDialogEx(CDeviceSettingDlgImage::IDD, pParent)
{
	m_pParent = pParent;
}

CDeviceSettingDlgImage::~CDeviceSettingDlgImage()
{
}

void CDeviceSettingDlgImage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_silder_liangdu);
	DDX_Control(pDX, IDC_SLIDER2, n_silder_duibidu);
	DDX_Control(pDX, IDC_SLIDER3, n_silder_baohedu);
	DDX_Control(pDX, IDC_SLIDER4, n_silder_ruidu);
	DDX_Control(pDX, IDC_SLIDER5, n_silder_secai);
	DDX_Control(pDX, IDC_COMBO2, m_ListEncMainFanzhuan);
}


BEGIN_MESSAGE_MAP(CDeviceSettingDlgImage, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDeviceSettingDlgImage::OnCbnSelchangeCombo2)
	//ON_WM_HSCROLL()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDeviceSettingDlgImage message handlers


BOOL CDeviceSettingDlgImage::PreTranslateMessage(MSG* pMsg)
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


BOOL CDeviceSettingDlgImage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_ListEncMainFanzhuan.AddString(L"正常");
	m_ListEncMainFanzhuan.AddString(L"左右翻转");
	m_ListEncMainFanzhuan.AddString(L"上下翻转");
	m_ListEncMainFanzhuan.AddString(L"左右上下翻转");


	m_silder_liangdu.SetRange(0, 100);
	m_silder_liangdu.SetPageSize(1);
	n_silder_duibidu.SetRange(0, 100);
	n_silder_duibidu.SetPageSize(1);
	n_silder_baohedu.SetRange(0, 100);
	n_silder_baohedu.SetPageSize(1);
	n_silder_ruidu.SetRange(0, 100);
	n_silder_ruidu.SetPageSize(1);
	n_silder_secai.SetRange(0, 30);
	n_silder_secai.SetPageSize(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDeviceSettingDlgImage::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}
void CDeviceSettingDlgImage::SetParam(int v1, int v2, int v3, int v4, int v5, int v6)
{
	m_silder_liangdu.SetPos(v1);
	n_silder_duibidu.SetPos(v2);
	n_silder_baohedu.SetPos(v3);
	n_silder_ruidu.SetPos(v4);
	n_silder_secai.SetPos(v5);

	m_ListEncMainFanzhuan.SetCurSel(v6);

}

void CDeviceSettingDlgImage::OnCbnSelchangeCombo2()
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
	string s = JsonBuildSetImageFlip(szDeviceId, szuuid, m_ListEncMainFanzhuan.GetCurSel());
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



void CDeviceSettingDlgImage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == GetDlgItem(IDC_SLIDER1))
	{
		TRACE("------ %d\n", nSBCode);
		switch (nSBCode)
		{
		case SB_PAGELEFT://影响鼠标点击一次滑块或用户按下PageUp和PageDown键，滑块移动的距离，由SetPageSize决定  
		case SB_PAGERIGHT:
		case SB_LEFT://用户按下Home键触发  
		case SB_RIGHT://用户按下End键触发  
		case SB_LINELEFT://用户按下↑↓←→键时触发  
		case SB_LINERIGHT://用户按下↑↓←→键时触发  
		case SB_THUMBTRACK://鼠标拖拽滑块时触发，此时nPos有效  
		{
			;
		}
		break;
		case SB_THUMBPOSITION://鼠标拖拽滑块并释放时触发，此时nPos有效  
			break;
		case SB_ENDSCROLL:
		{
			//设置亮度
			CDeviceSettingDlg * p = (CDeviceSettingDlg *)m_pParent;
			char szTmp[4096] = { 0 };
			int len = 0;
			char szDeviceId[256] = { 0 };
			if (theApp.m_ClientType == X_CLIENT_XXX_TYPE_CLASSIC)
			{
				sprintf(szDeviceId, "%d", p->m_st.classicinfo.dev_id);
			}
			//
			char szuuid[64] = { 0 };
			string s = JsonBuildSetImageBrightness(szDeviceId, szuuid, m_silder_liangdu.GetPos());
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
		default:
			break;
		}
	}
	if (pScrollBar == GetDlgItem(IDC_SLIDER2))
	{

		switch (nSBCode)
		{
		case SB_PAGELEFT://影响鼠标点击一次滑块或用户按下PageUp和PageDown键，滑块移动的距离，由SetPageSize决定  
		case SB_PAGERIGHT:
		case SB_LEFT://用户按下Home键触发  
		case SB_RIGHT://用户按下End键触发  
		case SB_LINELEFT://用户按下↑↓←→键时触发  
		case SB_LINERIGHT://用户按下↑↓←→键时触发  
		case SB_THUMBTRACK://鼠标拖拽滑块时触发，此时nPos有效  
		{
			;
		}
		break;
		case SB_THUMBPOSITION://鼠标拖拽滑块并释放时触发，此时nPos有效  
			break;
		case SB_ENDSCROLL:
		{
			//设置对比度
			//m_pDeviceInfo->param.img_con.contrast = n_silder_duibidu.GetPos();
			CDeviceSettingDlg * p = (CDeviceSettingDlg *)m_pParent;
			char szTmp[4096] = { 0 };
			int len = 0;
			char szDeviceId[256] = { 0 };
			if (theApp.m_ClientType == X_CLIENT_XXX_TYPE_CLASSIC)
			{
				sprintf(szDeviceId, "%d", p->m_st.classicinfo.dev_id);
			}
			//
			char szuuid[64] = { 0 };
			string s = JsonBuildSetImageContrast(szDeviceId, szuuid, n_silder_duibidu.GetPos());
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
		default:
			break;
		}
	}
	if (pScrollBar == GetDlgItem(IDC_SLIDER3))
	{

		switch (nSBCode)
		{
		case SB_PAGELEFT://影响鼠标点击一次滑块或用户按下PageUp和PageDown键，滑块移动的距离，由SetPageSize决定  
		case SB_PAGERIGHT:
		case SB_LEFT://用户按下Home键触发  
		case SB_RIGHT://用户按下End键触发  
		case SB_LINELEFT://用户按下↑↓←→键时触发  
		case SB_LINERIGHT://用户按下↑↓←→键时触发  
		case SB_THUMBTRACK://鼠标拖拽滑块时触发，此时nPos有效  
		{
			;
		}
		break;
		case SB_THUMBPOSITION://鼠标拖拽滑块并释放时触发，此时nPos有效  
			break;
		case SB_ENDSCROLL:
		{
			//设置饱和度
			//m_pDeviceInfo->param.img_sta.staturtion = n_silder_baohedu.GetPos();
			CDeviceSettingDlg * p = (CDeviceSettingDlg *)m_pParent;
			char szTmp[4096] = { 0 };
			int len = 0;
			char szDeviceId[256] = { 0 };
			if (theApp.m_ClientType == X_CLIENT_XXX_TYPE_CLASSIC)
			{
				sprintf(szDeviceId, "%d", p->m_st.classicinfo.dev_id);
			}
			//
			char szuuid[64] = { 0 };
			string s = JsonBuildSetImageStaturtion(szDeviceId, szuuid, n_silder_baohedu.GetPos());
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

		default:
			break;
		}
	}
	if (pScrollBar == GetDlgItem(IDC_SLIDER4))
	{
		switch (nSBCode)
		{
		case SB_PAGELEFT://影响鼠标点击一次滑块或用户按下PageUp和PageDown键，滑块移动的距离，由SetPageSize决定  
		case SB_PAGERIGHT:
		case SB_LEFT://用户按下Home键触发  
		case SB_RIGHT://用户按下End键触发  
		case SB_LINELEFT://用户按下↑↓←→键时触发  
		case SB_LINERIGHT://用户按下↑↓←→键时触发  
		case SB_THUMBTRACK://鼠标拖拽滑块时触发，此时nPos有效  
		{
			;
		}
		break;
		case SB_THUMBPOSITION://鼠标拖拽滑块并释放时触发，此时nPos有效  
			break;
		case SB_ENDSCROLL:
		{
			//锐度
			CDeviceSettingDlg * p = (CDeviceSettingDlg *)m_pParent;
			char szTmp[4096] = { 0 };
			int len = 0;
			char szDeviceId[256] = { 0 };
			if (theApp.m_ClientType == X_CLIENT_XXX_TYPE_CLASSIC)
			{
				sprintf(szDeviceId, "%d", p->m_st.classicinfo.dev_id);
			}
			//
			char szuuid[64] = { 0 };
			string s = JsonBuildSetImageSharpness(szDeviceId, szuuid, n_silder_ruidu.GetPos());
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

		default:
			break;
		}
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
