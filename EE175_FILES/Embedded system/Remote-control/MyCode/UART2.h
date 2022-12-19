#ifndef __UART2_H
#define __UART2_H	 
#include "sys.h"
#include "delay.h"

extern unsigned char Uart2_Receive_Data;
extern unsigned char Uart2_Receive_Change;

void Uart2_Init(u32 bound);
void UART2_SendByte(unsigned char byte);
void UART2_SendStr(unsigned char *s);

#endif
