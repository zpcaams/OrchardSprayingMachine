/*****************************************************************************/
/**
*
* @file usr_adc.h
*
*
******************************************************************************/

#ifndef USR_ADC_H
#define USR_ADC_H

#ifdef __cplusplus
	extern "C" {
#endif

/***************************** Include Files *********************************/

#include "stm32f10x.h"
#include "modbus_regs.h"
	
/************************** Constant Definitions *****************************/

#define	CHANNEL_NUM	6
#define	AVERAGE_NUM	1
#define	ADC_BUF_SIZE	(CHANNEL_NUM*AVERAGE_NUM)
	
/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void adc_init(void);

#ifdef __cplusplus
}
#endif

#endif /* USR_ADC_H */
