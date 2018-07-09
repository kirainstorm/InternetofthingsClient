// LockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "LockDlg.h"
#include "afxdialogex.h"


// CLockDlg dialog

IMPLEMENT_DYNAMIC(CLockDlg, CDialogEx)

CLockDlg::CLockDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLockDlg::IDD, pParent)
{

}

CLockDlg::~CLockDlg()
{
}

void CLockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLockDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CLockDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CLockDlg message handlers


void CLockDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


BOOL CLockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rc;
	AfxGetMainWnd()-> GetClientRect(rc);
	ClientToScreen(rc);

	CRect rcmove;
	rcmove.left = rc.left + rc.Width() - 300;
	rcmove.top = rc.top;
	rcmove.right = rcmove.left + 300;
	rcmove.bottom = rcmove.top + 180;

	MoveWindow(rcmove);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CLockDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
