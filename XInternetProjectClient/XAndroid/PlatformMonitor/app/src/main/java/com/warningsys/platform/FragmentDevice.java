package com.warningsys.platform;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ListView;

import com.alertdialogpro.AlertDialogPro;
import com.baoyz.widget.PullRefreshLayout;

//http://www.jianshu.com/p/94bede7d6f46



public class FragmentDevice extends Fragment {
    public ListView mListView = null;
    public MainDeviceListAdapter mAdapter = null;
    public com.baoyz.widget.PullRefreshLayout ptrFrame = null;

    public AlertDialogDismissListener _Listener=null;

    public View view;
    public FragmentDevice() {
        super();
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //可以在Bundle对象中获取一些在Activity中传过来的数据
    }


    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {

        view = inflater.inflate(R.layout.content_main_device, container, false);
        return view;
    }

    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);


        //从这一个时候开始，就可以在Fragment中使用getActivity().findViewById



        _Listener = new AlertDialogDismissListener();
        //
        mListView = (ListView) getActivity().findViewById(R.id.mainfra_id_listview);
        mListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view,
                                    int position, long id) {
                PlayDevice(position);
            }
        });
        mListView.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
            @Override
            public boolean onItemLongClick(final AdapterView<?> arg0,
                                           final View arg1, final int pos, long arg3) {
                if(ptrFrame != null)
                {
                    ptrFrame.setEnabled(false);
                }

//                String[] items = new String[4];
//                items[0] = getResources().getString(R.string.dervice_list_munu_pop_1);
//                items[1] = getResources().getString(R.string.dervice_list_munu_pop_2);
//                items[2] = getResources().getString(R.string.dervice_list_munu_pop_3);
//                items[3] = getResources().getString(R.string.dervice_list_munu_pop_4);

                String[] items = new String[2];
                items[0] = getResources().getString(R.string.dervice_list_munu_pop_1);
                items[1] = getResources().getString(R.string.dervice_list_munu_pop_2);


                new AlertDialogPro.Builder(SysApp.Ins().context)
                        .setItems(items, new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {

                                switch (which) {
                                    case 0:
                                        DeviceLocalPic(pos);
                                        break;
                                    case 1:
                                        DeviceLocalRecord(pos);
                                        break;
                                    case 2:
                                        OnlineDownRecord(pos);
                                        break;
                                    case 3:
                                        OnlinePlayback(pos);
                                        break;
                                }
                            }
                        })
                        .setOnDismissListener(_Listener)
                        .show();
                return true;
            }
        });
        if (mAdapter == null) {
            // getDeviceList();
            mAdapter = new MainDeviceListAdapter(view.getContext());
        }
        mListView.setAdapter(mAdapter);


        //
        ptrFrame = (com.baoyz.widget.PullRefreshLayout)getActivity().findViewById(R.id.swipeRefreshLayout);
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
                        //
                        if (SysApp.Ins().GetDevList())
                        {
                            message.what = 254;
                        }
                        else
                        {
                            message.what = 255;
                        }

//                        DeviceListItem[] list = NativeCaller.GetDeviceList(SysApp.Ins().mIp,
//                                SysApp.Ins().mPort, SysApp.Ins().mUser, SysApp.Ins().mPassword);
//                        if (list != null) {
//                            for (int i = 0; i < list.length; i++) {
//                                SysApp.Ins().InsertDeviceListItem(list[i]);
//                            }
//                            message.what = 254;
//                        }
//                        else
//                        {
//                            message.what = 255;
//                        }
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

       //
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






    private class AlertDialogDismissListener implements DialogInterface.OnDismissListener {
        public void onDismiss(DialogInterface dialog)
        {
            if(ptrFrame != null)
            {
                ptrFrame.setEnabled(true);
            }
        }
    }

    Handler myHandler = new Handler() {
        public void handleMessage(Message msg) {

            mListView.setEnabled(true);//恢复点击
            switch (msg.what) {
                case 254:
                {
                    ptrFrame.setRefreshing(false);
                    SysApp.Ins().showToast(SysApp.Ins().context, R.string.app_refreshing_ok);
                    mAdapter.notifyDataSetChanged();
                }
                break;
                case 255:
                {
                    ptrFrame.setRefreshing(false);
                    SysApp.Ins().showToast(SysApp.Ins().context,R.string.app_refreshing_error);
                }
                break;
            }
            super.handleMessage(msg);
        }
    };



    public void PlayDevice(int pos) {
        Intent ints = new Intent();
        SysApp.Ins().Position = pos;
        SysApp.Ins().POSITION_FROM_LIVE =false;
        ints.setClass(SysApp.Ins().context, PreviewDeviceActivity.class);
        startActivity(ints);
    }

    public void DeviceLocalPic(int pos) {
        Intent ints = new Intent();
        SysApp.Ins().Position = pos;
        SysApp.Ins().POSITION_FROM_LIVE =false;
        ints.setClass(SysApp.Ins().context, AlbumActivity.class);
        startActivity(ints);
    }

    public void DeviceLocalRecord(int pos) {
        Intent ints = new Intent();
        SysApp.Ins().Position = pos;
        SysApp.Ins().POSITION_FROM_LIVE =false;
        SysApp.Ins().isOnlineDownMp4 = false;
        ints.setClass(SysApp.Ins().context, PlaybackLocalActivity.class);
        startActivity(ints);
    }

    public void OnlineDownRecord(int pos) {
        Intent ints = new Intent();
        SysApp.Ins().Position = pos;
        SysApp.Ins().POSITION_FROM_LIVE =false;
        SysApp.Ins().isOnlineDownMp4 = true;
        ints.setClass(SysApp.Ins().context, PlaybackLocalActivity.class);
        startActivity(ints);
    }

    public void OnlinePlayback(int pos) {
        Intent ints = new Intent();
        SysApp.Ins().Position = pos;
        SysApp.Ins().POSITION_FROM_LIVE =false;
        ints.setClass(SysApp.Ins().context, OnlinePlaybackActivity.class);
        startActivity(ints);
    }


    //    @Override
//    public void onAttach(Context context)
//    {
//
//    }
//
//
//    @Override
//    public View onCreateView(LayoutInflater inflater, ViewGroup container,
//                             Bundle savedInstanceState) {
//
//
//
//        // Inflate the layout for this fragment
//        return inflater.inflate(R.layout.content_main_device, container, false);
//    }
//
//    @Override
//    protected void onCreate(Bundle savedInstanceState) {
//        super.onCreate(savedInstanceState);
//
//        setContentView(R.layout.activity_main_device);
//
//    }
//
//
//    public FragmentDevice() {}
//
//    public FragmentDevice(Context context)
//     //add this if you need something from resources) {
//    //I don't know what you mean by icon.getIcon()
//                //bitmap =  BitmapFactory.decodeByteArray(icon.getIcon(), 0, icon.getIcon().length);
//    //iconDrawable = new BitmapDrawable(context.getResources(), bitmap);
//    }
//
//
//    @Override
//    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
//        View V = inflater.inflate(R.layout.live_game_tab1, container, false);
//        return V;
//    }
//
//    @Override //you need the onViewCreated function again I'm on my phone so I might be wrong about the variables inside so make sure to take the right code and the functions inside
//    public void onViewCreated(View v) {
//        super.onViewCreated(v);
//        //code moved to constructor so the image is initialize only ones
//
//
//       // ImageButton imgButton = (ImageButton) v.findViewById(R.id.yourButtonId);
//
//        //.setImageDrawable(iconDrawable);
//    }
//}

}
