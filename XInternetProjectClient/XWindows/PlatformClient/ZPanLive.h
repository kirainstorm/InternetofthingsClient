#pragma once


#include "afxwin.h"
#include "VideoManageDlg.h"
#include "ClassicDeviceTree.h"
#include "IOTDeviceTree.h"
#include "ZPanPtzDlg.h"


class CZPanLive : public CDialogEx
{
	DECLARE_DYNAMIC(CZPanLive)
public:
	//virtual int OnMessageCallback(int nMessageType, int nMessageVal, void * pUser);

public:
	CZPanLive(CWnd* pParent = NULL);   // standard constructor
	virtual ~CZPanLive();

public:
	CWnd *m_parentDlg;
	CVideoManageDlg * m_pVideoDlg;//视频窗口
	CClassicDeviceTree m_tree;
	CZPanPtzDlg *m_pPtzDlg;

public:
	BOOL m_bShowLeftPan;
	int m_nLiveWidth, m_nLiveHeight;//窗口切换按钮大小，便于自动计算停靠位置
	void ShowHideLeftPan(BOOL _show);

public:
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	int XViewOpenStream(stDevice m_stDevice, int ispublish);
	int XViewCloseStream(int nIndex);

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
// Dialog Data
	enum { IDD = IDD_DIALOG_PAN_LIVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedButton1();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CBmpButton m_btnshowhide;
};
