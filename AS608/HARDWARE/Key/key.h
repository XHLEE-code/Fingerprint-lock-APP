#ifndef _KEY_H
#define _KEY_H

#include "stm32f10x.h"

#define setpin   GPIO_Pin_5
#define enterpin GPIO_Pin_8
#define uppin    GPIO_Pin_6
#define downpin  GPIO_Pin_7

#define IOEN   RCC_APB2Periph_GPIOB
#define IOPORT GPIOB
#define set    GPIO_ReadInputDataBit(IOPORT,setpin)
#define enter  GPIO_ReadInputDataBit(IOPORT,enterpin)
#define up     GPIO_ReadInputDataBit(IOPORT,uppin)
#define down   GPIO_ReadInputDataBit(IOPORT,downpin)


void keyio_init(void);
//void key_scanf (void);



#endif
