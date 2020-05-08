#include <string.h>
#include "delay.h" 	
//#include "usart2.h"
#include "as608.h"
#include "oled.h"
#include "key.h"
/*USART2*/

#define USART2_MAX_RECV_LEN		400					//最大接收缓存字节数
#define USART2_MAX_SEND_LEN		400					//最大发送缓存字节数
#define USART2_RX_EN 			1					//0,不接收;1,接收.

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//接收缓冲,最大USART2_MAX_RECV_LEN字节
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//发送缓冲,最大USART2_MAX_SEND_LEN字节
extern vu16 USART2_RX_STA;   						//接收数据状态

void usart2_init(u32 bound);				//串口2初始化 
void u2_printf(char* fmt,...);

u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 			  //发送缓冲,最大USART2_MAX_SEND_LEN字节

SysPara AS608Para;//指纹模块AS608参数
u16 ValidN;//模块内有效模板个数
u8** kbd_tbl;

//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
vu16 USART2_RX_STA=0;   	


void USART2_IRQHandler(void)
{
	u8 res;	      
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		res =USART_ReceiveData(USART2);		 
		if((USART2_RX_STA&(1<<15))==0)//接收完的一批数据,还没有被处理,则不再接收其他数据
		{ 
			if(USART2_RX_STA<USART2_MAX_RECV_LEN)	//还可以接收数据
			{
				TIM_SetCounter(TIM4,0);//计数器清空          				//计数器清空
				if(USART2_RX_STA==0) 				//使能定时器4的中断 
				{
					TIM_Cmd(TIM4,ENABLE);//使能定时器4
				}
				USART2_RX_BUF[USART2_RX_STA++]=res;	//记录接收到的值	 
			}else 
			{
				USART2_RX_STA|=1<<15;				//强制标记接收完成
			} 
		}
	}  				 											 
}   


u32 AS608Addr = 0XFFFFFFFF; //默认

//初始化PA6为下拉输入		    
//读摸出感应状态(触摸感应时输出高电平信号)


void TIM4_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//是更新中断
	{	 			   
		USART2_RX_STA|=1<<15;	//标记接收完成
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIM4更新中断标志    
		TIM_Cmd(TIM4, DISABLE);  //关闭TIM4 
	}	    
}


void TIM4_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//TIM4时钟使能    
	
	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断
	
	TIM_Cmd(TIM4,ENABLE);//开启定时器4
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}




void AS608_uart(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	

	USART_InitStructure.USART_BaudRate=57600;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2,ENABLE);
	
  TIM4_Int_Init(99,7199);		//10ms中断
	USART2_RX_STA=0;		//清零
	TIM_Cmd(TIM4,DISABLE);			//关闭定时器4
	
}
void AS608_NVIC(void)
{     
  
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	

}

void AS608_Init(void)
{
		AS608_uart();
		AS608_NVIC();
                //使能串口1 
}
u8 AS608mode=0;
void AS608_Key_scanf(void)
{
		if(set==1)
		
		{
			while(set);
				AS608mode++;
			if(AS608mode==4)
				AS608mode=0;
		}
		if(AS608mode==2)
		{
		    OLED_Fill(0x00);
        OLED_P8x16Str(0,0,"ADD FR!");
			 delay_ms(1000);
			 delay_ms(500);
			Add_FR();
		}
		if(AS608mode==3)
		{
		    OLED_Fill(0x00);
        OLED_P8x16Str(0,0,"DEL FR!");
			  delay_ms(1000);
			  delay_ms(500);
		  	Del_FR();
		}
				if(AS608mode==2)
		{
		    OLED_Fill(0x00);
       // OLED_P8x16Str(0,0,"press FR!");
			  delay_ms(1000);
			  delay_ms(500);
		  	press_FR();
		}

}


int FR_ID=0;
int Key_get(void)
{
	
	while(enter==0)
	{	
		  if(down==1)
			{
				while(down);
			if(FR_ID==0)
				FR_ID=299;
			else FR_ID--;
			}
			if(up==1)
			{while(up);
			if(FR_ID==300)
				FR_ID=0;
			else FR_ID++;
			}
			OLED_P8x16Str(0,4,"    ");
			Dis_Num(6,0,FR_ID,3);
			
	}
   return FR_ID;
}
//录指纹
void Add_FR(void)
{
	u8 i=0,ensure ,processnum=0;
	u16 ID;
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
        OLED_Fill(0x00);
        OLED_P8x16Str(0,0,"Pleas touch!");
				//LCD_Fill(0,100,lcddev.width,160,WHITE);
				//Show_Str_Mid(0,100,"Pleas touch finger!",16,240);//请按手指
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer1);//生成特征
					if(ensure==0x00)
					{
						OLED_Fill(0x00);
            OLED_P8x16Str(0,0,"Finger correct");
						//LCD_Fill(0,120,lcddev.width,160,WHITE);
						//Show_Str_Mid(0,120,"Fingerprint correct",16,240);//指纹正确
						i=0;
						processnum=1;//跳到第二步						
					}//else ShowErrMessage(ensure);				
				}//else ShowErrMessage(ensure);						
				break;
			
			case 1:
				i++;
						OLED_Fill(0x00);
            OLED_P8x16Str(0,0,"Pleas tagain!");			  
				//LCD_Fill(0,100,lcddev.width,160,WHITE);
				//Show_Str_Mid(0,100,"Pleas touch finger again!",16,240);//再按一次手指
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer2);//生成特征			
					if(ensure==0x00)
					{
						OLED_Fill(0x00);
            OLED_P8x16Str(0,0,"Finger correct");			
						//LCD_Fill(0,120,lcddev.width,160,WHITE);
						//Show_Str_Mid(0,120,"Fingerprint correct",16,240);//指纹正确
						i=0;
						processnum=2;//跳到第三步
					}//else //ShowErrMessage(ensure);	
				}//else //ShowErrMessage(ensure);		
				break;

			case 2:
						OLED_Fill(0x00);
            OLED_P8x16Str(0,0,"Compare twice");							
				//LCD_Fill(0,100,lcddev.width,160,WHITE);
				//Show_Str_Mid(0,100,"Compare twice fingerprint",16,240);//对比两次指纹
				ensure=PS_Match();
				if(ensure==0x00) 
				{
						OLED_Fill(0x00);
            OLED_P8x16Str(0,0,"Twice are same");				
					//LCD_Fill(0,120,lcddev.width,160,WHITE);
					//Show_Str_Mid(0,120,"Twice fingerprint are same",16,240);//两次指纹是一样的
					processnum=3;//跳到第四步
				}
				else 
				{
						OLED_Fill(0x00);
            OLED_P8x16Str(0,0,"pleas again!");								
					//LCD_Fill(0,100,lcddev.width,160,WHITE);
					//Show_Str_Mid(0,100,"Compare fail,pleas touch again!",16,240);//对比失败，请重新按手指
					//ShowErrMessage(ensure);
					i=0;
					processnum=0;//跳回第一步		
				}
				delay_ms(1000);
				break;

			case 3:
				OLED_Fill(0x00);
        OLED_P8x16Str(0,0,"Generate  print!");		
				//LCD_Fill(0,100,lcddev.width,160,WHITE);
				//Show_Str_Mid(0,100,"Generate fingerprint template",16,240);//产生一个指纹模板
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
				OLED_Fill(0x00);
        OLED_P8x16Str(0,0,"finger success!");							
					//LCD_Fill(0,120,lcddev.width,160,WHITE);
					//Show_Str_Mid(0,120,"Generate fingerprint success",16,240);//生成指纹模板成功
					processnum=4;//跳到第五步
				}else {processnum=0;/*ShowErrMessage(ensure);*/}
				delay_ms(1000);
				break;
				
			case 4:	
				OLED_Fill(0x00);
        OLED_P8x16Str(0,0,"Intput ID!");
			  OLED_P8x16Str(0,2,"save with ENT!!");
				//LCD_Fill(0,100,lcddev.width,160,WHITE);
				//Show_Str_Mid(0,100,"Intput ID and save with ENTER!",16,240);//输入ID并按“Enter”保存
				//Show_Str_Mid(0,120,"0=< ID <=299",16,240);	
				do
					ID=Key_get();//jianpan
				while(!(ID<300));//输入DI必须小于300
				ensure=PS_StoreChar(CharBuffer2,ID);//储存模板
				if(ensure==0x00) 
				{	
				OLED_Fill(0x00);
        OLED_P8x16Str(0,0,"Add finger");
			  OLED_P8x16Str(0,2,"success!!!!");
					//LCD_Fill(0,100,lcddev.width,160,WHITE);					
					//Show_Str_Mid(0,120,"Add fingerprint success!!!",16,240);//添加指纹成功
//					PS_ValidTempleteNum(&ValidN);//读库指纹个数
//				  OLED_Fill(0x00);
//					OLED_P8x16Str(0,0,"shengyu:");
//			  //OLED_P8x16Str(0,2,"success!!!!");
//					Dis_Num(4,0,AS608Para.PS_max-ValidN,3);
					//LCD_ShowNum(80,80,AS608Para.PS_max-ValidN,3,16);//显示剩余指纹个数
					delay_ms(1500);//延时后清除显示	
					//LCD_Fill(0,100,240,160,WHITE);
					FR_ID=0;
					return ;
				}else {processnum=0;//ShowErrMessage(ensure);
					}					
				break;				
		}
		delay_ms(800);
		if(i==5)//超过5次没有按手指则退出
		{
				OLED_Fill(0x00);
       
			//LCD_Fill(0,100,lcddev.width,160,WHITE);
			break;	
		}				
	}
}
extern u8 kaisuoflag;
SearchResult seach;
//刷指纹
void press_FR(void)
{
	
	u8 ensure;
	ensure=PS_GetImage();
	if(ensure==0x00)//获取图像成功 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //生成特征成功
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//搜索成功
			{			
				  OLED_Fill(0x00);
					OLED_P8x16Str(0,0,"Search success");		
         // kaisuoflag=1;				
				//LCD_Fill(0,100,lcddev.width,160,WHITE);
				//Show_Str_Mid(0,100,"Search fingerprint success",16,240);//搜索指纹成功				
				//str=mymalloc(50);
				OLED_P8x16Str(0,2,"ID:");
				Dis_Num(2,4,seach.pageID,3);
				OLED_P8x16Str(0,4,"score:");
				Dis_Num(4,7,seach.mathscore,3);
				//sprintf(str,"Match ID:%d  Match score:%d",seach.pageID,seach.mathscore);//显示匹配指纹的ID和分数
				//Show_Str_Mid(0,140,(u8*)str,16,240);
			//	myfree(str);
				delay_ms(1000);
				kaisuoflag=1;
				//return 1;
			}
			//else 
			//	ShowErrMessage(ensure);					
	  }
		//else
			//ShowErrMessage(ensure);
	 delay_ms(1000);//延时后清除显示
		
	 //LCD_Fill(0,100,lcddev.width,160,WHITE);
	}
		
}
extern u8 setmode;
//删除指纹
void Del_FR(void)
{
	u8  ensure;
	u16 num;
	 OLED_Fill(0x00);
	 OLED_P8x16Str(0,0,"Delete finger");
	//LCD_Fill(0,100,lcddev.width,160,WHITE);
	//Show_Str_Mid(0,100,"Delete fingerprint",16,240);//显示删除指纹
	//Show_Str_Mid(0,120,"Input ID and touch Enter!",16,240);//显示输入ID并按下“Enter”
	//Show_Str_Mid(0,140,"0=< ID <=299",16,240);
	delay_ms(50);
	//AS608_load_keyboard(0,170,(u8**)kbd_delFR);
	num=Key_get();//获取返回的数值

		
 if(num==299)
		ensure=PS_Empty();//清空指纹库
	else 
		ensure=PS_DeletChar(num,1);//删除单个指纹
	if(ensure==0)
	{
			 OLED_Fill(0x00);
	    OLED_P8x16Str(0,0,"Delete success");
		setmode=0;
	//	LCD_Fill(0,120,lcddev.width,160,WHITE);
	//	Show_Str_Mid(0,140,"Delete fingerprint success!!!",16,240);//删除指纹成功		
	}
  //else
		//ShowErrMessage(ensure);	
	delay_ms(1500);//延时后清除显示
	PS_ValidTempleteNum(&ValidN);//读库指纹个数
	 OLED_Fill(0x00);
		OLED_P8x16Str(0,0,"shengyu:");
			  //OLED_P8x16Str(0,2,"success!!!!");
		Dis_Num(4,0,AS608Para.PS_max-ValidN,3);
	//LCD_ShowNum(80,80,AS608Para.PS_max-ValidN,3,16);//显示剩余指纹个数

	//LCD_Fill(0,100,lcddev.width,160,WHITE);
	delay_ms(50);
	//AS608_load_keyboard(0,170,(u8**)kbd_menu);
}

void PS_StaGPIO_Init(void)
{   
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟
  //初始化读状态引脚GPIOA
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//输入下拉模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO	
}
//串口发送一个字节
static void MYUSART_SendData(u8 data)
{
	while((USART2->SR&0X40)==0); 
	USART2->DR = data;
}
//发送包头
static void SendHead(void)
{
	MYUSART_SendData(0xEF);
	MYUSART_SendData(0x01);
}
//发送地址
static void SendAddr(void)
{
	MYUSART_SendData(AS608Addr>>24);
	MYUSART_SendData(AS608Addr>>16);
	MYUSART_SendData(AS608Addr>>8);
	MYUSART_SendData(AS608Addr);
}
//发送包标识,
static void SendFlag(u8 flag)
{
	MYUSART_SendData(flag);
}
//发送包长度
static void SendLength(int length)
{
	MYUSART_SendData(length>>8);
	MYUSART_SendData(length);
}
//发送指令码
static void Sendcmd(u8 cmd)
{
	MYUSART_SendData(cmd);
}
//发送校验和
static void SendCheck(u16 check)
{
	MYUSART_SendData(check>>8);
	MYUSART_SendData(check);
}
//判断中断接收的数组有没有应答包
//waittime为等待中断接收数据的时间（单位1ms）
//返回值：数据包首地址
static u8 *JudgeStr(u16 waittime)
{
	char *data;
	u8 str[8];
	str[0]=0xef;str[1]=0x01;str[2]=AS608Addr>>24;
	str[3]=AS608Addr>>16;str[4]=AS608Addr>>8;
	str[5]=AS608Addr;str[6]=0x07;str[7]='\0';
	USART2_RX_STA=0;
	while(--waittime)
	{
		delay_ms(1);
		if(USART2_RX_STA&0X8000)//接收到一次数据
		{
			USART2_RX_STA=0;
			data=strstr((const char*)USART2_RX_BUF,(const char*)str);
			if(data)
				return (u8*)data;	
		}
	}
	return 0;
}
//录入图像 PS_GetImage
//功能:探测手指，探测到后录入指纹图像存于ImageBuffer。 
//模块返回确认字
u8 PS_GetImage(void)
{
  u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x01);
  temp =  0x01+0x03+0x01;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//生成特征 PS_GenChar
//功能:将ImageBuffer中的原始图像生成指纹特征文件存于CharBuffer1或CharBuffer2			 
//参数:BufferID --> charBuffer1:0x01	charBuffer1:0x02												
//模块返回确认字
u8 PS_GenChar(u8 BufferID)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x04);
	Sendcmd(0x02);
	MYUSART_SendData(BufferID);
	temp = 0x01+0x04+0x02+BufferID;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//精确比对两枚指纹特征 PS_Match
//功能:精确比对CharBuffer1 与CharBuffer2 中的特征文件 
//模块返回确认字
u8 PS_Match(void)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x03);
	temp = 0x01+0x03+0x03;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//搜索指纹 PS_Search
//功能:以CharBuffer1或CharBuffer2中的特征文件搜索整个或部分指纹库.若搜索到，则返回页码。			
//参数:  BufferID @ref CharBuffer1	CharBuffer2
//说明:  模块返回确认字，页码（相配指纹模板）
u8 PS_Search(u8 BufferID,u16 StartPage,u16 PageNum,SearchResult *p)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x08);
	Sendcmd(0x04);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(StartPage>>8);
	MYUSART_SendData(StartPage);
	MYUSART_SendData(PageNum>>8);
	MYUSART_SendData(PageNum);
	temp = 0x01+0x08+0x04+BufferID
	+(StartPage>>8)+(u8)StartPage
	+(PageNum>>8)+(u8)PageNum;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
	{
		ensure = data[9];
		p->pageID   =(data[10]<<8)+data[11];
		p->mathscore=(data[12]<<8)+data[13];	
	}
	else
		ensure = 0xff;
	return ensure;	
}
//合并特征（生成模板）PS_RegModel
//功能:将CharBuffer1与CharBuffer2中的特征文件合并生成 模板,结果存于CharBuffer1与CharBuffer2	
//说明:  模块返回确认字
u8 PS_RegModel(void)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x05);
	temp = 0x01+0x03+0x05;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;		
}
//储存模板 PS_StoreChar
//功能:将 CharBuffer1 或 CharBuffer2 中的模板文件存到 PageID 号flash数据库位置。			
//参数:  BufferID @ref charBuffer1:0x01	charBuffer1:0x02
//       PageID（指纹库位置号）
//说明:  模块返回确认字
u8 PS_StoreChar(u8 BufferID,u16 PageID)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x06);
	Sendcmd(0x06);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(PageID>>8);
	MYUSART_SendData(PageID);
	temp = 0x01+0x06+0x06+BufferID
	+(PageID>>8)+(u8)PageID;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;	
}
//删除模板 PS_DeletChar
//功能:  删除flash数据库中指定ID号开始的N个指纹模板
//参数:  PageID(指纹库模板号)，N删除的模板个数。
//说明:  模块返回确认字
u8 PS_DeletChar(u16 PageID,u16 N)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x07);
	Sendcmd(0x0C);
	MYUSART_SendData(PageID>>8);
	MYUSART_SendData(PageID);
	MYUSART_SendData(N>>8);
	MYUSART_SendData(N);
	temp = 0x01+0x07+0x0C
	+(PageID>>8)+(u8)PageID
	+(N>>8)+(u8)N;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//清空指纹库 PS_Empty
//功能:  删除flash数据库中所有指纹模板
//参数:  无
//说明:  模块返回确认字
u8 PS_Empty(void)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x0D);
	temp = 0x01+0x03+0x0D;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//写系统寄存器 PS_WriteReg
//功能:  写模块寄存器
//参数:  寄存器序号RegNum:4\5\6
//说明:  模块返回确认字
u8 PS_WriteReg(u8 RegNum,u8 DATA)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x05);
	Sendcmd(0x0E);
	MYUSART_SendData(RegNum);
	MYUSART_SendData(DATA);
	temp = RegNum+DATA+0x01+0x05+0x0E;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	if(ensure==0)
		printf("\r\n设置参数成功！");
	else
		printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}
//读系统基本参数 PS_ReadSysPara
//功能:  读取模块的基本参数（波特率，包大小等)
//参数:  无
//说明:  模块返回确认字 + 基本参数（16bytes）
u8 PS_ReadSysPara(SysPara *p)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x0F);
	temp = 0x01+0x03+0x0F;
	SendCheck(temp);
	data=JudgeStr(1000);
	if(data)
	{
		ensure = data[9];
		p->PS_max = (data[14]<<8)+data[15];
		p->PS_level = data[17];
		p->PS_addr=(data[18]<<24)+(data[19]<<16)+(data[20]<<8)+data[21];
		p->PS_size = data[23];
		p->PS_N = data[25];
	}		
	else
		ensure=0xff;
	if(ensure==0x00)
	{
		printf("\r\n模块最大指纹容量=%d",p->PS_max);
		printf("\r\n对比等级=%d",p->PS_level);
		printf("\r\n地址=%x",p->PS_addr);
		printf("\r\n波特率=%d",p->PS_N*9600);
	}
	else 
			printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}
//设置模块地址 PS_SetAddr
//功能:  设置模块地址
//参数:  PS_addr
//说明:  模块返回确认字
u8 PS_SetAddr(u32 PS_addr)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x07);
	Sendcmd(0x15);
	MYUSART_SendData(PS_addr>>24);
	MYUSART_SendData(PS_addr>>16);
	MYUSART_SendData(PS_addr>>8);
	MYUSART_SendData(PS_addr);
	temp = 0x01+0x07+0x15
	+(u8)(PS_addr>>24)+(u8)(PS_addr>>16)
	+(u8)(PS_addr>>8) +(u8)PS_addr;				
	SendCheck(temp);
	AS608Addr=PS_addr;//发送完指令，更换地址
  data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;	
		AS608Addr = PS_addr;
	if(ensure==0x00)
		printf("\r\n设置地址成功！");
	else
		printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}
//功能： 模块内部为用户开辟了256bytes的FLASH空间用于存用户记事本,
//	该记事本逻辑上被分成 16 个页。
//参数:  NotePageNum(0~15),Byte32(要写入内容，32个字节)
//说明:  模块返回确认字
u8 PS_WriteNotepad(u8 NotePageNum,u8 *Byte32)
{
	u16 temp;
  u8  ensure,i;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(36);
	Sendcmd(0x18);
	MYUSART_SendData(NotePageNum);
	for(i=0;i<32;i++)
	 {
		 MYUSART_SendData(Byte32[i]);
		 temp += Byte32[i];
	 }
  temp =0x01+36+0x18+NotePageNum+temp;
	SendCheck(temp);
  data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}
//读记事PS_ReadNotepad
//功能：  读取FLASH用户区的128bytes数据
//参数:  NotePageNum(0~15)
//说明:  模块返回确认字+用户信息
u8 PS_ReadNotepad(u8 NotePageNum,u8 *Byte32)
{
	u16 temp;
  u8  ensure,i;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x04);
	Sendcmd(0x19);
	MYUSART_SendData(NotePageNum);
	temp = 0x01+0x04+0x19+NotePageNum;
	SendCheck(temp);
  data=JudgeStr(2000);
	if(data)
	{
		ensure=data[9];
		for(i=0;i<32;i++)
		{
			Byte32[i]=data[10+i];
		}
	}
	else
		ensure=0xff;
	return ensure;
}
//高速搜索PS_HighSpeedSearch
//功能：以 CharBuffer1或CharBuffer2中的特征文件高速搜索整个或部分指纹库。
//		  若搜索到，则返回页码,该指令对于的确存在于指纹库中 ，且登录时质量
//		  很好的指纹，会很快给出搜索结果。
//参数:  BufferID， StartPage(起始页)，PageNum（页数）
//说明:  模块返回确认字+页码（相配指纹模板）
u8 PS_HighSpeedSearch(u8 BufferID,u16 StartPage,u16 PageNum,SearchResult *p)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x08);
	Sendcmd(0x1b);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(StartPage>>8);
	MYUSART_SendData(StartPage);
	MYUSART_SendData(PageNum>>8);
	MYUSART_SendData(PageNum);
	temp = 0x01+0x08+0x1b+BufferID
	+(StartPage>>8)+(u8)StartPage
	+(PageNum>>8)+(u8)PageNum;
	SendCheck(temp);
	data=JudgeStr(2000);
 	if(data)
	{
		ensure=data[9];
		p->pageID 	=(data[10]<<8) +data[11];
		p->mathscore=(data[12]<<8) +data[13];
	}
	else
		ensure=0xff;
	return ensure;
}
//读有效模板个数 PS_ValidTempleteNum
//功能：读有效模板个数
//参数: 无
//说明: 模块返回确认字+有效模板个数ValidN
u8 PS_ValidTempleteNum(u16 *ValidN)
{
	u16 temp;
  u8  ensure;
	u8  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//命令包标识
	SendLength(0x03);
	Sendcmd(0x1d);
	temp = 0x01+0x03+0x1d;
	SendCheck(temp);
  data=JudgeStr(2000);
	if(data)
	{
		ensure=data[9];
		*ValidN = (data[10]<<8) +data[11];
	}		
	else
		ensure=0xff;
	
	if(ensure==0x00)
	{
		printf("\r\n有效指纹个数=%d",(data[10]<<8)+data[11]);
	}
	else
		printf("\r\n%s",EnsureMessage(ensure));
	return ensure;
}
//与AS608握手 PS_HandShake
//参数: PS_Addr地址指针
//说明: 模块返新地址（正确地址）	
u8 PS_HandShake(u32 *PS_Addr)
{
	SendHead();
	SendAddr();
	MYUSART_SendData(0X01);
	MYUSART_SendData(0X00);
	MYUSART_SendData(0X00);	
	delay_ms(200);
	if(USART2_RX_STA&0X8000)//接收到数据
	{		
		if(//判断是不是模块返回的应答包				
					USART2_RX_BUF[0]==0XEF
				&&USART2_RX_BUF[1]==0X01
				&&USART2_RX_BUF[6]==0X07
			)
			{
				*PS_Addr=(USART2_RX_BUF[2]<<24) + (USART2_RX_BUF[3]<<16)
								+(USART2_RX_BUF[4]<<8) + (USART2_RX_BUF[5]);
				USART2_RX_STA=0;
				return 0;
			}
		USART2_RX_STA=0;					
	}
	return 1;		
}
//模块应答包确认码信息解析
//功能：解析确认码错误信息返回信息
//参数: ensure
const char *EnsureMessage(u8 ensure) 
{
	const char *p;
	switch(ensure)
	{
		case  0x00:
			p="OK";break;		
		case  0x01:
			p="数据包接收错误";break;
		case  0x02:
			p="传感器上没有手指";break;
		case  0x03:
			p="录入指纹图像失败";break;
		case  0x04:
			p="指纹图像太干、太淡而生不成特征";break;
		case  0x05:
			p="指纹图像太湿、太糊而生不成特征";break;
		case  0x06:
			p="指纹图像太乱而生不成特征";break;
		case  0x07:
			p="指纹图像正常，但特征点太少（或面积太小）而生不成特征";break;
		case  0x08:
			p="指纹不匹配";break;
		case  0x09:
			p="没搜索到指纹";break;
		case  0x0a:
			p="特征合并失败";break;
		case  0x0b:
			p="访问指纹库时地址序号超出指纹库范围";
		case  0x10:
			p="删除模板失败";break;
		case  0x11:
			p="清空指纹库失败";break;	
		case  0x15:
			p="缓冲区内没有有效原始图而生不成图像";break;
		case  0x18:
			p="读写 FLASH 出错";break;
		case  0x19:
			p="未定义错误";break;
		case  0x1a:
			p="无效寄存器号";break;
		case  0x1b:
			p="寄存器设定内容错误";break;
		case  0x1c:
			p="记事本页码指定错误";break;
		case  0x1f:
			p="指纹库满";break;
		case  0x20:
			p="地址错误";break;
		default :
			p="模块返回确认码有误";break;
	}
 return p;	
}




