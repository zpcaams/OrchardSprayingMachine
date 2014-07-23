/*****************************************************************************/
/**
*
* @file modbus_regs.h
*
*
******************************************************************************/

#ifndef _MODBUS_REGS_H
#define _MODBUS_REGS_H

#ifdef __cplusplus
	extern "C" {
#endif
      
/***************************** Include Files *********************************/
#include "stm32f10x.h"
#include "mb.h"
#include "mbutils.h"
	
/************************** Constant Definitions *****************************/

//��Ȧ��ʼ��ַ
#define REG_COILS_START       0x0000
//��Ȧ����
#define REG_COILS_SIZE        16

//���ؼĴ�����ʼ��ַ
#define REG_DISCRETE_START    0x0000
//���ؼĴ�������
#define REG_DISCRETE_SIZE     16

//����Ĵ�����ʼ��ַ
#define REG_INPUT_START       0x0000
//����Ĵ�������
#define REG_INPUT_NREGS       8

//���ּĴ�����ʼ��ַ
#define REG_HOLDING_START     0x0000
//���ּĴ�������
#define REG_HOLDING_NREGS     8

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
      
#ifdef __cplusplus
}
#endif

#endif /* _MODBUS_REGS_H */
