package com.fwjie.wifi1.activities;

import com.fwjie.wifi1.R;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.widget.RelativeLayout;


public class SplashActivity extends Activity {
	
	private RelativeLayout rl_splash_root;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_splash);
		
		loadMain();
		//initView();
		//initData();
		//intAnimation();
		
	}
	

	/**
	 * 加载主界面
	 */
	private void loadMain() {
		Intent intent = new Intent(SplashActivity.this, MainActivity.class);
		startActivity(intent);
		finish();				//结束当前Activity
	}
	
	
	/**
	 * 初始化界面的动化效果
	 */
	private void intAnimation() {
		AlphaAnimation aa = new AlphaAnimation(0, 1);	//动化的透明效果
		aa.setDuration(2000);	//动画的持续时间
		aa.setFillAfter(true);
		
		aa.setAnimationListener(new AnimationListener() {
			
			@Override
			public void onAnimationStart(Animation animation) {
				// TODO 自动生成的方法存根
				
			}
			
			@Override
			public void onAnimationRepeat(Animation animation) {
				
			}
			
			/**
			 * 动画执行完毕后加载主界面
			 */
			@Override
			public void onAnimationEnd(Animation animation) {
				loadMain();
				
			}
		});
		
		//给splash界面设置动画
		rl_splash_root.startAnimation(aa);
		
	}

	private void initData() {
		
	}

	private void initView() {
		rl_splash_root = (RelativeLayout) findViewById(R.id.rl_splash_root);
		
	}

}
