#include "stm32f10x.h"
#include "motor_step5.h"
#include "SysTickDelay.h"

void TIM1_PWM_Init(u16 arr,u16 psc)
{  
	 GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	TIM_BDTRInitTypeDef      TIM_BDTRInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
	                                                                     	
#if 0
   //���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


   //���ø�����Ϊ�����������,���TIM1 CH1N��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA,&GPIO_InitStructure);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM_CounterMode_CenterAligned3;//TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

 #if 1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
		TIM_OCInitStructure.TIM_OCNPolarity= TIM_OCPolarity_High; //TIM_OCPolarity_Low
		TIM_OCInitStructure.TIM_OutputNState= TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;//TIM_OCIdleState_Reset
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;//TIM_OCNIdleState_Reset //fail to set high

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx



  TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE �����ʹ��	

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	 
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
#else //fail to control
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
TIM_OCInitStructure.TIM_Pulse = 0;//clay 0>10
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;     
TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

TIM_OC1Init(TIM1, &TIM_OCInitStructure);
TIM_OC2Init(TIM1, &TIM_OCInitStructure);
TIM_OC3Init(TIM1, &TIM_OCInitStructure);

TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);

TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
TIM_BDTRInitStructure.TIM_DeadTime = 50;//clay
TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;               
TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;
TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

TIM_Cmd(TIM1, ENABLE);
 TIM_CCPreloadControl(TIM1,ENABLE);
TIM_CtrlPWMOutputs(TIM1, ENABLE);
#endif


   
}


void TIM4_Configuration(void)
	{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	//????PWM??=72000000/900=80Khz
	/* ---------------------------------------------------------------
	TIM4CLK ?PCLK1=36MHz
	TIM4 Configuration: generate 1 PWM signals :
    TIM4CLK = 36 MHz, Prescaler = 0x0, TIM4 counter clock = 36 MHz
    TIM4 ARR Register = 900 => TIM4 Frequency = TIM4 counter clock/(ARR + 1)
    TIM4 Frequency = 36 KHz.
    TIM4 Channel2 duty cycle = (TIM4_CCR2/ TIM4_ARR)* 100 
	TIM4CLK = 36 MHz, Prescaler = 0, TIM4 counter clock = 36MHz
	--------------------------------------------------------------- */
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 899; //???????????????????????????	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =0; //??????TIMx???????????  ???
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //??????:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //??TIM_TimeBaseInitStruct?????????TIMx???????
	
	/* Output Compare Active Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //???????:TIM????????2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
	TIM_OCInitStructure.TIM_Pulse = 0; //????????????????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //????:TIM???????
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;//clay TIM_OCIdleState_Reset
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //??TIM_OCInitStruct???????????TIMx
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //??TIM_OCInitStruct???????????TIMx
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //??TIM_OCInitStruct???????????TIMx
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //??TIM_OCInitStruct???????????TIMx
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //??TIMx?CCR2????????
	
	TIM_ARRPreloadConfig(TIM4, ENABLE); //??TIMx?ARR????????
	
	/* TIM4 enable counter */
	TIM_Cmd(TIM4, ENABLE);  //??TIMx??
	}


void TIM66_Configuration(uint16_t interval)
	{
		TIM_TimeBaseInitTypeDef  TIM5_TimeBaseStructure;
//		TIM_OCInitTypeDef  TIM_OCInitStructure;
		uint16_t peroid,scaler;
	/* TIM5 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
//					peroid = interval ;
//		bak_peroid = peroid;
//	scaler = 0;
		//--FOR TIMER INT------
	if ((interval > 999)){ // > 100ms
			scaler = 7200;//ms
			peroid = interval / 100 ;
		}
		else if ((interval > 99)&&(interval < 1000)){ // 100ms~10ms
			scaler = 720; //100us
			peroid = interval /10;
		}
		else { //< 1000 us
			scaler = 72;//10us
			peroid = interval ;
		}


	/* ---------------------------------------------------------------
	TIM5CLK ��PCLK1=36MHz
	TIM5CLK = 36 MHz, Prescaler = 7200, TIM5 counter clock = 5K,���ı�һ��Ϊ5K,���ھ�Ϊ10K
	--------------------------------------------------------------- */
	/* Time base configuration */
	TIM5_TimeBaseStructure.TIM_Period = peroid;//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM5_TimeBaseStructure.TIM_Prescaler =(scaler-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM5_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM5_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM5_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	/* Enables the Update event for TIM5 */
	//TIM_UpdateDisableConfig(TIM5,ENABLE); 	//ʹ�� TIM5 �����¼� 
	
	/* TIM IT enable */
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM5, //TIM5
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);
//	
	/* TIM5 enable counter */
	//TIM_Cmd(TIM5, ENABLE);  //ʹ��TIMx����
//	
//	TIM5_TimeBaseStructure.TIM_Period = 2000; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
//	TIM5_TimeBaseStructure.TIM_Prescaler =(1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
//	TIM5_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM5_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM5, &TIM5_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
//	
//	/* Output Compare Active Mode configuration: Channel1 */
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2 TIM_OCMode_PWM2 for ch2
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
//	TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
//	//TIM_OC3Init(TIM5, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
//	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
//	//TIM_OC3PreloadConfig(TIM5,TIM_OCPreload_Enable);
//		
//	TIM_ARRPreloadConfig(TIM5, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
//	
	
	
//	TIM_SetCompare1(TIM5,1000);
	/* TIM5 enable counter */
//	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIMx����
	}

void STEP5_M_timer_init(void){

	NVIC_InitTypeDef NVIC_InitStructure;
		/* Enable the TIM5 for motor0 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3�ж� PWM PA6/PA7
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

}

static bool test_int = FALSE;
void STEP5_int(void){
	//	STEP5_CLK_TOGGLE();
	if(test_int){
		STEP5_CLK_H();
		test_int = FALSE;
	}
	else
	{
		test_int = TRUE;
		STEP5_CLK_L();
	}
}


void STEP5_IO_init(){
		GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD, ENABLE);//|RCC_APB2Periph_AFIO
	

		/*GPIOA Configuration: TIM4 channel1*/	  //���ø�����Ϊ�����������,���TIM4 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = CLK5_PIN; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_Out_PP;//GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CLK5_PORT, &GPIO_InitStructure);


	STEP5_CLK_L();//default for rasing edge  	

	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //test
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������//GPIO_Mode_Out_PP--normal 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOA, GPIO_Pin_12);
	
                                                      	

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);


}

void STEP5_init(void){
	STEP5_IO_init();

	STEP5_M_timer_init();
	TIM66_Configuration(50000);
		
	TIM4_Configuration();
	TIM_SetCompare1(TIM4,400);	
	TIM_SetCompare2(TIM4,400);	
	TIM_SetCompare3(TIM4,400);	
	TIM_SetCompare4(TIM4,400);	
	
	TIM1_PWM_Init(899,0);
	TIM_SetCompare1(TIM1,400);	
	TIM_SetCompare2(TIM1,400);	
	TIM_SetCompare3(TIM1,400);	

	TIM_Cmd(TIM5, ENABLE); 
}
