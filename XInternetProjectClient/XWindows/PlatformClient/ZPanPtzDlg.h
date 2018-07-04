#pragma once
#include "afxwin.h"
// #include "PlatformClientDlg.h"

// CPtzAdvDlg dialog

class CZPanPtzDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CZPanPtzDlg)
	 
public:
	CZPanPtzDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CZPanPtzDlg();

// 	void SetDeviceInfo(stDevice st);
// 
// 	stDevice m_st;

	//CXClientStreamInterface* m_pDevObject;
	//int m_id;

// 	CImage img;
// 	HBITMAP m_bitmapBG;//±³¾°Í¼
public:
// Dialog Data
	enum { IDD = IDD_DIALOG_PAN_LIVE_PTZ };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CComboBox m_cbpreset;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);



	//-----------------------------------
	CBmpButton m_ptz_leftup;
	CBmpButton m_ptz_up;
	CBmpButton m_ptz_rightup;

	CBmpButton m_ptz_left;
	CBmpButton m_ptz_center;
	CBmpButton m_ptz_right;

	CBmpButton m_ptz_leftdown;
	CBmpButton m_ptz_dowm;
	CBmpButton m_ptz_rightdown;

	CBmpButton m_btn_bb1;
	CBmpButton m_btn_bb2;
	CBmpButton m_btn_jj1;
	CBmpButton m_btn_jj2;
	CBmpButton m_btn_gq1;
	CBmpButton m_btn_gq2;
	CStatic m_static1;
	CStatic m_static2;
	CStatic m_static3;
	//-------------------------------------
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void PostNcDestroy();
	afx_msg void OnPaint();
	CBmpButton m_btn_reset;
};
