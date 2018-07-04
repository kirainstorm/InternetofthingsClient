package com.platform.util;

import android.opengl.GLSurfaceView;

import com.platform.nativecaller.NativeCaller;

import javax.microedition.khronos.opengles.GL10;

public class DemoRenderer implements GLSurfaceView.Renderer {

	float _MAX_ZOOM = 2.7f;
	float _zoomcure = 1.0f;
	float _zoomcure_tmp = 1.0f;
	int _zoomcount = 2;
	int _x,_y,_width,_height;
	int _x_tmp,_y_tmp,_width_tmp,_height_tmp;


	public DemoRenderer(GLSurfaceView paramGLSurfaceView) {
		//paramGLSurfaceView.getHolder().setFormat(PixelFormat.TRANSLUCENT);
		paramGLSurfaceView.setEGLContextClientVersion(2);
	}
	@Override
	public void onDrawFrame(GL10 gl) {

		if( (_x != _x_tmp)||(_y != _y_tmp)||(_height != _height_tmp)||(_width != _width_tmp) || (_zoomcure != _zoomcure_tmp))
		{
			_x_tmp = _x;
			_y_tmp = _y;
			_width = _width_tmp;
			_height = _height_tmp;
            _zoomcure_tmp = _zoomcure;

			float m = (_width*_zoomcure);
			float n = (_height*_zoomcure);

			int k = (int)m;
			int z = (int)n;

			NativeCaller.openglViewport(_x,_y,k, z);
		}


		NativeCaller.openglStep();
	}
	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		_width = width;
		_height = height;
		_width_tmp = width;
		_height_tmp = height;

		//if (width > height)
			NativeCaller.openglViewport(0,0,width, height);
	}
	@Override
	public void onSurfaceCreated(GL10 gl,javax.microedition.khronos.egl.EGLConfig config) {
		_x = 0;
		_y = 0;
		_x_tmp = 0;
		_y_tmp = 0;
		NativeCaller.openglInit();
	}

	public void ZOOM_FANGDA(float centerx)
	{
		_zoomcure += 0.03f;
		if (_zoomcure > _MAX_ZOOM)
		{
			_zoomcure = _MAX_ZOOM;
			return;
		}

		float m = ((float)_width)*(_zoomcure-1.0f) * (centerx/(float)_width);
		_x = -(int)m;

		float n = ((float)_height)*(_zoomcure-1.0f) * (centerx/(float)_width);
		_y = -(int)n;
	}
	public void ZOOM_SUOXIAO(float centerx)
	{
		_zoomcure -= 0.03f;
		if (_zoomcure < 1.0f)
		{
			_zoomcure = 1.0f;
			_x = 0;
			_y = 0;
			return;
		}

		float m = ((float)_width)*(_zoomcure-1.0f) * (centerx/(float)_width);
		_x = -(int)m;

		float n = ((float)_height)*(_zoomcure-1.0f) * (centerx/(float)_width);
		_y = -(int)n;

	}


	//x,y为两点的差值
    //x:结束值-开始值
    //y:开始值-结束值
	public void Move(float x ,float y)
	{
		_x += (int)x;
		_y += (int)y;

		float m = (_width*_zoomcure);
		float n = (_height*_zoomcure);
		int k = (int)m;
		int z = (int)n;


		//向左
		if (_x < 0)
		{
			if (_x < (_width - k))
			{
				_x = _width - k;
			}
		}

		//向右
		if (_x > 0)
		{
			_x = 0;
		}


		//向上
		if (_y > 0)
		{
			_y = 0;
		}

		//向下
		if (_y < 0)
		{
			if (_y < (_height - z))
			{
				_y = _height - z;
			}
		}
	}
}