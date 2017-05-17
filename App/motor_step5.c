#include "stm32f10x.h"
#include "motor_step5.h"
#include "SysTickDelay.h"

void STEP5_timer_init(void){
	TIM_TimeBaseInitTypeDef  TIM3_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
		
	NVIC_InitTypeDef NVIC_InitStructure;
		/* Enable the TIM5 for motor0 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж� PWM PA6/PA7
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	TIM3_TimeBaseStructure.TIM_Period = 2000; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM3_TimeBaseStructure.TIM_Prescaler =(1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM3_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	/* Output Compare Active Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2 TIM_OCMode_PWM2 for ch2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	//TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	//TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
		
	TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	
	
	TIM_SetCompare1(TIM3,1000);
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����

	TIM_SetCompare1(TIM3,100); 
}

void STEP5_IO_init(){
		GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD, ENABLE);//|RCC_APB2Periph_AFIO
	

		/*GPIOA Configuration: TIM3 channel1*/	  //���ø�����Ϊ�����������,���TIM3 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = CLK5_PIN; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CLK5_PORT, &GPIO_InitStructure);


	STEP5_CLK_L();//default for rasing edge  	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //test
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������//GPIO_Mode_Out_PP--normal 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_12);

}

void STEP5_init(void){
	STEP5_IO_init();
	STEP5_timer_init();

}
