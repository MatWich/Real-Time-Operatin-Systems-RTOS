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
QueueHandle_t xQueueAdd = NULL;
QueueHandle_t xQueueSub = NULL;
QueueHandle_t xQueueMul = NULL;
QueueHandle_t xQueueDiv = NULL;
QueueHandle_t xQueueCli = NULL;
QueueHandle_t xQueueSer = NULL;

# define ARRLEN(a) (sizeof(a) / sizeof (a)[0])

// Structure for client input
typedef struct
{
	int a;
	int b;
	int choice;
}clientStruct;

// structure for server input
typedef struct
{
	int a;
	int b;
}serverStruct;



static void multiply(void *pvParameters)
{
	//const	TickType_t xDelay = 1000/portTICK_PERIOD_MS;
	(void) pvParameters;
	serverStruct receivedData;

	float calculatedValue = 0.0;
	for (;;)
	{
		if (xQueueReceive(xQueueMul, &receivedData, 100) == pdTRUE)
		{
			printf("MULTIPY a: %d, b: %d\n\r", receivedData.a, receivedData.b);
			calculatedValue = (float)(receivedData.a) * (float)receivedData.b;
			xQueueSend(xQueueSer, &calculatedValue, 10);
		}

	}

}

static void devision(void *pvParameters)
{
	//const	TickType_t xDelay = 1000/portTICK_PERIOD_MS;
	(void) pvParameters;
	serverStruct receivedData;

		float calculatedValue = 0.0;
		for (;;)
		{
			if (xQueueReceive(xQueueDiv, &receivedData, 100) == pdTRUE)
			{
				printf("DEVISION a: %d, b: %d\n\r", receivedData.a, receivedData.b);
				calculatedValue = (float)(receivedData.a) / (float)receivedData.b;
				xQueueSend(xQueueSer, &calculatedValue, 10);
			}

		}

}

static void addition(void *pvParameters)
{
	//const	TickType_t xDelay = 1000/portTICK_PERIOD_MS;
	(void) pvParameters;
	serverStruct receivedData;

		float calculatedValue = 0.0;
		for (;;)
		{
			if (xQueueReceive(xQueueAdd, &receivedData, 100) == pdTRUE)
			{
				printf("ADDITION a: %d, b: %d\n\r", receivedData.a, receivedData.b);
				calculatedValue = (float)(receivedData.a) + (float)receivedData.b;
				xQueueSend(xQueueSer, &calculatedValue, 10);
			}

		}

}

static void substraction(void *pvParameters)
{
	//const	TickType_t xDelay = 1000/portTICK_PERIOD_MS;
	(void) pvParameters;
	serverStruct receivedData;

		float calculatedValue = 0.0;
		for (;;)
		{
			if (xQueueReceive(xQueueSub, &receivedData, 100) == pdTRUE)
			{
				printf("SUBTRACTION a: %d, b: %d\n\r", receivedData.a, receivedData.b);
				calculatedValue = (float)(receivedData.a) - (float)receivedData.b;
				xQueueSend(xQueueSer, &calculatedValue, 10);
			}

		}

}

static void server(void *pvParameters)
{
	//const	TickType_t xDelay = 1000/portTICK_PERIOD_MS;
	(void) pvParameters;
	float retValue = 0.0;

	clientStruct dataToReceive;
	serverStruct dataToSend;

	for (;;)
	{
		if (xQueueReceive(xQueueCli, &dataToReceive, 0) == pdTRUE)
		{
			printf("RECEIVED DATA: %d, %d, %d \n\r", dataToReceive.a, dataToReceive.b, dataToReceive.choice);
			dataToSend.a = dataToReceive.a;
			dataToSend.b = dataToReceive.b;

			if (dataToReceive.choice == 1)
			{
				xQueueSend(xQueueAdd, &dataToSend, 0);
				xQueueReceive(xQueueSer, &retValue, 200);
			}
			else if (dataToReceive.choice == 2)
			{
				xQueueSend(xQueueSub, &dataToSend, 0);
				xQueueReceive(xQueueSer, &retValue, 200);
			}

			else if (dataToReceive.choice == 3)
			{
				xQueueSend(xQueueDiv, &dataToSend, 0);
				xQueueReceive(xQueueSer, &retValue, 200);
			}
			else if (dataToReceive.choice == 4)
			{
				xQueueSend(xQueueMul, &dataToSend, 0);
				xQueueReceive(xQueueSer, &retValue, 200);
			}
			else
			{
				printf("There is no such an option \n\r");
			}



			printf("Calculated Value: %f\n\r", retValue);
		}

	}
}

static void client(void *pvParameters)
{
	//const	TickType_t xDelay = 1000/portTICK_PERIOD_MS;
	(void) pvParameters;
	int a = 0;
	int b = 2;
	int choice = 1;
	clientStruct dataToSend;
	a = 2;
	b = 4;
	choice = 4;

	for (;;)
	{
		/* I don't know why I can't use getchar() but it work anyway */
//		printf("Insert first number: \n\r");
//
//		printf("Insert second number: \n\r");
//
//		printf("Select Operation: \n\r");
//		printf("1. Addition\n\r");
//		printf("2. Subtraction\n\r");
//		printf("3. Division\n\r");
//		printf("4. Multiplication\n\r");

		dataToSend.a = a;
		dataToSend.b = b;
		dataToSend.choice = choice;
		xQueueSend(xQueueCli, &dataToSend, 100);
	}
}


int main ( void )
{

	clientStruct dataToSend;
	serverStruct dataToSendFromSever;

	// Queues init
	xQueueAdd = xQueueCreate(1, sizeof(dataToSendFromSever));
	xQueueSub = xQueueCreate(1, sizeof(dataToSendFromSever));
	xQueueDiv = xQueueCreate(1, sizeof(dataToSendFromSever));
	xQueueMul = xQueueCreate(1, sizeof(dataToSendFromSever));

	xQueueCli = xQueueCreate(1, sizeof(dataToSend));
	xQueueSer = xQueueCreate(sizeof(float)*10, sizeof(float));

	// Tasks init
	xTaskCreate(server, "Server", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(addition, "Addition", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(substraction, "Subtraction", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(devision, "Division", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(multiply, "Multiplication", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(client, "Client", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);

	/* Start the scheduler itself. */
	vTaskStartScheduler();

	/* Should never get here unless there was not enough heap space to create
	the idle and other system tasks. */
	return 0;
}
/*-----------------------------------------------------------*/



