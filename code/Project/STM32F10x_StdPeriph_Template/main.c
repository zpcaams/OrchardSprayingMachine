/**
  ******************************************************************************
  * @file    USART/Printf/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* FreeModbus includes. */
#include "mb.h"
#include "modbus_regs.h"

#include <stdio.h>
#include "stm32f10x.h"
#include "osm.h"
#include "usr_adc.h"
#include "usr_pwm_out.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define	mainLED_TEST_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define	MB_TASK_PRIORITY        		( tskIDLE_PRIORITY + 2 )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;

/* Private function prototypes -----------------------------------------------*/
static void mb_task( void *pvParameters );

extern uint8_t ucRegDiscreteBuf[REG_DISCRETE_SIZE / 8];
extern uint16_t usRegInputBuf[REG_INPUT_NREGS];

/*
 * Setup the NVIC, LED outputs.
 */
static void prvSetupHardware( void );
static void prvLEDTestTask (void *pvParameters);

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
  
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     
       
    prvSetupHardware();
    xTaskCreate( prvLEDTestTask, ( signed char * ) "ldTst", 
                configMINIMAL_STACK_SIZE, NULL, mainLED_TEST_TASK_PRIORITY, NULL );
    xTaskCreate( mb_task, ( signed char * ) "MB", 
                configMINIMAL_STACK_SIZE, NULL, MB_TASK_PRIORITY, NULL );


//    /* Start the tasks and timer running. */
    vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for( ;; );
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
//  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* Loop until the end of transmission */
//  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */ 

/**
  * @}
  */
static void prvLEDTestTask (void *pvParameters)
{
    portTickType xLastWakeTime;
    
    xLastWakeTime = xTaskGetTickCount();
    for( ; ; )
    {
        osm_LEDToggle(LED0);
        
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)){
            //All On
            GPIO_ResetBits(GPIOA, GPIO_Pin_8);
            GPIO_ResetBits(GPIOA, GPIO_Pin_9);
            GPIO_ResetBits(GPIOA, GPIO_Pin_10);
        }else{
            if(usRegInputBuf[0]<2300){
                //Relay On
                GPIO_ResetBits(GPIOA, GPIO_Pin_8);
            }else{
                //Relay Off
                GPIO_SetBits(GPIOA, GPIO_Pin_8);
            }
            
            if(usRegInputBuf[1]<2200){
                GPIO_ResetBits(GPIOA, GPIO_Pin_9);
            }else{
                GPIO_SetBits(GPIOA, GPIO_Pin_9);
            }
            
            if(usRegInputBuf[2]<2200){
                GPIO_ResetBits(GPIOA, GPIO_Pin_10);
            }else{
                GPIO_SetBits(GPIOA, GPIO_Pin_10);
            }
        }
        
        vTaskDelayUntil( &xLastWakeTime, 100 / portTICK_RATE_MS );
    }
}

static void prvSetupHardware( void )
{
	/* Ensure that all 4 interrupt priority bits are used as the pre-emption
	priority. */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

	/* Set up the LED outputs. */
	osm_LEDInit( LED0 );
	
	/* Start with the LEDs off. */
	osm_LEDOn( LED0 );
    
	/* Setup ADC. */
    adc_init();
    
	/* Setup GPIO out. */
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Setup Mosfet Pins */
    /* Enable the GPIO Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* Configure the GPIO pin */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_9);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_10);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_11);
    
    /* Setup input Pins */
    /* Enable the GPIO Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    /* Configure the GPIO pin */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
}

static void mb_task( void *pvParameters )
{
    eMBErrorCode    eStatus;

    /* Select either ASCII or RTU Mode. */
    eStatus = eMBInit( MB_RTU, 0x02, NULL, 19200, NULL );
    assert( eStatus == MB_ENOERR );

    /* Enable the Modbus Protocol Stack. */
    eStatus = eMBEnable(  );

    for( ;; )
    {
        /* Call the main polling loop of the Modbus protocol stack. Internally
         * the polling loop waits for a new event by calling the port 
         * dependent function xMBPortEventGet(  ). In the FreeRTOS port the
         * event layer is built with queues.
         */
        ( void )eMBPoll(  );
    }
}

void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software 
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
volatile size_t xFreeStackSpace;

	/* This function is called on each cycle of the idle task.  In this case it
	does nothing useful, other than report the amout of FreeRTOS heap that 
	remains unallocated. */
	xFreeStackSpace = xPortGetFreeHeapSize();

	if( xFreeStackSpace > 100 )
	{
		/* By now, the kernel has allocated everything it is going to, so
		if there is a lot of heap remaining unallocated then
		the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
		reduced accordingly. */
	}
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
