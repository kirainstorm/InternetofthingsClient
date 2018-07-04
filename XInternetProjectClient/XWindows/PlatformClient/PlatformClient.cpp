
// PlatClient.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "PlatformClientDlg.h"
#include "HisOnlineplayDlg.h"
#include "LoginDlg.h"


void CSignalingMessageCallback::OnSignalingChannelMessageCallback(const char * jsondata, int datalen)
{
// 	if (NULL == theApp.m_pMainWnd)
// 	{
// 		return;
// 	}
// 
// 	CPlatClientDlg * pMainDlg = (CPlatClientDlg *)theApp.m_pMainWnd;
// 	if (NULL == pMainDlg->m_pDlgLog)
// 	{
// 		return;
// 	}
// 	if (NULL == pMainDlg->m_pDlgLog->m_listCtrl.GetSafeHwnd())
// 	{
// 		return;
// 	}

	TRACE("###### jsondata = %s \n", jsondata);


	//写文件


	//pMainDlg->m_pDlgLog->m_listCtrl.in

	return;
};







#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlatClientApp

BEGIN_MESSAGE_MAP(CPlatClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPlatClientApp construction

CPlatClientApp::CPlatClientApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pSignalingMessageCallback = nullptr;
	m_pClientInferface = nullptr;
	m_ClientType = X_CLIENT_XXX_TYPE_CLASSIC;
}


// The one and only CPlatClientApp object

CPlatClientApp theApp;


// CPlatClientApp initialization

BOOL CPlatClientApp::InitInstance()
{
	//简单内存泄漏检测方法，解决Detected memory leaks!问题
	//_CrtSetBreakAlloc(1754);

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	//--------------------------------------------


	//--------------------------------------------
	//获取语言分类
	char strModulePath[MAX_PATH] = { 0 };
	::GetModuleFileNameA(::GetModuleHandle(nullptr), strModulePath, MAX_PATH);
	std::string str(strModulePath);
	size_t nCount = str.find_last_of("\\");
	if (nCount < 0)
		nCount = str.find_last_of("/");

	if (nCount >= 0)
		str = str.substr(0, nCount + 1);

	str.append("language\\lg.lg");


	char FileName[256] = { 0 };
	memset(FileName, 0, sizeof(FileName));
	strcpy_s(FileName, str.c_str());

	//
	char *pLG = new char[3 * 1024];
	memset(pLG, 0, sizeof(pLG));
	FILE * file;
	fopen_s(&file, FileName, "r");//r 读
	if (file)
	{
		int a = fread(pLG, 1, 3*1024, file);
		pLG[a] = '\0';
		fclose(file);
	}

	WCHAR wszInfo[1024];
	MultiByteToWideChar(CP_UTF8, 0, pLG, strlen(pLG) + 1,
		wszInfo, sizeof(wszInfo) / sizeof(wszInfo[0]));


	delete pLG;



	CString cstringlg = wszInfo;

	CStringArray arr;
	int nIndex = 0; //
	while (1)
	{
		nIndex = cstringlg.Find(_T("\n"));
		if (nIndex >= 0)
		{
			arr.Add(cstringlg.Left(nIndex));
			cstringlg = cstringlg.Right(cstringlg.GetLength() - nIndex - 1);
		}
		else break;
	}
	arr.Add(cstringlg);


	CString CString111 = arr[0];
	CString CString222 = arr[1];


	//////////////////////////////////////////////////////////////////////////
	nIndex = 0; //
	while (1)
	{
		nIndex = CString111.Find(_T(";"));
		if (nIndex >= 0)
		{
			//TRACE(CString111.Left(nIndex));
			//TRACE("\n");

			CString ctrrr = CString111.Left(nIndex) + "\n";
			TRACE(ctrrr);
			m_strlg_string.Add(CString111.Left(nIndex));
			CString111 = CString111.Right(CString111.GetLength() - nIndex - 1);
		}
		else break;

		
	}
	m_strlg_string.Add(CString111);
	TRACE(CString111);
	TRACE("\n");

	//////////////////////////////////////////////////////////////////////////
	nIndex = 0; //
	while (1)
	{
		nIndex = CString222.Find(_T(";"));
		if (nIndex >= 0)
		{
			CString ctrrr = CString222.Left(nIndex) + "\n";
			TRACE(ctrrr);
			m_strlg_filename.Add(CString222.Left(nIndex));
			CString222 = CString222.Right(CString222.GetLength() - nIndex - 1);
		}
		else break;
	}
	m_strlg_filename.Add(CString222);
	TRACE(CString222);
	TRACE("\n");
// 	for (int i = 0; i < nSize; i++)
// 	{
// 		//Parse(arr[i]);
// 	}

	//--------------------------------------------

	memset(&m_clientSetting, 0, sizeof(m_clientSetting));
	CSysFileGetSet::Instance().Get(&m_clientSetting);


	InitLanguage();

	AfxGetApp()->m_pszAppName = _tcsdup(strAppname);

	//--------------------------------------------
	//CClientDB::Instance().SelectConfig(m_cfg);

	//--------------------------------------------
	m_NcHitHeight = 88;//等于标题栏高度
	m_TitleHeight = m_NcHitHeight;//标题栏+logo高度

	m_PanMinWidth = 48;//侧边栏最小宽度
	m_PanMaxWidth = 270;//侧边栏最大宽度

	m_spaceL = 0;//左侧边距
	m_spaceR = 0;//右侧边距
	m_spaceT = 0;//上边边距
	m_spaceB = 0;//下边边距
	m_spaceM = 0;


	//--------------------------------------------
	


	//--------------------------------------------


	//--------------------------------------------
	//m_hcfg = 190; //ptz对话框高度
	//m_hcfg_rec = 300;

	//--------------------------------------------



	//m_nHeightspace = 1;

	
	

	//--------------------------------------------
	//m_titleLeft = 1;


	//由于没有自绘能力，，侧边栏要偏白/灰，，视频窗口偏黑

	// >190
	int xx = (-167);
	m_cfg.bgr = 190 + xx;// 25;
	m_cfg.bgg = 190 + xx;//85;
	m_cfg.bgb = 190 + xx;//137;


	// <70?
	int zz = 20;
	m_cfg.bgvr = 37 + zz;
	m_cfg.bgvg = 37 + zz;
	m_cfg.bgvb = 37 + zz;

	zz = -17;
	m_cfg.bgvtr = 20 + zz;
	m_cfg.bgvtg = 20 + zz;
	m_cfg.bgvtb = 20 + zz;


	//彩色
	m_cfg.bglr = 0;
	m_cfg.bglg = 85;
	m_cfg.bglb = 120;




	//hbrushTitleBg = CreateSolidBrush(RGB(m_cfg.bgrtitle, m_cfg.bggtitle, m_cfg.bgbtitle));

	hbrushDlg = CreateSolidBrush(RGB(m_cfg.bgr, m_cfg.bgg, m_cfg.bgb));//对话框背景色

	hViewTitleBrushBg = CreateSolidBrush(RGB(m_cfg.bgvr, m_cfg.bgvg, m_cfg.bgvb));//视频窗口标题栏背景色

	hViewBrushBg = CreateSolidBrush(RGB(m_cfg.bgvtr, m_cfg.bgvtg, m_cfg.bgvtb));//视频窗口背景色






	img.Load(theApp.m_strSkinPath + L"bg/paper.png");
	m_bitmapBG = img.Detach();
	img.Destroy();
	img.Load(theApp.m_strSkinPath + L"title_bar/close_1.png");
	m_nCloseWidth = img.GetWidth();//<-------------------------------------------------
	m_nCloseHeight = img.GetHeight();//<-------------------------------------------------
	img.Destroy();

	//
	//
	XMediaInferfaceInit();
	m_pSignalingMessageCallback = new CSignalingMessageCallback();
	m_pClientInferface = XMediaCreateClientInferface(m_ClientType);
	///----------------------------------------------------------------------
#if 1
	CLoginDlg LoginDlg;
	if (LoginDlg.DoModal() != IDOK)
	{
		//AfxMessageBox(L"s");
		// Delete the shell manager created above.
		if (pShellManager != NULL)
		{
			delete pShellManager;
		}

		if (m_pClientInferface)
		{
			m_pClientInferface->XDelete();
			m_pClientInferface = nullptr;
		}

		if (m_pSignalingMessageCallback)
		{
			delete m_pSignalingMessageCallback;
			m_pSignalingMessageCallback = nullptr;
		}
		XMediaInferfaceUninit();
		// Since the dialog has been closed, return FALSE so that we exit the
		//  application, rather than start the application's message pump.
		return FALSE;
	}
#endif
	///----------------------------------------------------------------------



	CPlatClientDlg dlg;
	//CHisOnlineplayDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}
	

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	//===========================================
	if (m_pClientInferface)
	{
		m_pClientInferface->XDelete();
		m_pClientInferface = nullptr;
	}
	if (m_pSignalingMessageCallback)
	{
		delete m_pSignalingMessageCallback;
		m_pSignalingMessageCallback = nullptr;
	}
	XMediaInferfaceUninit();
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CPlatClientApp::InitLanguage()
{

	CString strFile = _T("");
	TCHAR szFilePath[MAX_PATH];
	ZeroMemory(szFilePath, MAX_PATH);
	if (GetModuleFileName(NULL, szFilePath, MAX_PATH))
	{
		PathRemoveFileSpec(szFilePath);
	}
	m_strSkinPath = szFilePath;
	m_strSkinPath += _T("\\Skin\\");
	strFile = szFilePath;
	strFile += _T("\\Language\\");

//	wcscat_s(szFilePath, _T("\\Language\\LanguageVer.lg"));

// 	CStdioFile file;
// 	if (!file.Open(szFilePath, CFile::modeRead, NULL))
// 	{
// 		return;
// 	}
// 
// 	CString strText = _T("");
// 	file.ReadString(strText);
// 	file.Close();
// 
// 	int nIndex = strText.Find(_T("="));
// 	CString strValue = strText.Mid(nIndex + 1);
// 	strValue.Trim();
// 	strValue.Trim(_T(";"));

	//0:简体中文
	//1：英文
	//long nValue = 

	//char* oldLocale = _strdup(setlocale(LC_CTYPE, NULL));

// 	switch (m_clientSetting.lg)
// 	{
// 	case 0:
// 		strFile += _T("Simple_Chinese.lg");
// 		//setlocale(LC_CTYPE, "chs");
// 		break;
// 	case 1:
// 		strFile += _T("English.lg");
// 		//setlocale(LC_CTYPE, "enu");
// 		break;
// 	default:
// 		strFile += _T("Simple_Chinese.lg");
// 		//setlocale(LC_CTYPE, "chs");
// 		break;
// 	}
	//  [7/2/2016 kiki]
	strFile += m_strlg_filename[m_clientSetting.lg];
	//  [7/2/2016 kiki]

	CLanguageIni& LangIni = CLanguageIni::GetInstance();
	LangIni.LoadFile(strFile);

	//setlocale(LC_CTYPE, oldLocale);
	//free(oldLocale);

	Get_String_From_Language_Ini_File(_T("str_string_company"), strCompany);

	Get_String_From_Language_Ini_File(_T("str_string_appname"), strAppname);
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CLanguageIni::CLanguageIni(void)
{
}


CLanguageIni::~CLanguageIni(void)
{
	m_mapLanguage.clear();
}


CString CLanguageIni::GetText(CString strKey)
{
	CString strValue = _T("");
	map<CString, CString>::iterator iter = m_mapLanguage.find(strKey);
	if (m_mapLanguage.end() != iter)
	{
		strValue = m_mapLanguage[strKey];
	}

	return strValue;
}

// UTF8转换为Unicode字符串，再转本地字符串
//szchar待转换的汉字的utf-8编码字符串
//wszUniCode 转换后的Unicode编码字符串
CString UTF8ToUni(char* szchar)
{
	//先计算转换后的unicode字符串的长度 
	int len;
	len = MultiByteToWideChar(CP_UTF8, 0, szchar, -1, NULL, 0);
	// 分配Unicode字符串缓冲区，并赋0
	wchar_t* wszUniCode = new wchar_t[len + 1];
	memset(wszUniCode, 0, len * 2 + 2);
	// 转换过程
	MultiByteToWideChar(CP_UTF8, 0, szchar, -1, wszUniCode, len);
	//本地化
	CString  str(wszUniCode);
	delete[] wszUniCode;
	return  str;
}
int splitString(CString str, CStringArray& strArray)
{
	strArray.RemoveAll();

	CString strTemp = str;	//此赋值不能少
	int nIndex = 0; //
	while (1)
	{
		nIndex = strTemp.Find(_T("\r\n"));
		if (nIndex >= 0)
		{
			strArray.Add(strTemp.Left(nIndex));
			strTemp = strTemp.Right(strTemp.GetLength() - nIndex - 1);
		}
		else break;
	}
	strArray.Add(strTemp);
	return strArray.GetSize();
}
void CLanguageIni::LoadFile(LPCTSTR lpszPathFile)
{
	m_mapLanguage.clear();

	CFile f;
	f.Open(lpszPathFile, CFile::modeReadWrite, NULL);
	char *s = new char[100 * 1024];
	ZeroMemory(s, 100 * 1024);
	f.Read(s, 100 * 1024);

	CString str = UTF8ToUni(s);

	delete[] s;
	//AfxMessageBox(str);
	//TRACE(s);
	f.Close();

	//str_high='Высокое';

	// 	CStdioFile file;
	// 	if(!file.Open(lpszPathFile, CFile::modeReadWrite, NULL))
	// 	{
	// 		return;
	// 	}
	// 	//Высокое
	// 	CString strContent = _T("");
	// 	while(file.ReadString(strContent))
	// 	{
	// 		TRACE(strContent);
	// 		Parse(strContent);
	// 	}
	// 
	// 	file.Close();

	CStringArray arr;
	int nSize = splitString(str, arr);

	for (int i = 0; i < nSize; i++)
	{
		Parse(arr[i]);
	}
}

void CLanguageIni::Parse(CString& str)
{
	int nIndex = -1;
	nIndex = str.Find('=');
	if (-1 == nIndex)
	{
		return;
	}

	CString strKey = _T("");
	CString strValue = _T("");

	strKey = str.Left(nIndex);
	strValue = str.Mid(nIndex + 1);
	strKey.Trim();
	strValue.Trim();
	strValue.Trim(_T("\'\";"));



	//AfxMessageBox(strValue);
	m_mapLanguage.insert(make_pair(strKey, strValue));
}

CLanguageIni& CLanguageIni::GetInstance()
{
	static CLanguageIni lg;
	return lg;
}

