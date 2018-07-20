下载最新x264源码 ftp://ftp.videolan.org/pub/x264/snapshots/last_x264.tar.bz2
下载gas-preprocessor https://github.com/libav/gas-preprocessor 并copy到 /usr/local/bin/gas-preprocessor.pl


x264 iOS build script
https://github.com/kewlbear/x264-ios


To build everything:
./build-x264.sh
To build for arm64:
./build-x264.sh arm64
To build fat library for armv7 and x86_64 (64-bit simulator):
./build-x264.sh armv7 x86_64
To build fat library from separately built thin libraries:
./build-x264.sh lipo