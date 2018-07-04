#pragma once
#include "afxwin.h"


// CDeviceSettingDlgTime dialog

class CDeviceSettingDlgTime : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceSettingDlgTime)

public:
	CDeviceSettingDlgTime(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeviceSettingDlgTime();
	void SetParam(int v1, string v2);
	void *m_pParent;


// Dialog Data
	enum { IDD = IDD_DEVICE_SETTING_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	CComboBox m_list_timezone;
	CString m_time_svr;
};
