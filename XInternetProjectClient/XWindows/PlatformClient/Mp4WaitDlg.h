#pragma once
#include "afxwin.h"


// CMp4WaitDlg dialog

class CMp4WaitDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMp4WaitDlg)
public:
	void * m_pMain;
	CString strLoad;
public:
	void Start(){ m_bClose = FALSE; };
	void Stop(){ m_bClose = TRUE; };
private:
	BOOL m_bClose;
public:
	CMp4WaitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMp4WaitDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_WAITMP4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


public:
	CStatic m_info;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
