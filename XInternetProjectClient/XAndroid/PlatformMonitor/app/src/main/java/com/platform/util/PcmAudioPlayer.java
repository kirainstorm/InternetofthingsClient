package com.platform.util;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.util.Log;

import java.util.LinkedList;
import java.util.List;

public class PcmAudioPlayer {

	public int m_sampleRateInHz;
	//public int m_inSize;
	
	public class CustomBufferHead {
		public int startcode;
		public int length;
	}

	public class CustomBufferData {
		public CustomBufferHead head;
		public byte[] data;
	}

	public class CustomBuffer {
		private List<CustomBufferData> databuffer = new LinkedList<PcmAudioPlayer.CustomBufferData>();

		public boolean addData(CustomBufferData data) {
			synchronized (this) {
				databuffer.add(data);
				return true;
			}
		}

		public CustomBufferData removeData() {
			synchronized (this) {
				if (databuffer.isEmpty()) {
					return null;
				}
				return databuffer.remove(0);
			}
		}

		public void clearAll() {
			synchronized (this) {
				databuffer.clear();
			}
		}
	}

	CustomBuffer audioBuufer = null;

	private boolean bAudioPlaying = false;
	private Thread audioThread = null;
	private AudioTrack m_AudioTrak = null;

	/**
	 * 
	 */
	public PcmAudioPlayer(int sampleRateInHz) {
		audioBuufer = new CustomBuffer();
		m_sampleRateInHz = sampleRateInHz;
		//m_inSize = inSize;
	}

	/**
	 * 
	 * @return
	 */
	public boolean isAudioPlaying() {
		return bAudioPlaying;
	}

	/**
	 * 
	 * @return
	 */
	public boolean AudioPlayStart() {
		synchronized (this) {
			if (bAudioPlaying) {
				return true;
			}
			bAudioPlaying = true;
			audioThread = new Thread(new AudioPlayThread());
			audioThread.start();
		}
		return true;
	}

	/**
	 * 
	 */
	public void AudioPlayStop() {
		synchronized (this) {
			if (!bAudioPlaying || null == audioThread) {
				return;
			}
			bAudioPlaying = false;

			try {
				audioThread.join();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			audioThread = null;
			audioBuufer.clearAll();
		}
	}

	private static final int AUDIO_BUFFER_START_CODE = 0xff00ff;

	/**
	 * 
	 * @param pcm
	 * @param len
	 */
	public void addData(byte[] pcm, int len) {
		//Log.d("TAG", "PcmAudioPlayer->addData len="+len);
		
		CustomBufferHead head = new CustomBufferHead();
		CustomBufferData datas = new CustomBufferData();
		head.length = len;
		head.startcode = AUDIO_BUFFER_START_CODE;
		datas.head = head;
		datas.data = pcm;
		audioBuufer.addData(datas);
	}

	/**
	 * 
	 * @return
	 */
	public boolean initAudioDev() {
		Log.d("TAG", "AudioTrack");
		int channelConfig;
		int audioFormat = 2;
		int mMinBufSize = 0;
		channelConfig = AudioFormat.CHANNEL_OUT_MONO;
		audioFormat = AudioFormat.ENCODING_PCM_16BIT;
		mMinBufSize = AudioTrack.getMinBufferSize(m_sampleRateInHz, channelConfig,
				audioFormat);
		Log.i("INFO", "--audio, mMinBufSize=" + mMinBufSize);//1114
		if (mMinBufSize == AudioTrack.ERROR_BAD_VALUE
				|| mMinBufSize == AudioTrack.ERROR) {
			return false;
		}

		try {
			
			m_AudioTrak = new AudioTrack(AudioManager.STREAM_MUSIC, m_sampleRateInHz,
					channelConfig, audioFormat, mMinBufSize * 2,
					AudioTrack.MODE_STREAM);

		} catch (IllegalArgumentException e) {
			e.printStackTrace();
			return false;
		}
		m_AudioTrak.play();
		return true;

	}
	


	
	class AudioPlayThread implements Runnable {

		@Override
		public void run() {
			if (!initAudioDev()) {

				return;
			}
			while (bAudioPlaying) {
				CustomBufferData dataes = audioBuufer.removeData();
				if (null == dataes) {
					try {
						Thread.sleep(1);
						continue;
					} catch (InterruptedException e) {
						e.printStackTrace();
						m_AudioTrak.stop();
						return;
					}
				}
				m_AudioTrak.write(dataes.data, 0, dataes.head.length);
			}
			Log.d("TAG", "stop/release Audio");
			m_AudioTrak.stop();
			m_AudioTrak.release();
			m_AudioTrak = null;

		}

	}
}
