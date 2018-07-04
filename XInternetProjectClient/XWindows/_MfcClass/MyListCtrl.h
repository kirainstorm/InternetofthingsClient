#pragma once
#include "afxcmn.h"
class CMyListCtrl :
	public CListCtrl
{
public:
	CMyListCtrl();
	~CMyListCtrl();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};

