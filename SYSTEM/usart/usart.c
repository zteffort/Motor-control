#include "usart.h"


void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* �������ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

void Usart_Config(void)
{
	#if 1
    /*��һ������ʼ��������ʹ�õ�GPIO*/
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    //��GPIO��ʱ��,PB6��PB7��������AHB1��
    RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK|DEBUG_USART_TX_GPIO_CLK,ENABLE);

    //����RX��ʹ�õ�GPIO
    GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//����
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
    //��GPIOB_Pin_7���ӵ�Usart_RX
    GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,DEBUG_USART_RX_SOURCE,DEBUG_USART_RX_AF);
    GPIO_Init(DEBUG_USART_RX_GPIO_PORT,&GPIO_InitStructure);
    
    //����TX��ʹ�õ�GPIO
    GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//����
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
    //��GPIOB_Pin_7���ӵ�Usart_RX
    GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,DEBUG_USART_TX_SOURCE,DEBUG_USART_TX_AF);
    GPIO_Init(DEBUG_USART_TX_GPIO_PORT,&GPIO_InitStructure);

    //�ڶ���������usart��ʼ���ṹ��
		/*��usart��ʱ��*/
    RCC_APB2PeriphClockCmd(DEBUG_USART_CLK,ENABLE);
    /*���������ã�DEBUG_USART_BAUDRATE*/
    USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
    /*Ӳ�������ƣ���ʹ��Ӳ����*/
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /*USARTģʽ���ƣ�ͬʱʹ�ܽ��պͷ���*/
    USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    /*У��λѡ�񣺲�ʹ��У��*/
    USART_InitStructure.USART_Parity = USART_Parity_No;
    /*һ��ֹͣλ*/
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    /* �ֳ�(����λ+У��λ)��8 */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    /* ���USART��ʼ������ */
    USART_Init(DEBUG_USART,&USART_InitStructure);

    /*��������ʹ���ж�*/
    /* Ƕ�������жϿ�����NVIC���� */
   NVIC_Configuration();
    /*ʹ�ܴ��ڽ����ж�*/
   USART_ITConfig(DEBUG_USART,USART_IT_RXNE,ENABLE);

    /*���Ĳ���ʹ�ܴ���*/
    USART_Cmd(DEBUG_USART,ENABLE);
		#endif
}

/*****************  ����һ���ַ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;
	
	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(DEBUG_USART, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USART);
}
/*********************************************END OF FILE**********************/
