#if !defined(AFX_VMCUBASESCROLLBAR_H__EB442742_EC74_4F6F_94FA_7833E431C59A__INCLUDED_)
#define AFX_VMCUBASESCROLLBAR_H__EB442742_EC74_4F6F_94FA_7833E431C59A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScrollBarEx.h : header file
//
//Author: Nobita
//Date: 2010.01.29
//I love Doraemon!!!

/////////////////////////////////////////////////////////////////////////////
// CScrollBarEx window

class CScrollBarEx : public CStatic
{
	// Construction
public:
	CScrollBarEx();

	// Attributes
public:
	BOOL InitScrollBar(CWnd *pCtrl, UINT nStyle);
	void UpdateScrollBar();

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrollBarEx)
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CScrollBarEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CScrollBarEx)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CWnd* m_pCtrl;
	UINT m_nScrollStyle;
private:
	void DrawVScroll(CDC* pDC);
	void DrawHScroll(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VMCUBASESCROLLBAR_H__EB442742_EC74_4F6F_94FA_7833E431C59A__INCLUDED_)