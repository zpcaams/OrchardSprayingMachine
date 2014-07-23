/*****************************************************************************/
/**
* @file modbus_regs.c
*
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "modbus_regs.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/* ��Ȧ״̬(R/W) */
/*
[0-3] Mosfet Output
*/
uint8_t ucRegCoilsBuf[REG_COILS_SIZE / 8] = {0x01,0x02};

/* ����״̬(R) */
/*
[0] IO input
*/
uint8_t ucRegDiscreteBuf[REG_DISCRETE_SIZE / 8] = {0x52,0xA1};

/* ����Ĵ���(R) */
/*
[0-5] ADC input
*/
uint16_t usRegInputBuf[REG_INPUT_NREGS] = {0x1000,0x1001,0x1002,0x1003,0x1004,0x1005,0x1006,0x1007};
/* ����Ĵ�����ʼ��ַ */
uint16_t usRegInputStart = REG_INPUT_START;

/* ���ּĴ���(R/W) */
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS] = {0x147b,0x3f8e,0x147b,0x400e,0x1eb8,0x4055,0x147b,0x408e};
/* ���ּĴ�����ʼ��ַ */
uint16_t usRegHoldingStart = REG_HOLDING_START;

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
  eMBErrorCode    eStatus = MB_ENOERR;
  int16_t         iNCoils = ( int16_t )usNCoils;
  int16_t         usBitOffset;
  uint8_t       *pucRegCoilsBuf = ucRegCoilsBuf;

  //���Ĵ����Ƿ���ָ����Χ��
  if( ( (int16_t)usAddress >= REG_COILS_START ) &&
        ( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) )
  {
    //����Ĵ���ƫ����
    usBitOffset = ( int16_t )( usAddress - REG_COILS_START );
    switch ( eMode )
    {
      case MB_REG_READ:
        while( iNCoils > 0 )
        {
          *pucRegBuffer++ = xMBUtilGetBits( pucRegCoilsBuf, usBitOffset,
                                          ( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) );
          pucRegCoilsBuf++;
          iNCoils -= 8;
          usBitOffset += 8;
        }
        break;

      case MB_REG_WRITE:
        while( iNCoils > 0 )
        {
          xMBUtilSetBits( pucRegCoilsBuf, usBitOffset,
                        ( uint8_t )( iNCoils > 8 ? 8 : iNCoils ),
                        *pucRegBuffer++ );
          pucRegCoilsBuf++;
          iNCoils -= 8;
        }
        /* Bit0-3 */
        if((ucRegCoilsBuf[0] & 0x01) == 0){
            GPIO_SetBits(GPIOA, GPIO_Pin_8);
        }else{
            GPIO_ResetBits(GPIOA, GPIO_Pin_8);
        }
        
        if((ucRegCoilsBuf[0] & 0x02) == 0){
            GPIO_SetBits(GPIOA, GPIO_Pin_9);
        }else{
            GPIO_ResetBits(GPIOA, GPIO_Pin_9);
        }
        
        if((ucRegCoilsBuf[0] & 0x04) == 0){
            GPIO_SetBits(GPIOA, GPIO_Pin_10);
        }else{
            GPIO_ResetBits(GPIOA, GPIO_Pin_10);
        }
        
        if((ucRegCoilsBuf[0] & 0x08) == 0){
            GPIO_SetBits(GPIOA, GPIO_Pin_11);
        }else{
            GPIO_ResetBits(GPIOA, GPIO_Pin_11);
        }
        break;
    }

  }
  else
  {
    eStatus = MB_ENOREG;
  }
  return eStatus;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
  eMBErrorCode    eStatus = MB_ENOERR;
  int16_t         iNDiscrete = ( int16_t )usNDiscrete;
  uint16_t        usBitOffset;
  uint8_t       *pucRegCoilsBuf = ucRegDiscreteBuf;

  //�жϼĴ���ʱ�����ƶ���Χ��
  if( ( (int16_t)usAddress >= REG_DISCRETE_START ) &&
        ( usAddress + usNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_SIZE ) )
  {
    //���ƫ����
    usBitOffset = ( uint16_t )( usAddress - REG_DISCRETE_START );
    
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)){
        ucRegDiscreteBuf[0] |= 0x01;
    }else{
        ucRegDiscreteBuf[0] &= ~0x01;
    }
    while( iNDiscrete > 0 )
    {
      *pucRegBuffer++ = xMBUtilGetBits( pucRegCoilsBuf, usBitOffset,
                                      ( uint8_t)( iNDiscrete > 8 ? 8 : iNDiscrete ) );
      iNDiscrete -= 8;
      usBitOffset += 8;
    }

  }
  else
  {
    eStatus = MB_ENOREG;
  }
  return eStatus;
}

eMBErrorCode 
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
  eMBErrorCode    eStatus = MB_ENOERR;
  int16_t         iRegIndex;
  
  //��ѯ�Ƿ��ڼĴ�����Χ��
  if( ( usAddress >= REG_INPUT_START ) \
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
  {
    //��ò���ƫ���������β�����ʼ��ַ-����Ĵ����ĳ�ʼ��ַ
    iRegIndex = ( int16_t )( usAddress - usRegInputStart );
    
    while( usNRegs > 0 )
    {
      *pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] >> 8 );
      *pucRegBuffer++ = ( uint8_t )( usRegInputBuf[iRegIndex] & 0xFF );
      iRegIndex++;
      usNRegs--;
    }
  }
  else
  {
    eStatus = MB_ENOREG;
  }

  return eStatus;
}

eMBErrorCode 
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{
  eMBErrorCode    eStatus = MB_ENOERR;
  int16_t         iRegIndex;
  
  //�жϼĴ����ǲ����ڷ�Χ��
  if( ( (int16_t)usAddress >= REG_HOLDING_START ) \
     && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
  {
    //����ƫ����
    iRegIndex = ( int16_t )( usAddress - usRegHoldingStart );
    
    switch ( eMode )
    {
      case MB_REG_READ:
        while( usNRegs > 0 )
        {
          *pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] >> 8 );
          *pucRegBuffer++ = ( uint8_t )( usRegHoldingBuf[iRegIndex] & 0xFF );
          iRegIndex++;
          usNRegs--;
        }
        break;

      case MB_REG_WRITE:
        while( usNRegs > 0 )
        {
          /* HoldingCB[0] is dac reg */
          if(iRegIndex == 0){
            u16 dac_reg;
            dac_reg = *pucRegBuffer++ << 8;
            dac_reg |= *pucRegBuffer++;
            DAC_SetChannel1Data(DAC_Align_12b_R, dac_reg);
          }
          usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
          usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
          iRegIndex++;
          usNRegs--;
        }
        break;
     }
  }
  else
  {
    eStatus = MB_ENOREG;
  }
  
  return eStatus;
}
