#include <string.h>
#include <stdio.h>
#include "Task.h"
#include "Motor.h"
#include "usart.h"

extern uint8_t rec_flag;
extern uint8_t tx_flag;

void ParseMotorCommand(char* command) {
    char* cmdType = strtok(command, ":"); // 读取命令类型
    if (cmdType == NULL) return;

    if (strcmp(cmdType, "Motor_Speed") == 0) {
        // 处理速度模式 Motor_Speed:Motor1:1000
        char* motorStr = strtok(NULL, ":");
        char* speedStr = strtok(NULL, ":");

        if (motorStr && speedStr) {
            uint8_t motorId = atoi(motorStr + 5);
            int32_t speed = atoi(speedStr);

            if (motorId >= 1 && motorId <= MotorNum) {
                if (speed == 0) {
                    Motor_StopCmd(motorId);
                    printf("\r\nMotor %d stop.\r\n", motorId);
                } else {
                    Motor_SpeedMode(motorId, speed);
                    printf("\r\nMotor %d speed = %d.\r\n", motorId, speed);
                }
            }
        }
    } 
    else if (strcmp(cmdType, "Motor_AngleSpeed") == 0) {
        // 处理角度速度模式 Motor_AngleSpeed:Motor1:1000:10000
        char* motorStr = strtok(NULL, ":");
        char* speedStr = strtok(NULL, ":");
        char* angleStr = strtok(NULL, ":");

        if (motorStr && speedStr && angleStr) {
            uint8_t motorId = atoi(motorStr + 5);
            uint16_t speed = (uint16_t)atoi(speedStr);
            int32_t angle = atoi(angleStr);

            if (motorId >= 1 && motorId <= MotorNum) {
							  
                Motor_AngleSpeedMode_multi(motorId, speed, angle);
                printf("\r\n Motor %d angle speed control: speed=%d, angle=%d.\r\n", motorId, speed, angle);
            }
        }
    }
		else if (strcmp(cmdType, "Motor_Rebrake") == 0)
		{
			char* idStr = strtok(NULL, ":");
			if(idStr)
				{
				uint8_t MotorID = (uint8_t)atoi(idStr);
				
				if(MotorID >= 1 && MotorID <= MotorNum)
						{
							Motor_RebrakeCmd(MotorID);
							printf("motor %d Rebrake\r\n",MotorID);
						}
					}
			
		}
		else if (strcmp(cmdType, "Motor_Brake") == 0)
		{
			char* idStr = strtok(NULL, ":");
			if(idStr)
				{
				uint8_t MotorID = (uint8_t)atoi(idStr);
				
				if(MotorID >= 1 && MotorID <= MotorNum)
						{
							Motor_BrakeCmd(MotorID);
							printf("motor %d Brake\r\n",MotorID);
						}
					}
			
		}
		else if (strcmp(cmdType, "Motor_stop") == 0)
		{
    // 可扩展支持其他模式，如 Torque、ReadStatus 等
			Motor_SpeedMode(1,0);
			Motor_SpeedMode(2,0);
		}
}


// 模拟串口接收数据
void SerialReceiveHandler(char* input) {
    //printf("Received command: %s\n", input);
    ParseMotorCommand(input); // 解析并执行命令
}





