/**
  ******************************************************************************
  * �ļ�����: bsp_gps_usartx.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: GPSģ����������
  ******************************************************************************
  * ˵����
  * ����������ӲʯYS-F1Mini��ʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "gps/bsp_gps_usartx.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
UART_HandleTypeDef husartx;
DMA_HandleTypeDef hdma_usartx_rx;
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ����Ӳ����ʼ������
  * �������: 
  * �� �� ֵ: ��
  * ˵    ��: 
  */
void GPS_USART_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
   /* ʹ�ܴ��ڹ�������GPIOʱ�� */
   USARTx_GPIO_ClK_ENABLE();

  /* �������蹦��GPIO���� */
  GPIO_InitStruct.Pin = USARTx_Tx_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(USARTx_Tx_GPIO, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = USARTx_Rx_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USARTx_Rx_GPIO, &GPIO_InitStruct);
  
  /* ��ʼ��DMA���� */  
  hdma_usartx_rx.Instance = USARTx_DMAx_CHANNELn;
  hdma_usartx_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_usartx_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usartx_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usartx_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usartx_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usartx_rx.Init.Mode = DMA_CIRCULAR;
  hdma_usartx_rx.Init.Priority = DMA_PRIORITY_HIGH;
  HAL_DMA_Init(&hdma_usartx_rx);

  __HAL_LINKDMA(&husartx,hdmarx,hdma_usartx_rx);
}


/**
  * ��������: ���ڲ�������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void MX_USARTx_Init(void)
{
  /* ��������ʱ��ʹ�� */
  USART_RCC_CLK_ENABLE();
  
  /* ʹ�ܴ���DMAʱ�� */
  USARTx_RCC_DMAx_CLK_ENABLE();
  
  husartx.Instance = USARTx;
  husartx.Init.BaudRate = USARTx_BAUDRATE;
  husartx.Init.WordLength = UART_WORDLENGTH_8B;
  husartx.Init.StopBits = UART_STOPBITS_1;
  husartx.Init.Parity = UART_PARITY_NONE;
  husartx.Init.Mode = UART_MODE_RX;
  husartx.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husartx.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&husartx);
  
  GPS_USART_Init();
  
  /* DMA interrupt configuration */
  HAL_NVIC_SetPriority(USARTx_DMAx_CHANNELn_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(USARTx_DMAx_CHANNELn_IRQn);  
  
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
