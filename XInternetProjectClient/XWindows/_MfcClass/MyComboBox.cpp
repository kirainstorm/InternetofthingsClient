#include "stdafx.h"
#include "MyComboBox.h"



CMyComboBox::CMyComboBox()
{
	m_cBkr = 233;
	m_cBkg = 233; 
	m_cBkb = 233;
}


CMyComboBox::~CMyComboBox()
{
}
BEGIN_MESSAGE_MAP(CMyComboBox, CComboBox)
	ON_WM_PAINT()
END_MESSAGE_MAP()



void CMyComboBox::_SetBkColor(int r, int g, int b)
{
	m_cBkr = r;
	m_cBkg = g;
	m_cBkb = b;
}

void CMyComboBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CComboBox::OnPaint() for painting messages
 	CRect rcClient;
 	GetClientRect(rcClient);
 	CPen pen(PS_SOLID, 1, RGB(64,64,64));
 	CPen *OldPen = (CPen *)dc.SelectObject(&pen);
	dc.MoveTo(rcClient.left, rcClient.top);
 	dc.LineTo(rcClient.left, rcClient.bottom);
 	dc.MoveTo(rcClient.left, rcClient.bottom);
 	dc.LineTo(rcClient.right, rcClient.bottom);
 	dc.MoveTo(rcClient.right, rcClient.bottom);
 	dc.LineTo(rcClient.right, rcClient.top);
 	dc.MoveTo(rcClient.right, rcClient.top);
 	dc.LineTo(rcClient.left, rcClient.top);

	//清除区域
	CBrush brush;
	//brush.CreateSolidBrush(RGB(theApp.m_cfg.bgr, theApp.m_cfg.bgg, theApp.m_cfg.bgb));
	brush.CreateSolidBrush(RGB(m_cBkr, m_cBkg, m_cBkb));
	dc.FillRect(CRect(rcClient.left, rcClient.top, rcClient.right +1, rcClient.bottom), &brush);
	brush.DeleteObject();

	//写字
	CString strText;
	GetWindowText(strText);
	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);//获取系统默认GUI字体
	HFONT loldfont = (HFONT)(SelectObject(dc, hFont));
	//dc.SetBkColor(RGB(theApp.m_cfg.bgr, theApp.m_cfg.bgg, theApp.m_cfg.bgb));
	dc.SetBkColor(RGB(m_cBkr, m_cBkg, m_cBkb));
	dc.SetTextColor(RGB(255,240,200));
	dc.DrawText(strText, CRect(rcClient.left + 3, rcClient.top + 5, rcClient.right - 3, rcClient.bottom - 3), DT_LEFT);
	SelectObject(dc, loldfont);
	DeleteObject(hFont);
}