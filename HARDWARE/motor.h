#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stm32f4xx.h"

#define MotorNum  32

/* 电机指令*/
typedef enum
{
	
			Motor_OFF = 0x80,    											//关机
			Motor_ON = 0x88,     											//运行
			Motor_Stop = 0x81,   											//停止
			Motor_Torque = 0xA1, 											//扭矩
			Motor_Speed = 0xA2, 										 	//速度
			Motor_Angle = 0xA3,  											//多圈位置角度
			Motor_AngleSpeed = 0xA4, 									//多圈角度速度
			Motor_ReadPID = 0x30,    									//读PID
			Motor_WritePID_TEMP = 0x31, 							//写PID断电失效
			Motor_WritePID_Perpetual = 0x32, 					//写PID断电有效
			Motor_ReadFault = 0x9A, 									//读取错误
			Motor_CleanFault = 0x9B,									//清除错误
			Motor_ReadStatus = 0x9C,									//读取温度、电压、转速、编码器
	    Motor_Rebrake = 0x8C,                     //解除刹车
	    Motor_Brake = 0x8C                        //刹车	
}Motor_Cmd;

typedef enum
{
	MotorON = 0,
	MotorOFF = 1,
	MotorRun = 2,
	MotorStop = 3,
	MotorFault = 4
}Motor_Status;

/* 电机参数结构体 */
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