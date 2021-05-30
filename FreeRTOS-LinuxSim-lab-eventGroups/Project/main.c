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
#include "event_groups.h"

/* Some definitions */
#define mainTIMER_TEST_PERIOD			( 50 )

/*-----------------------------------------------------------*/
EventGroupHandle_t xCreatedEventGroup;
EventGroupHandle_t xCreatedEventGroup2;	//new

# define ARRLEN(a) (sizeof(a) / sizeof (a)[0])
// TASKS x.1
const uint32_t task1_id = (1 << 0); // 0x01
const uint32_t task2_id = (1 << 1); // 0x02
const uint32_t task3_id = (1 << 2); // 0x03?

//const uint32_t all_tasks_id = (task1_id | task2_id | task3_id);
// TASKS x.2
const uint32_t task1_id_set = ((1 << 0) | (1 << 1));
const uint32_t task2_id_set = ((1 << 2) | (1 << 3));
const uint32_t task3_id_set = ((1 << 4) | (1 << 5));

static void task_1 ( void *pvParameters)
{
	( void ) pvParameters;
	while(1) {
		puts("Task one is executing...");
//		xEventGroupSetBits(xCreatedEventGroup, task1_id);
		xEventGroupSetBits(xCreatedEventGroup, task1_id_set);
		vTaskDelay(1000);
	}
}

static void task_2 ( void *pvParameters)
{
	( void ) pvParameters;
	while(1) {
		puts("Task two is executing...");
//		xEventGroupSetBits(xCreatedEventGroup, task2_id);
		xEventGroupSetBits(xCreatedEventGroup, task2_id_set);
		vTaskDelay(1000);
	}
}

static void task_3 ( void *pvParameters)
{
	( void ) pvParameters;
	while(1) {
		puts("Task three is executing...");
//		xEventGroupSetBits(xCreatedEventGroup, task3_id);
		xEventGroupSetBits(xCreatedEventGroup, task3_id_set);
		vTaskDelay(1000);
	}
}


static void sw_watchdog( void *pvParameters)
{
	( void ) pvParameters;
	while(1) {
//		uint32_t result = xEventGroupWaitBits(xCreatedEventGroup, task1_id | task2_id | task3_id,
//				pdTRUE,	//clear all bits on exit
//				pdTRUE,	// wait for all bits to be set
//				2000);	//timeout 2 sec for all bits to set

		uint32_t result = xEventGroupWaitBits(xCreatedEventGroup, task1_id_set | task2_id_set | task3_id_set,
						pdTRUE,	//clear all bits on exit
						pdTRUE,	// wait for all bits to be set
						2000);	//timeout 2 sec for all bits to set


		if ((result & (task1_id_set | task2_id_set | task3_id_set)) == (task1_id_set | task2_id_set | task3_id_set))
		{
			puts("System is healthy...");
		}
		else {
			if (!(result & task1_id_set))
			{
				puts("Task 1 stopped working");
			}
			if (!(result & task2_id_set))
			{
				puts("Task 2 stopped working");
			}
			if (!(result & task3_id_set))
			{
				puts("Task 3 stopped working");
			}
		}
	}
}

int main ( void )
{
	// GROUPS
	xCreatedEventGroup = xEventGroupCreate();
	xCreatedEventGroup2 = xEventGroupCreate();	//new

	// TASKS
	xTaskCreate(task_1, "task_1", 1000, NULL, 1, NULL);
	xTaskCreate(task_2, "task_2", 1000, NULL, 1, NULL);
	xTaskCreate(task_3, "task_3", 1000, NULL, 1, NULL);
	xTaskCreate(sw_watchdog, "sw_watchdog", 1000, NULL, 2, NULL);	// it has to have bigger pririty than others
	/* Start the scheduler itself. */
	vTaskStartScheduler();

	/* Should never get here unless there was not enough heap space to create
	the idle and other system tasks. */
	return 0;
}
/*-----------------------------------------------------------*/



