#pragma once
#include "afxwin.h"


// CTitleDlg dialog
enum MyEnumPanType
{
	EM_PAN_TYPE_LIVE = 0,
	EM_PAN_TYPE_LOOPPLAY,

	//
	//
	//EM_PAN_TYPE_REC,
	//EM_PAN_TYPE_JPG,
	EM_PAN_TYPE_MAP,
	//EM_PAN_TYPE_LOG,
};
class CTitleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTitleDlg)
public:
	CTitleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTitleDlg();
public:
	void * m_pParent;
	void ShowLivePanViewNumber(int n);
	void ShowPan(MyEnumPanType _type);


public:
	// Dialog Data
	enum { IDD = IDD_DIALOG_TITLE };

	void *m_parentDlg;
	void SetParentDlg(void *p)
	{
		m_parentDlg = p;
	};

	void InitFrameString();
	//bool m_rt;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Ö§³Ö

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedButtonClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBmpButton m_min;
	CBmpButton m_max;
	CBmpButton m_close;

	virtual BOOL OnInitDialog();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonMin();
	afx_msg void OnBnClickedButtonMax();
	virtual void PostNcDestroy();

	HBRUSH  brush;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	CBmpButton m_dropdown;
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CStatic m_title;
// 	afx_msg void OnScreenOne();
// 	afx_msg void OnScreenFoure();
// 	afx_msg void OnScreenNine();
// 	afx_msg void OnScreenSixteen();
// 	afx_msg void On32815();
// 	afx_msg void On32816();
// 	afx_msg void On32817Search();
	CBmpButton m_titile_live;
	CBmpButton m_titile_loopplay;
	CBmpButton m_titile_map;
	//
	CBmpButton m_titile_sc_1;
	CBmpButton m_titile_sc_4;
	CBmpButton m_titile_sc_6;
	CBmpButton m_titile_sc_8;
	CBmpButton m_titile_sc_9;
	CBmpButton m_titile_sc_16;
	CBmpButton m_titile_lock;
	CBmpButton m_titile_fullscreen;
	afx_msg void OnBnClickedBtnTitleFullsc();
	afx_msg void OnBnClickedBtnTitleLock();
	afx_msg void OnBnClickedBtnTitleMlog();
	//
	afx_msg void OnBnClickedBtnTitleSc1();
	afx_msg void OnBnClickedBtnTitleSc4();
	afx_msg void OnBnClickedBtnTitleSc6();
	afx_msg void OnBnClickedBtnTitleSc8();
	afx_msg void OnBnClickedBtnTitleSc9();
	afx_msg void OnBnClickedBtnTitleSc16();
	//
	afx_msg void OnBnClickedBtnTitleLive();
	//afx_msg void OnBnClickedBtnTitleRec();
	//afx_msg void OnBnClickedBtnTitleJpg();
	afx_msg void OnBnClickedBtnTitleMap();
	afx_msg void OnBnClickedBtnTitleLooplay();
	//
	afx_msg void OnBnClickedBtnTitleLb1();
	afx_msg void OnBnClickedBtnTitleLb2();
	CBmpButton m_lunboStart;
	CBmpButton m_lunboPause;
	CBmpButton m_logbtn;

	afx_msg void OnBnClickedDropDown();
};
