#pragma once


#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"
#include "MySliderCtrl.h"
// CHisOnlineplayDlg dialog
#include "Mp4WaitDlg.h"
#include "MyListBox.h"
#include "MyButton.h"
#define TIMER_ONLINE_PLAY_XZJD (WM_USER + 77)
#define TIMER_ONLINE_PLAY_BFJD (WM_USER + 76)


class CHisOnlineplayDlg : public CDialogEx
{

public:
	DECLARE_DYNAMIC(CHisOnlineplayDlg)



public:

	void ClearList();
	BOOL OnlineHisPlay();
	BOOL OnlineHisClose();
public:
	int DrawFrameRect(void);
	ST_SREVER_HIS_LOG m_stHisLogInfo;
	stDevice m_stDeviceInfo;
	CXClientDownloadInterface *m_pDownloadInterface;
public:
	HANDLE m_hThreadMp4;
	int GetWiteMp4FileProgress();
	//-------------------------------------------
	static int WorkerThreadRealtime(void*);
	void WorkerRealtime();
	CROSS_THREAD_HANDLE	m_hWorkerThreadRealtime;
	BOOL		m_bWorkerRealtimeStop;
	//------------------------------------------
	//void  ButtonLoadBitmap(CBmpButton &btn, LPCTSTR f1, LPCTSTR f2, LPCTSTR f3, LPCTSTR f4);


public:
	void * m_pParent;
	//int m_nCloseWidth, m_nCloseHeight;//关闭按钮的大小，便于自动计算停靠位置

	//CImage img;
	//HBITMAP m_bitmapBG;//背景图
	

	BOOL m_pPause;


private:
	BOOL m_bPlay;//当前是否正在播放
	BOOL m_bPause;

	CMp4WaitDlg m_waitDlg;

	static int WriteMp4Thread(void * param);

public:
	//
/*	CXClientStreamInterface *m_pDevInteface;*/
	

public:
	CHisOnlineplayDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHisOnlineplayDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_RECORD_ONLINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	
public:
	//CBmpButton m_btnmin;
	//CBmpButton m_btnmax;
	CBmpButton m_btnclose;
	afx_msg LRESULT OnNcHitTest(CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//afx_msg void OnBnClickedButton3();
	//afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	CStatic m_view;
	CBmpButton m_btnpause;
	CBmpButton m_btnSave;

	CMySliderCtrl m_silder;
	CStatic m_timeshow;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnClose();
//	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	virtual void PostNcDestroy();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CBmpButton m_max;
	CBmpButton m_close;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton4();
	CDateTimeCtrl m_ctrlDate;
	CDateTimeCtrl m_ctrlTime;
	CMyListBox m_listbox;
	afx_msg void OnBnClickedButton1();
	CMyButton m_btnSearch;
	afx_msg void OnLbnDblclkList1();
};
