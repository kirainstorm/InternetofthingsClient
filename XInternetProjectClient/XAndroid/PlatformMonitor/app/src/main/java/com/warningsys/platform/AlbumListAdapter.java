package com.warningsys.platform;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.CheckBox;
import android.widget.ImageView;
import android.widget.TextView;

import com.nostra13.universalimageloader.core.ImageLoader;

import java.io.File;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

/**
 * Created by kiki on 2016/7/30.
 */
public class AlbumListAdapter extends BaseAdapter{


    //DisplayImageOptions options;        // DisplayImageOptions是用于设置图片显示的类


    public interface OnCheckedChangeListener{
        public void onCheckedChanged(Set<String> set);
    }

    private Context context;
    /** 加载的文件路径集合 */
    List<String> mPaths;
    /**  当前选中的文件的集合 */
    Set<String> itemSelectedSet=new HashSet<String>();
    //当前是否处于删除状态
    public boolean bDelete =false;

    // 用来控制CheckBox的选中状况
    private HashMap<Integer,Boolean> isSelected = new HashMap<Integer, Boolean>();


    public AlbumListAdapter(Context con,List<String> paths) {
        super();
        this.context = con;
        this.mPaths = paths;

        // 使用DisplayImageOptions.Builder()创建DisplayImageOptions
//        options = new DisplayImageOptions.Builder()
//                    .showStubImage(R.drawable.ic_stub)          // 设置图片下载期间显示的图片
//                    .showImageForEmptyUri(R.drawable.ic_error)  // 设置图片Uri为空或是错误的时候显示的图片
//                .showImageOnFail(R.drawable.ic_error)       // 设置图片加载或解码过程中发生错误显示的图片
//                    .cacheInMemory(true)                        // 设置下载的图片是否缓存在内存中
//                    .cacheOnDisc(true)                          // 设置下载的图片是否缓存在SD卡中
//                    .displayer(new RoundedBitmapDisplayer(20))  // 设置成圆角图片
//                .build();                                   // 创建配置过得DisplayImageOption对象

    }

    public String getFilePathName(int pos)
    {
        return mPaths.get(pos);
    }
    public void clearSelect(){
        //
        isSelected.clear();
        for(int i=0; i<mPaths.size();i++) {
            isSelected.put(i,false);
        }
    }

    public void SetStatus(View view,int position)
    {
        AlbumListAdapter.JPGViewHolder holder = (AlbumListAdapter.JPGViewHolder) view.getTag();
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

    @Override
    public int getCount() {
        return mPaths.size();
    }

    @Override
    public String getItem(int position) {
        return mPaths.get(position);
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {

        JPGViewHolder tag = null;
        if (convertView == null)
        {
            tag = new JPGViewHolder();
            //
            convertView = LayoutInflater.from(context).inflate(R.layout.content_album_item, parent, false);
            tag.imageView = (ImageView) convertView.findViewById(R.id.imgThumbnail);
            tag.text = (TextView) convertView.findViewById(R.id.textView);
            tag.checkBox = (CheckBox) convertView.findViewById(R.id.checkbox);
            //
            convertView.setTag(tag);
        } else {
            tag = (JPGViewHolder) convertView.getTag();
        }

        //tag.checkBox.setOnClickListener(this);
        File tempFile =new File( mPaths.get(position).trim());
        tag.text.setText(tempFile.getName());
        ImageLoader.getInstance().displayImage("file:///"+ mPaths.get(position), tag.imageView);

        if(bDelete)
        {
            tag.checkBox.setVisibility(View.VISIBLE);
        }
        else
        {
            tag.checkBox.setChecked(false);
            tag.checkBox.setVisibility(View.INVISIBLE);
        }
        return convertView;
    }

//    @Override
//    public void onClick(View v) {
////        if(listener!=null){
////            //这里取了上两层父类，因为真正触onClick的是FilterImageView
////            //ThumbnaiImageView view=(ThumbnaiImageView)v.getParent().getParent();
////            //listener.onItemClick(AlbumGridView.this, view, view.getPosition(), 0L);
////        }
//    }
//    @Override
//    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
//        // TODO Auto-generated method stub
//        if(buttonView.getTag()==null) return;
//        if (isChecked)
//            itemSelectedSet.add(buttonView.getTag().toString());
//        else
//            itemSelectedSet.remove(buttonView.getTag().toString());
//    }



    public class JPGViewHolder {
        ImageView imageView;//缩略图
        TextView text;
        CheckBox checkBox;//勾选框
    }

}
