

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




��ffmpeg���µ�3.1.1






Mac��װBrew
��װ�������£�

ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)��

�������ļ�gas-preprocessor-master.zip��https://github.com/libav/gas-preprocessor


����gas-preprocessor.pl��/usr/local/bin�£�������˵�����Ƶ�/usr/sbin�£�
����OS X 10.11�Ժ�İ汾������/usr/sbin�޸ģ����Ը��Ƶ�/usr/local/bin��

cp -f gas-preprocessor.pl /usr/local/bin/


�޸��ļ�Ȩ�ޣ�chmod 777 /usr/local/bin/gas-preprocessor.pl




��װyasm
brew install yasm


ע�⣬Ҫ��/usr/local/bin ���Ƿ���yasm���û��ʹ�� ln -s yasm-**
��?



���ؽű�FFmpeg-iOS-build-script-master.zip��https://github.com/kewlbear/FFmpeg-iOS-build-script


��ѹ����ffmpeg-3.1.1�ŵ���Ŀ¼���ҵ��ļ� build-ffmpeg.sh ������Ҫ�޸�SOURCE="ffmpeg-3.1.1����


./build-ffmpeg.sh


������ɺ�Ȼ������./build-ffmpeg.sh lipo��


��������ǽ�.a�ļ��ϲ���һ��

��ffmpeg-iOS�ļ����뵽������



You should link your app with

libz.tbd
libbz2.tbd
libiconv.tbd





