package com.sricamlive.platform;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.TextView;

import com.baoyz.widget.PullRefreshLayout;
import com.bigkoo.pickerview.TimePickerView;
import com.platform.nativecaller.DeviceListItem;
import com.platform.nativecaller.HislogListItem;
import com.platform.nativecaller.NativeCaller;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;

public class OnlinePlaybackActivity extends AppCompatActivity {

    private DeviceListItem dev;
    private int Position = -1;
    Toolbar toolbar;
    public ListView mListView = null;

    TimePickerView pvTime;
    TextView tvTime;
    public com.baoyz.widget.PullRefreshLayout ptrFrame = null;
    Date dateSearch;


    OnlinePlaybackFileAdapter mHisAdeapter;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_online_playback);

        //
        Position = SysApp.Ins().Position;
        dev = SysApp.Ins().GetDeviceItemByIndex(Position);

        //
        toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        toolbar.setTitle(getResources().getString(R.string.app_online_his));//标题
        toolbar.setSubtitle(dev.plat_dev_name);//副标题
        setSupportActionBar(toolbar);
        toolbar.setNavigationIcon(R.drawable.ic_action_back);//设置Navigation 图标
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });


//        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
//        fab.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
//                        .setAction("Action", null).show();
//            }
//        });

        tvTime=(TextView) findViewById(R.id.tvTime);

        //时间选择器
        pvTime = new TimePickerView(this, TimePickerView.Type.YEAR_MONTH_DAY);
        //控制时间范围
        Calendar calendar = Calendar.getInstance();
        pvTime.setRange(calendar.get(Calendar.YEAR) - 1, calendar.get(Calendar.YEAR));//要在setTime 之前才有效果哦
        pvTime.setTime(new Date());
        pvTime.setCyclic(false);
        pvTime.setCancelable(true);

        //时间选择后回调
        pvTime.setOnTimeSelectListener(new TimePickerView.OnTimeSelectListener() {

            @Override
            public void onTimeSelect(Date date) {
                tvTime.setText( getResources().getString(R.string.app_select_date) + getTime(date));
                dateSearch = date;
                //开始搜索

            }
        });




        dateSearch = new Date();
        tvTime.setText( getResources().getString(R.string.app_select_date) + getTime(dateSearch));
        mListView = (ListView) findViewById(R.id.listView);


        //
        ptrFrame = (com.baoyz.widget.PullRefreshLayout)findViewById(R.id.swipeRefreshLayout);
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
                        Calendar calendar=Calendar.getInstance();
                        calendar.setTime(dateSearch);

                        Message message = new Message();
                        // ------------------------------------------------------
                        //
                        HislogListItem[] list = NativeCaller.GetHisLogList(dev.plat_dev_playback_ip,dev.plat_dev_playback_port, SysApp.Ins().mUser, SysApp.Ins().mPassword,
                                dev.plat_dev_id,calendar.get(Calendar.YEAR),calendar.get(Calendar.MONTH)+1,calendar.get(Calendar.DAY_OF_MONTH),0,0);
                        if (list != null)
                        {
                            ArrayList<HislogListItem> mList = new ArrayList<>();
                            for (int i = 0; i < list.length; i++) {
                                mList.add(list[i]);
                            }
                            mHisAdeapter= new OnlinePlaybackFileAdapter(OnlinePlaybackActivity.this,mList);
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
        mListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view,
                                    int position, long id) {
                OnlinePlayback(position);
            }
        });

//        mListView.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
//            @Override
//            public boolean onItemLongClick(final AdapterView<?> arg0,
//                                           final View arg1, final int pos, long arg3) {
//                if(ptrFrame != null)
//                {
//                    ptrFrame.setEnabled(false);
//                }
//
//                String[] items = new String[2];
//                items[0] = getResources().getString(R.string.online_list_munu_pop_1);
//                items[1] = getResources().getString(R.string.online_list_munu_pop_2);
//                new AlertDialogPro.Builder(OnlinePlaybackActivity.this)
//                        //.setTitle(mHisAdeapter.mList.get(pos).his_file.substring(mHisAdeapter.mList.get(pos).his_file.lastIndexOf("\\")+16))
//                        .setItems(items, new DialogInterface.OnClickListener() {
//                            @Override
//                            public void onClick(DialogInterface dialog, int which) {
//
//                                switch (which) {
//                                    case 0:
//                                        OnlinePlayback(pos);
//                                        break;
//                                    case 1:
//                                        DownloadMp4(pos);
//                                        break;
//                                }
//                            }
//                        })
//                        .setOnDismissListener(new AlertDialogDismissListener())
//                        .show();
//                return true;
//            }
//        });


        //弹出时间选择器
        tvTime.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                pvTime.show();
            }
        });
    }
    public static String getTime(Date date) {

        SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
        return format.format(date);
    }


    Handler myHandler = new Handler() {
        public void handleMessage(Message msg) {

            mListView.setEnabled(true);//恢复点击
            switch (msg.what) {
                case 254:
                {
                    ptrFrame.setRefreshing(false);
                    SysApp.Ins().showToast(OnlinePlaybackActivity.this,R.string.app_refreshing_ok);
                    mListView.setAdapter(mHisAdeapter);
                    mHisAdeapter.notifyDataSetChanged();
                }
                break;
                case 255:
                {
                    ptrFrame.setRefreshing(false);
                    SysApp.Ins().showToast(OnlinePlaybackActivity.this,R.string.app_refreshing_error);
                }
                break;
            }
            super.handleMessage(msg);
        }
    };


    private class AlertDialogDismissListener implements DialogInterface.OnDismissListener {
        public void onDismiss(DialogInterface dialog)
        {
            if(ptrFrame != null)
            {
                ptrFrame.setEnabled(true);
            }
        }
    }
    public void OnlinePlayback(int pos) {
        Intent ints = new Intent();
        SysApp.Ins().HislogItem = mHisAdeapter.mList.get(pos);
        ints.setClass(this, PreviewOnlineActivity.class);
        startActivity(ints);
    }

    public void DownloadMp4(int pos) {
//        Intent ints = new Intent();
//        SysApp.Ins().Position = pos;
//        SysApp.Ins().isOnlineDownMp4 = false;
//        ints.setClass(SysApp.Ins().context, PlaybackLocalActivity.class);
//        startActivity(ints);
    }
}
