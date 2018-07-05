package com.warningsys.phonedevice;

import android.app.Application;
import com.device.nativecaller.DeviceNativeCaller;

public class MyApplication extends Application {

    //
    private static MyApplication ourInstance = new MyApplication();
    public static MyApplication Ins() {
        return ourInstance;
    }
    private MyApplication() {
    }

    void ApplicationInit() {
        DeviceNativeCaller.Init(MyApplication.this);
    }
    void ApplicationDeinit() {
        DeviceNativeCaller.Free();
    }

//    @SuppressWarnings("unused")
//    public void VideoYUVData(byte[] yuv, int len) {
//        Log.d("video", "VideoData: len :+ " + len);
//        if (faceTrack != null) {
//            synchronized(MyApplication.class) {
//                runTrack(yuv);
//            }
//        }
//    }
}
