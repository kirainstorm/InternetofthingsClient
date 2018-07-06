package com.warningsys.platform;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;

import com.alertdialogpro.AlertDialogPro;
import com.platform.nativecaller.DeviceListItem;
import com.platform.util.FileOperateUtil;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

public class PlaybackLocalActivity extends AppCompatActivity{
    private DeviceListItem dev;
    private int Position = -1;
    Toolbar toolbar;

    PlaybackLocalFileAdapter mFileAdapter;
    public ListView mListView = null;
    public boolean PreviewFullScreenStatus = false;
    public boolean MainFraDeleteStatus = false;// 设备是否处于长按后的删除状态



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_playback_local);

        Position = SysApp.Ins().Position;
        dev = SysApp.Ins().GetDeviceItemByIndex(Position);
        //
        toolbar = (Toolbar) findViewById(R.id.toolbar);
        if(SysApp.Ins().POSITION_FROM_LIVE)
        {
            toolbar.setTitle(getResources().getString(R.string.record_title_live));//标题
        }
        else
        {
            if(SysApp.Ins().isOnlineDownMp4)
            {
                toolbar.setTitle(getResources().getString(R.string.record_title_onlinedown)+": "+dev.plat_dev_name);//标题
            }
            else
            {
                toolbar.setTitle(getResources().getString(R.string.record_title_device)+": "+dev.plat_dev_name);//标题
            }

           // toolbar.setSubtitle(dev.plat_dev_name);//副标题
        }
        setSupportActionBar(toolbar);
        toolbar.setNavigationIcon(R.drawable.ic_action_back);//设置Navigation 图标
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });


        //-----------------------------------------------------------
        String thumbFolder;
        if(SysApp.Ins().POSITION_FROM_LIVE)
        {
            thumbFolder = SysApp.Ins().sdRecPath + "/Mp4Files/" + SysApp.Ins().strLiveRecord;
        }
        else
        {
            if(SysApp.Ins().isOnlineDownMp4)
            {
                thumbFolder = SysApp.Ins().sdRecPath + "/Mp4Files/"+ dev.plat_dev_id+"/Download/" ;
            }
            else
            {
                thumbFolder = SysApp.Ins().sdRecPath + "/Mp4Files/" + dev.plat_dev_id;
            }

        }



        List<File> files = FileOperateUtil.listFiles(thumbFolder, ".mp4");
        List<String> paths = new ArrayList<String>();
        if (files != null && files.size() > 0) {
            for (File file : files) {
                paths.add(file.getAbsolutePath());
            }
        }
        //--------------------------------------------------------------
        mListView = (ListView) findViewById(R.id.listView1);
        mFileAdapter = new PlaybackLocalFileAdapter(this,paths);
        mListView.setAdapter(mFileAdapter);
        mListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view,
                                    int position, long id) {

            if (MainFraDeleteStatus)
            {
                mFileAdapter.SetStatus(view,position);
            }
            else
            {
                Intent ints = new Intent();
                SysApp.Ins().LocalMp4File = mFileAdapter.getFilePathName(position);
                ints.setClass(SysApp.Ins().context, PlaybackLocalMediaActivity.class);
                startActivity(ints);
            }
            }
        });
        mListView.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
            @Override
            public boolean onItemLongClick(final AdapterView<?> arg0,
                                           final View view, final int position, long arg3) {

                //
                if (MainFraDeleteStatus) {
                    return true;// 如果已经处于编辑状态，则返回
                }
                MainFraDeleteStatus = !MainFraDeleteStatus;
                if (MainFraDeleteStatus) {
                    enterDelete();
                    mFileAdapter.SetStatus(view,position);
                }

                return true;
            }
        });
    }
    public void enterDelete() {
        MainFraDeleteStatus = true;
        mFileAdapter.clearSelect();
        mFileAdapter.bDelete = true;
        mFileAdapter.notifyDataSetChanged();
        invalidateOptionsMenu();// 重新调用onCreateOptionMenu()
    }

    public void leaveDelete() {
        MainFraDeleteStatus = false;
        mFileAdapter.bDelete = false;
        mFileAdapter.clearSelect();
        mFileAdapter.notifyDataSetChanged();
        invalidateOptionsMenu();// 重新调用onCreateOptionMenu()
    }
    private class DeleteClickedListener implements DialogInterface.OnClickListener {
        private CharSequence mShowWhenClicked;

        public DeleteClickedListener(CharSequence showWhenClicked) {
            mShowWhenClicked = showWhenClicked;
        }

        @Override
        public void onClick(DialogInterface dialog, int which) {
            Set<String> items = mFileAdapter.getSelectedItems();

            for (String path : items) {
                boolean flag = FileOperateUtil.deleteFile(path, PlaybackLocalActivity.this);
                if (!flag)
                    Log.i("TAG", path);
            }
            mFileAdapter.clearSelectedItems();
            leaveDelete();
        }
    }
    // 删除设备
    public void DeleteDevices() {
        new AlertDialogPro.Builder(this)
                .setTitle(getResources().getString(R.string.app_delete))
                .setMessage(getResources().getString(R.string.app_delete_1))
                .setNegativeButton(R.string.app_cancel, null)
                .setPositiveButton(R.string.app_ok, new DeleteClickedListener("ok"))
                .show();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        if(MainFraDeleteStatus)
            getMenuInflater().inflate(R.menu.playback_delete, menu);
        return true;
    }
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.playback_delete) {
            DeleteDevices();
            return true;
        }

        // 点击了返回按钮
        if (item.getItemId() == android.R.id.home) {
            finish();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }


    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        // 点击了返回按键
        if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) { // 获取
            // back键
            if(MainFraDeleteStatus)
            {
                MainFraDeleteStatus = !MainFraDeleteStatus;
                leaveDelete();
                return true;
            }

        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    protected void onResume() {

        super.onResume();
    }

    @Override
    protected void onPause() {
        // TODO Auto-generated method stub
        super.onPause();
    }

    @Override
    protected void onPostResume() {
        // TODO Auto-generated method stub
        super.onPostResume();
    }

    @Override
    protected void onDestroy() {
        // TODO Auto-generated method stub
        super.onDestroy();
    }

}
