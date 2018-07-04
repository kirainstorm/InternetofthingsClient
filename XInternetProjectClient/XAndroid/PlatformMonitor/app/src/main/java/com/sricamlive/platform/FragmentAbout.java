package com.sricamlive.platform;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;


public class FragmentAbout extends Fragment {


    public View view;
    public FragmentAbout() {
        super();
    }


    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //可以在Bundle对象中获取一些在Activity中传过来的数据
    }


    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {

        view = inflater.inflate(R.layout.content_about, container, false);
        return view;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
//从这一个时候开始，就可以在Fragment中使用getActivity().findViewById

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


}
