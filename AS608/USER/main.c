#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "GPIO.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "key.h"
#include "BEEP.h"
#include "math.h"
#include "ESP8266.h"
#include "as608.h"
#include "stmflash.h"


#define setcount 5
#define pagecount 1
/*void devicecheck(void);
void deviceinit(void);
void Int_To_Str(int n, char *str);
void sendhandle1(int n,int m);
void sendhandle2(int n);
void rxdatahandle();*/
void key_scanf (void);
void key_AS608_scanf(void);


void UART_PutStr (USART_TypeDef* USARTx, uint8_t *str,uint8_t size);
void UART_PutChar (USART_TypeDef* USARTx, uint8_t Data);




/*void bjcheck();
char sendtxt[22];
char sendtxt1[7];

u8 deviceid[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
u8 devicedata[12]={0,0,0,0,0,0,0,0,0,0,0,0};
u8 devicecount=0,devicenum[20];


*/


void UART_PutStr (USART_TypeDef* USARTx, uint8_t *str,uint8_t size)    
			{   int i; 
			for (i=0;i<size;i++)    
			{    
        UART_PutChar(USARTx, *str);    
        str++;    
			}    
			} 
void UART_PutChar (USART_TypeDef* USARTx, uint8_t Data)
{			
			
	   
      USART_SendData(USARTx,Data);	
	     while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
	
			while(0){}  
}
//传感器参数
float HC05;
float temp;
float humi;
float PM25;
float MQ;
float Fire;
float GMDZ;
float HX711;
float HX711yuanshi;
//按键
char Page;
char setmode;
char keycount0;//显示翻页计数
char keycount1;//设置翻页计数
char setguangbiao;

char str[20];
//时间参数

u16 nian;
u8  yue;
u8  ri;
u8  xiaoshi;
u8  fenzhong;
u8  miao;
u16 setnian=2019;
u8  setyue=8;
u8  setri=8;
u8  setxiaoshi;
u8  setfenzhong;
u8  setmiao;
u8  guangbiao;
//设置上下限

u8 Tempup=100;
u8 Tempdown=0;
u8 Humiup=100;
u8 Humidown=0;
u8 HC05up=100;
u8 HC05down=0;
u8 Fireup=100;
u8 Firedown=0;
u8 MQup=100;
u8 MQdown=0;
u8 PM25up=100;
u8 PM25down=0;
u8 GQup=100;
u8 GQdown=0;
u8 HX711up=100;
u8 HX711down=0;

//mimasuo
u8 adminmima[4]={'6','6','6','6'};
u8 mima[4];
u8 mimazancun[4];
extern u8 datatemp[];
extern u32 AS608Addr;
extern SearchResult seach;
extern u8 TEXT_Buffer[];
u8 kaisuoflag=0;
u8 kaisuocount=0;
u8 shurucuowucishu=0;
u8 cont_flag = 0;
//char str1[5];
char StringBuf[50] = "";

int main(void)
{	
   mima_Read();
	 delay_init();
	 AS608_Init();
	 keyio_init();
	 OLED_Init();
	 ESP8266_uart();
	 ESP8266_NVIC();
	 Beep_Init();
	 JDQ_init();
	 TIM3_Int_Init(14999,7199);
	 BEEP=1;
	 delay_ms(1000);
	 BEEP=0; 
	 delay_ms(200);
	 OLED_Fill(0x00);
   	while(1)
	{
		key_AS608_scanf();
		sprintf(StringBuf, "12345678&msg=on%03d%03d%03d\r\n", cont_flag , shurucuowucishu , seach.pageID);

	}

}
u8 mima_check(void)
{
  int i;
	for(i=0;i<4;i++)
	{
		if(mima[i]!=datatemp[i])
			break;
	}
  if(i==4)
		
	{memset(mima,0,sizeof(mima));return 1;}
	else  return 0;
}
u8 adminmima_check(void)
{
  int i;
	for(i=0;i<4;i++)
	{
		if(mima[i]!=adminmima[i])
			break;
	}
  if(i==4)
		
	{memset(mima,0,sizeof(mima));return 1;}
	else  return 0;
}

void mima_display(void)
{

	Dis_Num(4,0,mima[0],1);
	Dis_Num(4,2,mima[1],1);
	Dis_Num(4,4,mima[2],1);
	Dis_Num(4,6,mima[3],1);
	
}
void key_AS608_scanf(void)
{
	if(kaisuocount==1)
	{	if(set==1)
		{
			 while(set==1);
				  setmode++;
					if(setmode==setcount)
					{setmode=0;}
					 OLED_Fill(0x00);
					setguangbiao=0;
					memset(mima,0,sizeof(mima));
		}
	}
		
		if(setmode==0)
		{
		  
//			ESP8266_GET();
			OLED_Fill(0x00);
			OLED_P8x16Str(0,0,"IN KEY OR PRESS");
			mima_display();
			
      delay_ms(200);
			press_FR();
			if(kaisuoflag==1)
			{
				kaisuoflag=0;
			  JDQ=0;
				cont_flag++;
				delay_ms(1000);
				delay_ms(1000);
				JDQ=1;
				kaisuocount=1;	
			}
			
			
			 if(enter==1)
			{
				
				OLED_Fill(0x00);
				while(enter==1);
		  	setguangbiao++;
				//SET_PM25_display();
				if(setguangbiao==5)
				{
				setguangbiao=0;
				}
				
							if(setguangbiao==4)
		  { if(mima_check()==1)
			{
				BEEP=0;
				shurucuowucishu=0;
				kaisuoflag=0;
			  JDQ=0;
				cont_flag ++;
				delay_ms(1000);
				delay_ms(1000);
				JDQ=1;
				kaisuocount=1;	
				
			}
			if(mima_check()==0)
			 {
					shurucuowucishu++;
				 if(shurucuowucishu>3)
				 {
					 
				  BEEP=1;
				  setmode=6;
				 }
			 } 
		  }
			
			}

		
				OLED_P8x16Str(56,4,"   ");
				//delay_ms(50);
				//SET_PM25_display();
				if(up==1)
				{
				while(up);
		  	mima[setguangbiao]++;
				if(mima[setguangbiao]==10)
				{
				mima[setguangbiao]=0;
				}
				//SET_PM25_display();
				}
				if(down==1)
				{
				//OLED_P8x16Str(56,4,"   ");
				//delay_ms(50);
				while(down);
					if(mima[setguangbiao]>PM25down)
				{
				mima[setguangbiao]--;
				}
		  	
				if(mima[setguangbiao]<0)
				{
				mima[setguangbiao]=9;
				}
				
		
				}
			}
		if(setmode==1)
		{
			OLED_P8x16Str(0,0,"SET KEY");
			mima_display();
			 if(enter==1)
			{
				
				OLED_Fill(0x00);
				while(enter==1);
		  	setguangbiao++;
				//SET_PM25_display();
				if(setguangbiao==5)
				{
				setguangbiao=0;
				}
			}
		
		
				OLED_P8x16Str(56,4,"   ");
				//delay_ms(50);
				//SET_PM25_display();
				if(up==1)
				{
				while(up);
		  	mima[setguangbiao]++;
				if(mima[setguangbiao]==10)
				{
				mima[setguangbiao]=0;
				}
				//SET_PM25_display();
				}
				if(down==1)
				{
				//OLED_P8x16Str(56,4,"   ");
				//delay_ms(50);
				while(down);
					if(mima[setguangbiao]>PM25down)
				{
				mima[setguangbiao]--;
				}
		  	
				if(mima[setguangbiao]<0)
				{
				mima[setguangbiao]=9;
				}
				
		
				}
				if(setguangbiao==4)
				{
				if(mima[0]!=0&&mima[0]!=0&&mima[0]!=0&&mima[0]!=0)
				{
				TEXT_Buffer[0]=mima[0];
				TEXT_Buffer[1]=mima[1];
				TEXT_Buffer[2]=mima[2];
				TEXT_Buffer[3]=mima[3];
				datatemp[0]=mima[0];
				datatemp[1]=mima[1];
				datatemp[2]=mima[2];
				datatemp[3]=mima[3];	
				mima_write();
				OLED_P8x16Str(56,6,"OK");
					delay_ms(1000);
				OLED_P8x16Str(56,6,"   ");}
				else {OLED_P8x16Str(56,6,"Error");delay_ms(1000);
				OLED_P8x16Str(56,6,"     ");}
        memset(mima,0,sizeof(mima));
					setguangbiao=0;
				
				}
				
		
		}
		
			if(setmode==2)
		{
		    OLED_Fill(0x00);
        OLED_P8x16Str(0,0,"ADD FR!");
			 delay_ms(1000);
			 delay_ms(500);
			Add_FR();
		}
		if(setmode==3)
		{
		    OLED_Fill(0x00);
        OLED_P8x16Str(0,0,"DEL FR!");
			  delay_ms(1000);
			  delay_ms(500);
		  	Del_FR();
		}
}
	

