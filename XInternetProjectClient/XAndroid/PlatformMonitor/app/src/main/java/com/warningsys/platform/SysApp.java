package com.warningsys.platform;

import android.app.Application;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Environment;
import android.util.Log;
import android.widget.Toast;

import com.alertdialogpro.AlertDialogPro;
import com.nostra13.universalimageloader.core.ImageLoader;
import com.nostra13.universalimageloader.core.ImageLoaderConfiguration;
import com.platform.nativecaller.DeviceListItem;
import com.platform.nativecaller.HislogListItem;
import com.platform.nativecaller.NativeCaller;
import com.platform.util.PcmAudioPlayer;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by kiki on 2016/7/28.
 */
public class SysApp extends Application {
    private String LdTag = "PlatSysApplication-->>";
    public Context context = null;
    public int channel = 0;// 通道号码(默认从0开始)
    private boolean bInit = false;
    public PcmAudioPlayer audioPlayer = null;
    public List<DeviceListItem> dev_list_device = new ArrayList<DeviceListItem>();
    public List<DeviceListItem> dev_list_live = new ArrayList<DeviceListItem>();
    //
    //
    //
    public int Position = 0;
    public boolean POSITION_FROM_LIVE = false;//是从直播页面传值过来的吗？
    public String PreviewUrl;
    //
    //
    //
    public String mIp ;//= "xplat.srikam.com";//13.75.108.171
    public int mPort = 6499;
    //
    public String mUser;
    public String mPassword;
    //
    public AppMainActivity MainAct = null;
    //
    String strLivePic = "LivePic";
    String strLiveRecord = "LiveRecord";
    public String picPath;//图像路径 //live 在该目录下增加了 LivePic 特殊目录
    public String sdRecPath;//录像路径//live 在该目录下增加了 LiveRecord 特殊目录
    //
    public boolean isOnlineDownMp4 = false;
    public String LocalMp4File;//要播放的mp4的地址
    //
    public HislogListItem HislogItem;



    private static SysApp ourInstance = new SysApp();

    public static SysApp Ins() {
        return ourInstance;
    }

    private SysApp() {
    }
    public void SetContext(Context con) {
        ImageLoader.getInstance().init(ImageLoaderConfiguration.createDefault(con));
        context = con;
    }
    public void onInitNative() {
        if (!SysApp.Ins().bInit) {
            Log.i(LdTag, "onInitNative");
            //System.loadLibrary("xmedia");

            picPath = getSDPath() + "/PLAT/Picture/";
            sdRecPath = getSDPath() + "/PLAT/Record";


            NativeCaller.Init();
            NativeCaller.SetMsgNotify(SysApp.Ins());
            SysApp.Ins().bInit = true;
        }

    }

    public void onDestroyNative() {
        Log.i(LdTag, "onDestroyNative");
        if (SysApp.Ins().bInit) {
            SysApp.Ins().bInit = false;
            NativeCaller.ResetMsgNotify();
            NativeCaller.Free();
        }
    }

    public String getSDPath() {
        File sdDir = null;
        boolean sdCardExist = Environment.getExternalStorageState().equals(
                Environment.MEDIA_MOUNTED); // 判断sd卡是否存在
        if (sdCardExist) {
            sdDir = Environment.getExternalStorageDirectory();// 获取跟目录
        }
        return sdDir.toString();


        //return  Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES).toString();
    }


//    private AlertDialog.Builder createAlertDialogBuilder() {
//        if (mTheme == NATIVE_THEME) {
//            return new AlertDialog.Builder(this);
//        }
//
//        return new AlertDialogPro.Builder(this, mTheme);
//    }
//
//    private AlertDialog createProgressDialog() {
//        if (mTheme == NATIVE_THEME) {
//            return new ProgressDialog(this);
//        }
//
//        return new ProgressDialogPro(this, mTheme);
//    }
    private class ButtonClickedListener implements DialogInterface.OnClickListener {
        private CharSequence mShowWhenClicked;

        public ButtonClickedListener(CharSequence showWhenClicked) {
            mShowWhenClicked = showWhenClicked;
        }

        @Override
        public void onClick(DialogInterface dialog, int which) {

        }
    }

    //弹出一个提示
    private Toast mToast = null;
    public void showToast(Context con, int messageId) {
        if (mToast != null) {
            mToast.cancel();
        }
        mToast = Toast.makeText(con, messageId, Toast.LENGTH_SHORT);
        mToast.show();
    }
    //弹出一个消息对话框
    public void showMessageAlertDialog(String strTitle,String strMessage) {
        new AlertDialogPro.Builder(SysApp.Ins().context)
                .setTitle(strTitle)
                .setMessage(strMessage)
                .setPositiveButton(R.string.app_ok, null)
                .show();
    }
//    private void showConfirmedAlertDialog(String strTitle,String strMessage){
//        new AlertDialogPro.Builder(SysApp.Ins().context)
//                .setTitle(strTitle)
//                .setMessage(strMessage)
//                .setPositiveButton(R.string.app_cancel, null)
//                .setPositiveButton(R.string.app_ok, new ButtonClickedListener("ok"))
//                .show();
//    }
    //-------------------------------------------------------------

    public int getAppBarHeight(Context con)
    {
        return dip2px(56,con)+getStatusBarHeight(con);
    }


    public int getStatusBarHeight(Context con)
    {
        int result = 0;
        int resourceId = con.getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0)
        {
            result = con.getResources().getDimensionPixelSize(resourceId);
        }
        return result;
    }


    public  int dip2px(float dipValue,Context con)
    {
        final float scale = con.getResources().getDisplayMetrics().density;
        return (int) (dipValue * scale + 0.5f);
    }

    /******************************************************************/
    public DeviceListItem GetDeviceItemByIndex(int i)
    {
        if(POSITION_FROM_LIVE)
        {
            return GetLiveDeviceItemByIndex(i);
        }
        else
        {
            return GetUserDeviceItemByIndex(i);

        }
    }


    public int GetUserDeviceCount() {
        return dev_list_device.size();
    }

    public DeviceListItem GetUserDeviceItemByIndex(int i) {
        return dev_list_device.get(i);
    }

    public DeviceListItem GetUserDeviceItemByDbid(int dbid) {

        for (int i = 0; i < GetUserDeviceCount(); i++) {
            if (dbid == dev_list_device.get(i).plat_dev_id) {
                return dev_list_device.get(i);
            }
        }

        return null;
    }

    public int GetLiveDeviceCount() {
        return dev_list_live.size();
    }

    public DeviceListItem GetLiveDeviceItemByIndex(int i) {
        return dev_list_live.get(i);
    }

    public DeviceListItem GetLiveDeviceItemByDbid(int dbid) {

        for (int i = 0; i < GetLiveDeviceCount(); i++) {
            if (dbid == dev_list_live.get(i).plat_dev_id) {
                return dev_list_live.get(i);
            }
        }

        return null;
    }
    public int InsertDeviceListItem(DeviceListItem dev) {

        if(0 == dev.plat_dev_ispublish)
        {
            dev_list_device.add(dev);
        }
        else
        {
            if(dev.plat_dev_online == 0)
                return 0;

            dev_list_live.add(dev);
        }


        return 0;
    }
//    public int DeleteAllDevice() {
//
//        return 0;
//    }



    // public int UpdataDeviceListItem(int index, DeviceListItem dev) {
    // int _id = dev_list.get(index).plat_dev_id;
    // dev_list.set(index, dev);
    //
    // return 0;
    // }



    /******************************************************************/
    public int startLivestream(DeviceListItem dev) {
//        Log.i("TAG", ">>>>>>>>>> 播放视频：" + "###" + dev.plat_dev_publish_ip
//                + "###" + dev.plat_dev_publish_port + "###" + mUser + "###"
//                + mPassword + "###[" + dev.plat_dev_id + "]###");

        if (POSITION_FROM_LIVE)
        {
            return NativeCaller.StartLivestream(Integer.toString(dev.plat_dev_id));
        }
        else
        {
            return NativeCaller.StartLivestream(Integer.toString(dev.plat_dev_id));
        }


    }

    public int stopLivestream() {
        return NativeCaller.StopLivestream();
    }




    public boolean GetDevList()
    {
        synchronized(this) {

            //
            NativeCaller.SetInfo(SysApp.Ins().mIp, SysApp.Ins().mPort, SysApp.Ins().mUser, SysApp.Ins().mPassword);

            DeviceListItem[] list = NativeCaller.GetDeviceListClassic();
            if (list != null) {
                dev_list_device.clear();
                dev_list_live.clear();

                for (int i = 0; i < list.length; i++) {
                    SysApp.Ins().InsertDeviceListItem(list[i]);
                }
                return true;
            }
        }




//        if (list != null) {
//            for (int i = 0; i < list.length; i++) {
//                SysApp.Ins().InsertDeviceListItem(list[i]);
//            }
//            return true;
//        }

        return  false;
    }


    //用户观看视频结束后调用该接口，那么用户的列表的图片就会改变
    public int CapDevLogoPic(int dbid, boolean isLive)
    {
        String s;
        if (isLive)
        {
            s = picPath + "Logo/" + strLivePic + "/" + dbid;
        }
        else
        {
            s = picPath + "Logo/"  + dbid;// getid //防止特殊符号
        }

        NativeCaller.CapPic(s, "logo.jpg");

        if (isLive)
        {
            MainAct.FFragmLive.mAdapter.notifyDataSetChanged();
        }
        else
        {
            MainAct.FFragmDevice.mAdapter.notifyDataSetChanged();
        }




        return 0;
    }


    public int CapPic(int dbid, String filename,boolean isLive) {
        String s;
        if (isLive)
        {
            s = picPath + strLivePic;
        }
        else
        {
            s = picPath + dbid;// getid //防止特殊符号
        }

        Log.d("PPPPCapPic", s + "/" + filename);
        //
        NativeCaller.CapPic(s, filename);
//        //----------------------------------------------
//        //发一个系统广播通知手机有图片更新
//        Intent intent = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
//        Uri uri = Uri.parse(s);
//        intent.setData(uri);
//        context.sendBroadcast(intent);//
//        //----------------------------------------------
        return 0;
    }

    public int ChannelRecord(int dbid, int start,boolean isLive) {
        Log.d("TAG", ">>>>>>>>>> ChannelRecord");

        if (isLive)
        {
            NativeCaller.ChannelRecord(sdRecPath, strLiveRecord, start);
        }
        else
        {
            String path;
            String s = String.valueOf(dbid);
            if(SysApp.Ins().isOnlineDownMp4)
            {
                s = s + "/Download";
            }


            NativeCaller.ChannelRecord(sdRecPath, s, start);
        }
        return 0;
    }

    public int PtzAction(int dbid, int lDirection, int val1) {
        // Log.d(TAG, ">>>>>>>>>> ptz_control deviceID:" + dev.getDeviceID()
        // NativeCaller.ctr(dbid, lDirection, 32);
        return 0;
    }

    public int startOnlineAudio(){
        if (null == audioPlayer) {
            audioPlayer = new PcmAudioPlayer(8000);
            audioPlayer.AudioPlayStart();
        }
        return 0;
    }
    public int stopOnlineAudio() {
        if (audioPlayer != null) {
            audioPlayer.AudioPlayStop();
            audioPlayer = null;
        }
        return 0;
    }
    public int startAudio() {
        if (null == audioPlayer) {
            audioPlayer = new PcmAudioPlayer(8000);
            audioPlayer.AudioPlayStart();
        }
        return NativeCaller.CtrlLivestreamAudio(SysApp.this, 1);
    }

    public int stopAudio() {
        if (audioPlayer != null) {
            audioPlayer.AudioPlayStop();
            audioPlayer = null;
        }
        return NativeCaller.CtrlLivestreamAudio(SysApp.this, 0);
    }

    @SuppressWarnings("unused")
    private void AudioData(byte[] pcm, int len) {
        //Log.d("audio", "AudioData: len :+ " + len);
        if (audioPlayer != null)
            audioPlayer.addData(pcm, len);
    }

    /******************************************************************/

    int n = 1;

    // public int onStartCommand(Intent intent, int flags, int startId) {
    // Log.i("INFO", "BridgeMyService------>onStartCommand() executed");

    // return super.onStartCommand(intent, flags, startId);
    // }

//    /**
//     * 消息回调,jni used
//     *
//     * @param login_id
//     * @param msgType
//     * @param param
//     */
	/*
	 * enum _MESSAGE_TYPE { _X_MSG_LOGIN_CONNECTING = 0, _X_MSG_LOGIN_CONNECTED,
	 *
	 * _X_MSG_OPEN_STREAM_CONNECTING, _X_MSG_OPEN_STREAM_CONNECTING_ERROR,
	 * _X_MSG_OPEN_STREAM_CONNECTED, _X_MSG_CLOSE_STREAM, };
	 */

	/*
	 * //网络状态 #define XMSG_NET_STATUS 0 #define
	 * XMSG_NET_STATUS____CONNECT_SUCCESS 0 #define
	 * XMSG_NET_STATUS____CONNECT_FAIlED 1 //INCLUDE : NOT CONNCTED or TIME_OUT
	 * #define XMSG_NET_STATUS____CONNECTING 2 #define
	 * XMSG_NET_STATUS____CONNECT_INVALID_PARAM 3 #define
	 * XMSG_NET_STATUS____DEVICE_NOT_ON_LINE 4
	 */
    @SuppressWarnings("unused")
    private void MsgNotify(int devid, int msgType, int msgSubType,
                           byte[] param1, byte[] param2) {

		/*
		 * if(msgType == 0) { if(param == 2) { if(aty_play != null) {
		 * //aty_play.mHandler.sendEmptyMessage(aty_play.TASK_CONN_ING); } }
		 *
		 * if(param == 1) { if(aty_play != null) {
		 * aty_play.mHandler.sendEmptyMessage(aty_play.TASK_CONN_ERROR); } }
		 * if(param == 0) { if(aty_play != null) {
		 * aty_play.mHandler.sendEmptyMessage(aty_play.TASK_CONN_ED); } }
		 *
		 * }
		 */

		/*
		 * // ----------------MSG_NOTIFY_TYPE_PPPP_STATUS 0 if (0 == msgType) {
		 *
		 * if (null != mainActivity) { mainActivity.setDataSetChanged(); } }
		 * else if (1 == msgType) {
		 *
		 * }
		 */

    }
}
