package com.platform.util;

import android.util.Log;

public class LoggingControl {

	public void LogV(int control, String tag, String msg) {
		if (0 == control) {
			Log.v(tag, msg);
		}
	}

	public void LogD(int control, String tag, String msg) {
		if (0 == control) {
			Log.d(tag, msg);
		}
	}

	public void LogI(int control, String tag, String msg) {
		if (0 == control) {
			Log.i(tag, msg);
		}
	}

	public void LogW(int control, String tag, String msg) {
		if (0 == control) {
			Log.w(tag, msg);
		}
	}

	public void LogE(int control, String tag, String msg) {
		if (0 == control) {
			Log.e(tag, msg);
		}
	}
}
