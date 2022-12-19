
#include "delay.h"
#include "sys.h"
#include "UART2.h"

void User_GPIO_Init(void);

#define Key_Pin1 PBin(0)
#define Key_Pin2 PBin(1)

#define LED_Pin1 PBout(6)
#define LED_Pin2 PBout(5)

int main(void)
{		
	delay_init();//��ʼ���ӳ�
	NVIC_Configuration();//�趨�жϹ���
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA , ENABLE);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//��ʼ��AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//ʹ��SW����ģʽ���ر�JTAGģʽ
	User_GPIO_Init();
	Uart2_Init(9600);
	delay_ms(500);
	while(1)
	{
		if(Uart2_Receive_Change)
		{
			Uart2_Receive_Change=0;
			
		}
		
		if(Key_Pin1==0)
		{
			delay_ms(10);
			if(Key_Pin1==0)
			{
				while(Key_Pin1==0);
				UART2_SendByte(0xc1);
			}
		}
		
		if(Key_Pin2==0)
		{
			delay_ms(10);
			if(Key_Pin2==0)
			{
				while(Key_Pin2==0);
				UART2_SendByte(0xc2);
			}
		}
	}
}

void User_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	LED_Pin1=0;
	LED_Pin2=0;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	 
}

void USART2_IRQHandler(void)
{
	u8 res;	    
 	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //���յ�����
	{	 		 
		res =USART_ReceiveData(USART2); 	//��ȡ���յ�������
		Uart2_Receive_Data=res;
		Uart2_Receive_Change=1;
		switch(Uart2_Receive_Data)
			{
				case 0xa0:LED_Pin1=1;break;
				case 0xa1:LED_Pin1=0;break;
				case 0xb0:LED_Pin2=1;break;
				case 0xb1:LED_Pin2=0;break;
			}
	}  											 
} 




