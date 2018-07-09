// DeviceSettingDlgTime.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "DeviceSettingDlgTime.h"
#include "afxdialogex.h"
#include "a_signaling_channel_json_reader_builder.h"
#include "DeviceSettingDlg.h"

// CDeviceSettingDlgTime dialog

IMPLEMENT_DYNAMIC(CDeviceSettingDlgTime, CDialogEx)

CDeviceSettingDlgTime::CDeviceSettingDlgTime(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeviceSettingDlgTime::IDD, pParent)
	, m_time_svr(_T(""))
{
	m_pParent = pParent;
}

CDeviceSettingDlgTime::~CDeviceSettingDlgTime()
{
}

void CDeviceSettingDlgTime::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO6, m_list_timezone);
	DDX_Text(pDX, IDC_EDIT7, m_time_svr);
}


BEGIN_MESSAGE_MAP(CDeviceSettingDlgTime, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDeviceSettingDlgTime::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDeviceSettingDlgTime message handlers


BOOL CDeviceSettingDlgTime::PreTranslateMessage(MSG* pMsg)
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


void CDeviceSettingDlgTime::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}

int ggg_timezone[30] = { -39600, -36000, -32400, -28800, -25200, -21600, -18000, -14400, -12600, -10800, -7200, -3600, 0,
3600, 7200, 10800, 12600, 14400, 16200, 18000, 19800, 21600, 25200, 28800, 32400, 34200, 36000, 36800, 39600, 43200 };
BOOL CDeviceSettingDlgTime::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_list_timezone.AddString(L"(UTC -11:00)Э������ʱ-11");
	m_list_timezone.AddString(L"(UTC -10:00) �����ģ�������Ⱥ��");
	m_list_timezone.AddString(L"(UTC -09:00) ����˹��");
	m_list_timezone.AddString(L"(UTC -08:00) ̫ƽ��ʱ��(�����ͼ��ô�),�¼�����������");
	m_list_timezone.AddString(L"(UTC -07:00) ɽ��ʱ��(�����ͼ��ô�)������˹����������������ɣ��");
	m_list_timezone.AddString(L"(UTC -06:00) �в�ʱ��(�����ͼ��ô�), �����ޣ�ī�����");
	m_list_timezone.AddString(L"(UTC -05:00) ����ʱ��(�����ͼ��ô�), ����, ����󣬹����ǣ����أ���ͼ�����ӡ�ڰ�����(����)");
	m_list_timezone.AddString(L"(UTC -04:00) ������ʱ��(���ô�), ������˹�����ǰͣ����ζأ�ʥ���Ǹ�, ����˹������ɭ");
	m_list_timezone.AddString(L"(UTC -03:30) Ŧ����");
	m_list_timezone.AddString(L"(UTC -03:00) ���������ɣ���������, ����ŵ˹����˹, ������");
	m_list_timezone.AddString(L"(UTC -02:00) �д�����Э������ʱ-02");
	m_list_timezone.AddString(L"(UTC -01:00) ��ý�Ⱥ�������ٶ�Ⱥ��");
	m_list_timezone.AddString(L"(UTC) ��������ƽʱ;Э������ʱ�������֣�����������˹�����׶أ�����������������ά�ǣ��׿���δ��");
	m_list_timezone.AddString(L"(UTC +01:00) ��ķ˹�ص������֣������ᣬ����˹�¸��Ħ��άҲ�ɣ����������£�������˹�������񣬲�³�������籾�������������裬�������ѣ���ɳ�������ղ����µúͿˣ��з�����");
	m_list_timezone.AddString(L"(UTC +02:00) ��������³�أ�����ʿ����貨������ף��ն����������������գ���ɳ�����ޣ��ŵ�, Ү·����, ����");
	m_list_timezone.AddString(L"(UTC +03:00) �͸������أ���˹�ˣ�ʥ�˵ñ�����˹�����������ޱ�, ���ŵ�, Ī˹��");
	m_list_timezone.AddString(L"(UTC +03:30) �º���");
	m_list_timezone.AddString(L"(UTC +04:00) �Ϳ�, �ڱ���˹, ��������, ��˹���أ���˹�������������£�·�׸ۣ������з����ȷ�˹��");
	m_list_timezone.AddString(L"(UTC +04:30) ������");
	m_list_timezone.AddString(L"(UTC +05:00) ��ʲ���͵£���˹����, ������, ��ʲ�ɣ�Ҷ�����ձ�");
	m_list_timezone.AddString(L"(UTC +05:30) �Ӷ�����, ����, �µ������");
	m_list_timezone.AddString(L"(UTC +06:00) ���⣬��ľ˹�ˣ��￨������ľͼ, ��˹����");
	m_list_timezone.AddString(L"(UTC +07:00) ����, ����, �żӴ���������ǣ���ľ˹�ˣ�����˹ŵ�Ƕ�˹�ˣ��Ʋ���");
	m_list_timezone.AddString(L"(UTC +08:00) ���������죬��ۣ���³ľ��, ��¡�£��¼���, ̨����������Ŀˣ��������У���˹");
	m_list_timezone.AddString(L"(UTC +09:00) �׶�, �ſ�Ŀ�, ���棬���ϣ�������������");
	m_list_timezone.AddString(L"(UTC +09:30) ����ģ���������");
	m_list_timezone.AddString(L"(UTC +10:00) ����˹�࣬�ص�,������ , ������,ī����, Ϥ��, Ī���ȱȸ�, ��������˹�п�");
	m_list_timezone.AddString(L"(UTC +10:30) ��ѫ����");
	m_list_timezone.AddString(L"(UTC +11:00) ����ά��������ӵ�, ŵ���˵����ǿ����գ������֣�������Ⱥ��, �¿��������");
	m_list_timezone.AddString(L"(UTC +12:00) ���ɵ¶����¿���, �����, 쳼�");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CDeviceSettingDlgTime::SetParam(int v1, string v2)
{
	for (int i = 0; i < 30; i++)
	{
		if (ggg_timezone[i] == v1)
		{
			m_list_timezone.SetCurSel(i);
			break;
		}
	}

	m_time_svr = __stringToCString(v2);
	UpdateData(FALSE);
}



void CDeviceSettingDlgTime::OnBnClickedButton1()
{
	UpdateData(TRUE);

	if (m_time_svr.IsEmpty())
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
	string s = JsonBuildSetTimeZone(szDeviceId, szuuid, ggg_timezone[m_list_timezone.GetCurSel()]);
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






	s = JsonBuildSetNtpServer(szDeviceId, szuuid, (char *)__cstringToString(m_time_svr).c_str());
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