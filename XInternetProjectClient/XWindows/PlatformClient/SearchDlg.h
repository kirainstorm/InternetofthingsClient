#pragma once


// CSearchDlg dialog
#include "MyListBox.h"
#include "afxwin.h"

class CSearchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchDlg)

public:
	CSearchDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSearchDlg();

public:
	CrossCriticalSection m_csList;
	BOOL m_bCanCheckTimeOut;


public:
	int m_nCloseWidth, m_nCloseHeight;//关闭按钮的大小，便于自动计算停靠位置
	CImage img;
	HBITMAP m_bitmapBG;//背景图

// Dialog Data
	enum { IDD = IDD_DIALOG_SEARCH_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CMyListBox m_list;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton1();
	CBmpButton m_close;
	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnLbnDblclkList1();
};
