/*****************************************************************************/
/**
* @file osm_ctrl.c
*
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "osm_ctrl.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

extern u16 ADCx_Buffer[ADCx_BUFFER_SIZE];
u32 Length[SENSOR_NUM];

void OSMDispTask (void *pvParameters)
{
	u8  i;
    portTickType xLastWakeTime;
    
    xLastWakeTime = xTaskGetTickCount();
    for( ; ; )
    {	
        for(i=0;i<SENSOR_NUM;i++){
          printf("Ch%i:%i\n", i, Length[i]);
        }
        printf("\n");
        vTaskDelayUntil( &xLastWakeTime, 2000 / portTICK_RATE_MS );
    }
}

void OSMCtrlTask (void *pvParameters)
{
	u8  i;
    portTickType xLastWakeTime;
    
    xLastWakeTime = xTaskGetTickCount();
    for( ; ; )
    {	
        for(i=0;i<SENSOR_NUM;i++){
//            ConvertedValue[i] = ADCx_Buffer[i] *3300/(0xFFF);
//            Length[i] = (ConvertedValue[i]*2150 + 350*3000)/3000;
            Length[i] = (ADCx_Buffer[i]*33*215 + 350*3*(0xFFF))/(3*0xFFF);
        }
        vTaskDelayUntil( &xLastWakeTime, 100 / portTICK_RATE_MS );
    }
}

void GreatOSMCtrlTask(void)
{
	xTaskCreate( OSMCtrlTask, ( signed char * ) "ctrl",
		  configMINIMAL_STACK_SIZE, NULL, OSMCtrl_TASK_PRIORITY, NULL );
    
	xTaskCreate( OSMDispTask, ( signed char * ) "disp",
		  configMINIMAL_STACK_SIZE, NULL, OSMDisp_TASK_PRIORITY, NULL );
    
	
}