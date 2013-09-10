/*****************************************************************************/
/**
*
* @file osm_ctrl.h
*
*
******************************************************************************/

#ifndef OSM_CTRL_H
#define OSM_CTRL_H

#ifdef __cplusplus
	extern "C" {
#endif

/***************************** Include Files *********************************/

#include "osm_common.h"
#include "osm_adc.h"
      
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void GreatOSMCtrlTask(void);
void OSMCtrlTask (void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif /* OSM_CTRL_H */
