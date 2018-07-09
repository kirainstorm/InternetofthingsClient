#pragma once


// CDeviceSettingDlgOsd dialog

class CDeviceSettingDlgOsd : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceSettingDlgOsd)

public:
	CDeviceSettingDlgOsd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeviceSettingDlgOsd();
	void SetParam(string osd);
	void *m_pParent;


// Dialog Data
	enum { IDD = IDD_DEVICE_SETTING_OSD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	CString m_osd_name;
};
