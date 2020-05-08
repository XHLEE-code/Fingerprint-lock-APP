#ifndef __ESP8266_H
#define __ESP8266_H	
#include "sys.h"


extern u8 cont_flag;

void TIM3_Int_Init(u16 arr,u16 psc);
void ESP8266_uart(void);
void ESP8266_NVIC(void);
void ESP8266_Init(void);
int  ESP8266_date_handle1(void);
void UART_PutStr_AT (USART_TypeDef* USARTx, char *str);
void esp8266_temp_send(void);
void esp8266_humi_send(void);
void esp8266_MQ_send(void);
void esp8266_PM25_send(void);
void esp8266_GMDZ_send(void);

void ESP8266_GET(void);
u8 mima_check(void);

void send_count(u8 count);

#endif
