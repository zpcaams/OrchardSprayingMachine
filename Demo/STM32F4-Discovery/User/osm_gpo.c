/*****************************************************************************/
/**
* @file osm_adc.c
*
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "osm_gpo.h"

/************************** Constant Definitions *****************************/

const uint16_t GPO_PIN[GPOn] = {    GPO0_PIN, GPO1_PIN, GPO2_PIN, GPO3_PIN, 
                                    GPO4_PIN, GPO5_PIN, GPO6_PIN, GPO7_PIN};

const uint32_t GPO_CLK[GPOn] = {    GPO0_CLK, GPO1_CLK, GPO2_CLK, GPO3_CLK,
                                    GPO4_CLK, GPO5_CLK, GPO6_CLK, GPO7_CLK};

/**************************** Type Definitions *******************************/

GPIO_TypeDef* GPO_PORT[GPOn] = {    GPO0_PORT, GPO1_PORT, GPO2_PORT, GPO3_PORT,
                                    GPO4_PORT, GPO5_PORT, GPO6_PORT, GPO7_PORT};

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

void GpoInit(void)
{
  uint32_t GPO_CLKn = 0;
  Gpo_TypeDef Gpo;
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPO Clock */
  for(Gpo=GPO0;Gpo<GPOn;Gpo++){
    GPO_CLKn |= GPO_CLK[Gpo];
  }
  RCC_AHB1PeriphClockCmd(GPO_CLKn, ENABLE);

  /* Configure the GPO pin */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  
  for(Gpo=GPO0;Gpo<GPOn;Gpo++){
      GPIO_InitStructure.GPIO_Pin = GPO_PIN[Gpo];
      GPIO_Init(GPO_PORT[Gpo], &GPIO_InitStructure);
  }
}

void GpoOn(Gpo_TypeDef Gpo)
{
  GPO_PORT[Gpo]->BSRRL = GPO_PIN[Gpo];
}

void GpoOff(Gpo_TypeDef Gpo)
{
  GPO_PORT[Gpo]->BSRRH = GPO_PIN[Gpo];  
}

void GpoToggle(Gpo_TypeDef Gpo)
{
  GPO_PORT[Gpo]->ODR ^= GPO_PIN[Gpo];
}

void GpoTestTask (void *pvParameters)
{
    Gpo_TypeDef Gpo;
    portTickType xLastWakeTime;
    
    xLastWakeTime = xTaskGetTickCount();
    for( ; ; )
    {
        for(Gpo=GPO0;Gpo<GPOn;Gpo++){
          GpoToggle(Gpo);
        }
        vTaskDelayUntil( &xLastWakeTime, 300 / portTICK_RATE_MS );
    }
}