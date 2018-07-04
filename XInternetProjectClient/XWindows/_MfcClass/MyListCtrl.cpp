#include "stdafx.h"
#include "MyListCtrl.h"

//#include "PlatformClient.h"
//extern CPlatClientApp theApp;
CMyListCtrl::CMyListCtrl()
{

}


CMyListCtrl::~CMyListCtrl()
{
}



BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CMyListCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CListCtrl::OnPaint() for painting messages

	CRect rcClient;
	GetClientRect(rcClient);
	CPen pen(PS_SOLID, 1, RGB(64, 64, 64));
	CPen *OldPen = (CPen *)dc.SelectObject(&pen);
	//dc.MoveTo(rcClient.left, rcClient.top);
	//dc.LineTo(rcClient.left, rcClient.bottom);
	//dc.MoveTo(rcClient.left, rcClient.bottom);
	//dc.LineTo(rcClient.right, rcClient.bottom);
	//dc.MoveTo(rcClient.right, rcClient.bottom);
	//dc.LineTo(rcClient.right, rcClient.top);
	dc.MoveTo(rcClient.right, rcClient.top);
	dc.LineTo(rcClient.left, rcClient.top);

	int xx = (-167);
	int bgr = 190 + xx;// 25;
	int bgg = 190 + xx;//85;
	int bgb = 190 + xx;//137;

	dc.SetBkColor(RGB(bgr, bgg, bgb));
	dc.SetTextColor(RGB(255, 255, 0));


}
void CMyListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  Add your code to draw the specified item
	// TODO: Add your code to draw the specified item
	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTVIEW);
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	ASSERT(NULL != dc.GetSafeHdc());
	// Save these value to restore them when done drawing.
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();

	// If this item is selected, set the background color 
	// and the text color to appropriate values. Also, erase
	// rect by filling it with the background color.
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		dc.FillSolidRect(&lpDrawItemStruct->rcItem,
			::GetSysColor(COLOR_HIGHLIGHT));
	}
	else
	{
		if (lpDrawItemStruct->itemID % 2)
			dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(128, 128, 128));
		else
			dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255, 128, 255));
	}

	// If this item has the focus, draw a red frame around the
	// item's rect.
	if ((lpDrawItemStruct->itemAction | ODA_FOCUS) &&
		(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		CBrush br(RGB(0, 0, 128));
		dc.FrameRect(&lpDrawItemStruct->rcItem, &br);
	}

	// Draw the text.
	CString strText(_T(""));
	CRect rcItem;

	for (int i = 0; i < GetHeaderCtrl()->GetItemCount(); i++)
	{
		strText = GetItemText(lpDrawItemStruct->itemID, i);
		GetSubItemRect(lpDrawItemStruct->itemID, i, LVIR_LABEL, rcItem);
		rcItem.left += 5;
		dc.DrawText(
			strText,
			strText.GetLength(),
			&rcItem,
			DT_LEFT | DT_SINGLELINE | DT_VCENTER);
	}

	// Reset the background color and the text color back to their
	// original values.
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);

	dc.Detach();

}