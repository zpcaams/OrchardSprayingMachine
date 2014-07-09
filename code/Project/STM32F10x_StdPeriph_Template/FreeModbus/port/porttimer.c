/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Constant Definitions -----------------------------*/
#define TIMx                TIM7
#define TIMx_IRQn           TIM7_IRQn
#define TIMx_IRQHandler     TIM7_IRQHandler
#define RCC_APB1Periph_TIMx RCC_APB1Periph_TIM7

/* ----------------------- static functions ---------------------------------*/

/* ----------------------- Start implementation -----------------------------*/
/**
  * @brief  Timer Initialize
  * @param  None
  * @retval None
  */
BOOL
xMBPortTimersInit( USHORT usTimeOut50us )
{
    /* TIMx is used as Modbus timer */
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    uint16_t PrescalerValue = 0;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx, ENABLE);
    
    /* Timer Config 
      SystemCoreClock / (PrescalerValue + 1) = 20KHz (50us)
      Timer Count Max = usTimeOut50us
    */

    /* Compute the prescaler value */
    PrescalerValue = (uint16_t) (SystemCoreClock / 20000) - 1; 
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = (uint16_t) usTimeOut50us;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(TIMx, ENABLE);

    /**
      * @brief  Configure the nested vectored interrupt controller.
      * @param  None
      * @retval None
      */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    /* Enable the TIMx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    vMBPortTimersDisable();
    return TRUE;
}


void
vMBPortTimersEnable(  )
{
  /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
  
  TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
  TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
  TIM_SetCounter(TIMx,0x0000);
  TIM_Cmd(TIMx, ENABLE);
}

void
vMBPortTimersDisable(  )
{
   /* Disable any pending timers. */
  TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
  TIM_ITConfig(TIMx, TIM_IT_Update, DISABLE);
  TIM_SetCounter(TIMx,0x0000);
  TIM_Cmd(TIMx, DISABLE);
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
void TIMx_IRQHandler(void)
{
    static portBASE_TYPE xTaskSwitch = pdFALSE;
    
    if (TIM_GetITStatus(TIMx, TIM_IT_Update) != RESET)
    {
        xTaskSwitch |= pxMBPortCBTimerExpired();
        
        TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
    }
  
    portEND_SWITCHING_ISR( xTaskSwitch );
}