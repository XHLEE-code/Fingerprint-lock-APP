//#include "pwm.h"
//#include "GPIO.h"
////////////////////////////////////////////////////////////////////////////////////	 
////本程序只供学习使用，未经作者许可，不得用于其它任何用途
////ALIENTEK Mini STM32开发板
////PWM  驱动代码			   
////正点原子@ALIENTEK
////技术论坛:www.openedv.com
////修改日期:2010/12/03
////版本：V1.0
////版权所有，盗版必究。
////Copyright(C) 正点原子 2009-2019
////All rights reserved

//////////////////////////////////////////////////////////////////////////////////// 	  

//void TIM4_PWMINPUT_INIT(u16 arr,u16 psc)
//{
//  
//		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//		NVIC_InitTypeDef NVIC_InitStructure;
//		TIM_ICInitTypeDef  TIM4_ICInitStructure;
//		GPIO_InitTypeDef GPIO_InitStructure;
// 
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);     //Open TIM4 clock
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //open gpioB clock
// 
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                               //GPIO??
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOB, &GPIO_InitStructure);
// 
//		TIM_TimeBaseStructure.TIM_Period = arr; //???????????????????????????  
//		TIM_TimeBaseStructure.TIM_Prescaler =psc; //??????TIMx???????????  
//		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //??????:TDTS = Tck_tim
//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????
//		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //??TIM_TimeBaseInitStruct?????????TIMx???????
// 

///*???????*/
//		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;                     
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
// 
//		TIM4_ICInitStructure.TIM_Channel = TIM_Channel_2;                   
//		TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;       
//		TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;   
//		TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; 
//		TIM4_ICInitStructure.TIM_ICFilter = 0x0; 
// 
//		TIM_PWMIConfig(TIM4, &TIM4_ICInitStructure);     //PWM????           
//		TIM_SelectInputTrigger(TIM4, TIM_TS_TI2FP2);     //???????? ,??TIM_TS_TI2FP2???PB7???:??TI1FP1?TI2FP2?????????,??PWM????????TIMx_CH1 /TIMx_CH2???       
//		TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);  //?????????	 ????????????????????????????0
//		TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);                                       
//		TIM_ITConfig(TIM4, TIM_IT_CC2|TIM_IT_Update, ENABLE);          //????
//		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2|TIM_IT_Update); //???????

//		TIM_Cmd(TIM4, ENABLE);    
//}

//static void TIM4_Mode_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_ICInitTypeDef TIM_ICInitStructure;   	

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	
//	//- 正交编码器输入引脚 PB->6   PB->7 -
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);                           

//	//- TIM4编码器模式配置 -
//	TIM_DeInit(TIM4);
//	TIM_TimeBaseStructure.TIM_Period = 0xFF;
//	TIM_TimeBaseStructure.TIM_Prescaler = 0;
//	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              
//                 
//	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//配置编码器模式触发源和极性
//	
//	TIM_ICStructInit(&TIM_ICInitStructure);																																		//配置滤波器
//	TIM_ICInitStructure.TIM_ICFilter = 6;
//	TIM_ICInit(TIM4, &TIM_ICInitStructure);
//		
//	TIM4->CNT = 0;

//	TIM_Cmd(TIM4, ENABLE);   //启动TIM4定时器

// 
//}

//void TIM4_Init(void)
//{
//  TIM4_Mode_Config();
//}

//void TIM2_Int_Init(u16 arr,u16 psc)
//{
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
//	
//	//定时器TIM3初始化
//	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
// 
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

//	//中断优先级NVIC设置
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


//	TIM_Cmd(TIM2, ENABLE);  //使能TIMx					 
//}
////定时器3中断服务程序
//void TIM2_IRQHandler(void)   //TIM3中断
//{	extern uint8_t setmode;
//	
////	float num;
////	/*if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
////		{
////		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
////		
////		if(setmode==0)
////			{
////			Count=TIM_GetCounter(TIM4);
////			OLED_P8x16Str(40,0,"Display");
////			OLED_P8x16Str(0,2,"SPEED:");
////			if(intflag==1)
////		  OLED_P8x16Str(0,4,"FWD");
////			else OLED_P8x16Str(0,4,"REV");
////			OLED_P8x16Str(70,2,"       ");
////			Dis_Float(2,70,Count1*5,1);
////			num=Count2/1000;
////			
////				
////			}
////			
////			/*if(setCount>Count1)
////			{
////				P=(setCount-Count1);
////				
////				TIM_SetCompare1(TIM3,P);
////			}
////			else TIM_SetCompare1(TIM3,0);
////			if(stop==1)
////				TIM_SetCompare1(TIM3,0);
////		Count1=0;
////   }
////*/
//}
//static void TIM3_GPIO_Config(void) 
//{
//  GPIO_InitTypeDef GPIO_InitStructure;

//	/* TIM3 clock enable */
//	//PCLK1经过2倍频后作为TIM3的时钟源等于72MHz
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

//  /* GPIOA and GPIOB clock enable */
//	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 

//  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

//  GPIO_Init(GPIOA, &GPIO_InitStructure);

//  /*GPIOB Configuration: TIM3 channel 3 and 4 as alternate function push-pull */
//  //GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 |GPIO_Pin_1;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//}

///*
// * 函数名：TIM3_Mode_Config
// * 描述  ：配置TIM3输出的PWM信号的模式，如周期、极性、占空比
// * 输入  ：无
// * 输出  ：无
// * 调用  ：内部调用
// */
//static void TIM3_Mode_Config(void)
//{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;

//	/* PWM信号电平跳变值 */
////	u16 CCR1_Val = 0;        
////	u16 CCR2_Val = 0;
////	u16 CCR3_Val = 0;
////	u16 CCR4_Val = 0;

///* -----------------------------------------------------------------------
//    TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
//    TIM3CLK = 72 MHz, Prescaler = 0x0, TIM3 counter clock = 72 MHz
//    TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
//    TIM3 Frequency = 72 KHz.
//    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
//    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
//    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
//    TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%
//  ----------------------------------------------------------------------- */

//  /* Time base configuration */		 
//  TIM_TimeBaseStructure.TIM_Period = 999;       //当定时器从0计数到999，即为1000次，为一个定时周期
//  TIM_TimeBaseStructure.TIM_Prescaler =71;	    //设置预分频：不预分频，即为72MHz
//  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式

//  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

//  /* PWM1 Mode configuration: Channel1 */
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
// // TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平

//    
// TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //使能通道1

//  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

// 
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  

//  TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //使能通道2

//  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  

//  TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //使能通道3

//  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

//  
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  

//  TIM_OC4Init(TIM3, &TIM_OCInitStructure);	//使能通道4

//  /* PWM1 Mode configuration: Channel4 */
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  


//  TIM_ARRPreloadConfig(TIM3, ENABLE);			 // 使能TIM3重载寄存器ARR

//  /* TIM3 enable counter */
//  TIM_Cmd(TIM3, ENABLE);                   //使能定时器3	
//}

///*
// * 函数名：TIM3_PWM_Init
// * 描述  ：TIM3 输出PWM信号初始化，只要调用这个函数
// *         TIM3的四个通道就会有PWM信号输出
// * 输入  ：无
// * 输出  ：无
// * 调用  ：外部调用
// */
//void TIM3_PWM_Init(void)
//{
//	TIM3_GPIO_Config();
//	TIM3_Mode_Config();	
//}

