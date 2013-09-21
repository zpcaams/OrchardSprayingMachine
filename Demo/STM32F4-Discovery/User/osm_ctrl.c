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

/*
 * 1. Convert Length(350~2500 mm) to Voltage(0~3000 mV)
 *  Voltage = (3000/2150)*(Length - 350);
 * 2. Convert Voltage(0~3300) to ADC Value(0~0xFFF)
 * 	AdcVal = (4095/3300)*Voltage;
 * 3. Convert Length(350~2500 mm) to ADC Value(0~0xFFF)
 * 	AdcVal = (4095/3300)*(3000/2150)*(Length - 350);
 * 		   = (819/473)(Length - 350);
 */
#define CnvLenToAdc(Length)	((819*((Length) - 350))/473)
#define CnvAdcToLen(Adc)	((473*Adc + 350*819)/819)

#define TotalChannelNum	6
/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

const u16 LLenMin[TotalChannelNum] = {1500, 1500, 1500, 1500, 1500, 1500};
const u16 LLenMax[TotalChannelNum] = {2500, 2500, 2500, 2500, 2500, 2500};
const u16 RLenMin[TotalChannelNum] = {1500, 1500, 1500, 1500, 1500, 1500};
const u16 RLenMax[TotalChannelNum] = {2500, 2500, 2500, 2500, 2500, 2500};

static const char *CtrlTaskName[TotalChannelNum] = {
        "Ctrl0", "Ctrl1", "Ctrl2", "Ctrl3", "Ctrl4","Ctrl5"
};
const u8 CtrlPara[TotalChannelNum] = {
        0, 1, 2, 3, 4, 5
};

extern u16 ADCx_Buffer[ADCx_BUFFER_SIZE];

void OSMDispTask (void *pvParameters)
{
	u8  i;
	u32	Length[SENSOR_NUM];
    portTickType xLastWakeTime;
    
    xLastWakeTime = xTaskGetTickCount();
    for( ; ; )
    {	
        for(i=0;i<SENSOR_NUM;i++){
        	Length[i] = CnvAdcToLen(ADCx_Buffer[i]);
        	printf("Ch%i:%i\n", i, Length[i]);
        }
        printf("\n");
        vTaskDelayUntil( &xLastWakeTime, 2000 / portTICK_RATE_MS );
    }
}

/*
 * Control Left(Right) Valve by the Value of Sensor
 * 	Valve	Sensor
 *	0		0	1	2
 *	1			1	2	3
 *	2				2	3	4
 *	3		5	6	7
 *	4			6	7	8
 *	5				7	8	9
 *
 */
void OSMCtrlTask (void *pvParameters)
{
	u8	i;
	u8	Channel, SensorChannel;
	u8	Spray;
    u32 AdcMin, AdcMax;
    portTickType xLastWakeTime;

    Channel = *((u8 *)pvParameters);
    if(Channel<3){
        SensorChannel = Channel;
    }else{
        SensorChannel = Channel + 2;
    }

    xLastWakeTime = xTaskGetTickCount();
    for( ; ; )
    {
    	Spray = 0;
        for (i=SensorChannel;i<SensorChannel+3;i++){
            if(Channel<3){
                AdcMin = CnvLenToAdc(LLenMin[i]);
                AdcMax = CnvLenToAdc(LLenMax[i]);
            }else{
                AdcMin = CnvLenToAdc(RLenMin[i-5]);
                AdcMax = CnvLenToAdc(RLenMax[i-5]);
            }
            if ((ADCx_Buffer[i]>AdcMin)&&(ADCx_Buffer[i]<AdcMax)){
            	Spray = 1;
            }
        }
        //Valve 0,1 is not used, Valve Num start from 2 to 7.
        if(Spray==1){
        	GpoOff(Channel+2);
        }else{
        	GpoOn(Channel+2);
        }
        vTaskDelayUntil( &xLastWakeTime, 100 / portTICK_RATE_MS );
    }
}

void GreatOSMCtrlTask(void)
{
	u8	i;

	for(i=0;i<TotalChannelNum;i++){
		xTaskCreate( OSMCtrlTask, ( signed char * ) CtrlTaskName[i],
			  configMINIMAL_STACK_SIZE, (void *)(&CtrlPara[i]), OSMCtrl_TASK_PRIORITY, NULL );
	}
	xTaskCreate( OSMDispTask, ( signed char * ) "Disp",
		  configMINIMAL_STACK_SIZE, NULL, OSMDisp_TASK_PRIORITY, NULL );
}
