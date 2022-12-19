#include "sys.h"
#include "delay.h"
#include <string.h>
#include <stdlib.h>
#include "ESP8266.h"
#include "AT_Command.h"

unsigned char ESP8266_Receive_Data[ESP8266_BUFFER_SIZE];
unsigned char ESP8266_Receive_Change;
unsigned char WIFI_Connect=0;




/*
开机运行初始化函数
初始化成功，返回0
*/

void ESP8266_Init()
{
	WIFI_Connect=0;
	AT_CommandSend("AT\r\n","OK",1,10);
	AT_CommandSend("ATE0\r\n","OK",1,10);
	AT_CommandSend("AT+CWMODE=1\r\n","OK",1,10);//设置STA模式
	AT_CommandSend("AT+CWJAP=\"test\",\"111222333\"\r\n","OK",100,10);//设置路由名称和密码
	AT_CommandSend("AT+CIPSTART=\"TCP\",\"112.124.108.26\",8038\r\n","CONNECTED",50,10);//链接云端服务器
}


void ESP8266_Send_Data(unsigned char *Send_Data)//发送一次数据最大时间为2秒，建议间隔时间3秒。
{
	unsigned char Send_Num;
	unsigned char CIPSEND_TenLessCom[]="AT+CIPSEND=0\r\n";
	unsigned char CIPSEND_TenCom[]="AT+CIPSEND=00\r\n";
	Send_Num=0;
	while(*(Send_Data+Send_Num)!='\0')
	{
		Send_Num++;
	}	
	if(Send_Num<10)
	{
		CIPSEND_TenLessCom[11]=Send_Num+0X30;
		AT_CommandSend(CIPSEND_TenLessCom,">",1,1);
	}
	else
	{
		CIPSEND_TenCom[11]=Send_Num/10+0X30;
		CIPSEND_TenCom[12]=Send_Num%10+0X30;
		AT_CommandSend(CIPSEND_TenCom,">",1,1);
	}
	
	AT_CommandSend(Send_Data,"SEND OK",10,2);
	
}



/*
读取接收到的数据，并把ID号传输到

ESP8266_Link_ID

把接收到的有效数据，转移到ESP8266_Receive_Data;
*/
void ESP8266_Read_Data(void)
{
	unsigned char Data_Num_B=0;
	unsigned char Des_Adress=0;

	unsigned char static Detect_Num;
	
	if(AT_FeedBack_Con==2)//有命令没有发送成功
	{
		AT_FeedBack_Con=0;
		if(AT_STRSTR(AT_Feedback_Command,"CIPST")!=0xffff)//没有注册到云端
		{
			ESP8266_Init();
		}
		if(AT_STRSTR(AT_Feedback_Command,"CWJAP")!=0xffff)//没有连接到路由器
		{
			ESP8266_Init();
		}
		if(AT_STRSTR(AT_Feedback_Command,"CIPSEN")!=0xffff)//检测到发送的数据没有成功
		{
			AT_Command_Replace();//取消数据发送
			Detect_Num++;
			if(Detect_Num>5)
			{
				ESP8266_Init();//从新初始化WIFI模块
			}
		}
		if(AT_STRSTR(AT_Feedback_Command,"DEV")!=0xffff)//检测到发送的命令没有传输到云平台
		{
			Detect_Num++;
			if(Detect_Num>5)
			{
				ESP8266_Init();//从新初始化WIFI模块
			}
		}
	}
	else
	{
		if(AT_FeedBack_Con==1)
		{
			Detect_Num=0;
			if(AT_STRSTR(AT_Feedback_Command,"CIPST")!=0xffff)//已经注册到云端
				WIFI_Connect=1;
		}
		
	}
	
	Des_Adress=AT_STRSTR(AT_Uart_RecvBuffer,"+IPD");
	
	if(Des_Adress!=0xff)//接收到传输的数据
	{
		ESP8266_Receive_Change=1;
		memset(ESP8266_Receive_Data,0,ESP8266_BUFFER_SIZE);//清空接收区
		AT_Uart_RecvBuffer[Des_Adress]=0xff;
		for(;Des_Adress<0xff;Des_Adress++)
		{
			if(AT_Uart_RecvBuffer[Des_Adress]==':')
			{
				break;
			}
		}
		for(Data_Num_B=0;Data_Num_B<0xff;Data_Num_B++)
		{
			ESP8266_Receive_Data[Data_Num_B]=AT_Uart_RecvBuffer[Des_Adress+1+Data_Num_B];
			if(AT_Uart_RecvBuffer[Des_Adress+Data_Num_B+1]=='\0')
			{
				break;
			}
		}
	}
	if(AT_FeedBack_Con==1)
		AT_Uart_DataInit();//清空接受的数据
	AT_FeedBack_Con=0;
}




