坑：文件夹命名不要带空格！

1.给项目gradle.properties加上以下代码，表示要用NDK了：
android.useDeprecatedNdk=true

 

2.在local.properties中加入ndk和sdk的路径：
ndk.dir=D\:\\work\\Android\\sdk\\ndk-bundle
sdk.dir=D\:\\work\\Android\\sdk

 

3.在app文件夹下的build.gradle中的defaultConfig里加入（.armeabi、armeabi-v7a、mips、x86）： 
 ndk{
    moduleName "NDKTest"
    abiFilters "armeabi-v7a","x86"
    }
 其中moduleName指要生成的.so文件名称，这个要记住，后面还会用到。abiFilters指要生成哪几个平台的so文件。



4.新建一个Java类，要通过这个类来调用C/C++的代码：
 public class GetString {
    static {
        System.loadLibrary("NDKTest");//导入生成的链接库文件
    }
    public static native String getStr();
}

 

其中用native修饰的方法，就是要在C/C++中实现的方法，System.loadLibrary(“NDKTest”)是表示要调用”NDKTest”这个so文件，就是3中的moduleName。

5.在\app\src\main\中新建jni目录（右键->new->Folder->JNI Folder）。然后编译（Build->Make Project）生成.class文件，这里主要是要得到GetString.class这个类。
1， 从Android Studio的Terminal里进入到， <Project>/src/main/java 目录下，一定要刀这个文件夹下来执行命令操作。
 2， 执行javah XXX.XX.XXX 即可，   XXX.XX.XXX是要生成.h文件的完整路径名，包名+文件名
 3， 然后就会在当前目录下生成相应的.h文件。
4:拷贝到jni



6.执行完以上命令后，会在\app\src\main\jni\下生成”com_laocaixw_ndktest_GetString.h”文件，里面应该有一个类似这样的函数：

 JNIEXPORT jstring JNICALL Java_com_laocaixw_ndk_GetString_getStr
  (JNIEnv *, jclass);

 

7.在\app\src\main\jni\里新建一个C/C++文件.cpp，将6中的头文件引入，并把上述函数复制进来，做以下修改：


 #include "com_laocaixw_ndktest_GetString.h"

JNIEXPORT jstring JNICALL Java_com_laocaixw_ndk_GetString_getStr
  (JNIEnv *env, jclass) {
  return env->NewStringUTF("Hello NDK");
  }

 

这里表示，当java中调用GetString.getStr()时，会调用C/C++中的上述这个函数，然后返回”Hello NDK”。

8.最后Make Project一下，看是不是在\app\build\intermediates\ndk\debug\下生成了so文件，如果是，那么应该大功告成，可以运行了。

9.最后，编译运行前，别忘了把打开了\app\build\intermediates\classes\debug\的Terminal命令行工具关掉，否则占用这个文件夹会导致编译不成功。因为编译的时候，会把build里的东西全部删除再重新生成，如果占用的话，删除不了就会出错。























