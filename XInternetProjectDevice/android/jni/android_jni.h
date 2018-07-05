#include <jni.h>
#ifndef _Included_com_warning_jni_HelloWorld_plat_satdf 
#define _Included_com_warning_jni_HelloWorld_plat_satdf
#ifdef __cplusplus
extern "C" 
{
#endif

//init//msg notify
JNIEXPORT void JNICALL Java_com_device_nativecaller_DeviceNativeCaller_Init(JNIEnv *env ,jobject obj, jobject msg_context_obj)
JNIEXPORT void JNICALL Java_com_device_nativecaller_DeviceNativeCaller_Free(JNIEnv *, jobject);


JNIEXPORT int Java_com_device_nativecaller_DeviceNativeCaller_StartService(JNIEnv * env, jobject obj, jstring ip, jstring uuid);
JNIEXPORT int Java_com_device_nativecaller_DeviceNativeCaller_StopService(JNIEnv * env, jobject obj);
JNIEXPORT int Java_com_device_nativecaller_DeviceNativeCaller_GetStatus(JNIEnv * env, jobject obj);
JNIEXPORT int Java_com_device_nativecaller_DeviceNativeCaller_SendMessage(JNIEnv * env, jobject obj,jstring msg);
JNIEXPORT int Java_com_device_nativecaller_DeviceNativeCaller_SendH264PCM(JNIEnv * env, jobject obj,jint nAVFrameType ,jbyteArray data, jint len,
			jint nVideoFrameRate, jint nVideoWidth, jint nVideoHeight,
		jint nAudioChannels, jint nAudioSamplesRate, jint nAudioBitsPerSample);

JNIEXPORT int Java_com_device_nativecaller_DeviceNativeCaller_SendYUV(JNIEnv * env, jobject obj,jbyteArray data, jint len,jint nVideoFrameRate, jint nVideoWidth, jint nVideoHeight);
JNIEXPORT int Java_com_device_nativecaller_DeviceNativeCaller_SendPCM(JNIEnv * env, jobject obj,jbyteArray data, jint len,jint nAudioChannels, jint nAudioSamplesRate, jint nAudioBitsPerSample);

#ifdef __cplusplus
}
#endif

#endif


