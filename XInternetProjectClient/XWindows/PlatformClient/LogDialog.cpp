// LogDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "LogDialog.h"
#include "afxdialogex.h"


// CLogDialog dialog

IMPLEMENT_DYNAMIC(CLogDialog, CDialogEx)

CLogDialog::CLogDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogDialog::IDD, pParent)
{
}

CLogDialog::~CLogDialog()
{
}

void CLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
	DDX_Control(pDX, IDC_BUTTON1, m_close);
}


BEGIN_MESSAGE_MAP(CLogDialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CLogDialog::OnNMRClickList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CLogDialog::OnBnClickedButton1)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CLogDialog message handlers


void CLogDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	OnPaintBmpBackground(this, dc);
}


BOOL CLogDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//DWORD dwExStyle = m_listCtrl.GetExtendedStyle();
	//dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	//m_listCtrl.SetExtendedStyle(dwExStyle);

	DWORD dwExStyle = m_listCtrl.GetExtendedStyle();
	dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_OWNERDRAWFIXED | LVS_OWNERDATA;
	m_listCtrl.SetExtendedStyle(dwExStyle);



	m_listCtrl.InsertColumn(0, _T("设备ID"));
	m_listCtrl.SetColumnWidth(0, 130);
	m_listCtrl.InsertColumn(1, _T("标题"));
	m_listCtrl.SetColumnWidth(1, 100);
	m_listCtrl.InsertColumn(2, _T("内容"));
	m_listCtrl.SetColumnWidth(2, 210);
	m_listCtrl.InsertColumn(3, _T("接收时间"));
	m_listCtrl.SetColumnWidth(3, 140);

	for (int i = 0; i < 300; i++)
	{
		m_listCtrl.InsertItem(i, L"A2W(pInfo->szDevID)");
	}


	//SendMessage(WM_SIZE, 0, 0);
	ButtonLoadBitmap(m_close, L"title_bar/close_1.png", L"title_bar/close_2.png", L"title_bar/close_3.png", L"title_bar/close_4.png");
	CenterWindow();
	//
	CRect rectClient;
	GetClientRect(rectClient);
	int nspace = 0;
	int mLeft = 0;
	int ntp = 42;
	//------------------------------------------------------------------------------------------------
	nspace = 0;
	m_close.SetWindowPos(NULL, rectClient.left + rectClient.Width() - nspace - theApp.m_nCloseWidth, 0, theApp.m_nCloseWidth, theApp.m_nCloseHeight, SWP_NOACTIVATE | SWP_NOZORDER);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
// void CLogDialog::InitFrameString()
// {
// 	Set_String_To_Wnd_From_Language_Ini_File(IDC_STATIC_1, _T("str_dlg_log_msg1"), this);
// 	Set_String_To_Wnd_From_Language_Ini_File(IDC_STATIC_2, _T("str_dlg_log_msg2"), this);
// 	Set_String_To_Wnd_From_Language_Ini_File(IDC_STATIC_3, _T("str_dlg_log_msg3"), this);
// 	Set_String_To_Wnd_From_Language_Ini_File(IDC_STATIC_4, _T("str_dlg_log_msg4"), this);
// }




void CLogDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_listCtrl.GetSafeHwnd() == NULL)
	{
		return;
	}

// 	int nspace = 0;
	CRect rc;
	GetClientRect(rc);
	m_listCtrl.MoveWindow(rc);
// 	m_btnClose.SetWindowPos(NULL, rc.left + rc.Width() - nspace - m_nCloseWidth, 0, m_nCloseWidth, m_nCloseHeight, SWP_NOACTIVATE | SWP_NOZORDER);

}


// LRESULT CLogDialog::OnNcHitTest(CPoint point)
// {
// 	// TODO: Add your message handler code here and/or call default
// 	CRect rect;
// 	GetClientRect(rect);
// 	ClientToScreen(rect);
// 	rect.bottom = rect.top + theApp.m_NcHitHeight;
// 
// 	if (rect.PtInRect(point))
// 		return HTCAPTION;
// 
// 
// 	return CDialogEx::OnNcHitTest(point);
// }


HBRUSH CLogDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
// 	if ((pWnd->GetDlgCtrlID() == IDC_STATIC_1) ||
// 		(pWnd->GetDlgCtrlID() == IDC_STATIC_2) ||
// 		(pWnd->GetDlgCtrlID() == IDC_STATIC_3) ||
// 		(pWnd->GetDlgCtrlID() == IDC_STATIC_4))
// 	{
// 		//pDC->SetTextColor(RGB(255, 255, 255));
// 		pDC->SetBkMode(TRANSPARENT);
// 		return (HBRUSH)GetStockObject(NULL_BRUSH);
// 	}
	// TODO:  Return a different brush if the default is not desired

	return (HBRUSH)theApp.hViewTitleBrushBg;
	return hbr;
}


BOOL CLogDialog::PreTranslateMessage(MSG* pMsg)
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


void CLogDialog::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}


void CLogDialog::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CLogDialog::InsetListData()
{
	//m_listCtrl.InsertItem(0, L"A2W(pInfo->szDevID)");
	//m_listCtrl.SetItemText(0, 1, L"A2W(pInfo->szMessTitle)");
	//m_listCtrl.SetItemText(0, 2, L"A2W(DecodeFromUTF8_S(pInfo->szMessContent))");
	//m_listCtrl.SetItemText(0, 3, L"A2W(pInfo->szMessTime)");
}

void CLogDialog::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
}


LRESULT CLogDialog::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SendMessage(WM_SYSCOMMAND, 0xF012, 0);
	return CDialogEx::OnNcHitTest(point);
}
