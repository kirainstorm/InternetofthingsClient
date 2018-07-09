#include "stdafx.h"
#include "MySliderCtrl.h"


class CSilderMemDC : public CDC
{
private:
	CBitmap* m_bmp;
	CBitmap* m_oldbmp;
	CDC*  m_pDC;
	CRect  m_Rect;
public:
	CSilderMemDC(CDC* pDC, const CRect& rect) : CDC()
	{
		CreateCompatibleDC(pDC);
		m_bmp = new CBitmap;
		m_bmp->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
		m_oldbmp = SelectObject(m_bmp);
		m_pDC = pDC;
		m_Rect = rect;
	}
	~CSilderMemDC()
	{
		m_pDC->BitBlt(m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height(),
			this, m_Rect.left, m_Rect.top, SRCCOPY);
		SelectObject(m_oldbmp);
		if (m_bmp != NULL)
			delete m_bmp;
	}
};

CMySliderCtrl::CMySliderCtrl()
{
	m_nProgress = 0;
	m_ProgerssBrush = CreateSolidBrush(RGB(255, 0, 0));
	proBrush.Attach(m_ProgerssBrush);


	m_allBrush = CreateSolidBrush(RGB(0, 255, 0));
	allBrush.Attach(m_allBrush);
}


CMySliderCtrl::~CMySliderCtrl()
{
	allBrush.Detach();
	proBrush.Detach();
}
BEGIN_MESSAGE_MAP(CMySliderCtrl, CSliderCtrl)
	//ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CMySliderCtrl::SetDownLoadProgress(int i)
{
	m_nProgress = i;
}
BOOL CMySliderCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
	//return CSliderCtrl::OnEraseBkgnd(pDC);
}

void CMySliderCtrl::OnPaint()
{
	CPaintDC dc(this);
	int nPos = GetPos();
	SetPos(nPos);
	CRect ThumbRC;
	GetThumbRect(ThumbRC);   //获取滑块区域
	//获取滑块的宽度和高度
	CBitmap bmp;
	bmp.LoadBitmap(IDB_THUMB);
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);
	//获取位图高度和宽度
	int bmpWidth = bmpInfo.bmWidth;
	int bmpHeight = bmpInfo.bmHeight;

	CRect ClientRC, ChanelRC;
	GetClientRect(ClientRC);  //获取客户区域
	GetChannelRect(ChanelRC);  //获取通道区域
	//绘制背景
	CBitmap bmpBK;
	BITMAP BKInfo;
	bmpBK.LoadBitmap(IDB_SLIDERBK);
	bmpBK.GetBitmap(&BKInfo);
	int nBKWidth = BKInfo.bmWidth;
	int nBKHeight = BKInfo.bmHeight;

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(&bmpBK);

	int nRightMargin = ClientRC.Width() - ChanelRC.left - ChanelRC.Width();
	ClientRC.right = ChanelRC.left + ChanelRC.Width() + nRightMargin;

	ClientRC.left = ChanelRC.left;
	CSilderMemDC bkMemDC(&dc, ClientRC);
	//绘制滑块的背景
	bkMemDC.StretchBlt(ChanelRC.left, 0, ChanelRC.Width() + ChanelRC.left, ClientRC.Height(),
		&memDC, 0, 0, nBKWidth, nBKHeight, SRCCOPY);

	//加载进度
	CRect procrect = ChanelRC;
	procrect.right = procrect.left + ChanelRC.Width()*m_nProgress / 100;
	bkMemDC.FillRect(ChanelRC, &allBrush);
	bkMemDC.FillRect(procrect, &proBrush);


	bmpBK.DeleteObject();
	memDC.DeleteDC();

	memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(&bmp);
	//绘制滑块
	bkMemDC.StretchBlt(ThumbRC.left, ThumbRC.top, ThumbRC.Width(), ClientRC.Height()-8,
		&memDC, 0, 0, bmpWidth, 10, SRCCOPY);
	bmp.DeleteObject();
	memDC.DeleteDC();
}
