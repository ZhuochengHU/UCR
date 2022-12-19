#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "sys.h"
#include "delay.h"
#include <string.h>
#include <stdlib.h>



#define ESP8266_BUFFER_SIZE 50	

extern unsigned char ESP8266_Receive_Data[ESP8266_BUFFER_SIZE];
extern unsigned char ESP8266_Receive_Change;
extern unsigned char WIFI_Connect;


void ESP8266_Read_Data(void);

void ESP8266_Send_Data(unsigned char *Send_Data);

void ESP8266_Init(void);

#endif
