#pragma once

#include "MyScrollBarEx.h"
#include "DeviceSettingDlg.h"
#define  AREA_ID_PPPPP 400000000
class CClassicDeviceTree : public CTreeCtrl
{
public:
	CClassicDeviceTree();

public:
	//
	
	//
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
	virtual ~CClassicDeviceTree();
protected:
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void PreSubclassWindow();
	afx_msg void OnPaint();
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClassictreeOpen();
	afx_msg void OnClassictreeLocalrec();
	afx_msg void OnClassictreeServerrec();
	afx_msg void OnClassictreeLocalpic();
	afx_msg void OnClassictreeDeviceset();
	afx_msg void OnClassictreeLoginfo();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};