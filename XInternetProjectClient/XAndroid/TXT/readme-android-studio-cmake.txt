>>>>>>>>>>1'x86','x86_64','mips','mips64''armeabi','armeabi-v7a','arm64-v8a'

app---build.gradle-----defaultconfig��
        ndk {
            //ѡ��Ҫ����Ķ�Ӧcpu���͵�.so�⡣
            //'x86','x86_64','mips','mips64''armeabi','armeabi-v7a','arm64-v8a'
            abiFilters 'armeabi-v7a'
        }