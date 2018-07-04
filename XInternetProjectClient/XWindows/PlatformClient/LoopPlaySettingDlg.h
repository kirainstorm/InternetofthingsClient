#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CLoopPlaySettingDlg dialog

class CLoopPlaySettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoopPlaySettingDlg)

public:
	CLoopPlaySettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoopPlaySettingDlg();


	//void UpdateDevList();

	void UpdataTreeItem();
	void TravelSelectedItem(CTreeCtrl *pTreeCtrl, HTREEITEM hItem);
	//
	HTREEITEM GetWantSelectedTreeItemByItemData(int v);
	HTREEITEM GetWantSelectedTreeItemByItemDataTravelSelectedItem(CTreeCtrl *pTreeCtrl, HTREEITEM hItem, int v);
	//
	CImageList m_FileViewImages;
	//
	void  SetItemCheckState(HTREEITEM item, BOOL bCheck);
	void  SetChildCheck(HTREEITEM item, BOOL bCheck);
	void  SetParentCheck(HTREEITEM item, BOOL bCheck);



public:

// Dialog Data
	enum { IDD = IDD_VIDEO_LOOPPLAY_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_pTreeDev;
	CComboBox m_spitewnd;
	CComboBox m_looptime;
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	CBmpButton m_close;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
