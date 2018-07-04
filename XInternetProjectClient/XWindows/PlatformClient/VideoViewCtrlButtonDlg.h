#pragma once


// CVideoViewPanDlg dialog

#include "afxwin.h"


class CVideoViewCtrlButton : public CDialogEx
{
	DECLARE_DYNAMIC(CVideoViewCtrlButton)

public:
	CVideoViewCtrlButton(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVideoViewCtrlButton();
public:
	BOOL m_bAudio;
	BOOL m_bTalk;
	BOOL m_bRecord;
	void * m_pVideoView;
public:
	void inilg();


public:
// Dialog Data
	enum { IDD = IDD_VIDEO_VIEW_PAN };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	public:
	CStatic m_title;
	CBmpButton m_btn_close;
	afx_msg void OnBnClickedBtnClose();
	CBmpButton m_btn_cap;
	afx_msg void OnBnClickedBtnCap();
	CBmpButton m_btn_sound;
	afx_msg void OnBnClickedBtnSound();
	CBmpButton m_btn_talk;
	afx_msg void OnBnClickedBtnTalk();
	CBmpButton m_btn_record;
	afx_msg void OnBnClickedBtnRecord();
	CBmpButton m_btnSet;
	afx_msg void OnBnClickedBtnParam();
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
