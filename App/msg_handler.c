/*
* 
*/
#include "stm32f10x_it.h"
#include "msg_handler.h"
#include <stdio.h>
#include "eval.h"
#include "UART_INTERFACE.h"

u8 BOARD_ID=1;
// bit 7~4: id/axis, bit 3~0: error id 
u8 ERR_REPORT_STA=0x21; 

u8 USART_RX_BUF[64];     //���ջ���,���64���ֽ�.
//����״̬
//bit7��������ɱ�־
//bit6�����յ�0x0d
//bit5~0�����յ�����Ч�ֽ���Ŀ
u8 USART_RX_STA=0;       //����״̬���

char TX_BUF[64];   
 const char *pERR_STR[ERR_MAX]={
	STR_ERR_STATUS,
	STR_ERR_SERVO_FAULT_OUT,
};

void msg_receive(){
	u8 t;
	u8 len;	
	if(USART_RX_STA&0x80)
			{					   
				len=USART_RX_STA&0x3f;//�õ��˴ν��յ������ݳ���
				printf("\n input :\n");
				for(t=0;t<len;t++)
				{
					//USART1->DR=USART_RX_BUF[t];
					//while((USART1->SR&0X40)==0);//�ȴ����ͽ���
					STM_EVAL_SendChar(USED_COM_NUMBER,(uint8_t)USART_RX_BUF[t]);
				}
				printf("\n\n");//���뻻��
				USART_RX_STA=0;
			}
}
void msg_send(){
	
	if(ERR_REPORT_STA){
		//memset(TX_BUF, 0, 64);
		sprintf(TX_BUF,"REPORT#ID@%d#AXIS@%d#STATUS@%s\n",BOARD_ID,ERR_REPORT_STA>>4,pERR_STR[ERR_REPORT_STA& ERR_MAX]);
		printf(TX_BUF);
		ERR_REPORT_STA=0;
	}
}

void msg_process(void){
	
	msg_receive();
	msg_send();

}
