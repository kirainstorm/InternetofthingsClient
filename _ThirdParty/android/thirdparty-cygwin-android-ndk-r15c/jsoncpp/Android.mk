LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include

FILE_LIST += $(wildcard $(LOCAL_PATH)/src/lib_json/*.cpp)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_LDLIBS += -llog -ldl
LOCAL_CPPFLAGS += -fexceptions


LOCAL_MODULE := jsoncpp
#LOCAL_MODULE_FILENAME:= libjsnlib


LOCAL_CFLAGS += -std=c++11
#include $(BUILD_SHARED_LIBRARY) 
include $(BUILD_STATIC_LIBRARY)