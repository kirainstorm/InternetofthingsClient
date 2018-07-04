最新的ndk编译后会生成 arm64-v8a armeabi-v7a x86 x86_64 


json_tool.h 添加
#define NO_LOCALE_SUPPORT


ndk-build.cmd NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk NDK_APPLICATION_MK=./Application.mk