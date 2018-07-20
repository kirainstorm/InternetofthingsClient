

You should link your app with


lib
b
libbz

libiconv

coremedia.framework

audiotoolbox.framework

quatzcore.framework

opengles.framewor
k
glkit.framework

coredata.framework

systemconfiguration.framework

VideoToolbox.framework




将ffmpeg更新到3.1.1






Mac安装Brew
安装命令如下：

ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)”

先下载文件gas-preprocessor-master.zip：https://github.com/libav/gas-preprocessor


复制gas-preprocessor.pl到/usr/local/bin下，网上有说法复制到/usr/sbin下，
但是OS X 10.11以后的版本不能向/usr/sbin修改，所以复制到/usr/local/bin下

cp -f gas-preprocessor.pl /usr/local/bin/


修改文件权限：chmod 777 /usr/local/bin/gas-preprocessor.pl




安装yasm
brew install yasm


注意，要在/usr/local/bin 看是否有yasm如果没有使用 ln -s yasm-**
牋?



下载脚本FFmpeg-iOS-build-script-master.zip：https://github.com/kewlbear/FFmpeg-iOS-build-script


解压，将ffmpeg-3.1.1放到该目录，找到文件 build-ffmpeg.sh （可能要修改SOURCE="ffmpeg-3.1.1”）


./build-ffmpeg.sh


编译完成后，然后输入./build-ffmpeg.sh lipo，


这个命令是讲.a文件合并成一个

把ffmpeg-iOS文件加入到工程中



You should link your app with

libz.tbd
libbz2.tbd
libiconv.tbd





