package com.sricamlive.platform;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ListView;

import com.baoyz.widget.PullRefreshLayout;

//https://github.com/lzyzsd/JsBridge


public class FragmentLive extends Fragment {

    public View view;
    public ListView mListView = null;
    public MainLiveListAdapter mAdapter = null;
    public com.baoyz.widget.PullRefreshLayout ptrFrame;

    public FragmentLive() {
        super();
    }


    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //可以在Bundle对象中获取一些在Activity中传过来的数据
    }


    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {

        view = inflater.inflate(R.layout.content_main_live, container, false);
        return view;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        Log.d("h5","*****************");


        //从这一个时候开始，就可以在Fragment中使用getActivity().findViewById

        //
        mListView = (ListView) getActivity().findViewById(R.id.live_id_listview);
        mListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view,
                                    int position, long id) {
                PlayLive(position);
            }
        });
        if (mAdapter == null) {
            // getDeviceList();
            mAdapter = new MainLiveListAdapter(getActivity());
        }
        mListView.setAdapter(mAdapter);

        //
        ptrFrame = (com.baoyz.widget.PullRefreshLayout)getActivity().findViewById(R.id.swipeRefreshLayout_live);
        ptrFrame.setRefreshStyle(PullRefreshLayout.STYLE_MATERIAL);
        // listen refresh event
        ptrFrame.setOnRefreshListener(new PullRefreshLayout.OnRefreshListener() {
            @Override
            public void onRefresh() {
                mListView.setEnabled(false);//禁止点击
                // start refresh
                new Thread( new Runnable() {
                    @Override
                    public void run() {

                        Message message = new Message();
                        // ------------------------------------------------------
                        //
                        if (SysApp.Ins().GetDevList())
                        {
                            message.what = 254;
                        }
                        else
                        {
                            message.what = 255;
                        }
                        // ------------------------------------------------------
                        myHandler.sendMessage(message);
                    }
                }).start();

            }
        });



    }

    @Override
    public void onResume() {
        super.onResume();
        //该方法为fragment从创建到显示Android系统调用的最后一个生命周期方法，调用完该方法时候，fragment就可以与用户互动了。
        SysApp.Ins().POSITION_FROM_LIVE =true;
        if (mAdapter != null)
        {
            mAdapter.notifyDataSetChanged();
            //SysApp.Ins().showToast(R.string.app_refresh_log);
        }
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


    Handler myHandler = new Handler() {
        public void handleMessage(Message msg) {

            mListView.setEnabled(true);//恢复点击
            switch (msg.what) {
                case 254:
                {
                    ptrFrame.setRefreshing(false);
                    SysApp.Ins().showToast(getActivity(),R.string.app_refreshing_ok);
                    mAdapter.notifyDataSetChanged();
                }
                break;
                case 255:
                {
                    ptrFrame.setRefreshing(false);
                    SysApp.Ins().showToast(getActivity(),R.string.app_refreshing_error);
                }
                break;
            }
            super.handleMessage(msg);
        }
    };

    public void PlayLive(int pos) {
        Intent ints = new Intent();
        SysApp.Ins().Position = pos;
        ints.setClass(SysApp.Ins().context, PreviewLiveActivity.class);
        startActivity(ints);
    }
}
