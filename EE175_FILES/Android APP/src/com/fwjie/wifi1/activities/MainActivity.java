package com.fwjie.wifi1.activities;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.net.URLEncoder;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

import com.fwjie.wifi1.R;
import com.fwjie.wifi1.utils.IOUtil;
import com.fwjie.wifi1.utils.MyConstant;
import com.fwjie.wifi1.utils.ToastUtil;

import android.app.Activity;
import android.app.TimePickerDialog;
import android.graphics.Color;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.StrictMode;
import android.text.format.Time;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.TimePicker;

/**
 * 主界面
 * @author Administrator
 *
 */
public class MainActivity extends Activity {
	
	private boolean alarmFlag = false;
	
	private TextView tvMainTitle;

	private Button btn_connect;
	
	private EditText et_ip;
	private EditText et_port;
	

	

	private ImageView ivStatus1,ivStatus2;
	
	private MediaPlayer mMediaPlayer;
	
	private Button   but1,but2;

	

	
	private String   ip;
	private int      port;
	
	private boolean isConnect = false;
	
	private Socket socket = null;
    private BufferedWriter wr ;
    private BufferedReader br ; 
    
    private Thread socketThread = null;
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.activity_main);
		
		initView();
		initData();
		initEvent();
		
		//解决NetworkOnMainThreadException异常
		StrictMode.setThreadPolicy(new StrictMode.ThreadPolicy.Builder()  
	        .detectDiskReads().detectDiskWrites().detectNetwork()  
	        .penaltyLog().build());  
		StrictMode.setVmPolicy(new StrictMode.VmPolicy.Builder()  
	        .detectLeakedSqlLiteObjects().detectLeakedClosableObjects()  
	        .penaltyLog().penaltyDeath().build()); 
	}

	public Handler myHandler = new Handler() {  

		@Override  
        public void handleMessage(Message msg) {  
        	switch (msg.what) {
				case MyConstant.CONNECT_SUCCESS:
					btn_connect.setText("Break");
					isConnect = true;
					ToastUtil.showToast(MainActivity.this, "Connect success");
					break;
				case MyConstant.CONNECT_TIMEOUT:
					ToastUtil.showToast(MainActivity.this, "ERROR");
					break;
				case MyConstant.CONNECT_ERROR:
					ToastUtil.showToast(MainActivity.this, "ERROR");
					btn_connect.setText("Connect");
					isConnect = false;
					break;
				 case MyConstant.DATETIME_UPDATE:
					 break;
				case MyConstant.CONNECT_READ:
					//ToastUtil.showToast(MainActivity.this, (String)msg.obj);
					String line = (String)msg.obj;
					System.out.println(line);
					String[] strs = line.split(",");
					

					if (strs[0].equals("AA") && strs.length == 3) {
						
						String prefix                = strs[0];
						String Str1                  = strs[1];
						String Str2                  = strs[2];

						if(Str1.equals("0"))
						{
							if (Str2.equals("1")) {
		                		ivStatus1.setImageResource(R.drawable.light_on);
		                	} 
							else
							{
		                		ivStatus1.setImageResource(R.drawable.light_off);
		                	}
						}
						if(Str1.equals("1"))
						{
							if (Str2.equals("1")) {
		                		ivStatus2.setImageResource(R.drawable.light_on);
		                	} 
							else
							{
		                		ivStatus2.setImageResource(R.drawable.light_off);
		                	}
						}
					} 
					

					break;
			}
        }  
	  
	};  
	

	/**
	 * 子线程进行Socket通信
	 * @author Administrator
	 *
	 */
	class MyThread extends Thread {  
		
		Message msg;
         	
        @Override  
        public void run() {  
        	initData();
        	
        	//连接服务器 并设置连接超时为5秒  
            socket = new Socket();
        	try {
				socket.connect(new InetSocketAddress(ip, port), 5000);
			} catch (SocketTimeoutException aa) {  
				msg = myHandler.obtainMessage();
                msg.what = MyConstant.CONNECT_TIMEOUT;
                msg.obj  = "Connect ERROR";	               
                myHandler.sendMessage(msg); 
                socketThread = null;
                return;
            } catch (IOException e1) {
				e1.printStackTrace();
				msg = myHandler.obtainMessage();
                msg.what = MyConstant.CONNECT_ERROR;
                msg.obj  = "Connect ERROR" + e1.toString();               
                myHandler.sendMessage(msg); 
                socketThread = null;
                return;
			} 
           
            try {
				wr = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
				br = new BufferedReader(new InputStreamReader(socket.getInputStream()));  
			} catch (IOException e1) {
				e1.printStackTrace();
				msg = myHandler.obtainMessage();
                msg.what = MyConstant.CONNECT_ERROR;
                msg.obj  = "Connect ERROR"+ e1.toString();       	               
                myHandler.sendMessage(msg); 
                socketThread = null;
                return;
			}
            
            msg = myHandler.obtainMessage();
            msg.what = MyConstant.CONNECT_SUCCESS;
            msg.obj  = "Connect Success";	               
            myHandler.sendMessage(msg); 
            isConnect = true;
            
            while(isConnect && !socket.isClosed()){
            	try {
            		if (socket.isInputShutdown()) {
            			//ToastUtil.showToast(MainActivity.this, "网络接收异常");
    	                isConnect = false;
    	                IOUtil.close(socket);
    	                socketThread = null;
    	                
    	                msg = myHandler.obtainMessage();
    	                msg.what = MyConstant.CONNECT_ERROR;
    	                msg.obj  = "Disconnected";               
    	                myHandler.sendMessage(msg); 
            		}
	                String line = br.readLine();    //读取发来服务器信息  
	                if (line != null && line.length() > 0)
	                {		       
	                	System.out.println(line);
		                msg = myHandler.obtainMessage();
		                msg.what = MyConstant.CONNECT_READ;
		                msg.obj  = line;	               
		                myHandler.sendMessage(msg);
	                }
	            } catch (IOException e) {  
	                //ToastUtil.showToast(MainActivity.this, "网络接收异常");
	                isConnect = false;
	                IOUtil.close(socket);
	                socketThread = null;
	                
	                msg = myHandler.obtainMessage();
	                msg.what = MyConstant.CONNECT_ERROR;
	                msg.obj  = "Disconnected";               
	                myHandler.sendMessage(msg); 
	                //tvMainTitle.setText("未连接");
	            }  
            }
            //IOUtil.close(socket);
            //socketThread = null;
        }  
    }  
	
	public void write(String str) {
		if (wr != null && !socket.isOutputShutdown()) {
			try {
				wr.write(str);
				//wr.newLine();
				wr.flush();
			} catch (IOException e) {
				e.printStackTrace();
				ToastUtil.showToast(MainActivity.this, "ERROR！");
			}
		}
	}
	
	Button.OnClickListener listener = new Button.OnClickListener() {//创建监听对象    
		public void onClick(View v){  
        	if(!isConnect) {
        		ToastUtil.showToast(MainActivity.this, "ERROR！");
        		return;
        	}
        	
        	switch (v.getId()) {
        	case R.id.BUT1:
        		write("1\n");
        		break;
        	case R.id.BUT2:
        		write("2\n");
        		break;
        	}
		} 
    };

    private void TcpServerDisconnect() {
		isConnect = false;
		try {
			if (socketThread != null && socketThread.isAlive()) {
				ToastUtil.showToast(MainActivity.this, "Disconnecting...");
				socketThread = null;
			} else if (socket != null && !socket.isClosed() && !socket.isConnected()) {
				ToastUtil.showToast(MainActivity.this, "Disconnecting...");
				IOUtil.close(socket);
			} else {
				ToastUtil.showToast(MainActivity.this, "Disconnected");
			}
		} catch (Exception e){
			
		}
		btn_connect.setText("Connect");
	}
    
    private void TcpServerConnecting() {
		ToastUtil.showToast(MainActivity.this, "On Connection...");
		try {
			if (socketThread != null && socketThread.isAlive()) {
				isConnect = false;
				socketThread = null;
			}
			if (socket != null && !socket.isClosed() && !socket.isConnected()) {
				IOUtil.close(socket);
			}
		} catch (Exception e){
			
		}
		socketThread = new MyThread();
		socketThread.start();
		isConnect = true;
	}

	    
	private void initEvent() {
		
		but1.setOnClickListener(listener);	
		but2.setOnClickListener(listener);	

		

		
		//创建TCP连接
		btn_connect.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				if(isConnect == false) {			
					TcpServerConnecting();
				} else {
					TcpServerDisconnect();
				}
			}
		});
	
	}
	
	private void initData() {
		ip     = et_ip.getText().toString();
		port   = Integer.parseInt(et_port.getText().toString());
		
	}

	private void initView() {
		
		
		et_ip       = (EditText) findViewById(R.id.et_ip);
		et_port     = (EditText) findViewById(R.id.et_port);
		btn_connect = (Button) findViewById(R.id.btn__connect);
		

		
		ivStatus1  	= (ImageView) findViewById(R.id.ivStatus1);
		ivStatus2  	= (ImageView) findViewById(R.id.ivStatus2);

		but1= (Button) findViewById(R.id.BUT1);
		but2= (Button) findViewById(R.id.BUT2);

		
	}
	private void startAlarm() {
		if (mMediaPlayer != null) {
			return;
		}
        mMediaPlayer = MediaPlayer.create(this, R.raw.beep);
        mMediaPlayer.setLooping(true);
        try {
            mMediaPlayer.prepare();
        } catch (IllegalStateException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        mMediaPlayer.start();
	}
		    
	private void stopAlarm() {
	    if (mMediaPlayer == null) {
	    	return;
	    }
	    if (mMediaPlayer.isLooping() || mMediaPlayer.isPlaying()) {
	    	mMediaPlayer.stop();
	    	mMediaPlayer = null;
	    }
	}
		
	@Override
	protected void onDestroy() {
		super.onDestroy();
	}

}



