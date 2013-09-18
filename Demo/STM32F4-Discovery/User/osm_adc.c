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

#include "osm_adc.h"

/************************** Constant Definitions *****************************/

#define	ADCx  ADC1

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

u16 ADCx_Buffer[ADCx_BUFFER_SIZE];

void AdcInitialise(void)
{
	/* ADC1 configuration *******************************************************/
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;
	
	/* Enable ADC1, DMA2 and GPIO clocks ****************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOA 
                         | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	/* DMA2 Stream0 channel0 configuration **************************************/
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&ADCx->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADCx_Buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = ADCx_BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	
	/* Configure
    
    PA0 PA1 PA2 PA3 PA4 PA5 PA6 PA7
        *   *   *
    PB0 PB1 PC0 PC1 PC2 PC3 PC4 PC5
    *   *   *   *   *       *   *  
    pin as analog input */
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
	/* ADC Common Init **********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	/* ADC1 Init ****************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = ADCx_BUFFER_SIZE;
	ADC_Init(ADCx, &ADC_InitStructure);
	
	/* ADC1 regular channel configuration *************************************/
	ADC_RegularChannelConfig(ADCx, ADC_Channel_1, 5, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_2, 6, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_3, 10, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_8, 1, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_9, 2, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_10, 9, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_11, 8, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_12, 7, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_14, 3, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_15, 4, ADC_SampleTime_480Cycles);
	
	/* Enable DMA request after last transfer (Single-ADC mode) */
	ADC_DMARequestAfterLastTransferCmd(ADCx, ENABLE);
	
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADCx, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADCx, ENABLE);

	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConv(ADCx);
}