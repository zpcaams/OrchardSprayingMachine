/*****************************************************************************/
/**
* @file osm_usart.c
*
*
* @note
*
* None.
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "osm_usart.h"

/************************** Constant Definitions *****************************/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#define USARTx                       USART3
#define USART_CLK                    RCC_APB1Periph_USART3
#define USART_RX_PIN                 GPIO_Pin_9
#define USART_TX_PIN                 GPIO_Pin_8
#define USART_GPIO_PORT              GPIOD
#define USART_GPIO_CLK               RCC_AHB1Periph_GPIOD
#define USART_AF_PORT                GPIO_AF_USART3
#define USART_RX_SOURCE              GPIO_PinSource9
#define USART_TX_SOURCE              GPIO_PinSource8

#define USART_QUEUE_SIZE			256
#define USART_TASK_PRIORITY			( tskIDLE_PRIORITY + 1UL )
/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/
static xQueueHandle xUsartQueue;

void UsartTask( void *pvParameters ) 
{
	char ch;
	portBASE_TYPE xStatus;

	for(;;) {
		// As long as there are characters in the queue fifo this code should
		// pop them out and send them as quick as possible out the UART.
		if( USART_GetFlagStatus( USARTx, USART_FLAG_TXE ) ) {
			// We don't want to block forever - need to check on Rx too.
			xStatus = xQueueReceive( xUsartQueue, &ch, portMAX_DELAY );
			if( xStatus == pdPASS ) USART_SendData( USARTx, ch );
		}
	}
}

void USART_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* USART GPIOs configuration **************************************************/
    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(USART_GPIO_CLK, ENABLE);

    /* Connect UART pins to AF */
    GPIO_PinAFConfig(USART_GPIO_PORT, USART_TX_SOURCE, USART_AF_PORT);
    GPIO_PinAFConfig(USART_GPIO_PORT, USART_RX_SOURCE, USART_AF_PORT);

    /* Configure USART RX and TX pins */
    GPIO_InitStructure.GPIO_Pin = USART_TX_PIN | USART_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);

    /* USART configuration ********************************************************/
    /* Enable UART clock */
    RCC_APB1PeriphClockCmd(USART_CLK, ENABLE);

    /* USART register init */
    USART_DeInit(USARTx);
    USART_StructInit(&USART_InitStructure);

    /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
    */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    /* USART configuration init*/
    USART_Init(USARTx, &USART_InitStructure);

    /* Enable USART */
    USART_Cmd(USARTx, ENABLE);

    /* Creat Debug Output Queue. */
    xUsartQueue = xQueueCreate( USART_QUEUE_SIZE, sizeof( char ) );

    /* Creat Debug Output Task. */
    xTaskCreate(    UsartTask, ( signed char * ) "Usart", 
                    configMINIMAL_STACK_SIZE,   NULL, 
                    USART_TASK_PRIORITY,        NULL ); 
}

PUTCHAR_PROTOTYPE
{
    xQueueSendToBack( xUsartQueue, &(ch), 0 );
    return ch;
}