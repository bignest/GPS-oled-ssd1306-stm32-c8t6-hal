#ifndef __BSP_LED_H__
#define __BSP_LED_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
typedef enum
{
  LED_OFF = 0,
  LED_ON  = 1,
  LED_TOGGLE = 2,
}LEDState_TypeDef;
#define IS_LED_STATE(STATE)           (((STATE) == LED_OFF) || ((STATE) == LED_ON) || ((STATE) == LED_TOGGLE))

/* �궨�� --------------------------------------------------------------------*/
#define LED1                          (uint8_t)0x01
#define LED2                          (uint8_t)0x02
#define IS_LED_TYPEDEF(LED)           (((LED) == LED1) || ((LED) == LED2))

/*
 * ���º궨�����ݸ�������Ӳ��ϢϢ��أ���Ҫ�鿴�������·ԭ��ͼ������ȷ��д��
 * ���磬��ԭ��ͼ����LED1�ƽ���stm32f103оƬ��PB0�����ϣ������й�LED1�ĺ궨��
 * ������GPIOB��GPIO_Pin_0��صģ�����ר�Ű���Щ�뿪����Ӳ����ص����ݶ���Ϊ�꣬
 * ������޸Ļ�����ֲ����ǳ����㡣
 */
#define LED1_RCC_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED1_GPIO_PIN                 GPIO_PIN_0
#define LED1_GPIO                     GPIOA


#define LED2_RCC_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED2_GPIO_PIN                 GPIO_PIN_1
#define LED2_GPIO                     GPIOA


#define LED1_ON                       HAL_GPIO_WritePin(LED1_GPIO,LED1_GPIO_PIN,GPIO_PIN_SET)    // ����ߵ�ƽ
#define LED1_OFF                      HAL_GPIO_WritePin(LED1_GPIO,LED1_GPIO_PIN,GPIO_PIN_RESET)  // ����͵�ƽ
#define LED1_TOGGLE                   HAL_GPIO_TogglePin(LED1_GPIO,LED1_GPIO_PIN)                // �����ת

#define LED2_ON                       HAL_GPIO_WritePin(LED2_GPIO,LED2_GPIO_PIN,GPIO_PIN_SET)    // ����ߵ�ƽ
#define LED2_OFF                      HAL_GPIO_WritePin(LED2_GPIO,LED2_GPIO_PIN,GPIO_PIN_RESET)  // ����͵�ƽ
#define LED2_TOGGLE                   HAL_GPIO_TogglePin(LED2_GPIO,LED2_GPIO_PIN)                // �����ת

/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void LED_GPIO_Init(void);
void LEDx_StateSet(uint8_t LEDx,LEDState_TypeDef state);

#endif  // __BSP_LED_H__

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
