
chmod 777 -R ./*

System.loadLibrary("ljx"); //ȥ��ǰ׺lib������׺.so

����˳��
		System.loadLibrary("avutil-52");
		System.loadLibrary("avcodec-55");
		//System.loadLibrary("swresample-0");
		System.loadLibrary("avformat-55");
		System.loadLibrary("swscale-2");
		//System.loadLibrary("avfilter-4");
		//System.loadLibrary("avdevice-55");
	
		
		System.loadLibrary("crypto");
		System.loadLibrary("ssl");
		
		
map:
		����������//g_mapdevice[nmap] = pdev;
	     Ҫ����g_mapdevice.insert(map<int, CKSDevInterface *>::value_type(devid,pdev));



cd /cygdrive/G/LatestPlatform/WinclientAndApp/Android/jni

$NDK/ndk-build
