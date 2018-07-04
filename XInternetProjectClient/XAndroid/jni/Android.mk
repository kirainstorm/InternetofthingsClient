LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

#  cd C:/boost_1_65_1111/build/_2/XInternetProjectClient/XAndroid/jni
PATH_INC_PATH := C:/boost_1_65_1111/build/_2/_ThirdParty/android/_inc
PATH_LIB_PATH := C:/boost_1_65_1111/build/_2/_ThirdParty/android/_lib
#--------------------------------------------------------------------------------------------
 #fdkaac
include $(CLEAR_VARS)
LOCAL_MODULE := fdkaac
LOCAL_SRC_FILES := $(PATH_LIB_PATH)/$(TARGET_ARCH_ABI)/libfdk-aac.a
include $(PREBUILT_STATIC_LIBRARY)
#openssl
include $(CLEAR_VARS)
LOCAL_MODULE := crypto
LOCAL_SRC_FILES := $(PATH_LIB_PATH)/$(TARGET_ARCH_ABI)/libcrypto.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE := ssl
LOCAL_SRC_FILES := $(PATH_LIB_PATH)/$(TARGET_ARCH_ABI)/libssl.a
include $(PREBUILT_STATIC_LIBRARY)
#x264
include $(CLEAR_VARS)
LOCAL_MODULE := x264
LOCAL_SRC_FILES := $(PATH_LIB_PATH)/$(TARGET_ARCH_ABI)/libx264.a
include $(PREBUILT_STATIC_LIBRARY)
#ffmpeg
include $(CLEAR_VARS)
LOCAL_MODULE := avcodec
LOCAL_SRC_FILES := $(PATH_LIB_PATH)/$(TARGET_ARCH_ABI)/libavcodec.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE := avformat
LOCAL_SRC_FILES := $(PATH_LIB_PATH)/$(TARGET_ARCH_ABI)/libavformat.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE := swscale
LOCAL_SRC_FILES := $(PATH_LIB_PATH)/$(TARGET_ARCH_ABI)/libswscale.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE := avutil
LOCAL_SRC_FILES := $(PATH_LIB_PATH)/$(TARGET_ARCH_ABI)/libavutil.a
include $(PREBUILT_STATIC_LIBRARY)
LOCAL_MODULE := avfilter
LOCAL_SRC_FILES := $(PATH_LIB_PATH)/$(TARGET_ARCH_ABI)/libavfilter.a
include $(PREBUILT_STATIC_LIBRARY)
LOCAL_MODULE := swresample
LOCAL_SRC_FILES := $(PATH_LIB_PATH)/$(TARGET_ARCH_ABI)/libswresample.a
include $(PREBUILT_STATIC_LIBRARY)
#boost
include $(CLEAR_VARS)
LOCAL_MODULE:= boost_system
LOCAL_SRC_FILES:= $(PATH_LIB_PATH)/$(TARGET_ARCH_ABI)/libboost_system.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE:= boost_thread
LOCAL_SRC_FILES:= $(PATH_LIB_PATH)/$(TARGET_ARCH_ABI)/libboost_thread.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE:= boost_timer
LOCAL_SRC_FILES:= $(PATH_LIB_PATH)/$(TARGET_ARCH_ABI)/libboost_timer.a
include $(PREBUILT_STATIC_LIBRARY)
#live555
include $(CLEAR_VARS)
LOCAL_MODULE := live555
LOCAL_SRC_FILES := $(PATH_LIB_PATH)/$(TARGET_ARCH_ABI)/liblive555.a
include $(PREBUILT_STATIC_LIBRARY)
#libjpeg-turbo
include $(CLEAR_VARS)
LOCAL_MODULE:= libjpeg-turbo
LOCAL_SRC_FILES:= $(PATH_LIB_PATH)/$(TARGET_ARCH_ABI)/libjpeg-turbo.a
include $(PREBUILT_STATIC_LIBRARY)
#jsoncpp
include $(CLEAR_VARS)
LOCAL_MODULE:= jsoncpp
LOCAL_SRC_FILES:= $(PATH_LIB_PATH)/$(TARGET_ARCH_ABI)/libjsoncpp.a
include $(PREBUILT_STATIC_LIBRARY)
#--------------------------------------------------------------------------------------------
# include $(CLEAR_VARS)
# PATH_TO_MY_SOURCE :=	$(LOCAL_PATH)/_inc
# PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../_Include
# PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../_Include/cpp_cross_download
# PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../_Include/cpp_nettoserver
# #inc
# PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_Include
# PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_Include/cpp_cross_display
# #libjpeg-turbo
# PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_ThirdParty/libjpeg-turbo/android/inc
# #ffmpeg
# PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_ThirdParty/x264/android/inc
# PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_ThirdParty/fdk_aac_amr/inc
# PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_ThirdParty/vo_aacenc/android/include
# PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_ThirdParty/ffmpeg/android_me/v287_x264_voaacenc/include
# #boost
# PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_ThirdParty/boost/android_1_55_0
# PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_ThirdParty/SmartPointers
# #openssl
# PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_ThirdParty/openssl/android/include
# #device
# PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_Common/Mp4File
# PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../_MediaInterface
# PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_Common/XPlayer
# PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_ThirdParty/fdk_aac_amr/inc

include $(CLEAR_VARS)
PATH_TO_MY_SOURCE :=	$(LOCAL_PATH)
PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../_Include
PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_Include
PATH_TO_MY_SOURCE +=	$(LOCAL_PATH)/../../../_Protocol
#libjpeg-turbo
PATH_TO_MY_SOURCE +=	$(PATH_INC_PATH)/jpeg-turbo
#ffmpeg
PATH_TO_MY_SOURCE +=	$(PATH_INC_PATH)/x264
PATH_TO_MY_SOURCE +=	$(PATH_INC_PATH)/fdkaac
PATH_TO_MY_SOURCE +=	$(PATH_INC_PATH)/ffmpeg
#boost
PATH_TO_MY_SOURCE +=	$(PATH_INC_PATH)/boost
#live555
PATH_TO_MY_SOURCE +=	$(PATH_INC_PATH)/live555
#openssl
PATH_TO_MY_SOURCE +=	$(PATH_INC_PATH)/openssl
#jsoncpp
PATH_TO_MY_SOURCE +=	$(PATH_INC_PATH)/jsoncpp
#
LOCAL_C_INCLUDES  +=$(PATH_TO_MY_SOURCE)
#--------------------------------------------------------------------------------------------
#
MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../_Include/*.cpp)
#
LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)
#--------------------------------------------------------------------------------------------




#--------------------------------------------------------------------------------------------
# static libraries.
LOCAL_STATIC_LIBRARIES := 	boost_system \
							boost_thread \
							boost_timer \
							avformat \
							avcodec \
							avutil \
							swscale \
							swresample \
							avfilter \
							fdkaac \
							x264 \
							live555 \
							libjpeg-turbo \
							ssl \
							crypto \
							jsoncpp
          
# shared libraries
# LOCAL_SHARED_LIBRARIES := \
# 		libandroid_runtime \
# 		libnativehelper \
# 		libcutils \
# 		libutils \
# 		lib_crypto \
# 		lib_ssl

#--------------------------------------------------------------------------------------------
#we need opengl
#LOCAL_LDLIBS    := -lGLESv1_CM -ldl -llog
#LOCAL_LDLIBS    := -llog

#LOCAL_LDLIBS    := -llog -lGLESv2 -lz

#
LOCAL_CPPFLAGS := -fexceptions
LOCAL_LDLIBS := -llog -lGLESv2 -lz
LOCAL_LDFLAGS += -fPIC
LOCAL_CPPFLAGS := -fexceptions
#shared library text segment is not shareable
LOCAL_DISABLE_FATAL_LINKER_WARNINGS := true




#
#$(call import-module, boost_1_55_0)


#APP_PLATFORM := android-8
#--------------------------------------------------------------------------------------------
# Here we give our module name and sourcefile(s)
LOCAL_MODULE  := xmedia

LOCAL_CFLAGS += -std=c++11
#--------------------------------------------------------------------------------------------
#this is a static library (.a)
#include $(BUILD_STATIC_LIBRARY)

#this is a shared library (.so)
include $(BUILD_SHARED_LIBRARY)

