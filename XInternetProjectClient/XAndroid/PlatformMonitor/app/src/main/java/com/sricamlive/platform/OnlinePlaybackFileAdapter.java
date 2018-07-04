package com.sricamlive.platform;

import android.content.Context;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.platform.nativecaller.HislogListItem;

import java.util.ArrayList;

/**
 * Created by sun on 2016/8/3.
 */
public class OnlinePlaybackFileAdapter extends BaseAdapter {

    Context context;
    ArrayList<HislogListItem> mList = new ArrayList<>();

    public OnlinePlaybackFileAdapter(Context con, ArrayList<HislogListItem> list) {
        super();
        this.context = con;
        this.mList = list;
    }

    @Override
    public int getCount() {
        // TODO Auto-generated method stub
        return mList.size();
    }

    @Override
    public Object getItem(int arg0) {
        // TODO Auto-generated method stub
        return mList.get(arg0);
    }

    @Override
    public long getItemId(int arg0) {
        // TODO Auto-generated method stub
        return arg0;
    }

    @Override
    public View getView(int pos, View convertView, ViewGroup parent) {
        // TODO Auto-generated method stub
        DataHisFileHolder d = null;


        if (null == convertView)
        {
            d = new DataHisFileHolder();
            convertView = LayoutInflater.from(context).inflate(R.layout.content_online_playback_item, parent, false);
            d.name = (TextView) convertView.findViewById(R.id.his_list_name);
            d.date = (TextView) convertView.findViewById(R.id.his_list_date);
            d.img = (ImageView) convertView.findViewById(R.id.his_list_img);
            convertView.setTag(d);
        } else {
            d = (DataHisFileHolder) convertView.getTag();
        }

        Log.d("aa",mList.get(pos).his_file.substring(mList.get(pos).his_file.lastIndexOf("\\")+1));
        d.name.setText(mList.get(pos).his_file.substring(mList.get(pos).his_file.lastIndexOf("\\")+16));

        long a = mList.get(pos).his_time_start_hour*3600+mList.get(pos).his_time_start_min*60 + mList.get(pos).his_time_start_sec;
        long b = mList.get(pos).his_time_end_hour*3600+mList.get(pos).his_time_end_min*60 + mList.get(pos).his_time_end_sec;
        //Log.d("TAG", "getView: "+b +" "+a);
        String strM = String.format("%02d:%02d:%02d",(b-a)/3600, ((b-a)-((b-a)/3600*3600))/60,((b-a)-((b-a)/3600*3600))%60);
        String strinf = mList.get(pos).his_time_start_year +"/"+mList.get(pos).his_time_start_mon +"/"+mList.get(pos).his_time_start_day +"  "+
                mList.get(pos).his_time_start_hour +":"+mList.get(pos).his_time_start_min +":"+mList.get(pos).his_time_start_sec+"   "
                + strM;
        d.date.setText(strinf);

        return convertView;
    }

    public class DataHisFileHolder {
        TextView name;
        TextView date;
        ImageView img;
    }
}
