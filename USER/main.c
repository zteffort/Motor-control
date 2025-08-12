#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "can.h"
#include "task.h"
#include "motor.h"
//ALIENTEK 探索者STM32F407开发板 实验27
//CAN通信实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK
	// 定义静态变量存储接收数据（需在中断函数外声明或在函数内声明为static）
static char rx_buffer[100];  // 接收缓冲区
static uint8_t rx_index = 0; // 缓冲区索引
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	Usart_Config();	//初始化串口波特率为115200
	LED_Init();					//初始化LED 
	KEY_Init(); 				//按键初始化  
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,3,CAN_Mode_LoopBack);//CAN初始化环回模式,波特率1000Kbps    
	printf("串口正常工作\r\n");

 									  
while(1)
	{
		
	}
}

void USART1_IRQHandler(void) {
    // 1. 检查是否触发RXNE（接收寄存器非空中断）
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        // 2. 读取接收到的数据（自动清除RXNE标志）
        uint8_t received_data = USART_ReceiveData(USART1);

        // 3. 处理结束符（\r或\n）
        if (received_data == '\r' || received_data == '\n') {
            if (rx_index > 0) { // 避免空命令
                rx_buffer[rx_index] = '\0';  // 添加字符串结束符
                SerialReceiveHandler(rx_buffer); // 调用命令解析函数 --主要命令函数
							printf("it is ok!\r\n");
                rx_index = 0;    // 重置索引
            }
        } 
        // 4. 存储普通字符到缓冲区
        else {
            if (rx_index < sizeof(rx_buffer) - 1) { // 预留结束符空间
                rx_buffer[rx_index++] = received_data;
            } else {
                // 缓冲区溢出处理（可选：重置或丢弃数据）
                rx_index = 0; // 示例：清空缓冲区重新开始接收
            }
        }

        // 5. 若库未自动清除标志，需手动清除（标准库通常自动清除）
        // USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }

    // 6. 可选：处理其他中断类型（如错误中断）
    if (USART_GetITStatus(USART1, USART_IT_ERR) != RESET) {
        // 处理帧错误、噪声错误、溢出错误等
        USART_ClearITPendingBit(USART1, USART_IT_ERR);
        // 示例：清除溢出错误标志（部分型号需读DR寄存器）
        if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET) {
            (void)USART_ReceiveData(USART1); // 读DR清除标志
            USART_ClearFlag(USART1, USART_FLAG_ORE);
        }
    }
}