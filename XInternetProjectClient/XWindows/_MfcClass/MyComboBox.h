#pragma once
#include "afxwin.h"
class CMyComboBox :
	public CComboBox
{
public:
	CMyComboBox();
	~CMyComboBox();
public:
	void _SetBkColor(int r, int g, int b);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

	

private:
	int m_cBkr, m_cBkg, m_cBkb;



};

