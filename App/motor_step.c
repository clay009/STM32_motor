#include "stm32f10x.h"
#include "motor_step.h"

/*
//excitation setting //default state at start-up/reset
M3	M2	M1	MODE_exciation
L		L		L		2 PHASE
L		L		H		1-2PHASE
L		H		L		W1-2
L		H		H		2W1-2
H		L		L		4W1-2
H		L		H		8W1-2
H		H		L		16W1-2
H		H		H		32W1-2
*/

void STEP_M_set_excitation(int mode){
	switch(mode){
		case 0:
			STEP_M3_L();
			STEP_M2_L();
			STEP_M1_L();
			break;
		case 1:
			STEP_M3_L();
			STEP_M2_L();
			STEP_M1_H();
			break;
		case 2:
			STEP_M3_L();
			STEP_M2_H();
			STEP_M1_L();
			break;
		case 3:
			STEP_M3_L();
			STEP_M2_H();
			STEP_M1_H();
			break;
		case 4:
			STEP_M3_H();
			STEP_M2_L();
			STEP_M1_L();
			break;
		case 5:
			STEP_M3_H();
			STEP_M2_L();
			STEP_M1_H();
			break;
		case 6:
			STEP_M3_H();
			STEP_M2_H();
			STEP_M1_L();
			break;
		case 7:
			STEP_M3_H();
			STEP_M2_H();
			STEP_M1_H();
			break;
		default:
		break;
	}
}

/*	CLK pin step signal iputt allows advancing excitation step 
VCC == H 
CLK rasing edge : excitation step feed
CLK failing	edge:	excitation step hold
*/
void STEP_M_CLK_toggle(void){
	//GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];		//ȡ������Ĵ�������
	//GPIOA->ODR ^= GPIO_Pin_12;
	CLK_PORT->ODR ^= CLK_PIN;
}

/* CW/CCW 
L: CW  FORWORD  excitation: 1->2->3->4
H: CCCW BACKWOARD	excation: 6->5->4
at clk rasing edge
*/
void STEP_MOT_set_clockwise(bool wise){
	if(wise){
		STEP_CW_H();
	}
	else{
		STEP_CW_L();
	}
}

/*
When the ENABLE pin is set Low, the output is forced OFF and goes to high impedance. However, the internal logic
circuits are operating, so the excitation position proceeds when the CLK is input. Therefore, when ENABLE pin is
returned to High, the output level conforms to the excitation position proceeded by the CLK input.
*/
void STEP_M_set_enable(bool enable){
	if(enable)
		STEP_EN_L();
	else
		STEP_EN_H();
}

/*
FDT 
>3.5v	SLOW DECAY
1.1V~3.1V OR OPEN : MIXED DECAY
<0.8V	FAST DECAY
*/
void STEP_M_DECAY(int fdt){
	if(fdt ==0)
		STEP_FDT_L();
	else
			STEP_FDT_H();
}

void STEP_M_IO_init(){
	GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD, ENABLE);//|RCC_APB2Periph_AFIO
	
  GPIO_InitStructure.GPIO_Pin = EN_PIN;//STEP_EN PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(EN_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = CW_PIN;//STEP_CW PA11 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(CW_PORT, &GPIO_InitStructure);

		/*GPIOA Configuration: TIM3 channel1*/	  //���ø�����Ϊ�����������,���TIM3 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = CLK_PIN; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CLK_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = FDT_PIN;//M1_FDT PC9
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(FDT_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = M1_PIN;////M1_M1 	PC10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(M1_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = M2_PIN;////M1_M2 	PC11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(M2_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = M3_PIN;////M1_M3 	PC12
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(M3_PORT, &GPIO_InitStructure);

	STEP_CLK_H();
	STEP_M3_L();
	STEP_M2_L();
	STEP_M1_L();
	STEP_CW_L();
	STEP_EN_L();
	STEP_FDT_L();
	

}

//static int bak_peroid =100;

void TIM_Configuration(int interval)
	{
		TIM_TimeBaseInitTypeDef  TIM3_TimeBaseStructure;
//		TIM_OCInitTypeDef  TIM_OCInitStructure;
		uint16_t peroid,scaler;
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
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
	TIM3CLK ��PCLK1=36MHz
	TIM3CLK = 36 MHz, Prescaler = 7200, TIM3 counter clock = 5K,���ı�һ��Ϊ5K,���ھ�Ϊ10K
	--------------------------------------------------------------- */
	/* Time base configuration */
	TIM3_TimeBaseStructure.TIM_Period = peroid; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM3_TimeBaseStructure.TIM_Prescaler =(scaler-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM3_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	/* Enables the Update event for TIM3 */
	//TIM_UpdateDisableConfig(TIM3,ENABLE); 	//ʹ�� TIM3 �����¼� 
	
	/* TIM IT enable */
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM3
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);
//	
	/* TIM3 enable counter */
	//TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
//	
//	TIM3_TimeBaseStructure.TIM_Period = 2000; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
//	TIM3_TimeBaseStructure.TIM_Prescaler =(1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
//	TIM3_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
//	
//	/* Output Compare Active Mode configuration: Channel1 */
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2 TIM_OCMode_PWM2 for ch2
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
//	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
//	//TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
//	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
//	//TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
//		
//	TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
//	
	
	
//	TIM_SetCompare1(TIM3,1000);
	/* TIM3 enable counter */
//	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
	}


void STEP_M_timer_init(void){

	NVIC_InitTypeDef NVIC_InitStructure;
		/* Enable the TIM5 for motor0 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж� PWM PA6/PA7
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

}

void STEP_M_set_clock(int us){
	TIM_Configuration(us);
}
void STEP_M_set_peroid(int percent){
	
	//TIM_SetCompare1(TIM3,bak_peroid*percent/100); 
}
void STEP_M_start_run(void){
	TIM_Cmd(TIM3, ENABLE); 
}

void STEP_M_stop_run(void){
	TIM_Cmd(TIM3, DISABLE);
}
void STEP_M_init(void){
	STEP_M_IO_init();
	STEP_M_timer_init();
}

