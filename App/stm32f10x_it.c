/**
  ******************************************************************************
  * @file    Project/Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    06/19/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "eval.h"  
//#include "GLCD.h"
//#include <stdio.h>
#include "motor_servo.h"
#include "motor_step.h"
#include "SysTickDelay.h"
#include "msg_handler.h"



/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
	{
//����һ:
//	delay_ms(10);//����
//	if (STM_EVAL_PBGetState(Button_WAKEUP) == 0x01)		//��������:�ߵ�ƽ��Ч
//		{
//		STM_EVAL_LEDToggle(LED1);
//		STM_EVAL_LEDToggle(LED2);
//		}		 
//	//EXTI->PR=1<<0;  //���LINE0�ϵ��жϱ�־λ  
//	/* Clear the Key Button EXTI line pending bit */  
//	EXTI_ClearITPendingBit(WAKEUP_BUTTON_EXTI_LINE);  //���EXTI0��·����λ

//������:
//	if(EXTI_GetITStatus(WAKEUP_BUTTON_EXTI_LINE) != RESET)	  //���ָ����EXTI0��·�������������
//		{
//		/* Clear the Key Button EXTI line pending bit */  
//		EXTI_ClearITPendingBit(WAKEUP_BUTTON_EXTI_LINE);  //���EXTI0��·����λ
//		/* Toggle LED1/2 */
//		STM_EVAL_LEDToggle(LED1);
//		STM_EVAL_LEDToggle(LED2);
//		}
	}

//�ⲿ�ж�15~10�������
void EXTI15_10_IRQHandler(void)
	{			
	delay_ms(10);    //����			 
	if (STM_EVAL_PBGetState(Button_KEY0) == 0x00)		//��������:�͵�ƽ��Ч
		{
		STM_EVAL_LEDToggle(LED2);
			 SERVO_M_fault_out();
		}
	else if (STM_EVAL_PBGetState(Button_KEY1) == 0x00)		//��������:�͵�ƽ��Ч
		{
		STM_EVAL_LEDToggle(LED2);
		}
	//EXTI->PR=1<<13;     //���LINE13�ϵ��жϱ�־λ  
	//EXTI->PR=1<<15;     //���LINE15�ϵ��жϱ�־λ  
	/* Clear the Key Button EXTI line pending bit */  
	EXTI_ClearITPendingBit(BUTTON0_EXTI_LINE);  //���EXTI13��·����λ
	EXTI_ClearITPendingBit(BUTTON1_EXTI_LINE);  //���EXTI15��·����λ
	}


/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
	{
	}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
	{
	}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)   //TIM3�ж�
	{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		/* Pin PD.02 toggling with frequency = 10KHz */
		//GPIO_WriteBit(GPIOD, GPIO_Pin_2, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_2)));
		//clay STM_EVAL_LEDToggle(LED2);
		//	SERVO_M_run_step(); no use, for pwm
		}
	}

	
/**
  * @brief  This function handles TIM5 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)   //TIM3�ж�
	{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		/* Pin PD.02 toggling with frequency = 10KHz */
		//STEP_M_CLK_toggle();//step motor use pwm
			SERVO_M_run_step();
		}
	}
	
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
//u8 USART_RX_BUF[64];     //���ջ���,���64���ֽ�.
////����״̬
////bit7��������ɱ�־
////bit6�����յ�0x0d
////bit5~0�����յ�����Ч�ֽ���Ŀ
//u8 USART_RX_STA=0;       //����״̬���

//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ��� 
void USART1_IRQHandler(void)	//����1�жϷ������
	{
	u8 Res;
	//STM_EVAL_LEDToggle(LED2);
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x80)==0)//����δ���
			{
			if(USART_RX_STA&0x40)//���յ���0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x80;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x40;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3F]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>63)USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}  		 
		}
	}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
