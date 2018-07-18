package com.device.nativecaller;

import android.content.Context;

public class DeviceNativeCaller {
	static {
		System.loadLibrary("xdevice");
	}

	/*
	初始化网络连接
	object：用于消息回调
	*/
	public native static int Init(Context object);

	/*
	退出，清理资源
	*/
	public native static int Free();

	/*
	开启连接
	ip：服务器IP
	uuid：该设备的设备ID
	*/
	public native static int StartService(String ip, String uuid);

	/*
	停止连接
	*/
	public native static int StopService();

	/*
	获取连接状态
	*/
	public native static int GetStatus();


	/*
	向服务器发送消息，必须时json数据，且必须满足协议格式
	*/
	public native static int SendMessage(String msg);


	/*
	向服务器发送数据，调用此函数并不表示数据真正被发到服务器上，只有服务器请求数据后才真正发送数据
	*/
	public native static int SendH264PCM(int nAVFrameType ,byte[] data, int len,
				int nVideoFrameRate, int nVideoWidth, int nVideoHeight,
			int nAudioChannels, int nAudioSamplesRate, int nAudioBitsPerSample);

	
	public native static int SendYUV(byte[] data, int len,int nVideoFrameRate, int nVideoWidth, int nVideoHeight);
	public native static int SendPCM(byte[] data, int len,int nAudioChannels, int nAudioSamplesRate, int nAudioBitsPerSample);




}