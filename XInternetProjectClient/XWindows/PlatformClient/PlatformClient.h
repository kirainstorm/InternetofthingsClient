
// PlatClient.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPlatClientApp:
// See PlatClient.cpp for the implementation of this class
//
#include "../_MediaInterface/MediaInterface.h"
class CSignalingMessageCallback : public CXSignalingChannelMessageCallback
{
public:
	CSignalingMessageCallback(){
		
	};
	~CSignalingMessageCallback(){
		
	};
public:
	virtual void OnSignalingChannelMessageCallback(const char * jsondata, int datalen);
};


class CPlatClientApp : public CWinApp
{
public:
	CPlatClientApp();
	
public:
	emXClientType m_ClientType;
	CXClientInferface * m_pClientInferface;
	CSignalingMessageCallback * m_pSignalingMessageCallback;
public:

	CImage img;
	HBITMAP m_bitmapBG;//����ͼ
	int m_nCloseWidth, m_nCloseHeight;//�رհ�ť�Ĵ�С�������Զ�����ͣ��λ��


	CStringArray m_strlg_filename;
	CStringArray m_strlg_string;

	//int m_src;
	//int m_viewnum;

	CString strCompany;
	CString strAppname;

	CString m_strSkinPath;
	HBRUSH hViewBrushBg, hViewTitleBrushBg, hbrushDlg;



	stConfig m_cfg;
	//---main dialog 
	//
	int m_NcHitHeight;//���ڱ������߶�
	int m_TitleHeight;//������+logo�߶�
	//
	int m_PanMinWidth;//�������С���
	int m_PanMaxWidth;//����������
	int m_spaceL;//���߾�
	int m_spaceR;//�Ҳ�߾�
	int m_spaceT;//�ϱ߱߾�
	int m_spaceB;//�±߱߾�
	int m_spaceM;//�м�߾�



// 	int m_nHeightspace;//�ָ��߸߶�
// 
// 	int m_hcfg;//ptz pan�߶�
// 	int m_hcfg_rec;//rec pan�߶�
// 
// 	//title dlg
// 	int m_titleLeft;//���߾�


	Ssy_Config_Struct m_clientSetting;
	
	string m_strip,m_strName, m_strPwd;
// Overrides
public:
	virtual BOOL InitInstance();

public:
	void InitLanguage();
// Implementation

	DECLARE_MESSAGE_MAP()
};


class CLanguageIni
{
public:
	CLanguageIni(void);
	~CLanguageIni(void);

	static CLanguageIni& GetInstance();

public:
	void LoadFile(LPCTSTR lpszPathFile);
	CString GetText(CString strKey);

private:

	void Parse(CString& str);

private:
	map<CString, CString> m_mapLanguage;


};


extern CPlatClientApp theApp;