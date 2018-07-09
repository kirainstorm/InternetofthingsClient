package com.warningsys.enc264;

import java.io.File;
import java.util.List;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Intent;
import android.content.res.Configuration;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.Size;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.Window;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Toast;

import com.warningenc.nativecaller.NativeCaller;
import com.warningsys.enc264.PcmAudioRecoder.AudioRecordResult;
import com.warningsys.live.R;

public class MainActivity extends Activity implements
		android.view.SurfaceHolder.Callback, AudioRecordResult {

	WebView webView;
	PcmAudioRecoder audioRecoder = null;
	private static final String TAG = "ssss";
 
	// �������
	private SurfaceView mSurfaceview = null; // SurfaceView����(��ͼ���)��Ƶ��ʾ
	private SurfaceHolder mSurfaceHolder = null; // SurfaceHolder����(����ӿ�)SurfaceView֧����
	private Camera mCamera = null; // Camera�������Ԥ��

	boolean bIfPreview = false;
	int mPreviewWidth = 320, mPreviewHeight = 240;
	// int mPreviewWidth = 960 , mPreviewHeight= 540;
	// int mPreviewWidth = 1280 , mPreviewHeight= 720;
	int Framerate4me = 15;

	private Camera.AutoFocusCallback mAutoFocusCallBack;
	private Camera.PreviewCallback previewCallback;

	private Camera.PreviewCallback mJpegPreviewCallback;
	private Camera.AutoFocusCallback mAutoFocusCallback;

	@Override
	protected void onCreate(Bundle savedInstanceState) {

		requestWindowFeature(Window.FEATURE_NO_TITLE);

		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		// getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

		NativeCaller.InitEnc(mPreviewWidth, mPreviewHeight);
		initSurfaceView();
		File sdDir = null;
		boolean sdCardExist = Environment.getExternalStorageState().equals(
				Environment.MEDIA_MOUNTED); // �ж�sd���Ƿ����
		if (sdCardExist) {
			sdDir = Environment.getExternalStorageDirectory();// ��ȡ��Ŀ¼
		}

		audioRecoder = new PcmAudioRecoder(this, 8000, 2048);
		audioRecoder.startRecord();

		webView = (WebView) findViewById(R.id.webView1);
		(webView.getSettings()).setJavaScriptEnabled(true);
		webView.loadUrl("http://120.76.117.94/webchat");
		webView.setWebViewClient(new WebViewClient() {
			@Override
			public boolean shouldOverrideUrlLoading(WebView view, String url) {
				// TODO Auto-generated method stub
				// ����ֵ��true��ʱ�����ȥWebView�򿪣�Ϊfalse����ϵͳ�����������������
				view.loadUrl(url);
				return true;
			}
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	private long mExitTime = 0; // �˳�ʱ��

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// ����˷��ذ���
		if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) { // ��ȡ//
																				// back��

			if ((System.currentTimeMillis() - mExitTime) > 2000) {
				Toast.makeText(this, "�ٰ�һ���˳�", Toast.LENGTH_SHORT).show();
				mExitTime = System.currentTimeMillis();
			} else {
				finish();
			}
			return true;
		}
		return super.onKeyDown(keyCode, event);
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		if (audioRecoder != null)
			audioRecoder.stopRecord();

		if (mCamera != null) {
			mCamera.stopPreview();
		}

		NativeCaller.DeiniEnc();

//		int currentVersion = android.os.Build.VERSION.SDK_INT;
//		if (currentVersion > android.os.Build.VERSION_CODES.ECLAIR_MR1) {
//			Intent startMain = new Intent(Intent.ACTION_MAIN);
//			startMain.addCategory(Intent.CATEGORY_HOME);
//			startMain.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
//			startActivity(startMain);
//			System.exit(0);
//		} else {// android2.1
//			ActivityManager am = (ActivityManager) getSystemService(ACTIVITY_SERVICE);
//			am.restartPackage(getPackageName());
//		}
		 android.os.Process.killProcess(android.os.Process.myPid());//
		// Android�����˳����׹رս��̵ķ���
		super.onDestroy();
	}

	@Override
	public void finish() {
		// TODO Auto-generated method stub

		super.finish();
	}

	// InitSurfaceView
	private void initSurfaceView() {
		mSurfaceview = (SurfaceView) this.findViewById(R.id.surfaceView1);
		mSurfaceHolder = mSurfaceview.getHolder(); // ��SurfaceView��ȡ��SurfaceHolder����
		mSurfaceHolder.addCallback(MainActivity.this); // SurfaceHolder����ص��ӿ�
		// mSurfaceHolder.setFixedSize(176, 144); // Ԥ����С�O��
		mSurfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);// �O���@ʾ����ͣ�setType��������

		mAutoFocusCallBack = new Camera.AutoFocusCallback() {
			@Override
			public void onAutoFocus(boolean success, Camera camera) {
				if (success) {
					// isAutoFocus = true;
					camera.setOneShotPreviewCallback(previewCallback);
					Log.d(TAG, "onAutoFocus success");
				}
			}
		};

		// previewCallback = new Camera.PreviewCallback() {
		// @Override
		// public void onPreviewFrame(byte[] data, Camera arg1) {
		// if (data != null)
		// {
		// Camera.Parameters parameters = mCamera.getParameters();
		// int imageFormat = parameters.getPreviewFormat();
		// Log.i("map", "Image Format: " + imageFormat);
		//
		// Log.i("CameraPreviewCallback", "data length:" + data.length);
		// if (imageFormat == ImageFormat.NV21)
		// {
		// // get full picture
		// Bitmap image = null;
		// int w = parameters.getPreviewSize().width;
		// int h = parameters.getPreviewSize().height;
		//
		// Rect rect = new Rect(0, 0, w, h);
		// YuvImage img = new YuvImage(data, ImageFormat.NV21, w, h, null);
		// ByteArrayOutputStream baos = new ByteArrayOutputStream();
		// if (img.compressToJpeg(rect, 100, baos))
		// {
		// image = BitmapFactory.decodeByteArray(baos.toByteArray(), 0,
		// baos.size());
		// imageView.setImageBitmap(image);
		// }
		//
		// }
		// }
		// }
		// };

		mJpegPreviewCallback = new Camera.PreviewCallback() {
			@Override
			public void onPreviewFrame(byte[] data, Camera camera) {
				// ���ݽ�����data,Ĭ����YUV420SP��
				// TODO Auto-generated method stub
				try {
					Log.i(TAG, "SurfaceHolder.PreviewCallback  data.length = "
							+ data.length);
					NativeCaller.InputData(0, data, data.length);

				} catch (Exception e) {
					Log.v("System.out", e.toString());
				}// endtry
			}// endonPriview
		};

	}

	// ======================================================================================================
	/* ��SurfaceHolder.Callback �ص������� */
	// SurfaceView����ʱ/����ʵ������Ԥ�����汻����ʱ���÷��������á�
	public void surfaceCreated(SurfaceHolder holder) {
		// TODO Auto-generated method stub
		mCamera = Camera.open();// ��������ͷ��2.3�汾��֧�ֶ�����ͷ,�贫�������
		try {
			Log.i(TAG, "SurfaceHolder.Callback��surface Created");
			mCamera.setPreviewDisplay(mSurfaceHolder);// set the surface to be
														// used for live preview

		} catch (Exception ex) {
			if (null != mCamera) {
				mCamera.release();
				mCamera = null;
			}
			Log.i(TAG + "initCamera", ex.getMessage());
		}
	}

	// ��SurfaceView/Ԥ������ĸ�ʽ�ʹ�С�����ı�ʱ���÷���������
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
		// TODO Auto-generated method stub
		Log.i(TAG, "SurfaceHolder.Callback��Surface Changed");
		// mPreviewHeight = height;
		// mPreviewWidth = width;
		initCamera();
	}

	public void surfaceDestroyed(SurfaceHolder holder)
	// SurfaceView����ʱ���÷���������
	{
		// TODO Auto-generated method stub
		Log.i(TAG, "SurfaceHolder.Callback��Surface Destroyed");
		if (null != mCamera) {
			mCamera.setPreviewCallback(null); // �������������ǰ����Ȼ�˳�����
			mCamera.stopPreview();
			bIfPreview = false;
			mCamera.release();
			mCamera = null;
		}
	}

	// ======================================================================================================

	/* ��2�������Ԥ���� */
	private void initCamera()// surfaceChanged�е���
	{
		Log.i(TAG, "going into initCamera");
		if (bIfPreview) {
			mCamera.stopPreview();// stopCamera();
		}
		if (null != mCamera) {
			try {
				/* Camera Service settings */
				Camera.Parameters parameters = mCamera.getParameters();
				// parameters.setFlashMode("off"); // �������
				parameters.setPictureFormat(PixelFormat.JPEG); // Sets the image
																// format for
																// picture
																// �趨��Ƭ��ʽΪJPEG��Ĭ��ΪNV21
				parameters.setPreviewFormat(PixelFormat.YCbCr_420_SP); // Sets
																		// the
																		// image
																		// format
																		// for
																		// preview
																		// picture��Ĭ��ΪNV21
				/*
				 * ��ImageFormat��JPEG/NV16(YCrCb format��used for
				 * Video)/NV21(YCrCb format��used for Image)/RGB_565/YUY2/YU12
				 */

				// �����ԡ���ȡcaera֧�ֵ�PictrueSize�������ܷ����ã���
				List<Size> pictureSizes = mCamera.getParameters()
						.getSupportedPictureSizes();
				List<Size> previewSizes = mCamera.getParameters()
						.getSupportedPreviewSizes();
				List<Integer> previewFormats = mCamera.getParameters()
						.getSupportedPreviewFormats();
				List<Integer> previewFrameRates = mCamera.getParameters()
						.getSupportedPreviewFrameRates();
				Log.i(TAG + "initCamera", "cyy support parameters is ");
				Size psize = null;
				for (int i = 0; i < pictureSizes.size(); i++) {
					psize = pictureSizes.get(i);
					Log.i(TAG + "initCamera", "PictrueSize,width: "
							+ psize.width + " height" + psize.height);
				}
				for (int i = 0; i < previewSizes.size(); i++) {
					psize = previewSizes.get(i);
					Log.i(TAG + "initCamera", "PreviewSize,width: "
							+ psize.width + " height" + psize.height);
				}
				Integer pf = null;
				for (int i = 0; i < previewFormats.size(); i++) {
					pf = previewFormats.get(i);
					Log.i(TAG + "initCamera", "previewformates:" + pf);
				}

				List<int[]> range = parameters.getSupportedPreviewFpsRange();
				Log.d(TAG, "range:" + range.size());
				for (int j = 0; j < range.size(); j++) {
					int[] r = range.get(j);
					for (int k = 0; k < r.length; k++) {
						Log.d(TAG, "SupportedPreviewFpsRange" + r[k]);
					}
				}
				parameters.setPreviewFpsRange(Framerate4me * 1000,
						Framerate4me * 1000);

				// List<int[]> fpsRange = param.getSupportedPreviewFpsRange();
				//
				// if (fpsRange.size() == 1) {
				// //fpsRange.get(0)[0] < CAMERA_PREVIEW_FPS <
				// fpsRange.get(0)[1]
				// param.setPreviewFpsRange(CAMERA_PREVIEW_FPS,
				// CAMERA_PREVIEW_FPS);
				// } else {
				// //pick first from list to limit framerate or last to maximize
				// framerate
				// param.setPreviewFpsRange(fpsRange.get(0)[0],
				// fpsRange.get(0)[1]);
				// }

				parameters.setPreviewFrameRate(Framerate4me);
				// �������պ�Ԥ��ͼƬ��С
				parameters.setPictureSize(mPreviewWidth, mPreviewHeight); // ָ������ͼƬ�Ĵ�С
				parameters.setPreviewSize(mPreviewWidth, mPreviewHeight); // ָ��preview�Ĵ�С
				// ���������� ����������������õĺ���ʵ�ֻ��Ĳ�һ��ʱ���ͻᱨ��

				// ��������ͷ�Զ�����
				if (this.getResources().getConfiguration().orientation != Configuration.ORIENTATION_LANDSCAPE) {
					parameters.set("orientation", "portrait"); //
					parameters.set("rotation", 90); // ��ͷ�Ƕ�ת90�ȣ�Ĭ������ͷ�Ǻ��ģ�
					mCamera.setDisplayOrientation(90); // ��2.2���Ͽ���ʹ��
				} else// ����Ǻ���
				{
					parameters.set("orientation", "landscape"); //
					mCamera.setDisplayOrientation(0); // ��2.2���Ͽ���ʹ��
				}

				/* ��Ƶ�����봦�� */
				// ��Ӷ���Ƶ��������

				// �趨���ò���������Ԥ��
				// mCamera.setParameters(parameters); //
				// ��Camera.Parameters�趨��Camera
				// mCamera.startPreview(); // ��Ԥ������

				// mCamera.autoFocus(mAutoFocusCallback);//�Զ��۽�

				mCamera.cancelAutoFocus();
				if (parameters.getFocusMode() != Camera.Parameters.FOCUS_MODE_AUTO) {
					parameters.setFocusMode(Parameters.FOCUS_MODE_AUTO);
				}
				// if (parameters.getMaxNumFocusAreas() > 0) {
				// List<Area> mylist = new ArrayList<Area>();
				// mylist.add(new Camera.Area(focusRect, 1000));
				// parameters.setFocusAreas(mylist);
				// }

				try {
					mCamera.cancelAutoFocus();
					mCamera.setParameters(parameters);
					mCamera.startPreview();
					// mCamera.setPreviewCallbackWithBuffer(mJpegPreviewCallback);

					mCamera.setPreviewCallback(mJpegPreviewCallback);
					// mCamera.autoFocus(new Camera.AutoFocusCallback() {
					//
					// @Override
					// public void onAutoFocus(boolean arg0, Camera arg1) {
					// // TODO Auto-generated method stub
					// if (mCamera.getParameters().getFocusMode() !=
					// Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE) {
					// Parameters parameters = mCamera.getParameters();
					// parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);
					// if (parameters.getMaxNumFocusAreas() > 0) {
					// parameters.setFocusAreas(null);
					// }
					// mCamera.setParameters(parameters);
					// }
					//
					// if(arg0){
					// // mCamera.setOneShotPreviewCallback(null);
					// Toast.makeText(MainActivity.this,
					// "�Զ��۽��ɹ�" , Toast.LENGTH_SHORT).show();
					// }
					//
					// }
					//
					// });
				} catch (Exception e) {
					e.printStackTrace();
				}

				// if (parameters.getMaxNumFocusAreas() > 0) {
				// List<Area> mylist = new ArrayList<Area>();
				// mylist.add(new Camera.Area(focusRect, 1000));
				// parameters.setFocusAreas(mylist);
				// }

				// parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);
				//
				// if (parameters.getFocusMode() !=
				// Camera.Parameters.FOCUS_MODE_AUTO) {
				// parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
				// }

				bIfPreview = true;

				// �����ԡ����ú��ͼƬ��С��Ԥ����С�Լ�֡��
				Camera.Size csize = mCamera.getParameters().getPreviewSize();
				mPreviewHeight = csize.height; //
				mPreviewWidth = csize.width;
				Log.i(TAG + "initCamera", "after setting, previewSize:width: "
						+ csize.width + " height: " + csize.height);
				csize = mCamera.getParameters().getPictureSize();
				Log.i(TAG + "initCamera", "after setting, pictruesize:width: "
						+ csize.width + " height: " + csize.height);
				Log.i(TAG + "initCamera", "after setting, previewformate is "
						+ mCamera.getParameters().getPreviewFormat());
				Log.i(TAG + "initCamera", "after setting, previewframetate is "
						+ mCamera.getParameters().getPreviewFrameRate());
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	@Override
	public void AudioRecordData(byte[] data, int len) {
		// TODO Auto-generated method stub
		Log.d("AudioRecordData","ddddddddddddd AudioRecordData len="+len);
		NativeCaller.InputData(1, data, len);
	}

	// // ����ȡ��ƵԤ��֡�Ľӿڡ�
	// mJpegPreviewCallback = new Camera.PreviewCallback()
	// {
	// @Override
	// public void onPreviewFrame(byte[] data, Camera camera)
	// {
	// //���ݽ�����data,Ĭ����YUV420SP��
	// // TODO Auto-generated method stub
	// try
	// {
	// Log.i(TAG, "going into onPreviewFrame");
	// //mYUV420sp = data; // ��ȡԭ����YUV420SP����
	// YUVIMGLEN = data.length;
	//
	// // ����ԭ��yuv420sp����
	// mYuvBufferlock.acquire();
	// System.arraycopy(data, 0, mYUV420SPSendBuffer, 0, data.length);
	// //System.arraycopy(data, 0, mWrtieBuffer, 0, data.length);
	// mYuvBufferlock.release();
	//
	// // ���������̣߳��翪��PEG���뷽ʽ�߳�
	// mSendThread1.start();
	//
	// } catch (Exception e)
	// {
	// Log.v("System.out", e.toString());
	// }// endtry
	// }// endonPriview
	// };
}
