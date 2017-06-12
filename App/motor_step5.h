/*
* for STEP  5 phase motor header 
timer 1 for pwm a an b bn c cn
A: 	PA8  	IN
AN:	PB13	OUT
B:	PA9		IN
BN:	PB14	OUT
C:	PA10	IN
CN:	PB15	OUT
#if TIM8_PWM
D:	PC7		IN
DN:	PB0	OUT
E:	PC8		IN
EN:	PB1		OUT
#else
timer 4 for pwm d dn e en
D:	PB6		IN
DN:	PB7		OUT
E:	PB8		IN
EN:	PB9		OUT
#endif
timer 6 for step 
*/

#ifndef MOTOR_STEP5_H_
#define MOTOR_STEP5_H_ 

#define TIM8_PWM 1

#define PHASE_A_H_ON 		TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable)
#define PHASE_A_H_OFF 		TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable)
#define PHASE_AN_L_ON 		TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable)
#define PHASE_AN_L_OFF 	TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable)
#define PHASE_A_RISING	 	TIM_SetCompare1(TIM1,pwm_duty_gap*(DUTY_STEP*2 -duty_count))
#define PHASE_A_FALLING 	TIM_SetCompare1(TIM1,pwm_duty_gap*(duty_count-DUTY_STEP*5))
#define PHASE_A_OUT_H		TIM_SetCompare1(TIM1,0)
#define PHASE_A_OUT_L		TIM_SetCompare1(TIM1,pwm_fre+100)

#define PHASE_B_H_ON 		TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable)
#define PHASE_B_H_OFF 		TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable)
#define PHASE_BN_L_ON 		TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable)
#define PHASE_BN_L_OFF 		TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Disable)
#define PHASE_B_RISING	 	TIM_SetCompare2(TIM1,pwm_duty_gap*(DUTY_STEP*4 -duty_count))
#define PHASE_B_FALLING 	TIM_SetCompare2(TIM1,pwm_duty_gap*(duty_count-DUTY_STEP*7))
#define PHASE_B_OUT_H		TIM_SetCompare2(TIM1,0)
#define PHASE_B_OUT_L		TIM_SetCompare2(TIM1,pwm_fre+100)

#define PHASE_C_H_ON 		TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable)
#define PHASE_C_H_OFF 		TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable)
#define PHASE_CN_L_ON 		TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable)
#define PHASE_CN_L_OFF 		TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Disable)
#define PHASE_C_RISING	 	TIM_SetCompare3(TIM1,pwm_duty_gap*(DUTY_STEP*6 -duty_count))
#define PHASE_C_FALLING_END 	TIM_SetCompare3(TIM1,pwm_duty_gap*(duty_count))
#define PHASE_C_FALLING_START 	TIM_SetCompare3(TIM1,pwm_duty_gap*( duty_count-DUTY_STEP*8))
#define PHASE_C_OUT_H		TIM_SetCompare3(TIM1,0)
#define PHASE_C_OUT_L		TIM_SetCompare3(TIM1,pwm_fre+100)


#if  TIM8_PWM
#define PHASE_D_H_ON 		TIM_CCxCmd(TIM8, TIM_Channel_2, TIM_CCx_Enable)
#define PHASE_D_H_OFF 		TIM_CCxCmd(TIM8, TIM_Channel_2, TIM_CCx_Disable)
#define PHASE_DN_L_ON 		TIM_CCxNCmd(TIM8, TIM_Channel_2, TIM_CCxN_Enable)
#define PHASE_DN_L_OFF 	TIM_CCxNCmd(TIM8, TIM_Channel_2, TIM_CCxN_Disable)
#define PHASE_D_RISING	 	TIM_SetCompare2(TIM8,pwm_duty_gap*(DUTY_STEP*8 -duty_count))
#define PHASE_D_FALLING 	TIM_SetCompare2(TIM8,pwm_duty_gap*(duty_count-DUTY_STEP))
#define PHASE_D_OUT_H		TIM_SetCompare2(TIM8,0)
#define PHASE_D_OUT_L		TIM_SetCompare2(TIM8,pwm_fre+100)

#define PHASE_E_H_ON 		TIM_CCxCmd(TIM8, TIM_Channel_3, TIM_CCx_Enable)
#define PHASE_E_H_OFF 		TIM_CCxCmd(TIM8, TIM_Channel_3, TIM_CCx_Disable)
#define PHASE_EN_L_ON 		TIM_CCxNCmd(TIM8, TIM_Channel_3, TIM_CCxN_Enable)
#define PHASE_EN_L_OFF 		TIM_CCxNCmd(TIM8, TIM_Channel_3, TIM_CCxN_Disable)
#define PHASE_E_RISING	 	TIM_SetCompare3(TIM8,pwm_duty_gap*(DUTY_STEP*10 -duty_count))
#define PHASE_E_FALLING 	TIM_SetCompare3(TIM8,pwm_duty_gap*(duty_count-DUTY_STEP*3))
#define PHASE_E_OUT_H		TIM_SetCompare3(TIM8,0)
#define PHASE_E_OUT_L		TIM_SetCompare3(TIM8,pwm_fre+100)
#else
#define PHASE_D_H_ON 		TIM_CCxCmd(TIM4, TIM_Channel_1, TIM_CCx_Enable)
#define PHASE_D_H_OFF 		TIM_CCxCmd(TIM4, TIM_Channel_1, TIM_CCx_Disable)
#define PHASE_DN_L_ON 		TIM_CCxCmd(TIM4, TIM_Channel_2, TIM_CCx_Enable)
#define PHASE_DN_L_OFF 	TIM_CCxCmd(TIM4, TIM_Channel_2, TIM_CCx_Disable)

#define PHASE_E_H_ON 		TIM_CCxCmd(TIM4, TIM_Channel_3, TIM_CCx_Enable)
#define PHASE_E_H_OFF 		TIM_CCxCmd(TIM4, TIM_Channel_3, TIM_CCx_Disable)
#define PHASE_EN_L_ON 		TIM_CCxCmd(TIM4, TIM_Channel_4, TIM_CCx_Enable)
#define PHASE_EN_L_OFF 		TIM_CCxCmd(TIM4, TIM_Channel_4, TIM_CCx_Disable)
#endif

#if 0
#define PHASE_A_IN_ON 		TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable)
#define PHASE_A_IN_OFF 		TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable)
#define PHASE_AN_OUT_ON 	TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Enable)
#define PHASE_AN_OUT_OFF 	TIM_CCxNCmd(TIM1, TIM_Channel_1, TIM_CCxN_Disable)

#define PHASE_B_IN_ON 		TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable)
#define PHASE_B_IN_OFF 		TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable)
#define PHASE_BN_OUT_ON 	TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Enable)
#define PHASE_BN_OUT_OFF 	TIM_CCxNCmd(TIM1, TIM_Channel_2, TIM_CCxN_Disable)

#define PHASE_C_IN_ON 		TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable)
#define PHASE_C_IN_OFF 		TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable)
#define PHASE_CN_OUT_ON 	TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Enable)
#define PHASE_CN_OUT_OFF 	TIM_CCxNCmd(TIM1, TIM_Channel_3, TIM_CCxN_Disable)

#define PHASE_D_IN_ON 		TIM_CCxCmd(TIM4, TIM_Channel_1, TIM_CCx_Enable)
#define PHASE_D_IN_OFF 		TIM_CCxCmd(TIM4, TIM_Channel_1, TIM_CCx_Disable)
#define PHASE_DN_OUT_ON 	TIM_CCxCmd(TIM4, TIM_Channel_2, TIM_CCx_Enable)
#define PHASE_DN_OUT_OFF 	TIM_CCxCmd(TIM4, TIM_Channel_2, TIM_CCx_Disable)

#define PHASE_E_IN_ON 		TIM_CCxCmd(TIM4, TIM_Channel_3, TIM_CCx_Enable)
#define PHASE_E_IN_OFF 		TIM_CCxCmd(TIM4, TIM_Channel_3, TIM_CCx_Disable)
#define PHASE_EN_OUT_ON 	TIM_CCxCmd(TIM4, TIM_Channel_4, TIM_CCx_Enable)
#define PHASE_EN_OUT_OFF 	TIM_CCxCmd(TIM4, TIM_Channel_4, TIM_CCx_Disable)
#endif

//test only
#define CLK5_PORT	GPIOA
#define CLK5_PIN		GPIO_Pin_6 //12->7
#define STEP5_CLK_H()	GPIO_SetBits(CLK5_PORT, CLK5_PIN)
#define STEP5_CLK_L()	GPIO_ResetBits(CLK5_PORT, CLK5_PIN)
#define STEP5_CLK_TOGGLE() CLK5_PORT->ODR ^= CLK5_PIN
#define TEST5_CLK_TOGGLE()  GPIOA->ODR ^= GPIO_Pin_12


extern void STEP5_motor_init(void);
extern void STEP5_motor_phase_INT(void);
	
#endif//MOTOR_STEP5_H_
