#pragma once

//windows 7下流畅 ，why？
//windows 7下流畅 ，why？
//windows 7下流畅 ，why？

#include <Windows.h>
#include <atltypes.h>
#include <map>
#include <vector>
using namespace std;

#include <ddraw.h>
#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib ")


#define DRAW_YUV 1 //yuv离屏表面不能getdc
#define DRAW_RGB 0


class CDisplay
{
public:
	//GUID guid：所属显卡通道的guid，创建ddraw使用该显卡
	//CRect DstRect：设置要输出的图像在显示器上的位置
	//int h ,int l , int x ,int y:要输出的图像区域，因为拼屏的话只要输出一部分
	//h,l: h*l 的屏幕拼接
	//x，y：h*l 的屏幕拼接地下标索引
	CDisplay(GUID guid, DEVMODEA dm, CRect DstRect, int h ,int l , int x ,int y);
	~CDisplay();
	//----------------------------------------------------------------------------------------------------------
public:
	void Release();

public:
	//nWidth：视频宽度
	//nHeight：视频高度
	int Init(int nWidth, int nHeight);//
	//要输出的图像区域，因为拼屏的话只要输出一部分
	//void	SetVideoSrcRect(CRect SrcRect);
	//闪烁屏幕
	void	Flash();
	//清除屏幕
	void	Refresh();
	//输入视频数据
	long	InputData(const void* pData, long lDataLen, long lDataFmt, DWORD dwWidth, DWORD dwHeight);

	long render();
	//
	GUID GetGUID(){ return m_pDisplayGUID; };
	//----------------------------------------------------------------------------------------------------------


private:
	int		SetVideoSize(DWORD dwWidth, DWORD dwHeight);//视频分辨率变化了，重新创建离屏表面
	int		CreateOffScreenSurface();

private:
	IDirectDraw7				*m_lpDDraw;
	IDirectDrawSurface7		*m_lpPrimaryBuffer;
	IDirectDrawSurface7		*m_lpBackBuffer;

private:
	GUID		m_pDisplayGUID;//所属显卡通道的guid，创建ddraw使用该显卡
	DEVMODEA	m_dm;
	CRect		m_OutDstRect;//在显示器上输出图像的位置
	CRect		m_VideoSrcRect;//要输出的图像区域，因为拼屏的话只要输出一部分

	DWORD		m_nWidth;		// 视频宽度
	DWORD		m_nHeight;		// 视频高度

private:
	int m_h, m_l, m_x, m_y;


};