#pragma once
#include "afxwin.h"

#include "MyListBox.h"
// CHisLocalplayDlg dialog

class CHisLocalplayDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHisLocalplayDlg)

public:
	CHisLocalplayDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHisLocalplayDlg();

public:
	stDevice m_stDeviceInfo;
	void SearchHislog_Local();
	void ClearList();


public:
// Dialog Data
	enum { IDD = IDD_DIALOG_RECORD_LOCAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	virtual void PostNcDestroy();
	CStatic m_title;
	CMyListBox m_list;
	CBmpButton m_close;
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLbnDblclkList1();
};
