#pragma once

#include <atlimage.h>
#include "BmpButton.h"
#include "BCMenu.h"
#include "BtnST.h"
#include "afxwin.h"
#include "PictureEx.h"




class CVideoLoopplayView : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoLoopplayView)

public:
	CVideoLoopplayView(CWnd* pParent = NULL);
	virtual ~CVideoLoopplayView();



public:
	void *m_pAfxMainWnd;
	void StartPreview(stDevice dev);
	void StopPreview();

private:
	//
	stDevice m_stDevice;
	//woker thread
	HANDLE m_hThread;
	volatile BOOL m_bExit;
	static void ThreadWoker(void * param);
	void Woker();
	//
	CXClientStreamInterface *m_pStreamObject;
	int XViewOpenStream();
	int XViewCloseStream();
	//
	void ShowLoadingGif(BOOL bTrue);
	HWND GetSafePlayWnf(){ return m_view.GetSafeHwnd(); }

public:
	enum { IDD = IDD_VIDEO_LOOPPLAY_VIEW };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CStatic m_view;
	virtual void PostNcDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CPictureEx m_loadingview;
};