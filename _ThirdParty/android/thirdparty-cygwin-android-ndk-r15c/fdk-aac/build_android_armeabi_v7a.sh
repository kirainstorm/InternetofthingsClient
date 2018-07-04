#!/bin/bash
#Change NDK to your Android NDK location
NDK=e:/android-ndk-r15c
PLATFORM=$NDK/platforms/android-15/arch-arm/
PREBUILT=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/windows-x86_64
PREFIX=c:/armlib/fdkaac_temp/armeabi-v7a
export TMPDIR="c:/armlib"

ARM_INC=$PLATFORM/usr/include
ARM_LIB=$PLATFORM/usr/lib

LDFLAGS=" -nostdlib -Bdynamic -Wl,--whole-archive -Wl,--no-undefined -Wl,-z,noexecstack  -Wl,-z,nocopyreloc -Wl,-soname,/system/lib/libz.so -Wl,-rpath-link=$ARM_LIB,-dynamic-linker=/system/bin/linker \
--cc=$PREBUILT/bin/arm-linux-androideabi-gcc \
--cross-prefix=$PREBUILT/bin/arm-linux-androideabi- -L$ARM_LIB  -lc -lgcc -lm -ldl  "

FLAGS="--host=arm-linux-androideabi --enable-static --disable-shared"

# export CXX="${CROSS_COMPILE}g++ --sysroot=${PLATFORM}"
# export LDFLAGS="$LDFLAGS"
# export CC="${CROSS_COMPILE}gcc --sysroot=${PLATFORM}"
CPU=arm

./configure $FLAGS --prefix=$PREFIX

  make clean
  make -j6
  make install



echo Android ARMv7-a builds finished



