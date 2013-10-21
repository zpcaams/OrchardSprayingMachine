/**
  ******************************************************************************
  * @file    STM32vldiscovery.h
  * @author  MCD Team
  * @version V1.0
  * @date    07/07/2010
  * @brief   Header file for STM32vldiscovery.c module.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __osm_H
#define __osm_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "STM32f10x.h"

/** @addtogroup Utilities
  * @{
  */ 
  
/** @addtogroup STM32vldiscovery
  * @{
  */ 

/** @defgroup STM32vldiscovery_Abstraction_Layer
  * @{
  */  

/** @defgroup STM32vldiscovery_HARDWARE_RESOURCES
  * @{
  */
  
/** @defgroup STM32vldiscovery_Exported_Types
  * @{
  */
typedef enum 
{
  LED0 = 0,
  LED1 = 1,
  LED2 = 2
} Led_TypeDef;

typedef enum 
{  
  BUTTON_USER = 0
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;              

/** 
  * @brief  STM32F100 Button Defines Legacy  
  */ 

#define Button_USER          BUTTON_USER
#define Mode_GPIO            BUTTON_MODE_GPIO
#define Mode_EXTI            BUTTON_MODE_EXTI
#define Button_Mode_TypeDef  ButtonMode_TypeDef


/** @addtogroup STM32vldiscovery_LOW_LEVEL_LED
  * @{
  */
#define LEDn                             3
#define LED0_PIN                         GPIO_Pin_12
#define LED0_GPIO_PORT                   GPIOB
#define LED0_GPIO_CLK                    RCC_APB2Periph_GPIOB  

#define LED1_PIN                         GPIO_Pin_13
#define LED1_GPIO_PORT                   GPIOB
#define LED1_GPIO_CLK                    RCC_APB2Periph_GPIOB  

#define LED2_PIN                         GPIO_Pin_14
#define LED2_GPIO_PORT                   GPIOB
#define LED2_GPIO_CLK                    RCC_APB2Periph_GPIOB  

/**
  * @}
  */ 
  
/** @addtogroup STM32vldiscovery_LOW_LEVEL_BUTTON
  * @{
  */  
#define BUTTONn                          1

/* * @brief USER push-button
 */
#define USER_BUTTON_PIN                   GPIO_Pin_0
#define USER_BUTTON_GPIO_PORT             GPIOA
#define USER_BUTTON_GPIO_CLK              RCC_APB2Periph_GPIOA
#define USER_BUTTON_EXTI_PORT_SOURCE      GPIO_PortSourceGPIOA
#define USER_BUTTON_EXTI_PIN_SOURCE       GPIO_PinSource0
#define USER_BUTTON_EXTI_LINE             EXTI_Line0
#define USER_BUTTON_EXTI_IRQn             EXTI0_IRQn

/**
  * @}
  */ 

/** @defgroup STM32vldiscovery_LOW_LEVEL__Exported_Functions
  * @{
  */ 
void osm_LEDInit(Led_TypeDef Led);
void osm_LEDOn(Led_TypeDef Led);
void osm_LEDOff(Led_TypeDef Led);
void osm_LEDToggle(Led_TypeDef Led);
void osm_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t osm_PBGetState(Button_TypeDef Button);

/**
  * @}
  */ 
    
#ifdef __cplusplus
}
#endif


#endif /* __STM32vldiscovery_H */

/**
  * @}
  */ 

/**
  * @}
  */  

/**
  * @}
  */
  
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
