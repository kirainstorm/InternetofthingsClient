#pragma once

#include "VideoViewDlg.h"

class CVideoManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoManageDlg)

public:
	CVideoManageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVideoManageDlg();

public:
	BOOL InitPanlLiveWnd(CWnd *pParent);
	void XOnMove(int nIndex){ m_pVideoWindows[nIndex]->XOnMove(); };
	int GetNextOpenWindows();


public:
	CVideoViewDlg * m_pVideoWindows[MAX_WINDOWS_NUM];
	int m_nFocusIndex;
	int m_nShow;
	CWnd * m_pWnd;
	BOOL m_bShowMax;
	void ClearAllSubWindowFoucs();//清除所有窗口选中状态
	void SetFocusViewIndex(int i);//设置当前被选中的窗口索引
	int ShowVideoWindows(int nShowNum = 9);//显示N分割窗口
	int ShowVideoWindowMax(int nInex, BOOL bShowMax);//双击某个窗口最大化显示
public:
	// Dialog Data
	enum { IDD = IDD_VIDEO_MANAGE };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void PostNcDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};
