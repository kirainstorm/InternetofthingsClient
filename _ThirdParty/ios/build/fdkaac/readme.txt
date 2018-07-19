https://github.com/kewlbear/fdk-aac-build-script-for-iOS
















修改了
ARCHS="arm64 armv7s x86_64 i386 armv7"

if [ $ARCH = "arm64" ]

添加了
CFLAGS="$CFLAGS -fembed-bitcode -mios-version-min=7.0"