�ӣ��ļ���������Ҫ���ո�

1.����Ŀgradle.properties�������´��룬��ʾҪ��NDK�ˣ�
android.useDeprecatedNdk=true

 

2.��local.properties�м���ndk��sdk��·����
ndk.dir=D\:\\work\\Android\\sdk\\ndk-bundle
sdk.dir=D\:\\work\\Android\\sdk

 

3.��app�ļ����µ�build.gradle�е�defaultConfig����루.armeabi��armeabi-v7a��mips��x86���� 
 ndk{
    moduleName "NDKTest"
    abiFilters "armeabi-v7a","x86"
    }
 ����moduleNameָҪ���ɵ�.so�ļ����ƣ����Ҫ��ס�����滹���õ���abiFiltersָҪ�����ļ���ƽ̨��so�ļ���



4.�½�һ��Java�࣬Ҫͨ�������������C/C++�Ĵ��룺
 public class GetString {
    static {
        System.loadLibrary("NDKTest");//�������ɵ����ӿ��ļ�
    }
    public static native String getStr();
}

 

������native���εķ���������Ҫ��C/C++��ʵ�ֵķ�����System.loadLibrary(��NDKTest��)�Ǳ�ʾҪ���á�NDKTest�����so�ļ�������3�е�moduleName��

5.��\app\src\main\���½�jniĿ¼���Ҽ�->new->Folder->JNI Folder����Ȼ����루Build->Make Project������.class�ļ���������Ҫ��Ҫ�õ�GetString.class����ࡣ
1�� ��Android Studio��Terminal����뵽�� <Project>/src/main/java Ŀ¼�£�һ��Ҫ������ļ�������ִ�����������
 2�� ִ��javah XXX.XX.XXX ���ɣ�   XXX.XX.XXX��Ҫ����.h�ļ�������·����������+�ļ���
 3�� Ȼ��ͻ��ڵ�ǰĿ¼��������Ӧ��.h�ļ���
4:������jni



6.ִ������������󣬻���\app\src\main\jni\�����ɡ�com_laocaixw_ndktest_GetString.h���ļ�������Ӧ����һ�����������ĺ�����

 JNIEXPORT jstring JNICALL Java_com_laocaixw_ndk_GetString_getStr
  (JNIEnv *, jclass);

 

7.��\app\src\main\jni\���½�һ��C/C++�ļ�.cpp����6�е�ͷ�ļ����룬���������������ƽ������������޸ģ�


 #include "com_laocaixw_ndktest_GetString.h"

JNIEXPORT jstring JNICALL Java_com_laocaixw_ndk_GetString_getStr
  (JNIEnv *env, jclass) {
  return env->NewStringUTF("Hello NDK");
  }

 

�����ʾ����java�е���GetString.getStr()ʱ�������C/C++�е��������������Ȼ�󷵻ء�Hello NDK����

8.���Make Projectһ�£����ǲ�����\app\build\intermediates\ndk\debug\��������so�ļ�������ǣ���ôӦ�ô󹦸�ɣ����������ˡ�

9.��󣬱�������ǰ�������˰Ѵ���\app\build\intermediates\classes\debug\��Terminal�����й��߹ص�������ռ������ļ��лᵼ�±��벻�ɹ�����Ϊ�����ʱ�򣬻��build��Ķ���ȫ��ɾ�����������ɣ����ռ�õĻ���ɾ�����˾ͻ����























