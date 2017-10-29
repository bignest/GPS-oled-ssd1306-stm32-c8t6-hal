#ifndef __BSP_USARTX_H__
#define __BSP_USARTX_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
//#define __GPS_LOG_FILE    //��������꣬��SD���ϵ�gpslog.txt�ļ����н��룻������Ļ�ʹ�ô���2����GPS��Ϣ����
//#define __GPS_DEBUG     //��������꣬��nmea����ʱ�����������Ϣ

#define USARTx                                 USART2
#define USARTx_BAUDRATE                        9600
#define USART_RCC_CLK_ENABLE()                 __HAL_RCC_USART2_CLK_ENABLE()
#define USART_RCC_CLK_DISABLE()                __HAL_RCC_USART2_CLK_DISABLE()

#define USARTx_GPIO_ClK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_Tx_GPIO_PIN                     GPIO_PIN_2
#define USARTx_Tx_GPIO                         GPIOA
#define USARTx_Rx_GPIO_PIN                     GPIO_PIN_3
#define USARTx_Rx_GPIO                         GPIOA

//#define USARTx_IRQHANDLER                      USART2_IRQHandler
//#define USARTx_IRQn                            USART2_IRQn

// ����DMA���
#define USARTx_DMAx_CHANNELn                   DMA1_Channel6
#define USARTx_RCC_DMAx_CLK_ENABLE()           __HAL_RCC_DMA1_CLK_ENABLE()
#define USARTx_DMAx_CHANNELn_IRQn              DMA1_Channel6_IRQn
#define USARTx_DMAx_CHANNELn_IRQHANDLER        DMA1_Channel6_IRQHandler

#define GPS_RBUFF_SIZE                         512                   //���ڽ��ջ�������С
#define HALF_GPS_RBUFF_SIZE                    (GPS_RBUFF_SIZE/2)    //���ڽ��ջ�����һ��  


/* ��չ���� ------------------------------------------------------------------*/
extern UART_HandleTypeDef husartx;
extern DMA_HandleTypeDef hdma_usartx_rx;

/* �������� ------------------------------------------------------------------*/
void MX_USARTx_Init(void);


#endif  /* __BSP_USARTX_H__ */

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
