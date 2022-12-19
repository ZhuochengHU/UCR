package com.fwjie.wifi1.utils;

import android.app.Activity;
import android.widget.Toast;

public class ToastUtil {

	/**
	 * 显示Toast
	 * 注:主线程和子线程都可以使用
	 */
	public static void showToast(final Activity context,final String text){
		if(Thread.currentThread().getName().equals("main")){
			Toast.makeText(context, text, Toast.LENGTH_SHORT).show();
		}else{
			context.runOnUiThread(new Runnable() {
				
				@Override
				public void run() {
					Toast.makeText(context, text, Toast.LENGTH_SHORT).show();
				}
			});
		}
	}
}
