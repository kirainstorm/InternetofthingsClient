package com.sricamlive.platform;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.nostra13.universalimageloader.core.ImageLoader;
import com.platform.nativecaller.DeviceListItem;

import java.io.File;

/**
 * Created by sun on 2016/8/1.
 */
public class MainLiveListAdapter extends BaseAdapter {

    private Context context;

    public MainLiveListAdapter(Context con) {
        this.context = con;
    }

    @Override
    public int getCount() {
//        Log.i("INFO",
//                "===============DeviceAdapter=================getCount():"
//                        + SysApp.Ins().GetLiveDeviceCount() + "");
        return SysApp.Ins().GetLiveDeviceCount();
    }

    @Override
    public Object getItem(int position) {
        return SysApp.Ins().GetLiveDeviceItemByIndex(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }




    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
//        Log.i("INFO", "===============DeviceAdapter=================getView():"
//                + position + "");
        DeviceListItem devItem = SysApp.Ins().GetLiveDeviceItemByIndex(position);// new
        // DeviceListItem();



        LiveDeviceTag tag = null;
        if (null != devItem && !"".equals(devItem)) {
            if (null == convertView) {
                tag = new LiveDeviceTag();

                convertView = LayoutInflater.from(context).inflate(
                        R.layout.activity_main_device_list, parent, false);

                tag.pic = (ImageView) convertView
                        .findViewById(R.id.device_list_l1);
                tag.piconline = (ImageView) convertView
                        .findViewById(R.id.imageView_device_online);
                tag.name = (TextView) convertView
                        .findViewById(R.id.device_list_name);
                tag.id = (TextView) convertView
                        .findViewById(R.id.device_list_id);
                tag.onlinenum = (TextView) convertView
                        .findViewById(R.id.device_list_onlinenum);

                convertView.setTag(tag);
            } else {
                tag = (LiveDeviceTag) convertView.getTag();
            }

            String imgPath= SysApp.Ins().picPath +  "Logo/" + SysApp.Ins().strLivePic + "/" + devItem.plat_dev_id  + "/logo.jpg";
            File file = new File(imgPath);
            if (file.exists())
            {
                ImageLoader.getInstance().displayImage("file:///"+ imgPath, tag.pic);
            }


            tag.name.setText(devItem.plat_dev_name);
            tag.id.setText(SysApp.Ins().MainAct.getResources().getString(R.string.dervice_list_info2) + devItem.plat_dev_id);
            tag.onlinenum.setText(SysApp.Ins().MainAct.getResources().getString(R.string.dervice_list_info3) + devItem.plat_dev_browsenum);
            if(1==devItem.plat_dev_online)
                tag.piconline.setImageResource(( R.drawable.zz_online));
            else
                tag.piconline.setImageResource((R.drawable.zz_offline));
        }


        return convertView;
    }

    public class LiveDeviceTag {
        //
        ImageView pic;
        //
        ImageView piconline;
        TextView name;
        TextView id;
        TextView onlinenum;
    }

}
