




#include "AT_Command.h"


unsigned int AT_Uart_RecvCount = 0;			// �������ݴ�С
unsigned char AT_Uart_RecvBuffer[AT_BUFFER_SIZE];//���ڴ��洮�ڽ��ܵ�����Ϣ



unsigned char AT_Command_Data[AT_Command_Data_Long];//���ڴ���Ҫ���͵�AT���� �жϵĻ�Ӧ���� �͵ƴ���ʱ��
unsigned char AT_Response_Data[AT_Response_Data_Long];//���ڴ�����Ҫ�жϵĻ�Ӧ����
unsigned char AT_Wite_Data[AT_WiteRepeat_Data_Long];//���ڴ���ÿ��ָ������ȴ�ʱ�� ��λ100ms
unsigned char AT_Repeat_Data[AT_WiteRepeat_Data_Long];//���ڴ���ÿ��ָ������ظ�����

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
	���ݻ����ʼ��
*/
void AT_Uart_DataInit()
{
	memset(AT_Uart_RecvBuffer,0,AT_BUFFER_SIZE);	// ��ջ�����
	AT_Uart_RecvCount = 0;								// �������±��λ
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
�ж�CHR_A�ַ����ڣ��Ƿ���CHR_B

����⵽A����B���ַ������򷵻��ַ���B�����ַ���A�е���λ
��û�м�⵽������ֵΪ0XFF

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
AT_FeedBack_Con ����ֵ:
0-û��������Ҫ����
1-����ͳɹ�
2-��һ�������ʧ��
*/


void AT_ReceiveData_Disable()//��ʱѭ�����ã�����ÿ��100ms����һ��
{

	unsigned char Num_A=0;
	unsigned int Adrss_Data;

	static unsigned char Start_EN=1;

	static unsigned char Wite_Num=0;

	if(AT_Command_Data[0]!=0)//��Ҫ����Ӧ������
	{
		if(Start_EN==1||Wite_Num==0)//��⵽�ǿ�ʼ���л��߾������ӳ�֮��
		{
			if(Start_EN==1)//����ǿ�ʼ���У���ȥ����ʼ��־������ATָ����ҷ�ֵWite_Num
			{
				Start_EN=0;
				
				Wite_Num=AT_Wite_Data[0];
				AT_SendStr(AT_Command_Data);
				AT_Repeat_Data[0]--;
			}
			else
			{
				Adrss_Data=AT_STRSTR(AT_Uart_RecvBuffer,AT_Response_Data);
				if(Adrss_Data!=0xffff||AT_Repeat_Data[0]==0)//�õ���Ч��Ӧ�������Ѿ��ظ��˹���Ĵ����������һ������
				{
					if(Adrss_Data!=0xffff)//����ͳɹ�
					{
						AT_FeedBack_Con=1;
					}
					else//�����ʧ��
					{
						AT_FeedBack_Con=2;
					}
					for(Num_A=0;Num_A<10;Num_A++)
					{
						AT_Feedback_Command[Num_A]=AT_Command_Data[Num_A];
					}
					Num_A=0;
					AT_Command_Replace();//����ǰ�������
					
					
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


