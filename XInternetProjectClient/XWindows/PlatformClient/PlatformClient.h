
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
	HBITMAP m_bitmapBG;//背景图
	int m_nCloseWidth, m_nCloseHeight;//关闭按钮的大小，便于自动计算停靠位置


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
	int m_NcHitHeight;//等于标题栏高度
	int m_TitleHeight;//标题栏+logo高度
	//
	int m_PanMinWidth;//侧边栏最小宽度
	int m_PanMaxWidth;//侧边栏最大宽度
	int m_spaceL;//左侧边距
	int m_spaceR;//右侧边距
	int m_spaceT;//上边边距
	int m_spaceB;//下边边距
	int m_spaceM;//中间边距



// 	int m_nHeightspace;//分割线高度
// 
// 	int m_hcfg;//ptz pan高度
// 	int m_hcfg_rec;//rec pan高度
// 
// 	//title dlg
// 	int m_titleLeft;//左侧边距


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