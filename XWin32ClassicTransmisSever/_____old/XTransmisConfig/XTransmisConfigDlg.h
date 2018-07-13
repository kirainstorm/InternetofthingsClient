
// XTransmisConfigDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "MyListBox.h"
#include "WinXPButtonST.h"


#define MAX_POST_THREAD 16




// CXTransmisConfigDlg dialog
class CXTransmisConfigDlg : public CDialogEx
{
// Construction
public:
	CXTransmisConfigDlg(CWnd* pParent = NULL);	// standard constructor

public:
	//BOOL m_bStarting;

	//BOOL m_bThreadExit;
	//HANDLE m_hTread;
	//void OpenThread();
	//void CloseThread();

public:
// Dialog Data
	enum { IDD = IDD_XTRANSMISCONFIG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CWinXPButtonST m_btnStartstop;
	CMyListBox m_list;
	afx_msg void OnLbnDblclkList1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
};
