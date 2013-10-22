/**
  ******************************************************************************
  * @file    TIM/PWM_Output/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usr_pwm_out.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup TIM_PWM_Output
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t ARR_Val = 1000 - 1;
uint16_t CCR1_Val = 1000;
uint16_t CCR2_Val = 1000;
uint16_t CCR3_Val = 750;
uint16_t CCR4_Val = 0;
uint16_t PrescalerValue = 0;

/* Private function prototypes -----------------------------------------------*/
static void RCC_Configuration(void);
static void GPIO_Configuration(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void pwm_out_init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    /* System Clocks Configuration */
    RCC_Configuration();

    /* GPIO Configuration */
    GPIO_Configuration();

    /* -----------------------------------------------------------------------
    TIM4 Configuration: generate 4 PWM signals with 4 different duty cycles:
    The TIM4CLK frequency is set to SystemCoreClock (Hz), to get TIM4 counter
    clock at 24 MHz the Prescaler is computed as following:
     - Prescaler = (TIM4CLK / TIM4 counter clock) - 1
    SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
    and Connectivity line devices and to 24 MHz for Low-Density Value line and
    Medium-Density Value line devices

    The TIM4 is running at 36 KHz: TIM4 Frequency = TIM4 counter clock/(ARR + 1)
                                                  = 24 MHz / 1000 = 24 KHz
    TIM4 Channel1 duty cycle = (TIM4_CCR1/ TIM4_ARR)* 100 = 100%
    TIM4 Channel2 duty cycle = (TIM4_CCR2/ TIM4_ARR)* 100 = 100%
    TIM4 Channel3 duty cycle = (TIM4_CCR3/ TIM4_ARR)* 100 = 75%
    TIM4 Channel4 duty cycle = (TIM4_CCR4/ TIM4_ARR)* 100 = 0%
    ----------------------------------------------------------------------- */
    /* Compute the prescaler value */
    PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = ARR_Val;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR1_Val;

    TIM_OC1Init(TIM4, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR2_Val;

    TIM_OC2Init(TIM4, &TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel3 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR3_Val;

    TIM_OC3Init(TIM4, &TIM_OCInitStructure);

    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel4 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

    TIM_OC4Init(TIM4, &TIM_OCInitStructure);

    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM4, ENABLE);

    /* TIM4 enable counter */
    TIM_Cmd(TIM4, ENABLE);
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
static void RCC_Configuration(void)
{
    /* TIM4 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* GPIOB clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
}

/**
  * @brief  Configure the TIM4 Ouput Channels.
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* GPIOB Configuration:TIM4 Channel 2, 3 and 4 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
