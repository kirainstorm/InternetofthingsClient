
// stdafx.cpp : source file that includes just the standard includes
// PlatClient.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "PlatformClient.h"
#include "a_signaling_channel_json_reader_builder.h"




void Set_String_To_Wnd_From_Language_Ini_File(UINT sID, CString sKey, CDialogEx * user)
{
	CWnd *pWnd = user->GetDlgItem(sID);
	if (NULL == pWnd) return;
	CLanguageIni& LangIni = CLanguageIni::GetInstance();
	CString cstr = LangIni.GetText(sKey);
	pWnd->SetWindowText(cstr);
	pWnd->Invalidate();
}

// 从文件获取语言信息
void Get_String_From_Language_Ini_File(CString sKey, CString& cStr)
{
	CLanguageIni& LangIni = CLanguageIni::GetInstance();
	cStr = LangIni.GetText(sKey);
}

void  ButtonLoadBitmap(CBmpButton &btn, LPCTSTR f1, LPCTSTR f2, LPCTSTR f3, LPCTSTR f4, int ctrlid ,CString str)
{
	CImage img;
	HBITMAP bitmap1, bitmap2, bitmap3, bitmap4;

	img.Load(theApp.m_strSkinPath + f1);
	bitmap1 = img.Detach();
	img.Destroy();
	img.Load(theApp.m_strSkinPath + f2);
	bitmap2 = img.Detach();
	img.Destroy();
	img.Load(theApp.m_strSkinPath + f3);
	bitmap3 = img.Detach();
	img.Destroy();
	img.Load(theApp.m_strSkinPath + f4);
	bitmap4 = img.Detach();
	img.Destroy();

	btn.ModifyStyle(WS_TABSTOP, BS_OWNERDRAW);
	btn.LoadBitmaps(bitmap1, bitmap2, bitmap3, bitmap4, ctrlid);
	btn.SizeToContent();
	btn.SetWindowText(L"");
	btn.SetTooltipText(str);
}

void OnPaintBmpBackground(CWnd * pWnd, CPaintDC &dc, int height)
{
	RECT rtClient;             //客户窗体大小
	pWnd->GetClientRect(&rtClient);
	int iWidth = rtClient.right;
	int iHeight = rtClient.bottom;
	if (height > 0)
	{
		iHeight = height;
	}
	//
	CDC   m_dcBack;
	m_dcBack.CreateCompatibleDC(&dc);
	m_dcBack.SelectObject(theApp.m_bitmapBG);
	//
	CDC   MemDC;
	CBitmap   MemBitmap;
	MemDC.CreateCompatibleDC(&dc);
	MemBitmap.CreateCompatibleBitmap(&dc, iWidth, iHeight);
	CBitmap   *pOldBit = MemDC.SelectObject(&MemBitmap);
	//
	MemDC.BitBlt(0, 0, iWidth, iHeight, &m_dcBack, 0, 0, SRCCOPY);
	dc.BitBlt(0, 0, iWidth, iHeight, &MemDC, 0, 0, SRCCOPY);
	//
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	m_dcBack.DeleteDC();
}