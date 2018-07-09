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
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemLongClickListener;
import android.widget.GridView;

import com.alertdialogpro.AlertDialogPro;
import com.platform.nativecaller.DeviceListItem;
import com.platform.util.FileOperateUtil;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

public class AlbumActivity extends AppCompatActivity implements AlbumListAdapter.OnCheckedChangeListener{

    @Override
    public void onCheckedChanged(Set<String> set)
    {

    }

    private GridView mAlbumView;
    private AlbumListAdapter mAlbumViewAdapter;
    public boolean mAlbumEditable = false;

    private DeviceListItem dev;
    private int Position = -1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_album);
        //
        Position = SysApp.Ins().Position;
        dev = SysApp.Ins().GetDeviceItemByIndex(Position);
        //
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        if(SysApp.Ins().POSITION_FROM_LIVE)
        {
            toolbar.setTitle(getResources().getString(R.string.pic_title_line));//标题
        }
        else
        {
            toolbar.setTitle(getResources().getString(R.string.pic_title_device)+": "+dev.plat_dev_name);//标题
            //toolbar.setSubtitle(dev.plat_dev_name);//副标题
        }
        setSupportActionBar(toolbar);
        toolbar.setNavigationIcon(R.drawable.ic_action_back);//设置Navigation 图标
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });
        //

        mAlbumView = (GridView) findViewById(R.id.albumview);
        loadAlbum(".jpg");

        mAlbumView.setOnItemClickListener(new OnItemClickListener() {

            @Override
            public void onItemClick(AdapterView<?> arg0, View view, int position,
                                    long arg3) {
                if (mAlbumEditable)
                {
                    mAlbumViewAdapter.SetStatus(view,position);
                }
                else
                {
                    Intent intent = new Intent(AlbumActivity.this, AlbumViewActivity.class);
                    intent.putExtra("file", mAlbumViewAdapter.getFilePathName(position));
                    intent.putExtra("Position", Position);
                    startActivity(intent);
                }
            }
        });
        mAlbumView.setOnItemLongClickListener(new OnItemLongClickListener() {

            @Override
            public boolean onItemLongClick(AdapterView<?> parent, View view,
                                           int position, long id) {

                if (mAlbumEditable)
                    return true;

                enterDelete();
                mAlbumViewAdapter.SetStatus(view,position);
                return true;
            }
        });


    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        if(mAlbumEditable)
            getMenuInflater().inflate(R.menu.albumaty_delete, menu);
        return true;
    }

    @Override
    public void onBackPressed() {
        if (mAlbumEditable) {
            leaveDelete();
            return;
        }
        super.onBackPressed();
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        //
        int id = item.getItemId();
        if (id == R.id.albumaty_delete) {
            showDeleteDialog();
            return true;
        }
        // 点击了返回按钮
        if (item.getItemId() == android.R.id.home) {
            finish();
            return true;
        }
        //
        return super.onOptionsItemSelected(item);
    }
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        // 点击了返回按键
        if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) {
            if(mAlbumEditable)
            {
                leaveDelete();
                return true;
            }

        }
        return super.onKeyDown(keyCode, event);
    }
    public void loadAlbum(String format) {
        // 获取根目录下缩略图文件夹

        String thumbFolder;
        if(SysApp.Ins().POSITION_FROM_LIVE)
        {
            thumbFolder = SysApp.Ins().picPath + SysApp.Ins().strLivePic;
        }
        else
        {
            thumbFolder = SysApp.Ins().picPath + dev.plat_dev_id;
        }

        List<File> files = FileOperateUtil.listFiles(thumbFolder, format);
        List<String> paths = new ArrayList<String>();
        if (files != null && files.size() > 0) {
            for (File file : files) {
                paths.add(file.getAbsolutePath());
            }
        }
        mAlbumViewAdapter = new AlbumListAdapter(this,paths);
        mAlbumView.setAdapter(mAlbumViewAdapter);
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    private void enterDelete() {
        mAlbumEditable = true;
        invalidateOptionsMenu();// 重新调用onCreateOptionMenu()
        mAlbumViewAdapter.clearSelect();
        mAlbumViewAdapter.bDelete = true;
        mAlbumViewAdapter.notifyDataSetChanged();
    }

    private void leaveDelete() {
        mAlbumEditable = false;
        this.invalidateOptionsMenu();// 重新调用onCreateOptionMenu()
        mAlbumViewAdapter.clearSelect();
        mAlbumViewAdapter.bDelete = false;
        mAlbumViewAdapter.notifyDataSetChanged();

    }

    private class DeleteClickedListener implements DialogInterface.OnClickListener {
        private CharSequence mShowWhenClicked;

        public DeleteClickedListener(CharSequence showWhenClicked) {
            mShowWhenClicked = showWhenClicked;
        }

        @Override
        public void onClick(DialogInterface dialog, int which) {
            Set<String> items = mAlbumViewAdapter.getSelectedItems();

            for (String path : items) {
                boolean flag = FileOperateUtil.deleteFile(
                        path, AlbumActivity.this);
                if (!flag)
                    Log.i("TAG", path);
            }
            mAlbumViewAdapter.clearSelectedItems();
            leaveDelete();
        }
    }
    private void showDeleteDialog() {
        new AlertDialogPro.Builder(this)
                .setTitle(getResources().getString(R.string.app_delete))
                .setMessage(getResources().getString(R.string.app_delete_1))
                .setNegativeButton(R.string.app_cancel, null)
                .setPositiveButton(R.string.app_ok, new DeleteClickedListener("ok"))
                .show();
    }

}
