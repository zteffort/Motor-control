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
//������ػ�
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
	
	/* ���÷��ͺ��� */
	MyCanSend(id,Motor_Data);
	return status;
}

//����ٶ�ģʽ
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
			Motor_Data[4] = (speed & 0xFF);         // ���ֽ�
      Motor_Data[5] = (speed >> 8)  & 0xFF;    // �ڶ��ֽ�
      Motor_Data[6] = (speed >> 16) & 0xFF;   // �����ֽ�
      Motor_Data[7] = (speed >> 24) & 0xFF;   // ���ֽ�
		}
		
		else if (speed < 0)
		{
			Speed = 0xffffffff + speed;
			Motor_Data[0] = Motor_Speed;
			Motor_Data[1] = id;
			Motor_Data[2] = 0x04;
			Motor_Data[3] = 0xE7;
			Motor_Data[4] = (Speed & 0xFF);         // ���ֽ�
      Motor_Data[5] = (Speed >> 8)  & 0xFF;    // �ڶ��ֽ�
      Motor_Data[6] = (Speed >> 16) & 0xFF;   // �����ֽ�
      Motor_Data[7] = (Speed >> 24) & 0xFF;   // ���ֽ�
		}
		MyCanSend(id,Motor_Data);
}
// ��Ȧλ�ýǶ�+�ٶȿ��ƺ�����֧�ָ����Ƕȿ��ƣ�
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
        // �����Ƕȣ�ת��Ϊ������ʽ
        angleValue = 0xFFFFFFFF - ((uint32_t)(-angleControl) - 1);
    }
    else
    {
        angleValue = (uint32_t)angleControl;
    }

    // ����λ����λд������
    Motor_Data[4] = (uint8_t)((angleValue >> 0) & 0xFF);   // ���ֽ�
    Motor_Data[5] = (uint8_t)((angleValue >> 8) & 0xFF);
    Motor_Data[6] = (uint8_t)((angleValue >> 16) & 0xFF);
    Motor_Data[7] = (uint8_t)((angleValue >> 24) & 0xFF);  // ���ֽ�

    MyCanSend(id, Motor_Data);
}


//���ֹͣ
void Motor_StopCmd(uint8_t id)
{
		CleanData();
		Motor_Data[0] = Motor_Stop;
		MyCanSend(id,Motor_Data);
}
//������ɲ��
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
//������ɲ��
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