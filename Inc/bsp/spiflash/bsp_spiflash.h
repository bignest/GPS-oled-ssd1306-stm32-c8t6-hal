#ifndef __BSP_SPIFLASH_H__
#define __BSP_SPIFLASH_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
//#define  SPI_FLASH_ID                       0xEF3015     //W25X16   2MB
//#define  SPI_FLASH_ID                       0xEF4015	   //W25Q16   4MB
#define  SPI_FLASH_ID                       0xEF4016	   //W25Q32   4MB
//#define  SPI_FLASH_ID                       0XEF4017     //W25Q64   8MB
//#define  SPI_FLASH_ID                       0XEF4018     //W25Q128  16MB YS-F1Pro开发默认使用

#define FLASH_SPIx                                 SPI1
#define FLASH_SPIx_RCC_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define FLASH_SPIx_RCC_CLK_DISABLE()               __HAL_RCC_SPI1_CLK_DISABLE()

#define FLASH_SPI_GPIO_ClK_ENABLE()                __HAL_RCC_GPIOA_CLK_ENABLE() 
#define FLASH_SPI_GPIO_PORT                        GPIOA
#define FLASH_SPI_SCK_PIN                          GPIO_PIN_5
#define FLASH_SPI_MISO_PIN                         GPIO_PIN_6
#define FLASH_SPI_MOSI_PIN                         GPIO_PIN_7

#define FLASH_SPI_CS_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()    
#define FLASH_SPI_CS_PORT                          GPIOA
#define FLASH_SPI_CS_PIN                           GPIO_PIN_4

#define FLASH_SPI_CS_ENABLE()                      HAL_GPIO_WritePin(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN, GPIO_PIN_RESET)
#define FLASH_SPI_CS_DISABLE()                     HAL_GPIO_WritePin(FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN, GPIO_PIN_SET)

/* 扩展变量 ------------------------------------------------------------------*/
extern SPI_HandleTypeDef hspiflash;

/* 函数声明 ------------------------------------------------------------------*/

void MX_SPIFlash_Init(void);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
uint32_t SPI_FLASH_ReadID(void);
uint32_t SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);

uint8_t SPI_FLASH_ReadByte(void);
uint8_t SPI_FLASH_SendByte(uint8_t byte);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

#endif  /* __BSP_SPIFLASH_H__ */

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
