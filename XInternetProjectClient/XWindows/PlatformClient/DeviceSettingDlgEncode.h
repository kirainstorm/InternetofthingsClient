#pragma once


// CDeviceSettingDlgEncode dialog

class CDeviceSettingDlgEncode : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceSettingDlgEncode)

public:
	CDeviceSettingDlgEncode(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeviceSettingDlgEncode();
	void SetParam(int v1, int v2, int v3, int v4, int v5);
	void *m_pParent;

// Dialog Data
	enum { IDD = IDD_DEVICE_SETTING_ENCODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	CComboBox m_ListEncMainFps;
	CComboBox m_ListEncMainSize;
	CComboBox m_ListEncMainBitrate;
	CComboBox m_ListEncSubBitrate;
	CComboBox m_ListEncSubSize;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnCbnSelchangeCombo7();
};
