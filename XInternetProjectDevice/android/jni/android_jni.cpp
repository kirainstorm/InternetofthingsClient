#include "stdio.h"
#include <jni.h>
#include <pthread.h>
#include <time.h>

//---------------------------

//请注意：在android下，，任何文件的指针类型都必须初始化为空


//---------------------------
#include "PlatformDefine.h"
#include "XStream.hpp"
#include "XDevice.h"
#include "XPlayer.hpp"
#include "XPlayerFdkaac.hpp"

//---------------------------

JavaVM* g_JavaVM = NULL;


#ifdef __cplusplus
extern "C" {
#endif

	//JNIEnv* 在其他线程中必须再次获得，线程提出后必须释放，否则程序会报错挂掉。这个是android系统强制的。
	//jobject必须new一个Global的Ref出来，然后全局共享之，这样其他线程就能用了。程序退出前要释放这个。
	//
	/*
	1.在JNI_OnLoad中，保存JavaVM*，这是跨线程的，持久有效的，而JNIEnv*则是当前线程有效的。一旦启动线程，用AttachCurrentThread方法获得env。
	2.通过JavaVM*和JNIEnv可以查找到jclass。
	3.把jclass转成全局引用，使其跨线程。
	4.然后就可以正常地调用你想调用的方法了。
	5.用完后，别忘了delete掉创建的全局引用和调用DetachCurrentThread方法。

	AttachCurrentThread:把jvm加载到当前线程
	DetachCurrentThread:把jvm从当前线程剥离

	另外要注意一个jobject如果要跨线程的话，需要申请为全局引用，NewGlobalRef函数
	还有就是，android的ReferenceTable比一般JNI应用的要小，只有512个，如果不注意清理的话很有可能因为ReferenceTable溢出而程序崩掉
	所以当这个GlobalRef不再使用时，要用DeleteGlobalRef把这个GlobalRef清除掉，LocalRef也是一样的
	*/
	//----------------------------------------------------------------------
	//
	
	//for msg notify
	JNIEnv	*m_pEnvForMsg = NULL;	//JNIEnv需要在每个线程中用自己的
	jobject g_MsgObj = NULL;		//NewGlobalRef,其他线程可以使用

	//----------------------------------------------------------------------
	CrossCriticalSection m_cs;
	//CXMediaDeviceInterface *g_devInterface = NULL;
	//COpenglRenderer * render = NULL;
	//CAndroidDataCallback *pCallback = NULL;
	//--------------------------------------------
	static int WorkerThreadMessageCallback(void* param);
	CROSS_THREAD_HANDLE	m_hWorkerThreadMessage = CROSS_THREAD_NULL;
	BOOL m_bWorkerMessageStop = TRUE;
	/************************************************************************/

	class CMessageCallback :public XDeviceMessageCallback
	{
	public:
		CMessageCallback(){};
		~CMessageCallback(){};

		virtual void OnDeviceMessageCallback(const char *jsonbuffer, int len)
		{

		};
	private:
		
	};

	CFdkaac * pFdkaac = NULL;
	XDeviceInterface * pDevice = NULL;
	CMessageCallback * pMessageCallback = NULL;

	JNIEXPORT void JNICALL Java_com_device_nativecaller_DeviceNativeCaller_Init(JNIEnv *env, jobject obj, jobject msg_context_obj)
	{
		if (m_pEnvForMsg == NULL)
		{
			m_pEnvForMsg = env;
			g_MsgObj = env->NewGlobalRef(msg_context_obj);
		}
		XNetStartStreamManager();
		pMessageCallback = new CMessageCallback;
		pFdkaac = new CFdkaac();
		pFdkaac->InitVoice(TRUE);
		//
		if (m_hWorkerThreadMessage == CROSS_THREAD_NULL)
		{
			m_bWorkerMessageStop = FALSE;
			m_hWorkerThreadMessage = CrossCreateThread(WorkerThreadMessageCallback, NULL);
		}
	}
	JNIEXPORT void JNICALL Java_com_device_nativecaller_DeviceNativeCaller_Free(JNIEnv *env, jobject obj)
	{
		pFdkaac->DeinitVoice();
		pFdkaac->Delete();
		delete pMessageCallback;
		XNetStopStreamManager();
		if (m_hWorkerThreadMessage)
		{
			m_bWorkerMessageStop = TRUE;
			CrossWaitThread(m_hWorkerThreadMessage);
			CrossCloseThread(m_hWorkerThreadMessage);
			m_hWorkerThreadMessage = CROSS_THREAD_NULL;
		}
		if (m_pEnvForMsg != NULL)
		{
			m_pEnvForMsg->DeleteGlobalRef(g_MsgObj);
			m_pEnvForMsg = NULL;
			g_MsgObj = NULL;
		}
	}

	JNIEXPORT int Java_com_device_nativecaller_DeviceNativeCaller_StartService(JNIEnv * env, jobject obj, jstring ip, jstring uuid)
	{
		if(NULL == pDevice)
		{
			char *szIP, *szUuid;
			szIP = (char*)env->GetStringUTFChars(ip, 0);
			szUuid = (char*)env->GetStringUTFChars(uuid, 0);
			//
			pDevice = CreateDeviceInstance();
			pDevice->XConnect(pMessageCallback, (const char *)szIP, (const char *)szUuid);
			//
			env->ReleaseStringUTFChars(ip, szIP);
			env->ReleaseStringUTFChars(uuid, szUuid);
			//
		}

		return 0;
	}
	JNIEXPORT int Java_com_device_nativecaller_DeviceNativeCaller_StopService(JNIEnv * env, jobject obj)
	{
		if(pDevice)
		{
			pDevice->XDelete();
			pDevice = NULL;
		}
		return 0;
	}
	JNIEXPORT int Java_com_device_nativecaller_DeviceNativeCaller_GetStatus(JNIEnv * env, jobject obj)
	{
		if(pDevice)
		{
			return pDevice->XGetStatus();
		}
		return -999;
	}
	JNIEXPORT int Java_com_device_nativecaller_DeviceNativeCaller_SendMessage(JNIEnv * env, jobject obj,jstring msg)
	{
		int ret = -999;
		if(pDevice)
		{
			char *szMessage;
			szMessage = (char*)env->GetStringUTFChars(msg, 0);
			//
			ret = pDevice->XSendMessage((const char *)szMessage,strlen(szMessage));
			//
			env->ReleaseStringUTFChars(msg, szMessage);
		}
		return ret;
	}
	JNIEXPORT int Java_com_device_nativecaller_DeviceNativeCaller_SendH264PCM(JNIEnv * env, jobject obj,jint nAVFrameType ,jbyteArray data, jint len,
		jint nVideoFrameRate, jint nVideoWidth, jint nVideoHeight,
		jint nAudioChannels, jint nAudioSamplesRate, jint nAudioBitsPerSample)
	{
		int ret = -999;
		if(pDevice)
		{
			jsize lsize  = len;//env->GetArrayLength(data);
			jbyte *audioBuf = (jbyte *)malloc(lsize * sizeof(jbyte));
			env->GetByteArrayRegion(data,0,lsize,audioBuf);
			//
			uint64_t tick = CrossGetTickCount64();
			if(0 == nAVFrameType)
			{
				ret = pDevice->XSendStream(XMEDIA_FREAM_TYPE_H264_IFRAME,(const char *)audioBuf,len,tick,
					nVideoFrameRate, nVideoWidth, nVideoHeight,
					nAudioChannels, nAudioSamplesRate, nAudioBitsPerSample);
			}
			if(1 == nAVFrameType)
			{
				ret = pDevice->XSendStream(XMEDIA_FREAM_TYPE_H264_PFRAME,(const char *)audioBuf,len,tick,
					nVideoFrameRate, nVideoWidth, nVideoHeight,
					nAudioChannels, nAudioSamplesRate, nAudioBitsPerSample);
			}
			if(12 == nAVFrameType)
			{
				char szTmp[3096] = { 0 };
				int nlen = 0;
				if (0 == pFdkaac->EncodeVoice(audioBuf,len,szTmp,&nlen))
				{
					ret = pDevice->XSendStream(XMEDIA_FREAM_TYPE_AAC,(const char *)szTmp,nlen,tick,
						nVideoFrameRate, nVideoWidth, nVideoHeight,
						nAudioChannels, nAudioSamplesRate, nAudioBitsPerSample);
				}
			}
			//
			free(audioBuf);
		}
		return ret;
	}

	JNIEXPORT int Java_com_device_nativecaller_DeviceNativeCaller_SendYUV(JNIEnv * env, jobject obj,jbyteArray data, jint len,jint nVideoFrameRate, jint nVideoWidth, jint nVideoHeight)
	{
		return 0;
	}
	JNIEXPORT int Java_com_device_nativecaller_DeviceNativeCaller_SendPCM(JNIEnv * env, jobject obj,jbyteArray data, jint len,jint nAudioChannels, jint nAudioSamplesRate, jint nAudioBitsPerSample)
	{
		return 0;
	}








	static int WorkerThreadMessageCallback(void* param)
	{

#if 0
		jmethodID m_jmidAudioNotify;
		JNIEnv *m_AudioThreadEnvLocal;

#ifdef _PLATFORM_ANDROID
		JNIEnv *env;
		jclass jclsProcess;
		jmethodID jmidProcess;
		bool isAttached = false;
		int status;
		status = g_JavaVM->GetEnv((void **)&env, JNI_VERSION_1_4);
		if (status < 0) {
			status = g_JavaVM->AttachCurrentThread(&env, NULL);
			if (status < 0) {
				Log("CZYXPlayInterface::WorkerThread: AttachCurrentThread Failed!!");
				return 0;
			}
			isAttached = true;
		}

		jclsProcess = env->GetObjectClass(g_AudioObj);
		if (jclsProcess == NULL) {
			Log("CZYXPlayInterface::WorkerThread: m_envLocalCmd->GetObjectClass(g_AudioObj) error\n");
			return 0;
		}
		jmidProcess = env->GetMethodID(jclsProcess, "AudioData", "([BI)V");
		if (jmidProcess == NULL) {
			Log("m_envLocalCmd->GetMethodID(clazz, \"AudioData\", error\n");
			return 0;
		}
		m_jmidAudioNotify = jmidProcess;
		m_AudioThreadEnvLocal = env;
#endif


		void *pDecoder = NULL;
		CROSS_DWORD64 dw = CrossGetTickCount64();
		MyOnlinePlayVideoInfo *  pData = NULL;
		void *pAudioDecIns = NULL;
		CDecDataCallback * pVideoCallback = new CDecDataCallback();
		int i = 1;
		while (!m_bWorkerMessageStop)
		{
			if (m_pPause)
			{
				CrossSleep(1);
				continue;
			}
			else
			{
				pData = m_DownloadCallback.GetNextVideoFrame();
			}

			if (NULL == pData)
			{
				//���Ž���?/���ڻ���?
				CrossSleep(1);
				continue;
			}

			CROSS_DWORD64 dwAudioAndDecvideoStart = CrossGetTickCount64();
			if (!m_pPause)
			{
				if (pData->nAudioCount > 0)
				{
					if (NULL == pAudioDecIns)
					{
						pAudioDecIns = CreateAudioDecInstance(RSPLAYER_CODEC_AUDIO_TYPE_AAC);
					}



					for (int i = 0; i < pData->nAudioCount; i++)
					{
						int len = 0;
						char pszBuf[4096] = { 0 };
						AudioDecData(pAudioDecIns, pData->stAudio[i].pAudioBuffer + sizeof(ST_SERVER_HEAD), pData->stAudio[i].nAudioLen, pszBuf, &len);
						pszBuf[len] = '\0';
						//
#ifdef _PLATFORM_ANDROID
						//�ص���ȥ
						//notify pcm data
						int PCMLEN = len;
						jbyteArray jBuff;
						jBuff = m_AudioThreadEnvLocal->NewByteArray(PCMLEN);
						m_AudioThreadEnvLocal->SetByteArrayRegion(jBuff, 0, PCMLEN,
							(jbyte*)pszBuf);
						//�ص�java��֪ͨ��Ƶ����
						m_AudioThreadEnvLocal->CallVoidMethod(g_AudioObj, m_jmidAudioNotify,
							jBuff, PCMLEN);
						m_AudioThreadEnvLocal->DeleteLocalRef(jBuff);

#endif
					}
				}

			}

			// 		
			// 
			ST_SERVER_HEAD *pframeHead = (ST_SERVER_HEAD *)pData->pVideoBuffer;

			if (NULL == pDecoder)
			{
				int nFrameTypeTemp = RSPLAYER_CODEC_VIDEO_TYPE_H264;
				//
				if (((pframeHead->nAVFrameType == XSERVER_FREAM_TYPE_H264_IFRAME) || (pframeHead->nAVFrameType == XSERVER_FREAM_TYPE_H264_PFRAME)) && (NULL == pDecoder))
				{
					nFrameTypeTemp = RSPLAYER_CODEC_VIDEO_TYPE_H264;
				}
				//
				if (((pframeHead->nAVFrameType == XSERVER_FREAM_TYPE_H265_IFRAME) || (pframeHead->nAVFrameType == XSERVER_FREAM_TYPE_H265_PFRAME)) && (NULL == pDecoder))
				{
					nFrameTypeTemp = RSPLAYER_CODEC_VIDEO_TYPE_H265;
				}
				//
				if (NULL == pDecoder)
				{
					pDecoder = CreateFFmpegInstance(nFrameTypeTemp, pVideoCallback);
					FFmpegStart(pDecoder);
#if (defined _PLATFORM_ANDROID) || (defined _PLATFORM_IOS) || (defined _PLATFORM_WINDOW_WIN10_UWP)
					pVideoCallback->SetUserDecVideoDataCallback(pCallback, NULL);
#endif
				}
			}

			if ((pframeHead->nAVFrameType == XSERVER_FREAM_TYPE_H264_IFRAME) ||
				(pframeHead->nAVFrameType == XSERVER_FREAM_TYPE_H264_PFRAME) ||
				(pframeHead->nAVFrameType == XSERVER_FREAM_TYPE_H265_IFRAME) ||
				(pframeHead->nAVFrameType == XSERVER_FREAM_TYPE_H265_PFRAME)
				)
			{
				//CROSS_TRACE("CMediaPreviewInterface::data_callback --- 666 \n");
				FFmpegInputData(pDecoder, (char *)pData->pVideoBuffer + sizeof(ST_SERVER_HEAD), pData->nVideoLen, pframeHead->nVideoFrameRate);
			}

#ifdef _PLATFORM_WINDOW
			int needSleep = (pData->nSleep - (CrossGetTickCount64() - dwAudioAndDecvideoStart)) / 15 * 15;
#else
			int needSleep = (pData->nSleep - (CrossGetTickCount64() - dwAudioAndDecvideoStart));
#endif

			int nenc= (CrossGetTickCount64() - dwAudioAndDecvideoStart);

			if (needSleep <= 0)
			{
				needSleep = 1;
			}
			CROSS_DWORD64 mmm = CrossGetTickCount64()-1;
#ifdef _PLATFORM_WINDOW
			if (needSleep > 14)
			{
				do
				{
					CrossSleep(1);//����֡������һ��ʱ��Ų�����һ֡
				} while (((CrossGetTickCount64() - mmm)) < needSleep);
			}
#else
			do
			{
				CrossSleep(1);//����֡������һ��ʱ��Ų�����һ֡
			} while (((CrossGetTickCount64() - mmm)) < needSleep);
#endif


			//CROSS_TRACE("onlineplayback ----- needsleep=%d , enc=%d , wait:%d   realsleep=%d", pData->nSleep,nenc , needSleep, (int)(CrossGetTickCount64() - mmm));

			//TRACE("##### %d\n", GetTickCount64() - mmm);
			dw = CrossGetTickCount64();//
		}
		if (pDecoder)
		{
			FFmpegStop(pDecoder);
			DestoryFFmpegInstance(pDecoder);
			pDecoder = NULL;
		}
		delete pVideoCallback;
		if (pAudioDecIns)
		{

			DestoryAudioDecInstance(pAudioDecIns);
		}
#ifdef _PLATFORM_ANDROID
		if (isAttached)
			g_JavaVM->DetachCurrentThread();
#endif
#endif
	return 0;


	}


	
	




	jint JNI_OnLoad(JavaVM* vm, void* reserved)
	{
		//F_LOG;

		Log("JNI_OnLoad start");
		g_JavaVM = vm;


		JNIEnv* env = 0;
		jint result = -1;
		if (vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
		{
			Log("JNI_OnLoad error");
			goto bail;
		}

		result = JNI_VERSION_1_4;

	bail:
		Log("JNI_OnLoad end");
		return result;
	}

	void JNI_OnUnload(JavaVM *vm, void *reserved)
	{
		//F_LOG;
	}


#ifdef __cplusplus
}
#endif
