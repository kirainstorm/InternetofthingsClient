#include "stdio.h"
#include <jni.h>
#include <pthread.h>
#include <time.h>

//---------------------------

//请注意：在android下，，任何文件的指针类型都必须初始化为空


//---------------------------
#include "XPlayer.hpp"
#include "XPlayerOpenGLRenderer.hpp"
#include "AndroidDataCallback.h"
#include "XStream.hpp"
//---------------------------


#include "XClientStructDefine.h"
#include "ClassicClientInstance.h"
#include "IOTClientInstance.h"
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
	//for audio thread
	JNIEnv	*m_pEnvForAudio = NULL;
	jobject g_AudioObj = NULL;
	//for play-back
	JNIEnv	*m_pEnvForPlayback = NULL;
	jobject g_PlaybackObj = NULL;

	//----------------------------------------------------------------------
	CrossCriticalSection m_cs;
	//
	CXClientInferface * pInstance = nullptr;
	COpenglRenderer * render = NULL;
	CAndroidDataCallback *pCallback = NULL;
	CXClientStreamInterface * pClientStreamInterface = nullptr;
	//
	char m_szIP[128],m_szUser[128],m_szPwd[128];

	//
	//--------------------------------------------
	//CDownloadCallback m_DownloadCallback;
	CROSS_THREAD_HANDLE	m_hWorkerThreadRealtime = CROSS_THREAD_NULL;
	BOOL m_bWorkerRealtimeStop = TRUE;
	BOOL m_pPause = FALSE;
	/************************************************************************/
	class CSignalingMessageCallback : public CXSignalingChannelMessageCallback
	{
	public:
		CSignalingMessageCallback(){
			
		};
		~CSignalingMessageCallback(){
			
		};
	public:
		virtual void OnSignalingChannelMessageCallback(const char * jsondata, int datalen)
		{

		};
	};
	CSignalingMessageCallback *	m_pSignalingMessageCallback = nullptr;

	JNIEXPORT void JNICALL Java_com_platform_nativecaller_NativeCaller_Init(JNIEnv *env, jobject obj)
	{
		XNetStartStreamManager();
		render = new COpenglRenderer();
		pCallback = new CAndroidDataCallback(render);
		m_pSignalingMessageCallback = new CSignalingMessageCallback;
		//
		// if (type == X_CLIENT_XXX_TYPE_CLASSIC)
		{
			pInstance = new CXClassicClientInstance();
		}
		// else
		// {
		// 	pInstance = new CXIotClientInstance();
		// }
	}
	JNIEXPORT void JNICALL Java_com_platform_nativecaller_NativeCaller_Free(JNIEnv *env, jobject obj)
	{
		pInstance->XDelete();
		delete render;
		delete pCallback;
		delete m_pSignalingMessageCallback;
		XNetStopStreamManager();
	}

	JNIEXPORT void JNICALL Java_com_platform_nativecaller_NativeCaller_SetInfo(JNIEnv *env, jobject obj, jstring ip, jint port, jstring user, jstring pwd)
	{
		char *szIP, *szUser, *szPwd;
		szIP = (char*)env->GetStringUTFChars(ip, 0);
		szUser = (char*)env->GetStringUTFChars(user, 0);
		szPwd = (char*)env->GetStringUTFChars(pwd, 0);
		//m_szIP[128],m_szUser[128],m_szPwd[128];
		memset(m_szIP,0,sizeof(m_szIP));
		memset(m_szUser,0,sizeof(m_szUser));
		memset(m_szPwd,0,sizeof(m_szPwd));
		//
		CROSS_STRCPY(m_szIP,szIP);
		CROSS_STRCPY(m_szUser,szUser);
		CROSS_STRCPY(m_szPwd,szPwd);
		//
		env->ReleaseStringUTFChars(ip, szIP);
		env->ReleaseStringUTFChars(user, szUser);
		env->ReleaseStringUTFChars(pwd, szPwd);
		//
		pInstance->InitInstance(m_szIP, m_szUser, m_szPwd, m_pSignalingMessageCallback);
	}

	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_openglInit(JNIEnv *env, jobject obj)
	{
		render->init();
		return 0;
	}
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_openglViewport(JNIEnv *env, jobject obj, jint x, jint y, jint width, jint height)
	{
		render->Viewport(x,y,width, height);
		return 0;
	}
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_openglStep(JNIEnv *env, jobject obj)
	{
		render->step();
		return 0;
	}
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_openglStop(JNIEnv *env, jobject obj)
	{
		render->destroy();
		return 0;
	}
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_openglPause(JNIEnv *env, jobject obj)
	{
		render->Pause();
		return 0;
	}
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_openglResume(JNIEnv *env, jobject obj)
	{
		render->Resume();
		return 0;
	}
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_openglClear(JNIEnv *env, jobject obj)
	{
		render->clear();
		return 0;
	}



	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_SetMsgNotify(JNIEnv * env, jobject obj, jobject context_obj)
	{
		if (m_pEnvForMsg == NULL)
		{
			m_pEnvForMsg = env;
			g_MsgObj = env->NewGlobalRef(context_obj);
		}
		return 0;
	}
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_ResetMsgNotify(JNIEnv * env, jobject obj)
	{
		if (m_pEnvForMsg != NULL)
		{
			m_pEnvForMsg->DeleteGlobalRef(g_MsgObj);
			m_pEnvForMsg = NULL;
			g_MsgObj = NULL;
		}
		return 0;
	}





	JNIEXPORT jobjectArray Java_com_platform_nativecaller_NativeCaller_GetDeviceListClassic(JNIEnv * env, jobject obj)
	{
		CROSS_TRACE("_GetDeviceList........");

		//
		jobjectArray args = 0;
#if 1

		//
		char *pRecvBuffer = new char[3 * 1024 * 1024];
		int nRecv = 0;
		//
		do 
		{
			pInstance->GetDevices(pRecvBuffer, nRecv);
			if (nRecv <= 0)
			{
				CROSS_TRACE("_GetDeviceList........ GetUserDevices = 0");
				break;
			}

			vector<ST_CLASSIC_DEVICE_INFO_BASE> r;
			for (int i = 0; i < (int)(nRecv / sizeof(ST_CLASSIC_DEVICE_INFO_BASE)); i++)
			{
				ST_CLASSIC_DEVICE_INFO_BASE st;
				memset(&st, 0, sizeof(st));
				memcpy(&st, pRecvBuffer + i*sizeof(ST_CLASSIC_DEVICE_INFO_BASE), sizeof(ST_CLASSIC_DEVICE_INFO_BASE));
				r.push_back(st);
			}

			
			
			// if (0 != CNetToServer::Instance().GetUserDevices(szIP,port,szUser,szPwd,r))
			// {
			// 	CROSS_TRACE("_GetDeviceList........ GetUserDevices Error");
			// 	break;
			// }
			// if (r.size() == 0)
			// {
			// 	CROSS_TRACE("_GetDeviceList........ GetUserDevices = 0");
			// 	break;
			// }
			//
			
			//查找java层的DeviceListItem
			jclass clazz = (env)->FindClass("com/platform/nativecaller/DeviceListItem");
			if (clazz == 0)
				break;

			// 
			jsize len = r.size();
			args = (env)->NewObjectArray(len, clazz, 0);


			//
			jfieldID fid_id = env->GetFieldID(clazz, "plat_dev_id", "I"); //if (fid_id == 0)	break;
			jfieldID fid_ispublish = env->GetFieldID(clazz, "plat_dev_ispublish", "I");
			jfieldID fid_browsenum = env->GetFieldID(clazz, "plat_dev_browsenum", "I");
			jfieldID fid_online = env->GetFieldID(clazz, "plat_dev_online", "I");
			//
			jfieldID fid_name = env->GetFieldID(clazz, "plat_dev_name", "Ljava/lang/String;");
			//
			jfieldID fid_areaid = env->GetFieldID(clazz, "plat_dev_areaid", "I");
			jfieldID fid_areaname = env->GetFieldID(clazz, "plat_dev_area_name", "Ljava/lang/String;");
			//
			jfieldID fid_mediaip = env->GetFieldID(clazz, "plat_dev_media_ip", "Ljava/lang/String;");
			//jfieldID fid_mediaport = env->GetFieldID(clazz, "plat_dev_media_port", "I");			
			//
			//jfieldID fid_playbackip = env->GetFieldID(clazz, "plat_dev_playback_ip", "Ljava/lang/String;");
			//jfieldID fid_playbackport = env->GetFieldID(clazz, "plat_dev_playback_port", "I");
			//
			//jfieldID fid_publiship = env->GetFieldID(clazz, "plat_dev_publish_ip", "Ljava/lang/String;");
			//jfieldID fid_publishport = env->GetFieldID(clazz, "plat_dev_publish_port", "I");

			//  
			for (int i = 0; i < len; i++)
			{
				//alloc ovject
				jobject _obj = env->AllocObject(clazz);//<<<<<<<<


				//SetXXXXField
				//
				(env)->SetIntField(_obj, fid_id, r[i].dev_id);
				//CROSS_TRACE("........ 1");
				(env)->SetIntField(_obj, fid_ispublish, r[i].dev_is_publish);
				//CROSS_TRACE("........ 2");
				(env)->SetIntField(_obj, fid_browsenum, r[i].dev_browse_num);
				//CROSS_TRACE("........ 3");
				(env)->SetIntField(_obj, fid_online, r[i].dev_online);
				//CROSS_TRACE("........ 4");
				(env)->SetObjectField(_obj, fid_name, (env)->NewStringUTF((const char*)r[i].dev_name));
				//CROSS_TRACE("........ 5");
				(env)->SetIntField(_obj, fid_areaid, r[i].dev_areaid);
				//CROSS_TRACE("........ 6");
				(env)->SetObjectField(_obj, fid_areaname, (env)->NewStringUTF((const char*)r[i].dev_area_name));
				//CROSS_TRACE("........ 7");
				(env)->SetObjectField(_obj, fid_mediaip, (env)->NewStringUTF((const char*)r[i].dev_media_ip));
				//CROSS_TRACE("........ 8");
				// (env)->SetIntField(_obj, fid_mediaport, r[i].dev_media_port);
				// //CROSS_TRACE("........ 9");
				// (env)->SetObjectField(_obj, fid_playbackip, (env)->NewStringUTF((const char*)r[i].dev_media_ip));
				// //CROSS_TRACE("........ 10");
				// (env)->SetIntField(_obj, fid_playbackport, r[i].dev_playback_port);
				// //CROSS_TRACE("........ 11");
				// (env)->SetObjectField(_obj, fid_publiship, (env)->NewStringUTF((const char*)r[i].dev_publish_ip));
				// //CROSS_TRACE("........ 10");
				// (env)->SetIntField(_obj, fid_publishport, r[i].dev_publish_port);
				//CROSS_TRACE("........ 11");
				//*****************************************
				//set object
				(env)->SetObjectArrayElement(args, i, _obj);
			}
			CROSS_TRACE("........ getlist size: %d", len);

		} while (0);


		delete []pRecvBuffer;

#endif
		return args;
	}
	




	//real-play
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_StartLivestream(JNIEnv *env, jobject obj, jstring uid)
	{
		int nRet = -1;

		if(pClientStreamInterface)
		{
			return -1;
		}
#if 1
		m_cs.Lock();
		do 
		{
			char *szUUID  = (char*)env->GetStringUTFChars(uid, 0);
			pClientStreamInterface = pInstance->CreateXClientStreamInterface(szUUID,0);
			pClientStreamInterface->XClientStreamSetDecodeVideoCallBack(pCallback,0);
			pClientStreamInterface->XClientStreamPlay();
			env->ReleaseStringUTFChars(uid, szUUID);
			nRet = 0;
		} while (0);
		//
		
		m_cs.Unlock();
#endif
		return nRet;
	}
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_StopLivestream(JNIEnv *env, jobject obj)
	{
#if 1
		m_cs.Lock();
		do 
		{
			if(!pClientStreamInterface)
			{
				break;
			}
			//
			pClientStreamInterface->XClientStreamClose();
			pClientStreamInterface->XDelete();
			pClientStreamInterface =nullptr;
		} while (0);
		m_cs.Unlock();
#endif
		Log("StopLivestream  ....... okokokok\n");
		return 0;
	}
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_CtrlLivestreamAudio(JNIEnv *env, jobject obj, jobject context_obj, jint open)
	{

#if 0

		m_cs.Lock();
		do
		{
			if(!pClientStreamInterface)
			{
				break;
			}

			if ((m_pEnvForAudio == NULL) && (open > 0))
			{
				Log("CtrlLivestreamAudio  ....... start\n");
				m_pEnvForAudio = env;
				g_AudioObj = env->NewGlobalRef(context_obj);
				g_devInterface->XPreviewSound(1);
			}
			else
			{
				Log("CtrlLivestreamAudio  ....... stop\n");
				g_devInterface->XPreviewSound(0);
				m_pEnvForAudio->DeleteGlobalRef(g_AudioObj);
				m_pEnvForAudio = NULL;
				g_AudioObj = NULL;
			}

		} while (0);
		m_cs.Unlock();

		Log("CtrlLivestreamAudio  .......\n");
#endif
		return 0;
	}
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_ChannelRecord(JNIEnv *env, jobject obj, jstring filepath, jstring devname, jint open)
	{
#if 1
		m_cs.Lock();
		do
		{
			if(!pClientStreamInterface)
			{
				break;
			}

			char *k = (char*)env->GetStringUTFChars(filepath, 0);
			char *f = (char*)env->GetStringUTFChars(devname, 0);
			pClientStreamInterface->XClientStreamRecord((const char*)k, (const char*)f, open == 1 ? TRUE : FALSE);
			env->ReleaseStringUTFChars(filepath, k);
			env->ReleaseStringUTFChars(devname, f);

		} while (0);
		m_cs.Unlock();
		Log("ChannelRecord  .......\n");
#endif
		return 0;
	}
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_CtrlPTZ(JNIEnv *, jobject, jint devid, jint ptzID, jint param, jint val)
	{
#if 0
		Log("CtrlPTZ  .......\n");
		if (NULL == g_devInterface)
		{
			Log("CtrlPTZ  ....... == NULL");
			return -1;
		}

		//g_devInterface->XDevicePTZControl(devid, ptzID, param, val);

#endif
		return 0;
	}
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_CapPic(JNIEnv *env, jobject obj, jstring filepath, jstring fileName)
	{
		m_cs.Lock();
		do
		{
			if(!pClientStreamInterface)
			{
				break;
			}
			char *k = (char*)env->GetStringUTFChars(filepath, 0);
			char *f = (char*)env->GetStringUTFChars(fileName, 0);
			pClientStreamInterface->XClientStreamCapturePictures((const char*)k, (const char*)f);
			env->ReleaseStringUTFChars(filepath, k);
			env->ReleaseStringUTFChars(fileName, f);

		} while (0);
		m_cs.Unlock();
		Log("CapPic  .......\n");
		return 0;
	}
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_GetLivestreamStatus(JNIEnv *env, jobject obj)
	{
		int ret = 0;		
#if 1
		//Log("GetLivestreamStatus  .......\n");
		m_cs.Lock();
		do
		{
			if(!pClientStreamInterface)
			{
				break;
			}
			emClientNetStatus em = pClientStreamInterface->XClientStreamGetStatus();
			ret = em;
		} while (0);
		m_cs.Unlock();
		//Log("GetLivestreamStatus  .......\n");
#endif
		return ret;
	}


JNIEXPORT jobjectArray Java_com_platform_nativecaller_NativeCaller_GetHisLogList(JNIEnv * env, jobject obj,
			jstring ip, jint port, jstring user, jstring pwd,jint devid,jint year ,jint mon , jint day ,jint starthour,jint startmin)
	{
				//define jobjectArray
		jobjectArray args = 0;


		#if 0
		char *szIP, *szUser, *szPwd;
		szIP = (char*)env->GetStringUTFChars(ip, 0);
		szUser = (char*)env->GetStringUTFChars(user, 0);
		szPwd = (char*)env->GetStringUTFChars(pwd, 0);

		CXMediaDeviceInterface * pDevObj = NULL;
		CXMediaDownloadInterface *pDownObj = NULL;



		do
		{
			pDevObj = new CMediaDevInterface();
			if(NULL == pDevObj)
			{
				break;
			}
// 			pDevObj->XDeviceSetInfo(
// 					szIP,
// 					port-1,
// 					szUser,
// 					szPwd,
// 					szIP,
// 					port,
// 					0,
// 					X_DEVICE_TYPE_NONE);

			//
			DeviceInfo_T t;
			memset(&t, 0, sizeof(DeviceInfo_T));
			strcpy(t.his_svr_ip, szIP);
			t.his_svr_port = port;
			strcpy(t.user_name, szUser);
			strcpy(t.user_pwd, szPwd);
			t.dev_id = devid;
			t.dev_channel = 0;
			//
			pDevObj->XDeviceSetInfo(t);




			pDownObj = pDevObj->CreateDownloadHisFileInstance();
			if (NULL == pDownObj)
			{
				break;
			}

			_XSERVER_TIME tm1, tm2;
			memset(&tm1, 0, sizeof(_XSERVER_TIME));
			memset(&tm2, 0, sizeof(_XSERVER_TIME));

			tm1.wYear = year;
			tm1.wMonth = mon;
			tm1.wDay = day;
			tm1.wHour = starthour;
			tm1.wMinute = startmin;
			tm1.wSecond = 1;

			tm2.wYear = year;
			tm2.wMonth = mon;
			tm2.wDay = day;
			tm2.wHour = 23;
			tm2.wMinute = 59;
			tm2.wSecond = 59;

			//CROSS_TRACE("GetHisLogList........ %s--%d--%s--%s--%d--%d--%d--%d--%d--%d",szIP,port,szUser,szPwd,devid,year,mon,day,starthour,startmin);

			vector<HIS_LOG_T> r;
			if (0 != pDownObj->XDownloadHisFileListFromMedia(tm1, tm2, r))
			{
				//CROSS_TRACE("GetHisLogList........ XDownloadHislogListFromMedia Error");
				break;
			}
			if (r.size() == 0)
			{
				//CROSS_TRACE("GetHisLogList........ XDownloadHislogListFromMedia = 0");
				break;
			}
			//find java "HislogListItem"
			jclass clazz = (env)->FindClass("com/platform/nativecaller/HislogListItem");
			if (clazz == 0)
				break;

			//new obj
			jsize len = r.size();
			args = (env)->NewObjectArray(len, clazz, 0);


			//��ȡ����ÿһ�������Ķ���
			jfieldID fid_id = env->GetFieldID(clazz, "his_id", "I");
			if (fid_id == 0){CROSS_TRACE("GetHisLogList........ fid_   1");}
			jfieldID fid_file = env->GetFieldID(clazz, "his_file", "Ljava/lang/String;");
			if (fid_file == 0){CROSS_TRACE("GetHisLogList........ fid_   2");}
			//

			jfieldID fid_s_1 = env->GetFieldID(clazz, "his_time_start_year", "I");
			if (fid_s_1 == 0){CROSS_TRACE("GetHisLogList........ fid_   3");}
			jfieldID fid_s_2 = env->GetFieldID(clazz, "his_time_start_mon", "I");
			if (fid_s_2 == 0){CROSS_TRACE("GetHisLogList........ fid_   4");}
			jfieldID fid_s_3 = env->GetFieldID(clazz, "his_time_start_day", "I");
			if (fid_s_3 == 0){CROSS_TRACE("GetHisLogList........ fid_   5");}
			jfieldID fid_s_4 = env->GetFieldID(clazz, "his_time_start_hour", "I");
			if (fid_s_4 == 0){CROSS_TRACE("GetHisLogList........ fid_   6");}
			jfieldID fid_s_5 = env->GetFieldID(clazz, "his_time_start_min", "I");
			if (fid_s_5 == 0){CROSS_TRACE("GetHisLogList........ fid_   7");}
			jfieldID fid_s_6 = env->GetFieldID(clazz, "his_time_start_sec", "I");
			if (fid_s_6 == 0){CROSS_TRACE("GetHisLogList........ fid_   8");}
			//
			jfieldID fid_e_1 = env->GetFieldID(clazz, "his_time_end_year", "I");
			if (fid_e_1 == 0){CROSS_TRACE("GetHisLogList........ fid_   9");}
			jfieldID fid_e_2 = env->GetFieldID(clazz, "his_time_end_mon", "I");
			if (fid_e_2 == 0){CROSS_TRACE("GetHisLogList........ fid_   10");}
			jfieldID fid_e_3 = env->GetFieldID(clazz, "his_time_end_day", "I");
			if (fid_e_3 == 0){CROSS_TRACE("GetHisLogList........ fid_   11");}
			jfieldID fid_e_4 = env->GetFieldID(clazz, "his_time_end_hour", "I");
			if (fid_e_4 == 0){CROSS_TRACE("GetHisLogList........ fid_   12");}
			jfieldID fid_e_5 = env->GetFieldID(clazz, "his_time_end_min", "I");
			if (fid_e_5 == 0){CROSS_TRACE("GetHisLogList........ fid_   13");}
			jfieldID fid_e_6 = env->GetFieldID(clazz, "his_time_end_sec", "I");
			if (fid_e_6 == 0){CROSS_TRACE("GetHisLogList........ fid_   14");}
			//

			//CROSS_TRACE("GetHisLogList........ 6");
			for (int i = 0; i < len; i++)
			{
				//alloc
				jobject _obj = env->AllocObject(clazz);//<<<<<<<</


				//
				(env)->SetIntField(_obj, fid_id, r[i].log_id);
				//CROSS_TRACE("........ 1");
				(env)->SetObjectField(_obj, fid_file, (env)->NewStringUTF(r[i].store_file));
				//CROSS_TRACE("........ 2");

				//
				(env)->SetIntField(_obj, fid_s_1, r[i].store_begin_time_tm.wYear);
				//CROSS_TRACE("........ 3");
				(env)->SetIntField(_obj, fid_s_2, r[i].store_begin_time_tm.wMonth);
				//CROSS_TRACE("........ 4");
				(env)->SetIntField(_obj, fid_s_3, r[i].store_begin_time_tm.wDay);
				//CROSS_TRACE("........ 5");
				(env)->SetIntField(_obj, fid_s_4, r[i].store_begin_time_tm.wHour);
				//CROSS_TRACE("........ 6");
				(env)->SetIntField(_obj, fid_s_5, r[i].store_begin_time_tm.wMinute);
				//CROSS_TRACE("........ 7");
				(env)->SetIntField(_obj, fid_s_6, r[i].store_begin_time_tm.wSecond);
				//CROSS_TRACE("........ 8");
				//
				(env)->SetIntField(_obj, fid_e_1, r[i].store_end_time_tm.wYear);
				//CROSS_TRACE("........ 9");
				(env)->SetIntField(_obj, fid_e_2, r[i].store_end_time_tm.wMonth);
				//CROSS_TRACE("........ 10");
				(env)->SetIntField(_obj, fid_e_3, r[i].store_end_time_tm.wDay);
				//CROSS_TRACE("........ 11");
				(env)->SetIntField(_obj, fid_e_4, r[i].store_end_time_tm.wHour);
				//CROSS_TRACE("........ 12");
				(env)->SetIntField(_obj, fid_e_5, r[i].store_end_time_tm.wMinute);
				//CROSS_TRACE("........ 13");
				(env)->SetIntField(_obj, fid_e_6, r[i].store_end_time_tm.wSecond);
				//CROSS_TRACE("........ 14");

				//*****************************************
				//set obj
				(env)->SetObjectArrayElement(args, i, _obj);
				//CROSS_TRACE("........ 15");
			}


			CROSS_TRACE("........ getlist size: %d", len);

		} while (0);
		//CROSS_TRACE("GetHisLogList........ 7");
		env->ReleaseStringUTFChars(ip, szIP);
		env->ReleaseStringUTFChars(user, szUser);
		env->ReleaseStringUTFChars(pwd, szPwd);
		//CROSS_TRACE("GetHisLogList........ 8");
		if (pDownObj)
		{
			//CROSS_TRACE("GetHisLogList........ 8 - 1");
			pDownObj->XDownloadHisFileClose();
			//CROSS_TRACE("GetHisLogList........ 8 - 2");
			pDownObj->XDownloadHisFileDelete();
			//CROSS_TRACE("GetHisLogList........ 8 - 3");
			pDownObj = NULL;
		}

		//CROSS_TRACE("GetHisLogList........ 9");
		if (pDevObj)
		{
			//pDevObj->XDeviceClose();
			pDevObj->XDeviceDelete();
			pDevObj = NULL;
		}
#endif
		//CROSS_TRACE("GetHisLogList........ 10");
		return args;
	}


	static int WorkerThreadOnlinePlayback(void*)
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
		while (!m_bWorkerRealtimeStop)
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
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_OpenHisLogOnlinePlay(JNIEnv * env, jobject obj, jobject context_obj, jstring ip, int port, jstring user, jstring pwd, jint devid,
		jint startyear, jint startmon, jint startday, jint starthour, jint startmin, jint startsec,
		jint endyear, jint endmon, jint endday, jint endhour, jint endmin, jint endsec)
	{

		int nRet = -1;
#if 0
		m_cs.Lock();

		

		char *szIP, *szUser, *szPwd;
		szIP = (char*)env->GetStringUTFChars(ip, 0);
		szUser = (char*)env->GetStringUTFChars(user, 0);
		szPwd = (char*)env->GetStringUTFChars(pwd, 0);

		do
		{

			if ((NULL != g_devInterface))
			{
				Log("OpenHisLogOnlinePlay  ....... !=NULL\n");
				break;
			}

 			Log("OpenHisLogOnlinePlay  .......\n");
			g_devInterface = new CMediaDevInterface();
			if (NULL == g_devInterface)
			{
				break;
			}

			DeviceInfo_T t;
			memset(&t, 0, sizeof(DeviceInfo_T));
			strcpy(t.his_svr_ip, szIP);
			t.his_svr_port = port;
			strcpy(t.user_name, szUser);
			strcpy(t.user_pwd, szPwd);
			t.dev_id = devid;
			t.dev_channel = 0;
			g_devInterface->XDeviceSetInfo(t);



			_XSERVER_TIME store_begin_time_tm;
			_XSERVER_TIME store_end_time_tm;

			memset(&store_begin_time_tm, 0, sizeof(_XSERVER_TIME));//////====
			memset(&store_end_time_tm, 0, sizeof(_XSERVER_TIME));

			store_begin_time_tm.wYear = startyear;
			store_begin_time_tm.wMonth = startmon;
			store_begin_time_tm.wDay = startday;
			store_begin_time_tm.wHour = starthour;
			store_begin_time_tm.wMinute = startmin;
			store_begin_time_tm.wSecond = startsec;

			store_end_time_tm.wYear = endyear;
			store_end_time_tm.wMonth = endmon;
			store_end_time_tm.wDay = endday;
			store_end_time_tm.wHour = endhour;
			store_end_time_tm.wMinute = endmin;
			store_end_time_tm.wSecond = endsec;


			//Log("+++++++++++++++   %d  %d  %d  %d  %d  %d  +++++++++++++ %d  %d  %d  %d  %d  %d  ",
			//	store_end_time_tm.wYear, store_end_time_tm.wMonth, store_end_time_tm.wDay, store_end_time_tm.wHour, store_end_time_tm.wMinute, store_end_time_tm.wSecond,
			//	store_begin_time_tm.wYear, store_begin_time_tm.wMonth, store_begin_time_tm.wDay, store_begin_time_tm.wHour, store_begin_time_tm.wMinute, store_begin_time_tm.wSecond);

			if (0 != m_DownloadCallback.Open(g_devInterface,t,store_begin_time_tm, store_end_time_tm))
			{
				Log("OpenHisLogOnlinePlay  ....... DownloadCallback.Open Error\n");
				break;
			}
			nRet = 0;
			//Log("Java_com_platform_nativecaller_NativeCaller_OpenHisLogOnlinePlay  444\n");

			m_pPause = FALSE;

			if (m_pEnvForAudio == NULL)
			{
				m_pEnvForAudio = env;
				g_AudioObj = env->NewGlobalRef(context_obj);
			}

			if (m_hWorkerThreadRealtime == CROSS_THREAD_NULL)
			{
				m_bWorkerRealtimeStop = FALSE;
				m_hWorkerThreadRealtime = CrossCreateThread(WorkerThreadOnlinePlayback, NULL);
			}
		} while (0);

		env->ReleaseStringUTFChars(pwd, szPwd);
		env->ReleaseStringUTFChars(user, szUser);
		env->ReleaseStringUTFChars(ip, szIP);

		if (0 != nRet)
		{
			if (g_devInterface)
			{
				g_devInterface->XDeviceLogout();
				g_devInterface->XDeviceDelete();
				g_devInterface = NULL;
			}

		}

		Log("OpenHisLogOnlinePlay  .......nRet = %d\n", nRet);
		m_cs.Unlock();

#endif
		return nRet;
	}
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_CloseHisLogOnlinePlay(JNIEnv * env, jobject obj)
	{

		#if 0 
		m_cs.Lock();

		do 
		{
			if ((NULL == g_devInterface))
			{
				Log("CloseHisLogOnlinePlay  ....... ==NULL\n");
				break;
			}
			Log("CloseHisLogOnlinePlay  .......\n");

			if (m_hWorkerThreadRealtime)
			{
				m_bWorkerRealtimeStop = TRUE;
				CrossWaitThread(m_hWorkerThreadRealtime);
				CrossCloseThread(m_hWorkerThreadRealtime);
				m_hWorkerThreadRealtime = CROSS_THREAD_NULL;
			}

			

			if (g_devInterface)
			{
				//g_devInterface->XDeviceClose();
				g_devInterface->XDeviceDelete();
				g_devInterface = NULL;
			}

			if (m_pEnvForAudio != NULL)
			{
				m_pEnvForAudio->DeleteGlobalRef(g_AudioObj);
				m_pEnvForAudio = NULL;
				g_AudioObj = NULL;
			}

			m_DownloadCallback.Close();


		} while (0);

		m_cs.Unlock();

#endif
		Log("CloseHisLogOnlinePlay  ....... ==ok\n");
		return 0;
	}
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_HisLogPause(JNIEnv * env, jobject obj, jint bPause)
	{
		m_pPause = !m_pPause;
		return 0;
	}
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_HisLogSeek(JNIEnv * env, jobject obj, jint bPos)
	{
		//m_DownloadCallback.SeekSecond(bPos);
		return 0;
	}

	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_GetHisLogProgress(JNIEnv * env, jobject obj)//������ֵ
	{
		//return m_DownloadCallback.GetFileSecond();
		return 0;
	}
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_GetHisLogPlayingProgress(JNIEnv * env, jobject obj)//������ֵ
	{
		//return m_DownloadCallback.GetPlayFileSecond();
		return 0;
	}
	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_GetHisLogOnlineDownProgress(JNIEnv * env, jobject obj)//������ֵ
	{
		//return m_DownloadCallback.GetDownloadFileSecond();
		return 0;
	}

	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_HisLogWriteMp4File(JNIEnv * env, jobject obj, jstring filepath, jstring filepathEx)
	{
		#if 0
        char *szfilepath;
        szfilepath = (char*)env->GetStringUTFChars(filepath, 0);
		m_DownloadCallback.WiteMp4File(szfilepath);
        env->ReleaseStringUTFChars(filepath, szfilepath);
#endif

 		return 0;
	}

// 
// 	//playback
// 	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_StartLocalPlayback(JNIEnv *env, jobject obj, jobject context_obj, jstring file)
// 	{
// 		int ret = -1;
// 
// 		do 
// 		{
// 			Log("Java_com_platform_nativecaller_NativeCaller_StartLocalPlayback 111\n");
// 			if ((NULL != g_devInterface) && (NULL != g_localpalyInterface) && (NULL != m_pEnvForPlayback))
// 			{
// 				break;
// 			}
// 			Log("Java_com_platform_nativecaller_NativeCaller_StartLocalPlayback 222\n");
// 			g_devInterface = new CMediaDevInterface();
// 
// 			if (NULL == g_devInterface)
// 			{
// 				break;
// 			}
// 			Log("Java_com_platform_nativecaller_NativeCaller_StartLocalPlayback 333\n");
// 
// 			if (m_pEnvForPlayback == NULL)
// 			{
// 				m_pEnvForPlayback = env;
// 				g_PlaybackObj = env->NewGlobalRef(context_obj);
// 			}
// 			Log("Java_com_platform_nativecaller_NativeCaller_StartLocalPlayback 444\n");
// 			g_devInterface->XDeviceSetInfo("", 0, "", "","", 0, 0, X_DEVICE_TYPE_NONE);
// 
// 			g_localpalyInterface = g_devInterface->CreateLocalPlaybackInstance();
// 			Log("Java_com_platform_nativecaller_NativeCaller_StartLocalPlayback 555\n");
// 			//g_localpalyInterface->XLocalPlaybackOpenForApp();
// 			char *szfile;
// 			szfile = (char*)env->GetStringUTFChars(file, 0);
// 			XSERVERTIME nStartTime, nEndTime;
// 			g_localpalyInterface->XLocalPlaybackOpenForApp(szfile, nStartTime, nEndTime, pCallback);
// 			env->ReleaseStringUTFChars(file, szfile);
// 			Log("Java_com_platform_nativecaller_NativeCaller_StartLocalPlayback 666\n");
// 			ret = 0;
// 		} while (0);
// 		return ret;
// 	}
// 	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_StopLocalPlayback(JNIEnv *env, jobject obj)
// 	{
// 		Log("Java_com_platform_nativecaller_NativeCaller_StopLocalPlayback 111\n");
// 		if ((NULL == g_devInterface) && (NULL == g_localpalyInterface) && (NULL == m_pEnvForPlayback))
// 		{
// 			return 0;
// 		}
// 		Log("Java_com_platform_nativecaller_NativeCaller_StopLocalPlayback 222\n");
// 		if(g_localpalyInterface)
// 		{
// 			g_localpalyInterface->XLocalPlaybackClose();
// 			g_localpalyInterface->XLocalPlaybackDelete();
// 			g_localpalyInterface = NULL;
// 		}
// 
// 		if(g_devInterface)
// 		{
// 			g_devInterface->XDeviceClose();
// 			g_devInterface->XDeviceDelete();
// 			g_devInterface = NULL;
// 		}
// 
// 		//
// 		if (m_pEnvForPlayback != NULL)
// 		{
// 			m_pEnvForPlayback->DeleteGlobalRef(g_PlaybackObj);
// 			m_pEnvForPlayback = NULL;
// 			g_PlaybackObj = NULL;
// 		}
// 
// 
// 		return 0;
// 	}
// 	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_CtrlLocalPlayback(JNIEnv *env, jobject obj, jint type, jint val)
// 	{
// 		if (NULL == g_localpalyInterface)
// 		{
// 			return -1;
// 		}
// 		g_localpalyInterface->XLocalPlaybackPlayCtrl(type, val);
// 		return 0;
// 	}

	//




// 	JNIEXPORT int JNICALL Java_com_platform_nativecaller_NativeCaller_YUV4202RGB565(JNIEnv * env, jobject obj, jbyteArray yuv, jbyteArray rgb, int width, int height)
// 	{
// // 		jbyte * Buf = (jbyte*)env->GetByteArrayElements(yuv, 0);
// // 		jbyte * Pixel = (jbyte*)env->GetByteArrayElements(rgb, 0);
// // 
// // 		ccvt_420p_rgb565(width, height, (const unsigned char *)Buf, (unsigned short *)Pixel);
// // 
// // 		env->ReleaseByteArrayElements(yuv, Buf, 0);
// // 		env->ReleaseByteArrayElements(rgb, Pixel, 0);
// 
// 
// 		return 0;
// 	}
























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

	void ccvt_420p_rgb565(int width, int height, const unsigned char *src, unsigned short *dst)
	{
		int line, col, linewidth;
		int y, u, v, yy, vr, ug, vg, ub;
		int r, g, b;
		const unsigned char *py, *pu, *pv;

		linewidth = width >> 1;
		py = src;
		pu = py + (width * height);
		pv = pu + (width * height) / 4;

		y = *py++;
		yy = y << 8;
		u = *pu - 128;
		ug = 88 * u;
		ub = 454 * u;
		v = *pv - 128;
		vg = 183 * v;
		vr = 359 * v;

		for (line = 0; line < height; line++) {
			for (col = 0; col < width; col++) {
				r = (yy + vr) >> 8;
				g = (yy - ug - vg) >> 8;
				b = (yy + ub) >> 8;

				if (r < 0)   r = 0;
				if (r > 255) r = 255;
				if (g < 0)   g = 0;
				if (g > 255) g = 255;
				if (b < 0)   b = 0;
				if (b > 255) b = 255;
				*dst++ = (((unsigned short)r >> 3) << 11) | (((unsigned short)g >> 2) << 5) | (((unsigned short)b >> 3) << 0);

				y = *py++;
				yy = y << 8;
				if (col & 1) {
					pu++;
					pv++;

					u = *pu - 128;
					ug = 88 * u;
					ub = 454 * u;
					v = *pv - 128;
					vg = 183 * v;
					vr = 359 * v;
				}
			} /* ..for col */
			if ((line & 1) == 0) { // even line: rewind
				pu -= linewidth;
				pv -= linewidth;
			}
		} /* ..for line */
	}

#ifdef __cplusplus
}
#endif
