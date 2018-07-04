// MyMessageBox.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "MyMessageBox.h"
#include "afxdialogex.h"

// MyMessageBox.cpp : implementation file

// CMyMessageBox dialog



IMPLEMENT_DYNAMIC(CMyMessageBox, CDialogEx)

CMyMessageBox::CMyMessageBox(CWnd* pParent /*=NULL*/)
: CDialogEx(CMyMessageBox::IDD, pParent)
{
//	m_bitmapBG = NULL;
}

CMyMessageBox::~CMyMessageBox()
{
// 	if (m_bitmapBG)
// 	{
// 		CBitmap bmp;
// 		bmp.Attach(m_bitmapBG);
// 		bmp.Detach();
// 		DeleteObject(m_bitmapBG);
// 	}
}

void CMyMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_close);
	DDX_Control(pDX, IDC_STATIC_MSG, m_MsgStatic);
}


BEGIN_MESSAGE_MAP(CMyMessageBox, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CMyMessageBox::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMyMessageBox message handlers


BOOL CMyMessageBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//img.Load(theApp.m_strSkinPath + L"bg/paper.png");
	//m_bitmapBG = img.Detach();

	ButtonLoadBitmap(m_close,
		L"title_bar/close_1.png",
		L"title_bar/close_2.png",
		L"title_bar/close_3.png",
		L"title_bar/close_4.png");


	m_MsgStatic.SetWindowText(m_strMsg);

	PostMessage(WM_SIZE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CMyMessageBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_KEYDOWN == pMsg->message)
	{
		UINT nKey = (int)pMsg->wParam;
		if (VK_RETURN == nKey){
			return TRUE;
		}

		if (VK_ESCAPE == nKey){
			//AfxGetMainWnd()->SendMessage(WM_KEYDOWN,VK_ESCAPE,0);
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CMyMessageBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CMyMessageBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	OnPaintBmpBackground(this, dc);
}


void CMyMessageBox::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if (m_close.GetSafeHwnd() == NULL)
	{
		return;
	}

	// 	CRect rectClient;
	// 	GetClientRect(rectClient);
	// 	m_close.SetWindowPos(NULL, rectClient.left + rectClient.Width() - m_nCloseWidth, 0, m_nCloseWidth, m_nCloseHeight, SWP_NOACTIVATE | SWP_NOZORDER);

}


void CMyMessageBox::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
