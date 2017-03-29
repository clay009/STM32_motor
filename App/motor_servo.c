/*
for AC sevor motor
*/
#include "stm32f10x.h"
#include "motor_servo.h"
#include "eval.h" //for EXI
#include "msg_handler.h"

static char SERVO_M_phase = 1;
static bool SERVO_M_clockwise_direction = FALSE  ; // TRUE: clockwise, FALSE: anti clockwise


void SERVO_M_IO_init(void){
	  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB| RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = CH1_PIN;//CH1 PC6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(CH1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CH1N_PIN;//CH1N  PA7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(CH1N_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CH2_PIN;//CH2 PC7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(CH2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CH2N_PIN;//CH2N  PB0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(CH2N_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CH3_PIN;//CH3 PC8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(CH3_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CH3N_PIN;//CH3N  PB1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(CH3N_PORT, &GPIO_InitStructure);
	
	//STOP pin
	GPIO_InitStructure.GPIO_Pin = STOP_PIN;//stop  pb8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(STOP_PORT, &GPIO_InitStructure);
	//fault out put //PB9 input
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//CH3N  PB1
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//	//test key0 PC1
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//CH3N  PB1
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOC, &GPIO_InitStructure);
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_13;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOC, &GPIO_InitStructure); 
	//fault output int
	STM_EVAL_PBInit(Button_KEY0, Mode_EXTI);	//for simulter 
//	STM_EVAL_PBInit(Button_KEY1, Mode_EXTI);	
	
}


void SERVO_M_set_timer_int(void){
	NVIC_InitTypeDef NVIC_InitStructure;
		/* Enable the TIM5 for motor0 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

void SERVO_M_anti_clockwise_run_one_step(void){
	
	switch(SERVO_M_phase){
		case 0: //phase 1
			CH2_ON();
			CH2N_OFF();
			CH2_ON();
			CH2N_OFF();
			CH3_ON();
			CH3N_OFF();
			break;
		case 1: //phase 2
			CH2_ON();
			CH2N_OFF();
			CH1_ON();
			CH1N_OFF();
			CH3_OFF();
			CH3N_ON();
			break;
		case 2: //phase 3
			CH2_ON();
			CH2N_OFF();
			CH1_OFF();
			CH1N_ON();
			CH3_OFF();
			CH3N_ON();
			break;
		case 3: //phase 4
			CH2_OFF();
			CH2N_ON();
			CH1_OFF();
			CH1N_ON();
			CH3_OFF();
			CH3N_ON();
			break;
		case 4: //phase 5
			CH2_OFF();
			CH2N_ON();
			CH1_OFF();
			CH1N_ON();
			CH3_ON();
			CH3N_OFF();
			break;
		case 5: //phase 6
			CH2_OFF();
			CH2N_ON();
			CH1_ON();
			CH1N_OFF();
			CH3_ON();
			CH3N_OFF();
			break;
		default:
			SERVO_M_phase = 5; //error state do nothing
			break;
	};
	SERVO_M_phase=(SERVO_M_phase+1)%6;
}

void SERVO_M_clockwise_run_one_step(void){
	
	switch(SERVO_M_phase){
		case 0: //phase 1
			CH1_ON();
			CH1N_OFF();
			CH2_ON();
			CH2N_OFF();
			CH3_ON();
			CH3N_OFF();
			break;
		case 1: //phase 2
			CH1_ON();
			CH1N_OFF();
			CH2_ON();
			CH2N_OFF();
			CH3_OFF();
			CH3N_ON();
			break;
		case 2: //phase 3
			CH1_ON();
			CH1N_OFF();
			CH2_OFF();
			CH2N_ON();
			CH3_OFF();
			CH3N_ON();
			break;
		case 3: //phase 4
			CH1_OFF();
			CH1N_ON();
			CH2_OFF();
			CH2N_ON();
			CH3_OFF();
			CH3N_ON();
			break;
		case 4: //phase 5
			CH1_OFF();
			CH1N_ON();
			CH2_OFF();
			CH2N_ON();
			CH3_ON();
			CH3N_OFF();
			break;
		case 5: //phase 6
			CH1_OFF();
			CH1N_ON();
			CH2_ON();
			CH2N_OFF();
			CH3_ON();
			CH3N_OFF();
			break;
		default:
			SERVO_M_phase = 5; //error state do nothing
			break;
	};
	SERVO_M_phase=(SERVO_M_phase+1)%6;
}

void SERVO_M_run_step(void){
	if (SERVO_M_clockwise_direction)  
		SERVO_M_clockwise_run_one_step();
	else 
		SERVO_M_anti_clockwise_run_one_step();
}

void SERVO_M_set_clockwise(bool wise){
	SERVO_M_clockwise_direction = wise;
}

//TIM_TimeBaseInitTypeDef  TIM5_TimeBaseStructure;
void TIM5_Configuration(int interval)
	{
TIM_TimeBaseInitTypeDef  TIM5_TimeBaseStructure;
		uint16_t peroid,scaler;
	/* TIM5 clock enable */
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
	TIM5CLK ��PCLK1=36MHz
	TIM5CLK = 36 MHz, Prescaler = 7200, TIM5 counter clock = 5K,���ı�һ��Ϊ5K,���ھ�Ϊ10K
	--------------------------------------------------------------- */
	/* Time base configuration */
	TIM5_TimeBaseStructure.TIM_Period = peroid; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM5_TimeBaseStructure.TIM_Prescaler =(scaler-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM5_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM5_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM5_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	/* Enables the Update event for TIM5 */
	//TIM_UpdateDisableConfig(TIM5,ENABLE); 	//ʹ�� TIM5 �����¼� 
	
	/* TIM IT enable */
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM5, //TIM2
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);
	
	/* TIM5 enable counter */
	//TIM_Cmd(TIM5, ENABLE);  //ʹ��TIMx����
	}


void MOTOT_stop(void){
	//SERVO_M_phase = 0; not change
	TIM_Cmd(TIM5, DISABLE);
	CH1_OFF();
	CH1N_OFF();
	CH2_OFF();
	CH2N_OFF();
	CH3_OFF();
	CH3N_OFF();
	SERVO_M_STOP();
}
void SERVO_M_start(void){
	SERVO_M_ENABLE();
	TIM_Cmd(TIM5, ENABLE);
}

void SERVO_M_set_step_interval(int us){
	TIM5_Configuration(us);
}

void SERVO_M_init(void){
	SERVO_M_IO_init();
	SERVO_M_set_timer_int();
	MOTOT_stop();
	//TIM5_Configuration();
}

void SERVO_M_fault_out(void){
	SERVO_M_STOP();
	ERR_REPORT_STA =  (SERVO_M_ID << 4 )|ERR_SERVO_FAULT_OUT;
}

