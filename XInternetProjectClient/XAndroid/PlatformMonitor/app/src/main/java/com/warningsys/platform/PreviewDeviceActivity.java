package com.warningsys.platform;

import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.Matrix;
import android.graphics.PointF;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.Toast;

import com.platform.nativecaller.DeviceListItem;
import com.platform.nativecaller.NativeCaller;
import com.platform.util.DemoRenderer;

import java.text.SimpleDateFormat;
import java.util.Date;
public class PreviewDeviceActivity extends AppCompatActivity implements
        OnClickListener, OnTouchListener{
    private DeviceListItem dev;
    private int Position = -1;
    Toolbar toolbar;

    // ------------------------------------------------
    //
    //private GestureDetector gestureDetector; // 用户滑动
    //
    public boolean bPlayPreview = false;
    public boolean bAudio = false;
    public boolean bRecord = false;
    //
    public boolean PreviewFullScreenStatus = false; // 播放窗口是否横屏全屏状态
    public DemoRenderer myRender;
    public GLSurfaceView myGlSurfaceView;
    private Matrix matrix;
    //
    public RelativeLayout PreviewLayoutMain;
    public RelativeLayout PreviewBottomlayout;
    //
    public ImageView LoadingImage;
    public Thread checkThread = null;
    public boolean bThreadRuning = false;
    public Animation hyperspaceJumpAnimation;
    //
    //
    //
    public ImageButton ImageBtnCapBar;
    public ImageButton ImageBtnRecordBar;
    public ImageButton ImageBtnSoundBar;
    //
    // -----------------------------------------------


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
       //this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);
        setContentView(R.layout.activity_preview_device);
        //
        Position = SysApp.Ins().Position;
        dev = SysApp.Ins().GetDeviceItemByIndex(Position);
        //

        toolbar = (Toolbar) findViewById(R.id.toolbar);
        //toolbarMinHeight = toolbar.getMinimumHeight();
        //toolbarHeight = toolbar.getHeight();
        toolbar.setTitle(getResources().getString(R.string.app_preview_device));//标题
        toolbar.setSubtitle(dev.plat_dev_name);//副标题
        setSupportActionBar(toolbar);
        toolbar.setNavigationIcon(R.drawable.ic_action_back);//设置Navigation 图标
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
        //-------------------------------------------------------------
        //
        matrix = new Matrix();
        myGlSurfaceView = (GLSurfaceView) findViewById(R.id.myGlSurfaceView);
        myRender = new DemoRenderer(myGlSurfaceView);
        myGlSurfaceView.setRenderer(myRender);
        myGlSurfaceView.setOnTouchListener(this);
        bPlayPreview = false;
        //
        PreviewLayoutMain = (RelativeLayout) findViewById(R.id.preview_layout_main);
        PreviewLayoutMain.setOnTouchListener(this);
        PreviewLayoutMain.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                if (!PreviewFullScreenStatus) {
                    //
                } else {
//                    if (PreviewBottomlayout.getVisibility() == View.INVISIBLE) {
//                        PreviewBottomlayout.setVisibility(View.VISIBLE);
//                    } else {
//                        PreviewBottomlayout.setVisibility(View.INVISIBLE);
//                    }
                    //myRender.ZOOMTEST();
                }
            }
        });
        PreviewBottomlayout = (RelativeLayout) findViewById(R.id.preview_bottom_layout);
        //PreviewBottomlayout.setVisibility(View.INVISIBLE);
        //
        //
        //
        // -----------------------------------------------------------------------------------
        // -----------------------------------------------------------------------------------
        ImageBtnCapBar = (ImageButton) findViewById(R.id.btnBarCapDevice);
        ImageBtnRecordBar = (ImageButton) findViewById(R.id.btnBarRecDevice);
        ImageBtnSoundBar = (ImageButton) findViewById(R.id.btnBarSoundDevice);
        //
        ImageBtnCapBar.setImageResource(R.drawable.zz_player_bar_cap_selector);
        ImageBtnRecordBar
                .setImageResource(R.drawable.zz_player_bar_record_selector);
        ImageBtnSoundBar
                .setImageResource(R.drawable.zz_player_bar_sound_selector);
        //
        ImageBtnCapBar.setOnClickListener(this);
        ImageBtnRecordBar.setOnClickListener(this);
        ImageBtnSoundBar.setOnClickListener(this);
        //-------------------------------------------------------------
        //
        // 继承普通Activity
        // <TabWidget>标签id必须为"@android:id/tabs"
        // <FrameLayout>标签id必须为"@android:id/tabcontent"
        // 这个方式在通过findViewById获得TabHost之后，必须要调用setup方法。
        //
        // 获取TabHost对象

        //-------------------------------------------------------------
        //
        LoadingImage = (ImageView)findViewById(R.id.img_device);
        // 加载动画
        hyperspaceJumpAnimation = AnimationUtils.loadAnimation(this, R.anim.loading_animation);
        // 使用ImageView显示动画
        LoadingImage.startAnimation(hyperspaceJumpAnimation);
        //
        LoadingImage.setVisibility(View.GONE);

        //-------------------------------------------------------------

        //由于用户有可能在横屏状态下打开了视频，而且没有开启锁定手机的方向的功能
        try {
            if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) {
                PreviewFullScreenStatus = true;
                fullScreenChange();
            } else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT) {
                // Toast.makeText(this, "哈哈，切竖屏", Toast.LENGTH_SHORT).show();
                PreviewFullScreenStatus = false;
                fullScreenChange();
            }
        } catch (Exception ex) {
        }
        //gestureDetector = new GestureDetector(new TabHostTouch());
    }
//    @Override
//    public boolean dispatchTouchEvent(MotionEvent event) {
//
//        if (gestureDetector.onTouchEvent(event)) {
//            event.setAction(MotionEvent.ACTION_CANCEL);
//        }
//        return super.dispatchTouchEvent(event);
//
//    }
//    private class TabHostTouch extends GestureDetector.SimpleOnGestureListener  {
//        DisplayMetrics dm = new DisplayMetrics();
//        //getWindowManager().getDefaultDisplay().getMetrics(dm);
//        int flaggingWidth = dm.widthPixels / 3;
//        int currentTabID;
//        @Override
//        public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX,
//                               float velocityY) {
//            if (Math.abs(e1.getX() - e2.getX()) > Math.abs(e1.getY()
//                    - e2.getY())
//                    && (e1.getX() - e2.getX() <= (-flaggingWidth) || e1.getX()
//                    - e2.getX() >= flaggingWidth)) {
//                if (e1.getX() - e2.getX() <= (-flaggingWidth)) {
//                    currentTabID = tabHost.getCurrentTab() - 1;
//                    if (currentTabID < 0) {
//                        currentTabID = 2 - 1;
//                    }
//                    tabHost.setCurrentTab(currentTabID);
//                    return true;
//                } else if (e1.getX() - e2.getX() >= flaggingWidth) {
//                    currentTabID = tabHost.getCurrentTab() + 1;
//                    if (currentTabID >= 2) {
//                        currentTabID = 0;
//                    }
//                    tabHost.setCurrentTab(currentTabID);
//                    return true;
//                }
//            }
//            return false;
//        }
//    }
//
//    class OnTabChangedListener implements OnTabChangeListener {
//
//        @Override
//        public void onTabChanged(String tabId) {
//            tabHost.setCurrentTabByTag(tabId);
//            System.out.println("tabid " + tabId);
//            System.out.println("curreny after: " + tabHost.getCurrentTabTag());
//            updateTab(tabHost);
//        }
//    }
//    //更新Tab标签的颜色，和字体的颜色
//    private void updateTab(final TabHost tabHost) {
//        for (int i = 0; i < tabHost.getTabWidget().getChildCount(); i++) {
//            View view = tabHost.getTabWidget().getChildAt(i);
//            TextView tv = (TextView) tabHost.getTabWidget().getChildAt(i)
//                    .findViewById(android.R.id.title);
//            tv.setTextSize(16);
//            // tv.setTypeface(Typeface.SERIF, 2); // 设置字体和风格
//            if (tabHost.getCurrentTab() == i)
//            {
//                // 选中
//                view.setBackgroundColor(Color.parseColor("#fefefe"));// 选中后的背景
//                tv.setTextColor(Color.parseColor("#00a2e9"));
//            }
//            else
//            {
//                // 不选中
//                view.setBackgroundColor(Color.parseColor("#fefefe"));
//                tv.setTextColor(Color.parseColor("#3a393e"));
//            }
//        }
//    }










    //全屏切换
    public void fullScreenChange() {
        WindowManager.LayoutParams attrs = getWindow().getAttributes();
        if (!PreviewFullScreenStatus) {
            attrs.flags &= (~WindowManager.LayoutParams.FLAG_FULLSCREEN);
            getWindow().setAttributes(attrs);
            // 取消全屏设置
            getWindow().clearFlags(
                    WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS);
        } else {
            attrs.flags |= WindowManager.LayoutParams.FLAG_FULLSCREEN;
            getWindow().setAttributes(attrs);
            getWindow().addFlags(
                    WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS);
        }
        //
        //PreviewBottomlayout.setVisibility(View.INVISIBLE);
        //
        if (PreviewFullScreenStatus) {
            toolbar.setVisibility(View.GONE);
            //
            //tabHost.setVisibility(View.GONE);
            //
            RelativeLayout.LayoutParams layoutParams = (RelativeLayout.LayoutParams) PreviewLayoutMain
                    .getLayoutParams();
            layoutParams.height = getWindow().getAttributes().height;
            PreviewLayoutMain.setLayoutParams(layoutParams);

        } else {
            toolbar.setVisibility(View.VISIBLE);
            //
            //tabHost.setVisibility(View.VISIBLE);
            //
            RelativeLayout.LayoutParams layoutParams = (RelativeLayout.LayoutParams) PreviewLayoutMain
                    .getLayoutParams();
            layoutParams.height = (int) getResources().getDimension(
                    R.dimen.preview_view_height);
            PreviewLayoutMain.setLayoutParams(layoutParams);
        }

    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) {
            // back // 点击了返回按键
            finish();
        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        try {
            super.onConfigurationChanged(newConfig);
            if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) {
                PreviewFullScreenStatus = true;
                fullScreenChange();
            } else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT) {
                // Toast.makeText(this, "哈哈，切竖屏", Toast.LENGTH_SHORT).show();
                PreviewFullScreenStatus = false;
                fullScreenChange();
            }
        } catch (Exception ex) {
        }
        super.onConfigurationChanged(newConfig);
    }

    @Override
    protected void onRestart() {
        super.onRestart();
    }
    @Override
    protected void onResume() {

        if(getRequestedOrientation()!=ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE){
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        }
        PreviewFullScreenStatus = true;
        fullScreenChange();
        OpenPreview();
        super.onResume();
    }
    @Override
    protected void onStart() {
        super.onStart();
    }

    @Override
    protected void onPause()
    {

        super.onPause();
    }




    @Override
    protected void onStop() {

        try {
            bThreadRuning = false;
            checkThread.join();
            checkThread = null;
            ClosePreview();
        } catch (Exception e) {
            Log.d("s", (e.getMessage()));
        }

        super.onStop();
    }
    @Override
    protected void onDestroy() {
        NativeCaller.openglStop();
        super.onDestroy();
    }


    Handler myHandler = new Handler() {
        public void handleMessage(Message msg) {

            int visibility = (1==msg.what)?View.GONE:View.VISIBLE;
            int old_v = LoadingImage.getVisibility();
            if(visibility != old_v)
            {
                if((1==msg.what))
                {
                    LoadingImage.clearAnimation();
                }
                else
                {
                    LoadingImage.startAnimation(hyperspaceJumpAnimation);
                }
                //Log.d("s", "Handler ------ "+visibility);
                LoadingImage.setVisibility(visibility);
            }
            super.handleMessage(msg);
        }
    };
    class CHECK_Thread implements Runnable {
        @Override
        public void run() {
            try {
                Thread.sleep(500);//先不显示转圈圈
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            while (bThreadRuning) {

                int k = NativeCaller.GetLivestreamStatus();
                Message message = new Message();
                message.what = k;
                PreviewDeviceActivity.this.myHandler.sendMessage(message);
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }
//    private float spacing(MotionEvent event) {
//        float x = event.getX(0) - event.getX(1);
//        float y = event.getY(0) - event.getY(1);
//        return (float)Math.sqrt(x * x + y * y);
//    }

    float oldDis,newDis;
    PointF pointMid;
    float startX,startY;

    private PointF getMidPointByEvent(MotionEvent event) {
        PointF point = new PointF();
        point.x = (event.getX() + event.getX(1)) / 2;
        point.y = (event.getY() + event.getY(1)) / 2;
        return point;
    }

    /**
     * 计算两点之间的距离
     *
     * @param event
     */
    private float getDisByXY(MotionEvent event) {
        // 第一个点
        float x = event.getX();
        float y = event.getY();
        // 第二个点
        float x2 = event.getX(1);
        float y2 = event.getY(1);
        return (float)Math.sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
    }
    @Override
    public boolean onTouch(View v, MotionEvent event) {

        int pointerCount = event.getPointerCount();
        if (pointerCount == 1) {
            switch (event.getAction()) {
                case MotionEvent.ACTION_DOWN:
                    startX = event.getX();
                    startY = event.getY();
                    break;
                case MotionEvent.ACTION_MOVE:
                    // 移动
                    float x = event.getX();
                    float y = event.getY();

                    float ppppppp = 2f;
                    if ((Math.abs(x-startX) > ppppppp) || ( Math.abs(y-startY) > ppppppp))
                    {
                        myRender.Move(x-startX,startY-y);
                        startX = event.getX();
                        startY = event.getY();
                    }
                    break;
                case MotionEvent.ACTION_UP:
                    break;
                default:
                    break;
            }
        }
        else if (pointerCount == 2) {
            // 缩放
            switch (event.getAction() & MotionEvent.ACTION_MASK) {
                case MotionEvent.ACTION_POINTER_DOWN:
                    // 第二根手指按下去
                    oldDis = getDisByXY(event);
                    // 缩放的中心点
                    pointMid = getMidPointByEvent(event);

                    break;
                case MotionEvent.ACTION_MOVE:
                    // 两根手指移动

                    newDis = getDisByXY(event);
                    if ((newDis - oldDis) > 1f)
                    {
                        myRender.ZOOM_FANGDA(pointMid.x);
                    }

                    if ((oldDis - newDis) > 1f)
                    {
                        myRender.ZOOM_SUOXIAO(pointMid.x);
                    }


                    //scale = newDis / oldDis;// 缩放比例
                    // 矩阵进行缩放设置
                    //matrix.postScale(scale, scale, pointMid.x, pointMid.y);
                    //img.setImageMatrix(matrix);
                    oldDis = newDis;





                    break;
                default:
                    break;
            }
        }
         return super.onTouchEvent(event);

       // return false;
    }
    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            // ---------------------------------------------------------------------------------------------------------
            case R.id.btnBarCapDevice: {
                //
                Date currentTime = new Date(System.currentTimeMillis());
                SimpleDateFormat format = new SimpleDateFormat("yyyyMMdd_HHmmss");
                String dateString = format.format(currentTime);
                //
                String s = dateString + ".jpg";
                SysApp.Ins().CapPic(dev.plat_dev_id, s,false);
                //
                Toast.makeText(SysApp.Ins().context,
                        R.string.mainfra_s_toast_info1, Toast.LENGTH_SHORT).show();

                AudioManager meng = (AudioManager) SysApp.Ins().context
                        .getSystemService(Context.AUDIO_SERVICE);
                int volume = meng.getStreamVolume(AudioManager.STREAM_NOTIFICATION);

                if (volume != 0) {
                    MediaPlayer shootMP = null;
                    if (shootMP == null)
                        shootMP = MediaPlayer
                                .create(SysApp.Ins().context,
                                        Uri.parse("file:///system/media/audio/ui/camera_click.ogg"));
                    if (shootMP != null)
                        shootMP.start();
                }
            }
            break;
            // ---------------------------------------------------------------------------------------------------------
            case R.id.btnBarRecDevice: {
                if (bRecord) {
                    ImageBtnRecordBar.setImageResource(R.drawable.zz_player_bar_record_selector);
                    SysApp.Ins().showToast(PreviewDeviceActivity.this,R.string.mainfra_s_toast_info3);
                } else {
                    ImageBtnRecordBar.setImageResource(R.drawable.zz_player_bar_record_selector_sel);
                    SysApp.Ins().showToast(PreviewDeviceActivity.this,R.string.mainfra_s_toast_info2);
                }
                bRecord = !bRecord;
                SysApp.Ins().ChannelRecord(dev.plat_dev_id, bRecord ? 1 : 0,false);
            }
            break;
            // ---------------------------------------------------------------------------------------------------------
            case R.id.btnBarSoundDevice: {
                if (bAudio) {
                    SysApp.Ins().stopAudio();
                    //
                    ImageBtnSoundBar.setImageResource(R.drawable.zz_player_bar_sound_selector);
                } else {
                    SysApp.Ins().startAudio();
                    //
                    ImageBtnSoundBar.setImageResource(R.drawable.zz_player_bar_sound_selector_sel);
                }
                bAudio = !bAudio;
            }
            break;
        }

    }



    public void OpenPreview() {
        if (!bPlayPreview) {
            SysApp.Ins().isOnlineDownMp4 = false;
            NativeCaller.openglResume();
            SysApp.Ins().startLivestream(dev);
            bPlayPreview = true;

            bThreadRuning = true;
            checkThread = new Thread(new CHECK_Thread());
            checkThread.start();
        }
    }

    public void ClosePreview() {

        if (bPlayPreview) {

            SysApp.Ins().CapDevLogoPic(dev.plat_dev_id, false);
                //SysApp.Ins().showToast(R.string.app_ok);

            if (bAudio) {
                SysApp.Ins().stopAudio();
                bAudio = false;
            }

            //
            if (bRecord) {
                SysApp.Ins().ChannelRecord(dev.plat_dev_id, 0,false);
                bRecord = false;
            }
            //

            SysApp.Ins().stopLivestream();
            //
            NativeCaller.openglClear();
            NativeCaller.openglPause();
            myGlSurfaceView.invalidate();
            bPlayPreview = false;
            try {
                Thread.sleep(300);//android内存回收太慢，阻塞一下
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
