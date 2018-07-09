// SearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "SearchDlg.h"
#include "afxdialogex.h"
//#include "BroadcastSearch.h"
#define TIMER_ID 5121
#define TIMER_TIMEOUT (TIMER_ID+1)
#define WM_SEARCH_DEVICE (WM_USER+222)
// CSearchDlg dialog
//#include "SearchSettingDlg.h"

IMPLEMENT_DYNAMIC(CSearchDlg, CDialogEx)

CSearchDlg::CSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSearchDlg::IDD, pParent)
{

}

CSearchDlg::~CSearchDlg()
{
}

void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_BUTTON1, m_close);
}


BEGIN_MESSAGE_MAP(CSearchDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CSearchDlg::OnBnClickedButton1)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_TIMER()
	ON_LBN_DBLCLK(IDC_LIST1, &CSearchDlg::OnLbnDblclkList1)
END_MESSAGE_MAP()


// CSearchDlg message handlers
// 
// void __search_callback(const char *ip, const XSEARCH_MESSAGE_T  * msg, void * pUser)
// {
// 	CSearchDlg *p = (CSearchDlg *)pUser;
// 
// 
// 	st_device_info * pMsg = new st_device_info;
// 	memset(pMsg, 0, sizeof(st_device_info));
// 	memcpy(pMsg->ip, ip, strlen(ip));
// 	memcpy(&pMsg->info, msg, sizeof(XSEARCH_MESSAGE_T));
// 
// 
// 
// 
// 
// 	p->SendMessage(WM_SEARCH_DEVICE, 0, (LPARAM)pMsg);
// 
// 
// 
// 	return;
// }
BOOL CSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	img.Load(theApp.m_strSkinPath + L"bg/paper.png");
	m_bitmapBG = img.Detach();

	//---- 最小化/最大化/关闭 --------------------------------------------------
	img.Load(theApp.m_strSkinPath + L"title_bar/close_1.png");
	m_nCloseWidth = img.GetWidth();//<-------------------------------------------------
	m_nCloseHeight = img.GetHeight();//<-------------------------------------------------
	img.Destroy();

	ButtonLoadBitmap(m_close,
		L"title_bar/close_1.png",
		L"title_bar/close_2.png",
		L"title_bar/close_3.png",
		L"title_bar/close_4.png");





	CRect rectClient;
	GetClientRect(rectClient);
	int nspace = 0;
	m_close.SetWindowPos(NULL, rectClient.left + rectClient.Width() - nspace - m_nCloseWidth, 0, m_nCloseWidth, m_nCloseHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	m_list.SetWindowPos(NULL, 3, m_nCloseHeight + 5, rectClient.Width() - 3 * 2, rectClient.Height() - 5 * 2 - m_nCloseHeight, SWP_NOACTIVATE | SWP_NOZORDER);



	m_list._SetColor(239, 239, 239, 239, 239, 239, 0, 0, 0);
	m_list.Invalidate();


	m_bCanCheckTimeOut = TRUE;
// 	if (CBroadcastSearch::Instance().Initialization(__search_callback, this))
// 	{
// 		SetTimer(TIMER_ID, 500, NULL);
// 		SetTimer(TIMER_TIMEOUT, 1000, NULL);
// 	}
// 	else
// 	{
// 		AfxMessageBox(L"bind error!");
// 		PostMessage(WM_CLOSE);
// 	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CSearchDlg::PreTranslateMessage(MSG* pMsg)
{
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


void CSearchDlg::OnBnClickedButton1()
{
// 	KillTimer(TIMER_ID);
// 	KillTimer(TIMER_TIMEOUT);
// 	CBroadcastSearch::Instance().unInitialization();
// 
// 
// 	m_csList.Lock();
// 	for (int i = m_list.GetCount() - 1; i >= 0; i--)
// 	{
// 		st_device_info * pMsgTemp = (st_device_info *)m_list.GetItemData(i);
// 		delete pMsgTemp;
// 		m_list.DeleteString(i);
// 	}
// 	m_csList.Unlock();
// 
// 
// 
// 	m_list.ResetContent();
	CDialogEx::OnCancel();
}


void CSearchDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	OnPaintBmpBackground(this, dc);
}


LRESULT CSearchDlg::OnNcHitTest(CPoint point)
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


void CSearchDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
// 	if (nIDEvent == TIMER_ID)
// 	{
// 		CBroadcastSearch::Instance().Search();
// 	}
// 
// 
// 	if (nIDEvent == TIMER_TIMEOUT)
// 	{
// 		//设备在列表的时间太长了，5秒钟没有搜到了，可能不在网络内了，移除他
// 
// 		m_csList.Lock();
// 
// 		if (m_bCanCheckTimeOut)
// 		{
// 			for (int i = m_list.GetCount() - 1; i >= 0; i--)
// 			{
// 				st_device_info * pMsgTemp = (st_device_info *)m_list.GetItemData(i);
// 				int k = GetTickCount() - (pMsgTemp->tick);
// 				if (k > 1500)
// 				{
// 					delete pMsgTemp;
// 					m_list.DeleteString(i);
// 				}
// 			}
// 		}
// 		m_csList.Unlock();
// 
// 	}
	CDialogEx::OnTimer(nIDEvent);
}


LRESULT CSearchDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
// 
// 	if (WM_SEARCH_DEVICE == message)
// 	{
// 		m_csList.Lock();
// 
// 		BOOL bFind = FALSE;
// 		st_device_info * pMsg = (st_device_info *)lParam;
// 		for (int i = 0; i < m_list.GetCount(); i++)
// 		{
// 			st_device_info * pMsgTemp = (st_device_info *)m_list.GetItemData(i);
// 
// 
// 
// 			if ((0 == strcmp(pMsgTemp->info.uuid, pMsg->info.uuid)) && (0 != strcmp(pMsgTemp->ip, pMsg->ip)))
// 			{
// 				bFind = TRUE;
// 				pMsg->tick = GetTickCount();
// 
// 				char szTmp[1024] = { 0 };
// 				sprintf_s(szTmp, " [%s]    [%s]    [%s]", pMsg->ip, pMsg->info.mac_wired, pMsg->info.uuid);
// 
// 
// 				WCHAR wszInfo[256] = { 0 };
// 				MultiByteToWideChar(CP_ACP, 0, szTmp, strlen(szTmp), wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
// 
// 
// 				delete pMsgTemp;
// 				m_list.DeleteString(i);
// 
// 				m_list.InsertString(i, wszInfo);
// 				m_list.SetItemData(i, (DWORD_PTR)pMsg);
// 				break;
// 			}
// 
// 
// 			if (0 == strcmp(pMsgTemp->info.uuid, pMsg->info.uuid))
// 			{
// 				bFind = TRUE;
// 				pMsgTemp->tick = GetTickCount();
// 				delete pMsg;
// 				break;
// 			}
// 
// 
// 
// 		}
// 
// 		if (FALSE == bFind)
// 		{
// 			pMsg->tick = GetTickCount();
// 
// 			char szTmp[1024] = { 0 };
// 
// 			sprintf_s(szTmp, " [%s]    [%s]    [%s]", pMsg->ip, pMsg->info.mac_wired, pMsg->info.uuid);
// 
// 			WCHAR wszInfo[256] = { 0 };
// 			MultiByteToWideChar(CP_ACP, 0, szTmp, strlen(szTmp), wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
// 
// 			int kkk = m_list.GetCount();
// 			m_list.InsertString(kkk, wszInfo);
// 			m_list.SetItemData(kkk, (DWORD_PTR)pMsg);
// 		}
// 
// 		m_list.Invalidate();
// 		m_csList.Unlock();
// 	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CSearchDlg::OnLbnDblclkList1()
{
// 
// 	int k = m_list.GetCurSel();
// 
// 	if (-1 == k)
// 	{
// 		return;
// 	}
// 
// 
// 	//停止超时检测，因为现在正在配置设备信息，删除数据可能报错
// 	m_csList.Lock();
// 	m_bCanCheckTimeOut = FALSE;
// 	KillTimer(TIMER_TIMEOUT);
// 	m_csList.Unlock();
// 
// 
// 
// 
// 	st_device_info * pMsgTemp = (st_device_info *)m_list.GetItemData(k);
// 
// 	
// 	CSearchSettingDlg dlg;
// 	dlg.m_pDeviceInfo = pMsgTemp;
// 	dlg.DoModal();
// 
// 
// 
// 
// 	//重新启动检查
// 	m_csList.Lock();
// 	m_bCanCheckTimeOut = TRUE;
// 	SetTimer(TIMER_TIMEOUT, 1000, NULL);
// 	m_csList.Unlock();
}
