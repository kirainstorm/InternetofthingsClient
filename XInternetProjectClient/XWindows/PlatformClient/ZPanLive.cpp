// DlgPanLive.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "ZPanLive.h"
#include "afxdialogex.h"
#include "PlatformClientDlg.h"
#include "PlatformClient.h"

extern CPlatClientApp theApp;
// CDlgPanLive dialog

IMPLEMENT_DYNAMIC(CZPanLive, CDialogEx)

CZPanLive::CZPanLive(CWnd* pParent /*=NULL*/)
	: CDialogEx(CZPanLive::IDD, pParent)
{
	m_parentDlg = pParent;
	m_bShowLeftPan = TRUE;
}

CZPanLive::~CZPanLive()
{
}

void CZPanLive::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Control(pDX, IDC_BUTTON1, m_btnshowhide);
}


BEGIN_MESSAGE_MAP(CZPanLive, CDialogEx)
	ON_WM_SIZE()
	ON_WM_NCDESTROY()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgPanLive message handlers

// CLeftPan message handlers
BOOL CZPanLive::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_tree.SetParentWnd(this);
	m_tree.ModifyStyle(0, TVS_NOHSCROLL);
	m_tree.Init();

	//////////////////////////////////////////////////////////////////////////
	CImage img;
	img.Load(theApp.m_strSkinPath + L"tool_bar_button/live_1.png");
	m_nLiveWidth = img.GetWidth();//<-------------------------------------------------
	m_nLiveHeight = img.GetHeight();//<-------------------------------------------------
	img.Destroy();
	//////////////////////////////////////////////////////////////////////////
	ButtonLoadBitmap(m_btnshowhide,
		L"tool_bar_button/more_1.png",
		L"tool_bar_button/more_2.png",
		L"tool_bar_button/more_3.png",
		L"tool_bar_button/more_4.png");


	m_pVideoDlg = new CVideoManageDlg;
	m_pVideoDlg->Create(CVideoManageDlg::IDD, this);
	m_pVideoDlg->InitPanlLiveWnd(this);
	m_pVideoDlg->ShowWindow(SW_SHOW);

	m_pPtzDlg = new CZPanPtzDlg;
	m_pPtzDlg->Create(CZPanPtzDlg::IDD, this);
	//m_pPtzDlg->InitPanlLiveWnd(this);
	m_pPtzDlg->ShowWindow(SW_SHOW);

// 	for (int i = 0; i < MAX_WINDOWS_NUM; i++)
// 	{
// 		ZeroMemory(&m_wData[i], sizeof(PLAY_WINDOW_DATA));
// 		m_wData[i].bPlay = FALSE;
// 		m_wData[i].hitem = NULL;
// 		m_pVideoDlg->XSetSubWindowData(i, &m_wData[i]);
// 
// 		m_pVideoDlg->XEnableSubWindowButtnOfIndex(i, FALSE);
// 	}
	CPlatClientDlg *pWnd = (CPlatClientDlg *)m_parentDlg;



	pWnd->m_pTopDlg->ShowLivePanViewNumber(theApp.m_clientSetting.spiltScreen);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CZPanLive::PreTranslateMessage(MSG* pMsg)
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
			CPlatClientDlg *pWnd = (CPlatClientDlg *)m_parentDlg;
			if (pWnd->m_fFullScreen)
			{
				pWnd->ShowWindowMax(FALSE);
			}
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
void CZPanLive::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	if (m_tree.GetSafeHwnd() == NULL)
	{
		return;
	}

	CPlatClientDlg *pWnd = (CPlatClientDlg *)m_pParentWnd;


	CRect rc_client;
	GetClientRect(rc_client);
	int show_btn_w = 48;
	int show_btn_h = 48;

	//
	m_tree.ShowWindow(SW_HIDE);

	int panMinWidth = theApp.m_PanMinWidth;//侧边栏宽度
	int panMaxWidth = theApp.m_PanMaxWidth;//侧边栏宽度


	int PtzHeight = 160;
	//
	//int zzspace = 11;
	//int ntreeleftex = -13;
	//int ntreewidthex = zzspace - ntreeleftex;//隐藏tree的垂直滚动条
	//
	if (m_bShowLeftPan)
	{
		m_btnshowhide.SetWindowPos(NULL,
			0,
			0,
			show_btn_w,
			show_btn_h, SWP_NOACTIVATE | SWP_NOZORDER);

		m_btnshowhide.ShowWindow(SW_HIDE);

// 		m_btnRefresh.SetWindowPos(NULL,
// 			rc_client.Width() - show_btn_w,
// 			0,
// 			show_btn_w,
// 			show_btn_h, SWP_NOACTIVATE | SWP_NOZORDER);
// 		m_btnRefresh.ShowWindow(SW_SHOW);
		//


		m_tree.ShowWindow(SW_SHOW);
		m_pPtzDlg->ShowWindow(SW_SHOW);


		m_pPtzDlg->SetWindowPos(NULL,
			1,
			1,
			panMaxWidth-1,
			PtzHeight, SWP_NOACTIVATE | SWP_NOZORDER);
		m_pPtzDlg->PostMessage(WM_SIZE);


		m_tree.SetWindowPos(NULL,
			1,
			PtzHeight + 2,
			panMaxWidth-1,
			rc_client.Height() - PtzHeight - 3, SWP_NOACTIVATE | SWP_NOZORDER);




		m_pVideoDlg->SetWindowPos(nullptr,
			rc_client.left + panMaxWidth,
			rc_client.top,
			rc_client.Width() - (m_bShowLeftPan ? panMaxWidth : panMinWidth),
			rc_client.Height(), SWP_NOACTIVATE | SWP_NOZORDER);




// 
// 		m_pHislogSearchPanDlg->SetWindowPos(NULL,
// 			0,
// 			show_btn_h,
// 			rc_client.Width(),
// 			rc_client.Height() - show_btn_h * 2, SWP_NOACTIVATE | SWP_NOZORDER);
// 		//
// 		if (m_emDlgShow == MY_DIALOG_SHOW_HIS)
// 		{
// 			m_pHislogSearchPanDlg->ShowWindow(SW_SHOW);
// 		}
// 		if (m_emDlgShow == MY_DIALOG_SHOW_PREVIEW)
// 		{
// 			m_tree.ShowWindow(SW_SHOW);
// 		}
// 
// 
// 		int m = rc_client.Width() / 5;
// 		int k = (m - show_btn_w) / 2;
// 		m_btnPreview.SetWindowPos(NULL, rc_client.left + k + m * 0, rc_client.top + (rc_client.Height() - show_btn_h), show_btn_w, show_btn_h, SWP_NOACTIVATE | SWP_NOZORDER);
// 		m_btnHis.SetWindowPos(NULL, rc_client.left + k + m * 1, rc_client.top + (rc_client.Height() - show_btn_h), show_btn_w, show_btn_h, SWP_NOACTIVATE | SWP_NOZORDER);
// 		m_btnFullScreen.SetWindowPos(NULL, rc_client.left + k + m * 2, rc_client.top + (rc_client.Height() - show_btn_h), show_btn_w, show_btn_h, SWP_NOACTIVATE | SWP_NOZORDER);

	}
	else
	{
		m_tree.ShowWindow(SW_HIDE);
		m_pPtzDlg->ShowWindow(SW_HIDE);
		m_pVideoDlg->MoveWindow(rc_client);



// 		m_btnRefresh.ShowWindow(SW_HIDE);

// 		m_btnshowhide.SetWindowPos(NULL,
// 			0,
// 			0,
// 			show_btn_w,
// 			show_btn_h, SWP_NOACTIVATE | SWP_NOZORDER);
// 		m_btnFullScreen.SetWindowPos(NULL, 0, rc_client.top + (rc_client.Height() - show_btn_h * 1), show_btn_w, show_btn_h, SWP_NOACTIVATE | SWP_NOZORDER);
// 		m_btnHis.SetWindowPos(NULL, 0, rc_client.top + (rc_client.Height() - show_btn_h * 2), show_btn_w, show_btn_h, SWP_NOACTIVATE | SWP_NOZORDER);
// 		m_btnPreview.SetWindowPos(NULL, 0, rc_client.top + (rc_client.Height() - show_btn_h * 3), show_btn_w, show_btn_h, SWP_NOACTIVATE | SWP_NOZORDER);

	}
}
void CZPanLive::OnNcDestroy()
{
	CDialogEx::OnNcDestroy();

	// TODO: Add your message handler code here
}
void CZPanLive::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	//m_tree.Deinit();
	delete this;
	CDialogEx::PostNcDestroy();
}
void CZPanLive::OnBnClickedButton1()
{
	//ShowHideLeftPan();
}
void CZPanLive::ShowHideLeftPan(BOOL _show)
{
	m_bShowLeftPan = _show;
}
// void CLeftPan::ResetCheck(emMyDialogShow type)
// {
// 
// 	CPlatClientDlg * pWnd = (CPlatClientDlg *)m_parentDlg;
// 
// 	for (int i = 0; i < MAX_WINDOWS_NUM; i++)
// 	{
// 		pWnd->m_pVideoDlg->m_pVideoWindows[i]->m_bPanShow = FALSE;
// 		pWnd->m_pVideoDlg->m_pVideoWindows[i]->m_pPanDlg->ShowWindow(SW_HIDE);
// 		if (pWnd->m_pVideoDlg->m_pVideoWindows[i]->m_pPtzDlg)
// 		{
// 			pWnd->m_pVideoDlg->m_pVideoWindows[i]->m_bShowPtzDlg = FALSE;
// 			pWnd->m_pVideoDlg->m_pVideoWindows[i]->m_pPtzDlg->ShowWindow(SW_HIDE);
// 		}
// 	}
// 	m_emDlgShow = type;
// 	m_btnPreview.EnableWindow(TRUE);
// 	m_btnHis.EnableWindow(TRUE);
// 	switch (type)
// 	{
// 	case MY_DIALOG_SHOW_PREVIEW:
// 		m_btnPreview.EnableWindow(FALSE);
// 		pWnd->m_pVideoDlg->ShowWindow(SW_SHOW);
// 		pWnd->m_pHisPlayer->ShowWindow(SW_HIDE);
// 		break;
// 	case MY_DIALOG_SHOW_HIS:
// 		m_btnHis.EnableWindow(FALSE);
// 		pWnd->m_pHisPlayer->ShowWindow(SW_SHOW);
// 		pWnd->m_pVideoDlg->ShowWindow(SW_HIDE);
// 		//m_pPlaybackDlg->RestDeviceList();
// 		break;
// 	default:
// 		break;
// 	}
// 	SendMessage(WM_SIZE, 0, 0);
// }
HBRUSH CZPanLive::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	return (HBRUSH)theApp.hbrushDlg;
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
// 
// void CLeftPan::OnBnClickedButton8()
// {
// 	//全屏
// 	CPlatClientDlg *pWnd = (CPlatClientDlg *)m_parentDlg;
// 	pWnd->ShowWindowMax();
// }
// void CLeftPan::OnBnClickedButton2()
// {
// 	ResetCheck(MY_DIALOG_SHOW_PREVIEW);
// }
// void CLeftPan::OnBnClickedButton3()
// {
// 	ResetCheck(MY_DIALOG_SHOW_HIS);
// }
// 
// //刷新列表
// void CLeftPan::OnBnClickedButton4()
// {
// #if 0
// 	//遍历设备，如果有正在播放的，则不刷新，提示用户。
// 	BOOL bFindOpen = FALSE;
// 	for (int k = 0; k < (int)CDeviceManager::Instance().m_DevList.size(); k++)
// 	{
// 		if (TRUE == CDeviceManager::Instance().m_DevList[k].bOpen)
// 		{
// 			bFindOpen = TRUE;
// 			break;
// 		}
// 	}
// 
// 	if (bFindOpen)
// 	{
// 		AfxMessageBox(L"请关闭所有正在播放的视频,然后重试");
// 		return;
// 	}
// 
// 
// 
// 
// 	vector<ST_SREVER_DEVICE_INFO_BASE> rs;
// 	if (0 != MediaInterfaceGetUserDevices(theApp.m_strip.c_str(), theApp.m_centerport, theApp.m_strName.c_str(), theApp.m_strPwd.c_str(), rs))
// 	{
// 		AfxMessageBox(L"刷新失败,请重试");
// 		return;
// 	}
// 
// 	m_tree.Deinit();
// 	CDeviceManager::Instance().ClearDevice();
// 
// 	for (int i = 0; i < (int)rs.size(); i++)
// 	{
// 		stDevice st;
// 		memset(&st, 0, sizeof(st));
// 		st.SvrDevInfo = rs[i];
// 		CDeviceManager::Instance().AddDevice(st);
// 	}
// 
// 	m_tree.UpdataTreeItem4Plat();
// #endif
// 
// }

LRESULT CZPanLive::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

// 	if (WM_VIDEO_WINDOW_CLOSE_STREAM == message)
// 	{
// 		int nIndex = (int)wParam;
// 		XViewCloseStream(nIndex);
// 	}
// 
// 	if (WM_VIDEO_WINDOW_CAPPIC == message)
// 	{
// 		int nIndex = (int)wParam;
// 		XViewCapturePictures(nIndex);
// 	}
// 
// 	if (WM_VIDEO_WINDOW_RECORD == message)
// 	{
// 		int nIndex = (int)wParam;
// 		int nStart = (int)lParam;
// 
// 		XViewRecord(nIndex, nStart);
// 	}
// 
// 	if (WM_VIDEO_WINDOW_SOUND == message)
// 	{
// 		int nIndex = (int)wParam;
// 		int nStart = (int)lParam;
// 
// 		XViewSound(nIndex, nStart);
// 	}
// 
// 	if (WM_VIDEO_WINDOW_TALK == message)
// 	{
// 		int nIndex = (int)wParam;
// 		int nStart = (int)lParam;
// 
// 		XViewTalk(nIndex, nStart);
// 	}

	// 	if (WM_VIDEO_WINDOW_VIDEOQUALITY == message)
	// 	{
	// 		int nIndex = (int)wParam;
	// 		int i = (int)lParam;
	// 
	// 
	// 		void *p = NULL;
	// 		m_pVideoDlg->XGetSubWindowData(nIndex, &p);
	// 		PLAY_WINDOW_DATA *pData = (PLAY_WINDOW_DATA *)p;
	// 
	// 
	// 		if (pData->pDevObject)
	// 		{
	// 			//pData->pDevObject->SetChannelParam(0, NULL, i, X_PARAM_TYPE_ENCODE);
	// 		}
	// 
	// 
	// 	}

	// 	if (WM_HIS_VIDEO_WINDOW_PAUSE == message)
	// 	{
	// 		int nIndex = (int)wParam;
	// 		int nStart = (int)lParam;
	// 
	// 		XHisViewPause(nIndex, nStart);
	// 	}
	// 
	// 	if (WM_HIS_VIDEO_WINDOW_STOP == message)
	// 	{
	// 		int nIndex = (int)wParam;
	// 		//int nStart = (int)lParam;
	// 
	// 		XHisViewStop(nIndex);
	// 	}
	// 
	// 
	// 	if (WM_HIS_VIDEO_WINDOW_POS_CALLBACK == message)
	// 	{
	// 		int nIndex = (int)wParam;
	// 		//int nStart = (int)lParam;
	// 
	// 		XHisViewPos(nIndex);
	// 	}
	// 	if (WM_HIS_VIDEO_WINDOW_SEEK == message)
	// 	{
	// 		int nIndex = (int)wParam;
	// 		int nStart = (int)lParam;
	// 
	// 		XHisViewSeek(nIndex, nStart);
	// 	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}


int CZPanLive::XViewOpenStream(stDevice m_stDevice, int nIndex)
{
	return m_pVideoDlg->m_pVideoWindows[nIndex]->XViewOpenStream(m_stDevice);
}

int CZPanLive::XViewCloseStream(int nIndex)
{
// 	if (nIndex < 0)
// 	{
// 		return 0;
// 	}
// 
// 	//m_csOpenRealtime.Lock();
// 
// 	void *p = NULL;
// 	m_pVideoDlg->XGetSubWindowData(nIndex, &p);
// 	PLAY_WINDOW_DATA *pData = (PLAY_WINDOW_DATA *)p;
// 
// 	if (pData->pDevObject)
// 	{
// 		pData->pDevObject->XDeviceLogout();
// 		pData->pDevObject->XDeviceDelete();
// 		pData->pDevObject = NULL;
// 	}
// 
// 	if (NULL != pData->hitem)
// 	{
// 		CDeviceManager::Instance().SetDeviceOpenStatusByDeviceID(pData->hitem, FALSE);
// 	}
// 
// 
// 
// 	m_pVideoDlg->XSetSubWindowTitleOfIndex(nIndex, "");
// 	m_pVideoDlg->XEnableSubWindowButtnOfIndex(nIndex, FALSE);
// 	m_tree.SetItemImage(pData->hitem, 2, 2);
// 	pData->hitem = NULL;
// 
// 	//m_csOpenRealtime.Unlock();
	return 0;

}


#if 0

int CDlgPanLive::OnMessageCallback(int nMessageType, int nMessageVal, void * pUser)
{

	PLAY_WINDOW_DATA * pItemData = (PLAY_WINDOW_DATA *)pUser;

	//pItemData->winIndex
	//m_pVideoDlg->SetTitleOfIndex(pItemData->winIndex, pItemData->dev_name);

	// 
	// 	if (nMessageType == XMSG_NET_STATUS)
	// 	{
	// 		char szString[256] = { 0 };
	// 		char szKey[256] = { 0 };
	// 		CString str;
	// 		//sprintf_s(szString, "%s ()", pItemData->dev_name);
	// 
	// 
	// 		switch (nMessage)
	// 		{
	// 		case XMSG_NET_STATUS____CONNECT_SUCCESS:
	// 			m_pVideoDlg->XSetSubWindowTitleOfIndex(pItemData->winIndex, pItemData->dev_name);
	// 			// 
	// 			// 			//pItemData->m_csPlay.Lock();
	// 			// 			
	// 			// 			if (pItemData->pPlayObject != NULL)
	// 			// 			{
	// 			// 				pItemData->pPlayObject->Close();
	// 			// 				pItemData->pPlayObject->Delete();
	// 			// 				pItemData->pPlayObject = NULL;
	// 			// 			}
	// 			// 
	// 			// 			if (pItemData->pPlayObject == NULL)
	// 			// 			{
	// 			// 				{
	// 			// 					// 				if (theApp.m_cfg.server_enable)
	// 			// 					// 				{
	// 			// 					// 					pItemData->pPlayObject = pItemData->pDevObject->CreatePlayObject(t.dev_id);
	// 			// 					// 				}
	// 			// 					// 				else
	// 			// 					{
	// 			// 						pItemData->pPlayObject = pItemData->pDevObject->CreatePlayObject(0);
	// 			// 					}
	// 			// 
	// 			// 					if (pItemData->pPlayObject == NULL)
	// 			// 					{
	// 			// 						//AfxMessageBox(L"打开设备失败");
	// 			// 						//pData->pDevObject->Delete();
	// 			// 						//pData->pDevObject = NULL;
	// 			// 						//lResult = -77;
	// 			// 
	// 			// 						//pItemData->m_csPlay.Unlock();
	// 			// 						return 0;
	// 			// 					}
	// 			// 
	// 			// 
	// 			// 					int lResult = pItemData->pPlayObject->OpenRealtime(m_pVideoDlg->get_hwnd(pItemData->winIndex), 0);
	// 			// 					if (lResult != 0)
	// 			// 					{
	// 			// 						pItemData->pPlayObject->Close();
	// 			// 						pItemData->pPlayObject->Delete();
	// 			// 						pItemData->pPlayObject = NULL;
	// 			// 						
	// 			// 					}
	// 			// 
	// 			// 					//pItemData->m_csPlay.Unlock();
	// 			// 					return 0;
	// 			// 				}
	// 			//			}
	// 
	// 			String_TO_Language_INI(L"str_message_callback_6", str);
	// 			WideCharToMultiByte(CP_UTF8, NULL, str, -1, szKey, sizeof(szKey), NULL, FALSE);
	// 			sprintf_s(szString, "%s (%s)", szKey, pItemData->dev_name);
	// 			CDatabase::Instance().LogInsert(pItemData->_id, theApp.m_clientSetting.server_enable ? 1 : 0, szString);
	// 			break;
	// 		case XMSG_NET_STATUS____CONNECT_FAIlED:
	// 			String_TO_Language_INI(L"str_message_callback_2", str);
	// 			WideCharToMultiByte(CP_UTF8, NULL, str, -1, szKey, sizeof(szKey), NULL, FALSE);
	// 			sprintf_s(szString, "%s (%s)", szKey, pItemData->dev_name);
	// 			m_pVideoDlg->XSetSubWindowTitleOfIndex(pItemData->winIndex, szString);
	// 
	// 			CDatabase::Instance().LogInsert(pItemData->_id, theApp.m_clientSetting.server_enable ? 1 : 0, szString);
	// 			break;
	// 		case XMSG_NET_STATUS____CONNECTING:
	// 			String_TO_Language_INI(L"str_message_callback_1", str);
	// 			WideCharToMultiByte(CP_UTF8, NULL, str, -1, szKey, sizeof(szKey), NULL, FALSE);
	// 			sprintf_s(szString, "%s (%s)", szKey, pItemData->dev_name);
	// 			m_pVideoDlg->XSetSubWindowTitleOfIndex(pItemData->winIndex, szString);
	// 
	// 			CDatabase::Instance().LogInsert(pItemData->_id, theApp.m_clientSetting.server_enable ? 1 : 0, szString);
	// 			break;
	// 		case XMSG_NET_STATUS____CONNECT_INVALID_PARAM:
	// 			String_TO_Language_INI(L"str_message_callback_3", str);
	// 			WideCharToMultiByte(CP_UTF8, NULL, str, -1, szKey, sizeof(szKey), NULL, FALSE);
	// 			sprintf_s(szString, "%s (%s)", szKey, pItemData->dev_name);
	// 			m_pVideoDlg->XSetSubWindowTitleOfIndex(pItemData->winIndex, szString);
	// 
	// 			CDatabase::Instance().LogInsert(pItemData->_id, theApp.m_clientSetting.server_enable ? 1 : 0, szString);
	// 			break;
	// 		case XMSG_NET_STATUS____DEVICE_NOT_ON_LINE:
	// 			String_TO_Language_INI(L"str_message_callback_4", str);
	// 			WideCharToMultiByte(CP_UTF8, NULL, str, -1, szKey, sizeof(szKey), NULL, FALSE);
	// 			sprintf_s(szString, "%s (%s)", szKey, pItemData->dev_name);
	// 			m_pVideoDlg->XSetSubWindowTitleOfIndex(pItemData->winIndex, szString);
	// 
	// 			CDatabase::Instance().LogInsert(pItemData->_id, theApp.m_clientSetting.server_enable ? 1 : 0, szString);
	// 			break;
	// 		case XMSG_NET_STATUS____DISCONNECTED:
	// 			String_TO_Language_INI(L"str_message_callback_5", str);
	// 			WideCharToMultiByte(CP_UTF8, NULL, str, -1, szKey, sizeof(szKey), NULL, FALSE);
	// 			sprintf_s(szString, "%s (%s)", szKey, pItemData->dev_name);
	// 			m_pVideoDlg->XSetSubWindowTitleOfIndex(pItemData->winIndex, szString);
	// 
	// 			CDatabase::Instance().LogInsert(pItemData->_id, theApp.m_clientSetting.server_enable ? 1 : 0, szString);
	// 			break;
	// 		default:
	// 			break;
	// 		}
	// 	}

#if 0
	switch (lMessageType)
	{
	case _X_MSG_LOGIN_CONNECTING:
	{
		pItemData->bLogin = FALSE;
		PostMessage(WM_TREE_ICO_TYPE, 2, (LPARAM)pItemData);
		//SetItemImage(pItemData->hItem, 2, 2);
	}
	break;
	case _X_MSG_LOGIN_CONNECTED:
	{
		pItemData->bLogin = TRUE;
		PostMessage(WM_TREE_ICO_TYPE, 1, (LPARAM)pItemData);
		//SetItemImage(pItemData->hItem, 1, 1);
	}

	break;

	case _X_MSG_OPEN_STREAM_CONNECTING:
	{
		//PostMessage(WM_TREE_ICO_TYPE, 3, (LPARAM)pItemData);
		//SetItemImage(pItemData->hItem, 3, 3);
		//CPlatClientDlg * p = (CPlatClientDlg *)m_ParentWnd;
		//p->ViewOpenStreamStatus(pItemData->hItem, pItemData->channelofwin, pItemData->devid,FALSE);
	}
	break;
	case _X_MSG_OPEN_STREAM_CONNECTED:
	{
		//PostMessage(WM_TREE_ICO_TYPE, 4, (LPARAM)pItemData);
		//SetItemImage(pItemData->hItem, 4, 4);
		//CPlatClientDlg * p = (CPlatClientDlg *)m_ParentWnd;
		//p->ViewOpenStreamStatus(pItemData->hItem, pItemData->channelofwin, pItemData->devid,TRUE);
	}

	break;
	default:
		break;
	}
#endif


	// 	m_csTree.Unlock();

	return 0;
}

#endif
