#!/bin/bash
#Change NDK to your Android NDK location
NDK=e:/android-ndk-r15c
PLATFORM=$NDK/platforms/android-15/arch-arm/
PREBUILT=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/windows-x86_64
PREFIX=c:/armlib/ffmpeg/armeabi-v7a
export TMPDIR="c:/armlib"

GENERAL="\
--enable-small \
--enable-cross-compile \
--extra-libs="-lgcc" \
--arch=arm \
--cc=$PREBUILT/bin/arm-linux-androideabi-gcc \
--cross-prefix=$PREBUILT/bin/arm-linux-androideabi- \
--nm=$PREBUILT/bin/arm-linux-androideabi-nm "



MODULES="\
--enable-gpl \
--enable-nonfree \
--enable-encoder=libfdk_aac \
--enable-decoder=libfdk_aac \
--enable-libfdk-aac \
--enable-libx264"

function build_ARMv7
{
  ./configure \
  --target-os=linux \
  --prefix=$PREFIX \
  ${GENERAL} \
  --sysroot=$PLATFORM \
  --disable-shared \
  --enable-static \
  --extra-cflags="$ADDI_CFLAGS -DANDROID -fPIC -ffunction-sections -funwind-tables -fstack-protector -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -fomit-frame-pointer -fstrict-aliasing -funswitch-loops -finline-limit=300" \
  --extra-ldflags="$ADDI_LDFLAGS -Wl,-rpath-link=$PLATFORM/usr/lib -L$PLATFORM/usr/lib -nostdlib -lc -lm -ldl -llog" \
  --enable-zlib \
  ${MODULES} \
  --disable-doc \
  --disable-programs \
  --disable-debug \
  --enable-neon

  make clean
  make -j6
  make install
}

ADDI_CFLAGS="-Ic:/armlib/x264/armeabi-v7a/include -Ic:/armlib/fdkaac/armeabi-v7a/include "
ADDI_LDFLAGS="-Lc:/armlib/x264/armeabi-v7a/lib  -lx264 -Lc:/armlib/fdkaac/armeabi-v7a/lib  -lfdk-aac"

build_ARMv7


echo Android ARMv7-a builds finished
