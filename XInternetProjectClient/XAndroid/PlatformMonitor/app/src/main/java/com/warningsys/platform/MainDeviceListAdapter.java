package com.warningsys.platform;

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

public class MainDeviceListAdapter extends BaseAdapter {
	private Context context;

	public MainDeviceListAdapter(Context con) {
		this.context = con;
	}

	@Override
	public int getCount() {
//		Log.i("INFO",
//				"===============DeviceAdapter=================getCount():"
//						+ SysApp.Ins().GetUserDeviceCount() + "");
		return SysApp.Ins().GetUserDeviceCount();
	}

	@Override
	public Object getItem(int position) {
		return SysApp.Ins().GetUserDeviceItemByIndex(position);
	}

	@Override
	public long getItemId(int position) {
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
//		Log.i("INFO", "===============DeviceAdapter=================getView():"
//				+ position + "");
		DeviceListItem devItem = SysApp.Ins().GetUserDeviceItemByIndex(position);// new
																				// DeviceListItem();

		MainDeviceTag tag = null;
		if (null != devItem && !"".equals(devItem)) {
			if (null == convertView) {
				tag = new MainDeviceTag();

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
				tag = (MainDeviceTag) convertView.getTag();
			}

			String imgPath= SysApp.Ins().picPath +  "Logo/"  + devItem.plat_dev_id + "/logo.jpg";
			File file = new File(imgPath);
			if (file.exists())
			{
				ImageLoader.getInstance().displayImage("file:///"+ imgPath, tag.pic);
			}
            else
            {
                tag.pic.setImageResource(R.drawable.zz_maincell);
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

	public class MainDeviceTag {
		//
		ImageView pic;
		//
		ImageView piconline;
		TextView name;
		TextView id;
		TextView onlinenum;
	}

}
