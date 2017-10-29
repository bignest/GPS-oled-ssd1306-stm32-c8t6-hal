/**
  ******************************************************************************
  * 文件名程: bsp_gps_usartx.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: GPS模块驱动程序
  ******************************************************************************
  * 说明：
  * 本例程配套硬石YS-F1Mini板使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */

/* 包含头文件 ----------------------------------------------------------------*/
#include "gps/bsp_gps_usartx.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
UART_HandleTypeDef husartx;
DMA_HandleTypeDef hdma_usartx_rx;
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 串口硬件初始化配置
  * 输入参数: 
  * 返 回 值: 无
  * 说    明: 
  */
void GPS_USART_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
   /* 使能串口功能引脚GPIO时钟 */
   USARTx_GPIO_ClK_ENABLE();

  /* 串口外设功能GPIO配置 */
  GPIO_InitStruct.Pin = USARTx_Tx_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(USARTx_Tx_GPIO, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = USARTx_Rx_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USARTx_Rx_GPIO, &GPIO_InitStruct);
  
  /* 初始化DMA外设 */  
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
  * 函数功能: 串口参数配置.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void MX_USARTx_Init(void)
{
  /* 串口外设时钟使能 */
  USART_RCC_CLK_ENABLE();
  
  /* 使能串口DMA时钟 */
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

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
