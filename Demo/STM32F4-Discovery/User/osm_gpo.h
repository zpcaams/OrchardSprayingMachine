/*****************************************************************************/
/**
*
* @file osm_gpo.h
*
*
******************************************************************************/

#ifndef OSM_GPO_H
#define OSM_GPO_H

#ifdef __cplusplus
	extern "C" {
#endif

/***************************** Include Files *********************************/
#include "osm_common.h"
	
/************************** Constant Definitions *****************************/

#define GPOn                        8

#define GPO0_PIN                    GPIO_Pin_0
#define GPO0_PORT                   GPIOD
#define GPO0_CLK                    RCC_AHB1Periph_GPIOD  
      
#define GPO1_PIN                    GPIO_Pin_1
#define GPO1_PORT                   GPIOD
#define GPO1_CLK                    RCC_AHB1Periph_GPIOD  
  
#define GPO2_PIN                    GPIO_Pin_2
#define GPO2_PORT                   GPIOD
#define GPO2_CLK                    RCC_AHB1Periph_GPIOD  
  
#define GPO3_PIN                    GPIO_Pin_3
#define GPO3_PORT                   GPIOD
#define GPO3_CLK                    RCC_AHB1Periph_GPIOD  
  
#define GPO4_PIN                    GPIO_Pin_4
#define GPO4_PORT                   GPIOD
#define GPO4_CLK                    RCC_AHB1Periph_GPIOD  
      
#define GPO5_PIN                    GPIO_Pin_5
#define GPO5_PORT                   GPIOD
#define GPO5_CLK                    RCC_AHB1Periph_GPIOD  
  
#define GPO6_PIN                    GPIO_Pin_6
#define GPO6_PORT                   GPIOD
#define GPO6_CLK                    RCC_AHB1Periph_GPIOD
      
#define GPO7_PIN                    GPIO_Pin_7
#define GPO7_PORT                   GPIOD
#define GPO7_CLK                    RCC_AHB1Periph_GPIOD
      
/**************************** Type Definitions *******************************/

typedef enum 
{
  GPO0 = 0,
  GPO1 = 1,
  GPO2 = 2,
  GPO3 = 3,
  GPO4 = 4,
  GPO5 = 5,
  GPO6 = 6,
  GPO7 = 7
} Gpo_TypeDef;

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void GpoInit(void);
void GpoOn(Gpo_TypeDef Gpo);
void GpoOff(Gpo_TypeDef Gpo);
void GpoTestTask (void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif /* OSM_GPO_H */
