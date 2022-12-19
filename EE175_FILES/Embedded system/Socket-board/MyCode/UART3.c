#include "UART3.h"

unsigned char Uart3_Receive_Data;
unsigned char Uart3_Receive_Change=0;

extern void AT_Uart_Data_Receive(unsigned char Recv_Data);

void USART3_IRQHandler(void)
{
	u8 res;	    
 	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //���յ�����
	{	 		 
		res =USART_ReceiveData(USART3); 	//��ȡ���յ�������
		Uart3_Receive_Data=res;
		Uart3_Receive_Change=1;
		AT_Uart_Data_Receive(Uart3_Receive_Data);
	}  											 
} 

/*
  ��������: ����һ���ֽ�
  ��������: byte ���� Ҫ���͵��ֽ�
  ����˵��: ��
*/
void UART3_SendByte(unsigned char byte)	
{
	USART_SendData(USART3,byte);//�򴮿�1��������
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}

/*
  ��������: ���ڷ����ַ���
  ��������: s ���� ָ���ַ�����ָ��(�ַ�����'\0'��β)
  ����˵��: ��
  ע��������ַ�����β��'\n'����ᷢ��һ���س�����      
*/
void UART3_SendStr(unsigned char *s)
{
	while( *s != '\0')
	{
		UART3_SendByte( *s );	
		s ++;
	}
}

//��ʼ��IO ����3
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void Uart3_Init(u32 bound)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);  

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,ENABLE);//��λ����3
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,DISABLE);//ֹͣ��λ
 
	
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;///��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;//�շ�ģʽ

  USART_Init(USART3, &USART_InitStructure);  //��ʼ������
  
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //ʹ�ܴ���2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
   
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���  
	UART3_SendByte(0xff);
}
