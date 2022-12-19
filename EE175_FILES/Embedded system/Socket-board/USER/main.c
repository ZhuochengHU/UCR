
#include "delay.h"
#include "sys.h"
#include "AT_Command.h"
#include "ESP8266.h"
#include "UART3.h"
#include "Timer2.h"
#include "UART2.h"


void User_GPIO_Init(void);

#define Relay_Pin PBout(12)

unsigned char MS100_Con=0;

unsigned char SentTime_Con=0;

unsigned char Mode_Con=0;

#define LED_Pin PCout(13)

#define Adress_Pin1 PBin(9)
#define Adress_Pin2 PBin(8)

unsigned char MS1000_Con=0;

int main(void)
{		
	delay_init();//��ʼ���ӳ�
	NVIC_Configuration();//�趨�жϹ���
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA , ENABLE);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//��ʼ��AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//ʹ��SW����ģʽ���ر�JTAGģʽ
	User_GPIO_Init();
	
	Uart3_Init(115200);
	TIM2_Int_Init(99,7199);
	delay_ms(500);LED_Pin=~LED_Pin;
	delay_ms(500);LED_Pin=~LED_Pin;
	delay_ms(500);LED_Pin=~LED_Pin;
	delay_ms(500);LED_Pin=~LED_Pin;
	delay_ms(500);LED_Pin=~LED_Pin;
	delay_ms(500);LED_Pin=~LED_Pin;
	ESP8266_Init();
	Uart2_Init(9600);
	
	if(Adress_Pin1==0)
	{
		UART2_SendByte(0xa0+Mode_Con);
	}
	if(Adress_Pin2==0)
		UART2_SendByte(0xb0+Mode_Con);
	while(1)
	{
		if(MS1000_Con>=10)
		{
			MS1000_Con=0;
			
		}
		if(Uart2_Receive_Change)
		{
			Uart2_Receive_Change=0;
			switch(Uart2_Receive_Data)
			{
				case 0xC1:
				{
					if(Adress_Pin1==0)
					{
						Mode_Con=Mode_Con ? 0:1;
						SentTime_Con=20;
						if(Mode_Con==1)
							Relay_Pin=0;
						else
							Relay_Pin=1;
						
						if(Adress_Pin1==0)
						{
							UART2_SendByte(0xa0+Mode_Con);
						}
						if(Adress_Pin2==0)
							UART2_SendByte(0xb0+Mode_Con);
					}
					
					
			
					break;
				}
				case 0xC2:
				{
					if(Adress_Pin2==0)
					{
						Mode_Con=Mode_Con ? 0:1;
						SentTime_Con=20;
						if(Mode_Con==1)
							Relay_Pin=0;
						else
							Relay_Pin=1;
						
						if(Adress_Pin1==0)
						{
							UART2_SendByte(0xa0+Mode_Con);
						}
						if(Adress_Pin2==0)
							UART2_SendByte(0xb0+Mode_Con);
					}
					
					break;
				}
			}
		}
		if(SentTime_Con>=30)
		{
			SentTime_Con=0;
			
			
			if(WIFI_Connect==1)
			{
				LED_Pin=~LED_Pin;
				if(Mode_Con==0)
				{
					if(Adress_Pin1==0)
						ESP8266_Send_Data("DEV_AA,0,0\r\n");
					else
						ESP8266_Send_Data("DEV_AA,1,0\r\n");
				}
				else
				{
					if(Adress_Pin1==0)
						ESP8266_Send_Data("DEV_AA,0,1\r\n");
					else
						ESP8266_Send_Data("DEV_AA,1,1\r\n");
				}
			}
			else
			{
				LED_Pin=1;
			}
		}
		if(MS100_Con>=10)
		{
			MS100_Con=0;
			
			MS1000_Con++;
			
			
			/////////////////////////////////////////////////////////
			AT_ReceiveData_Disable();//����ATָ��
			ESP8266_Read_Data();
			
			if(ESP8266_Receive_Change==1)//���յ������豸����������
			{
				if(ESP8266_Receive_Data[0]=='1')
				{
					if(Adress_Pin1==0)
					{
						Mode_Con=Mode_Con ? 0:1;
						SentTime_Con=20;
						if(Mode_Con==1)
							Relay_Pin=0;
						else
							Relay_Pin=1;
						
						if(Adress_Pin1==0)
						{
							UART2_SendByte(0xa0+Mode_Con);
						}
						if(Adress_Pin2==0)
							UART2_SendByte(0xb0+Mode_Con);
					}
					
				}
				if(ESP8266_Receive_Data[0]=='2')
				{
					if(Adress_Pin2==0)
					{
						Mode_Con=Mode_Con ? 0:1;
						SentTime_Con=20;
						if(Mode_Con==1)
							Relay_Pin=0;
						else
							Relay_Pin=1;
						
						if(Adress_Pin1==0)
						{
							UART2_SendByte(0xa0+Mode_Con);
						}
						if(Adress_Pin2==0)
							UART2_SendByte(0xb0+Mode_Con);
					}
					
				}
				
			}
			ESP8266_Receive_Change=0;
			
	
			///////////////////////////////////////////////////////////////////

			SentTime_Con++;
		}
	}
}

void TIM2_IRQHandler(void)   //TIM2�ж�
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־
		MS100_Con++;
	}
}


void User_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	Relay_Pin=1;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	LED_Pin=1;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}






