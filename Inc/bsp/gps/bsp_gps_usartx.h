#ifndef __BSP_USARTX_H__
#define __BSP_USARTX_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
//#define __GPS_LOG_FILE    //定义这个宏，对SD卡上的gpslog.txt文件进行解码；不定义的话使用串口2接收GPS信息解码
//#define __GPS_DEBUG     //定义这个宏，在nmea解码时会输出调试信息

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

// 串口DMA相关
#define USARTx_DMAx_CHANNELn                   DMA1_Channel6
#define USARTx_RCC_DMAx_CLK_ENABLE()           __HAL_RCC_DMA1_CLK_ENABLE()
#define USARTx_DMAx_CHANNELn_IRQn              DMA1_Channel6_IRQn
#define USARTx_DMAx_CHANNELn_IRQHANDLER        DMA1_Channel6_IRQHandler

#define GPS_RBUFF_SIZE                         512                   //串口接收缓冲区大小
#define HALF_GPS_RBUFF_SIZE                    (GPS_RBUFF_SIZE/2)    //串口接收缓冲区一半  


/* 扩展变量 ------------------------------------------------------------------*/
extern UART_HandleTypeDef husartx;
extern DMA_HandleTypeDef hdma_usartx_rx;

/* 函数声明 ------------------------------------------------------------------*/
void MX_USARTx_Init(void);


#endif  /* __BSP_USARTX_H__ */

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
