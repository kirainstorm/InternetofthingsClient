// HisLocalplayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "HisLocalplayDlg.h"
#include "afxdialogex.h"


// CHisLocalplayDlg dialog

IMPLEMENT_DYNAMIC(CHisLocalplayDlg, CDialogEx)

CHisLocalplayDlg::CHisLocalplayDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHisLocalplayDlg::IDD, pParent)
{

}

CHisLocalplayDlg::~CHisLocalplayDlg()
{
}

void CHisLocalplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_1, m_title);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_BUTTON1, m_close);
}


BEGIN_MESSAGE_MAP(CHisLocalplayDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CHisLocalplayDlg::OnBnClickedButton1)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_LBN_DBLCLK(IDC_LIST1, &CHisLocalplayDlg::OnLbnDblclkList1)
END_MESSAGE_MAP()


// CHisLocalplayDlg message handlers




BOOL CHisLocalplayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	ButtonLoadBitmap(m_close, L"title_bar/close_1.png", L"title_bar/close_2.png", L"title_bar/close_3.png", L"title_bar/close_4.png");
	CRect rectClient;
	GetClientRect(rectClient);
	m_close.SetWindowPos(NULL, rectClient.left + rectClient.Width() - 0 - theApp.m_nCloseWidth, 0, theApp.m_nCloseWidth, theApp.m_nCloseHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	//
	CenterWindow();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CHisLocalplayDlg::PreTranslateMessage(MSG* pMsg)
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


void CHisLocalplayDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	OnPaintBmpBackground(this, dc);
}
void CHisLocalplayDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}
void CHisLocalplayDlg::OnBnClickedButton1()
{
	//CDialogEx::OnOK();
	ShowWindow(SW_HIDE);
	DestroyWindow();
}
LRESULT CHisLocalplayDlg::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(rect);
	rect.bottom = rect.top + theApp.m_NcHitHeight;

	if (rect.PtInRect(point))
		return HTCAPTION;
	return CDialogEx::OnNcHitTest(point);
}
void CHisLocalplayDlg::ClearList()
{
// 	int nCount = m_list.GetCount();
// 	for (int i = 0; i < nCount; i++)
// 	{
// 		HIS_LOG_T* pInfo = (HIS_LOG_T*)m_list.GetItemData(i);
// 		delete pInfo;
// 	}
// 	m_list.ResetContent();
// 	m_list.Invalidate();
}

void CHisLocalplayDlg::SearchHislog_Local()
{
// 	ClearList();
// 
// 	stDevice st;
// 	CDeviceManager::Instance().GetDeviceByIndex(m_cblist.GetItemData(m_cblist.GetCurSel()), st);
// 
// 
// 	char szPath[MAX_PATH] = { 0 };
// 	sprintf_s(szPath, "%s\\Mp4Files\\%d",
// 		theApp.m_clientSetting.recPath,
// 		st.SvrDevInfo.dev_id
// 		);
// 	//
// 	WCHAR wszTempPath[256];
// 	MultiByteToWideChar(CP_UTF8, 0, szPath, strlen(szPath) + 1, wszTempPath, sizeof(wszTempPath) / sizeof(wszTempPath[0]));
// 	CString strTempPath = wszTempPath;
// 	get_files(strTempPath);
// 	//排序
// 	if (dirList.size() > 0)
// 	{
// 		dirList.sort();
// 	}
// 	//
// 	list<Directory_Info_t>::iterator _itor;
// 	for (_itor = dirList.begin(); _itor != dirList.end(); ++_itor)
// 	{
// 		//
// 		CString strname = (*_itor).strPath;
// 		wchar_t ss1 = '\\';
// 		strname = strname.Right(strname.GetLength() - strname.ReverseFind(ss1) - 1);
// 
// 
// 		//文件大小
// 		CFile cfile;
// 		int size = 0;
// 		if (cfile.Open((*_itor).strPath, CFile::modeRead))
// 		{
// 			size = (int)cfile.GetLength();
// 			cfile.Close();
// 		}
// 		int k = size / 1024 / 1024;
// 		if (k == 0)
// 		{
// 			k = 1;
// 		}
// 		CString strFileSize;
// 		strFileSize.Format(L"    [ %dMB ]", k);
// 
// 		strname += strFileSize;
// 		//
// 		int nCount = m_list.GetCount();
// 		m_list.InsertString(nCount, strname);
// 
// 		//
// 		HIS_LOG_T* pInfo = new HIS_LOG_T;
// 		ZeroMemory(pInfo, 0);
// 		WideCharToMultiByte(CP_ACP, NULL, (*_itor).strPath, -1, pInfo->store_file, sizeof(pInfo->store_file), NULL, FALSE);
// 		m_list.SetItemData(nCount, (DWORD_PTR)pInfo);
// 		//
// 	}
// 
// 	dirList.clear();
}

void CHisLocalplayDlg::OnLbnDblclkList1()
{
	// TODO: Add your control notification handler code here

// 	int i = m_list.GetCurSel();
// 	if (i == -1)
// 	{
// 		return;
// 	}
// 
// // 	if ((1 == m_cbSearchType.GetCurSel()) || (2 == m_cbSearchType.GetCurSel()))
// // 	{
// 		HIS_LOG_T* pInfo = (HIS_LOG_T*)m_list.GetItemData(i);
// 		CString str(pInfo->store_file);
// 		ShellExecute(NULL, _T("open"), str, NULL, NULL, SW_SHOWNORMAL);
}




// void CHislogSearchPan::SearchHislog_Downlaod()
// {
// 	ClearList();
// 
// 	stDevice st;
// 	CDeviceManager::Instance().GetDeviceByIndex(m_cblist.GetItemData(m_cblist.GetCurSel()), st);
// 
// 
// 	char szPath[MAX_PATH] = { 0 };
// 	sprintf_s(szPath, "%s\\Mp4Files\\%d\\Download",
// 		theApp.m_clientSetting.recPath,
// 		st.SvrDevInfo.dev_id
// 		);
// 	//
// 	WCHAR wszTempPath[256];
// 	MultiByteToWideChar(CP_UTF8, 0, szPath, strlen(szPath) + 1, wszTempPath, sizeof(wszTempPath) / sizeof(wszTempPath[0]));
// 	CString strTempPath = wszTempPath;
// 	get_files(strTempPath);
// 	//排序
// 	if (dirList.size() > 0)
// 	{
// 		dirList.sort();
// 	}
// 	//
// 	list<Directory_Info_t>::iterator _itor;
// 	for (_itor = dirList.begin(); _itor != dirList.end(); ++_itor)
// 	{
// 		//
// 		CString strname = (*_itor).strPath;
// 		wchar_t ss1 = '\\';
// 		strname = strname.Right(strname.GetLength() - strname.ReverseFind(ss1) - 1);
// 
// 
// 		//文件大小
// 		CFile cfile;
// 		int size = 0;
// 		if (cfile.Open((*_itor).strPath, CFile::modeRead))
// 		{
// 			size = (int)cfile.GetLength();
// 			cfile.Close();
// 		}
// 		int k = size / 1024 / 1024;
// 		if (k == 0)
// 		{
// 			k = 1;
// 		}
// 		CString strFileSize;
// 		strFileSize.Format(L"    [ %dMB ]", k);
// 
// 		strname += strFileSize;
// 		//
// 		int nCount = m_list.GetCount();
// 		m_list.InsertString(nCount, strname);
// 
// 		//
// 		HIS_LOG_T* pInfo = new HIS_LOG_T;
// 		ZeroMemory(pInfo, 0);
// 		WideCharToMultiByte(CP_ACP, NULL, (*_itor).strPath, -1, pInfo->store_file, sizeof(pInfo->store_file), NULL, FALSE);
// 		m_list.SetItemData(nCount, (DWORD_PTR)pInfo);
// 		//
// 	}
// 
// 	dirList.clear();
// }