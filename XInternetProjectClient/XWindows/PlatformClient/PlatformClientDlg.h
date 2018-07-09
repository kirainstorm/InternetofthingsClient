
// PlatClientDlg.h : header file
//

#pragma once


#include "afxcmn.h"
#include "afxwin.h"


//
#include "TitleDlg.h"
#include "ZPanLive.h"
#include "VideoLoopplayManageDlg.h"
#include "LogDialog.h"

// CPlatClientDlg dialog
class CPlatClientDlg : public CDialogEx
{
// Construction
public:
	CPlatClientDlg(CWnd* pParent = NULL);	// standard constructor
	//////////////////////////////////////////////////////////////////////////
public:
	void ShowPan(MyEnumPanType _type);
private:
	CImage img;
public:
	BOOL m_fFullScreen;//当前是否全屏
	BOOL m_IsZoomed;//当前是否最大化
	//
	CTitleDlg *m_pTopDlg;//标题菜单栏
	CZPanLive *m_pDlgPanLive;
	CVideoLoopplayManageDlg *m_pDlgLoopplay;
	//CLogDialog * m_pDlgLog;


	//CDlgPanRecord * m_pDlgPanRecord;
	//
	
	//CVideoManageDlg * m_pVideoDlg;//视频窗口
	//
	//CHisOnlineplayDlg * m_pHisPlayer;

	//PLAY_WINDOW_DATA m_wH isData[MAX_HIS_WINDOWS_NUM];
	//CHisVideoManageDlg * m_pHisVideoDlg;//历史视频窗口
	//
//	void ShowCurWindow(emCurDlg n);
public:



public:

// #if COMPANY_4_XJM
// 	void XJMDevAdd();
// #endif
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
public:
	int CPlatClientDlg::DrawFrameRect(void);

// Dialog Data
	enum { IDD = IDD_PLATCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//CButton m_syscfg;
	//CButton m_devcfg;
	//afx_msg void OnBnClickedButtonSysCfg();
	//afx_msg void OnBnClickedButtonDevCfg();
	///CButton m_fullscreen;
	//afx_msg void OnBnClickedButtonFullScreen();

	CrossCriticalSection m_csOpenRealtime;

	afx_msg void OnClose();
	//CListCtrl m_hisList;
	afx_msg void OnMenuDevices();



	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void On32771();
	//afx_msg void OnViewMenuParam();


	void ShowWindowMax(BOOL bShowMax = TRUE);
	//afx_msg void OnViewMenuPtzadv();
	afx_msg void On32789();

	afx_msg void OnMove(int x, int y);
	afx_msg void On32810();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
