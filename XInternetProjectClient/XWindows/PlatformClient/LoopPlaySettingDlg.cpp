// LoopPlaySettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "LoopPlaySettingDlg.h"
#include "afxdialogex.h"
#include "ClassicDeviceTree.h"


// CLoopPlaySettingDlg dialog

IMPLEMENT_DYNAMIC(CLoopPlaySettingDlg, CDialogEx)

CLoopPlaySettingDlg::CLoopPlaySettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoopPlaySettingDlg::IDD, pParent)
{

}

CLoopPlaySettingDlg::~CLoopPlaySettingDlg()
{
	m_FileViewImages.DeleteImageList();
}

void CLoopPlaySettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_pTreeDev);
	DDX_Control(pDX, IDC_COMBO1, m_spitewnd);
	DDX_Control(pDX, IDC_COMBO5, m_looptime);
	DDX_Control(pDX, IDC_BUTTON1, m_close);
}


BEGIN_MESSAGE_MAP(CLoopPlaySettingDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CLoopPlaySettingDlg::OnNMClickTree1)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CLoopPlaySettingDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLoopPlaySettingDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CLoopPlaySettingDlg message handlers





void CLoopPlaySettingDlg::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	LPNM_TREEVIEW pNMLV = reinterpret_cast<LPNM_TREEVIEW>(pNMHDR);

	CPoint point;
	UINT uFlag;
	GetCursorPos(&point);
	m_pTreeDev.ScreenToClient(&point);
	HTREEITEM item = m_pTreeDev.HitTest(point, &uFlag);
	if ((item) && (TVHT_ONITEMSTATEICON & uFlag))
	{
		m_pTreeDev.SelectItem(item);
		BOOL bCheck = m_pTreeDev.GetCheck(item);
		SetItemCheckState(item, !bCheck);
	}
	*pResult = 0;
}


BOOL CLoopPlaySettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	ButtonLoadBitmap(m_close, L"title_bar/close_1.png", L"title_bar/close_2.png", L"title_bar/close_3.png", L"title_bar/close_4.png");
	CRect rc;
	GetClientRect(rc);
	int space = 1;
	m_close.SetWindowPos(NULL, rc.left + rc.Width() - theApp.m_nCloseWidth, 0, theApp.m_nCloseWidth, theApp.m_nCloseHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	//

// 	CString str;
// 	str.Format(L"%d", theApp.m_clientSetting.loopplay_wnd);
// 	m_spitewnd.SetWindowText(str);
// 	str.Format(L"%d", theApp.m_clientSetting.loopplay_sec);
// 	m_looptime.SetWindowText(str);

	switch (theApp.m_clientSetting.loopplay_wnd)
	{
	case 1:
	{
		m_spitewnd.SetCurSel(0);
	}
		break;
	case 4:
	{
		m_spitewnd.SetCurSel(1);
	}
	break;
	case 6:
	{
		m_spitewnd.SetCurSel(2);
	}
	break;
	case 8:
	{
		m_spitewnd.SetCurSel(3);
	}
	break;
	case 9:
	{
		m_spitewnd.SetCurSel(4);
	}
	break;
	case 16:
	{
		m_spitewnd.SetCurSel(5);
	}
	break;
	default:
		break;
	}
	//15;30;60;120;240;
	switch (theApp.m_clientSetting.loopplay_sec)
	{
	case 15:
	{
		m_looptime.SetCurSel(0);
	}
	break;
	case 30:
	{
		m_looptime.SetCurSel(1);
	}
	break;
	case 60:
	{
		m_looptime.SetCurSel(2);
	}
	break;
	case 120:
	{
		m_looptime.SetCurSel(3);
	}
	break;
	case 240:
	{
		m_looptime.SetCurSel(4);
	}
	break;
	default:
		break;
	}

	//
	UpdataTreeItem();
	//


	CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CLoopPlaySettingDlg::PreTranslateMessage(MSG* pMsg)
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


void CLoopPlaySettingDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	OnPaintBmpBackground(this, dc);
}


void CLoopPlaySettingDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CLoopPlaySettingDlg::OnBnClickedButton2()
{
	//
	CString str;
	m_spitewnd.GetWindowText(str);
	theApp.m_clientSetting.loopplay_wnd = atoi(__cstringToString(str).c_str());
	m_looptime.GetWindowText(str);
	theApp.m_clientSetting.loopplay_sec = atoi(__cstringToString(str).c_str());
	//
	theApp.m_clientSetting.loopplaycount = 0;
	//
	HTREEITEM hSiblingItem = m_pTreeDev.GetRootItem();
	while (hSiblingItem)
	{
		TravelSelectedItem(&m_pTreeDev, hSiblingItem);
		hSiblingItem = m_pTreeDev.GetNextSiblingItem(hSiblingItem);
	}
	//
	CSysFileGetSet::Instance().Set(theApp.m_clientSetting);
	//
	CDialogEx::OnOK();
}

void CLoopPlaySettingDlg::TravelSelectedItem(CTreeCtrl *pTreeCtrl, HTREEITEM hItem)
{
	HTREEITEM hCurItem = pTreeCtrl->GetChildItem(hItem);
	HTREEITEM hNextItem;
	while (hCurItem)
	{
		int k = pTreeCtrl->GetItemData(hCurItem);
		BOOL bcheck = pTreeCtrl->GetCheck(hCurItem);
		if (k < AREA_ID_PPPPP &&  bcheck)
		{
			memset(theApp.m_clientSetting.loopid[theApp.m_clientSetting.loopplaycount], 0, sizeof(theApp.m_clientSetting.loopid[theApp.m_clientSetting.loopplaycount]));
			sprintf(theApp.m_clientSetting.loopid[theApp.m_clientSetting.loopplaycount], "%d", k);
			++theApp.m_clientSetting.loopplaycount;
			//
			OutputDebugString(pTreeCtrl->GetItemText(hCurItem));  //选中的设备节点
		}
		hNextItem = hCurItem;
		TravelSelectedItem(pTreeCtrl, hNextItem);
		hCurItem = pTreeCtrl->GetNextSiblingItem(hCurItem);
	}
}



void CLoopPlaySettingDlg::UpdataTreeItem()
{
	CBitmap *bmp;
	CImage img;
	HRESULT ret = img.Load(theApp.m_strSkinPath + L"device_hc.bmp"); // filename 是要加载的文件名（包含路径） 
	HBITMAP bitmap = img.Detach();
	bmp = CBitmap::FromHandle(bitmap);
	UINT nFlags = ILC_MASK;
	nFlags |= ILC_COLOR24;// (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;
	m_FileViewImages.Create(24, 24, nFlags, 0, 0);
	m_FileViewImages.Add(bmp, RGB(255, 0, 255));
	m_pTreeDev.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
	bmp->DeleteObject();



	WCHAR wszInfo[256];
	char szTmp[256] = { 0 };
	for (int i = 0; i < (int)CDeviceManager::Instance().GetCount(); i++)
	{

		if (CDeviceManager::Instance().m_DevList[i].classicinfo.dev_is_publish)
		{
			continue;
		}
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//第一次添加
		if (NULL == m_pTreeDev.GetRootItem())
		{
			//先添加区域
			wmemset(wszInfo, 0, sizeof(sizeof(wszInfo) / sizeof(wszInfo[0])));
			MultiByteToWideChar(CP_UTF8, 0, CDeviceManager::Instance().m_DevList[i].classicinfo.dev_area_name, strlen(CDeviceManager::Instance().m_DevList[i].classicinfo.dev_area_name) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
			HTREEITEM hItemArea = m_pTreeDev.InsertItem(wszInfo, 0, 0);
			m_pTreeDev.SetItemData(hItemArea, (DWORD_PTR)(CDeviceManager::Instance().m_DevList[i].classicinfo.dev_areaid + AREA_ID_PPPPP));
			m_pTreeDev.Expand(hItemArea, TVE_EXPAND);

			//再添加设备
			MultiByteToWideChar(CP_UTF8, 0, CDeviceManager::Instance().m_DevList[i].classicinfo.dev_name, strlen(CDeviceManager::Instance().m_DevList[i].classicinfo.dev_name) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));

			HTREEITEM hItem;
			if (0 != CDeviceManager::Instance().m_DevList[i].classicinfo.dev_online)
			{
				hItem = m_pTreeDev.InsertItem(wszInfo, 2, 2, hItemArea);
			}
			else
			{
				hItem = m_pTreeDev.InsertItem(wszInfo, 1, 1, hItemArea);
			}

			//CDeviceManager::Instance().m_DevList[i].hItem = hItem;
			m_pTreeDev.SetItemData(hItem, i);// (DWORD_PTR)CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_id);
			m_pTreeDev.Expand(hItemArea, TVE_EXPAND);

			continue;
		}
		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//遍历树插入适当的节点
		//只有2级，所以这样做
		//
		BOOL bFindArea = FALSE;
		HTREEITEM h;
		h = m_pTreeDev.GetRootItem();

		if ((int)m_pTreeDev.GetItemData(m_pTreeDev.GetRootItem()) == (CDeviceManager::Instance().m_DevList[i].classicinfo.dev_areaid + AREA_ID_PPPPP))
		{
			bFindArea = TRUE;
		}
		else
		{
			h = m_pTreeDev.GetNextSiblingItem(m_pTreeDev.GetRootItem());
			while (NULL != h)
			{
				int k = (int)m_pTreeDev.GetItemData(h);
				if (k == CDeviceManager::Instance().m_DevList[i].classicinfo.dev_areaid + AREA_ID_PPPPP)
				{
					bFindArea = TRUE;
					break;
				}
				h = m_pTreeDev.GetNextSiblingItem(h);
			}
		}

		if (bFindArea)
		{
			//添加设备
			MultiByteToWideChar(CP_UTF8, 0, CDeviceManager::Instance().m_DevList[i].classicinfo.dev_name, strlen(CDeviceManager::Instance().m_DevList[i].classicinfo.dev_name) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
			HTREEITEM hItem;
			if (0 != CDeviceManager::Instance().m_DevList[i].classicinfo.dev_online)
			{
				hItem = m_pTreeDev.InsertItem(wszInfo, 2, 2, h);
			}
			else
			{
				hItem = m_pTreeDev.InsertItem(wszInfo, 1, 1, h);
			}
			//HTREEITEM hItem = InsertItem(wszInfo, 1, 1, h);
			//CDeviceManager::Instance().m_DevList[i].hItem = hItem;
			m_pTreeDev.SetItemData(hItem, i); //(DWORD_PTR)CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_id);
			m_pTreeDev.Expand(h, TVE_EXPAND);
		}
		else
		{
			//先添加区域
			MultiByteToWideChar(CP_UTF8, 0, CDeviceManager::Instance().m_DevList[i].classicinfo.dev_area_name, strlen(CDeviceManager::Instance().m_DevList[i].classicinfo.dev_area_name) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
			HTREEITEM hItemArea = m_pTreeDev.InsertItem(wszInfo, 0, 0);
			m_pTreeDev.SetItemData(hItemArea, (DWORD_PTR)(CDeviceManager::Instance().m_DevList[i].classicinfo.dev_areaid + AREA_ID_PPPPP));
			m_pTreeDev.Expand(hItemArea, TVE_EXPAND);

			//再添加设备
			MultiByteToWideChar(CP_UTF8, 0, CDeviceManager::Instance().m_DevList[i].classicinfo.dev_name, strlen(CDeviceManager::Instance().m_DevList[i].classicinfo.dev_name) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
			HTREEITEM hItem;
			if (0 != CDeviceManager::Instance().m_DevList[i].classicinfo.dev_online)
			{
				hItem = m_pTreeDev.InsertItem(wszInfo, 2, 2, hItemArea);
			}
			else
			{
				hItem = m_pTreeDev.InsertItem(wszInfo, 1, 1, hItemArea);
			}
			//HTREEITEM hItem = InsertItem(wszInfo, 1, 1, hItemArea);
			//CDeviceManager::Instance().m_DevList[i].hItem = hItem;
			m_pTreeDev.SetItemData(hItem, i); //(DWORD_PTR)CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_id);
			m_pTreeDev.Expand(hItemArea, TVE_EXPAND);
		}
	}


	for (int i = 0; i < theApp.m_clientSetting.loopplaycount; i++)
	{
		int v = atoi(theApp.m_clientSetting.loopid[i]);
		HTREEITEM item = GetWantSelectedTreeItemByItemData(v);
		if (item)
		{
			m_pTreeDev.SetCheck(item, true);
			SetParentCheck(item, true);
		}
	}

}


HTREEITEM CLoopPlaySettingDlg::GetWantSelectedTreeItemByItemData(int v)
{
	HTREEITEM item = NULL;
	//
	HTREEITEM hSiblingItem = m_pTreeDev.GetRootItem();
	while (hSiblingItem)
	{
		item = GetWantSelectedTreeItemByItemDataTravelSelectedItem(&m_pTreeDev, hSiblingItem,v);
		if (item)
		{
			break;
		}
		hSiblingItem = m_pTreeDev.GetNextSiblingItem(hSiblingItem);
	}
	return item;
}

HTREEITEM CLoopPlaySettingDlg::GetWantSelectedTreeItemByItemDataTravelSelectedItem(CTreeCtrl *pTreeCtrl, HTREEITEM hItem, int v)
{
	HTREEITEM item = NULL;
	//
	HTREEITEM hCurItem = pTreeCtrl->GetChildItem(hItem);
	while (hCurItem)
	{
		int k = pTreeCtrl->GetItemData(hCurItem);
		if (k == v)
		{
			item = hCurItem;
			break;
		}
		//
		HTREEITEM hNextItem = hCurItem;
		item = GetWantSelectedTreeItemByItemDataTravelSelectedItem(pTreeCtrl, hNextItem,v);
		if (item)
		{
			break;
		}
		hCurItem = pTreeCtrl->GetNextSiblingItem(hCurItem);
	}
	//
	return item;
}




void CLoopPlaySettingDlg::SetItemCheckState(HTREEITEM item, BOOL bCheck)
{
	SetChildCheck(item, bCheck);
	SetParentCheck(item, bCheck);

}

void CLoopPlaySettingDlg::SetChildCheck(HTREEITEM item, BOOL bCheck)
{
	m_pTreeDev.Expand(item, TVE_EXPAND);
	HTREEITEM child = m_pTreeDev.GetChildItem(item);
	//DeviceInfo *temp;
	while (child)
	{
		int __aaaa = m_pTreeDev.GetItemData(child);
		if (__aaaa > AREA_ID_PPPPP)
		{
			/*
			if (temp->dwStatus !=2)
			{
			HideCheckBox(child);
			}else
			*/
			{
				m_pTreeDev.SetCheck(child, bCheck);
				SetChildCheck(child, bCheck);
			}
		}
		else
		{
			m_pTreeDev.SetCheck(child, bCheck);
			SetChildCheck(child, bCheck);
		}

		child = m_pTreeDev.GetNextSiblingItem(child);
	}
}

void CLoopPlaySettingDlg::SetParentCheck(HTREEITEM item, BOOL bCheck)
{
	HTREEITEM parent = m_pTreeDev.GetParentItem(item);
	if (parent == NULL)
		return;

	if (bCheck)
		m_pTreeDev.SetCheck(parent, bCheck);
	else
	{
		HTREEITEM bro = m_pTreeDev.GetNextItem(item, TVGN_NEXT);
		BOOL bFlag = false;
		while (bro)
		{
			if (m_pTreeDev.GetCheck(bro))
			{
				bFlag = true;
				break;
			}
			bro = m_pTreeDev.GetNextItem(bro, TVGN_NEXT);
		}
		if (!bFlag)
		{
			bro = m_pTreeDev.GetNextItem(item, TVGN_PREVIOUS);
			while (bro)
			{
				if (m_pTreeDev.GetCheck(bro))
				{
					bFlag = true;
					break;
				}
				bro = m_pTreeDev.GetNextItem(bro, TVGN_PREVIOUS);
			}
		}
		if (!bFlag)
			m_pTreeDev.SetCheck(parent, false);

	}
}
