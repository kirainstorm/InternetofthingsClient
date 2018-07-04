#pragma once
#include "afxcmn.h"



//透明滑块

class CMySliderCtrl :
	public CSliderCtrl
{
public:
	CMySliderCtrl();
	~CMySliderCtrl();

	//  [8/2/2016 kiki]
public:
	void SetDownLoadProgress(int i);//i百分比 1-100

private:
	HBRUSH m_ProgerssBrush;
	CBrush proBrush;
	int m_nProgress;
	HBRUSH m_allBrush;
	CBrush allBrush;
	//  [8/2/2016 kiki]
// private:
// 	CDC m_dcBk;
// 	CBitmap m_bmpBk;
// 	CBitmap *m_bmpBkOld;
// 	CPen m_penThumb;
// 	CPen m_penThumbLight;
// 	CPen m_penThumbLighter;
// 	CPen m_penThumbDark;
// 	CPen m_penThumbDarker;
// 	COLORREF m_crThumb;
// 	COLORREF m_crThumbLight;
// 	COLORREF m_crThumbLighter;
// 	COLORREF m_crThumbDark;
// 	COLORREF m_crThumbDarker;

protected:
	DECLARE_MESSAGE_MAP()
	//afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};

