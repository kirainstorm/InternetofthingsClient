package com.warningsys.platform;

import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.JavascriptInterface;
import android.webkit.WebView;
import android.webkit.WebViewClient;

//https://github.com/lzyzsd/JsBridge


public class FragmentHtml5 extends Fragment {

    public View view;
    WebView webView;
    public FragmentHtml5() {
        super();
    }


    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //可以在Bundle对象中获取一些在Activity中传过来的数据
    }


    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {

        view = inflater.inflate(R.layout.activity_main_h5, container, false);
        return view;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        Log.d("h5","*****************");


        //从这一个时候开始，就可以在Fragment中使用getActivity().findViewById
        webView = (WebView) getActivity().findViewById(R.id.webView_html5);
        (webView.getSettings()).setJavaScriptEnabled(true);
        webView.addJavascriptInterface(new WebAppInterface(getActivity()), "AndroidFunction");
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN)
            webView.getSettings().setAllowUniversalAccessFromFileURLs(true);

        //webView.loadUrl("http://live.sricam.com");
        webView.loadUrl("file:///android_asset/index.html");
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
    public void onResume() {
        super.onResume();
        //该方法为fragment从创建到显示Android系统调用的最后一个生命周期方法，调用完该方法时候，fragment就可以与用户互动了。

    }

    @Override
    public void onPause() {
        super.onPause();
        //fragment由活动状态变成非活跃执行的第一个回调方法，通常可以在这个方法中保存一些需要临时暂停的工作。例如：存音乐播放速度，然后在onResume()方法中恢复音乐播放进度。
    }

    @Override
    public void onStop() {
        super.onStop();
        //当onStop()返回的时候，fragment将从屏幕上消失。
    }

    public class WebAppInterface {
        Context mContext;

        /** Instantiate the interface and set the context */
        WebAppInterface(Context c){
            mContext = c;
        }

        /**?Show a toast from the web page */
        @JavascriptInterface
        public void showWebPreview(String _url){
            Intent ints = new Intent();
            SysApp.Ins().PreviewUrl = "file:///android_asset/preview.html?id="+_url;
            ints.setClass(SysApp.Ins().context, FragmentHtml5Activity.class);
            startActivity(ints);
        }

        @JavascriptInterface
        public void showWebAD(String _url) {
            Intent ints = new Intent();
            SysApp.Ins().PreviewUrl = _url;
            ints.setClass(SysApp.Ins().context, FragmentHtml5Activity.class);
            startActivity(ints);
        }

    }


}
