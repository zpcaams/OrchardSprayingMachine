/*
    FreeRTOS V7.5.2 - Copyright (C) 2013 Real Time Engineers Ltd.

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that has become a de facto standard.             *
     *                                                                       *
     *    Help yourself get started quickly and support the FreeRTOS         *
     *    project by purchasing a FreeRTOS tutorial book, reference          *
     *    manual, or both from: http://www.FreeRTOS.org/Documentation        *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>! NOTE: The modification to the GPL is included to allow you to distribute
    >>! a combined work that includes FreeRTOS without being obliged to provide
    >>! the source code for proprietary components outside of the FreeRTOS
    >>! kernel.

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available from the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?"                                     *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/*
 * This demo file demonstrates how to send data between an ISR and a 
 * co-routine.  A tick hook function is used to periodically pass data between
 * the RTOS tick and a set of 'hook' co-routines.
 *
 * hookNUM_HOOK_CO_ROUTINES co-routines are created.  Each co-routine blocks
 * to wait for a character to be received on a queue from the tick ISR, checks
 * to ensure the character received was that expected, then sends the number
 * back to the tick ISR on a different queue.
 * 
 * The tick ISR checks the numbers received back from the 'hook' co-routines 
 * matches the number previously sent.
 *
 * If at any time a queue function returns unexpectedly, or an incorrect value
 * is received either by the tick hook or a co-routine then an error is 
 * latched.
 *
 * This demo relies on each 'hook' co-routine to execute between each 
 * hookTICK_CALLS_BEFORE_POST tick interrupts.  This and the heavy use of 
 * queues from within an interrupt may result in an error being detected on 
 * slower targets simply due to timing.
 */

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "croutine.h"
#include "queue.h"

/* Demo application includes. */
#include "crhook.h"

/* The number of 'hook' co-routines that are to be created. */
#define hookNUM_HOOK_CO_ROUTINES        ( 4 )

/* The number of times the tick hook should be called before a character is 
posted to the 'hook' co-routines. */
#define hookTICK_CALLS_BEFORE_POST      ( 500 )

/* There should never be more than one item in any queue at any time. */
#define hookHOOK_QUEUE_LENGTH           ( 1 )

/* Don't block when initially posting to the queue. */
#define hookNO_BLOCK_TIME               ( 0 )

/* The priority relative to other co-routines (rather than tasks) that the
'hook' co-routines should take. */
#define mainHOOK_CR_PRIORITY			( 1 )
/*-----------------------------------------------------------*/

/*
 * The co-routine function itself.
 */
static void prvHookCoRoutine( xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex );


/*
 * The tick hook function.  This receives a number from each 'hook' co-routine
 * then sends a number to each co-routine.  An error is flagged if a send or 
 * receive fails, or an unexpected number is received.
 */
void vApplicationTickHook( void );

/*-----------------------------------------------------------*/

/* Queues used to send data FROM a co-routine TO the tick hook function.
The hook functions received (Rx's) on these queues.  One queue per
'hook' co-routine. */
static xQueueHandle xHookRxQueues[ hookNUM_HOOK_CO_ROUTINES ];

/* Queues used to send data FROM the tick hook TO a co-routine function.
The hood function transmits (Tx's) on these queues.  One queue per
'hook' co-routine. */
static xQueueHandle xHookTxQueues[ hookNUM_HOOK_CO_ROUTINES ];

/* Set to true if an error is detected at any time. */
static portBASE_TYPE xCoRoutineErrorDetected = pdFALSE;

/*-----------------------------------------------------------*/

void vStartHookCoRoutines( void )
{
unsigned portBASE_TYPE uxIndex, uxValueToPost = 0;

	for( uxIndex = 0; uxIndex < hookNUM_HOOK_CO_ROUTINES; uxIndex++ )
	{
		/* Create a queue to transmit to and receive from each 'hook' 
		co-routine. */
		xHookRxQueues[ uxIndex ] = xQueueCreate( hookHOOK_QUEUE_LENGTH, sizeof( unsigned portBASE_TYPE ) );
		xHookTxQueues[ uxIndex ] = xQueueCreate( hookHOOK_QUEUE_LENGTH, sizeof( unsigned portBASE_TYPE ) );

		/* To start things off the tick hook function expects the queue it 
		uses to receive data to contain a value.  */
		xQueueSend( xHookRxQueues[ uxIndex ], &uxValueToPost, hookNO_BLOCK_TIME );

		/* Create the 'hook' co-routine itself. */
		xCoRoutineCreate( prvHookCoRoutine, mainHOOK_CR_PRIORITY, uxIndex );
	}
}
/*-----------------------------------------------------------*/

static unsigned portBASE_TYPE uxCallCounter = 0, uxNumberToPost = 0;
void vApplicationTickHook( void )
{
unsigned portBASE_TYPE uxReceivedNumber;
signed portBASE_TYPE xIndex, xCoRoutineWoken;

	/* Is it time to talk to the 'hook' co-routines again? */
	uxCallCounter++;
	if( uxCallCounter >= hookTICK_CALLS_BEFORE_POST )
	{
		uxCallCounter = 0;

		for( xIndex = 0; xIndex < hookNUM_HOOK_CO_ROUTINES; xIndex++ )
		{
			xCoRoutineWoken = pdFALSE;
			if( crQUEUE_RECEIVE_FROM_ISR( xHookRxQueues[ xIndex ], &uxReceivedNumber, &xCoRoutineWoken ) != pdPASS )
			{
				/* There is no reason why we would not expect the queue to 
				contain a value. */
				xCoRoutineErrorDetected = pdTRUE;
			}
			else
			{
				/* Each queue used to receive data from the 'hook' co-routines 
				should contain the number we last posted to the same co-routine. */
				if( uxReceivedNumber != uxNumberToPost )
				{
					xCoRoutineErrorDetected = pdTRUE;
				}

				/* Nothing should be blocked waiting to post to the queue. */
				if( xCoRoutineWoken != pdFALSE )
				{
					xCoRoutineErrorDetected = pdTRUE;
				}
			}
		}

		/* Start the next cycle by posting the next number onto each Tx queue. */
		uxNumberToPost++;

		for( xIndex = 0; xIndex < hookNUM_HOOK_CO_ROUTINES; xIndex++ )
		{
			if( crQUEUE_SEND_FROM_ISR( xHookTxQueues[ xIndex ], &uxNumberToPost, pdFALSE ) != pdTRUE )
			{
				/* Posting to the queue should have woken the co-routine that 
				was blocked on the queue. */
				xCoRoutineErrorDetected = pdTRUE;
			}
		}
	}
}
/*-----------------------------------------------------------*/

static void prvHookCoRoutine( xCoRoutineHandle xHandle, unsigned portBASE_TYPE uxIndex )
{
static unsigned portBASE_TYPE uxReceivedValue[ hookNUM_HOOK_CO_ROUTINES ];
portBASE_TYPE xResult;

	/* Each co-routine MUST start with a call to crSTART(); */
	crSTART( xHandle );

	for( ;; )
	{
		/* Wait to receive a value from the tick hook. */
		xResult = pdFAIL;
		crQUEUE_RECEIVE( xHandle, xHookTxQueues[ uxIndex ], &( uxReceivedValue[ uxIndex ] ), portMAX_DELAY, &xResult );

		/* There is no reason why we should not have received something on
		the queue. */
		if( xResult != pdPASS )
		{
			xCoRoutineErrorDetected = pdTRUE;
		}

		/* Send the same number back to the idle hook so it can verify it. */
		xResult = pdFAIL;
		crQUEUE_SEND( xHandle, xHookRxQueues[ uxIndex ], &( uxReceivedValue[ uxIndex ] ), hookNO_BLOCK_TIME, &xResult );
		if( xResult != pdPASS )
		{
			/* There is no reason why we should not have been able to post to 
			the queue. */
			xCoRoutineErrorDetected = pdTRUE;
		}
	}

	/* Each co-routine MUST end with a call to crEND(). */
	crEND();
}
/*-----------------------------------------------------------*/

portBASE_TYPE xAreHookCoRoutinesStillRunning( void )
{
	if( xCoRoutineErrorDetected )
	{
		return pdFALSE;
	}
	else
	{
		return pdTRUE;
	}
}



