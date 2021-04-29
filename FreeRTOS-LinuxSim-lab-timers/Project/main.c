/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/*
 * main() creates all the demo application tasks, then starts the scheduler.
 * The web documentation provides more details of the standard demo application
 * tasks, which provide no particular functionality but do provide a good
 * example of how to use the FreeRTOS API.
 *
 * In addition to the standard demo tasks, the following tasks and tests are
 * defined and/or created within this file:
 *
 * "Check" task - This only executes every five seconds but has a high priority
 * to ensure it gets processor time.  Its main function is to check that all the
 * standard demo tasks are still operational.  While no errors have been
 * discovered the check task will print out "OK" and the current simulated tick
 * time.  If an error is discovered in the execution of a task then the check
 * task will print out an appropriate error message.
 *
 */


/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Some definitions */
#define mainTIMER_TEST_PERIOD			( 50 )

/*-----------------------EXCERCISE--------------------------------*/
/* .Run three timer driven tasks (Callback) with different period times. In one callback
calculate the time between successive calls of two other callbacks function. */

void vTimerCallback1(TimerHandle_t);
void vTimerCallback2(TimerHandle_t);
void vTimerCallback3(TimerHandle_t);


TimerHandle_t xTimer1;
TimerHandle_t xTimer2;
TimerHandle_t xTimer3;
BaseType_t xTimer1Started, xTimer2Started, xTimer3Started;
TickType_t succesfullCallbackTime1;
TickType_t succesfullCallbackTime2;


/*-----------------------------------------------------------*/


int main ( void )
{


	xTimer1 = xTimerCreate("Timer0", 1000/portTICK_PERIOD_MS * 2, pdTRUE, (void*) 0, vTimerCallback1);
	xTimer2 = xTimerCreate("Timer1", 1000/portTICK_PERIOD_MS * 3, pdTRUE, (void*) 0, vTimerCallback2);
	xTimer3 = xTimerCreate("Timer2", 1000/portTICK_PERIOD_MS * 4, pdTRUE, (void*) 0, vTimerCallback3);

	xTimer1Started = xTimerStart(xTimer1, 0);
	xTimer2Started = xTimerStart(xTimer2, 0);
	xTimer3Started = xTimerStart(xTimer3, 0);

	/* Start the scheduler itself. */
	vTaskStartScheduler();

	/* Should never get here unless there was not enough heap space to create
	the idle and other system tasks. */
	return 0;
}
/*-----------------------------------------------------------*/


void vTimerCallback1( TimerHandle_t  xTimer)
{
	const uint32_t ulMaxExpiryCountBeforeStopping = 10;
	uint32_t ulCount;
	configASSERT(xTimer);

	ulCount = (uint32_t) pvTimerGetTimerID(xTimer);
	ulCount++;

	if (ulCount >= ulMaxExpiryCountBeforeStopping)
	{
		xTimerStop( xTimer, 0);
	}
	else
	{
		vTimerSetTimerID(xTimer, (void*) ulCount);
	}
	printf("HELLO FROM TIMER 1 CALLBACK FUNC\r\n");
	succesfullCallbackTime1 = xTaskGetTickCount();
}

void vTimerCallback2( TimerHandle_t  xTimer)
{
	const uint32_t ulMaxExpiryCountBeforeStopping = 10;
	uint32_t ulCount;
	configASSERT(xTimer);

	ulCount = (uint32_t) pvTimerGetTimerID(xTimer);
	ulCount++;

	if (ulCount >= ulMaxExpiryCountBeforeStopping)
	{
		xTimerStop( xTimer, 0);
	}
	else
	{
		vTimerSetTimerID(xTimer, (void*) ulCount);
	}
	printf("HELLO FROM TIMER 2 CALLBACK FUNC\r\n");
	succesfullCallbackTime2 = xTaskGetTickCount();
}

void vTimerCallback3( TimerHandle_t  xTimer)
{
	const uint32_t ulMaxExpiryCountBeforeStopping = 10;
	uint32_t ulCount;
	configASSERT(xTimer);

	ulCount = (uint32_t) pvTimerGetTimerID(xTimer);
	ulCount++;

	if (ulCount >= ulMaxExpiryCountBeforeStopping)
	{
		xTimerStop( xTimer, 0);
	}
	else
	{
		vTimerSetTimerID(xTimer, (void*) ulCount);
	}
	printf("HELLO FROM TIMER 3 CALLBACK FUNC\r\n");

		TickType_t diff = abs(succesfullCallbackTime2 - succesfullCallbackTime1);
		printf("DIffrence in time between tasks: %d \r\n", diff);


}

