package com.platform.nativecaller;

/**
 * Created by kiki on 2016/7/28.
 */
public class DeviceListItem {


    // ------------------------------------------------------------------------------------
    public int plat_dev_id = 0; // 设备在数据库的id
    public int plat_dev_ispublish = 0;// 0:私有设备 1：publish device
    public int plat_dev_browsenum = 0;// 浏览人数
    public int plat_dev_online = 0;// 是否在线

    public String plat_dev_name = "";// 设备名称

    public int plat_dev_areaid = 0;// 区域id
    public String plat_dev_area_name = "";// 区域名称

    public String plat_dev_media_ip = "";// 实时流
    public int plat_dev_media_port = 0;

    public String plat_dev_playback_ip = "";// 历史流
    public int plat_dev_playback_port = 0;

    public String plat_dev_publish_ip = "";// 分发流
    public int plat_dev_publish_port = 0;


    // ------------------------------------------------------------------------------------

}

