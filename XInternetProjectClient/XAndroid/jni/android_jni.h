#include <jni.h>
#ifndef _Included_com_warning_jni_HelloWorld_plat_satdf 
#define _Included_com_warning_jni_HelloWorld_plat_satdf
#ifdef __cplusplus
extern "C" 
{
#endif

//init
JNIEXPORT void JNICALL Java_com_platform_nativecaller_NativeCaller_Init(JNIEnv *env ,jobject obj)
JNIEXPORT void JNICALL Java_com_platform_nativecaller_NativeCaller_Free(JNIEnv *, jobject);
//msg notify
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_SetMsgNotify(JNIEnv * env, jobject obj, jobject context_obj);
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_ResetMsgNotify(JNIEnv * env, jobject obj);

//opengl
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_openglInit(JNIEnv *env, jobject obj);
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_openglViewport(JNIEnv *env, jobject obj, jint x, jint y, jint width, jint height);
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_openglStep(JNIEnv *env, jobject obj);
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_openglStop(JNIEnv *env, jobject obj);
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_openglPause(JNIEnv *env, jobject obj);
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_openglResume(JNIEnv *env, jobject obj);
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_openglClear(JNIEnv *env, jobject obj);

//
JNIEXPORT jobjectArray Java_com_platform_nativecaller_NativeCaller_GetDeviceList(JNIEnv * env, jobject obj, jstring ip, jint port, jstring user, jstring pwd);

//real-play
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_StartLivestream(JNIEnv *env, jobject obj, jint devid, jint channel, jstring ip, jint port, jstring user, jstring pwd);
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_StopLivestream(JNIEnv *env, jobject obj);
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_CtrlLivestreamAudio(JNIEnv *env, jobject obj, jobject context_obj, jint open);
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_ChannelRecord(JNIEnv *env, jobject obj, jstring filepath, jstring devname, jint open);
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_CtrlPTZ(JNIEnv *, jobject, jint devid, jint ptzID, jint param, jint val);
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_CapPic(JNIEnv *env, jobject obj, jstring filepath, jstring fileName);

//playback
//JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_StartLocalPlayback(JNIEnv *env, jobject obj, jobject context_obj, jstring file);
//JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_StopLocalPlayback(JNIEnv *env, jobject obj);
//JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_CtrlLocalPlayback(JNIEnv *env, jobject obj, jint type, jint val);

//
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_GetLivestreamStatus(JNIEnv *env, jobject obj);

//talk
//JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_SendTalk(JNIEnv *, jobject, byte[] buffer,jint len);


JNIEXPORT jobjectArray Java_com_platform_nativecaller_NativeCaller_GetHisLogList(JNIEnv * env, jobject obj, jstring ip, jint port, jstring user, jstring pwd,jint devid,jint year ,jint mon , jint day ,jint starthour,jint startmin);


JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_OpenHisLogOnlinePlay(JNIEnv * env, jobject obj, jobject context_obj, jstring ip, int port, jstring user, jstring pwd, jint devid,
	jint startyear, jint startmon, jint startday, jint starthour, jint startmin, jint startsec,
	jint endyear, jint endmon, jint endday, jint endhour, jint endmin, jint endsec);
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_CloseHisLogOnlinePlay(JNIEnv * env, jobject obj);
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_HisLogPause(JNIEnv * env, jobject obj, jint bPause);
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_HisLogSeek(JNIEnv * env, jobject obj, jint bPos);

JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_GetHisLogProgress(JNIEnv * env, jobject obj);//???????
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_GetHisLogPlayingProgress(JNIEnv * env, jobject obj);//???????
JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_GetHisLogOnlineDownProgress(JNIEnv * env, jobject obj);//???????

JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_HisLogWriteMp4File(JNIEnv * env, jobject obj, jstring filepath, jstring filepathEx);


#ifdef __cplusplus
}
#endif

#endif


