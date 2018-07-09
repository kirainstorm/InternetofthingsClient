#pragma once
#include "afxwin.h"

// CMyButton

/*
http://bbs.csdn.net/topics/390336507

ʵ�ַ���
1.����ť����BS_OWNERDRAW��ʽ
2.����DrawItem����, ��������ư�ť
3.�ؼ�֮�����ǰѸ����ڵı������Ƶ���ť��, ʵ���Ӿ��ϵ�͸��
4.���ͨ��AlphaBlendʵ�ְ�͸��
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
