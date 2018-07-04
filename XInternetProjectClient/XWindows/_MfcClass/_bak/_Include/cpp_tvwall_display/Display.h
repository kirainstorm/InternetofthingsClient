#pragma once

//windows 7������ ��why��
//windows 7������ ��why��
//windows 7������ ��why��

#include <Windows.h>
#include <atltypes.h>
#include <map>
#include <vector>
using namespace std;

#include <ddraw.h>
#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib ")


#define DRAW_YUV 1 //yuv�������治��getdc
#define DRAW_RGB 0


class CDisplay
{
public:
	//GUID guid�������Կ�ͨ����guid������ddrawʹ�ø��Կ�
	//CRect DstRect������Ҫ�����ͼ������ʾ���ϵ�λ��
	//int h ,int l , int x ,int y:Ҫ�����ͼ��������Ϊƴ���Ļ�ֻҪ���һ����
	//h,l: h*l ����Ļƴ��
	//x��y��h*l ����Ļƴ�ӵ��±�����
	CDisplay(GUID guid, DEVMODEA dm, CRect DstRect, int h ,int l , int x ,int y);
	~CDisplay();
	//----------------------------------------------------------------------------------------------------------
public:
	void Release();

public:
	//nWidth����Ƶ���
	//nHeight����Ƶ�߶�
	int Init(int nWidth, int nHeight);//
	//Ҫ�����ͼ��������Ϊƴ���Ļ�ֻҪ���һ����
	//void	SetVideoSrcRect(CRect SrcRect);
	//��˸��Ļ
	void	Flash();
	//�����Ļ
	void	Refresh();
	//������Ƶ����
	long	InputData(const void* pData, long lDataLen, long lDataFmt, DWORD dwWidth, DWORD dwHeight);

	long render();
	//
	GUID GetGUID(){ return m_pDisplayGUID; };
	//----------------------------------------------------------------------------------------------------------


private:
	int		SetVideoSize(DWORD dwWidth, DWORD dwHeight);//��Ƶ�ֱ��ʱ仯�ˣ����´�����������
	int		CreateOffScreenSurface();

private:
	IDirectDraw7				*m_lpDDraw;
	IDirectDrawSurface7		*m_lpPrimaryBuffer;
	IDirectDrawSurface7		*m_lpBackBuffer;

private:
	GUID		m_pDisplayGUID;//�����Կ�ͨ����guid������ddrawʹ�ø��Կ�
	DEVMODEA	m_dm;
	CRect		m_OutDstRect;//����ʾ�������ͼ���λ��
	CRect		m_VideoSrcRect;//Ҫ�����ͼ��������Ϊƴ���Ļ�ֻҪ���һ����

	DWORD		m_nWidth;		// ��Ƶ���
	DWORD		m_nHeight;		// ��Ƶ�߶�

private:
	int m_h, m_l, m_x, m_y;


};