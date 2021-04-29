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
#include "semphr.h"

/* Some definitions */
#define mainTIMER_TEST_PERIOD			( 50 )

/*-----------------------------------------------------------*/
=
/*-----------------------------------------------------------*/
Socket_t xClient;
SemaphoreHandle_t xSemaphore = NULL;
# define ARRAY_SIZE 8
int global_variable = 0;
# define ARRLEN(a) (sizeof(a) / sizeof (a)[0])
int global_array[] = {0, 0, 0, 0, 0, 0 ,0, 0, 0, 0 };
int is_identical(int arr[], int arr1[], int size);

static void xStartTask_1(void *pvParameters ) //customer
{
	int array_size = ARRLEN(global_array);
	const	TickType_t xDelay = 100/portTICK_PERIOD_MS;
	const	TickType_t xDelays = 1/portTICK_PERIOD_MS;
	(void) pvParameters;
	int test_array[array_size];

	for( ;; ) {
		for ( int i = 0; i < array_size; i++)
			if (xSemaphore != NULL)
			{
				if (global_variable == i)
					vTaskDelay(xDelays);

				if (xSemaphoreTake(xSemaphore, (TickType_t) 1) == pdTRUE)
				{

					if (global_array[i] > 0 )
					{

						global_array[i]--;
						//printf("Decreasing by 1 the index no.  global_array[%d] = %d \r\n", i, global_array[i]);
						xSemaphoreGive(xSemaphore);
					}
					else
					{
						xSemaphoreGive(xSemaphore);
					}
					if (is_identical(global_array, test_array, array_size) == 0) {
					printf("Customer Loop");
					for (int j = 0; j < array_size; j++)
						printf("%d", global_array[j]);
					printf("\r\n");
					for (int j = 0; j < array_size; j++)
						test_array[j] = global_array[j];
					}
					vTaskDelay(xDelay);
				}
			}
			else{
			}
	}
}

static void xStartTask_2( void *pvParameters )
{
	int array_size = ARRLEN(global_array);
	const	TickType_t xDelay = 1000/portTICK_PERIOD_MS;
	(void) pvParameters;
	int test_array[array_size];
	for (int j = 0; j < array_size; j++)
		test_array[j] = global_array[j];

	for( ;; ) {
		for ( int i = 0; i < array_size; i++)
		if (xSemaphore != NULL)
		{
			if (xSemaphoreTake(xSemaphore, (TickType_t) 1) == pdTRUE)
			{
				if (global_array[i] == 0) {
					//vTaskDelay(xDelay);
					global_array[i]++;
					global_variable = i;
					//printf("Increasing by 1 the index no.  global_array[%d] = %d \r\n", i, global_array[i]);
					xSemaphoreGive(xSemaphore);
				}
				else{
					xSemaphoreGive(xSemaphore);
				}
				if (is_identical(global_array, test_array, array_size) == 0)
				{
					printf("Producer Loop");
					for (int j = 0; j < array_size; j++)
						printf("%d", global_array[j]);
					printf("\r\n");
					for (int j = 0; j < array_size; j++)
						test_array[j] = global_array[j];

				}
				vTaskDelay(xDelay);
			}
		}
		else{
		}
	}
}

int is_identical(int arr[], int arr1[], int size) {

	for (int i = 0; i < size; i++)
		if (arr1[i] != arr[i])
			return 0;
	return 1;


}

int main ( void )
{

	xSemaphore = xSemaphoreCreateMutex();

	xTaskCreate( xStartTask_1, "Task 1", configMINIMAL_STACK_SIZE, NULL, 0, NULL );
	xTaskCreate( xStartTask_2, "Task 2", configMINIMAL_STACK_SIZE, NULL, 0, NULL );

	/* Start the scheduler itself. */
	vTaskStartScheduler();

	/* Should never get here unless there was not enough heap space to create
	the idle and other system tasks. */
	return 0;
}
/*-----------------------------------------------------------*/



/*
// EX 4

xTimer1 = xTimerCreate("Timer0", 1000/portTICK_PERIOD_MS * 2, pdTRUE, (void*) 0, vTimerCallback1);
xTimer2 = xTimerCreate("Timer1", 1000/portTICK_PERIOD_MS * 3, pdTRUE, (void*) 0, vTimerCallback2);
xTimer3 = xTimerCreate("Timer2", 1000/portTICK_PERIOD_MS * 4, pdTRUE, (void*) 0, vTimerCallback3);

xTimer1Started = xTimerStart(xTimer1, 0);
xTimer2Started = xTimerStart(xTimer2, 0);
xTimer3Started = xTimerStart(xTimer3, 0);
*/


//xQueue = xQueueCreate(1, sizeof(int));

//xTaskCreate( xStartTask_1, "Task 1", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 2), &xFirstTaskHandle );
//xTaskCreate( xStartTask_2, "Task 2", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 3), &xPriorityTaskHandle );

//	// DIFFRENT PRIORITY
//	xTaskCreate( xStartTask_1, "Task 1", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1), NULL );
//	xTaskCreate( xStartTask_2, "Task 2", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 2), NULL );


//	xTaskCreate( xStartTask_1, "Task 1", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 2), NULL );
//	xTaskCreate( xStartTask_2, "Task 2", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 2), NULL );


/*
// EX 4

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
*/

