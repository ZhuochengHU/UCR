




#include "AT_Command.h"


unsigned int AT_Uart_RecvCount = 0;			// 接收数据大小
unsigned char AT_Uart_RecvBuffer[AT_BUFFER_SIZE];//用于储存串口接受到的信息



unsigned char AT_Command_Data[AT_Command_Data_Long];//用于储存要发送的AT命令 判断的回应数据 和灯带的时间
unsigned char AT_Response_Data[AT_Response_Data_Long];//用于储存需要判断的回应数据
unsigned char AT_Wite_Data[AT_WiteRepeat_Data_Long];//用于储存每次指令的最大等待时间 单位100ms
unsigned char AT_Repeat_Data[AT_WiteRepeat_Data_Long];//用于储存每条指令最大重复次数

void AT_Command_Data_AddByte(unsigned char Data)
{

	unsigned char Num_A=0;
	
	for(Num_A=0;Num_A<(AT_Command_Data_Long-1);Num_A++)
	{
		if(AT_Command_Data[Num_A]==0&&AT_Command_Data[Num_A+1]==0)
			break;
	}

		
	AT_Command_Data[Num_A]=Data;
}


unsigned char AT_CommandSend(unsigned char *Command_Data,unsigned char *Response_Data,unsigned char Wite_Data,unsigned char Repeat_Data)
{
	unsigned char Num_A=0;
	unsigned char Num_B=0;
	
	for(Num_B=0;Num_B<(AT_Command_Data_Long-1);Num_B++)
	{
		if(AT_Command_Data[Num_B]==0&&AT_Command_Data[Num_B+1]==0)
			break;
	}
	if(Num_B!=0)
		Num_B++;
		
	while(Command_Data[Num_A]!=0)
	{
		if((Num_B+Num_A)<(AT_Command_Data_Long-1))
		{
			AT_Command_Data[Num_B+Num_A]=Command_Data[Num_A];
			Num_A++;
		}
		else
		{
			return 0;
		}
	}
	AT_Command_Data[Num_B+Num_A]=0;
	/////////////////////////////////////////////////////////////////////////
	Num_A=0;
	for(Num_B=0;Num_B<(AT_Response_Data_Long-1);Num_B++)
	{
		if(AT_Response_Data[Num_B]==0&&AT_Response_Data[Num_B+1]==0)
			break;
	}
	
	if(Num_B!=0)
		Num_B++;
	
	while(Response_Data[Num_A]!=0)
	{
		if((Num_B+Num_A)<(AT_Response_Data_Long-1))
		{
			AT_Response_Data[Num_B+Num_A]=Response_Data[Num_A];
			Num_A++;
		}
		else
		{
			return 0;
		}
	}
	AT_Response_Data[Num_B+Num_A]=0;
	//////////////////////////////////////////////////////////////////////////
	
	for(Num_B=0;Num_B<AT_WiteRepeat_Data_Long;Num_B++)
	{
		if(AT_Wite_Data[Num_B]==0)
			break;
	}
	if(Num_B<AT_WiteRepeat_Data_Long)
		AT_Wite_Data[Num_B]=Wite_Data;
	else
		return 0;
	
	////////////////////////////////////////////////////////
	for(Num_B=0;Num_B<AT_WiteRepeat_Data_Long;Num_B++)
	{
		if(AT_Repeat_Data[Num_B]==0)
			break;
	}
	if(Num_B<AT_WiteRepeat_Data_Long)
		AT_Repeat_Data[Num_B]=Repeat_Data;
	else
		return 0;
	
	return 1;
}


extern void UART3_SendByte(unsigned char byte);

/*
	数据缓冲初始化
*/
void AT_Uart_DataInit()
{
	memset(AT_Uart_RecvBuffer,0,AT_BUFFER_SIZE);	// 清空缓冲区
	AT_Uart_RecvCount = 0;								// 缓冲区下标归位
}

void AT_Uart_Data_Receive(unsigned char Recv_Data)
{
	if(Recv_Data!=0)
	{
		AT_Uart_RecvBuffer[AT_Uart_RecvCount++] = Recv_Data;
		if(AT_Uart_RecvCount==AT_BUFFER_SIZE)
			AT_Uart_RecvCount=0;
	}
}


void AT_SendByte(unsigned char byte)
{
	UART3_SendByte(byte);
}

void AT_SendStr(unsigned char *s)
{
	while( *s != '\0')
	{
		AT_SendByte( *s );	
		s ++;
	}
}

/*
判断CHR_A字符串内，是否有CHR_B

当检测到A中有B的字符串，则返回字符串B的首字符在A中的数位
当没有检测到，返回值为0XFF

*/
unsigned int AT_STRSTR(unsigned char *CHR_A,unsigned char *CHR_B)
{
	unsigned int Num_A=0;
	unsigned int Num_B=0;
	while(CHR_A[Num_A]!=0)
	{
		Num_B=0;
		while(1)
		{
			if(*(CHR_A+Num_A+Num_B)!=*(CHR_B+Num_B))
				break;
			else
			{
				Num_B++;
				if(CHR_B[Num_B]==0)
					return(Num_A);
			}
		}
		Num_A++;
	}
	return(0xffff);
}



void AT_Command_Replace()
{
	unsigned char Num_A=0;
	unsigned char Num_B=0;
	
	for(Num_B=0;Num_B<(AT_Command_Data_Long-1);Num_B++)
	{
		if(AT_Command_Data[Num_B]==0)
			break;
	}
	Num_B++;
					
	for(Num_A=0;Num_A<AT_Command_Data_Long;Num_A++)
	{
		AT_Command_Data[Num_A]=AT_Command_Data[Num_A+Num_B];
		if((AT_Command_Data[Num_A+Num_B]==0&&AT_Command_Data[Num_A+Num_B+1]==0)||(Num_A+Num_B)==(AT_Command_Data_Long-1))
			break;
	}
					
	for(;Num_A<AT_Command_Data_Long;Num_A++)
	{
		AT_Command_Data[Num_A]=0;
	}
					///////////////////////////////////////////////////////////////////////////////
					
	for(Num_B=0;Num_B<(AT_Response_Data_Long-1);Num_B++)
	{
		if(AT_Response_Data[Num_B]==0)
			break;
	}
	Num_B++;
					
	for(Num_A=0;Num_A<AT_Response_Data_Long;Num_A++)
	{
		AT_Response_Data[Num_A]=AT_Response_Data[Num_A+Num_B];
		if((AT_Response_Data[Num_A+Num_B]==0&&AT_Response_Data[Num_A+Num_B+1]==0)||(Num_A+Num_B)==(AT_Response_Data_Long-1))
			break;
	}
					
	for(;Num_A<AT_Response_Data_Long;Num_A++)
	{
		AT_Response_Data[Num_A]=0;
	}
					////////////////////////////////////////////////////////////////////
					
	for(Num_B=0;Num_B<AT_WiteRepeat_Data_Long-1;Num_B++)
	{
		AT_Wite_Data[Num_B]=AT_Wite_Data[Num_B+1];
	}
	AT_Wite_Data[Num_B]=0;
					//////////////////////////////////////////////////////
	for(Num_B=0;Num_B<AT_WiteRepeat_Data_Long-1;Num_B++)
	{
		AT_Repeat_Data[Num_B]=AT_Repeat_Data[Num_B+1];
	}
	AT_Repeat_Data[Num_B]=0;
}



unsigned char AT_Feedback_Command[10];
unsigned char AT_FeedBack_Con=0;
/*
AT_FeedBack_Con 返回值:
0-没有命令需要发送
1-命令发送成功
2-有一个命令发送失败
*/


void AT_ReceiveData_Disable()//定时循环调用，建议每次100ms调用一次
{

	unsigned char Num_A=0;
	unsigned int Adrss_Data;

	static unsigned char Start_EN=1;

	static unsigned char Wite_Num=0;

	if(AT_Command_Data[0]!=0)//需要检测回应的数据
	{
		if(Start_EN==1||Wite_Num==0)//检测到是开始运行或者经过了延迟之后
		{
			if(Start_EN==1)//如果是开始运行，则去除开始标志，发送AT指令，并且幅值Wite_Num
			{
				Start_EN=0;
				
				Wite_Num=AT_Wite_Data[0];
				AT_SendStr(AT_Command_Data);
				AT_Repeat_Data[0]--;
			}
			else
			{
				Adrss_Data=AT_STRSTR(AT_Uart_RecvBuffer,AT_Response_Data);
				if(Adrss_Data!=0xffff||AT_Repeat_Data[0]==0)//得到有效回应，或者已经重复了过多的次数，则清除一次命令
				{
					if(Adrss_Data!=0xffff)//命令发送成功
					{
						AT_FeedBack_Con=1;
					}
					else//命令发送失败
					{
						AT_FeedBack_Con=2;
					}
					for(Num_A=0;Num_A<10;Num_A++)
					{
						AT_Feedback_Command[Num_A]=AT_Command_Data[Num_A];
					}
					Num_A=0;
					AT_Command_Replace();//将当前命令清除
					
					
				}
				else
				{
					AT_SendStr(AT_Command_Data);
					Wite_Num=AT_Wite_Data[0];
					AT_Repeat_Data[0]--;
				}
			}
		}
		else
		{
			Wite_Num--;
		}
	}
	else
	{
		Start_EN=1;
		AT_FeedBack_Con=0;
	}
}


