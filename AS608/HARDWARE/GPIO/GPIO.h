#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#define LED PCout(13)	// PD2	
#define BEEP PAout(8)

#define SHAKE GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)
#define RTHW  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)


void GPIO_Init_LED(void);//��ʼ��
void GPIO_Init_BEEP(void);

void GPIO_Init_RTHW(void);
		 				    
#endif
