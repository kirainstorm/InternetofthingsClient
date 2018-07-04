#ifndef _LIBRTMP_APP_H_3241dvser1223412
#define _LIBRTMP_APP_H_3241dvser1223412

#ifndef NULL
#define NULL	0
#endif
#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif


typedef struct _NaluUnit  
{  
	unsigned int type;    
	unsigned int parsesize;
	unsigned int targetsize; 
	unsigned char *targetdata;  
	unsigned int size; 
	unsigned char *data;  
}NaluUnit_t;

typedef struct _RTMPMetadata  
{  
	// video, must be h264 type   
	unsigned int    nWidth;  
	unsigned int    nHeight;  
	unsigned int    nFrameRate;      
	unsigned int    nSpsLen;  
	unsigned char   *Sps;  
	unsigned int    nPpsLen;  
	unsigned char   *Pps;   
} RTMPMetadata_t;  

typedef struct _RtmpApp_t{
	RTMP *Rtmp;
	RTMPMetadata_t MetaTags;
}RtmpApp_t;

 
class CLibRTMPApp
{
public:
	CLibRTMPApp();
	~CLibRTMPApp();
public:
	//所有函数返回0表示成功
	int RTMP264_Connect(const char* url);
	
	int RTMP264_SendH264Packet(unsigned char *data, unsigned int size, int bIsKeyFrame, unsigned int nTimeStamp);
	int RTMP264_SendAACPacket(unsigned char *data, unsigned int size, unsigned int nSampleRate, unsigned int nTimeStamp);
	int RTMP264_RTMP264_Close();
private:
	RtmpApp_t *rtmpapp;
private:
	int ParseH264NaluData(NaluUnit_t *nalu);
	int SendVideoSpsPps(unsigned char * sps, int sps_len, unsigned char *pps, int pps_len);
	int SendAACASCPacket(unsigned char *data, unsigned int size, unsigned int nSampleRate, unsigned int nTimeStamp);
};




#endif
