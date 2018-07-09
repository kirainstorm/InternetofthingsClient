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
	int channel[128];//所占用的窗口，从0开始，如果占用，则为1
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



//请注意：
//1：暂时只支持未分割的屏幕合并
//2：屏幕索引lChannel排列如下
//   1   2   3
//   4   5   6
//   7   8   9

#define MAX_DECODE_CHANNEL 64
enum emSCREENTYPE
{
	SCREEN_TYPE_SINGLE = 0,//当前屏幕被独立占用并且是单画面
	SCREEN_TYPE_FOURE,//当前屏幕被独立占用并且是4画面
	SCREEN_TYPE_UNITE_SINGLE,//当前屏幕属于某个组并且被独立占用
};
#define MAX_DISPALY_SET 32
struct stChannelParam
{
	GUID guid;
	DEVMODEA dm;
	char name[512];
	int channel;//当前GUID对应的屏幕索引

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
//显示窗口配置
struct stViewParam
{
	emSCREENTYPE type;
	DWORD64 tick;//唯一标识
	int channel;
	int subchannel;//当type==SCREEN_TYPE_FOURE时有效
	int otherchannel[128];//当type==SCREEN_TYPE_UNITE_SINGLE时有效


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
	//获取解码通道数
	long GetChannelCount(long * count, stDisplayInfo &st);

#ifdef TYPE_FOR_CONFIG	//给屏幕排序使用
	
	long InitChannelDecoder();

	long DestroyChannelDecoder();

	long FreshChannel(GUID guid);

	long SetChannelIndex(GUID guid,int nIndex);

	long SaveChannelGuidMapping();
#endif



#ifdef TYPE_FOR_SERVER
	//设置映射关系
	//该函数会检测各个guid是否有效
	long LoadChannelGuidMapping(int hang , int lie);
	long InitPlayView(stPlayView stView);
	long DestroyPlayView();

	//输入显示数据
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
	long m_nChannelCount;//扩展屏数量
	//map<DWORD64, stDisplaySet *> m_mapDisplay;//这里的DWORD64 为 stViewParam的tick

	int m_hang, m_lie;
private:
	stChannelParam m_channelset[MAX_DECODE_CHANNEL];

	stPlayView m_stPlayView;
private:
#ifdef TYPE_FOR_CONFIG	//给屏幕排序使用
	CDisplay * m_pDisplay[MAX_DECODE_CHANNEL];
#endif

private:
	inline BOOL CompareGUID(const GUID guid1, const GUID  guid2)
	{
		return !memcmp(&guid1, &guid2, sizeof(GUID));
	}


	
};