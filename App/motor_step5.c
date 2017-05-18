#include "stm32f10x.h"
#include "motor_step5.h"
#include "SysTickDelay.h"

void TIM1_PWM_Init(u16 arr,u16 psc)
{  
	 GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIO外设时钟使能
	                                                                     	

   //设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

  TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE 主输出使能	

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1预装载使能	 
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
 
   
}

void TIM3_Configuration(void)
	{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	//????PWM??=72000000/900=80Khz
	/* ---------------------------------------------------------------
	TIM3CLK ?PCLK1=36MHz
	TIM3 Configuration: generate 1 PWM signals :
    TIM3CLK = 36 MHz, Prescaler = 0x0, TIM3 counter clock = 36 MHz
    TIM3 ARR Register = 900 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
    TIM3 Frequency = 36 KHz.
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 
	TIM3CLK = 36 MHz, Prescaler = 0, TIM3 counter clock = 36MHz
	--------------------------------------------------------------- */
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 900; //???????????????????????????	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =0; //??????TIMx???????????  ???
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //??????:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //??TIM_TimeBaseInitStruct?????????TIMx???????
	
	/* Output Compare Active Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //???????:TIM????????2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
	TIM_OCInitStructure.TIM_Pulse = 0; //????????????????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //????:TIM???????
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //??TIM_OCInitStruct???????????TIMx
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //??TIMx?CCR2????????
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //??TIMx?ARR????????
	
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);  //??TIMx??
	}


void STEP5_timer_init(void){
	TIM_TimeBaseInitTypeDef  TIM3_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
		
	NVIC_InitTypeDef NVIC_InitStructure;
		/* Enable the TIM5 for motor0 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断 PWM PA6/PA7
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM3_TimeBaseStructure.TIM_Period = 2000; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM3_TimeBaseStructure.TIM_Prescaler =(1); //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM3_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	/* Output Compare Active Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2 TIM_OCMode_PWM2 for ch2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	//TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
	//TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
		
	TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	
	
	TIM_SetCompare1(TIM3,1000);
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设

	//TIM_SetCompare1(TIM3,100); 
}

void STEP5_IO_init(){
		GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD, ENABLE);//|RCC_APB2Periph_AFIO
	

		/*GPIOA Configuration: TIM3 channel1*/	  //设置该引脚为复用输出功能,输出TIM3 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = CLK5_PIN; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CLK5_PORT, &GPIO_InitStructure);


	STEP5_CLK_L();//default for rasing edge  	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //test
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出//GPIO_Mode_Out_PP--normal 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_12);
	
		/*GPIOB Configuration: TIM3 channel2*/	  //????????????,??TIM3 CH2?PWM????
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //??????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void STEP5_init(void){
	STEP5_IO_init();
	//STEP5_timer_init();
	//TIM3_Configuration();
	//TIM_SetCompare2(TIM3,400);	
	TIM1_PWM_Init(899,0);
	TIM_SetCompare1(TIM1,300);	
}
