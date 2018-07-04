#pragma once


// CDeviceSettingDlgImage dialog

class CDeviceSettingDlgImage : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceSettingDlgImage)

public:
	CDeviceSettingDlgImage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeviceSettingDlgImage();
	void SetParam(int v1, int v2, int v3, int v4, int v5, int v6);
	void *m_pParent;

// Dialog Data
	enum { IDD = IDD_DEVICE_SETTING_IMG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	CSliderCtrl m_silder_liangdu;//brightness
	CSliderCtrl n_silder_duibidu;//contrast
	CSliderCtrl n_silder_baohedu;//saturation
	CSliderCtrl n_silder_ruidu;//sharpness
	CSliderCtrl n_silder_secai;//hue
	CComboBox m_ListEncMainFanzhuan;
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
