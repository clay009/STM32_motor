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
	//GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];		//取反输出寄存器数据
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

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = EN_PIN;//STEP_EN PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(EN_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = CW_PIN;//STEP_CW PA11 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(CW_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = CLK_PIN;//STEP_CLK PA12 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
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
	STEP_M3_H();
	STEP_M2_H();
	STEP_M1_H();
	STEP_CW_H();
	STEP_EN_H();
	STEP_FDT_H();
}



void TIM5_Configuration(int interval)
	{
		TIM_TimeBaseInitTypeDef  TIM5_TimeBaseStructure;
		uint16_t peroid,scaler;
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
			
		
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
	TIM3CLK 即PCLK1=36MHz
	TIM3CLK = 36 MHz, Prescaler = 7200, TIM3 counter clock = 5K,即改变一次为5K,周期就为10K
	--------------------------------------------------------------- */
	/* Time base configuration */
	TIM5_TimeBaseStructure.TIM_Period = peroid; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM5_TimeBaseStructure.TIM_Prescaler =(scaler-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM5_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM5_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM5_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	/* Enables the Update event for TIM3 */
	//TIM_UpdateDisableConfig(TIM3,ENABLE); 	//使能 TIM3 更新事件 
	
	/* TIM IT enable */
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM5, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	
	/* TIM3 enable counter */
	//TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
	}


void STEP_M_timer_init(void){

	NVIC_InitTypeDef NVIC_InitStructure;
		/* Enable the TIM5 for motor0 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

}

void STEP_M_set_clock(int us){
	TIM5_Configuration(us);
}
void STEP_M_start_run(void){
	TIM_Cmd(TIM5, ENABLE); 
}

void STEP_M_init(void){
	STEP_M_IO_init();
	STEP_M_timer_init();
}

