#pragma once
/*

1 HBITMAP转成CBitmap
HBITMAP   hBmp;
方法①

CBitmap* pBmp = CBitmap::FromHandle(hBmp);
方法②

CBitmap bmp;
bmp.Attach(hBmp);
方法①转换出来的临时CBitmap对象似乎并不是可靠的！MSDN上说这个临时只在某一时间段内有效。

等到某个空闲的时候，这个临时CBitmap对象会被清除掉。所以觉得还是方法②好一点


注意点：
Attach和FromHandle的区别
FromHandle得到的指针是临时变量，
,通过Attach连接的句柄可以长久保留,但通过FromHandle得到的只是暂时的,
大概只在一个消息区间内有效,很快便会被删除,所以基本上不能用。
我用了FromHandle然后一直出错！！！



2 CBitmap转成HBITMAP
方法①
CBitmap*   pBmp;
HBITMAP   hBmp   =   (HBITMAP)(*pBmp);//(HBITMAP)是CBitmap类重载了的操作符.
方法②

CBitmap m_bitmap;
HBITMAP h_bmp;
h_bmp=m_bitmap.m_hObject;
*/


#include <afxwin.h>

#define WM_BMP_BTNDOWN (WM_USER+1124)
#define WM_BMP_BTNUP (WM_USER+1125)


class CBmpButton : public CButton
{

public:
	CBmpButton();


public:
	static int GetVersionI()		{return 10;}
	static LPCTSTR GetVersionC()	{return (LPCTSTR)_T("1.0");}


public:
	BOOL SetCur(HCURSOR hCursor, BOOL bRepaint); 
	BOOL SetCur(int nCursorId, BOOL bRepaint);
	void EnableBalloonTooltip();
	void ActivateTooltip(BOOL bEnable = TRUE);
	void SetTooltipText(LPCTSTR lpszText, BOOL bActivate = TRUE);

	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual void DrawTheText(CDC* pDC, LPCTSTR lpszText, RECT* rpItem, CRect* rpCaption, BOOL bIsDisabled = FALSE);

public:
	void SetTextAlignLeft()
	{
		m_bTextLeft = TRUE;
		Invalidate();
	};
	void SetMouseDown(BOOL bDown);
// 	BOOL LoadBitmaps(UINT nIDBitmapResource,
// 			UINT nIDBitmapResourceSel = 0,
// 			UINT nIDBitmapResourceFocus = 0,
// 			UINT nIDBitmapResourceDisabled = 0,
// 			int CtrlId = -1);

	BOOL LoadBitmaps(HBITMAP nIDBitmapResource,
		HBITMAP nIDBitmapResourceSel = NULL,
		HBITMAP nIDBitmapResourceFocus = NULL,
		HBITMAP nIDBitmapResourceDisabled = NULL,
		int CtrlId = -1);

	int m_nCtrlId;
	void SizeToContent();
	virtual ~CBmpButton();
private:
	BOOL m_bTextLeft;

	BOOL bMouseBown;
	BOOL OnMouse;
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	
protected:
	
	CBitmap m_bitmap;           
	CBitmap m_bitmapSel;        
	CBitmap m_bitmapFocus;      
	CBitmap m_bitmapDisabled;   

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	DECLARE_MESSAGE_MAP()
private:
	void InitToolTip();
	CToolTipCtrl m_ToolTip;
	DWORD		 m_dwToolTipStyle;	
	HCURSOR      m_hCursor;
public:
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};