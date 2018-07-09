#include "stdafx.h"
#include "PlatformClient.h"
#include "PlatformClientDlg.h"
#include "afxdialogex.h"

#include "SysSettingDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CPlatClientApp theApp;
// CAboutDlg dialog used for App About




class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPlatClientDlg dialog



CPlatClientDlg::CPlatClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPlatClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_fFullScreen = FALSE;
}

void CPlatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_TREE_DEVICE, m_tree);
	//DDX_Control(pDX, IDC_BUTTON_SYS_CFG, m_syscfg);
	//DDX_Control(pDX, IDC_BUTTON_DEV_CFG, m_devcfg);
	//DDX_Control(pDX, IDC_BUTTON_FULL_SCREEN, m_fullscreen);
	//DDX_Control(pDX, IDC_LIST_HIS, m_hisList);
}

BEGIN_MESSAGE_MAP(CPlatClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//ON_BN_CLICKED(IDC_BUTTON_SYS_CFG, &CPlatClientDlg::OnBnClickedButtonSysCfg)
	//ON_BN_CLICKED(IDC_BUTTON_DEV_CFG, &CPlatClientDlg::OnBnClickedButtonDevCfg)
	//ON_BN_CLICKED(IDC_BUTTON_FULL_SCREEN, &CPlatClientDlg::OnBnClickedButtonFullScreen)
	ON_WM_CLOSE()
//	ON_COMMAND(ID_MENU_DEVICES, &CPlatClientDlg::OnMenuDevices)
	ON_COMMAND(ID_32771, &CPlatClientDlg::On32771)
	//ON_COMMAND(ID_VIEW_MENU_PARAM, &CPlatClientDlg::OnViewMenuParam)
	//ON_COMMAND(ID_VIEW_MENU_PTZADV, &CPlatClientDlg::OnViewMenuPtzadv)
//	ON_COMMAND(ID_32789, &CPlatClientDlg::On32789)
	ON_WM_MOVE()
//	ON_COMMAND(ID_32810, &CPlatClientDlg::On32810)
	ON_WM_TIMER()
END_MESSAGE_MAP()




// CPlatClientDlg message handlers
void CPlatClientDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	//	DeleteObject(hbrushView);
}

void CPlatClientDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(2254);
	SetThreadExecutionState(ES_CONTINUOUS);


// 	if (m_bitmapBG)
// 	{
// 		CBitmap bmp;
// 		bmp.Attach(m_bitmapBG);
// 		bmp.Detach();
// 		DeleteObject(m_bitmapBG);
// 	}

	CDialogEx::OnClose();
}


BOOL CPlatClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//////////////////////////////////////////////////////////////////////////

	//------------------------------------------------------------------------
	

	SetWindowText(theApp.strCompany);

	//////////////////////////////////////////////////////////////////////////
	m_pTopDlg = new CTitleDlg(this);
	m_pTopDlg->Create(CTitleDlg::IDD, this);
	m_pTopDlg->SetParentDlg(this);
	m_pTopDlg->ShowWindow(SW_SHOW);

	//////////////////////////////////////////////////////////////////////////
	m_pDlgPanLive = new CZPanLive(this);
	m_pDlgPanLive->Create(CZPanLive::IDD, this);
	m_pDlgPanLive->ModifyStyle(0, WS_CHILDWINDOW);
	m_pDlgPanLive->ShowWindow(SW_HIDE);
	//
	m_pDlgLoopplay = new CVideoLoopplayManageDlg(this);
	m_pDlgLoopplay->Create(CVideoLoopplayManageDlg::IDD, this);
	m_pDlgLoopplay->ModifyStyle(0, WS_CHILDWINDOW);
	m_pDlgLoopplay->ShowWindow(SW_HIDE);
	//
// 	m_pDlgLog = new CLogDialog(this);
// 	m_pDlgLog->Create(CLogDialog::IDD, this);
// 	m_pDlgLog->ModifyStyle(0, WS_CHILDWINDOW);
// 	m_pDlgLog->ShowWindow(SW_HIDE);
	//


#if 0
	m_pDlgPanRecord = new CDlgPanRecord(this);
	m_pDlgPanRecord->Create(CDlgPanRecord::IDD, this);
	m_pDlgPanRecord->ShowWindow(SW_HIDE);
#endif



	

	//////////////////////////////////////////////////////////////////////////
// 	m_pHisVideoDlg = new CHisVideoManageDlg;
// 	m_pHisVideoDlg->Create(CHisVideoManageDlg::IDD, this);
// 	m_pHisVideoDlg->Init(this);
// 	m_pHisVideoDlg->ShowWindow(SW_HIDE);
// 	for (int i = 0; i < MAX_HIS_WINDOWS_NUM; i++)
// 	{
// 		ZeroMemory(&m_wHisData[i], sizeof(PLAY_WINDOW_DATA));
// 		m_wHisData[i].bPlay = FALSE;
// 		m_pHisVideoDlg->XSetSubWindowData(i, &m_wHisData[i]);
// 
// 		m_pHisVideoDlg->XEnableSubWindowButtnOfIndex(i, FALSE);
// 	}

// 	m_pPtz = new CDlgPtz(this);
// 	m_pPtz->Create(CDlgPtz::IDD, this);
// 	m_pPtz->ShowWindow(SW_SHOW);
// 
// 	m_pHisPlayer = new CHisOnlineplayDlg(this);
// 	m_pHisPlayer->Create(CHisOnlineplayDlg::IDD, this);
// 	m_pHisPlayer->ShowWindow(SW_HIDE);
	//////////////////////////////////////////////////////////////////////////
	//------------------------------------------------------------------------

// 	if (theApp.m_clientSetting.server_enable == 0)
// 	{
// 		CDatabase::Instance().PersonalSelectAllDevice(g_DevList);//本地浏览，不连接到服务器
// 	}







// 	m_tree.SetParentWnd(this);
// 	m_tree.ModifyStyle(0, TVS_NOHSCROLL);
// 	m_tree.Init();



	//m_devListDlg.DoModal();




	//ShowWindow(SW_MAXIMIZE);


// 	//  [2/15/2016 xjm] ------
// 	m_bShowLeftPan = TRUE;
// 
// 	m_pLeftPan = new CLeftPan(this);
// 	m_pLeftPan->Create(CLeftPan::IDD, this);
// //	m_pLeftPan->SetParentDlg(this);
// 	m_pLeftPan->ShowWindow(SW_SHOW);
// 	//------------------------


	//img.Load(theApp.m_strSkinPath + L"bg/paper.png");
	//m_bitmapBG = img.Detach();






	//m_pPtz->ShowWindow(SW_HIDE);
	//m_tree.ShowWindow(SW_HIDE);




	CRect rtClient;
	GetWindowRect(rtClient);
	rtClient.right = rtClient.left + 1000;
	rtClient.bottom = rtClient.top + 650;
	MoveWindow(rtClient);
	CenterWindow();

	// TODO: Add extra initialization here
	//SendMessage(WM_SIZE, 0, 0);
	Sleep(10);
	PostMessage(WM_SIZE, 0, 0);

	m_pTopDlg->ShowPan(EM_PAN_TYPE_LIVE);

	SetTimer(2254, 3000, NULL);



	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPlatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPlatClientDlg::OnPaint()
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
		CPaintDC dc(this);
		OnPaintBmpBackground(this, dc);
		DrawFrameRect();


		CDialogEx::OnPaint();
	}
}

int CPlatClientDlg::DrawFrameRect(void)
{
	CRect rt;
	GetClientRect(&rt);

	HBRUSH hbrush, hbrushOld;
	hbrush = CreateSolidBrush(RGB(185, 233, 238));


	HDC hdc = ::GetDC(m_hWnd);
	hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);

	FrameRect(hdc, &rt, hbrush);

	SelectObject(hdc, hbrushOld);
	::DeleteObject(hbrush);
	::DeleteObject(hbrushOld);
	::ReleaseDC(m_hWnd, hdc);

	return 0;
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPlatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
// void CPlatClientDlg::ShowHideLeftPan()
// {
// 	m_bShowLeftPan = !m_bShowLeftPan;
// 	SendMessage(WM_SIZE, 0, 0);
// }
// void CPlatClientDlg::ShowCurWindow(emCurDlg n)
// {
// 
// 	this->SendMessage(WM_SIZE, 0, 0);
// 
// 	//m_pVideoDlg->ShowWindow(SW_HIDE);
// 	//m_pHisVideoDlg->ShowWindow(SW_HIDE);
// 	//m_pPtz->ShowWindow(SW_HIDE);
// 	//m_pHisLog->ShowWindow(SW_HIDE);
// 
// //	m_nCurPanIndex = n;
// 	switch (n)
// 	{
// 	case DIALOG_CUR_LIVE:
// 		m_pVideoDlg->ShowWindow(SW_SHOW);
// 		//m_pPtz->ShowWindow(SW_SHOW);
// 		//
// 		m_pHisVideoDlg->ShowWindow(SW_HIDE);
// 		//m_pHisLog->ShowWindow(SW_HIDE);
// 		//
// 		break;
// 
// 
// 	case DIALOG_CUR_HIS:
// 		m_pHisVideoDlg->ShowWindow(SW_SHOW);
// 		//m_pHisLog->ShowWindow(SW_SHOW);
// 		//
// 		m_pVideoDlg->ShowWindow(SW_HIDE);
// 		//m_pPtz->ShowWindow(SW_HIDE);
// 		break;
// // 	case DIALOG_CUR_WALL:
// // 		m_pVideoDlg->ShowWindow(SW_HIDE);
// // 		//m_pPtz->ShowWindow(SW_HIDE);
// // 		//
// // 		m_pHisVideoDlg->ShowWindow(SW_HIDE);
// // 		//m_pHisLog->ShowWindow(SW_HIDE);
// // 
// // 		break;
// // 	case DIALOG_CUR_LOG:
// // 		break;
// // 	case DIALOG_CUR_EMAP:
// // 		break;
// 	default:
// 		break;
// 	}
// 
// 	
// }

void CPlatClientDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if ((GetSafeHwnd() == NULL) ||
		(m_pDlgPanLive->GetSafeHwnd() == NULL)
		)
	{
		return;
	}


	if (!m_fFullScreen)
	{
		CRect rectClient;
		GetClientRect(rectClient);

		if (rectClient.Width() <= 0)
			return;


		int titleHeight = theApp.m_NcHitHeight;//标题高度
// 		int panMinWidth = theApp.m_PanMinWidth;//侧边栏宽度
// 		int panMaxWidth = theApp.m_PanMaxWidth;//侧边栏宽度
		//int leftWidth = 10;
		int spaceL = theApp.m_spaceL;//左侧边距
		int spaceR = theApp.m_spaceR;//右侧边距
		int spaceT = theApp.m_spaceT;//上边边距
		int spaceB = theApp.m_spaceB;//下边边距

//		int spaceM = theApp.m_spaceM;//中间边距
		//int hcfg = theApp.m_hcfg;//pan高度
		//int nHeightspace = theApp.m_nHeightspace;//分割线高度



		m_pTopDlg->SetWindowPos(NULL, 
			0,//spaceL + (m_bShowLeftPan ? (rectClient.left + panMaxWidth) : (rectClient.left + panMinWidth)),
			rectClient.top, 
			rectClient.Width(),// - (m_bShowLeftPan ? panMaxWidth : panMinWidth),
			titleHeight, SWP_NOACTIVATE | SWP_NOZORDER);
		//m_pTopDlg->SendMessage(WM_SIZE, 0, 0);

		
// 		m_pLeftPan->SetWindowPos(NULL, 
// 			spaceL,
// 			rectClient.top + titleHeight + spaceT,
// 			(m_bShowLeftPan ? panMaxWidth : panMinWidth), 
// 			rectClient.Height() - spaceT - spaceB - titleHeight, SWP_NOACTIVATE | SWP_NOZORDER);
		//-----------------------------------------------------------------------------------------------------------------
		//main
		m_pDlgPanLive->SetWindowPos(nullptr,
			rectClient.left,
			rectClient.top + spaceT + titleHeight, 
			rectClient.Width(),
			rectClient.Height() - titleHeight - spaceT - spaceB, SWP_NOACTIVATE | SWP_NOZORDER);
		m_pDlgPanLive->PostMessage(WM_SIZE);


		m_pDlgLoopplay->SetWindowPos(nullptr,
			rectClient.left,
			rectClient.top + spaceT + titleHeight,
			rectClient.Width(),
			rectClient.Height() - titleHeight - spaceT - spaceB, SWP_NOACTIVATE | SWP_NOZORDER);
		m_pDlgLoopplay->PostMessage(WM_SIZE);


// 		m_pDlgLog->SetWindowPos(nullptr,
// 			rectClient.left,
// 			rectClient.top + spaceT + titleHeight,
// 			rectClient.Width(),
// 			rectClient.Height() - titleHeight - spaceT - spaceB, SWP_NOACTIVATE | SWP_NOZORDER);
// 		m_pDlgLog->PostMessage(WM_SIZE);





#if 0
		m_pDlgPanRecord->SetWindowPos(nullptr,
			rectClient.left,
			rectClient.top + spaceT + titleHeight,
			rectClient.Width(),
			rectClient.Height() - titleHeight - spaceT - spaceB, SWP_NOACTIVATE | SWP_NOZORDER);
		m_pDlgPanRecord->PostMessage(WM_SIZE);
#endif





// 		m_pHisPlayer->SetWindowPos(nullptr,
// 			m_bShowLeftPan ? (rectClient.left + panMaxWidth + spaceL) : (rectClient.left + panMinWidth + spaceL),
// 			rectClient.top + spaceT + titleHeight,
// 			rectClient.Width() - (m_bShowLeftPan ? panMaxWidth : panMinWidth) - spaceL - spaceM - spaceR,
// 			rectClient.Height() - titleHeight - spaceT - spaceB, SWP_NOACTIVATE | SWP_NOZORDER);
// 
// 


// 		m_pHisVideoDlg->SetWindowPos(nullptr, 
// 			m_bShowLeftPan ? (rectClient.left + panMaxWidth + spaceL) : (rectClient.left + panMinWidth + spaceL),
// 			rectClient.top + spaceT + titleHeight, 
// 			rectClient.Width() - (m_bShowLeftPan ? panMaxWidth : panMinWidth) - spaceL - spaceM - spaceR,
// 			rectClient.Height() - titleHeight - spaceT - spaceB, SWP_NOACTIVATE | SWP_NOZORDER);




// 
// 		//-----------------------------------------------------------------------------------------------------------------
// 		if (m_pTopDlg->m_curDlg == DIALOG_CUR_LIVE)
// 		{
// 			hcfg = theApp.m_hcfg;//pan高度
// 			//tree
// 			CRect rcTree;
// 			rcTree.left = rectClient.left + (rectClient.Width() - panWidth) - spaceR;
// 			rcTree.right = rcTree.left + panWidth;
// 			rcTree.top = rectClient.top + spaceT + titleHeight;
// 			rcTree.bottom = rcTree.top + rectClient.Height() - hcfg - titleHeight;
// 			m_tree.MoveWindow(rcTree);
// 
// 
// 			CRect rcPtz;
// 			rcPtz.left = rectClient.left + (rectClient.Width() - panWidth) - spaceR;
// 			rcPtz.right = rcPtz.left + panWidth;
// 			rcPtz.top = rectClient.top + (rectClient.Height() - hcfg) + spaceT + nHeightspace;
// 			rcPtz.bottom = rcPtz.top + hcfg - spaceB - spaceT - nHeightspace;
// 			m_pPtz->MoveWindow(rcPtz);
// 		}
// 
// 		//-----------------------------------------------------------------------------------------------------------------
// 		if (m_pTopDlg->m_curDlg == DIALOG_CUR_HIS)
// 		{
// 			hcfg = theApp.m_hcfg_rec;//pan高度
// 			//tree
// 			CRect rcTree;
// 			rcTree.left = rectClient.left + (rectClient.Width() - panWidth) - spaceR;
// 			rcTree.right = rcTree.left + panWidth;
// 			rcTree.top = rectClient.top + spaceT + titleHeight;
// 			rcTree.bottom = rcTree.top + rectClient.Height() - hcfg - titleHeight;
// 			m_tree.MoveWindow(rcTree);
// 
// 
// 			CRect rcPtz;
// 			rcPtz.left = rectClient.left + (rectClient.Width() - panWidth) - spaceR;
// 			rcPtz.right = rcPtz.left + panWidth;
// 			rcPtz.top = rectClient.top + (rectClient.Height() - hcfg) + spaceT + nHeightspace;
// 			rcPtz.bottom = rcPtz.top + hcfg - spaceB - spaceT - nHeightspace;
// 			m_pHisLog->MoveWindow(rcPtz);
// 		}
// 		//-----------------------------------------------------------------------------------------------------------------
// 		if (m_pTopDlg->m_curDlg == DIALOG_CUR_WALL)
// 		{
// 			hcfg = 1;//pan高度
// 			//tree
// 			CRect rcTree;
// 			rcTree.left = rectClient.left + (rectClient.Width() - panWidth) - spaceR;
// 			rcTree.right = rcTree.left + panWidth;
// 			rcTree.top = rectClient.top + spaceT + titleHeight;
// 			rcTree.bottom = rcTree.top + rectClient.Height() - hcfg - titleHeight;
// 			m_tree.MoveWindow(rcTree);
// 
// 
// 
// 
// 		}


// 		//-----------------------------------------------------------------------------------------------------------------
// 		//tree
// 		CRect rcTree;
// 		rcTree.left = rectClient.left + (rectClient.Width() - panWidth) - spaceR;
// 		rcTree.right = rcTree.left + panWidth;
// 		rcTree.top = rectClient.top + spaceT + titleHeight;
// 		rcTree.bottom = rcTree.top + rectClient.Height() - hcfg - titleHeight;
// 
// 		m_tree.MoveWindow(rcTree);
// 
// 
// 
// 	
// 
// 		//-----------------------------------------------------------------------------------------------------------------
// 		CRect rcPtz;
// 		rcPtz.left = rectClient.left + (rectClient.Width() - panWidth) - spaceR;
// 		rcPtz.right = rcPtz.left + panWidth;
// 
// 		rcPtz.top = rectClient.top + (rectClient.Height() - hcfg) + spaceT + nHeightspace;
// 
// 		rcPtz.bottom = rcPtz.top + hcfg - spaceB - spaceT - nHeightspace;
// 
// 
// 		m_pHisLog->MoveWindow(rcPtz);
// 		m_pPtz->MoveWindow(rcPtz);
		// 		m_syscfg.MoveWindow(rectClient.left + space + (rectClient.Width() - rightWidth), rectClient.top + space + rectClient.Height() - hcfg + nHeightspace , nsize, nsize);
		// 		m_devcfg.MoveWindow(rectClient.left + space + (rectClient.Width() - rightWidth) + space + nsize, rectClient.top + space + rectClient.Height() - hcfg + nHeightspace , nsize, nsize);
		// 		m_fullscreen.MoveWindow(rectClient.left + space + (rectClient.Width() - rightWidth) + (space + nsize) * 2, rectClient.top + space + rectClient.Height() - hcfg + nHeightspace , nsize, nsize);


	}
	else
	{
		CRect rectClient;
		GetClientRect(rectClient);

		m_pTopDlg->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER);
		//m_pLeftPan->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER);


		//m_pVideoDlg->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER);
		//m_pHisVideoDlg->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER);
		//m_tree.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER);
		//m_pPtz->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER);
		//m_pHisLog->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER);


		m_pDlgPanLive->SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
		m_pDlgLoopplay->SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
		//m_pDlgLog->SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(),SWP_NOACTIVATE | SWP_NOZORDER);

#if 0
		m_pDlgPanRecord->SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
#endif



		//m_pHisPlayer->SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(),
		//	SWP_NOACTIVATE | SWP_NOZORDER);
	}
}


HBRUSH CPlatClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

// 	if (pWnd->GetDlgCtrlID() == IDC_TREE_DEVICE)
// 	{
// 		pDC->SetBkMode(TRANSPARENT);
// 		return (HBRUSH)GetStockObject(NULL_BRUSH);
// 	}


	// TODO:  Change any attributes of the DC here
	//return (HBRUSH)theApp.hbrushBg;
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}




// 
// void CPlatClientDlg::OnBnClickedButtonSysCfg()
// {
// 	// TODO: Add your control notification handler code here
// }
// 
// 
// void CPlatClientDlg::OnBnClickedButtonDevCfg()
// {
// 	// TODO: Add your control notification handler code here
// // 	CDevListDlg m_devListDlg(this);
// // 	m_devListDlg.DoModal();
// }
// 
// 
// void CPlatClientDlg::OnBnClickedButtonFullScreen()
// {
// 	// TODO: Add your control notification handler code here
// }



void CPlatClientDlg::OnMenuDevices()
{
	// TODO: Add your command handler code here
	//CDevListDlg dlg(this);
	//dlg.DoModal();
}


LRESULT CPlatClientDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
// 
// 	if (WM_VIDEO_WINDOW_CLOSE_STREAM == message)
// 	{
// 		int nIndex = (int)wParam;
// 		XViewCloseStream(nIndex);
// 	}
// 
// 	if (WM_VIDEO_WINDOW_CAPPIC == message)
// 	{
// 		int nIndex = (int)wParam;
// 		XViewCapturePictures(nIndex);
// 	}
// 
// 	if (WM_VIDEO_WINDOW_RECORD == message)
// 	{
// 		int nIndex = (int)wParam;
// 		int nStart = (int)lParam;
// 
// 		XViewRecord(nIndex,nStart);
// 	}
// 
// 	if (WM_VIDEO_WINDOW_SOUND == message)
// 	{
// 		int nIndex = (int)wParam;
// 		int nStart = (int)lParam;
// 
// 		XViewSound(nIndex, nStart);
// 	}
// 	
// 	if (WM_VIDEO_WINDOW_TALK == message)
// 	{
// 		int nIndex = (int)wParam;
// 		int nStart = (int)lParam;
// 
// 		XViewTalk(nIndex, nStart);
// 	}
// 
// // 	if (WM_VIDEO_WINDOW_VIDEOQUALITY == message)
// // 	{
// // 		int nIndex = (int)wParam;
// // 		int i = (int)lParam;
// // 
// // 
// // 		void *p = NULL;
// // 		m_pVideoDlg->XGetSubWindowData(nIndex, &p);
// // 		PLAY_WINDOW_DATA *pData = (PLAY_WINDOW_DATA *)p;
// // 
// // 
// // 		if (pData->pDevObject)
// // 		{
// // 			//pData->pDevObject->SetChannelParam(0, NULL, i, X_PARAM_TYPE_ENCODE);
// // 		}
// // 
// // 
// // 	}
// 
// // 	if (WM_HIS_VIDEO_WINDOW_PAUSE == message)
// // 	{
// // 		int nIndex = (int)wParam;
// // 		int nStart = (int)lParam;
// // 
// // 		XHisViewPause(nIndex, nStart);
// // 	}
// // 
// // 	if (WM_HIS_VIDEO_WINDOW_STOP == message)
// // 	{
// // 		int nIndex = (int)wParam;
// // 		//int nStart = (int)lParam;
// // 
// // 		XHisViewStop(nIndex);
// // 	}
// // 
// // 
// // 	if (WM_HIS_VIDEO_WINDOW_POS_CALLBACK == message)
// // 	{
// // 		int nIndex = (int)wParam;
// // 		//int nStart = (int)lParam;
// // 
// // 		XHisViewPos(nIndex);
// // 	}
// // 	if (WM_HIS_VIDEO_WINDOW_SEEK == message)
// // 	{
// // 		int nIndex = (int)wParam;
// // 		int nStart = (int)lParam;
// // 
// // 		XHisViewSeek(nIndex, nStart);
// // 	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}




BOOL CPlatClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_KEYDOWN == pMsg->message)
	{
		UINT nKey = (int)pMsg->wParam;
		if (VK_RETURN == nKey){
			return TRUE;
		}

		if (VK_ESCAPE == nKey){
			//AfxGetMainWnd()->SendMessage(WM_KEYDOWN,VK_ESCAPE,0);
			if (m_fFullScreen)
			{
				ShowWindowMax(FALSE);
			}
			return TRUE;
		}
	}
	return __super::PreTranslateMessage(pMsg);
}


void CPlatClientDlg::On32771()
{
	// TODO: Add your command handler code here
	CSysSettingDlg sysDlg;
	if (sysDlg.DoModal() == IDOK)
	{
	}
}


// void CPlatClientDlg::OnViewMenuParam()
// {
// 	// TODO: Add your command handler code here
// 	void *p = NULL;
// 	m_pVideoDlg->XGetSubWindowData(m_pVideoDlg->XGetSubWindowFocusIndex(), &p);
// 	PLAY_WINDOW_DATA *pData = (PLAY_WINDOW_DATA *)p;
// 	CParamDlg dlg(pData->pDevObject, PARAM_4_YYJ, this);
// 	dlg.DoModal();
// }
// void CPlatClientDlg::OnViewMenuPtzadv()
// {
// 	// TODO: Add your command handler code here
// 
// 	void *p = NULL;
// 	m_pVideoDlg->XGetSubWindowData(m_pVideoDlg->XGetSubWindowFocusIndex(), &p);
// 	PLAY_WINDOW_DATA *pData = (PLAY_WINDOW_DATA *)p;
// 	CXJMPtzAdvDlg dlg(pData->pDevObject);
// 	dlg.DoModal();
// 
// 
// }


void CPlatClientDlg::ShowWindowMax(BOOL bShowMax)
{
	
	m_fFullScreen = bShowMax;

	if (m_fFullScreen)
	{
		m_pDlgPanLive->ShowHideLeftPan(FALSE);
		m_IsZoomed = AfxGetMainWnd()->IsZoomed();

		if (m_IsZoomed)
		{
			SendMessage(WM_SIZE, 0, 0);
		}
		else
		{
			AfxGetMainWnd()->SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
		}	
	}
	else
	{
		m_pDlgPanLive->ShowHideLeftPan(TRUE);
		if (m_IsZoomed)
		{
			SendMessage(WM_SIZE, 0, 0);
		}
		else
		{
			AfxGetMainWnd()->SendMessage(WM_SYSCOMMAND, SC_RESTORE);
		}
	}

//	SendMessage(WM_SIZE, 0, 0);

// 	if (AfxGetMainWnd()->IsZoomed())
// 		AfxGetMainWnd()->SendMessage(WM_SYSCOMMAND, SC_RESTORE);
// 	else
// 		AfxGetMainWnd()->SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);

}



void CPlatClientDlg::On32789()
{
	//密码修改
// 	CSysSettingPwdDlg dlg;
// 	dlg.DoModal();
}

void CPlatClientDlg::OnMove(int x, int y)
{
	__super::OnMove(x, y);

	// TODO: Add your message handler code here

	if (m_pDlgPanLive == NULL)
	{
		return;
	}
	for (int i = 0; i < MAX_WINDOWS_NUM; i++)
	{
		m_pDlgPanLive->m_pVideoDlg->XOnMove(i);
	}
}



void CPlatClientDlg::On32810()
{
	CString strAppname;
	Get_String_From_Language_Ini_File(_T("str_string_appnamesearch"), strAppname);


	TCHAR EXEPath[MAX_PATH];
	//TCHAR FilePath[MAX_PATH];

	if (GetModuleFileName(NULL, EXEPath, MAX_PATH)){
		PathRemoveFileSpec(EXEPath);
	}

	CString str;
	str.Format(_T("%s\\IPCameraSearch.exe"), EXEPath);

	//CWnd * hwnd = FindWindow(_T("IPCameraSearch"), NULL);
	CWnd * hwnd = FindWindow(NULL, strAppname);
	if (NULL == hwnd)
	{

// 		SHELLEXECUTEINFO ShExecInfo = { 0 };
// 		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
// 		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
// 		ShExecInfo.hwnd = NULL;
// 		ShExecInfo.lpVerb = NULL;
// 		ShExecInfo.lpFile = str;
// 		ShExecInfo.lpParameters = L"";
// 		ShExecInfo.lpDirectory = NULL;
// 		ShExecInfo.nShow = SW_SHOW;
// 		ShExecInfo.hInstApp = NULL;
// 		ShellExecuteEx(&ShExecInfo);
// 		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
		ShellExecute(NULL, _T("open"), str, NULL, NULL, SW_SHOWNORMAL);
	}
	else
	{
		//if (hwnd->IsIconic())
		{
			hwnd->ShowWindow(SW_SHOWNORMAL);
		}

		hwnd->BringWindowToTop();
		hwnd->SetForegroundWindow();
	}
}


void CPlatClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 2254)
	{
		SetThreadExecutionState(ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED);
	}
	__super::OnTimer(nIDEvent);
}
void CPlatClientDlg::ShowPan(MyEnumPanType _type)
{


	m_pDlgPanLive->ShowWindow(SW_HIDE);
	m_pDlgLoopplay->ShowWindow(SW_HIDE);
	//m_pDlgLog->ShowWindow(SW_HIDE);
	switch (_type)
	{
	case EM_PAN_TYPE_LIVE:
		m_pDlgPanLive->ShowWindow(SW_SHOW);
		break;
	case EM_PAN_TYPE_LOOPPLAY:
		m_pDlgLoopplay->ShowWindow(SW_SHOW);
		break;
// 	case EM_PAN_TYPE_LOG:
// 		//m_pDlgLog->ShowWindow(SW_SHOW);
// 		break;
// 
// 	case EM_PAN_TYPE_REC:
// 		//m_pDlgPanRecord->ShowWindow(SW_SHOW);
// 		break;
// 	case EM_PAN_TYPE_JPG:
// 		
// 		break;
	case EM_PAN_TYPE_MAP:
		
		break;

	default:
		break;
	}

}