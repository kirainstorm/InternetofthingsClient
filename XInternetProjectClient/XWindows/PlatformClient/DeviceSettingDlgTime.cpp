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
	m_list_timezone.AddString(L"(UTC -11:00)协调世界时-11");
	m_list_timezone.AddString(L"(UTC -10:00) 夏威夷，阿留申群岛");
	m_list_timezone.AddString(L"(UTC -09:00) 阿拉斯加");
	m_list_timezone.AddString(L"(UTC -08:00) 太平洋时间(美国和加拿大),下加利福尼亚州");
	m_list_timezone.AddString(L"(UTC -07:00) 山地时间(美国和加拿大)，拉巴斯，马萨特兰，亚利桑那");
	m_list_timezone.AddString(L"(UTC -06:00) 中部时间(美国和加拿大), 中美洲，墨西哥城");
	m_list_timezone.AddString(L"(UTC -05:00) 东部时间(美国和加拿大), 利马, 波哥大，哈瓦那，海地，切图马尔，印第安纳州(东部)");
	m_list_timezone.AddString(L"(UTC -04:00) 大西洋时间(加拿大), 加拉加斯，库亚巴，乔治敦，圣地亚哥, 拉巴斯，亚松森");
	m_list_timezone.AddString(L"(UTC -03:30) 纽芬兰");
	m_list_timezone.AddString(L"(UTC -03:00) 阿拉瓜伊纳，巴西利亚, 布宜诺斯艾丽斯, 格陵兰");
	m_list_timezone.AddString(L"(UTC -02:00) 中大西洋，协调世界时-02");
	m_list_timezone.AddString(L"(UTC -01:00) 佛得角群岛，亚速尔群岛");
	m_list_timezone.AddString(L"(UTC) 格林威治平时;协调世界时，都柏林，爱丁堡，里斯本，伦敦，卡萨布兰卡，蒙罗维亚，雷克雅未克");
	m_list_timezone.AddString(L"(UTC +01:00) 阿姆斯特丹，柏林，伯尔尼，罗马，斯德哥尔摩，维也纳，贝尔格莱德，布达佩斯，布拉格，布鲁塞尔，哥本哈根，马德里，巴黎，萨拉热窝，华沙，萨格勒布，温得和克，中非西部");
	m_list_timezone.AddString(L"(UTC +02:00) 安曼，贝鲁特，大马士革，的黎波里，哈拉雷，赫尔辛基，加里宁格勒，加沙，开罗，雅典, 耶路撒冷, 开罗");
	m_list_timezone.AddString(L"(UTC +03:00) 巴格达，科威特，明斯克，圣彼得堡，伊斯塔布尔，内罗毕, 利雅得, 莫斯科");
	m_list_timezone.AddString(L"(UTC +03:30) 德黑兰");
	m_list_timezone.AddString(L"(UTC +04:00) 巴库, 第比利斯, 阿布扎比, 马斯喀特，阿斯特拉罕，埃里温，路易港，萨拉托夫，伊热夫斯克");
	m_list_timezone.AddString(L"(UTC +04:30) 喀布尔");
	m_list_timezone.AddString(L"(UTC +05:00) 阿什哈巴德，伊斯兰堡, 卡拉奇, 塔什干，叶卡捷琳堡");
	m_list_timezone.AddString(L"(UTC +05:30) 加尔各答, 孟买, 新德里，钦奈");
	m_list_timezone.AddString(L"(UTC +06:00) 仰光，鄂木斯克，达卡，阿拉木图, 阿斯塔纳");
	m_list_timezone.AddString(L"(UTC +07:00) 曼谷, 河内, 雅加达，新西伯利亚，托木斯克，克拉斯诺亚尔斯克，科布多");
	m_list_timezone.AddString(L"(UTC +08:00) 北京，重庆，香港，乌鲁木齐, 吉隆坡，新加坡, 台北，伊尔库茨克，乌兰巴托，柏斯");
	m_list_timezone.AddString(L"(UTC +09:00) 首尔, 雅库茨克, 大阪，札幌，东京，赤塔市");
	m_list_timezone.AddString(L"(UTC +09:30) 达尔文，阿德莱德");
	m_list_timezone.AddString(L"(UTC +10:00) 布里斯班，关岛,霍巴特 , 堪培拉,墨尔本, 悉尼, 莫尔兹比港, 符拉迪沃斯托克");
	m_list_timezone.AddString(L"(UTC +10:30) 豪勋爵岛");
	m_list_timezone.AddString(L"(UTC +11:00) 布干维尔岛，马加丹, 诺福克岛，乔库尔达赫，萨哈林，所罗门群岛, 新喀里多尼亚");
	m_list_timezone.AddString(L"(UTC +12:00) 阿纳德尔，奥克兰, 惠灵顿, 斐济");
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