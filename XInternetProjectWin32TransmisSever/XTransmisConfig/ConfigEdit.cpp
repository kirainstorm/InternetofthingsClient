// ConfigEdit.cpp : implementation file
//

#include "stdafx.h"
#include "XTransmisConfig.h"
#include "ConfigEdit.h"
#include "afxdialogex.h"

#include "DBForConfig.h"
#include "ConfigFile.h"

// CConfigEdit dialog

IMPLEMENT_DYNAMIC(CConfigEdit, CDialogEx)

CConfigEdit::CConfigEdit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigEdit::IDD, pParent)
	, m_start1(8)
	, m_start2(10)
	, m_start3(12)
	, m_start4(14)
	, m_start5(16)
	, m_start6(18)
	, m_end1(10)
	, m_end2(12)
	, m_end3(14)
	, m_end4(16)
	, m_end5(18)
	, m_end6(21)
	, m_id(0)
{
	m_bMYENUM = CFG_ENUM_FOR_ADD;
}

CConfigEdit::~CConfigEdit()
{
}

void CConfigEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_list1);
	DDX_Control(pDX, IDC_COMBO2, m_list2);
	DDX_Control(pDX, IDC_COMBO3, m_list3);
	DDX_Control(pDX, IDC_COMBO4, m_list4);
	DDX_Control(pDX, IDC_COMBO5, m_list5);
	DDX_Control(pDX, IDC_COMBO6, m_list6);
	DDX_Text(pDX, IDC_EDIT1, m_start1);
	DDX_Text(pDX, IDC_EDIT3, m_start2);
	DDX_Text(pDX, IDC_EDIT5, m_start3);
	DDX_Text(pDX, IDC_EDIT7, m_start4);
	DDX_Text(pDX, IDC_EDIT9, m_start5);
	DDX_Text(pDX, IDC_EDIT11, m_start6);
	DDV_MinMaxInt(pDX, m_start1, 0, 24);
	DDV_MinMaxInt(pDX, m_start2, 0, 24);
	DDV_MinMaxInt(pDX, m_start3, 0, 24);
	DDV_MinMaxInt(pDX, m_start4, 0, 24);
	DDV_MinMaxInt(pDX, m_start5, 0, 24);
	DDV_MinMaxInt(pDX, m_start6, 0, 24);
	DDX_Text(pDX, IDC_EDIT2, m_end1);
	DDX_Text(pDX, IDC_EDIT4, m_end2);
	DDX_Text(pDX, IDC_EDIT6, m_end3);
	DDX_Text(pDX, IDC_EDIT8, m_end4);
	DDX_Text(pDX, IDC_EDIT10, m_end5);
	DDX_Text(pDX, IDC_EDIT12, m_end6);
	DDV_MinMaxInt(pDX, m_end1, 0, 24);
	DDV_MinMaxInt(pDX, m_end2, 0, 24);
	DDV_MinMaxInt(pDX, m_end3, 0, 24);
	DDV_MinMaxInt(pDX, m_end4, 0, 24);
	DDV_MinMaxInt(pDX, m_end5, 0, 24);
	DDV_MinMaxInt(pDX, m_end6, 0, 24);
	DDX_Text(pDX, IDC_EDIT13, m_id);
	DDV_MinMaxInt(pDX, m_id, 0, 400000000);
}


BEGIN_MESSAGE_MAP(CConfigEdit, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CConfigEdit::OnBnClickedButton1)
END_MESSAGE_MAP()


// CConfigEdit message handlers


BOOL CConfigEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CConfigEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here


	CDBForConfig::Instance().GetAllDevices();
	//
	m_list1.InsertString(0, L"请选择设备！");
	m_list2.InsertString(0, L"请选择设备！");
	m_list3.InsertString(0, L"请选择设备！");
	m_list4.InsertString(0, L"请选择设备！");
	m_list5.InsertString(0, L"请选择设备！");
	m_list6.InsertString(0, L"请选择设备！");
	m_list1.SetItemData(0, 0);
	m_list2.SetItemData(0, 0);
	m_list3.SetItemData(0, 0);
	m_list4.SetItemData(0, 0);
	m_list5.SetItemData(0, 0);
	m_list6.SetItemData(0, 0);
	//
	for (int i = 0; i < (int)CDBForConfig::Instance().m_stDevicves.size(); i++)
	{
		WCHAR wszInfo[256];
		MultiByteToWideChar(CP_ACP, 0, CDBForConfig::Instance().m_stDevicves[i].dev_name, strlen(CDBForConfig::Instance().m_stDevicves[i].dev_name) + 1,
			wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
	
		m_list1.InsertString(i + 1, wszInfo);
		m_list2.InsertString(i + 1, wszInfo);
		m_list3.InsertString(i + 1, wszInfo);
		m_list4.InsertString(i + 1, wszInfo);
		m_list5.InsertString(i + 1, wszInfo);
		m_list6.InsertString(i + 1, wszInfo);
		m_list1.SetItemData(i + 1, CDBForConfig::Instance().m_stDevicves[i].dev_id);
		m_list2.SetItemData(i + 1, CDBForConfig::Instance().m_stDevicves[i].dev_id);
		m_list3.SetItemData(i + 1, CDBForConfig::Instance().m_stDevicves[i].dev_id);
		m_list4.SetItemData(i + 1, CDBForConfig::Instance().m_stDevicves[i].dev_id);
		m_list5.SetItemData(i + 1, CDBForConfig::Instance().m_stDevicves[i].dev_id);
		m_list6.SetItemData(i + 1, CDBForConfig::Instance().m_stDevicves[i].dev_id);
	}
	//
	m_list1.SetCurSel(0);
	m_list2.SetCurSel(0);
	m_list3.SetCurSel(0);
	m_list4.SetCurSel(0);
	m_list5.SetCurSel(0);
	m_list6.SetCurSel(0);



	if ((CFG_ENUM_FOR_MODIFY == m_bMYENUM) || (CFG_ENUM_FOR_LOOK == m_bMYENUM))
	{

		//ST_LIVE_STRUCT
		//CDBForConfig::Instance().m_stDevicves

	
		{
			for (int i = 1; i < (int)CDBForConfig::Instance().m_stDevicves.size() + 1; i++)
			{
				//----------------------------------------------------------------------------------------------
				if (m_list1.GetItemData(i) == CConfigFile::Instance().m_stAllConfig.all[m_nIndex].dev[0].dev_id)
				{
					m_list1.SetCurSel(i);
				}
				//----------------------------------------------------------------------------------------------
				if (m_list2.GetItemData(i) == CConfigFile::Instance().m_stAllConfig.all[m_nIndex].dev[1].dev_id)
				{
					m_list2.SetCurSel(i);
				}
				//----------------------------------------------------------------------------------------------
				if (m_list3.GetItemData(i) == CConfigFile::Instance().m_stAllConfig.all[m_nIndex].dev[2].dev_id)
				{
					m_list3.SetCurSel(i);
				}
				//----------------------------------------------------------------------------------------------
				if (m_list4.GetItemData(i) == CConfigFile::Instance().m_stAllConfig.all[m_nIndex].dev[3].dev_id)
				{
					m_list4.SetCurSel(i);
				}
				//----------------------------------------------------------------------------------------------
				if (m_list5.GetItemData(i) == CConfigFile::Instance().m_stAllConfig.all[m_nIndex].dev[4].dev_id)
				{
					m_list5.SetCurSel(i);
				}
				//----------------------------------------------------------------------------------------------
				if (m_list6.GetItemData(i) == CConfigFile::Instance().m_stAllConfig.all[m_nIndex].dev[5].dev_id)
				{
					m_list6.SetCurSel(i);
				}
			}
		}

		//if (0 != CConfigFile::Instance().m_stAllConfig.all[m_nIndex].nPlatformID)
		{
			m_start1 = CConfigFile::Instance().m_stAllConfig.all[m_nIndex].dev[0].time_start;
			m_start2 = CConfigFile::Instance().m_stAllConfig.all[m_nIndex].dev[1].time_start;
			m_start3 = CConfigFile::Instance().m_stAllConfig.all[m_nIndex].dev[2].time_start;
			m_start4 = CConfigFile::Instance().m_stAllConfig.all[m_nIndex].dev[3].time_start;
			m_start5 = CConfigFile::Instance().m_stAllConfig.all[m_nIndex].dev[4].time_start;
			m_start6 = CConfigFile::Instance().m_stAllConfig.all[m_nIndex].dev[5].time_start;

			m_end1 = CConfigFile::Instance().m_stAllConfig.all[m_nIndex].dev[0].time_end;
			m_end2 = CConfigFile::Instance().m_stAllConfig.all[m_nIndex].dev[1].time_end;
			m_end3 = CConfigFile::Instance().m_stAllConfig.all[m_nIndex].dev[2].time_end;
			m_end4 = CConfigFile::Instance().m_stAllConfig.all[m_nIndex].dev[3].time_end;
			m_end5 = CConfigFile::Instance().m_stAllConfig.all[m_nIndex].dev[4].time_end;
			m_end6 = CConfigFile::Instance().m_stAllConfig.all[m_nIndex].dev[5].time_end;
		}



		m_id = CConfigFile::Instance().m_stAllConfig.all[m_nIndex].nPlatformID;

	}

	if (CFG_ENUM_FOR_MODIFY == m_bMYENUM)
	{
		SetWindowText(L"修改");
	}
	


	if (CFG_ENUM_FOR_LOOK == m_bMYENUM)
	{
		//CFG_ENUM_FOR_LOOK

		SetWindowText(L"浏览");

		m_list1.EnableWindow(FALSE);
		m_list2.EnableWindow(FALSE);
		m_list3.EnableWindow(FALSE);
		m_list4.EnableWindow(FALSE);
		m_list5.EnableWindow(FALSE);
		m_list6.EnableWindow(FALSE);

		((CEdit *)GetDlgItem(IDC_EDIT1))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDIT3))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDIT4))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDIT5))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDIT6))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDIT7))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDIT8))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDIT9))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDIT10))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDIT11))->EnableWindow(FALSE);
		((CEdit *)GetDlgItem(IDC_EDIT12))->EnableWindow(FALSE);


		((CEdit *)GetDlgItem(IDC_EDIT13))->EnableWindow(FALSE);


		((CButton *)GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE);
	}


	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CConfigEdit::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	if (!UpdateData())
	{
		return;
	}




	memset(&m_liveCfg, 0, sizeof(m_liveCfg));

	m_liveCfg.nPlatformID = m_id;

	m_liveCfg.dev[0].dev_id = m_list1.GetItemData(m_list1.GetCurSel());
	m_liveCfg.dev[1].dev_id = m_list2.GetItemData(m_list2.GetCurSel());
	m_liveCfg.dev[2].dev_id = m_list3.GetItemData(m_list3.GetCurSel());
	m_liveCfg.dev[3].dev_id = m_list4.GetItemData(m_list4.GetCurSel());
	m_liveCfg.dev[4].dev_id = m_list5.GetItemData(m_list5.GetCurSel());
	m_liveCfg.dev[5].dev_id = m_list6.GetItemData(m_list6.GetCurSel());


	m_liveCfg.dev[0].time_start = m_start1;
	m_liveCfg.dev[1].time_start = m_start2;
	m_liveCfg.dev[2].time_start = m_start3;
	m_liveCfg.dev[3].time_start = m_start4;
	m_liveCfg.dev[4].time_start = m_start5;
	m_liveCfg.dev[5].time_start = m_start6;


	m_liveCfg.dev[0].time_end = m_end1;
	m_liveCfg.dev[1].time_end = m_end2;
	m_liveCfg.dev[2].time_end = m_end3;
	m_liveCfg.dev[3].time_end = m_end4;
	m_liveCfg.dev[4].time_end = m_end5;
	m_liveCfg.dev[5].time_end = m_end6;


	if (CFG_ENUM_FOR_ADD == m_bMYENUM)
	{
		memcpy(&CConfigFile::Instance().m_stAllConfig.all[m_nIndex], &m_liveCfg, sizeof(m_liveCfg));
	}

	if (CFG_ENUM_FOR_MODIFY == m_bMYENUM)
	{
		memcpy(&CConfigFile::Instance().m_stAllConfig.all[m_nIndex], &m_liveCfg, sizeof(m_liveCfg));
	}

	CConfigFile::Instance().SaveConfigFile();


	CDialogEx::OnOK();
}
