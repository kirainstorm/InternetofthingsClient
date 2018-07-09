#pragma once

#include <atlimage.h>
#include "BmpButton.h"
#include "BCMenu.h"
#include "BtnST.h"
#include "afxwin.h"
#include "VideoViewCtrlButtonDlg.h"
#include "PictureEx.h"


#define WM_VIDEO_OPEN_CLOSE (WM_USER+2000)
#define WM_VIDEO_SELECT_ITEM (WM_USER+2001)


class CVideoViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoViewDlg)

public:
	CVideoViewDlg(CWnd* pParent = NULL);
	virtual ~CVideoViewDlg();
public:
	CXClientStreamInterface *m_pStreamObject;
	stDevice m_stDevice;
	//
	int XViewOpenStream(stDevice _stDevice);
	int XViewCloseStream();
	int XViewSound(int nStart);
	int XViewTalk(int nStart);
	int XViewCapturePictures();
	int XViewRecord(int nStart);
public:
	//
	//CVideoViewPtzDlg *m_pPtzDlg;
	//BOOL m_bShowPtzDlg;
	//
	CVideoViewCtrlButton *m_pCtrlPanDlg;
	//BOOL m_bShowPanDlg;
	//-
	void ParamSet();
	void XOnMove();
private:
	void SetCtrlPanPos();
public:

public:
	BOOL m_bThreadExit;
	HANDLE m_hThread;
	void CheckStatus();
	void ShowLoadingGif(BOOL bTrue);
public:
	int m_nIndex;
	BOOL m_bFoucs;
	BOOL m_bShowMax;
	void *m_pParent;
	//BOOL m_bInit;
	//void* UserParam;

	CWnd * m_pAfxMainWnd;//CPlatClientDlg

private:
	//BOOL m_bLbtnDown;
	//BOOL m_bDragging;
	//CImageList* m_pDragImage;
public:
	//DWORD m_dwMouseMoveTick;
	//BOOL m_bPreviewState;//////当前窗口是否处于观看视频状态
	//BOOL m_bMouseMove;
	//unsigned char m_nMouseMoveCount;
	BOOL m_bCanShowCtrlPan;//是否显示控制条
public:
	void drawSelected();
	void SetTitle(const char *szTitle);

	BOOL m_bVideoError;
	void SetVideoError(const char *szTitle);

	int DrawFrameRect(void);
	BOOL GetRecordStatus();


	BOOL m_bShowLoading;

public:
	enum { IDD = IDD_VIDEO_VIEW };
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