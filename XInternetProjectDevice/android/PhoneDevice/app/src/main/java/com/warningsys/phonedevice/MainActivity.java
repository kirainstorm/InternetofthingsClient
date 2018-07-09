package com.warningsys.phonedevice;

import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.media.MediaCodec;
import android.media.MediaFormat;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.Toast;

import com.device.nativecaller.DeviceNativeCaller;
import com.hw.EncoderDebugger;
import com.hw.NV21Convertor;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.Iterator;
import java.util.List;

public class MainActivity extends AppCompatActivity implements
        android.view.SurfaceHolder.Callback, PcmAudioRecoder.AudioRecordResult {
    int width = 640, height = 480;
    int framerate, bitrate;
    int mCameraId = Camera.CameraInfo.CAMERA_FACING_FRONT;
    MediaCodec mMediaCodec;
    SurfaceView surfaceView;
    SurfaceHolder surfaceHolder;
    Camera mCamera;
    NV21Convertor mConvertor;
    PcmAudioRecoder audioRecoder = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

//        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
//        fab.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
//                        .setAction("Action", null).show();
//            }
//        });


        //
        MyApplication.Ins().ApplicationInit();
        //DeviceNativeCaller.StartService("47.96.249.142","3001");

        DeviceNativeCaller.StartService("192.168.1.209","122");

        initMediaCodec();
        //
        audioRecoder = new PcmAudioRecoder(this, 8000, 2048);
        audioRecoder.startRecord();
        //
        surfaceView = (SurfaceView) findViewById(R.id.surfaceView1);
        surfaceView.getHolder().addCallback(this);
        surfaceView.getHolder().setFixedSize(getResources().getDisplayMetrics().widthPixels,
                getResources().getDisplayMetrics().heightPixels);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    private long mExitTime = 0; // 退出时间
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        // 点击了返回按键
        if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) { // 获取//
            // back键

            if ((System.currentTimeMillis() - mExitTime) > 2000) {
                Toast.makeText(this, "再按一次退出", Toast.LENGTH_SHORT).show();
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

        if (audioRecoder != null)
            audioRecoder.stopRecord();
        destroyCamera();
        mMediaCodec.stop();
        mMediaCodec.release();
        mMediaCodec = null;
        //
        DeviceNativeCaller.StopService();
        MyApplication.Ins().ApplicationDeinit();
        //
        android.os.Process.killProcess(android.os.Process.myPid());//
        // Android程序退出彻底关闭进程的方法
        super.onDestroy();
    }




    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
    private void initMediaCodec() {
        int dgree = getDgree();
        framerate = 15;
        bitrate = 512*1024;
        EncoderDebugger debugger = EncoderDebugger.debug(getApplicationContext(), width, height);
        mConvertor = debugger.getNV21Convertor();
        try {
            mMediaCodec = MediaCodec.createByCodecName(debugger.getEncoderName());
            MediaFormat mediaFormat;
            if (dgree == 0) {
                mediaFormat = MediaFormat.createVideoFormat("video/avc", height, width);
            } else {
                mediaFormat = MediaFormat.createVideoFormat("video/avc", width, height);
            }
            mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, bitrate);
            mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, framerate);
            mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT,
                    debugger.getEncoderColorFormat());
            mediaFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 1);
            mMediaCodec.configure(mediaFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
            mMediaCodec.start();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static int[] determineMaximumSupportedFramerate(Camera.Parameters parameters) {
        int[] maxFps = new int[]{0, 0};
        List<int[]> supportedFpsRanges = parameters.getSupportedPreviewFpsRange();
        for (Iterator<int[]> it = supportedFpsRanges.iterator(); it.hasNext(); ) {
            int[] interval = it.next();
            if (interval[1] > maxFps[1] || (interval[0] > maxFps[0] && interval[1] == maxFps[1])) {
                maxFps = interval;
            }
        }
        return maxFps;
    }


    private boolean ctreateCamera(SurfaceHolder surfaceHolder) {
        try {

            mCamera = Camera.open(mCameraId);
            //
            List<Camera.Size> pictureSizes = mCamera.getParameters().getSupportedPictureSizes();
            List<Camera.Size> previewSizes = mCamera.getParameters().getSupportedPreviewSizes();
            List<Integer> previewFormats = mCamera.getParameters().getSupportedPreviewFormats();
            List<Integer> previewFrameRates = mCamera.getParameters().getSupportedPreviewFrameRates();
            List<int[]> range = mCamera.getParameters().getSupportedPreviewFpsRange();
            Log.i("initCamera", "cyy support parameters is ");
            for (int i = 0; i < pictureSizes.size(); i++) {
                Camera.Size _size = pictureSizes.get(i);
                Log.i("initCamera", "PictrueSize,width: " + _size.width + " height" + _size.height);
            }
            for (int i = 0; i < previewSizes.size(); i++) {
                Camera.Size _size = previewSizes.get(i);
                Log.i("initCamera", "PreviewSize,width: " + _size.width + " height" + _size.height);
            }
            for (int i = 0; i < previewFormats.size(); i++) {
                Integer pf = previewFormats.get(i);
                Log.i("initCamera", "previewformates:" + pf);
            }
            for (int i = 0; i < previewFrameRates.size(); i++) {
                Integer pf = previewFrameRates.get(i);
                Log.i( "initCamera", "previewFrameRates:" + pf);
            }
            for (int j = 0; j < range.size(); j++) {
                int[] r = range.get(j);
                for (int k = 0; k < r.length; k++) {
                    Log.d("initCamera", "SupportedPreviewFpsRange" + r[k]);
                }
            }




            //
            Camera.Parameters parameters = mCamera.getParameters();
            int[] max = determineMaximumSupportedFramerate(parameters);
            Camera.CameraInfo camInfo = new Camera.CameraInfo();
            Camera.getCameraInfo(mCameraId, camInfo);
            int cameraRotationOffset = camInfo.orientation;
            int rotate = (360 + cameraRotationOffset - getDgree()) % 360;
            parameters.setRotation(rotate);
            parameters.setPreviewFormat(ImageFormat.NV21);
            List<Camera.Size> sizes = parameters.getSupportedPreviewSizes();
            parameters.setPreviewSize(width, height);
            parameters.setPreviewFpsRange(max[0], max[1]);
            //parameters.setPreviewFpsRange(framerate*1000, framerate*1000);
            mCamera.setParameters(parameters);
            mCamera.autoFocus(null);
            int displayRotation;
            displayRotation = (cameraRotationOffset - getDgree() + 360) % 360;
            mCamera.setDisplayOrientation(displayRotation);
            mCamera.setPreviewDisplay(surfaceHolder);
            return true;
        } catch (Exception e) {
//            StringWriter sw = new StringWriter();
//            PrintWriter pw = new PrintWriter(sw);
//            e.printStackTrace(pw);
//            String stack = sw.toString();
//            Toast.makeText(this, stack, Toast.LENGTH_LONG).show();
            destroyCamera();
            e.printStackTrace();
            return false;
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        surfaceHolder = holder;
        ctreateCamera(surfaceHolder);
        startPreview();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        stopPreview();
        destroyCamera();
    }
    int _timestamp = 0;

    long _____tick = 0;
    int _____count = 0;

    Camera.PreviewCallback previewCallback = new Camera.PreviewCallback() {
        byte[] mPpsSps = new byte[0];

        @Override
        public void onPreviewFrame(byte[] data, Camera camera) {
            if (data == null) {
                return;
            }

            if (_____tick == 0)
            {
                _____tick = System.currentTimeMillis();
            }
            _____count++;

            if ((System.currentTimeMillis() - _____tick) > 1000)
            {
                _____tick = System.currentTimeMillis();

                Log.d("@@","fps = "+ _____count);
                _____count= 0;
            }


            ByteBuffer[] inputBuffers = mMediaCodec.getInputBuffers();
            ByteBuffer[] outputBuffers = mMediaCodec.getOutputBuffers();
            byte[] dst = new byte[data.length];
            Camera.Size previewSize = mCamera.getParameters().getPreviewSize();
            if (getDgree() == 0) {
                dst = rotateNV21Degree90(data, previewSize.width, previewSize.height);
            } else {
                dst = data;
            }
            try {
                int bufferIndex = mMediaCodec.dequeueInputBuffer(5000000);
                if (bufferIndex >= 0) {
                    inputBuffers[bufferIndex].clear();
                    mConvertor.convert(dst, inputBuffers[bufferIndex]);
                    mMediaCodec.queueInputBuffer(bufferIndex, 0,
                            inputBuffers[bufferIndex].position(),
                            System.nanoTime() / 1000, 0);
                    MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
                    int outputBufferIndex = mMediaCodec.dequeueOutputBuffer(bufferInfo, 0);
                    while (outputBufferIndex >= 0) {
                        ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];
                        byte[] outData = new byte[bufferInfo.size];
                        outputBuffer.get(outData);

                        Log.d("-----", "=================" + outData[4]+"######"+outData.length);
                        int isframe = 0;
                        //记录pps和sps
                        if (outData[0] == 0 && outData[1] == 0 && outData[2] == 0 && outData[3] == 1 && outData[4] == 39) {
                            mPpsSps = outData;
                        }
                        else if (outData[0] == 0 && outData[1] == 0 && outData[2] == 0 && outData[3] == 1 && outData[4] == 37) {
                            //在关键帧前面加上pps和sps数据
                            isframe = 1;
                            byte[] iframeData = new byte[mPpsSps.length + outData.length];
                            System.arraycopy(mPpsSps, 0, iframeData, 0, mPpsSps.length);
                            System.arraycopy(outData, 0, iframeData, mPpsSps.length, outData.length);
                            outData = iframeData;
                        }

                        if(isframe == 1)
                            DeviceNativeCaller.SendH264PCM(0,outData,outData.length,framerate,width,height,1,8000,16);
                        else
                            DeviceNativeCaller.SendH264PCM(1,outData,outData.length,framerate,width,height,1,8000,16);

                        //if(null != _my51cplatformsdk)
                        {
                            String pathhh = "";
                            //long timestamp = System.currentTimeMillis();

//                            _timestamp += (1000/framerate);
//                            _my51cplatformsdk.nativePlatformSendAVData(1,1,outData,outData.length,isframe,
//                                    1,framerate,(int)_timestamp,6,0,pathhh);
                        }

                        mMediaCodec.releaseOutputBuffer(outputBufferIndex, false);
                        outputBufferIndex = mMediaCodec.dequeueOutputBuffer(bufferInfo, 0);
                    }
                } else {
                    Log.e("@@", "No buffer available !");
                }
            } catch (Exception e) {
//                StringWriter sw = new StringWriter();
//                PrintWriter pw = new PrintWriter(sw);
//                e.printStackTrace(pw);
//                String stack = sw.toString();
                //Log.e("save_log", stack);
                e.printStackTrace();
            } finally {
                mCamera.addCallbackBuffer(dst);
            }
        }

    };

    /**
     * 开启预览
     */
    public synchronized void startPreview() {
        if (mCamera != null) {
            mCamera.startPreview();
            int previewFormat = mCamera.getParameters().getPreviewFormat();
            Camera.Size previewSize = mCamera.getParameters().getPreviewSize();
            int size = previewSize.width * previewSize.height
                    * ImageFormat.getBitsPerPixel(previewFormat)
                    / 8;
            mCamera.addCallbackBuffer(new byte[size]);
            mCamera.setPreviewCallbackWithBuffer(previewCallback);
            //started = true;
            //btnSwitch.setText("停止");
        }
    }

    /**
     * 停止预览
     */
    public synchronized void stopPreview() {
        if (mCamera != null) {
            mCamera.stopPreview();
            mCamera.setPreviewCallbackWithBuffer(null);
            //started = false;
            // btnSwitch.setText("开始");
        }
    }

    /**
     * 销毁Camera
     */
    protected synchronized void destroyCamera() {
        if (mCamera != null) {
            mCamera.stopPreview();
            try {
                mCamera.release();
            } catch (Exception e) {

            }
            mCamera = null;
        }
    }

    private int getDgree() {
        int rotation = getWindowManager().getDefaultDisplay().getRotation();
        int degrees = 0;
        switch (rotation) {
            case Surface.ROTATION_0:
                degrees = 0;
                break; // Natural orientation
            case Surface.ROTATION_90:
                degrees = 90;
                break; // Landscape left
            case Surface.ROTATION_180:
                degrees = 180;
                break;// Upside down
            case Surface.ROTATION_270:
                degrees = 270;
                break;// Landscape right
        }
        return degrees;
    }

//    @Override
//    public void onClick(View v) {
//        switch (v.getId()) {
//            case R.id.btn_switch:
//                if (!started) {
//                    startPreview();
//                } else {
//                    stopPreview();
//                }
//                break;
//        }
//    }


    //=========================================================================================
    /**
     * 将YUV420SP数据顺时针旋转90度
     *
     * @param data        要旋转的数据
     * @param imageWidth  要旋转的图片宽度
     * @param imageHeight 要旋转的图片高度
     * @return 旋转后的数据
     */
    public static byte[] rotateNV21Degree90(byte[] data, int imageWidth, int imageHeight) {
        byte[] yuv = new byte[imageWidth * imageHeight * 3 / 2];
        // Rotate the Y luma
        int i = 0;
        for (int x = 0; x < imageWidth; x++) {
            for (int y = imageHeight - 1; y >= 0; y--) {
                yuv[i] = data[y * imageWidth + x];
                i++;
            }
        }
        // Rotate the U and V color components
        i = imageWidth * imageHeight * 3 / 2 - 1;
        for (int x = imageWidth - 1; x > 0; x = x - 2) {
            for (int y = 0; y < imageHeight / 2; y++) {
                yuv[i] = data[(imageWidth * imageHeight) + (y * imageWidth) + x];
                i--;
                yuv[i] = data[(imageWidth * imageHeight) + (y * imageWidth) + (x - 1)];
                i--;
            }
        }
        return yuv;
    }

    @Override
    public void AudioRecordData(byte[] data, int len) {
        DeviceNativeCaller.SendH264PCM(12,data,len,framerate,width,height,1,8000,16);
    }
}
