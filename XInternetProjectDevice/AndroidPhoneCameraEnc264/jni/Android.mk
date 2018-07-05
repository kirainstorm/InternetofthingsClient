LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)



###################################################################




#ffmpeg
include $(CLEAR_VARS)
LOCAL_MODULE := avcodec
LOCAL_SRC_FILES := ../../../_AndroidLib/libavcodec.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE := avformat
LOCAL_SRC_FILES := ../../../_AndroidLib//libavformat.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE := swscale
LOCAL_SRC_FILES := ../../../_AndroidLib//libswscale.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE := avutil
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_SRC_FILES := ../../../_AndroidLib//libavutil.a
include $(PREBUILT_STATIC_LIBRARY)
LOCAL_MODULE := avfilter
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_SRC_FILES := ../../../_AndroidLib//libavfilter.a
include $(PREBUILT_STATIC_LIBRARY)
LOCAL_MODULE := swresample
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_SRC_FILES := ../../../_AndroidLib//libswresample.a
include $(PREBUILT_STATIC_LIBRARY)
#boost
include $(CLEAR_VARS)
LOCAL_MODULE:= boost_system
LOCAL_SRC_FILES:= ../../../_AndroidLib//libboost_system.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE:= boost_thread
LOCAL_SRC_FILES:= ../../../_AndroidLib//libboost_thread.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
include $(PREBUILT_STATIC_LIBRARY)

 
include $(CLEAR_VARS)
LOCAL_MODULE:= boost_timer
LOCAL_SRC_FILES:= ../../../_AndroidLib//libboost_timer.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
include $(PREBUILT_STATIC_LIBRARY)

#openssl
include $(CLEAR_VARS)
LOCAL_MODULE := lib_crypto
LOCAL_SRC_FILES := ../../../_AndroidLib//libcrypto.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := lib_ssl
LOCAL_SRC_FILES := ../../../_AndroidLib//libssl.so
include $(PREBUILT_SHARED_LIBRARY)



#xnet
include $(CLEAR_VARS)
LOCAL_MODULE:= xnet
LOCAL_SRC_FILES:= ../../../_AndroidLib//libxnet.a
#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
include $(PREBUILT_STATIC_LIBRARY)

#fdkaac
include $(CLEAR_VARS)
LOCAL_MODULE:= fdkaac
LOCAL_SRC_FILES:= ../../../_AndroidLib//libfdk-aac.a
#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
include $(PREBUILT_STATIC_LIBRARY)

#x264
include $(CLEAR_VARS)
LOCAL_MODULE := x264
LOCAL_SRC_FILES := ../../../_AndroidLib/libx264.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
###################################################################
include $(CLEAR_VARS)
PATH_TO_MY_SOURCE :=	$(LOCAL_PATH)
#inc
PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_Include
PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_Include/Cross-platform
PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_Include/cpp_cross_platform_record
#PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_Include/cpp_cross_platform_display
#PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_Include/cpp_opengl
#PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_Include/cpp_dec
PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_Include/cpp_lib
PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_Include/cpp_search
PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_Include/cpp_nettoserver
#x264
PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_ThirdParty/x264/android/inc
#xplayer
PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_ThirdParty/fdk_aac_amr/inc
PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_Common/XPlayer
PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_Common/Mp4File

#libjpeg-turbo
PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_ThirdParty/libjpeg-turbo/android/inc
#ffmpeg
PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_ThirdParty/ffmpeg/android_me/include
#boost
PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_ThirdParty/boost/android_1_55_0
PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_ThirdParty/SmartPointers
#openssl
PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_ThirdParty/openssl/android/OpenSSL1.0.1cForAndroid-master/include


LOCAL_C_INCLUDES  +=$(PATH_TO_MY_SOURCE)






############################################################
#初步解法:一句话引入单个目录(不包括子目录)下的所有cpp源文件
MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
#xplayer
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../../_Common/XPlayer/Fdkaac.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../../_Common/Mp4File/Mp4Writer.cpp)


LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)








##############################################
# Here we give our module name and sourcefile(s)
LOCAL_MODULE  := enc264
 




###################################################
# static libraries.
LOCAL_STATIC_LIBRARIES := 	xnet\
							fdkaac \
							x264 \
							boost_system \
							boost_thread \
							boost_timer \
							avformat \
							avcodec \
							avutil \
							swscale \
							swresample \
							avfilter 
							#\
							xplayer \
							lib_jpeg \
							lib_simd \
							boost_system \
							boost_thread \
							boost_timer \




# shared libraries
LOCAL_SHARED_LIBRARIES := \
		libandroid_runtime \
		libnativehelper \
		libcutils \
		libutils \
		lib_crypto \
		lib_ssl
		#lib_libavcodec \
		lib_libavformat \
		lib_libswscale \
		lib_libavutil \

###############################################
#we need opengl
#LOCAL_LDLIBS    := -lGLESv1_CM -ldl -llog
#LOCAL_LDLIBS    := -llog

LOCAL_LDLIBS    := -llog -lGLESv2 -lz

#添加异常支持
LOCAL_CPPFLAGS := -fexceptions

#添加模块引用
#$(call import-module, boost_1_55_0)


#APP_PLATFORM := android-8


##################################################
#this is a static library (.a)
#include $(BUILD_STATIC_LIBRARY)

#this is a shared library (.so)
include $(BUILD_SHARED_LIBRARY)


