#ifndef __TASK__H_
#define __TASK__H_

#include "stm32f4xx.h"

#define MAX_CMD_LENGTH 20

// ������
#define MOTOR1 1
#define MOTOR2 2

//typedef struct 
//{
//	uint8_t pros; 			//�����־
//	uint8_t ATBUFF[256];//ָ���
//	uint8_t pos;        //ָ��λ��
//	
//}TypedefStructAT_Queue;

void ParseMotorCommand(char* command);
void SerialReceiveHandler(char* input);
void USART_ReceiveDataHandler(void);

#endif