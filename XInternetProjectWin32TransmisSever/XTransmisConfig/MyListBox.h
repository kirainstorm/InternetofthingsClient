#pragma once
#include "afxwin.h"


//��ע��clistbox��owner drawҪ�ĳ� Variable------sort��false ---- has  string :true

//���ResetContent��˸�Ļ���������ռ�丸���������ڵ�OnCtlColor������֮��ͬ�ı���ɫ

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
	COLORREF m_clrFore;//ǰ��ɫ
	COLORREF m_clrBack;//����ɫ
	CBrush m_brush;//������ˢ

public:
	DECLARE_MESSAGE_MAP()
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};

