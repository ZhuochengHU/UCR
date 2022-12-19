package com.fwjie.wifi1.utils;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.net.Socket;

public class IOUtil {
	
	public static void close(Socket socket)
	{
		if(socket != null && !socket.isClosed()){
			try {
				socket.close();
			} catch (IOException e) {
				// TODO 自动生成的 catch 块
				e.printStackTrace();
			}
			socket = null;
		}
		
	}

	public static void close(BufferedWriter wr) {
		if(wr != null){
			try {
				wr.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
	}

	public static void close(BufferedReader br) {
		if(br != null){
			try {
				br.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
	}

}
