#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stm32f4xx.h"

#define MotorNum  32

/* ���ָ��*/
typedef enum
{
	
			Motor_OFF = 0x80,    											//�ػ�
			Motor_ON = 0x88,     											//����
			Motor_Stop = 0x81,   											//ֹͣ
			Motor_Torque = 0xA1, 											//Ť��
			Motor_Speed = 0xA2, 										 	//�ٶ�
			Motor_Angle = 0xA3,  											//��Ȧλ�ýǶ�
			Motor_AngleSpeed = 0xA4, 									//��Ȧ�Ƕ��ٶ�
			Motor_ReadPID = 0x30,    									//��PID
			Motor_WritePID_TEMP = 0x31, 							//дPID�ϵ�ʧЧ
			Motor_WritePID_Perpetual = 0x32, 					//дPID�ϵ���Ч
			Motor_ReadFault = 0x9A, 									//��ȡ����
			Motor_CleanFault = 0x9B,									//�������
			Motor_ReadStatus = 0x9C,									//��ȡ�¶ȡ���ѹ��ת�١�������
	    Motor_Rebrake = 0x8C,                     //���ɲ��
	    Motor_Brake = 0x8C                        //ɲ��	
}Motor_Cmd;

typedef enum
{
	MotorON = 0,
	MotorOFF = 1,
	MotorRun = 2,
	MotorStop = 3,
	MotorFault = 4
}Motor_Status;

/* ��������ṹ�� */
struct MotorStruct
{
	uint8_t ID;
	uint8_t speed;
	uint8_t Current;
	uint8_t Voltage;
	Motor_Status Status;
};


int Motor_Switch(uint8_t id,Motor_Status status);
void Motor_SpeedMode(uint8_t id,int32_t speed);
void Motor_AngleSpeedMode_multi(uint8_t id, uint16_t maxSpeed, int32_t angleControl);
void Motor_StopCmd(uint8_t id);
void Motor_RebrakeCmd(uint8_t id);
void Motor_BrakeCmd(uint8_t id);
#endif