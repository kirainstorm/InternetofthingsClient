
// XTransmisConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XTransmisConfig.h"
#include "XTransmisConfigDlg.h"
#include "afxdialogex.h"
#include "ConfigEdit.h"
#include "ConfigFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXTransmisConfigDlg dialog



CXTransmisConfigDlg::CXTransmisConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CXTransmisConfigDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXTransmisConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON2, m_btnStartstop);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CXTransmisConfigDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CXTransmisConfigDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CXTransmisConfigDlg::OnBnClickedButton2)
	ON_LBN_DBLCLK(IDC_LIST1, &CXTransmisConfigDlg::OnLbnDblclkList1)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CXTransmisConfigDlg message handlers

// #define SUWEMJ_LEN (8 * 1024)
// #define SSSYYY_HEAD "XTransmisConfig\r\n"
// 
// void run_time_thread(void * param)
// {
// 	CXTransmisConfigDlg *p = (CXTransmisConfigDlg *)param;
// 
// 	CConfigFile::Instance().ReadConfigFile();//ֻ��ֹͣ�����޸ģ���������ֻ��ȡһ�ξͿ�����
// 
// 	//char  * pszTmp = new char[SUWEMJ_LEN];
// 	char  pszTmp[SUWEMJ_LEN] = { 0 };
// 
// 
// 	while (FALSE == p->m_bThreadExit)
// 	{
// 		memset(pszTmp, 0, SUWEMJ_LEN);
// 		memcpy(pszTmp, SSSYYY_HEAD, strlen(SSSYYY_HEAD));
// 		int nSeekStart = strlen(SSSYYY_HEAD);
// 
// 
// 		for (int i = 0; i < MAX_LIVE_STRUCT; i++) //ѭ��MAX_LIVE_STRUCT����¼
// 		{
// 			if (0 != CConfigFile::Instance().m_stAllConfig.all[i].nPlatformID)//�����жϸ�����¼�ǲ����Ѿ����ã�Ҫ����
// 			{
// 				for (int k = 0; k < 6; k++)//����6����¼���ҵ�һ������ʵ�
// 				{
// 
// 					if (0 != CConfigFile::Instance().m_stAllConfig.all[i].dev[k].dev_id) //��ǰ��¼������
// 					{
// 
// 						SYSTEMTIME   sysTime;
// 						GetLocalTime(&sysTime);
// 						int nLocalMinute = sysTime.wHour * 60 + sysTime.wMinute; //��ǰʱ����һ���еķ�����
// 
// 
// 						int nDevStartMinute = CConfigFile::Instance().m_stAllConfig.all[i].dev[k].time_start * 60;
// 						int nEndMinute = CConfigFile::Instance().m_stAllConfig.all[i].dev[k].time_end * 60;
// 
// 
// 						if ((nLocalMinute >= nDevStartMinute) && (nLocalMinute <= nEndMinute))
// 						{
// 							//�ҵ���һ�����ʵļ�¼
// 
// 							char tmp[1024] = { 0 };
// 
// 							sprintf_s(tmp, "#0#127.0.0.1#8000#%d#13800138000#13800138000#0#120.76.217.18#8002#%d\r\n",
// 								CConfigFile::Instance().m_stAllConfig.all[i].dev[k].dev_id,
// 								CConfigFile::Instance().m_stAllConfig.all[i].nPlatformID);
// 
// 
// // 							sprintf_s(tmp, "#0#127.0.0.1#8000#%d#13800138000#13800138000#0#192.168.1.186#8002#%d\r\n",
// // 								CConfigFile::Instance().m_stAllConfig.all[i].dev[k].dev_id,
// // 								CConfigFile::Instance().m_stAllConfig.all[i].nPlatformID);
// 
// 							memcpy(pszTmp + nSeekStart, tmp, strlen(tmp));
// 							nSeekStart += strlen(tmp);
// 
// 
// 
// 							break;//�˳�ȥ��һ���˳�ȥ�������Ĳ�����
// 						}
// 
// 					}
// 				}
// 
// 
// 			}
// 		}
// 
// 		CConfigFile::Instance().WriteTransmisSeverInfo(pszTmp);
// 
// 
// 
// 
// 		Sleep(500);
// 	}
// 
// 
// 
// 	TRACE(L"void run_time_thread(void * param)   ..... exit \n");
// }

BOOL CXTransmisConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_list._SetColor(239, 239, 239, 239, 239, 239, 66, 0, 0);
// 	m_bStarting = FALSE;
// 	m_hTread = NULL;
	SetWindowText(L"�������ù���");




	CConfigFile::Instance().ReadConfigFile();


	for (int i = 0; i < MAX_LIVE_STRUCT; i++)
	{
		if (0 == CConfigFile::Instance().m_stAllConfig.all[i].nPlatformID)
		{
			m_list.AddString(TEXT_NEED_ID);
		}
		else
		{
			CString str;
			str.Format(L"%s%d", TEXT_HAS_ID, CConfigFile::Instance().m_stAllConfig.all[i].nPlatformID);
			m_list.AddString(str);
		}
	}

	m_list.SetCurSel(0);



	//m_list.AddString(L"aaaaa");
	//m_list.AddString(L"bbbbbbbbbb");
	//m_list.SetCurSel(0);



	// 	for (int i = 0; i < 10*10000; i++)
	// 	{
	// 		CConfigFile::Instance().WriteTransmisSeverInfo("hhhhhhhhh\r\n#1#192.168.7.111#8000#9#13800138000#13800138000#0#10.30.0.18#1935#9999");
	// 		//Sleep(1);
	// 	}





	// 	HANDLE hSemaphore1 = CreateSemaphore(NULL, 1, 1, L"Global\\WriteTransSeverSemaphore");
	// 	//WaitForSingleObject(hSemaphore1, INFINITE);
	// 	//ReleaseSemaphore(hSemaphore1, 1, NULL);
	// 
	//  	HANDLE semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, _T("Global\\WriteTransSeverSemaphore"));
	// 
	// 
	// 
	// 	CloseHandle(semaphore);
	// 	CloseHandle(hSemaphore1);
	//	HANDLE hSemaphore2 = CreateSemaphore(NULL, 1, 1, L"Global\\WriteTransSeverSemaphore");
	//	HANDLE hSemaphore3 = CreateSemaphore(NULL, 1, 1, L"Global\\WriteTransSeverSemaphore");


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXTransmisConfigDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXTransmisConfigDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CXTransmisConfigDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_KEYDOWN == pMsg->message)
	{
		UINT nKey = (int)pMsg->wParam;
		if (VK_RETURN == nKey){
			return TRUE;
		}

		if (VK_ESCAPE == nKey){
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CXTransmisConfigDlg::OnBnClickedButton1()
{
	// 	int k = 0;
	// 	for (int i = 0; i < MAX_LIVE_STRUCT; i++)
	// 	{
	// 		if (1 == CConfigFile::Instance().m_stAllConfig.all[i].bUsed)
	// 		{
	// 			k++;
	// 		}
	// 	}
	// 
	// 	if (k > 2)
	// 	{
	// 		AfxMessageBox(L"��ʱֻ֧��2·����");
	// 		return;
	// 	}
	// 
	// 
	// 	CConfigEdit dlg;
	// 	dlg.m_nIndex = m_list.GetCount();
	// 	dlg.m_bMYENUM = CFG_ENUM_FOR_ADD;
	// 	if (dlg.DoModal() == IDOK)
	// 	{
	// 		m_list.ResetContent();
	// 		int k = 0;
	// 		for (int i = 0; i < MAX_LIVE_STRUCT; i++)
	// 		{
	// 			if (1 == CConfigFile::Instance().m_stAllConfig.all[i].bUsed)
	// 			{
	// 				//k++;
	// 				m_list.AddString(L"aaaaa");
	// 			}
	// 		}
	// 	}


}


void CXTransmisConfigDlg::OnBnClickedButton2()
{
// 	m_btnStartstop.EnableWindow(FALSE);
// 	m_btnStartstop.SetWindowText(!m_bStarting ? L"ֹͣ" : L"����");
// 	SetWindowText(m_bStarting ? L"��ֹͣ" : L"����������������......");
// 	m_list.SetFocus();
// 
// 	m_bStarting = !m_bStarting;
// 
// 
// 	if (m_bStarting)
// 	{
// 		
// 		CMenu *pSysMenu = GetSystemMenu(FALSE);
// 		pSysMenu->EnableMenuItem(SC_CLOSE, MF_DISABLED);
// 		OpenThread();
// 	}
// 	else
// 	{
// 		CloseThread();
// 		CMenu *pSysMenu = GetSystemMenu(FALSE);
// 		pSysMenu->EnableMenuItem(SC_CLOSE, MF_ENABLED);
// 	}
// 
// 	SetTimer(1, 300, NULL);
}


void CXTransmisConfigDlg::OnLbnDblclkList1()
{

	int k = m_list.GetCurSel();

	if (-1 == k)
	{
		return;
	}

	CConfigEdit dlg;
	dlg.m_nIndex = k;

// 	if (m_bStarting)
// 	{
// 		dlg.m_bMYENUM = CFG_ENUM_FOR_LOOK;
// 	}
// 	else
	{
		dlg.m_bMYENUM = CFG_ENUM_FOR_MODIFY;
	}
	dlg.DoModal();


	m_list.ResetContent();

	CConfigFile::Instance().ReadConfigFile();


	for (int i = 0; i < MAX_LIVE_STRUCT; i++)
	{
		if (0 == CConfigFile::Instance().m_stAllConfig.all[i].nPlatformID)
		{
			m_list.AddString(TEXT_NEED_ID);
		}
		else
		{
			CString str;
			str.Format(L"%s%d", TEXT_HAS_ID, CConfigFile::Instance().m_stAllConfig.all[i].nPlatformID);
			m_list.AddString(str);
		}
	}

}




void CXTransmisConfigDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
// 	KillTimer(1);
// 	m_btnStartstop.EnableWindow(TRUE);
// 	m_btnStartstop.SetState(FALSE);
	CDialogEx::OnTimer(nIDEvent);
}


void CXTransmisConfigDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
/*	CloseThread();*/
	CDialogEx::OnClose();
}
// 
// void CXTransmisConfigDlg::OpenThread()
// {
// 	if (NULL == m_hTread)
// 	{
// 		m_bThreadExit = FALSE;
// 		m_hTread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)run_time_thread, this, NULL, NULL);
// 	}
// 
// }
// void CXTransmisConfigDlg::CloseThread()
// {
// 	if (m_hTread)
// 	{
// 		m_bThreadExit = TRUE;
// 		WaitForSingleObject(m_hTread, INFINITE);
// 		CloseHandle(m_hTread);
// 		m_hTread = NULL;
// 	}
// 
// }