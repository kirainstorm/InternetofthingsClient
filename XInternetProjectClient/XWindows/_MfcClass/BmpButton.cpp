


#include "stdafx.h"
#include "BmpButton.h"

CBmpButton::CBmpButton()
{
	m_hCursor = NULL;
	
	m_ToolTip.m_hWnd = NULL;
	m_dwToolTipStyle = 0;

	OnMouse = FALSE;
	bMouseBown = FALSE;
	m_nCtrlId = -1;
	SetCur(::LoadCursorW(NULL, MAKEINTRESOURCEW(IDC_HAND)),FALSE);

	m_bTextLeft = FALSE;
}

CBmpButton::~CBmpButton()
{
	if (m_hCursor)
	{
		::DestroyCursor(m_hCursor);
	}
	m_bitmap.Detach();
	m_bitmapSel.Detach();
	m_bitmapFocus.Detach();
	m_bitmapDisabled.Detach();

	m_bitmap.DeleteObject();
	m_bitmapSel.DeleteObject();
	m_bitmapFocus.DeleteObject();
	m_bitmapDisabled.DeleteObject();
}

BEGIN_MESSAGE_MAP(CBmpButton, CButton)
	
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_SETCURSOR()
	
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CBmpButton::SizeToContent()
{
	ASSERT(m_bitmap.m_hObject != NULL);
	CSize bitmapSize;
	BITMAP bmInfo;
	VERIFY(m_bitmap.GetObject(sizeof(bmInfo), &bmInfo) == sizeof(bmInfo));
	VERIFY(SetWindowPos(NULL, -1, -1, bmInfo.bmWidth, bmInfo.bmHeight,
		SWP_NOMOVE|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOACTIVATE));
}

// BOOL CBmpButton::LoadBitmaps(UINT nIDBitmapResource,
// 						UINT nIDBitmapResourceSel,
// 						UINT nIDBitmapResourceFocus,
// 						UINT nIDBitmapResourceDisabled,
// 						int CtrlId)
// {
// 	
// 	m_nCtrlId = CtrlId;
// 	m_bitmap.DeleteObject();
// 	m_bitmapSel.DeleteObject();
// 	m_bitmapFocus.DeleteObject();
// 	m_bitmapDisabled.DeleteObject();
// 
// 	if (!m_bitmap.LoadBitmap(nIDBitmapResource))
// 	{
// 
// 		return FALSE;   
// 	}
// 	BOOL bAllLoaded = TRUE;
// 	if (nIDBitmapResourceSel != 0)
// 	{
// 		if (!m_bitmapSel.LoadBitmap(nIDBitmapResourceSel))
// 		{
// 
// 			bAllLoaded = FALSE;
// 		}
// 	}
// 	if (nIDBitmapResourceFocus != 0)
// 	{
// 		if (!m_bitmapFocus.LoadBitmap(nIDBitmapResourceFocus))
// 			bAllLoaded = FALSE;
// 	}
// 	if (nIDBitmapResourceDisabled != 0)
// 	{
// 		if (!m_bitmapDisabled.LoadBitmap(nIDBitmapResourceDisabled))
// 			bAllLoaded = FALSE;
// 	}
// 
// 	InitToolTip();
// 	return bAllLoaded;
// }
BOOL CBmpButton::LoadBitmaps(HBITMAP nIDBitmapResource,
	HBITMAP nIDBitmapResourceSel,
	HBITMAP nIDBitmapResourceFocus ,
	HBITMAP nIDBitmapResourceDisabled ,
	int CtrlId )
{
	m_nCtrlId = CtrlId;
	m_bitmap.DeleteObject();
	m_bitmapSel.DeleteObject();
	m_bitmapFocus.DeleteObject();
	m_bitmapDisabled.DeleteObject();

	if (!m_bitmap.Attach(nIDBitmapResource))
	{
		return FALSE;
	}
	BOOL bAllLoaded = TRUE;
	if (nIDBitmapResourceSel != 0)
	{
		if (!m_bitmapSel.Attach(nIDBitmapResourceSel))
		{

			bAllLoaded = FALSE;
		}
	}
	if (nIDBitmapResourceFocus != 0)
	{
		if (!m_bitmapFocus.Attach(nIDBitmapResourceFocus))
		{
			bAllLoaded = FALSE;
		}
			
	}
	if (nIDBitmapResourceDisabled != 0)
	{
		if (!m_bitmapDisabled.Attach(nIDBitmapResourceDisabled))
		{
			bAllLoaded = FALSE;
		}
			
	}

	InitToolTip();
	return bAllLoaded;
}
LRESULT CBmpButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{

	SendMessage(WM_KILLFOCUS,0,0);
	OnMouse = FALSE;
	return S_OK;
} 

void CBmpButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	
	CWnd*				wndUnderMouse = NULL;
	CWnd*				wndActive = this;
	TRACKMOUSEEVENT		csTME;

	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);

	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		csTME.cbSize = sizeof(csTME);
		csTME.dwFlags = TME_LEAVE;
		csTME.hwndTrack = m_hWnd;
		::_TrackMouseEvent(&csTME);
		OnMouse = TRUE;
		SendMessage(WM_SETFOCUS,(WPARAM)m_hWnd ,0);
	}
	CButton::OnMouseMove(nFlags, point);
}

void CBmpButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{

	ASSERT(lpDIS != NULL);
	
	ASSERT(m_bitmap.m_hObject != NULL);     

	CBitmap* pBitmap = &m_bitmap;
	UINT state = lpDIS->itemState;
	if ((state & ODS_SELECTED) && m_bitmapSel.m_hObject != NULL)
		pBitmap = &m_bitmapSel;
	else if ( (state & ODS_FOCUS) && (m_bitmapFocus.m_hObject != NULL) && OnMouse )
		pBitmap = &m_bitmapFocus;   
	else if ((state & ODS_DISABLED) && m_bitmapDisabled.m_hObject != NULL)
		pBitmap = &m_bitmapDisabled;   
	
	if(bMouseBown)
	{
		if(m_bitmapSel.m_hObject != NULL)
			pBitmap = &m_bitmapSel;
		else
			pBitmap = &m_bitmap;
	}

	if ( lpDIS->hDC == NULL ) return;
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	if ( pDC == NULL ) return;
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	if ( memDC == NULL ) 
	{
		return;
	}

	CBitmap* pOld = memDC.SelectObject(pBitmap);
	if (pOld == NULL)
	{
		DeleteDC(memDC);
		return;  
	}

	CRect rect;
	rect.CopyRect(&lpDIS->rcItem);
	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOld);	
	DeleteDC(memDC);
	pOld->DeleteObject();
	//pBitmap->DeleteObject();

	CString sTitle;
	GetWindowText(sTitle);
	CRect captionRect = lpDIS->rcItem;
	if (sTitle.IsEmpty() == FALSE)
	{
		DrawTheText(pDC, (LPCTSTR)sTitle, &lpDIS->rcItem, &captionRect);
	}

}

BOOL CBmpButton::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message) 
	{ 
		case WM_MOUSEMOVE: 
			{
				//InitToolTip();
				// 
				// m_ToolTip.RelayEvent(pMsg); //del  [4/20/2015 win8]

			if (m_ToolTip.m_hWnd != NULL)//add  [4/20/2015 win8]
			{
				m_ToolTip.RelayEvent(pMsg);
			}
				
				break; 
			}
		case WM_LBUTTONDBLCLK:
			{
				pMsg->message = WM_LBUTTONDOWN;
				break;
			}
		default:
			break;
	}

	//InitToolTip();
	//m_ToolTip.RelayEvent(pMsg);

	//if(pMsg->message == WM_LBUTTONDBLCLK)
	//	pMsg->message = WM_LBUTTONDOWN;
	
	return CButton::PreTranslateMessage(pMsg);
}

BOOL CBmpButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_hCursor != NULL)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CBmpButton::SetCur(HCURSOR hCursor, BOOL bRepaint)
{
	if (m_hCursor)
	{
		::DestroyCursor(m_hCursor);
		m_hCursor = NULL;
	}
	m_hCursor=hCursor;	
	if (bRepaint) Invalidate();
	return (m_hCursor == NULL ? FALSE : TRUE );
}

BOOL CBmpButton::SetCur(int nCursorId, BOOL bRepaint)
{
	HINSTANCE	hInstResource = NULL;
	if (m_hCursor)
	{
		::DestroyCursor(m_hCursor);
		m_hCursor = NULL;
	}
	if (nCursorId)
	{
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nCursorId), RT_GROUP_CURSOR);
		m_hCursor = (HCURSOR)::LoadImage(hInstResource, MAKEINTRESOURCE(nCursorId), IMAGE_CURSOR, 0, 0, 0);
		if (bRepaint) Invalidate();
	}
	return (m_hCursor == NULL ? FALSE : TRUE );
}

void CBmpButton::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		//EnableBalloonTooltip();
		m_ToolTip.Create(this, m_dwToolTipStyle);
		m_ToolTip.Activate(true);
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
		
	} 
}

void CBmpButton::SetTooltipText(LPCTSTR lpszText, BOOL bActivate)
{
	if (lpszText == NULL) return;
	InitToolTip();
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, lpszText, rectBtn, 1);
	}
	m_ToolTip.UpdateTipText(lpszText, this, 1);
	m_ToolTip.Activate(bActivate);
}

void CBmpButton::ActivateTooltip(BOOL bEnable)
{
	
	if (m_ToolTip.GetToolCount() == 0) return;
	
	m_ToolTip.Activate(bEnable);
}

void CBmpButton::EnableBalloonTooltip()
{
	m_dwToolTipStyle |= TTS_BALLOON;
}



void CBmpButton::SetMouseDown(BOOL bDown)
{
	bMouseBown = bDown;
	
	Invalidate();
}


void CBmpButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	
	if(m_nCtrlId > -1)
	{
		::SendMessage(GetParent()->m_hWnd,   WM_BMP_BTNDOWN,   m_nCtrlId/*可为id号*/,   0);   
	}
	CButton::OnLButtonDown(nFlags, point);
}

void CBmpButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	
	if(m_nCtrlId > -1)
	{
		::SendMessage(GetParent()->m_hWnd,   WM_BMP_BTNUP,   m_nCtrlId/*可为id号*/,   0); 
	}
	CButton::OnLButtonUp(nFlags, point);
}

void CBmpButton::DrawTheText(CDC* pDC, LPCTSTR lpszText, RECT* rpItem, CRect* rpCaption, BOOL bIsDisabled)
{
	if ( pDC == NULL )
	{
		return;
	}

	CRect centerRect = rpCaption;

	pDC->DrawText(lpszText, -1, rpCaption, DT_WORDBREAK | DT_CENTER | DT_CALCRECT);

	if (m_bTextLeft == TRUE)//////////////////////////////////////////////////////////////////////////
	{
		rpCaption->OffsetRect(10, (centerRect.Height() - rpCaption->Height()) / 2);
	}
	else
	{
		rpCaption->OffsetRect((centerRect.Width() - rpCaption->Width()) / 2, (centerRect.Height() - rpCaption->Height()) / 2);
	}


	pDC->SetBkMode(TRANSPARENT);

	if (bIsDisabled)
	{
		rpCaption->OffsetRect(1, 1);
		pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
		pDC->DrawText(lpszText, -1, rpCaption, DT_WORDBREAK | DT_CENTER);
		rpCaption->OffsetRect(-1, -1);
		pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
		pDC->DrawText(lpszText, -1, rpCaption, DT_WORDBREAK | DT_CENTER);
	} 
	else
	{
		//CFont mFont;
		//mFont.CreatePointFont(85, _T("微软雅黑"));
		//pDC->SetTextColor(RGB(151, 155, 163));
		//pDC->SelectObject(&mFont);
		//mFont.DeleteObject();
		pDC->SetTextColor(RGB(0, 0, 0)); 
		pDC->SetBkColor(RGB(0, 0, 0)); 
		pDC->DrawText(lpszText, -1, rpCaption, DT_WORDBREAK | DT_CENTER);
	} 
} 
