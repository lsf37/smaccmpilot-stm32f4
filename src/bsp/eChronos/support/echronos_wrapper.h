/*
 * echronos_wrapper.h
 *	Included by the freertos wrapper internally
 *  Created on: Nov 18, 2013
 *      Author: jxie @ NICTA
 */

#ifndef ECHRONOS_WRAPPER_H_
#define ECHRONOS_WRAPPER_H_

#include <assert.h>

//#define pdTRUE          1
//#define pdFALSE         0

//eChronos specific

//#define taskYIELD()		rtos_yield()

#define UNIMPLEMENTED() assert(!"UNIMPLEMENETED");
#define vNOP() UNIMPLEMENTED()
#define NOP(type) do { UNIMPLEMENTED(); return (type)0; } while(0)




#endif /* ECHRONOS_WRAPPER_H_ */
