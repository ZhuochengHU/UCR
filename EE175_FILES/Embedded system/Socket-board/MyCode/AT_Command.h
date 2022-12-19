#ifndef __AT_Command_H
#define __AT_Command_H	

#include <string.h>
#include "sys.h"

#define AT_BUFFER_SIZE  256					// 指令返回接收缓冲区
#define AT_Command_Data_Long 250
#define AT_Response_Data_Long 50
#define AT_WiteRepeat_Data_Long 15

extern unsigned char AT_Uart_RecvBuffer[AT_BUFFER_SIZE];
extern unsigned char AT_Feedback_Command[10];
extern unsigned char AT_FeedBack_Con;

unsigned char AT_CommandSend(unsigned char *Command_Data,unsigned char *Response_Data,unsigned char Wite_Data,unsigned char Repeat_Data);
void AT_Command_Data_AddByte(unsigned char Data);

void AT_Uart_DataInit(void);//数据缓冲初始化

void AT_Uart_Data_Receive(unsigned char Recv_Data);//在串口中断里接受数据

void AT_SendByte(unsigned char byte);
void AT_SendStr(unsigned char *s);
unsigned int AT_STRSTR(unsigned char *CHR_A,unsigned char *CHR_B);

void AT_ReceiveData_Disable(void);//定时循环调用，建议每次100ms调用一次

void AT_Command_Replace(void);

	
#endif
