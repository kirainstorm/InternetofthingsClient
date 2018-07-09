package com.warningsys.platform;

import android.content.Context;
import android.graphics.Bitmap;
import android.media.ThumbnailUtils;
import android.provider.MediaStore;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.CheckBox;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.File;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

/**
 * Created by kiki on 2016/7/30.
 */
public class PlaybackLocalFileAdapter extends BaseAdapter {
    private Context context;
    List<String> mPaths;
    /**  当前选中的文件的集合 */
    Set<String> itemSelectedSet=new HashSet<String>();
    // 用来控制CheckBox的选中状况
    public HashMap<Integer,Boolean> isSelected = new HashMap<Integer, Boolean>();
    //当前是否处于删除状态
    public boolean bDelete =false;




    public PlaybackLocalFileAdapter(Context con,List<String> paths) {
        super();
        this.context = con;
        this.mPaths = paths;


        isSelected = new HashMap<Integer, Boolean>();
        clearSelect();
    }



    public String getFilePathName(int pos)
    {
        return mPaths.get(pos);
    }
    public void clearSelect(){
        isSelected.clear();
        for(int i=0; i<mPaths.size();i++) {
            isSelected.put(i,false);
        }
    }
    public void SetStatus(View view,int position)
    {
        DataRecordFileHolder holder = (DataRecordFileHolder) view.getTag();
        holder.checkBox.toggle();
        isSelected.put(position,holder.checkBox.isChecked());
        if (holder.checkBox.isChecked())
            itemSelectedSet.add(mPaths.get(position));
        else
            itemSelectedSet.remove(mPaths.get(position));
    }
    public Set<String> getSelectedItems(){
        return itemSelectedSet;
    }
    public void clearSelectedItems(){
        //删除已经删除的
        for(int i=0; i<isSelected.size();i++) {
            if (isSelected.get(i))
            {
                mPaths.remove(i);
            }
        }
        //清空选择的file list
        itemSelectedSet.clear();
    }




//    public void SelectAll()
//    {
//        for (int i = 0; i < mPaths.size(); i++) {
//            isSelected.put(i, true);
//        }
//    }
//
//
//    public void DeleteItems()
//    {
//        for (int i = mPaths.size() -1; i >= 0 ; i--) {
//            if(isSelected.get(i))
//            {
//                boolean flag = false;
//
//                File file = new File(mPaths.get(i));
//                if (file.exists()) { // 文件不存在直接返回
//                    flag = file.delete();
//                }
//
//                mPaths.remove(i);
//            }
//        }
//        initDate();
//    }



    @Override
    public int getCount() {
        // TODO Auto-generated method stub
        return mPaths.size();
    }

    @Override
    public Object getItem(int arg0) {
        // TODO Auto-generated method stub
        return mPaths.get(arg0);
    }

    @Override
    public long getItemId(int arg0) {
        // TODO Auto-generated method stub
        return arg0;
    }

    @Override
    public View getView(int pos, View convertView, ViewGroup parent) {
        // TODO Auto-generated method stub
        DataRecordFileHolder d = null;


        if (null == convertView)
        {
            d = new DataRecordFileHolder();
            convertView = LayoutInflater.from(context).inflate(R.layout.content_playback_local_item, parent, false);
            d.name = (TextView) convertView.findViewById(R.id.fra_sdrec_list_name);
            d.date = (TextView) convertView.findViewById(R.id.fra_sdrec_list_date);
            d.img = (ImageView) convertView.findViewById(R.id.fra_sdrec_list_img);
            d.checkBox = (CheckBox) convertView.findViewById(R.id.fra_sdrec_list_item_cb);
            convertView.setTag(d);
        } else {
            d = (DataRecordFileHolder) convertView.getTag();
        }
        if(bDelete)
        {
            d.checkBox.setVisibility(View.VISIBLE);
        }
        else
        {
            d.checkBox.setVisibility(View.INVISIBLE);
        }
        File tempFile =new File( mPaths.get(pos).trim());
        d.name.setText(tempFile.getName());

        String sTmp = tempFile.getName();
        d.date.setText(sTmp.substring(0,4)+"/"+sTmp.substring(4,6)+"/"+sTmp.substring(6,8) +"  " +sTmp.substring(8,10)+":"+sTmp.substring(10,12)+":"+sTmp.substring(12,14));

        d.checkBox.setChecked(isSelected.get(pos));
        d.img.setImageBitmap(getVideoThumbnail(mPaths.get(pos),64,48, MediaStore.Images.Thumbnails.MINI_KIND));
        return convertView;
    }

    public class DataRecordFileHolder {
        TextView name;
        TextView date;
        ImageView img;
        CheckBox checkBox;
    }

    /**
     * 获取视频的缩略图
     * 先通过ThumbnailUtils来创建一个视频的缩略图，然后再利用ThumbnailUtils来生成指定大小的缩略图。
     * 如果想要的缩略图的宽和高都小于MICRO_KIND，则类型要使用MICRO_KIND作为kind的值，这样会节省内存。
     * @param videoPath 视频的路径
     * @param width 指定输出视频缩略图的宽度
     * @param height 指定输出视频缩略图的高度度
     * @param kind 参照MediaStore.Images.Thumbnails类中的常量MINI_KIND和MICRO_KIND。
     *            其中，MINI_KIND: 512 x 384，MICRO_KIND: 96 x 96
     * @return 指定大小的视频缩略图
     */
    private Bitmap getVideoThumbnail(String videoPath, int width, int height, int kind) {
        Bitmap bitmap = null;
        // 获取视频的缩略图
        bitmap = ThumbnailUtils.createVideoThumbnail(videoPath, kind);
        //System.out.println("w"+bitmap.getWidth());
        //System.out.println("h"+bitmap.getHeight());
        bitmap = ThumbnailUtils.extractThumbnail(bitmap, width, height,
                ThumbnailUtils.OPTIONS_RECYCLE_INPUT);
        return bitmap;
    }

}
