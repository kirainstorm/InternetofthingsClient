#pragma once
#include "afxwin.h"


//请注意clistbox框owner draw要改成 Variable------sort：false ---- has  string :true

//如果ResetContent闪烁的话，请在所占其父窗口区域内的OnCtlColor返回与之相同的背景色

class CMyListBox :
	public CListBox
{
public:
	CMyListBox();
	~CMyListBox();

public:
	void _SetColor(int bgr, int bgg, int bgb, int itr, int itg, int itb ,int fr,int fg,int fb);

private:
	int m_bgr, m_bgg, m_bgb, m_itr, m_itg, m_itb, m_fr, m_fg, m_fb;
private:
	COLORREF m_clrFore;//前景色
	COLORREF m_clrBack;//背景色
	CBrush m_brush;//背景画刷

public:
	DECLARE_MESSAGE_MAP()
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};

