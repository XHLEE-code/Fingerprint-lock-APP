//#include "pwm.h"
//#include "GPIO.h"
////////////////////////////////////////////////////////////////////////////////////	 
////������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
////ALIENTEK Mini STM32������
////PWM  ��������			   
////����ԭ��@ALIENTEK
////������̳:www.openedv.com
////�޸�����:2010/12/03
////�汾��V1.0
////��Ȩ���У�����ؾ���
////Copyright(C) ����ԭ�� 2009-2019
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
//	//- ������������������ PB->6   PB->7 -
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);                           

//	//- TIM4������ģʽ���� -
//	TIM_DeInit(TIM4);
//	TIM_TimeBaseStructure.TIM_Period = 0xFF;
//	TIM_TimeBaseStructure.TIM_Prescaler = 0;
//	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              
//                 
//	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising);	//���ñ�����ģʽ����Դ�ͼ���
//	
//	TIM_ICStructInit(&TIM_ICInitStructure);																																		//�����˲���
//	TIM_ICInitStructure.TIM_ICFilter = 6;
//	TIM_ICInit(TIM4, &TIM_ICInitStructure);
//		
//	TIM4->CNT = 0;

//	TIM_Cmd(TIM4, ENABLE);   //����TIM4��ʱ��

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

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
//	
//	//��ʱ��TIM3��ʼ��
//	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
// 
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

//	//�ж����ȼ�NVIC����
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


//	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx					 
//}
////��ʱ��3�жϷ������
//void TIM2_IRQHandler(void)   //TIM3�ж�
//{	extern uint8_t setmode;
//	
////	float num;
////	/*if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
////		{
////		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
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
//	//PCLK1����2��Ƶ����ΪTIM3��ʱ��Դ����72MHz
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

//  /* GPIOA and GPIOB clock enable */
//	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 

//  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

//  GPIO_Init(GPIOA, &GPIO_InitStructure);

//  /*GPIOB Configuration: TIM3 channel 3 and 4 as alternate function push-pull */
//  //GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 |GPIO_Pin_1;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//}

///*
// * ��������TIM3_Mode_Config
// * ����  ������TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
// * ����  ����
// * ���  ����
// * ����  ���ڲ�����
// */
//static void TIM3_Mode_Config(void)
//{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;

//	/* PWM�źŵ�ƽ����ֵ */
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
//  TIM_TimeBaseStructure.TIM_Period = 999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
//  TIM_TimeBaseStructure.TIM_Prescaler =71;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
//  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ

//  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

//  /* PWM1 Mode configuration: Channel1 */
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
// // TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ

//    
// TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��1

//  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

// 
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  

//  TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //ʹ��ͨ��2

//  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  

//  TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��3

//  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

//  
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  

//  TIM_OC4Init(TIM3, &TIM_OCInitStructure);	//ʹ��ͨ��4

//  /* PWM1 Mode configuration: Channel4 */
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  


//  TIM_ARRPreloadConfig(TIM3, ENABLE);			 // ʹ��TIM3���ؼĴ���ARR

//  /* TIM3 enable counter */
//  TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��3	
//}

///*
// * ��������TIM3_PWM_Init
// * ����  ��TIM3 ���PWM�źų�ʼ����ֻҪ�����������
// *         TIM3���ĸ�ͨ���ͻ���PWM�ź����
// * ����  ����
// * ���  ����
// * ����  ���ⲿ����
// */
//void TIM3_PWM_Init(void)
//{
//	TIM3_GPIO_Config();
//	TIM3_Mode_Config();	
//}

