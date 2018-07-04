// Mp4File.cpp : Defines the exported functions for the DLL application.
//

#ifndef MP4_FILE_HPP_ASDN23ENDSJHD23ENMDSNCJBXZCNJSD
#define MP4_FILE_HPP_ASDN23ENDSJHD23ENMDSNCJBXZCNJSD


#include "PlatformDefine.h"


#include "Mp4Writer.hpp"

#ifdef _PLATFORM_WINDOW
#include "Mp4Reader.hpp"
#endif

//write file
inline void * Mp4File_CreateInstance4WriteH264(const char * filepath,
	int videoWidth, int videoHeight, int videoFrameRate, 
	int audioChannels, int audioSamplesPerSec, int audioBitsPerSample, int audioBitRate, int audio_frame_size)
{
	return new CMp4Writer(WRITE_TYPE_H264, filepath, videoWidth, videoHeight, videoFrameRate, videoFrameRate, audioChannels, audioSamplesPerSec, audioBitsPerSample, audioBitRate, audio_frame_size);
};
// void * WINAPI Mp4File_CreateInstance4WriteH265(const char * filepath,
// 	int videoWidth, int videoHeight, int videoFrameRate, 
// 	int audioChannels, int audioSamplesPerSec, int audioBitsPerSample, int audioBitRate, int audio_frame_size)
// {
// 	return new CMp4Writer(WRITE_TYPE_H265, filepath, videoWidth, videoHeight, videoFrameRate, videoFrameRate, audioChannels, audioSamplesPerSec, audioBitsPerSample, audioBitRate, audio_frame_size);
// }
inline int Mp4File_DestoryInstance4Write(void * pWriter)
{
	if (pWriter)
	{
		(delete (CMp4Writer *)pWriter);
	}
	return 0;
};
inline int Mp4File_WriteData(void * pWriter, BOOL isVideo, BOOL isVideoI, const char * data, int nLen)
{
	if (pWriter)
	{
		return ((CMp4Writer *)pWriter)->WriteData(isVideo,isVideoI, data, nLen);
	}
	return 0;
};

//read file
// void * WINAPI Mp4File_CreateInstance4Read(const char * filename)
// {
// 	return new CMp4Reader(filename);
// }
// int WINAPI Mp4File_DestoryInstance4Read(void * pReader)
// {
// 	if (pReader)
// 	{
// 		(delete (CMp4Reader *)pReader);
// 	}
// 
// 	return 0;
// }
// int WINAPI Mp4File_ReadData(void * pReader, bool *isVideo, char * data, int *nLen)
// {
// 	if (pReader)
// 	{
// 		return ((CMp4Reader *)pReader)->ReadData(isVideo, data, nLen);
// 	}
// 	return 0;
// }
// int WINAPI Mp4File_ReadStart(void * pReader)
// {
// 	if (pReader)
// 	{
// 		return ((CMp4Reader *)pReader)->ReadStart();
// 	}
// 	return 0;
// }
// int WINAPI Mp4File_ReadStop(void * pReader)
// {
// 	if (pReader)
// 	{
// 		return ((CMp4Reader *)pReader)->ReadStop();
// 	}
// 	return 0;
// }


#endif