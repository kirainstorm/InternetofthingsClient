LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := live555

SOURCE_LIST := $(wildcard $(LOCAL_PATH)/BasicUsageEnvironment/*.cpp)
SOURCE_LIST += $(wildcard $(LOCAL_PATH)/groupsock/*.cpp)
SOURCE_LIST += $(wildcard $(LOCAL_PATH)/groupsock/*.c)
SOURCE_LIST += $(wildcard $(LOCAL_PATH)/liveMedia/*.cpp)
SOURCE_LIST += $(wildcard $(LOCAL_PATH)/liveMedia/*.c)
SOURCE_LIST += $(wildcard $(LOCAL_PATH)/UsageEnvironment/*.cpp)

LOCAL_SRC_FILES := $(SOURCE_LIST)

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/BasicUsageEnvironment/include \
	$(LOCAL_PATH)/BasicUsageEnvironment \
	$(LOCAL_PATH)/UsageEnvironment/include \
	$(LOCAL_PATH)/UsageEnvironment \
	$(LOCAL_PATH)/groupsock/include \
	$(LOCAL_PATH)/groupsock \
	$(LOCAL_PATH)/liveMedia/include \
	$(LOCAL_PATH)/liveMedia

#此处参数请参考config.cygwin等
LOCAL_CPPFLAGS := -fexceptions -DNEWLOCALE_NOT_USED=1 -DNULL=0 -DNO_SSTREAM=1 -UIP_ADD_SOURCE_MEMBERSHIP -DSOCKLEN_T=socklen_t -DBSD=1

#include $(BUILD_SHARED_LIBRARY) 
include $(BUILD_STATIC_LIBRARY)