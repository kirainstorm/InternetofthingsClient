#pragma once

#include "Display.h"
#include <Windows.h>
#include <atltypes.h>
#include <ddraw.h>
#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dxgi.lib")
#include <map>
#include <vector>
#include <vector>
using namespace std;

#define TYPE_FOR_CONFIG

//#ifndef TYPE_FOR_CONFIG
#define TYPE_FOR_SERVER
//#endif

/////////////////////////////////////////////////////////
#define MAX_PLAY_WINDOWS_CDGHGVB  1024
#define JHGSD_FILE  _T("D:\\mzyx.info")
struct __view
{
	bool used;

	int id;
	//int dviid;
	int m_ldb;
	CRect oldrc;

	int max_view;

	int devid[4];
	char szInfo[4][256];





	//----------------------------------------------------
	int channel[128];//��ռ�õĴ��ڣ���0��ʼ�����ռ�ã���Ϊ1
	//-----------------------------------------------------
};


struct __old_infomation
{
	int nHang;
	int nLie;
	int xResolution;
	int yResolution;

	__view view[MAX_PLAY_WINDOWS_CDGHGVB];
};
/////////////////////////////////////////////////////////////////////////////



//��ע�⣺
//1����ʱֻ֧��δ�ָ����Ļ�ϲ�
//2����Ļ����lChannel��������
//   1   2   3
//   4   5   6
//   7   8   9

#define MAX_DECODE_CHANNEL 64
enum emSCREENTYPE
{
	SCREEN_TYPE_SINGLE = 0,//��ǰ��Ļ������ռ�ò����ǵ�����
	SCREEN_TYPE_FOURE,//��ǰ��Ļ������ռ�ò�����4����
	SCREEN_TYPE_UNITE_SINGLE,//��ǰ��Ļ����ĳ���鲢�ұ�����ռ��
};
#define MAX_DISPALY_SET 32
struct stChannelParam
{
	GUID guid;
	DEVMODEA dm;
	char name[512];
	int channel;//��ǰGUID��Ӧ����Ļ����

	BOOL ok;
};
struct stDisplayInfo
{
	stChannelParam play[MAX_DISPALY_SET];
};
//struct stDisplaySet
//{
//	CDisplay * play[MAX_DISPALY_SET];
//};
//-------------------------------------------------------------------
struct stChannelMapping
{
	int count;
	stChannelParam param[128];
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
//��ʾ��������
struct stViewParam
{
	emSCREENTYPE type;
	DWORD64 tick;//Ψһ��ʶ
	int channel;
	int subchannel;//��type==SCREEN_TYPE_FOUREʱ��Ч
	int otherchannel[128];//��type==SCREEN_TYPE_UNITE_SINGLEʱ��Ч


	HWND hwd;

	vector<CDisplay *> m_mapDisplay;
	
};

struct stPlayView
{
	int count;
	stViewParam param[128];
};
//-------------------------------------------------------------------

class CDecodeCard_DS
{
public:
	CDecodeCard_DS();
	~CDecodeCard_DS();

public:
	//��ȡ����ͨ����
	long GetChannelCount(long * count, stDisplayInfo &st);

#ifdef TYPE_FOR_CONFIG	//����Ļ����ʹ��
	
	long InitChannelDecoder();

	long DestroyChannelDecoder();

	long FreshChannel(GUID guid);

	long SetChannelIndex(GUID guid,int nIndex);

	long SaveChannelGuidMapping();
#endif



#ifdef TYPE_FOR_SERVER
	//����ӳ���ϵ
	//�ú����������guid�Ƿ���Ч
	long LoadChannelGuidMapping(int hang , int lie);
	long InitPlayView(stPlayView stView);
	long DestroyPlayView();

	//������ʾ����
	long InputData(stViewParam stView, const void* pData, long lDataLen, long lDataFmt, DWORD dwWidth, DWORD dwHeight, const wchar_t* szOutText = NULL, int nTextLen = 0);
	long FreshData(stViewParam stView);
#endif



private:
	emSCREENTYPE GetChannelType(long lChannel);
	int GetChannelIndex(int ch);
private:
	int GetDevMode(CStringA DevName, DEVMODEA& dm);
	static BOOL WINAPI DDEnumCallbackEx(GUID FAR *lpGUID, LPSTR lpDriverDescription, LPSTR lpDriverName, LPVOID lpContext, HMONITOR hm);
	//long IsChannelSet(long lChannel);
	//void SetDisplay(long lChannel, DWORD dwWidth, DWORD dwHeight);

private:
	int m_nErrorCode;
	long m_nChannelCount;//��չ������
	//map<DWORD64, stDisplaySet *> m_mapDisplay;//�����DWORD64 Ϊ stViewParam��tick

	int m_hang, m_lie;
private:
	stChannelParam m_channelset[MAX_DECODE_CHANNEL];

	stPlayView m_stPlayView;
private:
#ifdef TYPE_FOR_CONFIG	//����Ļ����ʹ��
	CDisplay * m_pDisplay[MAX_DECODE_CHANNEL];
#endif

private:
	inline BOOL CompareGUID(const GUID guid1, const GUID  guid2)
	{
		return !memcmp(&guid1, &guid2, sizeof(GUID));
	}


	
};