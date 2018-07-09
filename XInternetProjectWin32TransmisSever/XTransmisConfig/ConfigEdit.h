#pragma once
#include "afxwin.h"
#include "DBForConfig.h"

// CConfigEdit dialog

enum MyEnumForEdit
{
	CFG_ENUM_FOR_ADD = 0,//��Ч
	CFG_ENUM_FOR_MODIFY,
	CFG_ENUM_FOR_LOOK


};

class CConfigEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigEdit)

public:
	CConfigEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigEdit();

public:
	MyEnumForEdit m_bMYENUM;
	int m_nIndex;//Ҫ��ӵ��������б�����+1��//Ҫ�޸ĵ�����//���������

	ST_LIVE_STRUCT m_liveCfg;

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_list1;
	CComboBox m_list2;
	CComboBox m_list3;
	CComboBox m_list4;
	CComboBox m_list5;
	CComboBox m_list6;
	int m_start1;
	int m_start2;
	int m_start3;
	int m_start4;
	int m_start5;
	int m_start6;

	int m_end1;
	int m_end2;
	int m_end3;
	int m_end4;
	int m_end5;
	int m_end6;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	int m_id;
};
