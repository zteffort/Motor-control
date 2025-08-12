#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "can.h"
#include "task.h"
#include "motor.h"
//ALIENTEK ̽����STM32F407������ ʵ��27
//CANͨ��ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK
	// ���徲̬�����洢�������ݣ������жϺ������������ں���������Ϊstatic��
static char rx_buffer[100];  // ���ջ�����
static uint8_t rx_index = 0; // ����������
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	Usart_Config();	//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
	KEY_Init(); 				//������ʼ��  
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,3,CAN_Mode_LoopBack);//CAN��ʼ������ģʽ,������1000Kbps    
	printf("������������\r\n");

 									  
while(1)
	{
		
	}
}

void USART1_IRQHandler(void) {
    // 1. ����Ƿ񴥷�RXNE�����ռĴ����ǿ��жϣ�
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        // 2. ��ȡ���յ������ݣ��Զ����RXNE��־��
        uint8_t received_data = USART_ReceiveData(USART1);

        // 3. �����������\r��\n��
        if (received_data == '\r' || received_data == '\n') {
            if (rx_index > 0) { // ���������
                rx_buffer[rx_index] = '\0';  // ����ַ���������
                SerialReceiveHandler(rx_buffer); // ��������������� --��Ҫ�����
							printf("it is ok!\r\n");
                rx_index = 0;    // ��������
            }
        } 
        // 4. �洢��ͨ�ַ���������
        else {
            if (rx_index < sizeof(rx_buffer) - 1) { // Ԥ���������ռ�
                rx_buffer[rx_index++] = received_data;
            } else {
                // ���������������ѡ�����û������ݣ�
                rx_index = 0; // ʾ������ջ��������¿�ʼ����
            }
        }

        // 5. ����δ�Զ������־�����ֶ��������׼��ͨ���Զ������
        // USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }

    // 6. ��ѡ�����������ж����ͣ�������жϣ�
    if (USART_GetITStatus(USART1, USART_IT_ERR) != RESET) {
        // ����֡��������������������
        USART_ClearITPendingBit(USART1, USART_IT_ERR);
        // ʾ���������������־�������ͺ����DR�Ĵ�����
        if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET) {
            (void)USART_ReceiveData(USART1); // ��DR�����־
            USART_ClearFlag(USART1, USART_FLAG_ORE);
        }
    }
}