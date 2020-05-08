#include "stm32f10x_gpio.h"
#include "key.h"
#include "delay.h"
#include "oled.h"
#include <math.h>


/*display*/

char display[2][7]={"setmode","display"};
char clear[9]="         ";

/**/


extern u8 setmode,Page;
u8 upflag,downflag;
u8 count,count1;
void keyio_init(void)
{
		    GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB2PeriphClockCmd(IOEN, ENABLE );           
        GPIO_InitStructure.GPIO_Pin = setpin|enterpin|uppin|downpin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD ;   //????
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(IOPORT, &GPIO_InitStructure);
}


