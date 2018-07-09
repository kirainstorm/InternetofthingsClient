#pragma once

#include "VideoLoopplayView.h"

/*
CXClientStreamInterface *m_pStreamObject;
stDevice m_stDevice;
//
int XViewOpenStream(stDevice _stDevice);
int XViewCloseStream();
*/
class CVideoLoopplayManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoLoopplayManageDlg)

public:
	CVideoLoopplayManageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVideoLoopplayManageDlg();

public:
	BOOL InitPanlLiveWnd(CWnd *pParent);

public:
	void StartLoopplay(); //开始轮询
	void PauseLoopplay();
	void StopLoopplay(); //结束轮询

private:
	//
	int m_wndNum;
	int m_WaitSecond;
	//woker thread
	HANDLE m_hThread;
	volatile BOOL m_bExit;
	volatile BOOL m_bPause;
	static void ThreadWoker(void * param);
	void Woker();
	//
	vector<stDevice> m_stList;



private:
	CWnd * m_pWnd;

private:
	CVideoLoopplayView * m_pVideoWindows[MAX_WINDOWS_NUM];
	int m_nShow;
	int m_nFocusIndex;
	BOOL m_bShowMax;
	int ShowVideoWindows(int nShowNum = 9);//显示N分割窗口
	int ShowVideoWindowMax(int nInex, BOOL bShowMax);//双击某个窗口最大化显示




public:
	// Dialog Data
	enum { IDD = IDD_VIDEO_LOOPPLAY_MANAGE };
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
