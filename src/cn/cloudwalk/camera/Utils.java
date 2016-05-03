package cn.cloudwalk.camera;

import android.util.Log;

public class Utils {
	public static void LOGD(final String tag, String message) {
		// ������־�ļ����ж��Ƿ��ӡ��־
		 Log.d(tag, message);
	
		
	}
	public static void LOGD(String message) {
		// ������־�ļ����ж��Ƿ��ӡ��־
		 Log.d("MyGL", message);
	
		
	}
	public static void LOGV(final String tag, String message) {

		Log.v(tag, message);

	}



	public static void LOGI(String tag, String message) {
		Log.i(tag, message);
	}

	

	public static void LOGW(String tag, String message) {
		Log.w(tag, message);
	}

	

	public static void LOGE(String tag, String message) {
		Log.e(tag, message);
	}

	
}
