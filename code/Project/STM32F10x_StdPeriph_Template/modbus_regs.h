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

//线圈起始地址
#define REG_COILS_START       0x0000
//线圈数量
#define REG_COILS_SIZE        16

//开关寄存器起始地址
#define REG_DISCRETE_START    0x0000
//开关寄存器数量
#define REG_DISCRETE_SIZE     16

//输入寄存器起始地址
#define REG_INPUT_START       0x0000
//输入寄存器数量
#define REG_INPUT_NREGS       8

//保持寄存器起始地址
#define REG_HOLDING_START     0x0000
//保持寄存器数量
#define REG_HOLDING_NREGS     8

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
      
#ifdef __cplusplus
}
#endif

#endif /* _MODBUS_REGS_H */
