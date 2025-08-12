#include "motor.h"
#include "can.h"

uint8_t Motor_Data[8]={0};
void CleanData()
{
	for(int i=0;i<8;i++)
	{
		Motor_Data[i]=0;
	}
}

struct MotorStruct Motor[MotorNum];
//电机开关机
int Motor_Switch(uint8_t id,Motor_Status status)
{
	CleanData();
	if(status == MotorON)
	{
		Motor_Data[0] = Motor_ON;
	}
	else if(status == MotorOFF)
	{
		Motor_Data[0] = Motor_OFF;
	}
	
	/* 调用发送函数 */
	MyCanSend(id,Motor_Data);
	return status;
}

//电机速度模式
void Motor_SpeedMode(uint8_t id,int32_t speed)
{
		CleanData();
		int32_t Speed;
		speed=speed*100;
		if(speed >= 0)
		{
			Motor_Data[0] = Motor_Speed;
			Motor_Data[1] = id;
			Motor_Data[2] = 0x04;
			Motor_Data[3] = 0xE7;
			Motor_Data[4] = (speed & 0xFF);         // 低字节
      Motor_Data[5] = (speed >> 8)  & 0xFF;    // 第二字节
      Motor_Data[6] = (speed >> 16) & 0xFF;   // 第三字节
      Motor_Data[7] = (speed >> 24) & 0xFF;   // 高字节
		}
		
		else if (speed < 0)
		{
			Speed = 0xffffffff + speed;
			Motor_Data[0] = Motor_Speed;
			Motor_Data[1] = id;
			Motor_Data[2] = 0x04;
			Motor_Data[3] = 0xE7;
			Motor_Data[4] = (Speed & 0xFF);         // 低字节
      Motor_Data[5] = (Speed >> 8)  & 0xFF;    // 第二字节
      Motor_Data[6] = (Speed >> 16) & 0xFF;   // 第三字节
      Motor_Data[7] = (Speed >> 24) & 0xFF;   // 高字节
		}
		MyCanSend(id,Motor_Data);
}
// 多圈位置角度+速度控制函数（支持负数角度控制）
void Motor_AngleSpeedMode_multi(uint8_t id, uint16_t maxSpeed, int32_t angleControl)
{
    CleanData();
    Motor_Data[0] = Motor_AngleSpeed;
    Motor_Data[1] = id;

    Motor_Data[2] = (uint8_t)(maxSpeed & 0xFF);
    Motor_Data[3] = (uint8_t)((maxSpeed >> 8) & 0xFF);

    uint32_t angleValue;

    if (angleControl < 0)
    {
        // 负数角度，转换为补码形式
        angleValue = 0xFFFFFFFF - ((uint32_t)(-angleControl) - 1);
    }
    else
    {
        angleValue = (uint32_t)angleControl;
    }

    // 按高位到低位写入数据
    Motor_Data[4] = (uint8_t)((angleValue >> 0) & 0xFF);   // 低字节
    Motor_Data[5] = (uint8_t)((angleValue >> 8) & 0xFF);
    Motor_Data[6] = (uint8_t)((angleValue >> 16) & 0xFF);
    Motor_Data[7] = (uint8_t)((angleValue >> 24) & 0xFF);  // 高字节

    MyCanSend(id, Motor_Data);
}


//电机停止
void Motor_StopCmd(uint8_t id)
{
		CleanData();
		Motor_Data[0] = Motor_Stop;
		MyCanSend(id,Motor_Data);
}
//电机解除刹车
void Motor_RebrakeCmd(uint8_t id)
{
	  CleanData();
		Motor_Data[0] = Motor_Rebrake;
	  Motor_Data[1] = id;
    Motor_Data[2] = 0x01;
    Motor_Data[3] = 0xD8;
    Motor_Data[4] = 0x01;
    Motor_Data[5] = 0x01;
    Motor_Data[6] = 0x00;
    Motor_Data[7] = 0x00;
		MyCanSend(id,Motor_Data);
}
//电机解除刹车
void Motor_BrakeCmd(uint8_t id)
{
	  CleanData();
		Motor_Data[0] = Motor_Brake;
	  Motor_Data[1] = id;
    Motor_Data[2] = 0x01;
    Motor_Data[3] = 0xD8;
    Motor_Data[4] = 0x00;
    Motor_Data[5] = 0x00;
    Motor_Data[6] = 0x00;
    Motor_Data[7] = 0x00;
		MyCanSend(id,Motor_Data);
}