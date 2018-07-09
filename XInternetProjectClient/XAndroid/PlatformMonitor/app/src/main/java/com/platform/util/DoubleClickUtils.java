package com.platform.util;


public class DoubleClickUtils {
	private static long lastClickTime = 0;

	public static boolean isNotDoubleClick() {
		long time = System.currentTimeMillis();
		long diff = time - lastClickTime;
		lastClickTime = time;
		if (diff > 0 && diff < 300) {
			lastClickTime = 0;// ��ֹ3��
			return false;
		}

		return true;
	}
}
