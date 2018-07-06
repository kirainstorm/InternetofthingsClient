package com.warningsys.platform;

import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.Window;
import android.view.WindowManager;

/*
Android启动页白屏或黑屏终极解决方案
http://www.zhouzhuo810.me/blog/?p=238
①添加style
    <style name="WelcomeTheme" parent="AppTheme">
        <item name="android:windowBackground">@drawable/welcome_page</item>
        <item name="android:windowFullscreen">true</item>
    </style>

②设置style
在AndroidManifest.xml文件中给启动Activity设置style
android:theme=”@style/WelcomeTheme”
③启动Activity不要调用setContentView。<<==========
ok。
附录：
延时跳转示例代码：
        new AsyncTask<Void, Void, Void>() {
            @Override
            protected Void doInBackground(Void... params) {
                try {
                    Thread.sleep(2000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                return null;
            }

            @Override
            protected void onPostExecute(Void aVoid) {
                super.onPostExecute(aVoid);
                Intent intent = new Intent(WelcomeActivity.this, LoginActivity.class);
                startActivity(intent);
                finish();
            }
        }.execute();
*/

public class StartupActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getSupportActionBar().hide();
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        //setContentView(R.layout.activity_start_splash_screen);
//        new Handler().postDelayed(new Runnable() {
//            public void run() {
//                Intent intent = new Intent();
//                intent.setClass(StartupActivity.this, LoginPlatActivity.class);
//                startActivity(intent);
//                //实现淡入浅出的效果
//                overridePendingTransition(android.R.anim.fade_in,android.R.anim.fade_out);
//                StartupActivity.this.finish();
//            }
//        }, 1500);

        new AsyncTask<Void, Void, Void>() {
            @Override
            protected Void doInBackground(Void... params) {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                return null;
            }

            @Override
            protected void onPostExecute(Void aVoid) {
                super.onPostExecute(aVoid);
                Intent intent = new Intent(StartupActivity.this, LoginPlatActivity.class);
                startActivity(intent);
                finish();
            }
        }.execute();
    }
}
