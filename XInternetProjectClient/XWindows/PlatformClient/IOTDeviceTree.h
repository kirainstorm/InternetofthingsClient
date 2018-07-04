#pragma once

#include "MyScrollBarEx.h"

class CIOTDeviceTree : public CTreeCtrl
{
public:
	CIOTDeviceTree();

public:
	CWnd * m_ParentWnd;
	CrossCriticalSection m_csTree;
	HTREEITEM m_hSelectedItem;
	CImageList m_FileViewImages;
	//
	void Init();
	void Deinit();
	void UpdataTreeItem();
	void SetParentWnd(CWnd * parent){ m_ParentWnd = parent; };
	//
	CScrollBarEx m_HScroll;
	CScrollBarEx m_VScroll;
public:
	virtual ~CIOTDeviceTree();
protected:
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void PreSubclassWindow();
	afx_msg void OnPaint();
};