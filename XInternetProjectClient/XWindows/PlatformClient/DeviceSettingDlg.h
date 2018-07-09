#pragma once
#include "afxcmn.h"


// CDeviceSettingDlg dialog
#include "DeviceSettingDlgEncode.h"
#include "DeviceSettingDlgImage.h"
#include "DeviceSettingDlgNet.h"
#include "DeviceSettingDlgOsd.h"
#include "DeviceSettingDlgPlatform.h"
#include "DeviceSettingDlgTime.h"
#include "DeviceSettingDlgWifi.h"
#include "afxwin.h"


class CDeviceSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceSettingDlg)

public:
	CDeviceSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeviceSettingDlg();

public:
	stDevice m_st;

public:
	CDeviceSettingDlgEncode * m_pDlgEnc;
	CDeviceSettingDlgImage * m_pDlgImage;
	CDeviceSettingDlgNet * m_pDlgNet;
	CDeviceSettingDlgOsd * m_pDlgOsd;
	CDeviceSettingDlgPlatform * m_pDlgPlatform;
	CDeviceSettingDlgTime * m_pDlgTime;
	CDeviceSettingDlgWifi * m_pDlgWifi;
	//
	int m_nSelItem;
	void ShowPage(int index);


// Dialog Data
	enum { IDD = IDD_DEVICE_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabParam;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();
	virtual void PostNcDestroy();
	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	CBmpButton m_close;
	afx_msg void OnBnClickedButton1();
};
