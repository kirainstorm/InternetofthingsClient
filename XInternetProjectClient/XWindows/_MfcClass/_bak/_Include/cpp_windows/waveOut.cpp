#include "StdAfx.h"
#include "waveOut.h"

CWaveOut::CWaveOut()
{


	m_hWavOut					= NULL;
}

CWaveOut::~CWaveOut()
{
	Close();
}

void __stdcall mywaveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance,DWORD dwParam1, DWORD dwParam2)
{
	if (uMsg == WOM_DONE)
	{
		WAVEHDR *pWavhdr = (WAVEHDR *)dwParam1;
		CWaveOut *pWvo = (CWaveOut *)dwInstance;
		pWvo->m_cs.Lock();
		pWvo->m_buf_free.push_back((char *)pWavhdr);
		pWvo->m_cs.Unlock();
	}
}

int CWaveOut::input(unsigned char* buf , int videoLen)
{
	size_t nSize = m_buf_free.size();
	if (nSize == 0)
	{
		TRACE("!!!~~~~~### %d - %d\n",videoLen,m_buf_free.size());
		return 0;
	}

// 	ASSERT(videoLen <= MAX_AU_SIZE);
// 	if ( videoLen > MAX_AU_SIZE )
// 	{
// 		TRACE("!!!videoLen error %d - %d\n",videoLen,m_buf_free.size());
// 		return 0;
// 	}

	m_cs.Lock();
	if (m_buf_free.size() <= 0)
	{
		m_cs.Unlock();
		return 0;
	}
	WAVEHDR *pWavehdr = (WAVEHDR *)m_buf_free.front();
	m_buf_free.pop_front();
	m_cs.Unlock();


	//memset(pWavehdr->lpData, sizeof(pWavehdr->lpData), 0);
	memcpy(pWavehdr->lpData, buf, videoLen);
	waveOutWrite(m_hWavOut, pWavehdr, sizeof(WAVEHDR));



	return 0;
}
BOOL CWaveOut::Open(int bufsize, int nChannels, int nSamplesPerSec, int wBitsPerSample, int cbSize)
{
	if (m_hWavOut != NULL)
	{
		return TRUE;
	}

	/*

	TWaveFormatEx = packed record
	wFormatTag: Word;       {ָ����ʽ����; Ĭ�� WAVE_FORMAT_PCM = 1;}
	nChannels: Word;        {ָ���������ݵ�ͨ����; ������Ϊ 1, ������Ϊ 2}
	nSamplesPerSec: DWORD;  {ָ����������(ÿ���������)}
	nAvgBytesPerSec: DWORD; {ָ�����ݴ����ƽ������(ÿ����ֽ���)}
	nBlockAlign: Word;      {ָ�������(��λ�ֽ�), ����������ݵ���С��λ}
	wBitsPerSample: Word;   {������С(�ֽ�)}
	cbSize: Word;           {������Ϣ��С; PCM ��ʽû����ֶ�}
	end;
	{16 λ������ PCM �Ŀ������ 4 �ֽ�(ÿ������2�ֽ�, 2��ͨ��)}
	*/
	m_waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	m_waveFormat.nChannels = nChannels;// 1;
	m_waveFormat.nSamplesPerSec = nSamplesPerSec;// 8000;
	m_waveFormat.wBitsPerSample = wBitsPerSample;// 16;
	m_waveFormat.nAvgBytesPerSec = wBitsPerSample*nChannels*nSamplesPerSec / 8;// 16000;
	m_waveFormat.nBlockAlign = wBitsPerSample*nChannels/8;// 2;
	m_waveFormat.cbSize = cbSize;// 0;

	/*
	waveOutOpen(
	lphWaveOut: PHWaveOut;   {���ڷ����豸�����ָ��; ��� dwFlags=WAVE_FORMAT_QUERY, ����Ӧ�� nil}
	uDeviceID: UINT;         {�豸ID; ����ָ��Ϊ: WAVE_MAPPER, ������������ݸ����Ĳ��θ�ʽѡ����ʵ��豸}
	lpFormat: PWaveFormatEx; {TWaveFormat �ṹ��ָ��; TWaveFormat ����Ҫ����Ĳ��θ�ʽ}
	dwCallback: DWORD        {�ص�������ַ�򴰿ھ��; ����ʹ�ûص�����, ��Ϊ nil}
	dwInstance: DWORD        {���ص�������ʵ������; �����ڴ���}
	dwFlags: DWORD           {��ѡ��}
	)
	*/
	MMRESULT nRet = waveOutOpen(&m_hWavOut, WAVE_MAPPER, &m_waveFormat, (DWORD)mywaveOutProc, (DWORD) this, CALLBACK_FUNCTION);
	if (nRet != MMSYSERR_NOERROR)
	{
		return FALSE;
	}


	for (int i = 0; i < BUFFER_SIZE; ++i)
	{
		m_buf[i] = new char[bufsize + sizeof(WAVEHDR)];


		WAVEHDR *pWavhdr		= (WAVEHDR *)m_buf[i];
		pWavhdr->dwBufferLength = bufsize;
		pWavhdr->lpData			= m_buf[i] + sizeof(WAVEHDR);
		pWavhdr->dwFlags		= 0;

		waveOutPrepareHeader(m_hWavOut, pWavhdr, sizeof(WAVEHDR));
		m_buf_free.push_back((char *)pWavhdr);
	}


//	m_nTemp = 0;
	return TRUE;
}

BOOL CWaveOut::Close()
{
	if (m_hWavOut != NULL)
	{
		while (m_buf_free.size() != BUFFER_SIZE)
			Sleep(80);
		m_buf_free.clear();
		waveOutPause(m_hWavOut);
		waveOutReset(m_hWavOut);

		for (int i=0; i<BUFFER_SIZE; ++i)
		{
			WAVEHDR *pWavhdr = (WAVEHDR *)m_buf[i];
			waveOutUnprepareHeader(m_hWavOut, pWavhdr, sizeof(WAVEHDR));
			delete[]m_buf[i];
		}

		waveOutClose(m_hWavOut);
		m_hWavOut = NULL;

	}
		
	return TRUE;
}
