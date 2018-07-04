#pragma once


// CDeviceSettingDlgPlatform dialog

class CDeviceSettingDlgPlatform : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceSettingDlgPlatform)

public:
	CDeviceSettingDlgPlatform(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeviceSettingDlgPlatform();
	void SetParam(string v1, string v2);
	void *m_pParent;


// Dialog Data
	enum { IDD = IDD_DEVICE_SETTING_PLATFORM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedButton1();
	CString m_strPlat_ip;
	CString m_strPlat_id;
};
