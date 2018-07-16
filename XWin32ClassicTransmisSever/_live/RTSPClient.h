#ifndef RTSP_CLIENT_H
#define RTSP_CLIENT_H

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
typedef void (* AVCallback) (unsigned char* data, int len, int type,  int iframe,  struct timeval timestamp);
extern void RTSPClientDelete();
extern int RTSPClientInit(AVCallback pAVFun );
extern int RTSPClientStart(char* rtspUrl );
extern int RTSPClientStop( );
#if 0
extern int RTSPClientStart(char* rtspUrl, AVCallback pAVFun);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* RTSP_CLIENT_H */


