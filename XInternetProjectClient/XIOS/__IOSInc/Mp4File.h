#ifndef _Mp4FILE_h_q32412i437o12qfdsaer
#define _Mp4FILE_h_q32412i437o12qfdsaer



#include "PlatformDefine.h"

#ifdef _PLATFORM_WINDOW
#ifndef WINAPI
#define WINAPI __stdcall
#endif
#endif

#ifdef _PLATFORM_ANDROID
#ifndef WINAPI
#define WINAPI
#endif
#endif

#ifdef _PLATFORM_IOS
#ifndef WINAPI
#define WINAPI
#endif
#endif

//mp4 = h264/h264 + aac
//mp4¼�������������£�����֡����������㹻���������壬��ע����һ��
//��--��--֡��--i֡���
//ͨ�� 1--ÿ���������8000--������С16--������16000--�ɼ������С2048
void * WINAPI Mp4File_CreateInstance4WriteH264(const char * filepath,
	int videoWidth, int videoHeight, int videoFrameRate, 
	int audioChannels, int audioSamplesPerSec, int audioBitsPerSample, int audioBitRate, int audio_frame_size);
void * WINAPI Mp4File_CreateInstance4WriteH265(const char * filepath,
	int videoWidth, int videoHeight, int videoFrameRate, 
	int audioChannels, int audioSamplesPerSec, int audioBitsPerSample, int audioBitRate, int audio_frame_size);
int WINAPI Mp4File_DestoryInstance4Write(void * pWriter);
int WINAPI Mp4File_WriteData(void * pWriter, BOOL isVideo,BOOL isVideoI, const char * data, int nLen); //дһ֡






//read file
// void * WINAPI Mp4File_CreateInstance4Read(const char * filename);//��Ӧ�÷��ؽ�������
// int WINAPI Mp4File_DestoryInstance4Read(void * pReader);
// int WINAPI Mp4File_ReadStart(void * pReader);
// int WINAPI Mp4File_ReadData(void * pReader, bool *isVideo, char * data, int *nLen);//��һ֡
// int WINAPI Mp4File_ReadStop(void * pReader);

#endif
