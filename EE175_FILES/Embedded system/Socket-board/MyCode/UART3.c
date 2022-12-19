#include "UART3.h"

unsigned char Uart3_Receive_Data;
unsigned char Uart3_Receive_Change=0;

extern void AT_Uart_Data_Receive(unsigned char Recv_Data);

void USART3_IRQHandler(void)
{
	u8 res;	    
 	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收到数据
	{	 		 
		res =USART_ReceiveData(USART3); 	//读取接收到的数据
		Uart3_Receive_Data=res;
		Uart3_Receive_Change=1;
		AT_Uart_Data_Receive(Uart3_Receive_Data);
	}  											 
} 

/*
  功能描述: 发送一个字节
  函数参数: byte —— 要发送的字节
  返回说明: 无
*/
void UART3_SendByte(unsigned char byte)	
{
	USART_SendData(USART3,byte);//向串口1发送数据
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
}

/*
  功能描述: 串口发送字符串
  函数参数: s —— 指向字符串的指针(字符串以'\0'结尾)
  返回说明: 无
  注：如果在字符串结尾有'\n'，则会发送一个回车换行      
*/
void UART3_SendStr(unsigned char *s)
{
	while( *s != '\0')
	{
		UART3_SendByte( *s );	
		s ++;
	}
}

//初始化IO 串口3
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void Uart3_Init(u32 bound)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;//上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);  

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,ENABLE);//复位串口3
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3,DISABLE);//停止复位
 
	
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;///奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;//收发模式

  USART_Init(USART3, &USART_InitStructure);  //初始化串口
  
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //使能串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
   
  USART_Cmd(USART3, ENABLE);                    //使能串口  
	UART3_SendByte(0xff);
}
