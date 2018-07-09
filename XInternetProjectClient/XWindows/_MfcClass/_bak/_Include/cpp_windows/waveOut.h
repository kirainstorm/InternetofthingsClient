#ifndef __WAV_OUT_H__
#define __WAV_OUT_H__

#pragma once
#include <afxmt.h>
#include <mmsystem.h>
#include <list>
using namespace std;

#define BUFFER_SIZE		(32)
#pragma comment(lib,"Winmm")

//#define MAX_AU_SIZE (2000)//这里要改，，！！！！一定是对方传过来的数据的大小



//#define BLOCK_SIZE		(MAX_AU_SIZE + sizeof(WAVEHDR))

class CWaveOut
{
public:
	
	//char szTempBuff[2048];
	//int m_nTemp;
public:
	WAVEFORMATEX	m_waveFormat;
	HWAVEOUT		m_hWavOut;

public:
	char*			m_buf[BUFFER_SIZE];// [BLOCK_SIZE];
	list<char*>		m_buf_free;
	CCriticalSection m_cs;
	

	CWaveOut();
	virtual ~CWaveOut();

	BOOL Open(int bufsize, int nChannels, int nSamplesPerSec, int wBitsPerSample, int cbSize=0);
	BOOL Close();

	int input(unsigned char *buf , int videoLen);
	BOOL inlineis_start() { return (m_hWavOut != NULL); }
private:


};

#endif 


