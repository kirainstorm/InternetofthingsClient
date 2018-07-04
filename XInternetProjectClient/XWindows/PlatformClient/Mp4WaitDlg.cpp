// Mp4WaitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "Mp4WaitDlg.h"
#include "afxdialogex.h"
#include "HisOnlineplayDlg.h"
#define TIMER_4_WAIT (WM_USER + 7)

// CMp4WaitDlg dialog

IMPLEMENT_DYNAMIC(CMp4WaitDlg, CDialogEx)

CMp4WaitDlg::CMp4WaitDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMp4WaitDlg::IDD, pParent)
{
	//m_bClose = FALSE;
	
}

CMp4WaitDlg::~CMp4WaitDlg()
{
}

void CMp4WaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_1, m_info);
}


BEGIN_MESSAGE_MAP(CMp4WaitDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMp4WaitDlg message handlers

void CMp4WaitDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == TIMER_4_WAIT)
	{
#if 1
		if (m_bClose)
		{

			CDialogEx::OnOK();
		}
		CHisOnlineplayDlg *pWnd = (CHisOnlineplayDlg *)m_pMain;

		int m = pWnd->GetWiteMp4FileProgress();
		if (m == 100)
		{
			CDialogEx::OnOK();
		}

		CString str2;
		str2.Format(L"%d", m);
		CString strShow = strLoad + str2 + L"%";

		m_info.SetWindowText(strShow);

#endif



	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CMp4WaitDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetTimer(TIMER_4_WAIT, 500, NULL);

	Get_String_From_Language_Ini_File(_T("str_dlg_160802_msg7"), strLoad);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CMp4WaitDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (WM_KEYDOWN == pMsg->message)
	{
		UINT nKey = (int)pMsg->wParam;
		if (VK_RETURN == nKey){
			return TRUE;
		}

		if (VK_ESCAPE == nKey){
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}