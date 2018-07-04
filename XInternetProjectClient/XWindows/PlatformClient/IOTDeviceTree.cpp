#include "stdafx.h"
#include "IOTDeviceTree.h"

#include "PlatformClient.h"
#include "PlatformClientDlg.h"

extern CPlatClientApp theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIOTDeviceTree::CIOTDeviceTree()
{
	m_hSelectedItem = NULL;
}

CIOTDeviceTree::~CIOTDeviceTree()
{
	m_FileViewImages.DeleteImageList();
}

BEGIN_MESSAGE_MAP(CIOTDeviceTree, CTreeCtrl)
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_DELETEITEM()
	ON_WM_CTLCOLOR()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CIOTDeviceTree::OnNMCustomdraw)
	ON_WM_NCPAINT()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CIOTDeviceTree::Init()
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
	SetImageList(&m_FileViewImages, TVSIL_NORMAL);
	bmp->DeleteObject();
	//
	UpdataTreeItem();
	//
	SetBkColor(RGB(theApp.m_cfg.bgr, theApp.m_cfg.bgg, theApp.m_cfg.bgb));
	//SetBkColor(RGB(32, 32, 32));
	SetTextColor(RGB(255, 255, 255));
}
// void CDeviceTree::UpdataTreeItem4Plat1()
// {
// 	WCHAR wszInfo[256];
// 	char szTmp[256] = { 0 };
// 	for (int i = 0; i < (int)CDeviceManager::Instance().GetCount(); i++)
// 	{
// 		//先添加区域
// 		wmemset(wszInfo, 0, sizeof(sizeof(wszInfo) / sizeof(wszInfo[0])));
// 		//再添加设备
// 		MultiByteToWideChar(CP_ACP, 0, CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_name, strlen(CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_name) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
// 
// 		HTREEITEM hItem = InsertItem(wszInfo, 1, 1, GetNextSiblingItem( GetRootItem()));
// 
// 
// 		CDeviceManager::Instance().m_DevList[i].hItem = hItem;
// 		SetItemData(hItem, i);// (DWORD_PTR)CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_id);
// 		Expand(GetNextSiblingItem(GetRootItem()), TVE_EXPAND);
// 	}
// }

void CIOTDeviceTree::Deinit()
{
	m_csTree.Lock();
	//
	
	//
	DeleteAllItems();
	//
	m_csTree.Unlock();
	TRACE(" ###>>> void CDeviceTree::Deinit() -> while exit\n");
}



void CIOTDeviceTree::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	POINT pt;
	if (!GetCursorPos(&pt))
		return;
	ScreenToClient(&pt);
	UINT uFlags;
	HTREEITEM hItem = HitTest(pt, &uFlags);
	m_hSelectedItem = hItem;
	if (NULL == hItem)
	{
		return;
	}
	SelectItem(hItem); 





	int img1, img2;
	GetItemImage(m_hSelectedItem, img1, img2);
// 	if (img1 == 3)
// 	{
// 		return;
// 	}
	if (img1 == 1)
	{
		return;
	}


// 	CDlgPanLive * p = (CDlgPanLive *)m_ParentWnd;
// 	int mmm = p->m_pVideoDlg->GetNextOpenWindows();
// 	stDevice st;
// 	CDeviceManager::Instance().GetDeviceByUuid((char *)GetItemData(m_hSelectedItem), st);
// 	st.hItem = m_hSelectedItem;
// 	if (0 == p->XViewOpenStream(st, mmm))
// 	{
// 		CDeviceManager::Instance().SetDeviceOpenStatusByDeviceID(m_hSelectedItem, TRUE);
// 		SetItemImage(m_hSelectedItem, 3, 3);
// 	}


}

void CIOTDeviceTree::UpdataTreeItem()
{
// 	WCHAR wszInfo[256];
// 	char szTmp[256] = { 0 };
// 	for (int i = 0; i < (int)CDeviceManager::Instance().GetCount(); i++)
// 	{
// 
// // 		if (CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_is_publish)
// // 		{
// // 			continue;
// // 		}
// 
// #if 1
// 
// 
// #if 1
// 
// 		wmemset(wszInfo, 0, sizeof(sizeof(wszInfo) / sizeof(wszInfo[0])));
// 		MultiByteToWideChar(CP_ACP, 0, CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_name, strlen(CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_name) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
// 
// 		HTREEITEM hItem;// = InsertItem(wszInfo, 1, 1, GetRootItem());
// 
// 		if (0 != CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_online)
// 		{
// 			hItem = InsertItem(wszInfo, 2, 2, GetRootItem());
// 		}
// 		else
// 		{
// 			hItem = InsertItem(wszInfo, 1, 1, GetRootItem());
// 		}
// 
// 
// 		CDeviceManager::Instance().m_DevList[i].hItem = hItem;
// 		SetItemData(hItem, (DWORD)CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_uuid);// (DWORD_PTR)CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_id);
// 		Expand(GetRootItem(), TVE_EXPAND);
// #else
// 
// 		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 		//第一次添加
// 		if (NULL == GetRootItem())
// 		{
// 			//先添加区域
// 			wmemset(wszInfo, 0, sizeof(sizeof(wszInfo) / sizeof(wszInfo[0])));
// 			MultiByteToWideChar(CP_UTF8, 0, CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_area_name, strlen(CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_area_name) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
// 			HTREEITEM hItemArea = InsertItem(wszInfo, 0, 0);
// 			SetItemData(hItemArea, (DWORD_PTR)(CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_areaid + AREA_ID_PPPPP));
// 			Expand(hItemArea, TVE_EXPAND);
// 
// 			//再添加设备
// 			MultiByteToWideChar(CP_UTF8, 0, CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_name, strlen(CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_name) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
// 
// 			HTREEITEM hItem;
// 			if (0 != CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_online)
// 			{
// 				hItem = InsertItem(wszInfo, 2, 2, hItemArea);
// 			}
// 			else
// 			{
// 				hItem = InsertItem(wszInfo, 1, 1, hItemArea);
// 			}
// 
// 			CDeviceManager::Instance().m_DevList[i].hItem = hItem;
// 			SetItemData(hItem, i);// (DWORD_PTR)CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_id);
// 			Expand(hItemArea, TVE_EXPAND);
// 
// 			continue;
// 		}
// 		//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 		//遍历树插入适当的节点
// 		//只有2级，所以这样做
// 		//
// 		BOOL bFindArea = FALSE;
// 		HTREEITEM h;
// 		h = GetRootItem();
// 
// 		if ((int)GetItemData(GetRootItem()) == (CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_areaid + AREA_ID_PPPPP))
// 		{
// 			bFindArea = TRUE;
// 		}
// 		else
// 		{
// 			h = GetNextSiblingItem(GetRootItem());
// 			while (NULL != h)
// 			{
// 				int k = (int)GetItemData(h);
// 				if (k == CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_areaid + AREA_ID_PPPPP)
// 				{
// 					bFindArea = TRUE;
// 					break;
// 				}
// 				h = GetNextSiblingItem(h);
// 			}
// 		}
// 
// 
// 
// 		if (bFindArea)
// 		{
// 			//添加设备
// 			MultiByteToWideChar(CP_UTF8, 0, CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_name, strlen(CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_name) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
// 			HTREEITEM hItem;
// 			if (0 != CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_online)
// 			{
// 				hItem = InsertItem(wszInfo, 2, 2, h);
// 			}
// 			else
// 			{
// 				hItem = InsertItem(wszInfo, 1, 1, h);
// 			}
// 			//HTREEITEM hItem = InsertItem(wszInfo, 1, 1, h);
// 			CDeviceManager::Instance().m_DevList[i].hItem = hItem;
// 			SetItemData(hItem, i); //(DWORD_PTR)CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_id);
// 			Expand(h, TVE_EXPAND);
// 		}
// 		else
// 		{
// 			//先添加区域
// 			MultiByteToWideChar(CP_UTF8, 0, CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_area_name, strlen(CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_area_name) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
// 			HTREEITEM hItemArea = InsertItem(wszInfo, 0, 0);
// 			SetItemData(hItemArea, (DWORD_PTR)(CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_areaid + AREA_ID_PPPPP));
// 			Expand(hItemArea, TVE_EXPAND);
// 
// 			//再添加设备
// 			MultiByteToWideChar(CP_UTF8, 0, CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_name, strlen(CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_name) + 1, wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));
// 			HTREEITEM hItem;
// 			if (0 != CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_online)
// 			{
// 				hItem = InsertItem(wszInfo, 2, 2, hItemArea);
// 			}
// 			else
// 			{
// 				hItem = InsertItem(wszInfo, 1, 1, hItemArea);
// 			}
// 			//HTREEITEM hItem = InsertItem(wszInfo, 1, 1, hItemArea);
// 			CDeviceManager::Instance().m_DevList[i].hItem = hItem;
// 			SetItemData(hItem, i); //(DWORD_PTR)CDeviceManager::Instance().m_DevList[i].SvrDevInfo.dev_id);
// 			Expand(hItemArea, TVE_EXPAND);
// 		}
// 
// #endif
// 
// #endif
// 
// 
// 	}
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CIOTDeviceTree::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
#if 0
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	return;
#endif

	NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pNMHDR;
	*pResult = CDRF_NOTIFYITEMDRAW;
	//|CDRF_NOTIFYPOSTPAINT|CDRF_NOTIFYSUBITEMDRAW|CDRF_NOTIFYPOSTERASE;
	LPNMTVCUSTOMDRAW pDraw = (LPNMTVCUSTOMDRAW)pNMHDR;
	DWORD dwDrawStage = pDraw->nmcd.dwDrawStage;

	if (dwDrawStage & CDDS_ITEM)
	{
		HTREEITEM hItem = (HTREEITEM)pDraw->nmcd.dwItemSpec;
		//CTreeCtrl    &LeftTree = GetTreeCtrl();
		HTREEITEM hTreeSelect = GetSelectedItem();
		if ((hTreeSelect) && (hTreeSelect == hItem)) //TviTxt.Find("WIN2K") > -1)// found
		{
			pDraw->clrTextBk = 0x3e3e3e;
			pDraw->clrText = 0xffffff;
		}
	}

}


void CIOTDeviceTree::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	CTreeCtrl::PreSubclassWindow();
	m_HScroll.InitScrollBar(this, SB_HORZ);
	m_VScroll.InitScrollBar(this, SB_VERT);
}

void CIOTDeviceTree::OnPaint()
{
	//  CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CTreeCtrl::OnPaint() for painting messages

	CTreeCtrl::OnPaint();
	m_HScroll.UpdateScrollBar();
	m_VScroll.UpdateScrollBar();
}