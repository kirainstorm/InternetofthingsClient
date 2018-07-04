#pragma once
#include "afxwin.h"


// CLoginDlg dialog

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoginDlg();

public:
	BOOL bCanLogin;
	HANDLE m_hLoginThread;
	void _Login();
	void InitFrameString();//≥ı ºªØ”Ô—‘


public:
	// Dialog Data
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	CEdit m_editUser;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CEdit m_editPwd;
	CEdit m_editIP;
	afx_msg void OnPaint();
	CBmpButton m_close;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButton1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatic m_title;
	CComboBox m_cbLg;
	afx_msg void OnCbnSelchangeCombo1();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CButton m_checkpwd;
	CBmpButton m_helpName;
	CButton m_btncancel;
	CButton m_btnlogin;
};
