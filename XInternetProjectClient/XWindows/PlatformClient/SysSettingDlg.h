#pragma once


#include "afxwin.h"

// CSysSettingDlg dialog

class CSysSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSysSettingDlg)

public:
	CSysSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSysSettingDlg();

	void * m_pParent;
	int m_nCloseWidth, m_nCloseHeight;//关闭按钮的大小，便于自动计算停靠位置

	CImage img;
	//HBITMAP m_bitmapBG;//背景图

	//CString m_picPath;
	//CString m_recPath;

public:
// Dialog Data
	enum { IDD = IDD_SYS_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBmpButton m_close;
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedBtnPicSel();
	afx_msg void OnBnClickedBtnRecSel();
	afx_msg void OnBnClickedBtnPicLook();
	afx_msg void OnBnClickedBtnRecLook();
	CEdit m_editPic;
	CEdit m_editRecLocal;
	CComboBox m_ccScreen;
	//CComboBox m_reclLen;
	//CComboBox m_recTime;
	//CComboBox m_recSpace;
	//CButton m_cover;
};
