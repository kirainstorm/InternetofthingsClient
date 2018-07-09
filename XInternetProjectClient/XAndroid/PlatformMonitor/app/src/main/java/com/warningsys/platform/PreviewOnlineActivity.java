package com.warningsys.platform;

import android.content.DialogInterface;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.KeyEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.SeekBar;

import com.alertdialogpro.AlertDialogPro;
import com.platform.nativecaller.DeviceListItem;
import com.platform.nativecaller.NativeCaller;
import com.platform.util.DemoRenderer;

public class PreviewOnlineActivity extends AppCompatActivity {

    public boolean bOpenHisPlay = false; //是否打开了历史回访

    public boolean bThreadRuning = false;
    public Thread checkThread = null;

    private DeviceListItem dev;
    private int Position = -1;

    public DemoRenderer myRender;
    public GLSurfaceView myGlSurfaceView;

    boolean bPause = false;
    ImageView mImgPause;
    ImageView mImgSave;
    /*
    SeekBar的常用属性有以下一些：
    android:max[integer]//设置拖动条的最大值
    android:progress[integer]//设置当前的进度值
    android:secondaryProgress[integer]//设置第二进度，通常用做显示视频等的缓冲效果
    android:thumb[drawable]//设置滑块的图样
    android:progressDrawable[drawable]//设置进度条的图样

    SeekBar事件监听器
　　在使用SeekBar时，我们可以通过设置事件监听器setOnSeekBarChangeListener来获取SeekBar的当前状态。在SeekBar中通常需要监听以下3个事件：
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser);
    public void onStartTrackingTouch(SeekBar seekBar);
    public void onStopTrackingTouch(SeekBar seekBar);
　　其中，onProgressChanged()用于监听SeekBar进度值的改变；onStartTrackingTouch()用于监听SeekBar开始拖动；onStopTrackingTouch()用于监听SeekBar停止拖动。

    */
    SeekBar seeekbar;
    int allsec = 0;//文件长度 秒
    int playsec = 0;//播放进度 秒
    int downsec = 0;//下载进度 秒
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_preview_online);

        //
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        toolbar.setVisibility(View.GONE);

        //
        Position = SysApp.Ins().Position;
        dev = SysApp.Ins().GetDeviceItemByIndex(Position);
        //
        myGlSurfaceView = (GLSurfaceView) findViewById(R.id.surfaceView);
        myRender = new DemoRenderer(myGlSurfaceView);
        myGlSurfaceView.setRenderer(myRender);
        //
        seeekbar = (SeekBar)findViewById(R.id.seekBar);
        seeekbar.setMax(100);//总体刻度
        seeekbar.setProgress(0);//播放进度
        seeekbar.setSecondaryProgress(0);//缓存进度
        //
        NativeCaller.openglResume();

        //SysApp.this
        SysApp.Ins().startOnlineAudio();


        int ret = NativeCaller.OpenHisLogOnlinePlay(SysApp.Ins(),dev.plat_dev_playback_ip,dev.plat_dev_playback_port, SysApp.Ins().mUser,
                SysApp.Ins().mPassword,dev.plat_dev_id,
                SysApp.Ins().HislogItem.his_time_start_year,SysApp.Ins().HislogItem.his_time_start_mon,SysApp.Ins().HislogItem.his_time_start_day,
                SysApp.Ins().HislogItem.his_time_start_hour,SysApp.Ins().HislogItem.his_time_start_min,SysApp.Ins().HislogItem.his_time_start_sec,
                SysApp.Ins().HislogItem.his_time_end_year,SysApp.Ins().HislogItem.his_time_end_mon,SysApp.Ins().HislogItem.his_time_end_day,
                SysApp.Ins().HislogItem.his_time_end_hour,SysApp.Ins().HislogItem.his_time_end_min,SysApp.Ins().HislogItem.his_time_end_sec);
        if (0 == ret)
        {
            bOpenHisPlay = true;//<<<<<<<<<<<<<<<<<<<<<<<<<<<======================


            mImgPause = (ImageView)findViewById(R.id.imageViewPause);
            mImgSave = (ImageView)findViewById(R.id.imageViewSave);
            //
            allsec =  NativeCaller.GetHisLogProgress();
            seeekbar.setMax(allsec);//总体刻度
            //
            OpenCheckThread();

            seeekbar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()
            {
                public void onStopTrackingTouch(SeekBar seekBar)
                {
                    int progress = seekBar.getProgress();
                    if (progress > downsec)
                    {
                        OpenCheckThread();
                        return;
                    }
                    NativeCaller.HisLogSeek(progress);
                    OpenCheckThread();
                }
                public void onStartTrackingTouch(SeekBar seekBar)
                {
                    CloseCheckThread();
                }
                public void onProgressChanged(SeekBar seekBar, int progress,boolean fromUser)
                {

                }
            });



            mImgPause.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    bPause = !bPause;
                    NativeCaller.HisLogPause(bPause?1:0);
                    if (bPause)
                        mImgPause.setImageDrawable(getResources().getDrawable(R.drawable.zz_playback_pause_sel));
                    else
                        mImgPause.setImageDrawable(getResources().getDrawable(R.drawable.zz_playback_pause));
                }
            });


            mImgSave.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    new AlertDialogPro.Builder(PreviewOnlineActivity.this)
                            .setTitle(getResources().getString(R.string.app_save))
                            .setMessage(getResources().getString(R.string.app_save_1))
                            .setNegativeButton(R.string.app_cancel, null)
                            .setPositiveButton(R.string.app_ok, new SaveClickedListener("ok"))
                            .show();
                }
            });
        }
        else
        {
            SysApp.Ins().showToast(PreviewOnlineActivity.this,R.string.app_cannot_open_hisplay);
        }


    }

    private class SaveClickedListener implements DialogInterface.OnClickListener {
        private CharSequence mShowWhenClicked;

        public SaveClickedListener(CharSequence showWhenClicked) {
            mShowWhenClicked = showWhenClicked;
        }

        @Override
        public void onClick(DialogInterface dialog, int which) {
            CloseCheckThread();
            NativeCaller.HisLogWriteMp4File(SysApp.Ins().sdRecPath + "/Mp4Files/"+ dev.plat_dev_id+"/Download/","");
            OpenCheckThread();
            SysApp.Ins().showToast(PreviewOnlineActivity.this,R.string.app_save_2);
        }
    }
    class CHECK_Thread implements Runnable {
        @Override
        public void run() {
            while (bThreadRuning) {



                Message message = new Message();
                message.what = 0;
                PreviewOnlineActivity.this.myHandler.sendMessage(message);
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }
    Handler myHandler = new Handler() {
        public void handleMessage(Message msg) {
            //
            allsec =  NativeCaller.GetHisLogProgress();
            playsec =  NativeCaller.GetHisLogPlayingProgress();
            downsec =  NativeCaller.GetHisLogOnlineDownProgress();
            //
            seeekbar.setMax(allsec);
            seeekbar.setProgress(playsec);
            seeekbar.setSecondaryProgress(downsec);


            //Log.d("pos","+++++++++++++++++++++++++++++============" + allsec + "   " +playsec+ "   " +downsec);

            super.handleMessage(msg);
        }
    };

    public void OpenCheckThread()
    {
        //synchronized(this) {
            if (!bThreadRuning)
            {
                bThreadRuning = true;
                checkThread = new Thread(new CHECK_Thread());
                checkThread.start();
            }
       // }
    }
    public void CloseCheckThread()
    {
       // synchronized(this) {
            if (bThreadRuning)
            {
                bThreadRuning = false;
                try {
                    checkThread.join();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
       // }
    }
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        // 点击了返回按键
        if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) { // 获取
            closeHisplay();
            finish();
        }
        return super.onKeyDown(keyCode, event);
    }
    @Override
    protected void onStop() {
        closeHisplay();
        super.onStop();
    }
    @Override
    protected void onDestroy() {
        NativeCaller.openglClear();
        NativeCaller.openglPause();
        NativeCaller.openglStop();
        super.onDestroy();
    }

    public void closeHisplay()
    {
        SysApp.Ins().stopOnlineAudio();
        synchronized(this) {
            if (bOpenHisPlay)
            {
                CloseCheckThread();
                NativeCaller.CloseHisLogOnlinePlay();
                bOpenHisPlay = false;
            }
        }
    }



//
//
//    public class MySeekBar extends SeekBar
//    {
//        private Bitmap controlPointBitmap;//控制点图片
//        int seekBarBgHeight=12;//进度条背景宽度
//        Paint progressPaint;//进度条画笔
//        Paint bgPaint;//背景画笔
//        Paint cachPaint;//缓冲进度条画笔
//        int progressBarWidth=0;
//        int progressBarHeight=0;
//        private int lastCachProg=0;//secondProgress的上一次位置
//        private Region cachRegion;//缓冲进度条已缓冲区域
//        private Rect currentRect;//当前缓冲进度更新的一段距离
//        private float changeSizeScale;//全屏切换缩放比例
//
//        public MySeekBar(Context context) {
//            super(context);
//
//
//        }
//
//        private void init(Context context){
//            controlPointBitmap= BitmapFactory.decodeResource(getResources(),R.drawable.btn_control_points);
//            progressPaint=new Paint();
//            progressPaint.setColor(getResources().getColor(R.color.seekbar_progresscolor));
//            bgPaint=new Paint();
//            bgPaint.setColor(getResources().getColor(R.color.seekbar_bg));
//            cachPaint=new Paint();
//            cachPaint.setColor(getResources().getColor(R.color.seekbar_cachcolor));
//        }
//
//
//        @SuppressLint("DrawAllocation")
//        @Override
//        protected synchronized void onDraw(Canvas canvas) {
//            if(progressBarWidth!=0&&progressBarWidth!=getWidth()){//进度条大小是否改变
//                changeSizeScale=(float)getWidth()/(float)progressBarWidth;
//            }
//            progressBarWidth = getWidth();
//            progressBarHeight = getHeight();
//            int pTop=(progressBarHeight-seekBarBgHeight)/2;
//            int pBottom=(progressBarHeight-seekBarBgHeight)/2+seekBarBgHeight;
//            Drawable thumb=getThumb();
//            //绘制进度条背景
//            Rect bgRect = new Rect(0,pTop,progressBarWidth,pBottom);
//            canvas.drawRect(bgRect, bgPaint);
//            //绘制缓冲进度
//            drawCachProgress(canvas,progressBarWidth,pTop,pBottom);
//
//            //绘制播放进度
//            Rect progressRect = new Rect(0,pTop,thumb.getBounds().left,pBottom);
//            canvas.drawRect(progressRect, progressPaint);
//            //绘制控制点
//            canvas.drawBitmap(controlPointBitmap,thumb.getBounds().left,progressBarHeight/2-controlPointBitmap.getWidth()/2,null);
//        }
//        //计算绘制缓冲进度
//        private void drawCachProgress(Canvas canvas,int progressBarWidth,int pTop,int pBottom){
//            if(changeSizeScale!=0){//进度条尺寸变化了重新计算一遍已缓冲进度
//                Region region=getCachRegion(pTop,pBottom);
//                cachRegion=new Region(0, pTop,1, pBottom);
//                RegionIterator iter = new RegionIterator(region);
//                Rect r = new Rect();
//                while (iter.next(r)) {
//                    r.set((int)(r.left*changeSizeScale), r.top,(int)(r.right*changeSizeScale), r.bottom);
//                    cachRegion.op(r, Op.UNION);//求并集
//                }
//                changeSizeScale=0;
//            }
//
//            int secondProg=getSecondaryProgress();
//            float scale=(float)secondProg/(float)getMax();
//
//            if((secondProg-lastCachProg)>=0&&(secondProg-lastCachProg)<=2000){//小于2s的都视为没有拖动
//                if(currentRect==null){
//                    currentRect= new Rect(0,pTop,(int)(scale*progressBarWidth),pBottom);
//                }
//                currentRect.right=(int)(scale*progressBarWidth);
//            }else{//手指拖动了 重新定位
//                if(currentRect!=null){
//                    currentRect.left=(int)(scale*progressBarWidth);//定位到新的位置
//                    currentRect.right=(int)(scale*progressBarWidth)+1;
//                }
//            }
//            lastCachProg=secondProg;
//            if(currentRect!=null){
//                getCachRegion(pTop,pBottom).op(currentRect, Op.UNION);//求并集
//            }
//            drawRegion(canvas,getCachRegion(pTop,pBottom),cachPaint);//绘制并集
//        }
//
//        //绘制并集
//        private void drawRegion(Canvas canvas, Region rgn, Paint paint)
//        {
//            RegionIterator iter = new RegionIterator(rgn);
//            Rect r = new Rect();
//            while (iter.next(r)) {
//                canvas.drawRect(r, paint);
//            }
//        }
//        //懒加载
//        private Region getCachRegion(int pTop,int pBottom) {
//            if(cachRegion==null){
//                cachRegion=new Region(0, pTop,1, pBottom);
//            }
//            return cachRegion;
//        }
//
//
//
//    }
}
