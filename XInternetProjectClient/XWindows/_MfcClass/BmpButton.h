#pragma once
/*

1 HBITMAPת��CBitmap
HBITMAP   hBmp;
������

CBitmap* pBmp = CBitmap::FromHandle(hBmp);
������

CBitmap bmp;
bmp.Attach(hBmp);
������ת����������ʱCBitmap�����ƺ������ǿɿ��ģ�MSDN��˵�����ʱֻ��ĳһʱ�������Ч��

�ȵ�ĳ�����е�ʱ�������ʱCBitmap����ᱻ����������Ծ��û��Ƿ����ں�һ��


ע��㣺
Attach��FromHandle������
FromHandle�õ���ָ������ʱ������
,ͨ��Attach���ӵľ�����Գ��ñ���,��ͨ��FromHandle�õ���ֻ����ʱ��,
���ֻ��һ����Ϣ��������Ч,�ܿ��ᱻɾ��,���Ի����ϲ����á�
������FromHandleȻ��һֱ��������



2 CBitmapת��HBITMAP
������
CBitmap*   pBmp;
HBITMAP   hBmp   =   (HBITMAP)(*pBmp);//(HBITMAP)��CBitmap�������˵Ĳ�����.
������

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