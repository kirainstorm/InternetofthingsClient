#!/bin/bash
#Change NDK to your Android NDK location
NDK=e:/android-ndk-r15c
PLATFORM=$NDK/platforms/android-21/arch-arm64/
PREBUILT=$NDK/toolchains/aarch64-linux-android-4.9/prebuilt/windows-x86_64
PREFIX=c:/armlib/ffmpeg/arm64-v8a
export TMPDIR="c:/armlib"

GENERAL="\
--enable-small \
--enable-cross-compile \
--extra-libs="-lgcc" \
--arch=aarch64 \
--cc=$PREBUILT/bin/aarch64-linux-android-gcc \
--cross-prefix=$PREBUILT/bin/aarch64-linux-android- \
--nm=$PREBUILT/bin/aarch64-linux-android-nm"

MODULES="\
--enable-gpl \
--enable-nonfree \
--enable-encoder=libfdk_aac \
--enable-decoder=libfdk_aac \
--enable-libfdk-aac \
--enable-libx264"



function build_arm64
{
  ./configure \
  --logfile=conflog.txt \
  --target-os=linux \
  --prefix=$PREFIX \
  ${GENERAL} \
  --sysroot=$PLATFORM \
  --extra-cflags="$ADDI_CFLAGS" \
  --extra-ldflags="$ADDI_LDFLAGS -lx264 -Wl,-rpath-link=$PLATFORM/usr/lib -L$PLATFORM/usr/lib -nostdlib -lc -lm -ldl -llog" \
  --disable-shared \
  --enable-static \
  --disable-doc \
  --disable-programs \
  --disable-debug \
  --enable-zlib \
  ${MODULES}

  make clean
  make -j6
  make install
}

ADDI_CFLAGS="-Ic:/armlib/x264/arm64-v8a/include -Ic:/armlib/fdkaac/arm64-v8a/include "
ADDI_LDFLAGS="-Lc:/armlib/x264/arm64-v8a/lib  -lx264 -Lc:/armlib/fdkaac/arm64-v8a/lib  -lfdk-aac"

build_arm64


echo Android ARM64 builds finished
