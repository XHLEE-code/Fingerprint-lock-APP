#include "ESP8266.h"
#include "delay.h"
#include "BEEP.h"
#define ESP8266_ON 0
char str1[5];
u8 ESP8266_R_ST_FLAG=0,ESP8266_R_FL_FLAG=0;
char ESP8266_R_1[30];
u8 ESP8266_R_count=0;

extern  char StringBuf[50];

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc;  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 
	TIM_ITConfig( 
		TIM3, 
		TIM_IT_Update ,
		ENABLE  
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM3, ENABLE);  
							 
}

void ESP8266_uart(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO  ,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate=115200;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1,ENABLE);

}
void ESP8266_NVIC(void)
{     
  
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0; 
	NVIC_Init(&NVIC_InitStructure);
	

	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
}

void HMISends(char *buf1)
{
	u8 i=0;
	while (1) {
        if (buf1[i]!=0) {
			USART_SendData(USART1, buf1[i]);  
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET){
                ;
            };
			i++;
		} else {
            return;
        }
    }
}

void TIM3_IRQHandler(void)  
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  
			HMISends(StringBuf);
		}
}


//void UART_PutStr_AT (USART_TypeDef* USARTx, char *str)    
//			{     
//				   while(*str != '\0')
//	        {
//		        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET); 
//		        USART_SendData(USARTx ,*str++);
//	        }
//			} 
//void ESP8266_Init(void)
//{
//		ESP8266_uart();
//		ESP8266_NVIC();
//		
//	  UART_PutStr_AT(USART1,"+++");
//	  delay_ms(1000);
//    UART_PutStr_AT(USART1,"AT+CWMODE=2\r\n");
//		delay_ms(500);
//		UART_PutStr_AT(USART1,"AT+RST\r\n");
//		delay_ms(1000);
//	  delay_ms(1000);
//		UART_PutStr_AT(USART1,"+++");
//	  delay_ms(1000);
//		UART_PutStr_AT(USART1,"AT+CWSAP=\"bysj\",\"123456789\",1,3\r\n");
//		delay_ms(500);
//		UART_PutStr_AT(USART1,"AT+CIPMUX=1\r\n");
//	  delay_ms(500);
//		UART_PutStr_AT(USART1,"AT+CIPSERVER=1,5000\r\n");
//}

//static void MYUSART2_SendData(u8 data)
//{
//	while((USART1->SR&0X40)==0); 
//	USART1->DR = data;
//}

//void send_count(u8 count)
//{
//	MYUSART2_SendData(0x01);
//	MYUSART2_SendData(count);
//	MYUSART2_SendData(0x01);
//}

//void ESP8266_GET(void)
//{
//		if(ESP8266_R_ST_FLAG==1&&ESP8266_R_FL_FLAG==1)
//			{
//				USART_Cmd(USART1,DISABLE);
//			  ESP8266_date_handle1();
//				ESP8266_R_count=0;
//				ESP8266_R_FL_FLAG=0;
//				ESP8266_R_ST_FLAG=0;
//	      USART_Cmd(USART1,ENABLE); 
//		}	
//}

//extern u8 mima[4];	
//int ESP8266_date_handle1(void)
//{		
//	mima[0]=ESP8266_R_1[1]-0x30;
//	mima[1]=ESP8266_R_1[2]-0x30;
//	mima[2]=ESP8266_R_1[3]-0x30;
//	mima[3]=ESP8266_R_1[4]-0x30;
//	if(mima_check()==1)
//	{
//		JDQ=0;
//		cont_flag ++;
//				delay_ms(1000);
//				delay_ms(1000);
//				delay_ms(1000);
//				delay_ms(1000);
//				delay_ms(1000);
//				delay_ms(1000);
//				JDQ=1;
//	}
//	memset(mima,0,sizeof(mima));
//	memset(ESP8266_R_1,0,sizeof(ESP8266_R_1));
//}

//void USART1_IRQHandler(void)                	//串口1中断服务程序
//	{	
//		uint8_t rxd;
//		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
//		{ 
//			USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//			if(ESP8266_R_FL_FLAG==0)
//			{ 
//				rxd=USART_ReceiveData(USART1);
//				if(rxd=='*')
//				{
//					ESP8266_R_ST_FLAG=1;	
//				}
//		  
//			if(ESP8266_R_ST_FLAG==1&&ESP8266_R_FL_FLAG==0)
//				{
//				
//				ESP8266_R_1[ESP8266_R_count]=USART_ReceiveData(USART1);
//			  ESP8266_R_count++;
//				if(rxd=='#')
//				{
//					 
//						ESP8266_R_FL_FLAG=1;
//					  USART_Cmd(USART1,DISABLE);
//				}	
//		  	}	
//				
//				
//			}
//				
//				
//			
//		}
//	
//	
//} 

//#if 0
//void USART1_IRQHandler(void)                	//串口1中断服务程序
//	{	
//		uint8_t rxd;
//		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
//		{ 
//			USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//			if(ESP8266_R_FL_FLAG==0)
//			{ 
//				rxd=USART_ReceiveData(USART1);
//				if(rxd=='+')
//				{
//					ESP8266_R_ST_FLAG=1;
//				}
//		  
//			if(ESP8266_R_ST_FLAG==1&&ESP8266_R_FL_FLAG==0)
//				{
//				
//				ESP8266_R_1[ESP8266_R_count]=USART_ReceiveData(USART1);
//			  ESP8266_R_count++;
//				if(rxd=='\n')
//				{
//					 
//					ESP8266_R_FL_FLAG=1;
//					ESP8266_GET();
//			
//				}	
//		  	}	
//					if(ESP8266_R_count==5)
//			  {
//					if(ESP8266_R_1[1]!='I'||ESP8266_R_1[2]!='P'||ESP8266_R_1[3]!='D')
//					{
//						ESP8266_R_ST_FLAG=0;
//						rxd=0;
//						
//						for(ESP8266_R_count=6;ESP8266_R_count>0;ESP8266_R_count--)
//						{
//								ESP8266_R_1[ESP8266_R_count]=0;
//						}
//					}
//					
//					
//			  }
//				
//			}
//				
//				
//			
//		}
//	
//	
//} 
//#endif
