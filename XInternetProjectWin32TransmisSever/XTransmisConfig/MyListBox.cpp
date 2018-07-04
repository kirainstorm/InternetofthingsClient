#include "stdafx.h"
#include "MyListBox.h"


CMyListBox::CMyListBox()
{
	m_bgr = 200;
	m_bgg = 200;
	m_bgb = 200;
	m_itr = 233;
	m_itg = 233;
	m_itb = 233;
	m_fr = 0;
	m_fg = 0;
	m_fb = 0;
}


CMyListBox::~CMyListBox()
{
	m_brush.DeleteObject();
}
BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()
void CMyListBox::_SetColor(int bgr, int bgg, int bgb, int itr, int itg, int itb, int fr, int fg, int fb)
{
	m_bgr = bgr;
	m_bgg = bgg;
	m_bgb = bgb;
	m_itr = itr;
	m_itg = itg;
	m_itb = itb;
	m_fr = fr;
	m_fg = fg;
	m_fb = fb;

	m_brush.DeleteObject();
	m_brush.CreateSolidBrush(RGB(m_bgr, m_bgg, m_bgb));
}
void CMyListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->itemID != -1)
	{
		// TODO: Add your code to draw the specified item
		ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);
		//LPCTSTR lpszText = (LPCTSTR)lpDrawItemStruct->itemData;
		//ASSERT(lpszText != NULL);
		CDC dc;

		dc.Attach(lpDrawItemStruct->hDC);

		// Save these value to restore them when done drawing.
		COLORREF crOldTextColor = dc.GetTextColor();
		COLORREF crOldBkColor = dc.GetBkColor();

		// If this item is selected, set the background color 
		// and the text color to appropriate values. Also, erase
		// rect by filling it with the background color.
		if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
			(lpDrawItemStruct->itemState & ODS_SELECTED))
		{
			//dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
			dc.SetTextColor(RGB(m_fr, m_fg, m_fb));
			dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
			dc.FillSolidRect(&lpDrawItemStruct->rcItem,
				::GetSysColor(COLOR_HIGHLIGHT));
		}
		else
		{
			// 		if (lpDrawItemStruct->itemID % 2)
			// 			dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(128, 128, 128));
			// 		else
			// 			dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255, 128, 255));
			dc.SetTextColor(RGB(m_fr, m_fg, m_fb));
			dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(m_itr, m_itg, m_itb));
		}

		// If this item has the focus, draw a red frame around the
		// item's rect.
		if ((lpDrawItemStruct->itemAction | ODA_FOCUS) &&
			(lpDrawItemStruct->itemState & ODS_FOCUS))
		{
			//CBrush br(RGB(0, 0, 128));
			//dc.FrameRect(&lpDrawItemStruct->rcItem, &br);
		}
		lpDrawItemStruct->rcItem.left += 10;
		// Draw the text.


		CString strTmp;
		GetText(lpDrawItemStruct->itemID, strTmp);     //这样得到Text

		dc.DrawText(
			strTmp,
			strTmp.GetLength(),
			&lpDrawItemStruct->rcItem,
			DT_LEFT | DT_SINGLELINE | DT_VCENTER);

		// Reset the background color and the text color back to their
		// original values.
		//dc.SetTextColor(crOldTextColor);
		dc.SetTextColor(RGB(m_fr, m_fg, m_fb));
		dc.SetBkColor(crOldBkColor);

		dc.Detach();
	}
}
void CMyListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 40;
}
BOOL CMyListBox::OnEraseBkgnd(CDC* pDC)
{
// 	CRect rect;
// 	GetClientRect(rect);
// 	CBrush hbr;
// 	hbr.CreateSolidBrush(RGB(m_bgr, m_bgg, m_bgb));
// 	CBrush *pOldBrush = pDC->SelectObject(&hbr);//把当前的画笔选入DC
// 	pDC->FillRect(rect, &hbr);
// 	pDC->SelectObject(pOldBrush);//还原DC
// 	hbr.DeleteObject();


	return FALSE;//注销下面的那句话
	//return CListBox::OnEraseBkgnd(pDC);
}
HBRUSH CMyListBox::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  Change any attributes of the DC here

	pDC->SetBkColor(RGB(m_bgr, m_bgg, m_bgb));
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)m_brush.GetSafeHandle();
	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}
