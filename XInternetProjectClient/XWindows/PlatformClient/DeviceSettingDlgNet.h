#pragma once


// CDeviceSettingDlgNet dialog

class CDeviceSettingDlgNet : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceSettingDlgNet)

public:
	CDeviceSettingDlgNet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeviceSettingDlgNet();
	void SetParam(int v1, string v2);
	void *m_pParent;


// Dialog Data
	enum { IDD = IDD_DEVICE_SETTING_NET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedButton12();
	CButton m_net_dhcp;
	CString m_net_ip;
};
