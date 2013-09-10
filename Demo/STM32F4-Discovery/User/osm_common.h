/*****************************************************************************/
/**
*
* @file osm_common.h
*
*
******************************************************************************/

#ifndef OSM_COMMON_H
#define OSM_COMMON_H

#ifdef __cplusplus
 extern "C" {
#endif

/***************************** Include Files *********************************/

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* ST Driver includes. */
#include "stm32f4xx_conf.h"
 
/************************** Constant Definitions *****************************/

#define OSMCtrl_TASK_PRIORITY				    ( tskIDLE_PRIORITY + 2UL )
 
/**************************** Type Definitions *******************************/
 
/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

#ifdef __cplusplus
}
#endif

#endif /* OSM_COMMON_H */
