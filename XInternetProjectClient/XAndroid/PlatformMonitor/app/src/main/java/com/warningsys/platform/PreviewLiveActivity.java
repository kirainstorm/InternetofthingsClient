package com.warningsys.platform;

import android.content.Context;
import android.content.res.Configuration;
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
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.webkit.WebView;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.Toast;

import com.platform.nativecaller.DeviceListItem;
import com.platform.nativecaller.NativeCaller;
import com.platform.util.DemoRenderer;

import java.text.SimpleDateFormat;
import java.util.Date;

public class PreviewLiveActivity extends AppCompatActivity  implements OnClickListener{
    private DeviceListItem dev;
    private int Position = -1;
    Toolbar toolbar;
    WebView webView;
    public boolean bPlayPreview = false;
    public boolean PreviewFullScreenStatus = false; // 播放窗口是否横屏全屏状态
    public ImageButton ImageBtnCapBar;
    public ImageButton ImageBtnRecordBar;
    public ImageButton ImageBtnSoundBar;
    public ImageButton ImageBtnTalkBar;
    public boolean bAudio = false;
    public boolean bTalk = false;
    public boolean bRecord = false;

    public GLSurfaceView myGlSurfaceView;
    // -----------------------------------------------
    public RelativeLayout PreviewLayoutMain;
    public RelativeLayout PreviewBottomlayout;
    // public RelativeLayout PreviewTopLayout;
    public ImageView LoadingImage;
    public Thread checkThread = null;
    public boolean bThreadRuning = false;
    Animation hyperspaceJumpAnimation;
    // -----------------------------------------------
    // public TabHost tabHost;

    private final int MINLEN = 80;
    private boolean lefRitUpDowTag = false;
    DemoRenderer myRender;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        //this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);
        setContentView(R.layout.activity_preview_live);
        //
        Position = SysApp.Ins().Position;// bundle.getInt("Position");
        dev = SysApp.Ins().GetDeviceItemByIndex(Position);
        //
        toolbar = (Toolbar) findViewById(R.id.toolbar);
        toolbar.setTitle(getResources().getString(R.string.app_live));//标题
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

//        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
//        fab.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
//                        .setAction("Action", null).show();
//            }
//        });

        //-------------------------------------------------------------
        //
        myGlSurfaceView = (GLSurfaceView) findViewById(R.id.myGlSurfaceView);
        myRender = new DemoRenderer(myGlSurfaceView);
        myGlSurfaceView.setRenderer(myRender);
        //myGlSurfaceView.setOnTouchListener(this);
        bPlayPreview = false;
        //
        PreviewLayoutMain = (RelativeLayout) findViewById(R.id.preview_layout_main);
        PreviewLayoutMain.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
//                if (!PreviewFullScreenStatus) {
//                    //
//                } else
                {
//                    if (PreviewBottomlayout.getVisibility() == View.INVISIBLE) {
//                        PreviewBottomlayout.setVisibility(View.VISIBLE);
//                    } else {
//                        PreviewBottomlayout.setVisibility(View.INVISIBLE);
//                    }
                }
            }
        });
        PreviewBottomlayout = (RelativeLayout) findViewById(R.id.preview_bottom_layout);
        //PreviewBottomlayout.setVisibility(View.INVISIBLE);
        PreviewBottomlayout.setVisibility(View.VISIBLE);


        //
        //
        //
        // -----------------------------------------------------------------------------------
        // -----------------------------------------------------------------------------------
        ImageBtnCapBar = (ImageButton) findViewById(R.id.btnBarCapLive);
        ImageBtnRecordBar = (ImageButton) findViewById(R.id.btnBarRecLive);
        ImageBtnSoundBar = (ImageButton) findViewById(R.id.btnBarSoundLive);
        ImageBtnTalkBar = (ImageButton) findViewById(R.id.btnBarTalkLive);
        //
        ImageBtnCapBar.setImageResource(R.drawable.zz_player_bar_cap_selector);
        ImageBtnRecordBar.setImageResource(R.drawable.zz_player_bar_record_selector);
        ImageBtnSoundBar.setImageResource(R.drawable.zz_player_bar_sound_selector);
        ImageBtnTalkBar.setImageResource(R.drawable.zz_player_bar_talk_selector);
        //;
        ImageBtnCapBar.setOnClickListener(this);
        ImageBtnRecordBar.setOnClickListener(this);
        ImageBtnSoundBar.setOnClickListener(this);
        ImageBtnTalkBar.setOnClickListener(this);

        // -------------------------------------------------------------
        //
        LoadingImage = (ImageView) findViewById(R.id.img_chat);
        // 加载动画
        hyperspaceJumpAnimation = AnimationUtils.loadAnimation(this,
                R.anim.loading_animation);
        // 使用ImageView显示动画
        LoadingImage.startAnimation(hyperspaceJumpAnimation);
        //
        // LoadingImage.setVisibility(View.INVISIBLE);
        //
        bThreadRuning = true;
        checkThread = new Thread(new CHECK_Thread());
        checkThread.start();
        // -------------------------------------------------------------
        webView = (WebView) findViewById(R.id.webView_chat);
        (webView.getSettings()).setJavaScriptEnabled(true);
//        webView.loadUrl("http://120.76.117.94/webchat");
//        webView.setWebViewClient(new WebViewClient() {
//            @Override
//            public boolean shouldOverrideUrlLoading(WebView view, String url) {
//                // TODO Auto-generated method stub
//                // 返回值是true的时候控制去WebView打开，为false调用系统浏览器或第三方浏览器
//                view.loadUrl(url);
//                return true;
//            }
//        });


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
    }

    /**
     * 全屏切换
     */
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

        // mNavigationDrawerFragment.HideShowSidebar(PreviewFullScreenStatus);//
        // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

        // PreviewTopLayout.setVisibility(View.INVISIBLE);
        //PreviewBottomlayout.setVisibility(View.INVISIBLE);

        if (PreviewFullScreenStatus) {
            //
            //PreviewBottomlayout.setVisibility(View.GONE);
            toolbar.setVisibility(View.GONE);
            //
            RelativeLayout.LayoutParams layoutParams = (RelativeLayout.LayoutParams) PreviewLayoutMain
                    .getLayoutParams();
            layoutParams.height = getWindow().getAttributes().height;
            PreviewLayoutMain.setLayoutParams(layoutParams);

        } else {
            //
           // PreviewBottomlayout.setVisibility(View.VISIBLE);
            toolbar.setVisibility(View.VISIBLE);
            //
            RelativeLayout.LayoutParams layoutParams = (RelativeLayout.LayoutParams) PreviewLayoutMain
                    .getLayoutParams();
            layoutParams.height = (int) getResources().getDimension(
                    R.dimen.preview_view_height);
            // layoutParams.bottomMargin = (int)
            // getResources().getDimension(R.dimen.preview_bottom_bar_height);
            PreviewLayoutMain.setLayoutParams(layoutParams);
            // PreviewFra.layoutPlayerBottomBar.setVisibility(View.VISIBLE);
        }

    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        // 点击了返回按键
        if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) { // 获取
            // back
            finish();
        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        // TODO Auto-generated method stub
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
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        // getMenuInflater().inflate(R.menu.platform_preview_chat, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        // if (id == R.id.action_settings) {
        // return true;
        // }

        // 点击了返回按钮
        if (item.getItemId() == android.R.id.home) {
            finish();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onRestart() {
        // TODO Auto-generated method stub
        super.onRestart();
    }

    @Override
    protected void onResume() {
        // TODO Auto-generated method stub
        OpenPreview();
        super.onResume();
    }

    @Override
    protected void onStart() {
        // TODO Auto-generated method stub
        super.onStart();
    }
    protected void onPause()
    {
        if (bPlayPreview) {
            SysApp.Ins().CapDevLogoPic(dev.plat_dev_id, true);
            //SysApp.Ins().showToast(R.string.app_ok);
        }
        super.onPause();
    }
    @Override
    protected void onStop() {
        // TODO Auto-generated method stub
        bThreadRuning = false;
        try {
            checkThread.join();
        } catch (Exception e) {
            // TODO: handle exception
        }
        ClosePreview();
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        // TODO Auto-generated method stub
        NativeCaller.openglStop();
        super.onDestroy();
    }

    Handler myHandler = new Handler() {
        public void handleMessage(Message msg) {

            int visibility = (1 == msg.what) ? View.GONE : View.VISIBLE;

            int old_v = LoadingImage.getVisibility();

            if (visibility != old_v) {
                if ((1 == msg.what)) {
                    LoadingImage.clearAnimation();
                } else {
                    LoadingImage.startAnimation(hyperspaceJumpAnimation);
                }
                Log.d("s", "Handler ------ " + visibility);
                LoadingImage.setVisibility(visibility);
            }

            super.handleMessage(msg);
        }
    };

    class CHECK_Thread implements Runnable {

        @Override
        public void run() {
            while (bThreadRuning) {

                int k = NativeCaller.GetLivestreamStatus();
                Message message = new Message();
                message.what = k;
                PreviewLiveActivity.this.myHandler.sendMessage(message);
                try {
                    Thread.sleep(300);
                } catch (InterruptedException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
        }
    }

    public static String getStringDate() {
        Date currentTime = new Date(System.currentTimeMillis());
        SimpleDateFormat format = new SimpleDateFormat("yyyyMMdd_HHmmss");
        String dateString = format.format(currentTime);
        return dateString;
    }

    @Override
    public void onClick(View v) {
        // TODO Auto-generated method stub
        switch (v.getId()) {
            // ---------------------------------------------------------------------------------------------------------
            case R.id.btnBarCapLive: {
                //
                String s = getStringDate() + ".jpg";
                SysApp.Ins().CapPic(dev.plat_dev_id, s,true);
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
            case R.id.btnBarRecLive: {
                if (bRecord) {
                    ImageBtnRecordBar
                            .setImageResource(R.drawable.zz_player_bar_record_selector);

                    Toast.makeText(SysApp.Ins().context,
                            R.string.mainfra_s_toast_info3, Toast.LENGTH_SHORT)
                            .show();
                } else {
                    ImageBtnRecordBar
                            .setImageResource(R.drawable.zz_player_bar_record_selector_sel);

                    Toast.makeText(SysApp.Ins().context,
                            R.string.mainfra_s_toast_info2, Toast.LENGTH_SHORT)
                            .show();
                }
                bRecord = !bRecord;

                SysApp.Ins().ChannelRecord(dev.plat_dev_id, bRecord ? 1 : 0,true);
            }
            break;
            // ---------------------------------------------------------------------------------------------------------
            case R.id.btnBarSoundLive: {
                if (bAudio) {

                    SysApp.Ins().stopAudio();
                    // AudioBuffer.ClearAll();
                    ImageBtnSoundBar
                            .setImageResource(R.drawable.zz_player_bar_sound_selector);
                } else {
                    // AudioBuffer.ClearAll();
                    SysApp.Ins().startAudio();
                    ImageBtnSoundBar
                            .setImageResource(R.drawable.zz_player_bar_sound_selector_sel);
                }
                bAudio = !bAudio;
            }
            break;
        }
    }

    public void OpenPreview() {
        if (!bPlayPreview) {

            // int _id = SysApp.Ins().GetDeviceItemByIndex(_which)
            // .getDBID();
            // nDBIdPreview[nIndex] = _id;

            // SysApp.Ins().GetDeviceItemByDbid(_id).isPlaying = true;
            NativeCaller.openglResume();

            // myTextView[nIndex].setText(SysApp.Ins()
            // .GetDeviceItemByIndex(_which).getDevicename());

            SysApp.Ins().startLivestream(dev);
            bPlayPreview = true;
        }
    }

    public void ClosePreview() {

        if (bPlayPreview) {

            // int _id = nDBIdPreview[nIndex];

            // SysApp.Ins().GetDeviceItemByDbid(_id).isPlaying = false;

            // myTextView[nIndex].setText(getResources().getString(
            // R.string.mainfra_s_preview_title_choice));

            // NativeCaller.openglClear(0);
            // myGlSurfaceView.invalidate();

            SysApp.Ins().CapDevLogoPic(dev.plat_dev_id,true);

            if (bRecord) {
                SysApp.Ins().ChannelRecord(dev.plat_dev_id, 0,true);
                bRecord = false;
            }

            if (bAudio) {
                SysApp.Ins().stopAudio();
                bAudio = false;
            }

            // if (bTalk) {
            // if (SysApp.Ins().MainAct.audioRecoder != null) {
            // SysApp.Ins().MainAct.audioRecoder.stopRecord();
            // SysApp.Ins().stopPPPPTalk(_id);
            // }
            //
            // bTalk = false;
            // }
            SysApp.Ins().stopLivestream();

            // nDBIdPreview[nIndex] = -1;
            NativeCaller.openglClear();
            NativeCaller.openglPause();
            myGlSurfaceView.invalidate();

            bPlayPreview = false;
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
    }
}
