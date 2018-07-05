https://github.com/openstf/android-libjpeg-turbo



用1.4.0替换的






























1:解压文件


2:Cygwin进入libjpeg-turbo-1.4.0目录

 ./configure --with-jpeg8


3:libjpeg-turbo-1.4.0改为jni


4:将android.mk拷贝进去


5：$NDK/ndk-build



jmorecfg.h 
#ifndef XMD_H                   /* X11/xmd.h correctly defines INT32 */

//typedef long INT32;

#endif