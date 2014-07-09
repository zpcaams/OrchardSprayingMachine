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
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbrtu.h"
#include "mbport.h"

/* ----------------------- Constant Definitions -----------------------------*/

#if defined COM1
    /**
     * @brief Definition for COM port1, connected to USART1
     */ 
    #define COMx                        USART1
    #define COMx_IRQn                   USART1_IRQn
    #define COMx_IRQHandler             USART1_IRQHandler
    #define COMx_CLK                    RCC_APB2Periph_USART1
    #define COMx_CLK_CONFIG             RCC_APB2PeriphClockCmd
    #define COMx_TX_PIN                 GPIO_Pin_9
    #define COMx_TX_GPIO_PORT           GPIOA
    #define COMx_TX_GPIO_CLK            RCC_APB2Periph_GPIOA
    #define COMx_RX_PIN                 GPIO_Pin_10
    #define COMx_RX_GPIO_PORT           GPIOA
    #define COMx_RX_GPIO_CLK            RCC_APB2Periph_GPIOA
    #define COMx_DR_Address             ((uint32_t)0x40013804)

    #define DMAx                        DMA1
    #define DMAx_IRQn                   DMA1_Channel4_IRQn
    #define DMAx_COMx_TX_Channel        DMA1_Channel4
    #define DMAx_ChannelX_IRQHandler    DMA1_Channel4_IRQHandler
    #define DMAx_FLAG_TCx               DMA1_FLAG_TC4

#elif defined COM2
    /**
     * @brief Definition for COM port2, connected to USART2
     */ 
    #define COMx                        USART2
    #define COMx_IRQn                   USART2_IRQn
    #define COMx_IRQHandler             USART2_IRQHandler
    #define COMx_CLK                    RCC_APB1Periph_USART2
    #define COMx_CLK_CONFIG             RCC_APB1PeriphClockCmd
    #define COMx_TX_PIN                 GPIO_Pin_2
    #define COMx_TX_GPIO_PORT           GPIOA
    #define COMx_TX_GPIO_CLK            RCC_APB2Periph_GPIOA
    #define COMx_RX_PIN                 GPIO_Pin_3
    #define COMx_RX_GPIO_PORT           GPIOA
    #define COMx_RX_GPIO_CLK            RCC_APB2Periph_GPIOA
    #define COMx_DR_Address             ((uint32_t)0x40004404)

    #define DMAx                        DMA1
    #define DMAx_IRQn                   DMA1_Channel7_IRQn
    #define DMAx_COMx_TX_Channel        DMA1_Channel7
    #define DMAx_ChannelX_IRQHandler    DMA1_Channel7_IRQHandler
    #define DMAx_FLAG_TCx               DMA1_FLAG_TC7


#elif defined COM3
    /**
     * @brief Definition for COM port3, connected to USART3
     */ 
    #define COMx                        USART3
    #define COMx_IRQn                   USART3_IRQn
    #define COMx_IRQHandler             USART3_IRQHandler
    #define COMx_CLK                    RCC_APB1Periph_USART3
    #define COMx_CLK_CONFIG             RCC_APB1PeriphClockCmd
    #define COMx_TX_PIN                 GPIO_Pin_10
    #define COMx_TX_GPIO_PORT           GPIOB
    #define COMx_TX_GPIO_CLK            RCC_APB2Periph_GPIOB
    #define COMx_RX_PIN                 GPIO_Pin_11
    #define COMx_RX_GPIO_PORT           GPIOB
    #define COMx_RX_GPIO_CLK            RCC_APB2Periph_GPIOB
    #define COMx_DR_Address             ((uint32_t)0x40004804)

    #define DMAx                        DMA1
    #define DMAx_IRQn                   DMA1_Channel2_IRQn
    #define DMAx_COMx_TX_Channel        DMA1_Channel2
    #define DMAx_ChannelX_IRQHandler    DMA1_Channel2_IRQHandler
    #define DMAx_FLAG_TCx               DMA1_FLAG_TC2

#else
    #error "COMx Port for RS485 Must Defined!"
#endif

/**
 * @brief Definition for 485 DR_REn
 */ 
#if defined DE_REn_PB12
    #define DE_REn_PIN                  GPIO_Pin_12
    #define DE_REn_GPIO_PORT            GPIOB
    #define DE_REn_GPIO_CLK             RCC_APB2Periph_GPIOB

#elif defined DE_REn_PA8
    #define DE_REn_PIN                  GPIO_Pin_8
    #define DE_REn_GPIO_PORT            GPIOA
    #define DE_REn_GPIO_CLK             RCC_APB2Periph_GPIOA

#elif defined DE_REn_PA11
    #define DE_REn_PIN                  GPIO_Pin_11
    #define DE_REn_GPIO_PORT            GPIOA
    #define DE_REn_GPIO_CLK             RCC_APB2Periph_GPIOA

#else
    #error "RS485 DE_REn Pin Must Defined!"
#endif

/**
 * @brief Definition for 485 PV
 */ 
#if defined PV_PB13
    #define PV_PIN                      GPIO_Pin_13
    #define PV_GPIO_PORT                GPIOB
    #define PV_GPIO_CLK                 RCC_APB2Periph_GPIOB

#elif defined PV_PA8
    #define PV_PIN                      GPIO_Pin_8
    #define PV_GPIO_PORT                GPIOA
    #define PV_GPIO_CLK                 RCC_APB2Periph_GPIOA

#else
    #error "RS485 PV Pin Must Defined!"
#endif
/* ----------------------- Type Definitions ---------------------------------*/
 
/* ----------------------- Macros (Inline Functions) Definitions ------------*/

/* ----------------------- static functions ---------------------------------*/
void prvvUARTTxReadyISR( void );
void prvvUARTRxISR( void );

/* ----------------------- Variable Definitions -----------------------------*/
static DMA_InitTypeDef DMA_InitStructure;

/* ----------------------- Start implementation -----------------------------*/

/**
  * @brief  串口初始化
  * @param  ucPORT      串口号
  *         ulBaudRate  波特率
  *         ucDataBits  数据位
  *         eParity     校验位 
  * @retval None
  */
BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
    (void)ucPORT;     //不修改端口
    (void)ucDataBits; //不修改数据位长度
    (void)eParity;    //不修改校验格式

/*************************** System Clocks Configuration **********************/
    /* DMA clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* Enable Usart and 485_DE_REn GPIO clock */
    RCC_APB2PeriphClockCmd(COMx_TX_GPIO_CLK | COMx_RX_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(DE_REn_GPIO_CLK, ENABLE);

    /* Enable UART clock */
    COMx_CLK_CONFIG(COMx_CLK, ENABLE);
  
/*************************** Configure the GPIO ports *************************/
    GPIO_InitTypeDef GPIO_InitStructure;
    /* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = COMx_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(COMx_TX_GPIO_PORT, &GPIO_InitStructure);

    /* Configure USART Rx as input floating */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = COMx_RX_PIN;
    GPIO_Init(COMx_RX_GPIO_PORT, &GPIO_InitStructure);

    /* Configure 485_DE_REn as output push-pull */
    GPIO_InitStructure.GPIO_Pin = DE_REn_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(DE_REn_GPIO_PORT, &GPIO_InitStructure);

    /* Configure PV as output push-pull and set ON*/
    GPIO_InitStructure.GPIO_Pin = PV_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(DE_REn_GPIO_PORT, &GPIO_InitStructure);
    GPIO_SetBits(PV_GPIO_PORT, PV_PIN);
    
/*************************** NVIC configuration *******************************/
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Enable the COMx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = COMx_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* Enable the DMAx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = DMAx_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
/*************************** Configure the DMA ********************************/
    /* Init COMx DMA */
    DMA_DeInit(DMAx_COMx_TX_Channel);
    DMA_InitStructure.DMA_PeripheralBaseAddr = COMx_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)xMBRTUGetRTUBuf();
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMAx_COMx_TX_Channel, &DMA_InitStructure);
    
/*************************** Configure COMx ***********************************/ 
    USART_InitTypeDef USART_InitStructure;
    /* USARTx configured as follow:
        - BaudRate = ulBaudRate  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
    */
    USART_InitStructure.USART_BaudRate = ulBaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    /* USART configuration */
    USART_Init(COMx, &USART_InitStructure);
    
    /* Enable COMx DMA TX request */
    USART_DMACmd(COMx, USART_DMAReq_Tx, ENABLE);
    
    /* Enable USART */
    USART_Cmd(COMx, ENABLE);
    
    return TRUE;
}

/**
  * @brief  控制接收和发送状态
  * @param  xRxEnable 接收使能、
  *         xTxEnable 发送使能
  * @retval None
  */
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    if(xRxEnable){
        USART_ITConfig(COMx, USART_IT_RXNE, ENABLE);
        GPIO_ResetBits(DE_REn_GPIO_PORT,DE_REn_PIN);
    }else{
        USART_ITConfig(COMx, USART_IT_RXNE, DISABLE);
        GPIO_SetBits(DE_REn_GPIO_PORT,DE_REn_PIN);
    }

    if(xTxEnable){        
        /* DMA_InitStructure has been initiallized before,
            Only Update Tx Buffer Size Here */
        DMA_InitStructure.DMA_BufferSize = xMBRTUGetSndBufCnt();
        DMA_Init(DMAx_COMx_TX_Channel, &DMA_InitStructure);
        
        /* Enable COMx DMA TX Channel and Interrupt */
        DMA_ITConfig(DMAx_COMx_TX_Channel, DMA_IT_TC, ENABLE);
        DMA_Cmd(DMAx_COMx_TX_Channel, ENABLE);
    }else{
        DMA_ITConfig(DMAx_COMx_TX_Channel, DMA_IT_TC, DISABLE);
        
        DMA_Cmd(DMAx_COMx_TX_Channel, DISABLE);
    }
}

/**
  * @brief  通过串口发送数据
  * @param  None
  * @retval None
  */
BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
  //发送数据
  USART_SendData(COMx, ucByte);
  return TRUE;
}

/**
  * @brief  从串口获得数据
  * @param  None
  * @retval None
  */
BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
  //接收数据
  *pucByte = USART_ReceiveData(COMx);
  return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
void prvvUARTTxReadyISR( void )
{
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR( void )
{
}
 
/**
  * @brief  COMx接收中断服务函数
  * @param  None
  * @retval None
  */
void COMx_IRQHandler(void)
{
    static BOOL     xTaskWokenReceive = pdFALSE;
    
    if(USART_GetITStatus(COMx, USART_IT_RXNE) == SET){
        reset_com_flash_counter();
        xTaskWokenReceive = pxMBFrameCBByteReceived();
        USART_ClearITPendingBit(COMx, USART_IT_RXNE);
    }
    portEND_SWITCHING_ISR(( xTaskWokenReceive ) ? pdTRUE : pdFALSE);

}

/**
  * @brief  COMx DMA 发送完成中断服务函数
  * @param  None
  * @retval None
  */
void DMAx_ChannelX_IRQHandler(void)
{
    static BOOL     xTaskWokenTransmit = pdFALSE;
    
    if(DMA_GetFlagStatus(DMAx_FLAG_TCx) == SET){
        xTaskWokenTransmit = pxMBFrameCBTransmitterEmpty();
        DMA_ClearITPendingBit(DMAx_FLAG_TCx);
    }
    portEND_SWITCHING_ISR(( xTaskWokenTransmit ) ? pdTRUE : pdFALSE);

}