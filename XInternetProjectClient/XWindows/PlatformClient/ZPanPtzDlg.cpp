// PtzAdvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlatformClient.h"
#include "ZPanPtzDlg.h"
#include "afxdialogex.h"
#include "a_signaling_channel_json_reader_builder.h"
#include "PlatformClientDlg.h"

// CPtzAdvDlg dialog
#define PTZ_LEFT_SPACE 1
//#define PTZ_TOP_SPACE 7

#define PTZ_PAN_WIDTH  46
#define PTZ_PAN_HEIGHT 46


#define PTZ_JGD_WIDTH 32
#define PTZ_JGD_HEIGHT PTZ_PAN_HEIGHT




#define				CTRLID_PTZ_MD_STOP							0									// 停止运动
#define				CTRLID_PTZ_MD_LEFT							1									// 向左
#define				CTRLID_PTZ_MD_RIGHT							2									// 向右
#define				CTRLID_PTZ_MD_UP							3									// 向上
#define				CTRLID_PTZ_MD_DOWN							4									// 向下
#define				CTRLID_PTZ_MD_LEFT_UP						5									// 左上
#define				CTRLID_PTZ_MD_LEFT_DOWN						6									// 左下
#define				CTRLID_PTZ_MD_RIGHT_UP						7									// 右上
#define				CTRLID_PTZ_MD_RIGHT_DOWN					8									// 右下


#define				CTRLID_PTZ_ZOOM_ADD						10									// 
#define				CTRLID_PTZ_ZOOM_INC						11									// 

#define				CTRLID_PTZ_FOCUS_ADD						12									// 
#define				CTRLID_PTZ_FOCUS_INC						13									// 

#define				CTRLID_PTZ_IRIS_OPEN					14									// 
#define				CTRLID_PTZ_IRIS_CLOSE						15									// 




IMPLEMENT_DYNAMIC(CZPanPtzDlg, CDialogEx)

CZPanPtzDlg::CZPanPtzDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CZPanPtzDlg::IDD, pParent)
{
	//m_id = _id;
	//m_bitmapBG = NULL;
}
// void CVideoViewPtzDlg::SetDeviceInfo(stDevice st)
// {
// 	memcpy(&m_st, &st, sizeof(st));
// }
CZPanPtzDlg::~CZPanPtzDlg()
{
	// 	if (m_bitmapBG)
	// 	{
	// 		CBitmap bmp;
	// 		bmp.Attach(m_bitmapBG);
	// 		bmp.Detach();
	// 		DeleteObject(m_bitmapBG);
	// 	}
}

void CZPanPtzDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbpreset);
	//////////////////////////////////////////////////////////////////////////
	DDX_Control(pDX, IDC_PTZ_BTN_UP, m_ptz_up);
	DDX_Control(pDX, IDC_PTZ_BTN_STOP, m_ptz_center);
	DDX_Control(pDX, IDC_PTZ_BTN_RIGHT, m_ptz_right);
	DDX_Control(pDX, IDC_PTZ_BTN_LEFT, m_ptz_left);
	DDX_Control(pDX, IDC_PTZ_BTN_DOWN, m_ptz_dowm);
	DDX_Control(pDX, IDC_PTZ_BTN_LEFTUP, m_ptz_leftup);
	DDX_Control(pDX, IDC_PTZ_BTN_RIGHTUP, m_ptz_rightup);
	DDX_Control(pDX, IDC_PTZ_BTN_LEFTDOWN, m_ptz_leftdown);
	DDX_Control(pDX, IDC_PTZ_BTN_RIGHTDOWN, m_ptz_rightdown);
	DDX_Control(pDX, IDC_BTN_Z1, m_btn_bb1);
	DDX_Control(pDX, IDC_BTN_Z2, m_btn_bb2);
	DDX_Control(pDX, IDC_BTN_J1, m_btn_jj1);
	DDX_Control(pDX, IDC_BTN_J2, m_btn_jj2);
	DDX_Control(pDX, IDC_BTN_G1, m_btn_gq1);
	DDX_Control(pDX, IDC_BTN_G2, m_btn_gq2);
	DDX_Control(pDX, IDC_STATIC_PTZ1, m_static1);
	DDX_Control(pDX, IDC_STATIC_PTZ2, m_static2);
	DDX_Control(pDX, IDC_STATIC_PTZ3, m_static3);
	//-----------------------------------------
	DDX_Control(pDX, IDC_BTN_J3, m_btn_reset);
}


BEGIN_MESSAGE_MAP(CZPanPtzDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CZPanPtzDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CZPanPtzDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CZPanPtzDlg::OnBnClickedButton3)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CPtzAdvDlg message handlers





BOOL CZPanPtzDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//ptz

	//------------------------------------------------------------------------------------------------------------------
	ButtonLoadBitmap(m_ptz_leftup, L"ptz/ptz_01.png", L"ptz/ptz__01.png", L"ptz/ptz_01.png", L"ptz/ptz__01.png", CTRLID_PTZ_MD_LEFT_UP);
	ButtonLoadBitmap(m_ptz_up, L"ptz/ptz_02.png", L"ptz/ptz__02.png", L"ptz/ptz_02.png", L"ptz/ptz__02.png", CTRLID_PTZ_MD_UP);
	ButtonLoadBitmap(m_ptz_rightup, L"ptz/ptz_03.png", L"ptz/ptz__03.png", L"ptz/ptz_03.png", L"ptz/ptz__03.png", CTRLID_PTZ_MD_RIGHT_UP);

	ButtonLoadBitmap(m_ptz_left, L"ptz/ptz_04.png", L"ptz/ptz__04.png", L"ptz/ptz_04.png", L"ptz/ptz__04.png", CTRLID_PTZ_MD_LEFT);
	ButtonLoadBitmap(m_ptz_center, L"ptz/ptz_05.png", L"ptz/ptz__05.png", L"ptz/ptz_05.png", L"ptz/ptz__05.png");
	ButtonLoadBitmap(m_ptz_right, L"ptz/ptz_06.png", L"ptz/ptz__06.png", L"ptz/ptz_06.png", L"ptz/ptz__06.png", CTRLID_PTZ_MD_RIGHT);

	ButtonLoadBitmap(m_ptz_leftdown, L"ptz/ptz_07.png", L"ptz/ptz__07.png", L"ptz/ptz_07.png", L"ptz/ptz__07.png", CTRLID_PTZ_MD_LEFT_DOWN);
	ButtonLoadBitmap(m_ptz_dowm, L"ptz/ptz_08.png", L"ptz/ptz__08.png", L"ptz/ptz_08.png", L"ptz/ptz__08.png", CTRLID_PTZ_MD_DOWN);
	ButtonLoadBitmap(m_ptz_rightdown, L"ptz/ptz_09.png", L"ptz/ptz__09.png", L"ptz/ptz_09.png", L"ptz/ptz__09.png", CTRLID_PTZ_MD_RIGHT_DOWN);
	//------------------------------------------------------------------------------------------------------------------



	m_static1.ModifyStyle(0, SS_CENTERIMAGE);
	m_static2.ModifyStyle(0, SS_CENTERIMAGE);
	m_static3.ModifyStyle(0, SS_CENTERIMAGE);

	//------------------------------------------------------------------------------------------------------------------
	ButtonLoadBitmap(m_btn_bb1, L"ptz/zoom_f_1.png", L"ptz/zoom_f_2.png", L"ptz/zoom_f_1.png", L"ptz/zoom_f_2.png", CTRLID_PTZ_ZOOM_ADD);
	ButtonLoadBitmap(m_btn_bb2, L"ptz/zoom_n_1.png", L"ptz/zoom_n_2.png", L"ptz/zoom_n_1.png", L"ptz/zoom_n_2.png", CTRLID_PTZ_ZOOM_INC);

	ButtonLoadBitmap(m_btn_jj1, L"ptz/focus_f_1.png", L"ptz/focus_f_2.png", L"ptz/focus_f_1.png", L"ptz/focus_f_2.png", CTRLID_PTZ_FOCUS_ADD);
	ButtonLoadBitmap(m_btn_jj2, L"ptz/focus_n_1.png", L"ptz/focus_n_2.png", L"ptz/focus_n_1.png", L"ptz/focus_n_2.png", CTRLID_PTZ_FOCUS_INC);


	ButtonLoadBitmap(m_btn_gq1, L"ptz/focus_f_1.png", L"ptz/focus_f_2.png", L"ptz/focus_f_1.png", L"ptz/focus_f_2.png", CTRLID_PTZ_FOCUS_ADD);
	ButtonLoadBitmap(m_btn_gq2, L"ptz/focus_n_1.png", L"ptz/focus_n_2.png", L"ptz/focus_n_1.png", L"ptz/focus_n_2.png", CTRLID_PTZ_FOCUS_INC);


	ButtonLoadBitmap(m_btn_reset, L"ptz/reset_1.png", L"ptz/reset_2.png", L"ptz/reset_1.png", L"ptz/reset_2.png", CTRLID_PTZ_MD_STOP);

	m_ptz_leftup.SetWindowText(L"");
	//m_ptz_leftup.SetTooltipText(L"sssss");

	m_ptz_up.SetWindowText(L"");
	//m_ptz_up.SetTooltipText(L"sssss");


	m_ptz_rightup.SetWindowText(L"");
	//m_ptz_rightup.SetTooltipText(L"sssss");


	m_ptz_left.SetWindowText(L"");
	//m_ptz_left.SetTooltipText(L"sssss");


	m_ptz_center.SetWindowText(L"");
	//m_ptz_center.SetTooltipText(L"sssss");

	m_ptz_right.SetWindowText(L"");
	//m_ptz_right.SetTooltipText(L"sssss");



	m_ptz_leftdown.SetWindowText(L"");
	//m_ptz_leftdown.SetTooltipText(L"sssss");


	m_ptz_dowm.SetWindowText(L"");
	//m_ptz_dowm.SetTooltipText(L"sssss");

	m_ptz_rightdown.SetWindowText(L"");
	//m_ptz_rightdown.SetTooltipText(L"sssss");
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	//m_btn_bb1.ShowWindow(SW_HIDE);
	//m_btn_jj1.ShowWindow(SW_HIDE);
	m_btn_gq1.ShowWindow(SW_HIDE);

	//m_btn_bb2.ShowWindow(SW_HIDE);
	//m_btn_jj2.ShowWindow(SW_HIDE);
	m_btn_gq2.ShowWindow(SW_HIDE);

	m_static1.ShowWindow(SW_HIDE);
	m_static2.ShowWindow(SW_HIDE);
	m_static3.ShowWindow(SW_HIDE);

	//	img.Load(theApp.m_strSkinPath + L"ptz/ptz0.png");
	//	m_bitmapBG = img.Detach();



	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//---------------------------------------------------------------------------


	//在OnInitDialog()中加入以下代码：
	//加入WS_EX_LAYERED扩展属性
	// 	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE,
	// 		GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) ^ 0x80000);
	// 	HINSTANCE hInst = LoadLibrary(L"User32.DLL");
	// 	if (hInst)
	// 	{
	// 		typedef BOOL(WINAPI *MYFUNC)(HWND, COLORREF, BYTE, DWORD);
	// 		MYFUNC fun = NULL;
	// 		//取得SetLayeredWindowAttributes函数指针
	// 		fun = (MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
	// 		//if (fun)fun(this->GetSafeHwnd(), RGB(240,240,240), 90, 2);//注意：fun的参数128不能太小，否则就完全透明了！
	// 		if (fun)fun(this->GetSafeHwnd(), RGB(240, 240, 240), 64, 1);//注意：fun的参数128不能太小，否则就完全透明了！
	// 		FreeLibrary(hInst);
	// 	}
	//HWND hWnd : 窗口句柄
	//COLORREF col : 透明化颜色
	//BYTE bAlpha : ＝0：整个窗口透明, = 255 完全不透明
	//DWORD dwFlags : ＝1：仅颜色 col 透明， = 2 : 窗口按照bAlpha变量进行透明处理。



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


HBRUSH CZPanPtzDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	static HBRUSH hbrushDlg = CreateSolidBrush(RGB(51, 51, 51));
	return (HBRUSH)hbrushDlg;
	// TODO:  Change any attributes of the DC here
	//return (HBRUSH)theApp.hViewBrushBg;
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CZPanPtzDlg::OnBnClickedButton1()
{
	//预置位设置
	CString str;
	m_cbpreset.GetWindowText(str);

	int k = _ttoi(str) - 1;

	//m_pDevObject->TransparentTransmission (0, PTZ_PRESET_SET_PRESET, k);

}


void CZPanPtzDlg::OnBnClickedButton2()
{
	//预置位调用
	CString str;
	m_cbpreset.GetWindowText(str);

	int k = _ttoi(str) - 1;

	//m_pDevObject->TransparentTransmission(0, PTZ_PRESET_GO_PRESET, k);
}


void CZPanPtzDlg::OnBnClickedButton3()
{
	//预置位删除
	CString str;
	m_cbpreset.GetWindowText(str);

	int k = _ttoi(str) - 1;

	//m_pDevObject->TransparentTransmission(0, PTZ_PRESET_CL_PRESET, k);
}

void CZPanPtzDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here


	if ((cx <= 0) || (cy <= 0))
	{
		return;
	}

	if (GetSafeHwnd() == NULL)
	{
		return;
	}


	if (m_ptz_leftup.GetSafeHwnd() == NULL)
	{
		return;
	}

	int PTZ_TOP_SPACE = 10;

	//////////////////////////////////////////////////////////////////////////
	//------------------------------------------------------------------------------------------------------------------
	m_ptz_leftup.SetWindowPos(NULL, PTZ_LEFT_SPACE, PTZ_TOP_SPACE, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	m_ptz_up.SetWindowPos(NULL, PTZ_LEFT_SPACE + PTZ_PAN_WIDTH, PTZ_TOP_SPACE, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	m_ptz_rightup.SetWindowPos(NULL, PTZ_LEFT_SPACE + PTZ_PAN_WIDTH * 2, PTZ_TOP_SPACE, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);

	m_ptz_left.SetWindowPos(NULL, PTZ_LEFT_SPACE, PTZ_TOP_SPACE + PTZ_PAN_HEIGHT, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	m_ptz_center.SetWindowPos(NULL, PTZ_LEFT_SPACE + PTZ_PAN_WIDTH, PTZ_TOP_SPACE + PTZ_PAN_HEIGHT, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	m_ptz_right.SetWindowPos(NULL, PTZ_LEFT_SPACE + PTZ_PAN_WIDTH * 2, PTZ_TOP_SPACE + PTZ_PAN_HEIGHT, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);

	m_ptz_leftdown.SetWindowPos(NULL, PTZ_LEFT_SPACE, PTZ_TOP_SPACE + PTZ_PAN_HEIGHT * 2, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	m_ptz_dowm.SetWindowPos(NULL, PTZ_LEFT_SPACE + PTZ_PAN_WIDTH, PTZ_TOP_SPACE + PTZ_PAN_HEIGHT * 2, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	m_ptz_rightdown.SetWindowPos(NULL, PTZ_LEFT_SPACE + PTZ_PAN_WIDTH * 2, PTZ_TOP_SPACE + PTZ_PAN_HEIGHT * 2, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);


	int  leftspace = 1 + PTZ_PAN_WIDTH * 3 + 15;
	int __a = 20;
	int ns = 10;
	int ts = 60;//文本框宽度
	PTZ_TOP_SPACE = 10 + __a;

	m_btn_bb1.SetWindowPos(NULL, leftspace, PTZ_TOP_SPACE, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	m_btn_bb2.SetWindowPos(NULL, leftspace + ts, PTZ_TOP_SPACE, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);

	PTZ_TOP_SPACE = 45 + __a;
	m_btn_jj1.SetWindowPos(NULL, leftspace, PTZ_TOP_SPACE, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	m_btn_jj2.SetWindowPos(NULL, leftspace + ts, PTZ_TOP_SPACE, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);

	PTZ_TOP_SPACE = 80 + __a;
	m_btn_reset.SetWindowPos(NULL, leftspace, PTZ_TOP_SPACE, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);


#if 0
	m_btn_bb1.SetWindowPos(NULL, PTZ_LEFT_SPACE + PTZ_PAN_WIDTH * 3 + ns, PTZ_TOP_SPACE, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	m_btn_jj1.SetWindowPos(NULL, PTZ_LEFT_SPACE + PTZ_PAN_WIDTH * 3 + ns, PTZ_TOP_SPACE + PTZ_PAN_HEIGHT, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	m_btn_gq1.SetWindowPos(NULL, PTZ_LEFT_SPACE + PTZ_PAN_WIDTH * 3 + ns, PTZ_TOP_SPACE + PTZ_PAN_HEIGHT * 2, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);


	m_btn_bb2.SetWindowPos(NULL, PTZ_LEFT_SPACE + PTZ_PAN_WIDTH * 3 + ns + ts + PTZ_JGD_WIDTH, PTZ_TOP_SPACE, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	m_btn_jj2.SetWindowPos(NULL, PTZ_LEFT_SPACE + PTZ_PAN_WIDTH * 3 + ns + ts + PTZ_JGD_WIDTH, PTZ_TOP_SPACE + PTZ_PAN_HEIGHT, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	m_btn_gq2.SetWindowPos(NULL, PTZ_LEFT_SPACE + PTZ_PAN_WIDTH * 3 + ns + ts + PTZ_JGD_WIDTH, PTZ_TOP_SPACE + PTZ_PAN_HEIGHT * 2, PTZ_PAN_WIDTH, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);


	m_static1.SetWindowPos(NULL, PTZ_LEFT_SPACE + PTZ_PAN_WIDTH * 3 + ns + PTZ_JGD_WIDTH, PTZ_TOP_SPACE, ts, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	m_static2.SetWindowPos(NULL, PTZ_LEFT_SPACE + PTZ_PAN_WIDTH * 3 + ns + PTZ_JGD_WIDTH, PTZ_TOP_SPACE + PTZ_PAN_HEIGHT, ts, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
	m_static3.SetWindowPos(NULL, PTZ_LEFT_SPACE + PTZ_PAN_WIDTH * 3 + ns + PTZ_JGD_WIDTH, PTZ_TOP_SPACE + PTZ_PAN_HEIGHT * 2, ts, PTZ_PAN_HEIGHT, SWP_NOACTIVATE | SWP_NOZORDER);
#endif


	//////////////////////////////////////////////////////////////////////////
}
LRESULT CZPanPtzDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	int nCtrlID = (int)wParam;


	char szTmp[4096] = { 0 };
	int len = 0;
	char szDeviceId[256] = { 0 };






	switch (message)
	{
	case  WM_BMP_BTNUP:
	{
		// 		CPlatClientDlg * pWnd = (CPlatClientDlg *)m_pParentDlg;
		// 
		// 		int  n = pWnd->m_pVideoDlg->get_focus_Index();
		// 
		// 		void *p = NULL;
		// 		pWnd->m_pVideoDlg->get_data(n, &p);
		// 
		// 		if (NULL == p)
		// 		{
		// 			return CDialogEx::WindowProc(message, wParam, lParam);;
		// 		}
		// 		WINDOW_DATA *pData = (WINDOW_DATA *)p;
		// 
		// 
		// 		if (pData->pDevObject == NULL)
		// 		{
		// 			return CDialogEx::WindowProc(message, wParam, lParam);;
		// 		}

		int speed = 32;
		//		int channel = m_id;


		switch (nCtrlID)
		{
		case CTRLID_PTZ_MD_LEFT:
		case CTRLID_PTZ_MD_RIGHT:
		case CTRLID_PTZ_MD_UP:
		case CTRLID_PTZ_MD_DOWN:
			//case CTRLID_PTZ_MD_LEFT_UP:
			//case CTRLID_PTZ_MD_LEFT_DOWN:
			//case CTRLID_PTZ_MD_RIGHT_UP:
			//case CTRLID_PTZ_MD_RIGHT_DOWN:
		{

			CPlatClientDlg * pMainWnd = (CPlatClientDlg *)AfxGetMainWnd();
			int nFocusIndex = pMainWnd->m_pDlgPanLive->m_pVideoDlg->m_nFocusIndex;
			//pMainWnd->m_pDlgPanLive->m_pVideoDlg->m_pVideoWindows[nFocusIndex]->m_stDevice;

			if (theApp.m_ClientType == X_CLIENT_XXX_TYPE_CLASSIC)
			{
				sprintf(szDeviceId, "%d", pMainWnd->m_pDlgPanLive->m_pVideoDlg->m_pVideoWindows[nFocusIndex]->m_stDevice.classicinfo.dev_id);
			}

			char szuuid[64] = { 0 };


			if (pMainWnd->m_pDlgPanLive->m_pVideoDlg->m_pVideoWindows[nFocusIndex]->m_pStreamObject)
			{
				string s = JsonBuildPtzInfo(szDeviceId, "stop", 0, szuuid);
				theApp.m_pClientInferface->SignalingCtrl(szDeviceId, s.c_str(), s.length(), szTmp, len);
			}


			//m_pDevObject->TransparentTransmission(channel, PTZ_MD_STOP, 0);
		}
		break;
		case CTRLID_PTZ_ZOOM_ADD:
		{
			//m_pDevObject->TransparentTransmission(channel, PTZ_ACTION_ZOOM_STOP, 1);
		}
		break;
		case CTRLID_PTZ_ZOOM_INC:
		{
			//m_pDevObject->TransparentTransmission(channel, PTZ_ACTION_ZOOM_STOP, 0);
		}
		break;
		case CTRLID_PTZ_FOCUS_ADD:
		{
			//m_pDevObject->TransparentTransmission(channel, PTZ_ACTION_FOCUS_STOP, 1);
		}
		break;
		case CTRLID_PTZ_FOCUS_INC:
		{
			//m_pDevObject->TransparentTransmission(channel, PTZ_ACTION_FOCUS_STOP, 0);
		}
		break;
		case CTRLID_PTZ_IRIS_OPEN:
		{
			//m_pDevObject->TransparentTransmission(channel, PTZ_ACTION_IRIS_STOP, 1);
		}
		break;
		case CTRLID_PTZ_IRIS_CLOSE:
		{
			//m_pDevObject->TransparentTransmission(channel, PTZ_ACTION_IRIS_STOP, 0);
		}
		break;
		}
	}
	break;
	case WM_BMP_BTNDOWN:
	{

		// 		CPlatClientDlg * pWnd = (CPlatClientDlg *)m_pParentDlg;
		// 
		// 		int  n = pWnd->m_pVideoDlg->get_focus_Index();
		// 
		// 		void *p = NULL;
		// 		pWnd->m_pVideoDlg->get_data(n, &p);
		// 
		// 		if (NULL == p)
		// 		{
		// 			return CDialogEx::WindowProc(message, wParam, lParam);;
		// 		}
		// 		WINDOW_DATA *pData = (WINDOW_DATA *)p;
		// 
		// 
		// 		if (pData->pDevObject == NULL)
		// 		{
		// 			return CDialogEx::WindowProc(message, wParam, lParam);;
		// 		}

		int speed = 32;
		//		int channel = m_id;


		CPlatClientDlg * pMainWnd = (CPlatClientDlg *)AfxGetMainWnd();
		int nFocusIndex = pMainWnd->m_pDlgPanLive->m_pVideoDlg->m_nFocusIndex;
		//pMainWnd->m_pDlgPanLive->m_pVideoDlg->m_pVideoWindows[nFocusIndex]->m_stDevice;

		if (theApp.m_ClientType == X_CLIENT_XXX_TYPE_CLASSIC)
		{
			sprintf(szDeviceId, "%d", pMainWnd->m_pDlgPanLive->m_pVideoDlg->m_pVideoWindows[nFocusIndex]->m_stDevice.classicinfo.dev_id);
		}

		char szuuid[64] = { 0 };



		switch (nCtrlID)
		{
		case CTRLID_PTZ_MD_LEFT:
		{
			if (pMainWnd->m_pDlgPanLive->m_pVideoDlg->m_pVideoWindows[nFocusIndex]->m_pStreamObject)
			{
				string s = JsonBuildPtzInfo(szDeviceId, "left", 0, szuuid);
				theApp.m_pClientInferface->SignalingCtrl(szDeviceId, s.c_str(), s.length(), szTmp, len);
			}
		}
		break;
		case CTRLID_PTZ_MD_RIGHT:
		{
			if (pMainWnd->m_pDlgPanLive->m_pVideoDlg->m_pVideoWindows[nFocusIndex]->m_pStreamObject)
			{
				string s = JsonBuildPtzInfo(szDeviceId, "right", 0, szuuid);
				theApp.m_pClientInferface->SignalingCtrl(szDeviceId, s.c_str(), s.length(), szTmp, len);
			}

		}
		break;
		case CTRLID_PTZ_MD_UP:
		{
			if (pMainWnd->m_pDlgPanLive->m_pVideoDlg->m_pVideoWindows[nFocusIndex]->m_pStreamObject)
			{
				string s = JsonBuildPtzInfo(szDeviceId, "up", 0, szuuid);
				theApp.m_pClientInferface->SignalingCtrl(szDeviceId, s.c_str(), s.length(), szTmp, len);
			}
		}
		break;
		case CTRLID_PTZ_MD_DOWN:
		{
			if (pMainWnd->m_pDlgPanLive->m_pVideoDlg->m_pVideoWindows[nFocusIndex]->m_pStreamObject)
			{
				string s = JsonBuildPtzInfo(szDeviceId, "down", 0, szuuid);
				theApp.m_pClientInferface->SignalingCtrl(szDeviceId, s.c_str(), s.length(), szTmp, len);
			}
		}
		break;
		case CTRLID_PTZ_MD_LEFT_UP:
		{
			if (pMainWnd->m_pDlgPanLive->m_pVideoDlg->m_pVideoWindows[nFocusIndex]->m_pStreamObject)
			{
				string s = JsonBuildPtzInfo(szDeviceId, "leftup", 0, szuuid);
				theApp.m_pClientInferface->SignalingCtrl(szDeviceId, s.c_str(), s.length(), szTmp, len);
			}
		}
		break;
		case CTRLID_PTZ_MD_LEFT_DOWN:
		{
			if (pMainWnd->m_pDlgPanLive->m_pVideoDlg->m_pVideoWindows[nFocusIndex]->m_pStreamObject)
			{
				string s = JsonBuildPtzInfo(szDeviceId, "leftdown", 0, szuuid);
				theApp.m_pClientInferface->SignalingCtrl(szDeviceId, s.c_str(), s.length(), szTmp, len);
			}
		}
		break;
		case CTRLID_PTZ_MD_RIGHT_UP:
		{
			if (pMainWnd->m_pDlgPanLive->m_pVideoDlg->m_pVideoWindows[nFocusIndex]->m_pStreamObject)
			{
				string s = JsonBuildPtzInfo(szDeviceId, "rightup", 0, szuuid);
				theApp.m_pClientInferface->SignalingCtrl(szDeviceId, s.c_str(), s.length(), szTmp, len);
			}
		}
		break;
		case CTRLID_PTZ_MD_RIGHT_DOWN:
		{
			if (pMainWnd->m_pDlgPanLive->m_pVideoDlg->m_pVideoWindows[nFocusIndex]->m_pStreamObject)
			{
				string s = JsonBuildPtzInfo(szDeviceId, "rightdown", 0, szuuid);
				theApp.m_pClientInferface->SignalingCtrl(szDeviceId, s.c_str(), s.length(), szTmp, len);
			}
		}
		break;
		case CTRLID_PTZ_ZOOM_ADD:
		{
			if (pMainWnd->m_pDlgPanLive->m_pVideoDlg->m_pVideoWindows[nFocusIndex]->m_pStreamObject)
			{
				//string s = JsonBuildPtzInfo(szDeviceId, "left", 0, szuuid);
				//theApp.m_pClientInferface->SignalingCtrl(szDeviceId, s.c_str(), s.length(), szTmp, len);
			}
		}
		break;
		case CTRLID_PTZ_ZOOM_INC:
		{
			if (pMainWnd->m_pDlgPanLive->m_pVideoDlg->m_pVideoWindows[nFocusIndex]->m_pStreamObject)
			{
				//string s = JsonBuildPtzInfo(szDeviceId, "left", 0, szuuid);
				//theApp.m_pClientInferface->SignalingCtrl(szDeviceId, s.c_str(), s.length(), szTmp, len);
			}
		}
		break;
		case CTRLID_PTZ_FOCUS_ADD:
		{
		}
		break;
		case CTRLID_PTZ_FOCUS_INC:
		{
		}
		break;
		case CTRLID_PTZ_IRIS_OPEN:
		{
		}
		break;
		case CTRLID_PTZ_IRIS_CLOSE:
		{
		}
		break;






		// 		case PTZ_LEFT_UP:
		// 		case PTZ_RIGHT_UP:
		// 		case PTZ_LEFT_DOWN:
		// 		case PTZ_RIGHT_DOWN:
		// 			m_pWnd->SendMessage(WM_PTZ_CTRL, PTZ_UP_STOP, 0);
		// 			break;
		// 		case PTZ_UP:
		// 			m_pWnd->SendMessage(WM_PTZ_CTRL, PTZ_UP_STOP, 0);
		// 			break;
		// 		case PTZ_DOWN:
		// 			m_pWnd->SendMessage(WM_PTZ_CTRL, PTZ_DOWN_STOP, 0);
		// 			break;
		// 		case PTZ_LEFT:
		// 			m_pWnd->SendMessage(WM_PTZ_CTRL, PTZ_LEFT_STOP, 0);
		// 			break;
		// 		case PTZ_RIGHT:
		// 			m_pWnd->SendMessage(WM_PTZ_CTRL, PTZ_RIGHT_STOP, 0);
		// 			break;
		}
	}
	break;
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}

void CZPanPtzDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}


void CZPanPtzDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	//OnPaintBmpBackground(this, dc);
}
