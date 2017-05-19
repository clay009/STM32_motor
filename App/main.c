/**********************************************
File Name: ALIENTEK MINISTM32 ʵ��7 ��ʱ���ж�ʵ��(��)
Revision:
Corporation:
Microcontroller Corporation:ST
Microcontroller name:STM32F103RBT6 ARM
Compiler: Keil MDK_ARM 4.11
Author: moweidong
E-mail: moweidongsabbit@21cn.com
Date : 2010.12.05
*********************************************/
//���������豸:
//ALIENTEK_MiniSTM32_REVB0������
//JLINK V7
/*********************************************
//��
//CMSIS:V1.20
//STM32F10x_StdPeriph_Driver:V3.1.1
*********************************************/

//������ʹ����printf������LCD�ʹ�����ʾ����,��Щ��Option for Target���������¸���
//Targetѡ��ҳ:����Use MicroLIB(ʹ��΢��)

//LED0:PA8
//LED1:PD2	TIM3_ETR

//ע��:�ڽ���JTAG�Ӻ��������ָ�:
//1.����KEILΪJTAG��SW����ģʽ
//2.�����ϵ�,���°��ϵĸ�λ��������,���KEIL�µ�FLASH���ذ�ť,ʹJTAG��SW���ع��߼����ɽӿں����̷ſ�,
//��Ŀ��������ȡIC���ϵ��ʱ���п���Ȩ,ʹ��δ�������JTAG��SW��Ч��ʱ��ȡ�ɿ���IC������¼

#include "stm32f10x.h"
//#include "GLCD.h"
//#include "USART.h"
#include "eval.h"
#include "SysTickDelay.h"
#include "UART_INTERFACE.h"
#include <stdio.h>
#include "motor_servo.h"
#include "motor_step.h"
#include "motor_step5.h"
#include "msg_handler.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
//clay mask TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void TIM3_Configuration(void);

//u8 USART_RX_BUF[64];     //���ջ���,���64���ֽ�.
////����״̬
////bit7��������ɱ�־
////bit6�����յ�0x0d
////bit5~0�����յ�����Ч�ֽ���Ŀ
//u8 USART_RX_STA=0;       //����״̬���


void GPIO_Configuration(void)
	{
	//GPIO_InitTypeDef GPIO_InitStructure;
				
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure USART1 Rx (PA.10) as input floating */
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//GPIO_Init(GPIOA, &GPIO_InitStructure);#ifdef USE_STM3210C_EVAL

	/* Initialize LEDs and Key Button mounted on STM3210X-EVAL board */       
	STM_EVAL_LEDInit(LED1);
	STM_EVAL_LEDInit(LED2);
	}

//ϵͳ�жϹ���
void NVIC_Configuration(void)
	{ 
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */  
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	//�������ȼ����飺��ռ���ȼ�0λ,�����ȼ�4λ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//�������ȼ����飺��ռ���ȼ�2λ,�����ȼ�2λ
	
	//�����������λ�ú�ƫ��
	#ifdef  VECT_TAB_RAM  
		/* Set the Vector Table base location at 0x20000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 		//������λ��RAM
	#else  /* VECT_TAB_FLASH  */
		/* Set the Vector Table base location at 0x08000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   //������λ��FLASH
	#endif

//	/* Enable the TIM3 for motor0 global Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	
		/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//USART1�ж�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1
	
	}

//����ϵͳʱ��,ʹ�ܸ�����ʱ��
void RCC_Configuration(void)
	{
	SystemInit();	
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA 
	//	|RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC
	//	|RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE
	//	|RCC_APB2Periph_ADC1  | RCC_APB2Periph_AFIO 
	//	|RCC_APB2Periph_SPI1, ENABLE );
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALL ,ENABLE );
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 
	//	|RCC_APB1Periph_USART3|RCC_APB1Periph_TIM2	                           
	//	, ENABLE );
	}
	




//������������
void Init_All_Periph(void)
	{
	
	RCC_Configuration();	
	
	NVIC_Configuration();

	GPIO_Configuration();
	
		//SERVO_M_init();
	//USART1_Configuration();
	//clay USART_Configuration(115200);//115200 9600
	//clay USART_RX_IntteruptEnable(USED_COM_NUMBER);		//�����ж�ʹ��
	
	//TIM3_Configuration();
	}








void Delay(vu32 nCount)
	{
	for(; nCount != 0; nCount--);
	}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
//void Delay(__IO uint32_t nCount)
//	{
//	for(; nCount != 0; nCount--);
//	}

	u8 KEY_Scan(void)
	{	 
	static u8 key_up=1;//�������ɿ���־	
	if(key_up&&((STM_EVAL_PBGetState(Button_KEY1) == 0x00)||(STM_EVAL_PBGetState(Button_WAKEUP) == 0x01)))
		{
		delay_ms(10);//ȥ���� 
		key_up=0;

		if (STM_EVAL_PBGetState(Button_KEY1) == 0x00)		//��������:�͵�ƽ��Ч
			{
			return 2;
			}
		if (STM_EVAL_PBGetState(Button_WAKEUP) == 0x01)		//��������:�ߵ�ƽ��Ч
			{
			return 3;
			}
		}
		else if((STM_EVAL_PBGetState(Button_KEY1) == 0x01)&&(STM_EVAL_PBGetState(Button_WAKEUP) == 0x00)) key_up=1; 	    
	return 0;// �ް�������
	}


/* set phase to \"positive with PWM */
void pwm_set_pwm_hi(u16 phase)
{
    TIM_SelectOCxM(TIM1, phase, TIM_OCMode_PWM1);
    TIM_CCxCmd(TIM1, phase, TIM_CCx_Enable);
    TIM_CCxNCmd(TIM1, phase, TIM_CCxN_Disable);

	TIM_CCxCmd(TIM4, TIM_Channel_1, TIM_CCx_Enable);
	TIM_CCxCmd(TIM4, TIM_Channel_2, TIM_CCx_Enable);
	TIM_CCxCmd(TIM4, TIM_Channel_3, TIM_CCx_Enable);
	TIM_CCxCmd(TIM4, TIM_Channel_4, TIM_CCx_Enable);
	//TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
	//TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);

	//    TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);     TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Disable); 
}
//********************************************************************************  
/* set phase to \"negative with PWM */
void pwm_set_pwm_lo(u16 phase)
{
    TIM_SelectOCxM(TIM1, phase, TIM_OCMode_PWM1);
    TIM_CCxCmd(TIM1, phase, TIM_CCx_Disable);
    TIM_CCxNCmd(TIM1, phase, TIM_CCxN_Enable);

	TIM_CCxCmd(TIM4, TIM_Channel_1, TIM_CCx_Disable);
	TIM_CCxCmd(TIM4, TIM_Channel_2, TIM_CCx_Disable);
	TIM_CCxCmd(TIM4, TIM_Channel_4, TIM_CCx_Disable);
	TIM_CCxCmd(TIM4, TIM_Channel_3, TIM_CCx_Disable);
		//TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
	//TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);
	 //   TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);     TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable); 
}

void pwm_set____off(u16 phase)
{
    TIM_CCxCmd(TIM1, phase, TIM_CCx_Disable);
    TIM_CCxNCmd(TIM1, phase, TIM_CCxN_Disable);
}

#define SLOW 600//600 //50*1000--10Hz
#define FAST 	100   //50 10K
#define GAP  50
int main(void)
	{  
		uint16_t counter = 0;
		u8 Key_Vlaue; 
//	u8 len=0;	
//	u16 times=0; 
		
	Init_All_Periph();
	SysTick_Initaize();
	STM_EVAL_LEDOff(LED1);	 //Ϩ��LED0
	STM_EVAL_LEDOff(LED2);	 //Ϩ��LED0

#if 0

		SERVO_M_init();
		SERVO_M_set_clockwise(FALSE);
		SERVO_M_set_step_interval(100); //us
		SERVO_M_start();
//		
		STEP_M_init();
		STEP_M_set_excitation(3);//div 8 --24000
		STEP_M_set_clock(SLOW); //speed
		//STEP_M_set_peroid(250);//just for hw pwm
		STEP_M_reset_counter();
		STEP_M_set_plus_num_per_circle(800*20);// uint_16 65536
		STEP_M_set_target_position(20, 30);
		
		STEP_MOT_set_clockwise(TRUE);
		STEP_M_DECAY(0);
		STEP_M_set_enable(TRUE);
		STEP_M_start_run();
//		//read id from eeprom
		printf("\nREPORT#ID@%d#STATUS@ready\n",BOARD_ID);
	STM_EVAL_PBInit(Button_WAKEUP,Mode_GPIO);
	STM_EVAL_PBInit(Button_KEY1, Mode_GPIO);	
#else
	STEP5_init();
	//STEP5_CLK_H();
#endif	
	while(1)
		{
			//printf("\n uncompleted circle =%d , ",STEP_M_get_uncompleted_circle());

			
			STEP5_CLK_L();
			pwm_set_pwm_hi(TIM_Channel_1);
			pwm_set_pwm_hi(TIM_Channel_2);
			pwm_set_pwm_hi(TIM_Channel_3);
			//TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable); 
			//TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);
			delay_ms(5);
			pwm_set_pwm_lo(TIM_Channel_1);
			pwm_set_pwm_lo(TIM_Channel_2);
			pwm_set_pwm_lo(TIM_Channel_3);
			//TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable); 
			//TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);						
			STEP5_CLK_H();
			delay_ms(5);
			#if 0
			pwm_set____off(TIM_Channel_1);
			pwm_set____off(TIM_Channel_2);
			pwm_set____off(TIM_Channel_3);
			delay_ms(5);
			#endif
			
			#if 0
						msg_process();
			delay_ms(50);	
			Key_Vlaue=KEY_Scan();//�õ���ֵ
			if(Key_Vlaue)
				{						   
				switch(Key_Vlaue)
					{				 
					case 1://for servo motor int
						//STM_EVAL_LEDToggle(LED1);
						break;
					case 2: //KEY 1
						STM_EVAL_LEDToggle(LED1);
						//STEP_M_stop_run();//NO NEED
						if( SLOW - GAP*counter > FAST)
							counter ++ ;
						STEP_M_set_clock(SLOW - GAP*counter);						
						//STEP_M_start_run();
						break;
					case 3:				//wakeup
						//STM_EVAL_LEDToggle(LED1);
						STM_EVAL_LEDToggle(LED2);
						//STEP_M_stop_run();//NO NEED
					  if(counter>0)
							counter --;
						STEP_M_set_clock(SLOW - GAP*counter);					
						//STEP_M_start_run();
						break;
					}
					
					DEBUG("\n counter = %d",counter);
				} 
			
			Key_Vlaue=KEY_Scan();//�õ���ֵ
			if(Key_Vlaue)
				{						   
				switch(Key_Vlaue)
					{				 
					case 1://for servo motor int
						STEP5_CLK_L();
						TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable); 
						TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);
						STEP5_CLK_H();
						break;
					case 2: //KEY 1
						STEP5_CLK_L();
						TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable); 
						TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);
						STEP5_CLK_H();
						break;
					case 3:				//wakeup
						STEP5_CLK_L();
						TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable); 
						TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);
						delay_ms(20);
						TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable); 
						TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);
						delay_ms(20);
						STEP5_CLK_L();
						TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable); 
						TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable);
						delay_ms(20);
						TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable); 
						TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable);						
						STEP5_CLK_H();
						break;
					}
					
					DEBUG("\n counter = %d",counter);
				} 
			#endif

		}//while
	}//main





#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif





