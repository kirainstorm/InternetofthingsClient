#pragma once


// CDeviceSettingDlgWifi dialog

class CDeviceSettingDlgWifi : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceSettingDlgWifi)

public:
	CDeviceSettingDlgWifi(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeviceSettingDlgWifi();
	void SetParam(string v1, string v2);
	void *m_pParent;


// Dialog Data
	enum { IDD = IDD_DEVICE_SETTING_WIFI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton11();
	CString m_wifi_ssid;
	CString m_wifi_pwd;
};
