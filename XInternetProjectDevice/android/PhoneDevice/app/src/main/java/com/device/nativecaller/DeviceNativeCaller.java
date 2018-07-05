package com.device.nativecaller;

import android.content.Context;

public class DeviceNativeCaller {
	static {
		System.loadLibrary("xdevice");
	}

	public native static int Init(Context object_video);
	public native static int Free();

	public native static int StartService(String ip, String uuid);
	public native static int StopService();
	public native static int GetStatus();
	public native static int SendMessage(String msg);
	public native static int SendH264PCM(int nAVFrameType ,byte[] data, int len,
				int nVideoFrameRate, int nVideoWidth, int nVideoHeight,
			int nAudioChannels, int nAudioSamplesRate, int nAudioBitsPerSample);

	public native static int SendYUV(byte[] data, int len,int nVideoFrameRate, int nVideoWidth, int nVideoHeight);
	public native static int SendPCM(byte[] data, int len,int nAudioChannels, int nAudioSamplesRate, int nAudioBitsPerSample);




}