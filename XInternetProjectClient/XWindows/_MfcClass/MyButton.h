#pragma once
#include "afxwin.h"

// CMyButton

/*
http://bbs.csdn.net/topics/390336507

实现方法
1.给按钮加上BS_OWNERDRAW样式
2.重载DrawItem函数, 在这里绘制按钮
3.关键之处就是把父窗口的背景复制到按钮上, 实现视觉上的透明
4.最后通过AlphaBlend实现半透明
*/


class CMyButton : public CButton
{
	DECLARE_DYNAMIC(CMyButton)

public:
	CMyButton();
	virtual ~CMyButton();
public:
	void SetBkColor(COLORREF color);
	void SetTextColor(COLORREF color);
private:
	bool m_bOver;
	bool m_bDown;
	bool m_bDisable;
	BOOL m_bTracking;
	COLORREF m_bkColor;
	COLORREF m_textColor;
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnable(BOOL bEnable);
private:
	void ButtonInit();
	void DrawButton();
	void DrawButton(HDC hDestDC);
};
