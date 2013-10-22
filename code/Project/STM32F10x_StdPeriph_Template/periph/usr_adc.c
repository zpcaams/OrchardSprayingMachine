/*****************************************************************************/
/**
* @file usr_adc.c
*
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "usr_adc.h"

/************************** Constant Definitions *****************************/
#define	ADCx  ADC1
#define	DMAx_Channelx  DMA1_Channel1
#define ADCx_DR_Address    ((uint32_t)0x4001244C)

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
static void RCC_Configuration(void);
static void GPIO_Configuration(void);

/************************** Variable Definitions *****************************/

u16 adc_buf[ADC_BUF_SIZE];

void adc_init(void)
{
  	/* ADC1 configuration *******************************************************/
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
    
    /* System clocks configuration ---------------------------------------------*/
    RCC_Configuration();

    /* GPIO configuration ------------------------------------------------------*/
    GPIO_Configuration();
    
    /* DMA1 channel1 configuration ----------------------------------------------*/
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADCx_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&adc_buf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = ADC_BUF_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    /* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = CHANNEL_NUM;
    ADC_Init(ADCx, &ADC_InitStructure);
    
	/* ADC1 regular channel configuration *************************************/
	/* With an ADCCLK = 12 MHz and a sampling time of 239.5 cycles:
        Tconv = 239.5 + 12.5 = 252 cycles = 21 ¦Ìs
        For all 5 channel takes 105us */
    ADC_RegularChannelConfig(ADCx, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_5, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_6, 3, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_7, 4, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_8, 5, ADC_SampleTime_239Cycles5);
	
    /* Enable DMA1 channel1 */
    DMA_Cmd(DMAx_Channelx, ENABLE);

    /* Enable ADC1 DMA */
    ADC_DMACmd(ADCx, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADCx, ENABLE);
  
    /* Enable ADC1 reset calibration register */   
    ADC_ResetCalibration(ADCx);
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADCx));

    /* Start ADC1 calibration */
    ADC_StartCalibration(ADCx);
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADCx));
     
    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
static void RCC_Configuration(void)
{
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
  /* ADCCLK = PCLK2/2 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div2); 
#else
  /* ADCCLK = PCLK2/4 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
#endif
  /* Enable peripheral clocks ------------------------------------------------*/
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* Enable ADC1 and GPIOA, GPIOB clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA 
                         | RCC_APB2Periph_GPIOB, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure
    
    PA0 PA1 PA2 PA3 PA4 PA5 PA6 PA7
                    *   *   *   *
    PB0 PB1
    *
    pin as analog input */
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
