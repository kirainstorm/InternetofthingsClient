package com.warningsys.platform;

import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.os.Build;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.Toolbar;
import android.view.MenuItem;
import android.view.View;
import android.view.WindowManager;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.FrameLayout;

//http://www.jianshu.com/p/2dd60e6bd798

public class FragmentHtml5Activity extends AppCompatActivity {

    WebView webView;
    private InsideWebChromeClient mInsideWebChromeClient;
    private FrameLayout mFrameLayout;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_fragment_html5);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        //toolbar.setTitle(getResources().getString(R.string.app_about));//标题
        toolbar.setTitle("返回");
        setSupportActionBar(toolbar);
        //toolbar.setLogo(R.drawable.pause_1);
        toolbar.setNavigationIcon(R.drawable.ic_action_back);//设置Navigation 图标

        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });

        mFrameLayout = (FrameLayout) findViewById(R.id.mFrameLayout);
        webView = (WebView) findViewById(R.id.webView_html5_preview);
        (webView.getSettings()).setJavaScriptEnabled(true);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN)
            webView.getSettings().setAllowUniversalAccessFromFileURLs(true);

        WebSettings settings = webView.getSettings();
        settings.setJavaScriptEnabled(true);
        settings.setJavaScriptCanOpenWindowsAutomatically(true);
        settings.setPluginState(WebSettings.PluginState.ON);
        //settings.setPluginsEnabled(true);
        settings.setAllowFileAccess(true);
        settings.setLoadWithOverviewMode(true);
        settings.setUseWideViewPort(true);
        settings.setCacheMode(WebSettings.LOAD_NO_CACHE);
        settings.setCacheMode(WebSettings.LOAD_DEFAULT);
        mInsideWebChromeClient = new InsideWebChromeClient();
        InsideWebViewClient mInsideWebViewClient = new InsideWebViewClient();
        //javascriptInterface = new JavascriptInterface();
        // mWebView.addJavascriptInterface(javascriptInterface, "java2js_laole918");
        webView.setWebChromeClient(mInsideWebChromeClient);
        webView.setWebViewClient(mInsideWebViewClient);



        webView.loadUrl(SysApp.Ins().PreviewUrl);
        webView.setWebViewClient(new WebViewClient() {

            @Override
            public boolean shouldOverrideUrlLoading(WebView view, String url) {
                // TODO Auto-generated method stub
                // 返回值是true的时候控制去WebView打开，为false调用系统浏览器或第三方浏览器


                view.loadUrl(url);
                return true;
            }
        });
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        // 点击了返回按钮
        if (item.getItemId() == android.R.id.home) {
            finish();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onDestroy() {
        webView.removeAllViews();
        webView.destroy();
        super.onDestroy();
    }

    @Override
    public void onPause() {
        super.onPause();
        webView.onPause();
    }

    @Override
    public void onResume() {
        super.onResume();
        webView.onResume();
    }

//    @Override
//    public void onBackPressed() {
//        if (webView.canGoBack()) {
//            webView.goBack();
//            return;
//        }
//        super.onBackPressed();
//    }

//    @Override
//    public void onDestroy() {
//        webView.destroy();
//        super.onDestroy();
//    }


    @Override
    public void onConfigurationChanged(Configuration config) {
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        super.onConfigurationChanged(config);
        switch (config.orientation) {
            case Configuration.ORIENTATION_LANDSCAPE:
                toolbar.setVisibility(View.GONE);
                getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
                getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
                break;
            case Configuration.ORIENTATION_PORTRAIT:
                toolbar.setVisibility(View.VISIBLE);
                getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
                getWindow().addFlags(WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
                break;
        }
    }


    private class InsideWebChromeClient extends WebChromeClient {
        private View mCustomView;
        private CustomViewCallback mCustomViewCallback;

        @Override
        public void onShowCustomView(View view, CustomViewCallback callback) {
            super.onShowCustomView(view, callback);
            if (mCustomView != null) {
                callback.onCustomViewHidden();
                return;
            }
            mCustomView = view;
            mFrameLayout.addView(mCustomView);
            mCustomViewCallback = callback;
            webView.setVisibility(View.GONE);
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        }

        public void onHideCustomView() {
            webView.setVisibility(View.VISIBLE);
            if (mCustomView == null) {
                return;
            }
            mCustomView.setVisibility(View.GONE);
            mFrameLayout.removeView(mCustomView);
            mCustomViewCallback.onCustomViewHidden();
            mCustomView = null;
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
            super.onHideCustomView();
        }
    }


    private class InsideWebViewClient extends WebViewClient {
        @Override
        public boolean shouldOverrideUrlLoading(WebView view, String url) {
            return true;
        }

        @Override
        public void onPageFinished(WebView view, String url) {
            super.onPageFinished(view, url); //mWebView.loadUrl(javascript); }

        }
    }
}