#!/bin/bash

TOOLS_ROOT=e:/android-ndk-r15c

#
# Warning !!! Android Build !!!
#
# Default to API 21 for it is the minimum requirement for 64 bit archs.
# IF you need to build for min api level 16, you need to modify it to 14 and will not build for 64 bit archs.
# api level 16 build is better because api level 21 and higher may have problem like
#
#     https://github.com/openssl/openssl/issues/988
#     http://stackoverflow.com/questions/37122126/whats-the-exact-significance-of-android-ndk-platform-version-compared-to-api-le
#
# So if you not need 64 bit arch api level 16 is better
#
# But but but cURL could not build by android-20 and earlier :-(
# So you can build openssl with android-16 then build cURL with android-21
#


#if [ "${1}" == "cURL" ]; then
#    ANDROID_API=${ANDROID_API:-21}
#else
#    ANDROID_API=${ANDROID_API:-16}
#fi

#ANDROID_API=${ANDROID_API:-16}

#编译64位需要传进来ANDROID_API
ANDROID_API=android-15
PREFIX=c:/armlib/openssl/armeabi-v7a
ARCH=android-armeabi
TOOLCHAIN_ROOT=$TOOLS_ROOT/toolchains/arm-linux-androideabi-4.9/prebuilt/windows-x86_64 #${TOOLS_ROOT}/${OUT}-android-toolchain





#ARCHS=("android" "android-armeabi" "android-x86" "android-mips")
#ABIS=("armeabi" "armeabi-v7a" "x86" "mips")
# ANDROID_API=${ANDROID_API:-21}
#  ARCHS=("android" "android-armeabi" "android64-aarch64" "android-x86" "android64" "android-mips" "android-mips64")
#  ABIS=("armeabi" "armeabi-v7a" "arm64-v8a" "x86" "x86_64" "mips" "mips64")

NDK=${TOOLS_ROOT}


# configure() {

  
  #OUT=$2; 
  # CLANG=${3:-""};



  if [ "$ARCH" == "android" ]; then
    export ARCH_FLAGS="-mthumb"
    export ARCH_LINK=""
    export TOOL="arm-linux-androideabi"
    NDK_FLAGS="--arch=arm"
  elif [ "$ARCH" == "android-armeabi" ]; then
    export ARCH_FLAGS="-march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mthumb -mfpu=neon"
    export ARCH_LINK="-march=armv7-a -Wl,--fix-cortex-a8"
    export TOOL="arm-linux-androideabi"
    export SYSROOT=${TOOLS_ROOT}/platforms/${ANDROID_API}/arch-arm/
    NDK_FLAGS="--arch=arm"
  elif [ "$ARCH" == "android64-aarch64" ]; then
    export ARCH_FLAGS=""
    export ARCH_LINK=""
    export TOOL="aarch64-linux-android"
    NDK_FLAGS="--arch=arm64"
    export SYSROOT=${TOOLS_ROOT}/platforms/${ANDROID_API}/arch-arm64/

  # elif [ "$ARCH" == "android-x86" ]; then
  #   export ARCH_FLAGS="-march=i686 -mtune=intel -msse3 -mfpmath=sse -m32"
  #   export ARCH_LINK=""
  #   export TOOL="i686-linux-android"
  #   NDK_FLAGS="--arch=x86"
  # elif [ "$ARCH" == "android64" ]; then
  #   export ARCH_FLAGS="-march=x86-64 -msse4.2 -mpopcnt -m64 -mtune=intel"
  #   export ARCH_LINK=""
  #   export TOOL="x86_64-linux-android"
  #   NDK_FLAGS="--arch=x86_64"
  # elif [ "$ARCH" == "android-mips" ]; then
  #   export ARCH_FLAGS=""
  #   export ARCH_LINK=""
  #   export TOOL="mipsel-linux-android"
  #   NDK_FLAGS="--arch=mips"
  # elif [ "$ARCH" == "android-mips64" ]; then
  #   export ARCH="linux64-mips64"
  #   export ARCH_FLAGS=""
  #   export ARCH_LINK=""
  #   export TOOL="mips64el-linux-android"
  #   NDK_FLAGS="--arch=mips64"
  fi;

  # [ -d ${TOOLCHAIN_ROOT} ] || python $NDK/build/tools/make_standalone_toolchain.py \
  #                                    --api ${ANDROID_API} \
  #                                    --stl libc++ \
  #                                    --install-dir=${TOOLCHAIN_ROOT} \
  #                                    $NDK_FLAGS

  export TOOLCHAIN_PATH=${TOOLCHAIN_ROOT}/bin
  export NDK_TOOLCHAIN_BASENAME=${TOOLCHAIN_PATH}/${TOOL}
  export CROSS_SYSROOT=$SYSROOT
  # if [ -z "${CLANG}" ]; then
    export CC=${NDK_TOOLCHAIN_BASENAME}-gcc
    export CXX=${NDK_TOOLCHAIN_BASENAME}-g++
  # else
  #   export CC=${NDK_TOOLCHAIN_BASENAME}-clang
  #   export CXX=${NDK_TOOLCHAIN_BASENAME}-clang++
  # fi;
  export LINK=${CXX}
  export LD=${NDK_TOOLCHAIN_BASENAME}-ld
  export AR=${NDK_TOOLCHAIN_BASENAME}-ar
  export RANLIB=${NDK_TOOLCHAIN_BASENAME}-ranlib
  export STRIP=${NDK_TOOLCHAIN_BASENAME}-strip
  export CPPFLAGS=${CPPFLAGS:-""}
  export LIBS=${LIBS:-""}
  export CFLAGS="${ARCH_FLAGS} -fpic -ffunction-sections -funwind-tables -fstack-protector -fno-strict-aliasing -finline-limit=64"
  export CXXFLAGS="${CFLAGS} -std=c++11 -frtti -fexceptions"
  export LDFLAGS="${ARCH_LINK}"
  echo "**********************************************"
  echo "use ANDROID_API=${ANDROID_API}"
  echo "use NDK=${NDK}"
  echo "export ARCH=${ARCH}"
  echo "export NDK_TOOLCHAIN_BASENAME=${NDK_TOOLCHAIN_BASENAME}"
  echo "export SYSROOT=${SYSROOT}"
  echo "export CC=${CC}"
  echo "export CXX=${CXX}"
  echo "export LINK=${LINK}"
  echo "export LD=${LD}"
  echo "export AR=${AR}"
  echo "export RANLIB=${RANLIB}"
  echo "export STRIP=${STRIP}"
  echo "export CPPFLAGS=${CPPFLAGS}"
  echo "export CFLAGS=${CFLAGS}"
  echo "export CXXFLAGS=${CXXFLAGS}"
  echo "export LDFLAGS=${LDFLAGS}"
  echo "export LIBS=${LIBS}"
  echo "**********************************************"
# }




# #CPU架构
# ARCH=arm
# NDK_PATH=e:/android-ndk-r15c
# PREFIX=c:/armlib/openssl/armeabi-v7a

# #编译工具和Platform设置
# if [ "$ARCH" = "arm" ]; then
#     HOST=arm-linux-androideabi
#     EXFLAGS="-march=armv7-a -mfpu=neon -mfloat-abi=softfp -ffast-math"
#     ARCHTARG=arm
#     CROSS=arm-linux-androideabi
#     HOSTNAME=arm
#     COMPILER=android-armeabi     
#     SYSROOT=$NDK_PATH/platforms/android-15/arch-arm  #platform路径，设置成你的路径
#     TOOLCHAIN=$NDK_PATH/toolchains/${HOST}-4.9/prebuilt/windows-x86_64/bin/${CROSS} #编译工具路径 设置成你的路径
# elif [ "$ARCH" = "arm64-v8a" ]; then
#     HOST=aarch64-linux-android
#     ARCHTARG=arm64
#     CROSS=aarch64-linux-android
#     HOSTNAME=aarch64
#     COMPILER=android64-aarch64    
#     SYSROOT=$NDK_PATH/platforms/android-21/arch-arm64
#     TOOLCHAIN=$NDK_PATH/toolchains/${HOST}-4.9/prebuilt/windows-x86_64/bin/${CROSS} #编译工具路径 设置成你的路径
# fi

# #TOOLCHAIN=$NDK_PATH/toolchains/${HOST}-4.9/prebuilt/darwin-x86_64/bin/${CROSS} #编译工具路径 设置成你的路径

# #android_NDK
# export NDK_PATH
# #android_Platform
# export SYSROOT
# #android_tools
# export CC=${TOOLCHAIN}-gcc #c工具
# export CXX=${TOOLCHAIN}-g++ #c++工具
# export LD=${TOOLCHAIN}-ld #ld工具
# export STRIP=${TOOLCHAIN}-strip #strip工具
# export AR=${TOOLCHAIN}-ar #ar工具
# export NM=${TOOLCHAIN}-nm #nm工具
# export RANLIB=${TOOLCHAIN}-ranlib  # ranlib工具
# export CFLAGS="${CFLAGS} ${EXFLAGS}"

# # export MACHINE=armv7
# # export RELEASE=2.6.37
# export SYSTEM=android
# export ARCH=${ARCHTARG}
# #export CROSS_COMPILE= ${HOST}-
# export ANDROID_SYSROOT=${SYSROOT}
# # export SYSROOT="$ANDROID_SYSROOT"
# # export NDK_SYSROOT="$ANDROID_SYSROOT"
# # export ANDROID_NDK_SYSROOT="$ANDROID_SYSROOT"
# # export ANDROID_API="$_ANDROID_API"

# # # CROSS_COMPILE and ANDROID_DEV are DFW (Don't Fiddle With). Its used by OpenSSL build system.
# # # export CROSS_COMPILE="arm-linux-androideabi-"
# # export ANDROID_DEV="$ANDROID_NDK_ROOT/platforms/$_ANDROID_API/$_ANDROID_ARCH/usr"
# # export HOSTCC=gcc

#裁剪 根据具体需求进行裁剪  prefix修改成你的安装路径
# ./Configure $COMPILER no-shared \
# no-ssl2 \
# no-comp no-asm no-idea \
# no-hw no-engine no-dso \
# --prefix=/Users/feng/third-lib/openssl/android/${COMPILER} \
# --sysroot=${SYSROOT}



./Configure ${ARCH} \
no-shared \
no-asm \
--prefix=${PREFIX}

# #clean
# make clean
# #make
# make -j4
# #install安装
# make install

make clean

if make -j4; then
    OUTPUT_ROOT=${PREFIX}
    LOCALDIR=`pwd`

    [ -d ${OUTPUT_ROOT}/include ] || mkdir -p ${OUTPUT_ROOT}/include
    cp -r ${LOCALDIR}/include/openssl ${OUTPUT_ROOT}/include

    [ -d ${OUTPUT_ROOT}/lib ] || mkdir -p ${OUTPUT_ROOT}/lib
    cp ${LOCALDIR}/libcrypto.a ${OUTPUT_ROOT}/lib
    cp ${LOCALDIR}/libssl.a ${OUTPUT_ROOT}/lib
  fi;

