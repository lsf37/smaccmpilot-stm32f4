/*
 * echronos_wrapper.c
 *
 *  Created on: Nov 18, 2013
 *      Author: jxie @ NICTA
 */

#include "eChronos.h"
#include "rtos.h"
#include "task.h"
#include "mutex.h"
#include "queue.h"
#include "port.h"
#include "echronos_wrapper.h"
#include <assert.h>

// FIXME: This has to go.
void vTaskStartScheduler(void)
{
	xPortStart();
    eChronosStartRTOS();
}

/* SEM/MUTEX APIs */
xSemaphoreHandle xSemaphoreCreateCounting( unsigned portBASE_TYPE uxCountValue, unsigned portBASE_TYPE uxInitialCount)
{
	return eChronosCreateSemaphoreCounting(uxCountValue, uxInitialCount);
}

xSemaphoreHandle xSemaphoreCreateMutex(void)
{
	 return  eChronosCreateMutex();
}

xSemaphoreHandle xSemaphoreCreateRecursiveMutex(void)
{
	 return eChronosCreateRecursiveMutex();
}

signed portBASE_TYPE  xSemaphoreTake(xSemaphoreHandle xMuxId, portTickType xBlockTime )
{
	return eChronosMutexTake(xMuxId, xBlockTime);
}

signed portBASE_TYPE xSemaphoreGive(xSemaphoreHandle xMuxId)
{
	return eChronosMutexGive(xMuxId);
}

portBASE_TYPE xSemaphoreTakeRecursive(xSemaphoreHandle xMuxId,portTickType xBlockTime )
{
	return eChronosMutexTakeRecursive(xMuxId, xBlockTime);
}

portBASE_TYPE xSemaphoreGiveRecursive(xSemaphoreHandle xMuxId )
{
	return eChronosMutexGiveRecursive(xMuxId);
}


signed portBASE_TYPE  xSemaphoreGiveFromISR(xSemaphoreHandle xMuxId,signed portBASE_TYPE * pxHigherPriorityTaskWoken ){

	/* FIXME: This (calling a sempahore function from an ISR) isn't actually 
	   allowed in eChronos. Ideally this should signal a separate task that 
	   is responsible for dealing with the semaphore. */

	/* If the following is set to true, our caller should context switch 
	   (yield) before leaving the ISR, if false, the caller doesn't do anything 
	   special. However, since in eChronos an ISR can't yield, and we 
	   implement yield from ISR as a noop, it doesn't matter what we set 
	   this to: the caller will do nothing special.  So arbirarily set it 
	   to false. */ 
	*pxHigherPriorityTaskWoken = pdFALSE;

	return xSemaphoreGive(xMuxId);
}

void* xSemaphoreGetMutexHolder( xSemaphoreHandle xMuxId)
{
	return eChronosGetMutexHolder(xMuxId);
}

void vSemaphoreDelete( xSemaphoreHandle xSemaphore)
{
	UNIMPLEMENTED();
}



signed portBASE_TYPE xTaskGenericCreate( pdTASK_CODE pxTaskCode, const signed char * const pcName,
											unsigned short usStackDepth, void *pvParameters,
											unsigned portBASE_TYPE uxPriority,
											xTaskHandle *pxCreatedTask, portSTACK_TYPE *puxStackBuffer,
											const xMemoryRegion * const xRegions )
{
	/* if we can't return a task id, then there's no point looking up the task */
	if(pxCreatedTask != NULL){
		*pxCreatedTask = eChronosCreateTask(pxTaskCode);
		if(*pxCreatedTask){
			 return pdPASS;
		}else{
			return errNO_TASK_TO_RUN;
		}
	}
    return pdPASS;
}

// FIXME: freertos implements these as a call to xTaskCreateGeneric...
signed portBASE_TYPE xTaskCreate( pdTASK_CODE pxTaskCode, const signed char * pcName,
									unsigned short usStackDepth, void *pvParameters,
									unsigned portBASE_TYPE uxPriority, xTaskHandle *pxCreatedTask){
	if(pxCreatedTask != NULL){
		*pxCreatedTask = eChronosCreateTask(pxTaskCode);
		if(*pxCreatedTask){
			 return pdPASS;
		}else{
			return errNO_TASK_TO_RUN;
		}
	}
    return pdPASS;
}

xTaskHandle xTaskGetCurrentTaskHandle( void )
{
	return eChronosGetCurrentTaskHandle();
}

void vTaskSetApplicationTaskTag( xTaskHandle xTask, pdTASK_HOOK_CODE pxHookFunction )
{
    vNOP();
}



portTickType xTaskGetTickCount(void)
{
	return eChronosGetSysTick();
}

void vTaskDelay( portTickType xMsToDelay )
{
	unsigned long curTime = eChronosGetSysTick()
	eChronosTaskDelayUntil(&curTime, xMsToDelay);

}

void vTaskDelayUntil( portTickType * const pxPreviousWakeTime, portTickType xTimeIncrement )
{
	eChronosTaskDelayUntil(pxPreviousWakeTime, xTimeIncrement);
}

void taskYIELD(void)
{
	eChronos_yield();
}

xQueueHandle xQueueCreate( unsigned portBASE_TYPE uxQueueLength, unsigned portBASE_TYPE uxItemSize, unsigned char ucQueueType)
{
	UNIMPLEMENTED();
	retrun 0;
}

signed portBASE_TYPE xQueueSend( xQueueHandle pxQueue, const void * const pvItemToQueue, portTickType xTicksToWait, portBASE_TYPE xCopyPosition)
{
	UNIMPLEMENTED();
	return errQUEUE_FULL;
}

signed portBASE_TYPE xQueueReceive( xQueueHandle pxQueue, void * const pvBuffer, portTickType xTicksToWait, portBASE_TYPE xJustPeeking )
{
	UNIMPLEMENTED();
	return pdFALSE;
}

signed portBASE_TYPE xQueueSendFromISR( xQueueHandle pxQueue, const void * const pvItemToQueue, signed portBASE_TYPE *pxHigherPriorityTaskWoken, portBASE_TYPE xCopyPosition )
{
	UNIMPLEMENTED();
	return errQUEUE_FULL;
}

signed portBASE_TYPE xQueueReceiveFromISR( xQueueHandle pxQueue, void * const pvBuffer, signed portBASE_TYPE *pxHigherPriorityTaskWoken)
{
	UNIMPLEMENTED();
	return pdFALSE;
}

unsigned portBASE_TYPE uxQueueMessagesWaiting( const xQueueHandle pxQueue )
{
	UNIMPLEMENTED();
	return 0;
}



