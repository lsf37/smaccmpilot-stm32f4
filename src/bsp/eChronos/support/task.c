/*
 * task.c
 *
 *  Created on: Nov 18, 2013
 *      Author: jxie @ NICTA
 */

#include <stdlib.h>
#include "rtos-kochab.h"
#include "stm32f4xx.h"
#include "debug.h"
#include "mutex.h"
#include "task.h"
#include "port.h"
#include "config.h"


extern void *entry_fn[];

extern void UNIMPLEMENTED(void);
#define vNOP() UNIMPLEMENTED()
#define NOP(type) do { UNIMPLEMENTED(); return (type)0; } while(0)


void * eChronosCreateTask(void * pxTaskCode)
{
    uint32_t tskId;

    for (tskId = RTOS_TASK_ID_ZERO; tskId <= RTOS_TASK_ID_MAX; tskId++) {
        if ((void *)entry_fn[tskId] == pxTaskCode) {
            return (void *)tskId;
        }
    }

    return NULL;
}

void eChronosStartRTOS(void)
{
    rtos_start();
}

void * eChronosGetCurrentTaskHandler(void)
{
    uint32_t tskId = rtos_task_current();

    return (void *)tskId;
}

unsigned long eChronosGetSysTick(void)
{
    return rtos_timer_current_ticks;
}



static void * BlockOnDelay_sem;
static int first_time_call_on_delay = 1;

void eChronosTaskDelayUntil(unsigned long * const pxPreviousWakeTime, unsigned long xTimeIncrement)
{
	unsigned long cur_time = rtos_timer_current_ticks;
	unsigned long wake_time = *pxPreviousWakeTime + xTimeIncrement;

	if(first_time_call_on_delay || BlockOnDelay_sem == NULL){
		BlockOnDelay_sem = eChronosCreateSemaphoreCounting(16, 0);
		first_time_call_on_delay = 0;
	}

	//should be block from here
	while(wake_time > cur_time){
		//error, wait again
		eChronosMutexTake(BlockOnDelay_sem, wake_time - cur_time);
		cur_time = rtos_timer_current_ticks;
	}
}

/* XXX: Is there a better place to put this? */
bool
echronos_systick(void)
{
    rtos_timer_tick();

    return true;
}
