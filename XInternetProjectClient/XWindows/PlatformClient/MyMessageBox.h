#pragma once


// CMyMessageBox dialog
#include "BmpButton.h"

class CMyMessageBox : public CDialogEx
{
	DECLARE_DYNAMIC(CMyMessageBox)
public:
	CString m_strMsg;
	CImage img;
	//HBITMAP m_bitmapBG;//����ͼ
	int m_nCloseWidth, m_nCloseHeight;//�رհ�ť�Ĵ�С�������Զ�����ͣ��λ��

public:
	CMyMessageBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMyMessageBox();

	// Dialog Data
	enum { IDD = IDD_DIALOG_MY_MESSAGEBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	CBmpButton m_close;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButton1();
	CStatic m_MsgStatic;
};
