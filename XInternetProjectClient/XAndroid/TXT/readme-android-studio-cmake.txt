>>>>>>>>>>1'x86','x86_64','mips','mips64''armeabi','armeabi-v7a','arm64-v8a'

app---build.gradle-----defaultconfig中
        ndk {
            //选择要编译的对应cpu类型的.so库。
            //'x86','x86_64','mips','mips64''armeabi','armeabi-v7a','arm64-v8a'
            abiFilters 'armeabi-v7a'
        }