package com.platform.util;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.util.Log;

public class PcmAudioRecoder {
	private AudioRecordResult audioResult = null;
	private Thread recordThread = null;
	private boolean bRecordThreadRuning = false;
	private int m_in_buf_size = 0;
	private AudioRecord m_in_rec = null;
	private byte[] m_in_bytes = null;

	
	public int m_sampleRateInHz;
	public int m_outSize;
	
	/**
	 * 
	 * @param result
	 */
	public PcmAudioRecoder(AudioRecordResult result,int sampleRateInHz,int outSize) {
		m_outSize = outSize;
		m_sampleRateInHz = sampleRateInHz;
		audioResult = result;
		//initRecorder();
	}

	/**
	 * 
	 * @return
	 */
	public boolean initRecorder() {
		m_in_buf_size = AudioRecord.getMinBufferSize(m_sampleRateInHz,
				AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT);//ENCODING_PCM_16BIT
		Log.d("PcmAudioRecoder", "m_in_buf_size: " + m_in_buf_size);
		
		m_in_buf_size = m_outSize;
		
		try {
			m_in_rec = new AudioRecord(MediaRecorder.AudioSource.MIC, m_sampleRateInHz,
					AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT,
					m_in_buf_size);//ENCODING_PCM_16BIT
		} catch (Exception e) {
			// TODO: handle exception
			Log.d("PcmAudioRecoder()", "init audio Recorder error");
		}
		

		if (null == m_in_rec) {
			Log.d("PcmAudioRecoder()", "init audio Recorder error");
			return false;
		}
		m_in_bytes = new byte[m_in_buf_size];
		return true;
	}
	/**
	 * 
	 */
	public void releaseRecord() {
		Log.d("tag", "releaseRecord");
		m_in_rec.release();
		m_in_rec = null;
	}
	/**
	 * 
	 */
	public void startRecord() {
		synchronized (this) {
			if (!initRecorder())
				return;
			
			
			
			Log.d("PcmAudioRecoder()", "startRecord");
			if (bRecordThreadRuning) {
				Log.d("PcmAudioRecoder", "is startRecord");
				return;
			}
			Log.d("PcmAudioRecoder", "begin startRecord");
			bRecordThreadRuning = true;
			recordThread = new Thread(new RecordThread());
			recordThread.start();
		}
	}

	/**
	 * 
	 */
	public void stopRecord() {
		synchronized (this) {
			if (!bRecordThreadRuning) {
				return;
			}
			bRecordThreadRuning = false;
			try {
				recordThread.join();
				Log.d("PcmAudioRecoder", "stopRecord()");
			} catch (Exception e) {
				// TODO: handle exception
			}
			releaseRecord();
		}
	}




	class RecordThread implements Runnable {

		@Override
		public void run() {
			m_in_rec.startRecording();
			while (bRecordThreadRuning) {
				int nRet = m_in_rec.read(m_in_bytes, 0, m_in_buf_size);
				Log.d("PcmAudioRecoder", "bAudioRecording runing.... nRet: "
						+ nRet);
				if (nRet == 0) {
					return;
				}
				//Log.d("PcmAudioRecoder", "read audio");
				if (audioResult != null) {
					audioResult.AudioRecordData(m_in_bytes, nRet);
				}
			}
			m_in_rec.stop();

		}

	}


	public interface AudioRecordResult {
		abstract public void AudioRecordData(byte[] data, int len);
	}
}
