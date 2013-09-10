/*****************************************************************************/
/**
*
* @file osm_adc.h
*
*
******************************************************************************/

#ifndef OSM_ADC_H
#define OSM_ADC_H

#ifdef __cplusplus
	extern "C" {
#endif

/***************************** Include Files *********************************/
#include "osm_common.h"
	
/************************** Constant Definitions *****************************/

#define	SENSOR_NUM	10
#define	AVERAGE_NUM	1
#define	ADCx_BUFFER_SIZE	(SENSOR_NUM*AVERAGE_NUM)
	
/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void AdcInitialise(void);

#ifdef __cplusplus
}
#endif

#endif /* OSM_ADC_H */
