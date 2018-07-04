package com.platform.nativecaller;

import android.content.Context;

/**
 * Created by kiki on 2016/7/28.
 */
public class NativeCaller {
//        static {
//            System.loadLibrary("crypto");
//            System.loadLibrary("ssl");
//            System.loadLibrary("xmedia");
//        }

    //
    public native static void Init();

    public native static void Free();

    //
    public native static void SetMsgNotify(Context object);

    public native static void ResetMsgNotify();

    //
    public static native void openglViewport(int x, int y,int width, int height); // onSurfaceChanged

    public static native void openglStep(); // onDrawFrame

    public static native void openglInit();// onSurfaceCreated

    public static native void openglStop(); //

    public static native void openglPause(); //

    public static native void openglResume(); //

    public static native void openglClear(); //

    //
    public native static DeviceListItem[] GetDeviceList(String ip, int port,
                                                        String user, String pwd);

    //
    public native static int StartLivestream(int devid, int channel, String ip,
                                             int port, String user, String pwd);

    public native static int StopLivestream();

    public native static int CtrlLivestreamAudio(Context context_obj, int open);

    public native static int ChannelRecord(String filepath, String filepathEx,
                                           int start);

    public native static int CapPic(String filepath, String filename);
    public native static int GetLivestreamStatus();


    public native static HislogListItem[] GetHisLogList(String ip, int port, String user, String pwd ,int devid,int year ,int mon , int day ,int starthour,int startmin);




    public native static int OpenHisLogOnlinePlay(Context context_obj,String ip, int port, String user, String pwd ,int devid,
                                                  int startyear ,int startmon , int startday ,int starthour,int startmin,int startsec,
                                                  int endyear ,int endtmon , int endday ,int endhour,int endmin,int endsec);
    public native static int CloseHisLogOnlinePlay();
    public native static int HisLogPause(int bPause);
    public native static int HisLogSeek(int bPos);

    public native static int GetHisLogProgress();//返回秒值
    public native static int GetHisLogPlayingProgress();//返回秒值
    public native static int GetHisLogOnlineDownProgress();//返回秒值

    public native static int HisLogWriteMp4File(String filepath, String filepathEx);

    // playback
    //public native static int StartLocalPlayback(Context context_obj, String file);
    //public native static int StopLocalPlayback();
    //public native static int CtrlLocalPlayback(int type, int val);

}
