#ifndef __UART3_H
#define __UART3_H	 
#include "sys.h"
#include "delay.h"

extern unsigned char Uart3_Receive_Data;
extern unsigned char Uart3_Receive_Change;

void Uart3_Init(u32 bound);
void UART3_SendByte(unsigned char byte);
void UART3_SendStr(unsigned char *s);

#endif
