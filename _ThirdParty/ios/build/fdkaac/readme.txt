https://github.com/kewlbear/fdk-aac-build-script-for-iOS
















�޸���
ARCHS="arm64 armv7s x86_64 i386 armv7"

if [ $ARCH = "arm64" ]

�����
CFLAGS="$CFLAGS -fembed-bitcode -mios-version-min=7.0"